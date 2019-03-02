/**************************************************************************
**   Recompile IT Services, Development Dept
**   2/2/2011 2011
**   All Rights Reserved
**************************************************************************/

#ifndef TOOL_REGISTRATION_H
#define TOOL_REGISTRATION_H

#include <QString>
#include <QObject>
#include <QNetworkReply>

class Registration : public QObject
{
Q_OBJECT
public:
    Registration(QObject *parent);
    // check registration
    bool checkRegistration();
    bool checkRegistrationWithKey( QString licenseKey );
    // register online
    bool onlineRegister( QString email, QString code );
    // return last error
    QString getLastError();
    // check if it has finished
    bool isReplyFinished();

private:
    QString last_error;
    bool onlineReplyFinished;
    void cleanRegistry();

private slots:
    void replyFinished( QNetworkReply *reply );
};

#endif // TOOL_REGISTRATION_H
