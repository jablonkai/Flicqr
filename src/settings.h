#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore/QString>

class Settings
{
public:
    static Settings *instance() { static Settings instance; return &instance; }

    void setUserName(const QString &name) { _userName = name; }
    QString userName() const { return _userName; }

    void setUserID(const QString &id) { _userID = id; }
    QString userID() const { return _userID; }

    void setOauthTokenKey(const QString &key) { _oauthTokenKey = key; }
    QString oauthTokenKey() const { return _oauthTokenKey; }

    void setOauthTokenSecret(const QString &secret) { _oauthTokenSecret = secret; }
    QString oauthTokenSecret() const { return _oauthTokenSecret; }

private:
    Settings();
    ~Settings();

    QString _userName;
    QString _userID;
    QString _oauthTokenKey;
    QString _oauthTokenSecret;
};

#endif // SETTINGS_H
