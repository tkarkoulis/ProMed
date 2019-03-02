/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/20/2010 2010
**   All Rights Reserved
**************************************************************************/

//#include <QLayoutIterator>
#include <QCommandLinkButton>
#include <QScrollArea>

#include "ui_widget_bigcalendar.h"
#include "widget_bigcalendar.h"
#include "dbconn.h"
#include "mainwindow.h"
#include "obj_appointment.h"
#include "obj_patient.h"

BigCalendar::BigCalendar(QWidget *parent, QString os) :
    QWidget(parent),
    ui(new Ui::BigCalendar)
{
    ui->setupUi(this);

    // start with current Date
    selectedDate = QDate().currentDate();
    selectedDateWeek = QDate().currentDate();

    // set default month and year
    ui->cal_month->setCurrentIndex( selectedDate.month()-1 );
    ui->cal_year->setValue( selectedDate.year() );
    ui->monthLabel->setText( selectedDateWeek.toString("MMMM yyyy") );

    this->osType = os;

    // reload the calendar
    reloadMonth();
    reloadWeek();
}

void BigCalendar::clearLayout(QLayout *l) {
    // clear
    QLayoutItem *item;
    //QLayoutIterator it = l->iterator();

    while((item = l->takeAt(0)) != 0) {

	if (item->layout()) {
		clearLayout((QLayout *)item->layout());
	}
	ui->calendarDataLayout->removeWidget(item->widget());
	delete item->widget();
    }
}

void BigCalendar::createMonthHeaders() {
    // create month header
    QString headerStyle = "background:#e1e1e1; border-right:1px solid #eeeeee;border-bottom:1px solid #eeeeee;";

    QLabel *day1 = new QLabel( tr("Monday") );
    day1->setObjectName( "day1" );
    day1->setStyleSheet(headerStyle);
    day1->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    day1->setMinimumSize(50,50);
    day1->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->calendarDataLayout->addWidget(day1, 0, 1 );

    QLabel *day2 = new QLabel( tr("Tuesday") );
    day2->setObjectName( "day2" );
    day2->setStyleSheet(headerStyle);
    day2->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    day2->setMinimumSize(50,50);
    day2->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->calendarDataLayout->addWidget(day2, 0, 2 );

    QLabel *day3 = new QLabel( tr("Wednesday") );
    day3->setObjectName( "day3" );
    day3->setStyleSheet(headerStyle);
    day3->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    day3->setMinimumSize(50,50);
    day3->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->calendarDataLayout->addWidget(day3, 0, 3 );

    QLabel *day4 = new QLabel( tr("Thursday") );
    day4->setObjectName( "day4" );
    day4->setStyleSheet(headerStyle);
    day4->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    day4->setMinimumSize(50,50);
    day4->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->calendarDataLayout->addWidget(day4, 0, 4 );

    QLabel *day5 = new QLabel( tr("Friday") );
    day5->setObjectName( "day5" );
    day5->setStyleSheet(headerStyle);
    day5->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    day5->setMinimumSize(50,50);
    day5->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->calendarDataLayout->addWidget(day5, 0, 5 );

    QLabel *day6 = new QLabel( tr("Saturday") );
    day6->setObjectName( "day6" );
    day6->setStyleSheet(headerStyle);
    day6->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    day6->setMinimumSize(50,50);
    day6->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->calendarDataLayout->addWidget(day6, 0, 6 );

    QLabel *day7 = new QLabel( tr("Sunday") );
    day7->setObjectName( "day7" );
    day7->setStyleSheet(headerStyle);
    day7->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    day7->setMinimumSize(50,50);
    day7->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->calendarDataLayout->addWidget(day7, 0, 7 );
}

void BigCalendar::createWeekHeaders() {
    // create month header
    QString headerStyle = "background:#e1e1e1; border-right:1px solid #eeeeee;border-bottom:1px solid #eeeeee;";

    QLabel *day1 = new QLabel( tr("Monday") );
    day1->setObjectName( "day1" );
    day1->setStyleSheet(headerStyle);
    day1->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    day1->setMinimumSize(50,50);
    day1->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->calendarDataWeekLayout->addWidget(day1, 0, 1 );

    QLabel *day2 = new QLabel( tr("Tuesday") );
    day2->setObjectName( "day2" );
    day2->setStyleSheet(headerStyle);
    day2->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    day2->setMinimumSize(50,50);
    day2->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->calendarDataWeekLayout->addWidget(day2, 0, 2 );

    QLabel *day3 = new QLabel( tr("Wednesday") );
    day3->setObjectName( "day3" );
    day3->setStyleSheet(headerStyle);
    day3->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    day3->setMinimumSize(50,50);
    day3->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->calendarDataWeekLayout->addWidget(day3, 0, 3 );

    QLabel *day4 = new QLabel( tr("Thursday") );
    day4->setObjectName( "day4" );
    day4->setStyleSheet(headerStyle);
    day4->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    day4->setMinimumSize(50,50);
    day4->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->calendarDataWeekLayout->addWidget(day4, 0, 4 );

    QLabel *day5 = new QLabel( tr("Friday") );
    day5->setObjectName( "day5" );
    day5->setStyleSheet(headerStyle);
    day5->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    day5->setMinimumSize(50,50);
    day5->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->calendarDataWeekLayout->addWidget(day5, 0, 5 );

    QLabel *day6 = new QLabel( tr("Saturday") );
    day6->setObjectName( "day6" );
    day6->setStyleSheet(headerStyle);
    day6->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    day6->setMinimumSize(50,50);
    day6->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->calendarDataWeekLayout->addWidget(day6, 0, 6 );

    QLabel *day7 = new QLabel( tr("Sunday") );
    day7->setObjectName( "day7" );
    day7->setStyleSheet(headerStyle);
    day7->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    day7->setMinimumSize(50,50);
    day7->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->calendarDataWeekLayout->addWidget(day7, 0, 7 );
}

void BigCalendar::reloadWeek() {
    // clear week
    clearLayout( ui->calendarDataWeekLayout );

    createWeekHeaders();

    // create the week view
    QDate d = selectedDateWeek;

    int currentColumn = 1;

    QString thisMonthStyle = "background:#ffffff; color:#222222; padding:5px; border-right:1px solid #eeeeee;border-bottom:1px solid #eeeeee;";
    QString thisMonthStyleNoData = "background:#ffffff; color:#222222; border-bottom:1px solid #eeeeee;";
    QString thisMonthStyleWithData = "QLabel{ background:#ffffff; color:#222222;}";
    QString thisMonthStyleDate = "background:#ffffff; color:#222222; padding-left:5px; padding-right:5px; padding-bottom:0px; border:0px;";
    QString thisMonthStyleAppData = "QLabel{ font-family: Tahoma; background:#ffffff; padding-left:8px; border:0px; padding-right:5px; padding-top:0px; color:blue;}";
    QString thisMonthHeaderStyle = "color:#666666; padding:5px; padding-bottom:0px; border-right:1px solid #eeeeee;";
    QString otherMonthStyle = "background:#f5f5f5; color: #666666; padding:5px; border-right:1px solid #eeeeee;border-bottom:1px solid #eeeeee;";
    QString scrollAreaStyle = ".QWidget { border-right:1px solid #eeeeee; border-bottom:1px solid #eeeeee; } QScrollArea QWidget QScrollBar, QScrollArea QScrollBar { background:none; }";

    for( int i = 1; i <= 7; i++ ) {
        // get the date of this day
        d = d.addDays( i-d.dayOfWeek() );

        // create a QWidget
        QWidget *dayWrapper = new QWidget();
        dayWrapper->setObjectName( "dayWrapper" );
        // set its layout to vertical
        dayWrapper->setLayout( new QVBoxLayout() );
        // fix margins
        dayWrapper->layout()->setContentsMargins(0,0,0,0);
        // fix spacing
        dayWrapper->layout()->setSpacing(0);

        // create the number QLabel
        QLabel *dayHeader = new QLabel( d.toString("dd/MM/yyyy") );
        // set Font
        dayHeader->setFont( QFont("Tahoma", 8) );
        // set stylesheet
        dayHeader->setStyleSheet(thisMonthStyle);
        // set size expand at FIXED
        dayHeader->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );

        // add to wrapper layout
        dayWrapper->layout()->addWidget(dayHeader);

        QWidget *scrollWrapper = new QWidget();
        scrollWrapper->setObjectName( "scrollWrapper" );
        scrollWrapper->setLayout( new QVBoxLayout() );
        scrollWrapper->layout()->setContentsMargins(0,0,0,0);
        scrollWrapper->setStyleSheet( scrollAreaStyle );

        dayWrapper->layout()->addWidget( scrollWrapper );

        // create the ScrollArea
        QScrollArea *sa = new QScrollArea();
        // set scroll area layout
        sa->setLayout( new QVBoxLayout() );
        sa->setFrameStyle( QFrame::NoFrame );
        sa->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
        sa->layout()->setContentsMargins(0,0,10,0);
        sa->setWidgetResizable(true);

        scrollWrapper->layout()->addWidget(sa);

        // set text inside
        // get appointments for this date
        QList<Appointment> appointments = DBConn::instance()->appointments( d.toString("yyyy-MM-dd") );
        // check cell integrity
        if( appointments.size() <= 0 ) {
            QWidget *tmp = new QWidget();
            tmp->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
            sa->setWidget( tmp );
        } else {
            // create the QWidget that will hold the data
            QWidget *day = new QWidget();
            // set size policy
            day->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
            // set the layout
            day->setLayout( new QVBoxLayout() );
            // set no padding
            day->layout()->setContentsMargins(1, 1, 1, 1);
            day->layout()->setSpacing(0);
            // set font
            day->setFont( QFont("Tahoma", 8) );
            // set stylesheet
            day->setStyleSheet(thisMonthStyleWithData);

            // add data
            for( int z = 0; z < appointments.size(); z++ ) {

                // create the date QLabel
                QLabel *date = new QLabel();
                // set size expand at 100%
                date->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );
                // set content alignment
                date->setAlignment( Qt::AlignTop );
                // set Font
                date->setFont( QFont("Tahoma", 8) );
                // set stylesheet
                date->setStyleSheet(thisMonthStyleDate);

                Appointment a = appointments.at(z); // get appointment
                Patient p = DBConn::instance()->patient(a.patient_id); // get patient
                // add 1st line (time)
                date->setText( QTime::fromString(a.ap_time, "HH:mm").toString("HH:mm") );

                // create the data QPushButton
                QLabel *app_data = new QLabel();
                // set size expand at 100%
                app_data->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
                // set content alignment
                app_data->setAlignment( Qt::AlignTop );
                // set Font
                app_data->setFont( QFont("Tahoma", 8) );
                // set stylesheet
                app_data->setStyleSheet(thisMonthStyleAppData);
                // set mouse
                app_data->setCursor( Qt::PointingHandCursor );
                // set value of patientID
                //app_data->setProperty( "patient_id", a.patient_id );

                // CONNECT TO SLOT
                connect( app_data, SIGNAL(linkActivated(QString)), this, SLOT(app_clicked(QString)) );

                // add line (name)
                app_data->setText( "<a href='" + QString::number( a.patient_id ) + "' style='text-decoration:none;'>" + p.name + " " + p.surname + "</a>" );
                app_data->setToolTip( QTime::fromString(a.ap_time, "HH:mm:ss a").toString("HH:mm a") + "\n" + p.name + " " + p.surname );
                app_data->setToolTip( app_data->toolTip() + "\n " + a.ap_notes + "\n" );

                // add to qwidget
                day->layout()->addWidget( date );
                day->layout()->addWidget( app_data );
            }

            // add last widget for expansion reasons
            QLabel *placeholder = new QLabel();
            placeholder->setStyleSheet("border:0px;");
            day->layout()->addWidget( placeholder );

            // add to general box
            sa->setWidget(day);
        }

        // add to main Layout
        ui->calendarDataWeekLayout->addWidget(dayWrapper, 1, currentColumn);

        // go to next column
        currentColumn++;
    }
}

void BigCalendar::reloadMonth() {
    // clear calendar
    clearLayout( ui->calendarDataLayout );

    createMonthHeaders();

    // create the month view
    QDate d = selectedDate;
    // get the 1st of month date
    d = QDate( d.year(), d.month(), 1 );

    int currentRow = 1;
    int currentColumn = 1;

    QString smallDayStyle = "";

    if( this->osType == "OSX" )
        smallDayStyle = "font-size: 11pt;";
    else
        smallDayStyle = "font-size: 8pt;";

    QString thisMonthStyle = "background:#ffffff; color:#222222; padding:5px; border-right:1px solid #eeeeee;border-bottom:1px solid #eeeeee;";
    QString thisMonthStyleNoData = "background:#ffffff; color:#222222; border-right:1px solid #eeeeee;border-bottom:1px solid #eeeeee;";
    QString thisMonthStyleWithData = "background:#FFFFd0; color:#222222; border-right:1px solid #eeeeee;border-bottom:1px solid #eeeeee;";
    QString thisMonthStyleDate = "background:none; color:#222222; padding-left:2px; padding-right:5px; padding-bottom:0px; border:0px;";
    QString thisMonthStyleAppData = "QLabel{ background:none; padding-left:8px; border:0px; padding-right:5px; padding-top:0px; color:blue;}";
    QString otherMonthStyle = "background:#f5f5f5; color: #666666; padding:5px; border-right:1px solid #eeeeee;border-bottom:1px solid #eeeeee;";
    QString thisMonthHeaderStyleWithData = "background:#FFFFd0;color:#666666; padding:5px; padding-bottom:0px; border-right:1px solid #eeeeee;" + smallDayStyle;
    QString thisMonthHeaderStyleNoData = "color:#666666; padding:5px; padding-bottom:0px; border-right:1px solid #eeeeee;" + smallDayStyle;

    if( d.dayOfWeek() > 1 ) {
	// fix days before 1st of month
	for( int i = 1; i < d.dayOfWeek(); i++ ) {
	    // create a QLabel
	    QLabel *day = new QLabel();
	    // set size ot expand at 100%
	    day->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
	    // set content alignment
	    day->setAlignment( Qt::AlignTop );
	    // set Font
	    day->setFont( QFont("Tahoma", 8) );
	    // set stylesheet
            day->setStyleSheet( otherMonthStyle + smallDayStyle );

	    // set text inside
	    day->setText( d.addDays(i-d.dayOfWeek()).toString("dd") );

	    // add to main Layout
	    ui->calendarDataLayout->addWidget(day, currentRow, i );

	    // update flag
	    currentColumn = i;
	}
    }

    // go to next column
    currentColumn++;

    for( int i = 1; i <= d.daysInMonth(); i++ ) {
	// get the date of this day
	d = QDate( d.year(), d.month(), i );
	// create a QWidget
	QWidget *dayWrapper = new QWidget();
	// set its layout to vertical
	dayWrapper->setLayout( new QVBoxLayout() );
	// fix margins
	dayWrapper->layout()->setContentsMargins(0,0,0,0);
	// fix spacing
	dayWrapper->layout()->setSpacing(0);
	// create the number QLabel
	QLabel *dayHeader = new QLabel( d.toString("dd") );
	// set Font
	dayHeader->setFont( QFont("Tahoma", 8) );
	// set stylesheet
        dayHeader->setStyleSheet( thisMonthHeaderStyleNoData );
	// set size expand at FIXED
	dayHeader->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );

	// add to wrapper layout
	dayWrapper->layout()->addWidget(dayHeader);

	// set text inside
	// get appointments for this date
	QList<Appointment> appointments = DBConn::instance()->appointments( d.toString("yyyy-MM-dd") );
	// check cell integrity
	if( appointments.size() <= 0 ) {
	    QLabel *tmp = new QLabel();
            tmp->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
	    tmp->setStyleSheet(thisMonthStyleNoData);
	    dayWrapper->layout()->addWidget( tmp );
	} else {

            // our day isnt empty, so add the stylesheet for background-color.
            dayHeader->setStyleSheet( thisMonthHeaderStyleWithData );

	    // create the QWidget that will hold the data
	    QWidget *day = new QWidget();
	    // set size policy
	    day->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
	    // set the layout
	    day->setLayout( new QVBoxLayout() );
	    // set no padding
	    day->layout()->setContentsMargins(1, 1, 1, 1);
	    day->layout()->setSpacing(0);
	    // set font
	    day->setFont( QFont("Tahoma", 8) );
	    // set stylesheet
            day->setStyleSheet(thisMonthStyleWithData + smallDayStyle);

            QLabel *day_data = new QLabel();
            day_data->setAlignment( Qt::AlignVCenter | Qt::AlignCenter );
            // set stylesheet
            day_data->setStyleSheet(thisMonthStyleDate + smallDayStyle);
            day_data->setText( "\n  " + QString::number( appointments.size() ) + " " + tr("appointments.") );

            QLabel *day_more = new QLabel();
            day_more->setAlignment( Qt::AlignRight | Qt::AlignBottom );
            day_more->setStyleSheet(thisMonthStyleDate + smallDayStyle);
            day_more->setText( "<a href='" + d.toString("yyyy-MM-dd") + "' style='text-decoration:none;'>" + tr("view") );
            connect( day_more, SIGNAL(linkActivated(QString)) , this, SLOT(day_clicked(QString)) );

            day->layout()->addWidget(day_data);
            day->layout()->addWidget(day_more);

            /*
	    // add data
	    for( int z = 0; z < appointments.size(); z++ ) {

		// create the date QLabel
		QLabel *date = new QLabel();
		// set size expand at 100%
		date->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );
		// set content alignment
		date->setAlignment( Qt::AlignTop );
		// set Font
		date->setFont( QFont("Tahoma", 8) );
		// set stylesheet
                date->setStyleSheet(thisMonthStyleDate + smallDayStyle);

		Appointment a = appointments.at(z); // get appointment
		Patient p = DBConn::instance()->patient(a.patient_id); // get patient
		// add 1st line (time)
		date->setText( QTime::fromString(a.ap_time, "HH:mm:ss a").toString("HH:mm a") );

		// create the data QPushButton
                QLabel *app_data = new QLabel();
		// set size expand at 100%
                app_data->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
                // set alignment
                app_data->setAlignment( Qt::AlignTop );
		// set Font
		app_data->setFont( QFont("Tahoma", 8) );
		// set stylesheet
                app_data->setStyleSheet(thisMonthStyleAppData + smallDayStyle );
		// set mouse
		app_data->setCursor( Qt::PointingHandCursor );

		// CONNECT TO SLOT
                connect( app_data, SIGNAL(linkActivated(QString)) , this, SLOT(app_clicked(QString)) );

		// add line (name)
                app_data->setText( "<a href='" + QString::number( a.patient_id ) + "' style='text-decoration:none;'>" + p.name + " " + p.surname + "</a>" );
		app_data->setToolTip( QTime::fromString(a.ap_time, "HH:mm:ss a").toString("HH:mm a") + "\n" + p.name + " " + p.surname );
		app_data->setToolTip( app_data->toolTip() + "\n " + a.ap_notes + "\n" );

		// add to qwidget
                day->layout()->addWidget( date );
		day->layout()->addWidget( app_data );
                day->layout()->setAlignment( app_data, Qt::AlignTop );
            }*/

	    // add to general box
	    dayWrapper->layout()->addWidget(day);
	}

	// add to main Layout
	ui->calendarDataLayout->addWidget(dayWrapper, currentRow, currentColumn);

	if( currentColumn == 7 ) {
	    currentColumn = 1;
	    currentRow++;
	} else currentColumn++;
    }
}

BigCalendar::~BigCalendar()
{
    delete ui;
}

void BigCalendar::on_cal_previous_clicked() {
    // remove 1 month
    selectedDate = selectedDate.addMonths(-1);

    // set default month and year
    ui->cal_month->setCurrentIndex( selectedDate.month()-1 );
    ui->cal_year->setValue( selectedDate.year() );

    // reload table
    reloadMonth();
}

void BigCalendar::on_cal_next_clicked() {
    // remove 1 month
    selectedDate = selectedDate.addMonths(1);

    // set default month and year
    ui->cal_month->setCurrentIndex( selectedDate.month()-1 );
    ui->cal_year->setValue( selectedDate.year() );

    // reload table
    reloadMonth();
}

void BigCalendar::on_cal_month_currentIndexChanged(int index)
{
    // set the month
    selectedDate.setDate( selectedDate.year(), (index+1), selectedDate.day() );

    // reload
    reloadMonth();
}

void BigCalendar::on_cal_year_valueChanged(int index)
{
    // set the month
    selectedDate.setDate( index, selectedDate.month(), selectedDate.day() );

    // reload
    reloadMonth();
}

void BigCalendar::app_clicked( QString link) {
    // get the property
    int pat_id = link.toInt();
    // load the patient window
    MainWindow::instance()->goViewPatient( pat_id );
}

void BigCalendar::day_clicked(QString link) {
    // get the property
    selectedDateWeek = QDate::fromString( link, "yyyy-MM-dd" );
    // reload the week
    reloadWeek();
    // load the patient window
    ui->tabWidget->setCurrentIndex(1);
}

void BigCalendar::on_toolButton_clicked()
{
    selectedDateWeek = selectedDateWeek.addDays(-7);

    ui->monthLabel->setText( selectedDateWeek.toString("MMMM yyyy") );

    reloadWeek();
}

void BigCalendar::on_toolButton_2_clicked()
{
    selectedDateWeek = selectedDateWeek.addDays(7);

    ui->monthLabel->setText( selectedDateWeek.toString("MMMM yyyy") );

    reloadWeek();
}
