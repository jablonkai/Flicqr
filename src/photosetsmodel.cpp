#include "photosetsmodel.h"

PhotosetsModel::PhotosetsModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

QVariant PhotosetsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= photosetList.size())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
    {
        if (index.column() == 0)
            return photosetList.at(index.row())->title();
//        if (index.column() == 1)
//            return photosetList.at(index.row()).translated;
    }
    else
        return QVariant();
}

QVariant PhotosetsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        if (section == 0)
            return tr("Photosets");
    else
        return QString("%1").arg(section);
}

Qt::ItemFlags PhotosetsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index); // | Qt::ItemIsEditable;
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

    //if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
//    {
 //       if (index.column() == 0)
            return photosetList.at(index.row())->id();
//        if (index.column() == 1)
//            return photosetList.at(index.row()).translated;
  //  }
    //else
      //  return QString();
}

void PhotosetsModel::clear()
{
    photosetList.clear();
}

void PhotosetsModel::addPhotoset(Photoset *photoset)
{
    insertRows(rowCount(QModelIndex()), 1);
    photosetList.append(photoset);
/*    insertRows(rowCount(QModelIndex()), 1);
    list.append(entry);
    setSaved(false);*/
}

void PhotosetsModel::activateSet(const QModelIndex &index)
{
    activePhotoset = photosetList.at(index.row());
}
