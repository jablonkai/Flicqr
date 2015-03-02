#include "photosetsmodel.h"

PhotosetsModel::PhotosetsModel(QObject *parent) :
    QAbstractItemModel(parent), activePhotoset(NULL)
{
}

PhotosetsModel::~PhotosetsModel()
{
    delete activePhotoset;
}

QVariant PhotosetsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= photosetList.size())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
            return photosetList.at(index.row())->title();
    }
    else if (role == Qt::ToolTipRole)
    {
        if (index.column() == 0)
            return QString("Photos: %1\nVideos: %2\nCreated on: %3\nUpdated on: %4").arg(photosetList.at(index.row())->photoCount()).arg(photosetList.at(index.row())->videoCount()).arg(photosetList.at(index.row())->createdDate().toString()).arg(photosetList.at(index.row())->updatedDate().toString());
    }
    else
        return QVariant();
}

QVariant PhotosetsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        if (section == 0)
            return tr("Photosets");
    }
    else
        return QString("%1").arg(section);
}

Qt::ItemFlags PhotosetsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index);
}

bool PhotosetsModel::insertRows(int position, int rows, const QModelIndex &index)
{
    beginInsertRows(QModelIndex(), position, position + rows - 1);
    endInsertRows();
    return true;
}

QString PhotosetsModel::currentID(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() >= photosetList.size())
        return QString();

    return photosetList.at(index.row())->ID();
}

void PhotosetsModel::clear()
{
    photosetList.clear();
}

void PhotosetsModel::addPhotoset(Photoset *photoset)
{
    insertRows(rowCount(QModelIndex()), 1);
    photosetList.append(photoset);
}

void PhotosetsModel::activateSet(const QModelIndex &index)
{
    activePhotoset = photosetList.at(index.row());
}
