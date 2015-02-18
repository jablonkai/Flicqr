#ifndef FLICKRAPI_H
#define FLICKRAPI_H

#include <QtCore/QObject>

#define API_KEY "0097a0e5e60f69a177e0d0dfa0bb9c6a"
#define API_SECRET "5b2e1d4c168eccb1"

class QIODevice;
class QNetworkAccessManager;
class QNetworkReply;
class QProgressDialog;

class Photoset;
class PhotosetsModel;

namespace OAuth {
class Client;
}

class FlickrAPI : public QObject
{
    Q_OBJECT

public:
    explicit FlickrAPI(QObject *parent = 0);
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

private slots:
    void parseNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *netAccessManager;
    QProgressDialog *pDialog;
    OAuth::Client *client;
    QString flickrRestUrl;
    PhotosetsModel *photosets;

    void saveToDisk(QString fName, QIODevice *data);
};

#endif // FLICKRAPI_H
