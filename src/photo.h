#ifndef PHOTO_H
#define PHOTO_H

#include <QtCore/QDateTime>
#include <QtCore/QString>

class Photo
{
public:
    Photo();
    Photo(const QString &__id, const QString &__title) : _id(__id), _title(__title) {}

    void setID(const QString &__id) { _id = __id; }
    QString id() const { return _id; }

    void setTitle(const QString &__title) { _title = __title; }
    QString title() const { return _title; }

    void setSize(const int &s) { _size = s; }
    int size() const { return _size; }

    void setDateTaken(const QDateTime &t) { _taken = t; }
    QDateTime dateTaken() const { return _taken; }

private:
    QString _id;
    QString _title;
    int _size;
    QDateTime _taken;
    bool _isPublic;
    bool _isFriend;
    bool _isFamily;
};

#endif // PHOTO_H

/*{ "photo":
 * { "id": "15788651237",
 * "secret": "01476d09c3",
 * "server": "7509",
 * "farm": 8,
 * "dateuploaded": "1418031908",
 * "isfavorite": 0,
 * "license": 0,
 * "safety_level": 0,
 * "rotation": 0,
 * "originalsecret": "4723d34c11",
 *  "originalformat": "jpg",
 *
    "owner": { "nsid": "23924687@N08", "username": "Jablonkai Tamás", "realname": "", "location": "", "iconserver": 0, "iconfarm": 0, "path_alias": "jablonkaitamas" },
    "title": { "_content": "DSC02661" },
    "description": { "_content": "                               " },
    "visibility": { "ispublic": 0, "isfriend": 0, "isfamily": 0 },
    "dates": { "posted": "1418031908", "taken": "2014-12-07 15:13:31", "takengranularity": 0, "takenunknown": 0, "lastupdate": "1418031909" },
    "permissions": { "permcomment": 3, "permaddmeta": 2 }, "views": 4,
    "editability": { "cancomment": 1, "canaddmeta": 1 },
    "publiceditability": { "cancomment": 1, "canaddmeta": 0 },
    "usage": { "candownload": 1, "canblog": 1, "canprint": 1, "canshare": 1 },
    "comments": { "_content": 0 },
    "notes": {
      "note": [

      ] },
    "people": { "haspeople": 0 },
    "tags": {
      "tag": [

      ] },
    "urls": {
      "url": [
        { "type": "photopage", "_content": "https:\/\/www.flickr.com\/photos\/jablonkaitamas\/15788651237\/" }
      ] }, "media": "photo" }, "stat": "ok" }*/

/*{ "sizes": { "canblog": 1, "canprint": 1, "candownload": 1,
    "size": [
      { "label": "Square", "width": 75, "height": 75, "source": "https:\/\/farm8.staticflickr.com\/7509\/15788651237_01476d09c3_s.jpg", "url": "https:\/\/www.flickr.com\/photos\/jablonkaitamas\/15788651237\/sizes\/sq\/", "media": "photo" },
      { "label": "Large Square", "width": "150", "height": "150", "source": "https:\/\/farm8.staticflickr.com\/7509\/15788651237_01476d09c3_q.jpg", "url": "https:\/\/www.flickr.com\/photos\/jablonkaitamas\/15788651237\/sizes\/q\/", "media": "photo" },
      { "label": "Thumbnail", "width": 100, "height": 75, "source": "https:\/\/farm8.staticflickr.com\/7509\/15788651237_01476d09c3_t.jpg", "url": "https:\/\/www.flickr.com\/photos\/jablonkaitamas\/15788651237\/sizes\/t\/", "media": "photo" },
      { "label": "Small", "width": "240", "height": "180", "source": "https:\/\/farm8.staticflickr.com\/7509\/15788651237_01476d09c3_m.jpg", "url": "https:\/\/www.flickr.com\/photos\/jablonkaitamas\/15788651237\/sizes\/s\/", "media": "photo" },
      { "label": "Small 320", "width": "320", "height": "240", "source": "https:\/\/farm8.staticflickr.com\/7509\/15788651237_01476d09c3_n.jpg", "url": "https:\/\/www.flickr.com\/photos\/jablonkaitamas\/15788651237\/sizes\/n\/", "media": "photo" },
      { "label": "Medium", "width": "500", "height": "375", "source": "https:\/\/farm8.staticflickr.com\/7509\/15788651237_01476d09c3.jpg", "url": "https:\/\/www.flickr.com\/photos\/jablonkaitamas\/15788651237\/sizes\/m\/", "media": "photo" },
      { "label": "Medium 640", "width": "640", "height": "480", "source": "https:\/\/farm8.staticflickr.com\/7509\/15788651237_01476d09c3_z.jpg", "url": "https:\/\/www.flickr.com\/photos\/jablonkaitamas\/15788651237\/sizes\/z\/", "media": "photo" },
      { "label": "Medium 800", "width": "800", "height": "600", "source": "https:\/\/farm8.staticflickr.com\/7509\/15788651237_01476d09c3_c.jpg", "url": "https:\/\/www.flickr.com\/photos\/jablonkaitamas\/15788651237\/sizes\/c\/", "media": "photo" },
      { "label": "Large", "width": "1024", "height": "768", "source": "https:\/\/farm8.staticflickr.com\/7509\/15788651237_01476d09c3_b.jpg", "url": "https:\/\/www.flickr.com\/photos\/jablonkaitamas\/15788651237\/sizes\/l\/", "media": "photo" },
      { "label": "Large 1600", "width": "1600", "height": "1200", "source": "https:\/\/farm8.staticflickr.com\/7509\/15788651237_7a0601f660_h.jpg", "url": "https:\/\/www.flickr.com\/photos\/jablonkaitamas\/15788651237\/sizes\/h\/", "media": "photo" },
      { "label": "Large 2048", "width": "2048", "height": "1536", "source": "https:\/\/farm8.staticflickr.com\/7509\/15788651237_ef93350351_k.jpg", "url": "https:\/\/www.flickr.com\/photos\/jablonkaitamas\/15788651237\/sizes\/k\/", "media": "photo" },
      { "label": "Original", "width": "5152", "height": "3864", "source": "https:\/\/farm8.staticflickr.com\/7509\/15788651237_4723d34c11_o.jpg", "url": "https:\/\/www.flickr.com\/photos\/jablonkaitamas\/15788651237\/sizes\/o\/", "media": "photo" }
    ] }, "stat": "ok" }*/
