/**************************************************************************
**   Recompile IT Services, Development Dept
**   11/1/2010 2010
**   All Rights Reserved
**************************************************************************/
#include <QCryptographicHash>

#include "widget_login.h"
#include "ui_widget_login.h"
#include "dialog_generaldialog.h"
#include "dbconn.h"
#include "mainwindow.h"

LogIn::LogIn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogIn)
{
    // show myself
    ui->setupUi(this);

    // fix the title
    ui->login_title->setText( QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion() );
}

LogIn::~LogIn()
{
    delete ui;
}

void LogIn::clear() {
    ui->login_pass->clear();
}

void LogIn::startLogin() {
    // we need to cross-check the password that we get from there
    // get the current password:
    QString currentpass = DBConn::instance()->settingValue("dental_office_password");

    qDebug() << currentpass;

    // lets encode please
    QString typedpass = QCryptographicHash::hash( ui->login_pass->text().toLatin1(), QCryptographicHash::Md5 );

    qDebug() << typedpass.toLatin1();

    if( typedpass == currentpass ) {
	// SUCCESSFUL
	MainWindow::instance()->doLogin();
    } else {
	GeneralDialog *gd = new GeneralDialog("error", tr("login"), tr("Error:"), tr("The password you provided was not correct. Please try again!") );
	gd->exec();
    }
}

void LogIn::on_but_login_clicked() {
    // login
    startLogin();
}

void LogIn::on_login_pass_returnPressed() {
    // login
    startLogin();
}
