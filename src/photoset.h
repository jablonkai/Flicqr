#ifndef PHOTOSET_H
#define PHOTOSET_H

#include <QtCore/QAbstractItemModel>
#include <QtCore/QList>
#include <QtCore/QString>

#include "photo.h"

class Photoset : public QAbstractItemModel
{
    Q_OBJECT

public:
    Photoset();
    Photoset(const QString &__id, const QString __title) : _id(__id), _title(__title) {}

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const { return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex(); }
    QModelIndex parent(const QModelIndex &child) const { return QModelIndex(); }
    int rowCount(const QModelIndex &parent) const { return photos.count(); }
    int columnCount(const QModelIndex &parent) const { return 1; }
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) {}
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) {}

    QString currentID(const QModelIndex &index) const;

    void clear();
    void addPhoto(Photo *photo);

    void setID(const QString &__id) { _id = __id; }
    QString id() const { return _id; }

    void setTitle(const QString &__title) { _title = __title; }
    QString title() const { return _title; }

    void setFolder(const QString &_folder) { localFolder = _folder; }
    QString folder() const { return localFolder; }

    QList<Photo*> photoList() const { return photos; }

private:
    QString _id;
    uint _photos;
    uint _videos;
    QString _title;
    QString _description;

    QString localFolder;

    QList<Photo*> photos;
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
