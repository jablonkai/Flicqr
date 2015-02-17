#ifndef PHOTOSET_H
#define PHOTOSET_H

#include <QtCore/QAbstractItemModel>
#include <QtCore/QList>
#include <QtCore/QString>

#include "photo.h"

class Photoset// : public QAbstractItemModel
{
public:
    Photoset();
    Photoset(const QString &__id, const QString __title) : _id(__id), _title(__title) {}

    void setID(const QString &__id) { _id = __id; }
    QString id() const { return _id; }

    void setTitle(const QString &__title) { _title = __title; }
    QString title() const { return _title; }

    void addPhoto(const Photo &_photo) { photos.push_back(_photo); }

private:
    QString _id;
    uint _photos;
    uint _videos;
    QString _title;
    QString _description;

//    QList<Photo> photos;
};

#endif // PHOTOSET_H

/*
 {"id":"72157648334181328",
"primary":"15343017349",
"secret":"8e5c1a8778",
"server":"3942",
"farm":4,
"photos":"58",
"videos":0,
"title":{"_content":"2014-10-11 - No meg\u00e1llj csak! 38"},
"description":{"_content":""},
"needs_interstitial":0,
"visibility_can_see_set":1,
"count_views":"2",
"count_comments":"0",
"can_comment":1,
"date_create":"1413231750",
"date_update":"1413233131"}
 */
