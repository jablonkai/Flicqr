#ifndef PHOTO_H
#define PHOTO_H

#include <QtCore/QString>

class Photo
{
public:
    Photo();
    Photo(const QString &__id, const QString &__title) : _id(__id), _title(__title) {}



private:
    QString _id;
    QString _title;
};

#endif // PHOTO_H
