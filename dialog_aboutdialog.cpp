/**************************************************************************
**   Recompile IT Services, Development Dept
**   11/3/2010 2010
**   All Rights Reserved
**************************************************************************/

#include <QProcess>

#include "mainwindow.h"
#include "dialog_aboutdialog.h"
#include "ui_dialog_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent, QString osType ) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    // fix title
    ui->about_title->setText( QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion() );

    // fix fonts for MAC and WIN
    if( osType == "OSX" ) {
        ui->textBrowser->setHtml( ui->textBrowser->toHtml().replace( "font-size:13pt;", "font-size:11pt;" ) );
    } else if( osType == "WIN" )
        ui->textBrowser->setHtml( ui->textBrowser->toHtml().replace( "font-size:13pt;", "font-size:8pt;" ) );
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
