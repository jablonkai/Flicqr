#ifndef PHOTOSETSMODEL_H
#define PHOTOSETSMODEL_H

#include <QtCore/QAbstractItemModel>

#include "photoset.h"

class PhotosetsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit PhotosetsModel(QObject *parent = 0);
    ~PhotosetsModel();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const { return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex(); }
    QModelIndex parent(const QModelIndex &child) const { return QModelIndex(); }
    int rowCount(const QModelIndex &parent) const { return photosetList.count(); }
    int columnCount(const QModelIndex &parent) const { return 1; }
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) {}
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) {}

    QString currentID(const QModelIndex &index) const;

    void clear();
    void addPhotoset(Photoset *photoset);

    Photoset *activeSet() const { return activePhotoset; }
//    Photoset *byId(const QString id) const { return 0; }
    void activateSet(const QModelIndex &index);

signals:

public slots:

private:
    Photoset *activePhotoset;
    QList<Photoset*> photosetList;
};

#endif // PHOTOSETSMODEL_H
