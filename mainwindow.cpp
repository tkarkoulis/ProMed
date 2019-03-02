#include <QMessageBox>
#include <QDate>
#include <QDebug>
#include <QSettings>
#include <QTimer>
#include <QProgressBar>
#include <QNetworkAccessManager>
#include <QNetworkConfigurationManager>
#include <QFile>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDesktopServices>
#include <QUrlQuery>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog_generaldialog.h"
#include "dialog_aboutdialog.h"
#include "obj_appointment.h"
#include "dbconn.h"

MainWindow *MainWindow::s_instance = 0;

MainWindow::MainWindow(QWidget *parent, QString osType ) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // load Screens (except add patient)
    listPatientsScreen = new ListPatients(NULL);
    bigCalendarScreen = new BigCalendar(0, osType);
    statisticsScreen = new Statistics();
    settingsScreen = new DentalSettings();
    backupScreen = new BackupUI();
    browserScreen = new InternalBrowser();
    newPatientForm = 0;
    viewPatientScreen = 0;

    s_instance = this;

    // first start - not logged in
    loggedIn = false;
    isDemo = DBConn::instance()->demo();

    // load myself
    ui->setupUi(this);

    // set the Operating System Type
    s_osType = osType;

    // fix fonts for MAC and WIN
    if( osType == "OSX" ) {
        // general stylesheet
        this->setStyleSheet( "QWidget { font-size:12pt; }" "#formTitle { font-size:13pt; color: #fff; }"
                             "#formHeader1, #formHeader2, #formHeader3 { font-size:11pt; color: #fff; }"
                             "#dlg_text{ font-size:12pt; } #dlg_text_title { font-weight:bold; font-size: 12pt; }"
                             "QPushButton{ font-size: 12pt; }"
                             "#BackupUI #toolButton { font-size: 14px; }"
                             "#day1, #day2, #day3, #day4, #day5, #day6, #day7 { font-size:12pt; }"
                             "#scrollAreaPatRecord QToolButton { font-size:11pt; }");

        // main textbrowser
        ui->textBrowser->setHtml( ui->textBrowser->toHtml().replace( "font-size:10pt;", "font-size:11pt;" ) );
        ui->textBrowser->setHtml( ui->textBrowser->toHtml().replace( "font-size:13pt;", "font-size:11pt;" ) );
    } else if( osType == "WIN" ) {
        // general stylesheet
        this->setStyleSheet( "QWidget { font-size:8pt; }" "#formTitle { font-size:10pt; color: #fff; }"
                             "#formHeader1, #formHeader2, #formHeader3 { font-size:8pt; color: #fff; }"
                             "#dlg_text{ font-size:8pt; } #dlg_text_title { font-weight:bold; font-size: 9pt; }"
                             "QPushButton{ font-size: 8pt; }"
                             "#day1, #day2, #day3, #day4, #day5, #day6, #day7 { font-size:9pt; }");

        // main textBrowser
        ui->textBrowser->setHtml( ui->textBrowser->toHtml().replace( "font-size:10pt;", "font-size:8pt;" ) );
        ui->textBrowser->setHtml( ui->textBrowser->toHtml().replace( "font-size:13pt;", "font-size:8pt;" ) );
    }

    // fix statusbar
    clockLabel = new QLabel( QDateTime().currentDateTime().toString("d MMM, yyyy HH:mm:ss") );
    clockLabel->setStyleSheet("margin-left:10px;");
    ui->statusBar->setStyleSheet("QLabel{ font-size:11px;} #statusBar{ border-top:1px solid #999; }");
    ui->statusBar->addWidget( clockLabel );

    // set timer
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateClock()));
    timer->start(1000);

    this->setWindowTitle( QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion() );

    QSettings settings;

    // initialize the application data
    ui->info_pname->setText( QCoreApplication::applicationName() );
    ui->info_pversion->setText( tr("Version: ") + QCoreApplication::applicationVersion() );
    ui->info_plicense->setText( tr("Licensed To: ") + settings.value("reg_name").toString() + " - " + settings.value("reg_email").toString() );

    QTimer::singleShot( 2000, this, SLOT(checkUpdates()) );

    // fix fullscreen menu
    if( settings.value("fullscreen").toInt() > 0 ) ui->actionFullscreen->setChecked(true);

    // if not logged in, dont
    if( !loggedIn && DBConn::instance()->settingValue("dental_office_password") != "" && !isDemo ) {
	// create login box
	closeDialogs();
	disableMenus();

	loginScreen = new LogIn();
	ui->mainpageWidget->parentWidget()->layout()->addWidget(loginScreen);
	loginScreen->show();
	return;
    } else {
	ui->actionLogout->setEnabled(false);
        loadAppointmentsForDate( QDate().currentDate().toString("yyyy-MM-dd") );
    }
}

void MainWindow::checkUpdates() {
    // first check if we have internet
    QNetworkConfigurationManager *mngr = new QNetworkConfigurationManager();

    // return if not active
    if( !mngr->isOnline() ) {
	// need to return, we are not online
	return;
    }

    // start network access manager
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    // connect to finished reply
    connect(manager, SIGNAL( finished(QNetworkReply*) ), this, SLOT( checkLicenseVersionRequestFinished(QNetworkReply*) ));

    // set request
    QNetworkRequest request( QUrl("http://promed.re-compile.net/promed/index.php") );
    request.setHeader( QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded" );

    QSettings settings;
    QByteArray data;
    QUrl params;
    QUrlQuery q;

    // add email & activationCode
    q.addQueryItem("email", settings.value("reg_email").toString() );
    q.addQueryItem("activationCode", settings.value("reg_code").toString() );
    q.addQueryItem("macID", "macID" );
    q.addQueryItem("checkLicense", "true" );

        params.setQuery(q);

    data.append( params.toString() );
    data.remove(0,1);

    // send the request
    manager->post( request, data );

    // create the version request
    QNetworkAccessManager *manager2 = new QNetworkAccessManager();

    // connect to finished reply
    connect(manager2, SIGNAL( finished(QNetworkReply*) ), this, SLOT( checkAppVersionRequestFinished(QNetworkReply*) ));

    QNetworkRequest request2( QUrl("http://promed.re-compile.net/promed/currentversion.php") );
    request2.setHeader( QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded" );

    // send the request
    manager2->post( request2, "" );

}

void MainWindow::checkLicenseVersionRequestFinished(QNetworkReply *reply) {

    // set next check
    QTimer::singleShot( 300000, this, SLOT(checkUpdates()) );

    if( reply->error() == QNetworkReply::NoError ) {
	// do the things you need to do in here
	QString replyString = reply->readAll();

	if( replyString == "OK" ) {
	    // everything is ok, return silently
	    return;
	} else if( replyString == "ERR_NOACC" ) {
	    // license was not verfied (it has been deleted by the admins of the ProMed website or something
	    // need to unset the license file, remove the entries from the .ini, display error message and quit
	    GeneralDialog *gd = new GeneralDialog( "error", "not licensed", "License verification error", "Your license was not verified, or expired. Please contact support." );
	    gd->exec();

	    // clean registry
	    QSettings settings;
	    settings.remove("reg_email");
	    settings.remove("reg_code");
	    settings.remove("reg_name");

	    // clean license file
	    QFile::remove( settings.value( "resources_path" ).toString() + "/prmd.lcs" );
	    // exit application
	    exit(0);
	}
    }
}

void MainWindow::checkAppVersionRequestFinished(QNetworkReply * reply) {
    // check for error
    if( reply->error() == QNetworkReply::NoError ) {
	// do the things you need to do in here
	QString currentVersion = reply->readAll();

	if( currentVersion == QCoreApplication::applicationVersion() ) {
	    // same version, do nothing
	} else {
	    // different version, need to find out if its bigger or smaller

	    if( QCoreApplication::applicationVersion() < currentVersion ) {

		// display new version message
                GeneralDialog *gd = new GeneralDialog("info", QObject::tr("new application version"),
                                                      QObject::tr("New version available!"),
                                                      QObject::tr("There is a new version available for download! Contact support (promed@re-compile.com) for more information!") );
                // make the button to add to the dialog
                QPushButton *button = new QPushButton();
                button->setText( tr("Download Update") );
                button->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
                // connect to download Page
                connect( button, SIGNAL(clicked()), this, SLOT(openDownloadPage()) );
                // add button to dialog
                gd->addButton(button);
                // show dialog
		gd->exec();
	    }
	}
    }
}

void MainWindow::openDownloadPage() {

    // open QSEttings
    QSettings settings;

    // create url with client data
    QUrl url("http://promed.re-compile.net/promed/getUpdates.php?email=" + settings.value("reg_email").toString() + "&activationCode=" + settings.value("reg_code").toString() + "&ver=" + QCoreApplication::applicationVersion() );

    // if fullscreen, go to normal
    if( this->isFullScreen() ) this->showNormal();

    // open using appropriate browser
    QDesktopServices::openUrl( url );
}

void MainWindow::doLogin() {
    // perform login - if we were called, login returned true;
    loggedIn = true;

    // close login
    loginScreen->hide();
    ui->mainpageWidget->parentWidget()->layout()->removeWidget(loginScreen);

    // start date
    ui->calendarWidget->setSelectedDate( QDate().currentDate() );

    loadAppointmentsForDate( QDate().currentDate().toString("yyyy-MM-dd") );

    ui->actionLogout->setEnabled(true);

    // load home
    goHome();
}

void MainWindow::doLogout() {
    // logout
    loggedIn = false;

    closeDialogs();
    disableMenus();

    // show
    ui->mainpageWidget->parentWidget()->layout()->addWidget(loginScreen);
    loginScreen->show();
    loginScreen->clear();

    this->setWindowTitle( QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion() );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::goHome() {
    // close the dialogs
    MainWindow::closeDialogs();

    qDebug() << "closed dialogs, going home :";

    // re-enable home screen
    if( ui->mainpageWidget->isHidden() ) ui->mainpageWidget->show();

    // enable buttons
    enableMenus();

    // disable my button
    ui->actionHome->setEnabled(false);

    // fix today's date
    ui->calendarWidget->setSelectedDate( QDate().currentDate() );

    // load appointments for today
    loadAppointmentsForDate( QDate().currentDate().toString("yyyy-MM-dd") );

    // fix window title
    setWindowTitle( QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion() + " - " + tr("Home") );
}

void MainWindow::goPatientList(bool reload = false) {
    // close the dialogs
    closeDialogs();

    // get the mainWidget Layout
    QLayout *lay = ui->mainpageWidget->parentWidget()->layout();

    // reload
    if( reload ) listPatientsScreen->reload();

    // add listPatients to the layout
    lay->addWidget( listPatientsScreen );

    // enable buttons
    enableMenus();

    // disable my button
    ui->actionList_Patients->setEnabled(false);

    if( listPatientsScreen->isHidden() ) listPatientsScreen->show();

    // fix window title
    setWindowTitle( QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion() + " - " + tr("Patients") );
}

void MainWindow::goViewPatient(int pID) {
    // close the dialogs
    closeDialogs();

    qDebug() << "closed dialogs, opening view patient for pat :" << pID;

    // get the mainWidget Layout
    QLayout *lay = ui->mainpageWidget->parentWidget()->layout();

    // create add patient form
    viewPatientScreen = new ViewPatient(this, pID);

    // add new addition form
    lay->addWidget( viewPatientScreen );

    viewPatientScreen->show();

    // fix window title
    setWindowTitle( QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion() + " - " + tr("View Patient Record") );
}

void MainWindow::goAddPatient(int patID) {
    // close the dialogs
    closeDialogs();

    // get the mainWidget Layout
    QLayout *lay = ui->mainpageWidget->parentWidget()->layout();

    // create add patient form
    if( patID > 0 ) // EDIT
	newPatientForm = new AddPatient(0, patID);
    else { // ADD
	newPatientForm = new AddPatient();

	// make New... button disabled, since we are already in NEW
	ui->actionNew->setEnabled(false);
    }

    // add new addition form
    lay->addWidget( newPatientForm );

    // fix window title
    setWindowTitle( QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion() + " - " + tr("Add/Edit Patient Record") );
}

void MainWindow::goCalendar() {
    // close the dialogs
    closeDialogs();

    // get the mainWidget Layout
    QLayout *lay = ui->mainpageWidget->parentWidget()->layout();

    // reload
    bigCalendarScreen->reloadMonth();

    // add listPatients to the layout
    lay->addWidget( bigCalendarScreen );

    // enable buttons
    enableMenus();

    // disable my button
    ui->actionAppointments->setEnabled(false);

    if( bigCalendarScreen->isHidden() ) bigCalendarScreen->show();

    // fix window title
    setWindowTitle( QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion() + " - " + tr("Appointments Calendar") );
}

void MainWindow::goStatistics() {
    // close the dialogs
    closeDialogs();

    // get the mainWidget Layout
    QLayout *lay = ui->mainpageWidget->parentWidget()->layout();

    // reload
    // statisticsScreen->reload();

    // add listPatients to the layout
    lay->addWidget( statisticsScreen );

    // enable buttons
    enableMenus();

    // disable my button
    ui->actionStatistics->setEnabled(false);

    if( statisticsScreen->isHidden() ) statisticsScreen->show();

    // fix window title
    setWindowTitle( QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion() + " - " + tr("Statistics") );
}

void MainWindow::goSettings() {
    // close the dialogs
    closeDialogs();

    // get the mainWidget Layout
    QLayout *lay = ui->mainpageWidget->parentWidget()->layout();

    // reload
    settingsScreen->reload();

    // add listPatients to the layout
    lay->addWidget( settingsScreen );

    // enable buttons
    enableMenus();

    // disable my button
    ui->actionSettings->setEnabled(false);

    if( settingsScreen->isHidden() ) settingsScreen->show();

    // fix window title
    setWindowTitle( QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion() + " - " + tr("Application Settings") );
}

void MainWindow::goBackup() {
    // close the dialogs
    closeDialogs();

    // get the mainWidget Layout
    QLayout *lay = ui->mainpageWidget->parentWidget()->layout();

    // clear
    // backupScreen->clear();

    // add listPatients to the layout
    lay->addWidget( backupScreen );

    // enable buttons
    enableMenus();

    // disable my button
    ui->actionBackup->setEnabled(false);

    if( backupScreen->isHidden() ) backupScreen->show();

    // fix window title
    setWindowTitle( QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion() + " - " + tr("System Backup") );
}

void MainWindow::goBrowser() {
    // close the dialogs
    closeDialogs();

    // get the mainWidget Layout
    QLayout *lay = ui->mainpageWidget->parentWidget()->layout();

    // load the url
    browserScreen->loadURL( "https://www.e-prescription.gr/e-pre/faces/Login" );

    // add listPatients to the layout
    lay->addWidget( browserScreen );

    // enable buttons
    enableMenus();

    // disable my button
    ui->actionE_syntagografisi->setEnabled(false);

    if( browserScreen->isHidden() ) browserScreen->show();

    // fix window title
    setWindowTitle( QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion() + " - " + tr("Browser (beta)") );
}

void MainWindow::closeDialogs() { // called before showing a screen
    // close or hide all the screens

    // main page
    s_instance->ui->mainpageWidget->hide();

    // new Patient
    // get the mainWidget Layout
    QLayout *lay = ui->mainpageWidget->parentWidget()->layout();

    // new Patient Form
    if ( newPatientForm != NULL ) {
	lay->removeWidget(newPatientForm);
	newPatientForm->hide();
    }

    //qDebug() << "closed half";

    // patient List
    if( !listPatientsScreen->isHidden() ) {
	lay->removeWidget( listPatientsScreen );
	listPatientsScreen->hide();
	qDebug() << "hide listpatient";
    }

    // calendar
    if( !bigCalendarScreen->isHidden() ) {
	lay->removeWidget( bigCalendarScreen );
	bigCalendarScreen->hide();
	qDebug() << "hide bigCalendarScreen";
    }

    // statistics
    if( !statisticsScreen->isHidden() ) {
	lay->removeWidget( statisticsScreen );
	statisticsScreen->hide();
	qDebug() << "hide statisticsScreen";
    }

    // settings
    if( !settingsScreen->isHidden() ) {
	lay->removeWidget( settingsScreen );
	settingsScreen->hide();
	qDebug() << "hide settingsScreen";
    }

    // backup
    if( !backupScreen->isHidden() ) {
	lay->removeWidget( backupScreen );
	backupScreen->hide();
	qDebug() << "hide backupScreen";
    }

    // e-syntagografisi
    if( !browserScreen->isHidden() ) {
	lay->removeWidget( browserScreen );
	browserScreen->hide();
	qDebug() << "hide browserScreen";
    }

    if( viewPatientScreen != NULL ) {
	lay->removeWidget( viewPatientScreen );
	viewPatientScreen->hide();
	qDebug() << "hide viewpatient";
    }

    //qDebug() << "closed all";

    // enable all the buttons
    enableMenus();
}

void MainWindow::enableMenus() {
    ui->actionHome->setEnabled(true);
    ui->actionNew->setEnabled(true);
    ui->actionList_Patients->setEnabled(true);
    ui->actionAppointments->setEnabled(true);
    ui->actionStatistics->setEnabled(true);
    ui->actionSettings->setEnabled(true);
    ui->actionBackup->setEnabled(true);
    ui->actionE_syntagografisi->setEnabled(true);

    //ui->actionLogout->setEnabled(true);
}

void MainWindow::disableMenus() {
    ui->actionHome->setEnabled(false);
    ui->actionNew->setEnabled(false);
    ui->actionList_Patients->setEnabled(false);
    ui->actionAppointments->setEnabled(false);
    ui->actionStatistics->setEnabled(false);
    ui->actionSettings->setEnabled(false);
    ui->actionLogout->setEnabled(false);
    ui->actionBackup->setEnabled(false);
    ui->actionE_syntagografisi->setEnabled(false);
}

void MainWindow::loadAppointmentsForDate(QString date) {

    // get appointments list
    QList<Appointment> appointments = DBConn::instance()->appointments(date);

    // clear window    
    QLayoutItem *child;
    while ((child = ui->scrollAreaWidgetContents->layout()->takeAt(0)) != 0) {
        delete child;
    }

    // go through that list
    for( int i = 0; i < appointments.size(); i++ ) {
	// Create Labels
	Appointment a = appointments.at(i);
	Patient p = DBConn::instance()->patient(a.patient_id);
        QTime t = QTime::fromString( a.ap_time, "HH:mm" );
        QLabel *appLabel = new QLabel( t.toString("HH:mm") + " - " + p.name + " " + p.surname );
	appLabel->setFont( QFont( "Tahoma", 9, 75, false) );
	appLabel->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
	QLabel *appLabel2 = new QLabel( a.ap_notes );
	appLabel2->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
	appLabel2->setWordWrap(true);
	// add to layout
        ui->scrollAreaWidgetContents->layout()->addWidget(appLabel);
        ui->scrollAreaWidgetContents->layout()->addWidget(appLabel2);
    }

    // in the end add verical spacer
   QLabel *l = new QLabel("");
   ui->scrollAreaWidgetContents->layout()->addWidget(l);

   // now calculate inside the month, what happens
   QDate from = QDate().fromString( QDate().currentDate().toString("yyyy-MM-01") );

   for( int i = 1; i <= from.daysInMonth(); i++ ) {
        QDate today = QDate().fromString( QDate().currentDate().toString("yyyy-MM-" + QString::number(i) ), "yyyy-MM-d" );

        if( !today.toString("yyyy").isEmpty() ) {
            QList<Appointment> apps = DBConn::instance()->appointments( today.toString("yyyy-MM-dd") );

            if( apps.size() > 0 ) {
                QTextCharFormat tf; tf.setFontWeight( QFont::Bold );

                if( s_osType == "WIN" ) tf.setFontPointSize( 11 );
                if( s_osType == "OSX" ) tf.setFontPointSize( 15 );

                ui->calendarWidget->setDateTextFormat( today, tf );
            }
        }
   }
}

void MainWindow::reloadPatientsList() {
    // reload patients list
    listPatientsScreen->reload();
}

void MainWindow::on_actionNew_triggered() {
    goAddPatient(0);
}

void MainWindow::on_actionQuit_triggered()
{
    // confirm first
    GeneralDialog *gd = new GeneralDialog("question", tr("Quit"), "", tr("Are you sure you wish to Quit ProMed?") );

    if( gd->exec() == QDialog::Accepted ) {
	exit(0);
    }
}

void MainWindow::testSlot() {
    goHome();
}

void MainWindow::on_actionHome_triggered()
{
    goHome();
}

void MainWindow::on_actionList_Patients_triggered()
{
    goPatientList(false);
}

void MainWindow::on_calendarWidget_selectionChanged()
{
    // load appointments for this day
    loadAppointmentsForDate( ui->calendarWidget->selectedDate().toString("yyyy-MM-dd") );
}

void MainWindow::on_actionAppointments_triggered()
{
    goCalendar();
}

void MainWindow::on_actionStatistics_triggered()
{
    goStatistics();
}

void MainWindow::on_actionSettings_triggered()
{
    goSettings();
}

void MainWindow::on_actionLogout_triggered()
{
    doLogout();
}

void MainWindow::on_actionAbout_triggered()
{
    //qDebug() << "ABOUT DIALOG";

    AboutDialog *ad = new AboutDialog(this, s_osType );
    ad->exec();
}

void MainWindow::updateClock() {
    clockLabel->setText( QDateTime().currentDateTime().toString("d MMM, yyyy HH:mm:ss") );
}

void MainWindow::on_actionBackup_triggered() {
    // show the backup screen
    goBackup();
}

void MainWindow::on_actionE_syntagografisi_triggered()
{
    goBrowser();
}

void MainWindow::on_actionFullscreen_toggled(bool is_set)
{
    QSettings settings;
    if( is_set ) {
        this->showFullScreen();
        settings.setValue( "fullscreen", 1 );
    } else {
        this->showNormal();
        settings.setValue( "fullscreen", 0 );
    }
}

QString MainWindow::getOSType() {
    return this->s_osType;
}
