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
    QString ID() const { return _id; }

    void setTitle(const QString &__title) { _title = __title; }
    QString title() const { return _title; }

    void setFormat(const QString &f) { _format = f; }
    QString format() const { return _format; }

    void setDescription(const QString &desc) { _description = desc; }
    QString description() const { return _description; }

    void setDateTaken(const QDateTime &t) { _taken = t; }
    QDateTime dateTaken() const { return _taken; }

    void setDateUploaded(const QDateTime &u) { _uploaded = u; }
    QDateTime dateUploaded() const { return _uploaded; }

    void setDateUpdate(const QDateTime &u) { _update = u; }
    QDateTime dateUpdate() const { return _update; }

    void setPublic(const bool p) { _isPublic = p; }
    bool isPublic() const { return _isPublic; }

    void setFriend(const bool f) { _isFriend = f; }
    bool isFriend() const { return _isFriend; }

    void setFamily(const bool f) { _isFamily = f; }
    bool isFamily() const { return _isFamily; }

private:
    QString _id;
    QString _title;
    QString _format;

    QString _description;

    QDateTime _taken;
    QDateTime _uploaded;
    QDateTime _update;

    bool _isPublic;
    bool _isFriend;
    bool _isFamily;
};

#endif // PHOTO_H
