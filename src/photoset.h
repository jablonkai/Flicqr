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
    int columnCount(const QModelIndex &parent) const { return 3; }
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
    QString ID() const { return _id; }

    void setTitle(const QString &__title) { _title = __title; }
    QString title() const { return _title; }

    void setPhotoCount(const int n) { _photos = n; }
    int photoCount() const { return _photos; }

    void setVideoCount(const int n) { _videos = n; }
    int videoCount() const { return _videos; }

    void setDescription(const QString desc) { _description = desc; }
    QString description() const { return _description; }

    void setCreatedDate(const QDateTime created) { _create = created; }
    QDateTime createdDate() const { return _create; }

    void setUpdatedDate(const QDateTime updated) { _update = updated; }
    QDateTime updatedDate() const { return _update; }

    void setFolder(const QString &_folder) { localFolder = _folder; }
    QString folder() const { return localFolder; }

    Photo *byID(const QString _id) const { foreach(Photo *photo, photos) if (photo->ID() == _id) return photo; return 0; }

    QString titleByID(const QString _id) const { foreach(Photo *photo, photos) if(photo->ID() == _id) return photo->title(); return 0; }

    QList<Photo*> photoList() const { return photos; }

private:
    QString _id;
    QString _title;
    int _photos;
    int _videos;
    QString _description;
    QDateTime _create;
    QDateTime _update;

    QString localFolder;

    Photo *primaryPhoto;
    QList<Photo*> photos;
};

#endif // PHOTOSET_H
