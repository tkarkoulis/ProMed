/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/22/2010 2010
**   All Rights Reserved
**************************************************************************/
#include <QDebug>
#include <QCryptographicHash>
#include <QSettings>

#include "widget_dentalsettings.h"
#include "ui_widget_dentalsettings.h"
#include "obj_setting.h"
#include "dbconn.h"
#include "mainwindow.h"
#include "dialog_generaldialog.h"

DentalSettings::DentalSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DentalSettings)
{
    ui->setupUi(this);

    // hide errors
    ui->sett_errors->hide();
}

void DentalSettings::reload() {
    // load the settings from database
    QSettings settings;

    ui->sett_oldPass->setText("");

    // dental Office Name
    Setting s = DBConn::instance()->setting( "dental_office_name" );

    if( s.id > 0 && s.value != "" ) { // record exists
	// load it
	ui->sett_officeName->setText( s.value );
    }

    // dental Office Name 2
   s = DBConn::instance()->setting( "dental_office_name2" );

    if( s.id > 0 && s.value != "" ) { // record exists
	// load it
	ui->sett_officeName2->setText( s.value );
    }

    // dental Office Address
    s = DBConn::instance()->setting( "dental_office_address" );

    if( s.id > 0 && s.value != "" ) { // record exists
	// load it
        ui->sett_officeAddr->setPlainText( s.value );
    }

    // dental Office Telephone
    s = DBConn::instance()->setting( "dental_office_telephone" );

    if( s.id > 0 && s.value != "" ) { // record exists
	// load it
	ui->sett_officeTel->setText( s.value );
    }

    // dental currency
    ui->sett_currency->setCurrentIndex( ui->sett_currency->findText( DBConn::instance()->settingValue("dental_office_currency") ) );

    // language
    ui->sett_lang->setCurrentIndex( ui->sett_lang->findText( settings.value("language").toString() ) );
}

DentalSettings::~DentalSettings()
{
    delete ui;
}

void DentalSettings::on_pushButton_clicked()
{
    // init error messages
    QString errors = "";
    ui->sett_newPass->setStyleSheet( "" );
    ui->sett_reTypePass->setStyleSheet( "" );
    ui->sett_oldPass->setStyleSheet( "" );

    // save officeName
    Setting s;
    s.setting = "dental_office_name";
    s.value = ui->sett_officeName->text();
    // store
    if( !DBConn::instance()->insertSetting(s) ) qDebug() << "setting storage error!";

    // save officeName2
    s.setting = "dental_office_name2";
    s.value = ui->sett_officeName2->text();
    // store
    if( !DBConn::instance()->insertSetting(s) ) qDebug() << "setting storage error!";

    // save officeAddress
    s.setting = "dental_office_address";
    s.value = ui->sett_officeAddr->toPlainText();
    // store
    if( !DBConn::instance()->insertSetting(s) ) qDebug() << "setting storage error!";

    // save officeTel
    s.setting = "dental_office_telephone";
    s.value = ui->sett_officeTel->text();
    // store
    if( !DBConn::instance()->insertSetting(s) ) qDebug() << "setting storage error!";

    // save office Password
    // first double check that the original pass is correct

    bool savePass = true;

    // if all are empty, dont save anytthing, skip
    if( ui->sett_oldPass->text() == "" && ui->sett_newPass->text() == "" && ui->sett_reTypePass->text() == "" )
	savePass = false;

    if( savePass ) {
	QString oldPass = DBConn::instance()->settingValue("dental_office_password");

	if( oldPass == ui->sett_oldPass->text() | ui->sett_oldPass->text() != "" ) {
	    qDebug() << "trying to update password";
        if( oldPass == ui->sett_oldPass->text() | oldPass == QCryptographicHash::hash( ui->sett_oldPass->text().toLatin1(), QCryptographicHash::Md5 ) ) {
		// start changing password, go to stage 2
		if( ui->sett_newPass->text() == ui->sett_reTypePass->text() ) {
		    // ok proceed to stage 3, change the password
		    s.setting = "dental_office_password";

		    // if no pass, then clear it
		    if( ui->sett_reTypePass->text() == "" )
			s.value = "";
		    else
            s.value = QCryptographicHash::hash( ui->sett_reTypePass->text().toLatin1(), QCryptographicHash::Md5 );

		    // store
		    qDebug() << "updating password...";
		    if( !DBConn::instance()->insertSetting(s) ) qDebug() << "setting storage error!";

		    // clean
		    ui->sett_errors->hide();
		    ui->sett_newPass->setStyleSheet( "" );
		    ui->sett_reTypePass->setStyleSheet( "" );
		    ui->sett_oldPass->setStyleSheet( "" );

		} else {
		    // create error message
		    errors += tr(" - New Passwords do not match!\n");
		    ui->sett_newPass->setStyleSheet( "border-color:red" );
		    ui->sett_reTypePass->setStyleSheet( "border-color:red" );
		}
	    } else {
		// create error message
		errors += tr(" - Old password is NOT correct!\n");
		ui->sett_oldPass->setStyleSheet( "border-color:red" );
	    }
	}
    }

    // save Currency
    s.setting = "dental_office_currency";
    s.value = ui->sett_currency->currentText();
    // store
    if( !DBConn::instance()->insertSetting(s) ) qDebug() << "setting storage error!";

    // save Language
    QSettings settings;
    if( settings.value("language").toString() != ui->sett_lang->currentText() ) {
	// change language!
	settings.setValue("language", ui->sett_lang->currentText() );

	GeneralDialog *gd = new GeneralDialog("info", tr("language changed"), tr("language has changed!"), tr("Please restart the application for the changes to take effect!") );
	gd->exec();

    } else {
	settings.setValue("language", ui->sett_lang->currentText() );
    }

    if( errors != "" ) {
	// errors
	ui->sett_errors->setText( errors );
	ui->sett_errors->show();
    } else {
	// output success!
	GeneralDialog *gd = new GeneralDialog("info", tr("application settings"), "", tr("Application settings have been successfully saved!") );
	gd->exec();
	ui->sett_oldPass->setText("");
	ui->sett_newPass->setText("");
	ui->sett_reTypePass->setText("");
    }
}

void DentalSettings::on_pushButton_2_clicked()
{
    // go home
    MainWindow::instance()->goHome();
}
