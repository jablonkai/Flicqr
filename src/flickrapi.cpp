#include "flickrapi.h"

#include <liboauthcpp/liboauthcpp.h>
#include <QtCore/QEventLoop>
#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QUrl>
#include <QtGui/QDesktopServices>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QImage>
#include <QtGui/QProgressDialog>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtWebKitWidgets/QWebFrame>
#include <QtWebKitWidgets/QWebPage>

#include "photosetsmodel.h"
#include "settings.h"
#include "ui_mainwindow.h"

FlickrAPI::FlickrAPI(QObject *parent, Ui::MainWindow *_ui) :
    QObject(parent), ui(_ui), pDialog(NULL)
{
    netAccessManager = new QNetworkAccessManager(this);
    photosets = new PhotosetsModel;
    flickrRestUrl = QString("https://api.flickr.com/services/rest/?format=json&nojsoncallback=1&");

    connect(netAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNetworkReply(QNetworkReply*)));

    if (!Settings::instance()->oauthTokenKey().isNull() && !Settings::instance()->oauthTokenSecret().isNull())
    {
        client = new OAuth::Client(new OAuth::Consumer(API_KEY, API_SECRET), new OAuth::Token(Settings::instance()->oauthTokenKey().toStdString(), Settings::instance()->oauthTokenSecret().toStdString()));
        sendRequest(flickrRestUrl + QString(client->getURLQueryString(OAuth::Http::Get, flickrRestUrl.toStdString() + "method=flickr.test.login").c_str()));

        getPhotosetsList();
    }
}

FlickrAPI::~FlickrAPI()
{
    delete netAccessManager;
    delete pDialog;
    delete client;
    delete photosets;
}

void FlickrAPI::authenticate()
{
    netAccessManager->clearAccessCache();

    OAuth::Consumer *consumer = new OAuth::Consumer(API_KEY, API_SECRET);
    OAuth::Client oauth(consumer);
    QEventLoop loop;
    QWebPage webPage;

    connect(&webPage, SIGNAL(loadFinished(bool)), &loop, SLOT(quit()));

    // 1
    std::string oAuthQueryString = oauth.getURLQueryString(OAuth::Http::Get, "https://www.flickr.com/services/oauth/request_token?oauth_callback=oob");
    webPage.mainFrame()->load(QUrl(QString("https://www.flickr.com/services/oauth/request_token?") + QString(oAuthQueryString.c_str())));
    loop.exec();

    QString requestTokenResp = webPage.mainFrame()->toPlainText();
    OAuth::Token request_token = OAuth::Token::extract(requestTokenResp.toStdString());

    // 2
    QDesktopServices::openUrl(QUrl(QString("https://www.flickr.com/services/oauth/authorize?oauth_token=%1&perms=read").arg(request_token.key().c_str())));
    bool ok;
    QString pin = QInputDialog::getText(0, tr("PIN"), tr("PIN:"), QLineEdit::Normal, "", &ok, 0, Qt::ImhNone);
    if (!ok)
        return;

    request_token.setPin(pin.toStdString());

    // 3
    oauth = OAuth::Client(consumer, &request_token);
    oAuthQueryString = oauth.getURLQueryString(OAuth::Http::Get, "https://www.flickr.com/services/oauth/access_token", std::string(""), true);
    webPage.mainFrame()->load(QUrl(QString("https://www.flickr.com/services/oauth/access_token?") + QString(oAuthQueryString.c_str())));
    loop.exec();

    QString accessTokenResp = webPage.mainFrame()->toPlainText();
    OAuth::KeyValuePairs accessTokenRespData = OAuth::ParseKeyValuePairs(accessTokenResp.toStdString());
    OAuth::Token *accessToken = new OAuth::Token(OAuth::Token::extract(accessTokenRespData));

    Settings::instance()->setOauthTokenKey(accessToken->key().c_str());
    Settings::instance()->setOauthTokenSecret(accessToken->secret().c_str());

    if (client)
        delete client;
    client = new OAuth::Client(consumer, accessToken);

    oAuthQueryString = client->getURLQueryString(OAuth::Http::Get, flickrRestUrl.toStdString() + "method=flickr.test.login");

    QUrl requestUrl(flickrRestUrl + QString(oAuthQueryString.c_str()));
    QNetworkRequest netRequest(requestUrl);
    netAccessManager->get(netRequest);

    getPhotosetsList();
}

void FlickrAPI::getPhotosetsList()
{
    pDialog = new QProgressDialog(tr("downloading album list..."), tr("Cancel"), 1, 100);
    pDialog->setWindowModality(Qt::ApplicationModal);
    pDialog->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    pDialog->setCancelButton(0);
    pDialog->show();

    photosets->clear();
    sendRequest(flickrRestUrl + QString(client->getURLQueryString(OAuth::Http::Get, flickrRestUrl.toStdString() + "method=flickr.photosets.getList").c_str()));
}

void FlickrAPI::downloadPhotoset(const QString &directory)
{
    netAccessManager->clearAccessCache();

    if (photosets->activeSet() == NULL)
    {
        QMessageBox::warning(0, tr("Warning"), tr("Please select an album first!"));
        return;
    }

    photosets->activeSet()->setFolder(directory);
    QList<Photo*> photos = photosets->activeSet()->photoList();

    pDialog = new QProgressDialog(tr("photos downloading..."), tr("Cancel"), 1, photos.size());
    connect(pDialog, SIGNAL(canceled()), this, SLOT(dowloadCanceled()));
    pDialog->setWindowModality(Qt::ApplicationModal);
    pDialog->show();

    albumDownloading = true;
    foreach (Photo *photo, photos)
        sendRequest(flickrRestUrl + QString(client->getURLQueryString(OAuth::Http::Get, flickrRestUrl.toStdString() + "method=flickr.photos.getSizes&photo_id=" + photo->ID().toStdString()).c_str()));
}

void FlickrAPI::activated(const QModelIndex &index)
{
    netAccessManager->clearAccessCache();

    photosets->activateSet(index);
    photosets->activeSet()->clear();
    sendRequest(flickrRestUrl + QString(client->getURLQueryString(OAuth::Http::Get, flickrRestUrl.toStdString() + "method=flickr.photosets.getPhotos&photoset_id=" + photosets->activeSet()->ID().toStdString()).c_str()));
}

void FlickrAPI::photoActivated(const QModelIndex &index)
{
    Photo *photo = photosets->activeSet()->selected(index);

    ui->titleLabel->setText(photo->title());
    ui->descriptionTextBrowser->setText(photo->description());
    ui->takenLabel->setText(QString("Taken: %1").arg(photo->dateTaken().toString()));
    ui->uploadedLabel->setText(QString("Uploaded: %1").arg(photo->dateUploaded().toString()));
    ui->updateLabel->setText(QString("Updated: %1").arg(QDateTime::fromTime_t(0) == photo->dateUpdate() ? tr("never") : photo->dateUpdate().toString()));
    ui->publicCheckBox->setChecked(photo->isPublic());
    ui->friendsCheckBox->setChecked(photo->isFriend());
    ui->familyCheckBox->setChecked(photo->isFamily());

    albumDownloading = false;
    sendRequest(flickrRestUrl + QString(client->getURLQueryString(OAuth::Http::Get, flickrRestUrl.toStdString() + "method=flickr.photos.getSizes&photo_id=" + photo->ID().toStdString()).c_str()));
}

void FlickrAPI::sendRequest(const QString &request)
{
    QUrl requestUrl(request);
    QNetworkRequest netRequest(requestUrl);
    netAccessManager->get(netRequest);
}

void FlickrAPI::parseNetworkReply(QNetworkReply *reply)
{
    QString reqUrl = reply->url().toString();

    if (reqUrl.contains("staticflickr") && albumDownloading)
    {
        QString fName = reply->url().toString().split("/").last().split("_").first();
        fName = photosets->activeSet()->titleByID(fName);
        fName.append("." + reply->url().toString().split(".").last());

        saveToDisk(fName, reply);
        pDialog->setValue(pDialog->value() + 1);

        reply->deleteLater();
        return;
    }
    else if (reqUrl.contains("staticflickr") && !albumDownloading)
    {
        QImage image;
        image.load(reply, "JPG");
        ui->pictureLabel->setPixmap(QPixmap::fromImage(image));
        ui->pictureLabel->repaint();

        reply->deleteLater();
        return;
    }

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonObject = jsonDocument.object();
    QJsonObject jsonData;

    if (reqUrl.contains("method=flickr.test.login"))
    {
        jsonData = jsonObject.value("user").toObject();
        Settings::instance()->setUserID(jsonData.value("id").toString());
        Settings::instance()->setUserName(jsonData.value("username").toObject().value("_content").toString());
    }
    else if (reqUrl.contains("method=flickr.photos.getInfo"))
    {
        jsonData = jsonObject.value("photo").toObject();
        Photo *photo = photosets->activeSet()->byID(jsonData.value("id").toString());

        if (photo != NULL)
        {
            photo->setFormat(jsonData.value("originalformat").toString());
            photo->setDescription(jsonData.value("description").toObject().value("_content").toString());
            photo->setDateTaken(QDateTime::fromString(jsonData.value("dates").toObject().value("taken").toString(), Qt::ISODate));
            photo->setDateUploaded(QDateTime::fromTime_t(jsonData.value("dateuploaded").toString().toLongLong()));
            photo->setDateUpdate(QDateTime::fromTime_t(jsonData.value("visibility").toObject().value("lastupdate").toString().toLongLong()));
            photo->setPublic(jsonData.value("visibility").toObject().value("ispublic").toInt());
            photo->setFriend(jsonData.value("visibility").toObject().value("isfriend").toInt());
            photo->setFamily(jsonData.value("visibility").toObject().value("isfamily").toInt());
        }
    }
    else if (reqUrl.contains("method=flickr.photos.getSizes"))
    {
        jsonData = jsonObject.value("sizes").toObject();
        QJsonArray jsonArray = jsonData.value("size").toArray();

        if (albumDownloading)
        {
            for (int i = 0; i < jsonArray.size(); ++i)
                if (jsonArray[i].toObject().value("label").toString() == "Original")
                {
                    QString source = jsonArray[i].toObject().value("source").toString();
                    sendRequest(source);

                    emit sendResponse(QString::fromUtf8(jsonDocument.toJson()));
                    reply->deleteLater();
                    return;
                }
        }
        else
        {
            for (int i = 0; i < jsonArray.size(); ++i)
                if (jsonArray[i].toObject().value("label").toString() == "Large Square")
                {
                    QString source = jsonArray[i].toObject().value("source").toString();
                    sendRequest(source);

                    emit sendResponse(QString::fromUtf8(jsonDocument.toJson()));
                    reply->deleteLater();
                    return;
                }
        }
    }
    else if (reqUrl.contains("method=flickr.photosets.getInfo"))
    {
        jsonData = jsonObject.value("photoset").toObject();
        Photoset *photoSet = photosets->byID(jsonData.value("id").toString());

        if (jsonData.value("count_photos").type() == QJsonValue::Double)
            photoSet->setPhotoCount(jsonData.value("count_photos").toInt());
        else if (jsonData.value("count_photos").type() == QJsonValue::String)
            photoSet->setPhotoCount(jsonData.value("count_photos").toString().toInt());

        if (jsonData.value("count_videos").type() == QJsonValue::Double)
            photoSet->setVideoCount(jsonData.value("count_videos").toInt());
        else if (jsonData.value("count_videos").type() == QJsonValue::String)
            photoSet->setVideoCount(jsonData.value("count_videos").toString().toInt());

        photoSet->setDescription(jsonData.value("description").toObject().value("_content").toString());
        photoSet->setCreatedDate(QDateTime::fromTime_t(jsonData.value("date_create").toString().toLongLong()));
        photoSet->setUpdatedDate(QDateTime::fromTime_t(jsonData.value("date_update").toString().toLongLong()));

        pDialog->setValue(pDialog->value() + 1);
    }
    else if (reqUrl.contains("method=flickr.photosets.getList"))
    {
        jsonData = jsonObject.value("photosets").toObject();
        QJsonArray jsonArray = jsonData.value("photoset").toArray();
        pDialog->setMaximum(jsonArray.size());

        for (int i = 0; i < jsonArray.size(); ++i)
        {
            QJsonObject iObject = jsonArray[i].toObject();
            photosets->addPhotoset(new Photoset(iObject["id"].toString(), iObject["title"].toObject().value("_content").toString()));
            sendRequest(flickrRestUrl + QString(client->getURLQueryString(OAuth::Http::Get, flickrRestUrl.toStdString() + "method=flickr.photosets.getInfo&photoset_id=" + iObject["id"].toString().toStdString()).c_str()));
        }

        int page;
        int pages;

        if (jsonData.value("page").type() == QJsonValue::Double)
            page = jsonData.value("page").toInt();
        else if (jsonData.value("page").type() == QJsonValue::String)
            page = jsonData.value("page").toString().toInt();

        if (jsonData.value("pages").type() == QJsonValue::Double)
            pages = jsonData.value("pages").toInt();
        else if (jsonData.value("pages").type() == QJsonValue::String)
            pages = jsonData.value("pages").toString().toInt();

        if (pages != page)
            sendRequest(flickrRestUrl + QString(client->getURLQueryString(OAuth::Http::Get, flickrRestUrl.toStdString() + "method=flickr.photosets.getList" + "&page=" + QString("%1").arg(page + 1).toStdString()).c_str()));
    }
    else if (reqUrl.contains("method=flickr.photosets.getPhotos"))
    {
        jsonData = jsonObject.value("photoset").toObject();
        Photoset *photoSet = photosets->byID(jsonData.value("id").toString());
        QJsonArray jsonArray = jsonData.value("photo").toArray();

        for (int i = 0; i < jsonArray.size(); ++i)
        {
            QJsonObject iObject = jsonArray[i].toObject();
            photoSet->addPhoto(new Photo(iObject["id"].toString(), iObject["title"].toString()));
            sendRequest(flickrRestUrl + QString(client->getURLQueryString(OAuth::Http::Get, flickrRestUrl.toStdString() + "method=flickr.photos.getInfo&photo_id=" + iObject["id"].toString().toStdString()).c_str()));
        }

        int page;
        int pages;

        if (jsonData.value("page").type() == QJsonValue::Double)
            page = jsonData.value("page").toInt();
        else if (jsonData.value("page").type() == QJsonValue::String)
            page = jsonData.value("page").toString().toInt();

        if (jsonData.value("pages").type() == QJsonValue::Double)
            pages = jsonData.value("pages").toInt();
        else if (jsonData.value("pages").type() == QJsonValue::String)
            pages = jsonData.value("pages").toString().toInt();

        if (pages != page)
            sendRequest(flickrRestUrl + QString(client->getURLQueryString(OAuth::Http::Get, flickrRestUrl.toStdString() + "method=flickr.photosets.getPhotos&photoset_id=" + photosets->activeSet()->ID().toStdString() + "&page=" + QString("%1").arg(page + 1).toStdString()).c_str()));

        emit photoSetActivated(photosets->activeSet());
    }

    reply->deleteLater();
    emit sendResponse(QString::fromUtf8(jsonDocument.toJson()));
}

void FlickrAPI::dowloadCanceled()
{
    netAccessManager->clearAccessCache();
}

void FlickrAPI::saveToDisk(QString fName, QIODevice *data)
{
    QFile file(photosets->activeSet()->folder() + "/" + fName);
    file.open(QIODevice::WriteOnly);
    file.write(data->readAll());
    file.close();
}
