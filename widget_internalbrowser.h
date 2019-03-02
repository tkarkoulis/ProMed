/**************************************************************************
**   Recompile IT Services, Development Dept
**   11/29/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef WIDGET_INTERNALBROWSER_H
#define WIDGET_INTERNALBROWSER_H

#include <QWidget>
#include <QtNetwork/QNetworkAccessManager>

namespace Ui {
    class InternalBrowser;
}

class InternalBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit InternalBrowser(QWidget *parent = 0);
    void loadURL( QString url );
    ~InternalBrowser();

private:
    Ui::InternalBrowser *ui;
    QNetworkAccessManager *n_manager;
    bool flag_urlLoaded;

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void sslErrors(QNetworkReply * reply, const QList<QSslError> & errors);
};

#endif // WIDGET_INTERNALBROWSER_H
