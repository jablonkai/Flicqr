#include "photoset.h"

Photoset::Photoset()
{
}

QVariant Photoset::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= photos.size())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
    {
        if (index.column() == 0)
            return photos.at(index.row())->title();
        else if (index.column() == 1)
            return photos.at(index.row())->format();
        else if (index.column() == 2)
            return photos.at(index.row())->dateTaken();
    }
    else
        return QVariant();
}

QVariant Photoset::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        if (section == 0)
            return tr("Title");
        else if (section == 1)
            return tr("Format");
        else if (section == 2)
            return tr("Taken");
    else
        return QString("%1").arg(section);
}

Qt::ItemFlags Photoset::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index);
}

bool Photoset::insertRows(int position, int rows, const QModelIndex &index)
{
    beginInsertRows(QModelIndex(), position, position + rows - 1);
    endInsertRows();
    return true;
}

QString Photoset::currentID(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() >= photos.size())
        return QString();

    return photos.at(index.row())->ID();
}

void Photoset::clear()
{
    photos.clear();
}

void Photoset::addPhoto(Photo *photo)
{
    insertRows(rowCount(QModelIndex()), 1);
    photos.append(photo);
}
