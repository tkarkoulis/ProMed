#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QNetworkReply>
#include "widget_addpatient.h"
#include "widget_listpatients.h"
#include "widget_viewpatient.h"
#include "widget_bigcalendar.h"
#include "widget_statistics.h"
#include "widget_dentalsettings.h"
#include "widget_login.h"
#include "widget_backupui.h"
#include "widget_internalbrowser.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

// singleton
static MainWindow *s_instance;

public:
    explicit MainWindow(QWidget *parent = 0, QString osType = "WIN" );
    void goHome();
    void goPatientList(bool reload);
    void goViewPatient(int patID);
    void goAddPatient(int patID);
    void goCalendar();
    void goStatistics();
    void goSettings();
    void goBackup();
    void goBrowser();
    void doLogin();
    ListPatients *listPatientsScreen;
    void reloadPatientsList();

    // singleton instance
    static MainWindow *instance() {
	if( !s_instance ) {
	    s_instance = new MainWindow;
	}
	return s_instance;
    }


    QString getOSType();
    QString s_osType;

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    AddPatient *newPatientForm;
    BigCalendar *bigCalendarScreen;
    ViewPatient *viewPatientScreen;
    Statistics *statisticsScreen;
    DentalSettings *settingsScreen;
    BackupUI *backupScreen;
    InternalBrowser *browserScreen;
    LogIn *loginScreen;
    QLabel *clockLabel;
    void closeDialogs();
    void enableMenus();
    void disableMenus();
    void loadAppointmentsForDate( QString date );
    bool loggedIn;
    void doLogout();
    bool isDemo;

private slots:
    void on_actionFullscreen_toggled(bool is_set);
    void on_actionE_syntagografisi_triggered();
    void on_actionBackup_triggered();
    void on_actionAbout_triggered();
    void on_actionLogout_triggered();
    void on_actionSettings_triggered();
    void on_actionStatistics_triggered();
    void on_actionAppointments_triggered();
    void on_calendarWidget_selectionChanged();
    void on_actionList_Patients_triggered();
    void on_actionQuit_triggered();
    void on_actionNew_triggered();
    void on_actionHome_triggered();
    void updateClock();
    void checkUpdates();
    // network slots
    void checkLicenseVersionRequestFinished(QNetworkReply*);
    void checkAppVersionRequestFinished(QNetworkReply*);
    void openDownloadPage();

public slots:
    void testSlot();
};

#endif // MAINWINDOW_H
