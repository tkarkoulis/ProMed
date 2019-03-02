/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/21/2010 2010
**   All Rights Reserved
**************************************************************************/

#include "widget_statistics.h"
#include "mainwindow.h"
#include "obj_patient.h"
#include "ui_widget_statistics.h"
#include "dbconn.h"
#include <QDebug>
#include <QToolButton>
#include <QBoxLayout>
#include <QLayout>
#include <QTabWidget>
#include <QStringList>
#include <QComboBox>
#include <QLabel>

Statistics::Statistics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Statistics)
{
    ui->setupUi(this);

    ui->w_monthTop->setStyleSheet( "QLabel { font-size: 10px; } " );

    app_date = QDate().currentDate();
    op_date = QDate().currentDate();

    ui->btn_next->setLayoutDirection(Qt::RightToLeft);
    ui->btn_opNext->setLayoutDirection(Qt::RightToLeft);

    ui->tabWidget->setTabText(0, tr("Appointments") );
    ui->tabWidget->setTabText(1, tr("Operations") );
    ui->tabWidget->setTabText(2, tr("Demographic") );
    //ui->tabWidget->setTabText(3, tr("Economic") );

    ui->tbl_operationsNames->header()->resizeSection(0, 5);
    ui->tbl_demoData->header()->resizeSection(0,5);

    reloadAppointments();
    reloadOperations();
}

void Statistics::reloadAppointments() {

    // replace month text
    ui->label_top_month->setText( app_date.toString("MMMM") );

    // need to clear the layout.

    QLayoutItem *child;
    while ((child = ui->w_monthChartValues->layout()->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }

    QLayoutItem *child3;
    while ((child3 = ui->w_monthBottom->layout()->takeAt(0)) != 0) {
        delete child3->widget();
        delete child3;
    }

    QLayoutItem *child2;
    while ((child2 = ui->w_monthChartHValues->layout()->takeAt(0)) != 0) {
        delete child2;
    }


    int max = DBConn::instance()->maxAppointmentsPerDayPerMonth( app_date.toString("yyyy-MM-dd") );

    QLabel *lbl_app = new QLabel("a\np\np\no\ni\nn\nt\nm\ne\nn\nt\ns");
    lbl_app->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    ui->w_monthChartHValues->layout()->addWidget( lbl_app );

    int itemHeight = 0;

    if( max > 0 ) itemHeight = 245 / max;

    ui->w_monthChartVValues->layout()->addWidget( new QLabel("") );

    ui->w_monthChartValues->layout()->setSpacing(0);

    for( int i = 1; i <= app_date.daysInMonth(); i++ ) {

        int today = DBConn::instance()->appointmentsInDay( QDate(app_date.year(), app_date.month(), i).toString("yyyy-MM-dd") );

        QLabel *ln = new QLabel( QString::number(i) );
        ln->setAlignment( Qt::AlignHCenter );
        ln->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );
        ln->setStyleSheet( "border-top:1px solid #333333;" );

        // create test data
        QWidget *w = new QWidget();
        w->setLayout( new QVBoxLayout() );
        w->setContentsMargins(0,0,0,0);
        w->layout()->setContentsMargins( 0,0,0,0 );
        w->layout()->setSpacing( 0 );
        w->layout()->setMargin(0);
        w->setStyleSheet( "background:none;" );

        QLabel *l = new QLabel();

        if( today > 0 )
            l->setText( QString::number( today ) );
        else
            l->setText( "" );

        l->setAlignment( Qt::AlignHCenter );
        l->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );

        QWidget *tb = new QWidget();
        tb->setStyleSheet( "border:1px solid #ffffff; background: #fb" + QString().sprintf("%02d", 99-(3*today)  ) + "00;" );
        tb->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );

        tb->setMinimumHeight( itemHeight*today  );

        w->layout()->addWidget( new QWidget(w) );
        w->layout()->addWidget(l);
        w->layout()->addWidget(tb);
        w->layout()->addWidget(ln);

        ui->w_monthChartValues->layout()->addWidget( w );
    }

    ui->w_monthChartValues->layout()->addWidget( new QWidget() );


    // now fix the months chart in the bottom of the page
    for( int m = 1; m <= 12; m++ ) {
        // make the date
        QDate d = QDate( app_date.year(), m, 1 );

        int maxInMonth = DBConn::instance()->appointmentsInMonth( d );

        QLabel *ln = new QLabel( d.toString("MMM") );
        ln->setAlignment( Qt::AlignHCenter );
        ln->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );
        //ln->setStyleSheet( "border-top:1px solid #333333;" );

        // create data
        QWidget *w = new QWidget();
        w->setLayout( new QVBoxLayout() );
        w->setContentsMargins(0,0,0,0);
        w->layout()->setContentsMargins( 0,0,0,0 );
        w->layout()->setSpacing( 0 );
        w->layout()->setMargin(0);
        w->setStyleSheet( "background:none;" );

        QLabel *l = new QLabel();

        if( maxInMonth > 0 )
            l->setText( QString::number( maxInMonth ) );
        else
            l->setText( "" );

        l->setAlignment( Qt::AlignHCenter );
        l->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );

        QWidget *tb = new QWidget();
        tb->setStyleSheet( "border:0px solid #ffffff; background: #fb" + QString().sprintf("%02d", 99-(3*maxInMonth)  ) + "00;" );
        tb->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );

        tb->setMinimumHeight( 2*maxInMonth  );

        w->layout()->addWidget( new QWidget(w) );
        w->layout()->addWidget(l);
        w->layout()->addWidget(tb);
        w->layout()->addWidget(ln);

        ui->w_monthBottom->layout()->addWidget( w );
    }
}

void Statistics::reloadOperations() {

    // set month
    ui->lbl_operationsMonth->setText( op_date.toString("MMMM") );

    // set number of operations
    ui->lbl_operationsNum->setText( QString::number(DBConn::instance()->operationsInMonth(op_date)) );

    // clear layouts
    QLayoutItem *child;
    while ((child = ui->w_mostFreqOpTypes->layout()->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }

    QLayoutItem *child3;
    while ((child3 = ui->w_opMonthBottom->layout()->takeAt(0)) != 0) {
        delete child3->widget();
        delete child3;
    }

    // clear the TreeWidget
    ui->tbl_operationsNames->clear();

    // get distinct operation types
    QStringList s_l = DBConn::instance()->operationTypes( op_date );

    for( int i = 0; i < s_l.size(); i++ ) {
        // create string
        QString s = (QString) s_l.at(i);

        // add the label
        QLabel *l = new QLabel();
        l->setWordWrap(true);
        l->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Minimum );
        l->setStyleSheet( "border-bottom:1px solid #aaaaaa; padding-bottom:3px;" );
        l->setText( s );

        ui->w_mostFreqOpTypes->layout()->addWidget(l);
    }

    QLabel *lbl = new QLabel();
    lbl->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Expanding );
    ui->w_mostFreqOpTypes->layout()->addWidget( lbl );

    // now lets fix the patients table for this month
    QList<Patient> patients = DBConn::instance()->operationPatients( QDate( op_date.year(), op_date.month(), 1 ).toString("yyyy-MM-dd"), QDate( op_date.year(), op_date.month(), op_date.daysInMonth()).toString("yyyy-MM-dd") );

    // iterate through the list and get the patient names
    for( int i = 0; i < patients.size(); i++ ) {
        // get patient
        Patient p = patients.at(i);

        // add the patient in the table view
        QStringList row;
        row << QString::number(p.id) << p.name << p.surname << p.fathersName;
        QTreeWidgetItem* item = new QTreeWidgetItem( ui->tbl_operationsNames, row);
        item->setSizeHint(0, QSize(5, 25) );

        // add row to table
        ui->tbl_operationsNames->addTopLevelItem(item);
    }

    ui->tbl_operationsNames->sortByColumn( 1, Qt::AscendingOrder);

    // now fix the months chart in the bottom of the page
    for( int m = 1; m <= 12; m++ ) {
        // make the date
        QDate d = QDate( op_date.year(), m, 1 );

        int maxInMonth = DBConn::instance()->operationsInMonth( d );

        QLabel *ln = new QLabel( d.toString("MMM") );
        ln->setAlignment( Qt::AlignHCenter );
        ln->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );

        // create data
        QWidget *w = new QWidget();
        w->setLayout( new QVBoxLayout() );
        w->setContentsMargins(0,0,0,0);
        w->layout()->setContentsMargins( 0,0,0,0 );
        w->layout()->setSpacing( 0 );
        w->layout()->setMargin(0);
        w->setStyleSheet( "background:none;" );

        QLabel *l = new QLabel();

        if( maxInMonth > 0 )
            l->setText( QString::number( maxInMonth ) );
        else
            l->setText( "" );

        l->setAlignment( Qt::AlignHCenter );
        l->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );

        QWidget *tb = new QWidget();
        tb->setStyleSheet( "border:0px solid #ffffff; background: #fb" + QString().sprintf("%02d", 99-(3*maxInMonth)  ) + "00;" );
        tb->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );

        tb->setMinimumHeight( 2*maxInMonth  );

        w->layout()->addWidget( new QWidget(w) );
        w->layout()->addWidget(l);
        w->layout()->addWidget(tb);
        w->layout()->addWidget(ln);

        ui->w_opMonthBottom->layout()->addWidget( w );
    }
}

void Statistics::reloadDemographic() {
    // we get the values of all the demoXXXX fields and we add to the query;

    // clear the table
    ui->tbl_demoData->clear();

    // prepare birthdate and others
    QString birthdate = "";
    QString gender = "";
    QString insurance = "";

    if( ui->spin_demoYear->value() > 0 ) birthdate += QString::number(ui->spin_demoYear->value());
    if( ui->combo_demoMonth->currentIndex() > 0 ) birthdate += "-" + QString().sprintf("%02d", ui->combo_demoMonth->currentIndex() ) + "-";
    if( ui->combo_demoDay->currentIndex() > 0 ) birthdate += QString().sprintf("%02d", ui->combo_demoDay->currentIndex() );

    if( ui->combo_demoGender->currentIndex() > 0 ) gender = ui->combo_demoGender->currentText();
    if( ui->combo_demoInsurance->currentIndex() > 0 ) insurance = ui->combo_demoInsurance->currentText();

    QList<Patient> p_found = DBConn::instance()->patientsFromDemo( ui->txt_demoName->text(), ui->txt_demoSurname->text(), gender,
                                                                   insurance, ui->txt_demoCity->text(), birthdate );

    for( int i = 0; i < p_found.size(); i++ ) {
        // get Patient
        Patient p = p_found.at(i);

        // add the patient in the table view
        QStringList row;
        row << QString::number(p.id) << p.name << p.surname << p.fathersName << p.gender << QDate().fromString( p.birthdate, "yyyy-MM-dd").toString( "dd MMM, yyyy" ) ;
        QTreeWidgetItem* item = new QTreeWidgetItem( ui->tbl_demoData, row);
        item->setSizeHint(0, QSize(5, 25) );

        // add row to table
        ui->tbl_demoData->addTopLevelItem(item);
    }

    ui->tbl_demoData->sortByColumn( 1, Qt::AscendingOrder);
}

Statistics::~Statistics()
{
    delete ui;
}

void Statistics::on_btn_refresh_clicked()
{
    reloadAppointments();
}

void Statistics::on_btn_previous_clicked()
{
    app_date = app_date.addMonths(-1);

    reloadAppointments();
}

void Statistics::on_btn_next_clicked()
{
    app_date = app_date.addMonths(1);

    reloadAppointments();
}

void Statistics::on_btn_opRefresh_clicked()
{
    reloadOperations();
}

void Statistics::on_tbl_operationsNames_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
    // ok lets get the item's ID value
    QString patID = item->text(0);

    MainWindow::instance()->goViewPatient( patID.toInt() );
}

void Statistics::on_btn_opPrevious_clicked()
{
    op_date = op_date.addMonths(-1);

    reloadOperations();
}

void Statistics::on_btn_opNext_clicked()
{
    op_date = op_date.addMonths(1);

    reloadOperations();
}

void Statistics::on_btn_demoSearch_clicked()
{
    reloadDemographic();
}

void Statistics::on_txt_demoSurname_returnPressed()
{
    reloadDemographic();
}

void Statistics::on_txt_demoName_returnPressed()
{
    reloadDemographic();
}

void Statistics::on_txt_demoCity_returnPressed()
{
    reloadDemographic();
}

void Statistics::on_spin_demoYear_editingFinished()
{
    reloadDemographic();
}

void Statistics::on_tbl_demoData_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
    // ok lets get the item's ID value
    QString patID = item->text(0);

    MainWindow::instance()->goViewPatient( patID.toInt() );
}
