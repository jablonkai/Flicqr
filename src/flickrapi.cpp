#include "flickrapi.h"

#include <liboauthcpp/liboauthcpp.h>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QUrl>
#include <QtGui/QDesktopServices>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtWidgets/QInputDialog>

#include "photosetsmodel.h"
#include "settings.h"

#include <iostream>

FlickrAPI::FlickrAPI(QObject *parent) :
    QObject(parent)
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
    delete client;
    delete photosets;
}

void FlickrAPI::authenticate()
{
    if (client)
        delete client;

    OAuth::Consumer *consumer = new OAuth::Consumer(API_KEY, API_SECRET);
    OAuth::Client oauth(consumer);

    // 1
    std::string oAuthQueryString = oauth.getURLQueryString(OAuth::Http::Get, "https://www.flickr.com/services/oauth/request_token?oauth_callback=oob");
    QDesktopServices::openUrl(QUrl(QString("https://www.flickr.com/services/oauth/request_token?") + QString(oAuthQueryString.c_str())));
    QString requestTokenResp = QInputDialog::getText(0, tr("Request Token Response"), tr("Response:"), QLineEdit::Normal, "", 0, 0, Qt::ImhNone);
    OAuth::Token request_token = OAuth::Token::extract(requestTokenResp.toStdString());

    // 2
    QDesktopServices::openUrl(QUrl(QString("https://www.flickr.com/services/oauth/authorize?oauth_token=%1&perms=read").arg(request_token.key().c_str())));
    QString pin = QInputDialog::getText(0, tr("PIN"), tr("PIN:"), QLineEdit::Normal, "", 0, 0, Qt::ImhNone);
    request_token.setPin(pin.toStdString());

    // 3
    oauth = OAuth::Client(consumer, &request_token);
    oAuthQueryString = oauth.getURLQueryString(OAuth::Http::Get, "https://www.flickr.com/services/oauth/access_token", std::string(""), true);
    QDesktopServices::openUrl(QUrl(QString("https://www.flickr.com/services/oauth/access_token?") + QString(oAuthQueryString.c_str())));
    QString accessTokenResp = QInputDialog::getText(0, tr("Access Token Response"), tr("Response:"), QLineEdit::Normal, "", 0, 0, Qt::ImhNone);
    OAuth::KeyValuePairs accessTokenRespData = OAuth::ParseKeyValuePairs(accessTokenResp.toStdString());
    OAuth::Token *accessToken = new OAuth::Token(OAuth::Token::extract(accessTokenRespData));

    Settings::instance()->setOauthTokenKey(accessToken->key().c_str());
    Settings::instance()->setOauthTokenSecret(accessToken->secret().c_str());

    client = new OAuth::Client(consumer, accessToken);

    oAuthQueryString = client->getURLQueryString(OAuth::Http::Get, flickrRestUrl.toStdString() + "method=flickr.test.login");

    QUrl requestUrl(flickrRestUrl + QString(oAuthQueryString.c_str()));
    QNetworkRequest netRequest(requestUrl);
    netAccessManager->get(netRequest);

    getPhotosetsList();
}

void FlickrAPI::getPhotosetsList()
{
    photosets->clear();
    sendRequest(flickrRestUrl + QString(client->getURLQueryString(OAuth::Http::Get, flickrRestUrl.toStdString() + "method=flickr.photosets.getList").c_str()));
}

void FlickrAPI::downloadPhotoset(const QString &directory)
{
    QList<Photo*> photos = photosets->activeSet()->photoList();
    foreach (Photo *photo, photos)
        sendRequest(flickrRestUrl + QString(client->getURLQueryString(OAuth::Http::Get, flickrRestUrl.toStdString() + "method=flickr.photos.getSizes&photo_id=" + photo->id().toStdString()).c_str()));
}

void FlickrAPI::sendRequest(const QString &request)
{
    QUrl requestUrl(request);
    QNetworkRequest netRequest(requestUrl);
    netAccessManager->get(netRequest);
}

void FlickrAPI::activated(const QModelIndex &index)
{
    photosets->activateSet(index);
    photosets->activeSet()->clear();
    sendRequest(flickrRestUrl + QString(client->getURLQueryString(OAuth::Http::Get, flickrRestUrl.toStdString() + "method=flickr.photosets.getphotos&photoset_id=" + photosets->activeSet()->id().toStdString()).c_str()));
}

void FlickrAPI::parseNetworkReply(QNetworkReply *reply)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonObject = jsonDocument.object();
    QJsonObject jsonData;

    QString key = jsonObject.keys().first();

//    QJsonValue jsonValue;
//    QJsonArray jsonArray;
//    std::cout << qHash(key) << std::endl;

/*    switch (qHash(key))
    {
    case 'user':
        std::cout << "hö" << std::endl;
        break;
    default:
        break;
    }*/

    if (key == "user")
    {
        jsonData = jsonObject["user"].toObject();
        Settings::instance()->setUserID(jsonData["id"].toString());
        Settings::instance()->setUserName(jsonData["username"].toObject()["_content"].toString());
    }
    else if (key == "photosets")
    {
        jsonData = jsonObject["photosets"].toObject();
        QJsonArray jsonArray = jsonData["photoset"].toArray();

        for (int i = 0; i < jsonArray.size(); ++i)
        {
            QJsonObject iObject = jsonArray[i].toObject();
            photosets->addPhotoset(new Photoset(iObject["id"].toString(), iObject["title"].toObject()["_content"].toString()));
        }
    }
    else if (key == "photoset")
    {
        jsonData = jsonObject["photoset"].toObject();
        QJsonArray jsonArray = jsonData["photo"].toArray();

        for (int i = 0; i < jsonArray.size(); ++i)
        {
            QJsonObject iObject = jsonArray[i].toObject();
            photosets->activeSet()->addPhoto(new Photo(iObject["id"].toString(), iObject["title"].toString()));
        }
        emit photoSetActivated(photosets->activeSet());
    }
    else if (key == "sizes")
    {
        jsonData = jsonObject["sizes"].toObject();
        QJsonArray jsonArray = jsonData["size"].toArray();
        QJsonObject lastObject = jsonArray.last().toObject();
        QString source = lastObject["source"].toString();


        std::cout << source.toStdString() << std::endl;
    }

    emit sendResponse(QString::fromUtf8(jsonDocument.toJson()));
}
