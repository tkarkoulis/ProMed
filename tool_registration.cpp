/**************************************************************************
**   Recompile IT Services, Development Dept
**   2/2/2011 2011
**   All Rights Reserved
**************************************************************************/

#include <QSettings>
#include <QNetworkAccessManager>
#include <QCoreApplication>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QFile>
#include <QTextCodec>
#include <QUrlQuery>
#include "tool_registration.h"
#include "tool_recompilecrypto.h"
#include "dialog_generaldialog.h"

Registration::Registration(QObject *parent) : QObject(parent)
{
    onlineReplyFinished = true;
}

bool Registration::checkRegistration() {

    // return true; // DEMO

    // init settings
    QSettings settings;

    // open the registration file and get the data
    QFile f( settings.value("resources_path").toString() + "/prmd.lcs" );

    if( f.open( QIODevice::ReadOnly ) ) {

	// get the file contents
	QString contents = f.readAll();

	return checkRegistrationWithKey( contents );

    } else {
	// couldnt open the file, output error
	last_error = "ERR_NOFILE";
	return false;
    }
}

bool Registration::checkRegistrationWithKey(QString licenseKey) {

    QSettings settings;

    if( !settings.value("reg_email").toString().isEmpty() && !settings.value("reg_code").toString().isEmpty() && !settings.value("reg_name").toString().isEmpty() ) {

	// get the reg_email and reg_code and reg_name
	// and compare it to the values in our license file.
	QString l_email, l_code, l_name;
	QString f_email, f_code, f_license ,f_date, f_name;

	l_email = settings.value("reg_email").toString();
	l_code = settings.value("reg_code").toString();
	l_name = settings.value("reg_name").toString();

	// decode the key
	// start crypto
	RecompileCrypto rc;

	// create the key
	QString decryptedKey = rc.decodeString( licenseKey );

	// analyze the licenseKey
	QStringList lst_licenseKey = decryptedKey.split( ":|:" );

	f_email = lst_licenseKey.at(0);
	f_code = lst_licenseKey.at(1);
	f_license = lst_licenseKey.at(2);
	f_date = lst_licenseKey.at(3);
	f_name = lst_licenseKey.at(4);

	// perform key checks

	if( l_email == f_email && l_code == f_code && l_name == f_name ) {
	    // stage 1

	    // get the mac addresses of system
	    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();

	    for( int i=0; i < ifaces.size(); i++ ) {
		QNetworkInterface it = ifaces.at(i);

		if( ("LIC"+it.hardwareAddress()) == f_license ) return true;
	    }

	    last_error = "ERR_NOMAC";

	    // clean registry
	    cleanRegistry();

	    return false;

	} else {
	    last_error = "ERR_KEYSFAIL";

	    // clean registry
	    cleanRegistry();

	    return false;
	}


	QString encoded_str = rc.encodeString( settings.value("reg_email").toString() );
	if( encoded_str == settings.value("reg_code").toString() ) {
	    // do nothing, just login
	    return true;
	} else {
	    last_error = "ERR_NOAUTH";
	    return false;
	}
    } else {
	last_error = "ERR_NOT_REGISTERED";
	return false;
    }
}

bool Registration::onlineRegister( QString email, QString code ) {

    // start network access manager
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    // connect to finished reply
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    // set request
    QNetworkRequest request( QUrl("http://promed.re-compile.net/promed/index.php") );
    request.setHeader( QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded" );

    QByteArray data;
    QUrl params;
    QUrlQuery q;

    // add email & activationCode
    q.addQueryItem("email", email.toLatin1() );
    q.addQueryItem("activationCode", code.toLatin1() );



    // add NIC mac address
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();

    for( int i=0; i < ifaces.size(); i++ ) {
        QNetworkInterface it = ifaces.at(i);

        if( it.flags().testFlag( QNetworkInterface::IsUp ) ) {
            // make sure the hw-address isnt empty
            if( !it.hardwareAddress().isEmpty() ) {
            // get it
            q.addQueryItem( "macID", it.hardwareAddress().toLatin1() );
            break;
            }
        }
    }

    params.setQuery(q);

    data.append( params.toString() );
    data.remove(0,1);

    // set the reply flag;
    onlineReplyFinished = false;

    // send the request
    manager->post( request, data );

    return true;
}

QString Registration::getLastError() {
    return last_error;
}

bool Registration::isReplyFinished() {
    return onlineReplyFinished;
}

void Registration::replyFinished(QNetworkReply *reply) {
    // output
    QString returned = reply->readAll();

    if ( returned == "ERR_NODATA" ) {

	// output error and quit
	QMessageBox::warning(0, "", QObject::tr("Online Activation Failed: error code: ") + "ERR_NODATA" );
	exit(0);

    } else if( returned == "ERR_MAXLICENSES" ) {

	// output error and quit
	QMessageBox::warning(0, "", QObject::tr("Online Activation Failed: You don't have any available Licenses.") );
	exit(0);

    } else if( returned == "ERR_SAVE" ) {

	// output error and quit
	QMessageBox::warning(0, "", QObject::tr("Online Activation Failed: We could not save your license in our database. Please contact support.") );
	exit(0);

    } else if( returned == "ERR_NOACC" ) {

	// output error and quit
	QMessageBox::warning(0, "", QObject::tr("Online Activation Failed: Your e-mail and activation code check failed. Please try again.") );
	exit(0);

    } else if( returned.isEmpty() ) {

	// output error and quit
	QMessageBox::warning(0, "", QObject::tr("Online Activation Failed: No response from server. Please check your Internet connection settings.") );
	exit(0);

    } else {
	// we have passed, continue with licensing

	// init QSettings
	QSettings settings;

	// open the license file
	QString path = settings.value( "resources_path" ).toString();

	// open the file (overwrite)
	QFile f( path + "/prmd.lcs" );

	if( f.open( QIODevice::ReadWrite ) ) {

	    // write to the file
	    QTextStream ts( &f );
	    ts.setAutoDetectUnicode(true);
	    // set codec
	    QTextCodec *utfcodec = QTextCodec::codecForName( "UTF-8" );
	    ts.setCodec( utfcodec );

	    // write
	    ts << returned;

	    // close file
	    f.close();

	    // decrypt key
	    RecompileCrypto rc;
	    QString decryptedKey = rc.decodeString( returned );

	    // break key down
	    QStringList strlst = decryptedKey.split(":|:");

	    // write settings
	    settings.setValue("reg_email", strlst.at(0) );
	    settings.setValue("reg_code", strlst.at(1) );
	    settings.setValue("reg_name", strlst.at(4) );

	    // test the license
	    if( checkRegistrationWithKey(returned) ) {

		GeneralDialog *gd = new GeneralDialog( "info", QObject::tr("Product Activation"), QObject::tr("Successful Activation"), QObject::tr("Your product has been licensed successfully! Thank you for purchasing ") + QCoreApplication::applicationName() );
		gd->exec();

	    } else {
		// output error and quit
		QMessageBox::warning(0, "", QObject::tr("Online Activation Failed: Registration did not succeed") + "(error:" + last_error + "). " + QObject::tr("Program will now exit.") );

		// clean registry
		cleanRegistry();

		exit(0);
	    }

	} else {
	    // output error and quit
	    QMessageBox::warning(0, "", QObject::tr("Online Activation Failed: Could not write license file. Please contact support.") );
	    exit(0);
	}
    }

    onlineReplyFinished = true;
}

void Registration::cleanRegistry() {

    QSettings settings;

    // clean registry
    settings.remove("reg_email");
    settings.remove("reg_code");
    settings.remove("reg_name");

    // clean license file
    QFile::remove( settings.value( "resources_path" ).toString() + "/prmd.lcs" );
}
