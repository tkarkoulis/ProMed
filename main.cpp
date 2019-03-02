#include <QtWidgets/QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QSettings>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include <QSplashScreen>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QTimer>
//#include <CoreFoundation/CFURL.h>
//#include <CoreFoundation/CFBundle.h>
#include "dialog_generaldialog.h"
#include "dialog_registerdialog.h"
#include "tool_recompilecrypto.h"
#include "tool_registration.h"
#include "mainwindow.h"
#include "dbconn.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // load translator
    QTranslator translator;
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

    // fix qsettings
    QCoreApplication::setOrganizationName("Recompile");
    QCoreApplication::setOrganizationDomain("re-compile.com");
    QCoreApplication::setApplicationName("ProMed");
    QCoreApplication::setApplicationVersion("2.0.1");

    QString osType = "OSX"; // WIN or OSX

#ifdef TARGET_OS_MAC
    QDir dd( QCoreApplication::applicationFilePath() );
    QString bundlePath = dd.absolutePath();
#endif

    // current working directory
    QString cwd = "";

    if( osType == "WIN" ) {
        QSettings::setDefaultFormat( QSettings::IniFormat ); // WIN
        QSettings::setPath( QSettings::IniFormat, QSettings::UserScope, "resources/" );
    } else {
        QSettings::setDefaultFormat( QSettings::IniFormat ); // OSX
        QSettings::setPath( QSettings::IniFormat, QSettings::UserScope, bundlePath + "/resources" );
        cwd = bundlePath + "/";
    }

    //QMessageBox::information(0, "", QDir::currentPath() );

    // check if the folder exists
    //QSettings settings(QCoreApplication::applicationDirPath()+"/../Resources/Recompile/ProMed.ini", QSettings::IniFormat);
    QSettings settings;

    // take care of language
    if( settings.value("language").toString().isEmpty() ) {
	// empty, make english default
	settings.setValue("language", "English");
	// load translator
    translator.load( "promed_en" );
	// install translator
	a.installTranslator( &translator );
    } else {
	if( settings.value("language").toString() == "English" )
            translator.load( cwd + "translations/promed_en" );
	if( settings.value("language").toString() == "Greek" )
            translator.load( cwd + "translations/promed_el" );
	if( settings.value("language").toString() == "French" )
            translator.load( cwd + "translations/promed_fr" );

	// install
	a.installTranslator( &translator );
    }

    // create splash screen
    QPixmap pixmap(":/images/splash.jpg");
    QSplashScreen splash(pixmap);
    // show splash screen
    splash.show();

    splash.setStyleSheet("font-size:10px;");
    splash.showMessage( QObject::tr("loading...") );
    a.processEvents();

    splash.showMessage( QObject::tr("creating settings...") );
    a.processEvents();

    splash.showMessage( QObject::tr("checking folders...") );
    a.processEvents();

    // get data dir
    QDir d( QFileInfo(settings.fileName()).absolutePath() );

    qDebug() << "------ " << settings.fileName();

    // set it in settings
    settings.setValue( "data_path", d.absolutePath() + "/" + QCoreApplication::applicationName() );
    settings.setValue("resources_path", d.absolutePath() );
    settings.setValue( "cwd", cwd );

    if( d.exists( d.absolutePath() + "/" + QCoreApplication::applicationName() ) ) { // do nothing, dir exists
	qDebug() << "Dir exists...!" << d.absolutePath() + "/" + QCoreApplication::applicationName();
    } else {
	if( !d.mkdir( d.absolutePath() + "/" + QCoreApplication::applicationName() ) ) {
	    // output the general dialog and tell the user to create it
	    GeneralDialog *gd = new GeneralDialog( "warning", "create directories", "",
			"The system could not create the following directories:\n" + d.absolutePath() + "/"
			 + QCoreApplication::applicationName() +
			 "\nPlease create it manually in order for the system to work properly." );
	    gd->setMinimumHeight(170);
	    gd->setStyleSheet("QLabel { font-size:11px; }");
	    gd->exec();
	}
	qDebug() << "Creating directory...!" << d.absolutePath() + "/" + QCoreApplication::applicationName();
    }

    splash.showMessage( QObject::tr("checking license...") );
    a.processEvents();

//    if ( !DBConn::instance()->demo() ) {
//    // check license
//    Registration *r = new Registration(0);

//    if( !r->checkRegistration() ) { // registration failed

//	// check reason it failed
//	if( r->getLastError() == "ERR_NOAUTH" ) {
//	    settings.remove("reg_email");
//	    settings.remove("reg_code");
//	    settings.remove("reg_name");

//	    // clean license file
//	    QFile::remove( settings.value( "resources_path" ).toString() + "/prmd.lcs" );

//	    GeneralDialog *gd = new GeneralDialog( "error", QObject::tr("product activation"), "",
//			QObject::tr("This application could not validate the user license. Please restart the application "
//					  "and re-enter your registration data.") );
//	    gd->exec();
//	    exit(0);
//	} else if( r->getLastError() == "ERR_NOT_REGISTERED" | r->getLastError() == "ERR_NOFILE" ) {
//	    // start registration dialog
//	    RegisterDialog *rd = new RegisterDialog();
//	    if( rd->exec() == QDialog::Accepted ) {
//		// lets try to validate
//		if( !rd->getCode().isEmpty() && !rd->getEmail().isEmpty() ) {
//		    // stage 2

//		    // do online registration
//		    if( !r->onlineRegister( rd->getEmail(), rd->getCode() ) ) {
//			GeneralDialog *gd = new GeneralDialog( "error", QObject::tr("product activation"), "",
//				    QObject::tr("Your online activation has failed!") );
//			gd->exec();
//		    }

//		    // make program wait for the slot to finish
//		    while( !r->isReplyFinished() ) a.processEvents();

//		} else {
//		    GeneralDialog *gd = new GeneralDialog( "error", QObject::tr("product activation"), "",
//				QObject::tr("You need to enter both your e-mail and your activation code. Please re-start the program and try again.") );
//		    gd->exec();
//		    exit(0);
//		}
//	    } else exit(0);
//	} else if( r->getLastError() == "ERR_NOMAC" ) {
//	    GeneralDialog *gd = new GeneralDialog( "error", QObject::tr("license error"), "",
//			QObject::tr("This license is not authorized for this machine. Please contact support.") );
//	    gd->exec();
//	    exit(0);
//	} else if( r->getLastError() == "ERR_KEYSFAIL" ) {
//	    GeneralDialog *gd = new GeneralDialog( "error", QObject::tr("license error"), "",
//			QObject::tr("License verification failed! Reason: key_mismatch. Please contact support.") );
//	    gd->exec();
//	    exit(0);
//        } else {
//            GeneralDialog *gd = new GeneralDialog( "error", QObject::tr("license error"), "",
//                        QObject::tr("License verification failed! Reason: unknown. Please contact support.") );
//            gd->exec();
//            exit(0);
//        }
//    }

//    }

    splash.showMessage( QObject::tr("starting...") );
    a.processEvents();

    MainWindow w(0, osType );
    w.setWindowIcon( QIcon(":/images/ProMedIcon") );
    w.setWindowOpacity( 0.0 );

    if( settings.value("fullscreen").toInt() ) w.showFullScreen();
    else w.show();

    // animate fade in
    QPropertyAnimation *animation = new QPropertyAnimation( &w, "windowOpacity");
    animation->setDuration(350);
    animation->setStartValue( 0.0 );
    animation->setEndValue( 1.0 );

    QTimer::singleShot(1500, animation, SLOT(start()) );

    QTimer::singleShot(2000, &splash, SLOT(close()) );

    return a.exec();
}
