#ifndef FLICKRAPI_H
#define FLICKRAPI_H

#include <QtCore/QObject>

#define API_KEY "0097a0e5e60f69a177e0d0dfa0bb9c6a"
#define API_SECRET "5b2e1d4c168eccb1"

class QIODevice;
class QNetworkAccessManager;
class QNetworkReply;
class QProgressDialog;

namespace OAuth {
class Client;
}

class Photoset;
class PhotosetsModel;

namespace Ui {
class MainWindow;
}

class FlickrAPI : public QObject
{
    Q_OBJECT

public:
    explicit FlickrAPI(QObject *parent = 0, Ui::MainWindow *_ui = 0);
    ~FlickrAPI();

    PhotosetsModel *photosetsModel() const { return photosets; }

    void authenticate();
    void getPhotosetsList();
    void downloadPhotoset(const QString &directory);

    void sendRequest(const QString &request);

signals:
    void sendResponse(const QString &response);
    void photoSetActivated(Photoset *album);

public slots:
    void activated(const QModelIndex &index);
    void photoActivated(const QModelIndex &index);

private slots:
    void parseNetworkReply(QNetworkReply *reply);
    void dowloadCanceled();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *netAccessManager;
    QProgressDialog *pDialog;
    OAuth::Client *client;
    QString flickrRestUrl;
    PhotosetsModel *photosets;
    bool albumDownloading;

    void saveToDisk(QString fName, QIODevice *data);
};

#endif // FLICKRAPI_H
