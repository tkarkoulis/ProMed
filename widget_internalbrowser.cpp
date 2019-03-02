/**************************************************************************
**   Recompile IT Services, Development Dept
**   11/29/2010 2010
**   All Rights Reserved
**************************************************************************/

#include "widget_internalbrowser.h"
#include "ui_widget_internalbrowser.h"

#include <QDebug>
#include <QtNetwork/QNetworkReply>

InternalBrowser::InternalBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InternalBrowser)
{
    ui->setupUi(this);

    //n_manager = new QNetworkAccessManager();
    connect( ui->webView->page(), SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)) );

    //ui->webView->page()->setNetworkAccessManager(n_manager);

    flag_urlLoaded = false;
}

void InternalBrowser::loadURL(QString url) {
    // load url, if not loaded before
    if( !flag_urlLoaded ) {
	// load this url
	ui->webView->load( QUrl(url) );
	ui->webView->show();

	// set flag
	flag_urlLoaded = true;
    }
}

void InternalBrowser::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {
    reply->ignoreSslErrors();
}

InternalBrowser::~InternalBrowser()
{
    delete ui;
}

void InternalBrowser::on_pushButton_clicked()
{
    ui->webView->back();
}


void InternalBrowser::on_pushButton_2_clicked()
{
    ui->webView->reload();
}
