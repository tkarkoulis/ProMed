/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/13/2010 2010
**   All Rights Reserved
**************************************************************************/

#include <QPushButton>
#include <QDate>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QSettings>
#include <time.h>
#include "widget_listpatients.h"
#include "dialog_generaldialog.h"
#include "dialog_printdialog.h"
#include "ui_widget_listpatients.h"
#include "dbconn.h"
#include "mainwindow.h"

ListPatients::ListPatients(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListPatients)
{
    ui->setupUi(this);

    // hide myself
    this->hide();

    // fix the buttons at the bottom
    ui->horizontalLayout->setAlignment( Qt::AlignLeft );

    // set basic data for tree widget
    ui->treeWidget->setColumnCount(10);
    ui->treeWidget->setAlternatingRowColors(true);

    // create table header
    QHeaderView *hv = new QHeaderView(Qt::Horizontal);
    hv->setDefaultAlignment( Qt::AlignCenter );
    //hv->setResizeMode( QHeaderView::Interactive );
    hv->setStretchLastSection(true);
    // set Header
    ui->treeWidget->setHeader(hv);

    QStringList columns;
    columns << "" << "ID" <<  tr("Surname") <<  tr("Name") <<
         tr("Father's Name") <<  tr("Birthdate") << tr("Age") <<  tr("Tel. 1") <<
         tr("Insurance") <<  tr("VAT #") <<  tr("Cancer Trace") << tr("Children") <<  tr("Nationality")  << tr("Inheritance") <<  "";
    ui->treeWidget->setHeaderLabels(columns);

    // resize sections
    hv->resizeSection(0, 5);
    hv->resizeSection(1, 50);
    hv->resizeSection(2, 150);
    hv->resizeSection(9, 5);

    ui->treeWidget->header()->setSortIndicatorShown(true);
    //ui->treeWidget->header()->setClickable(true);

    connect( ui->treeWidget->header(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(treeWidget_sortChanged(int,Qt::SortOrder)) );
    connect( ui->treeWidget->header(), SIGNAL(sectionClicked(int)), this, SLOT(treeWidget_headerClicked(int)) );

    // load first time
    reload();
    // sort first time
    ui->treeWidget->sortByColumn( 2, Qt::AscendingOrder);


}

void ListPatients::reload() {
    // clear the table
    ui->treeWidget->clear();
    ui->treeWidget->sortByColumn( ui->treeWidget->header()->sortIndicatorSection(), Qt::AscendingOrder);

    ui->filter_nationality->clear();
     ui->filter_nationality->addItems(DBConn::instance()->get_nationalities());
    // reload
    loadPatientsfromDB();
}

void ListPatients::loadPatientsfromDB() {
    // load the patients from the database
    QList<Patient> patients = DBConn::instance()->patients();

    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);


    // loop
    for (int i = 0; i < patients.size(); ++i) {
        Patient tmp_p = patients.at(i);

        // fix date
        QDate d = QDate::fromString( tmp_p.birthdate, "yyyy-MM-dd" );

        // fix telephone 1
        QString tmp_telephone = "";
        if( !tmp_p.tel1.isEmpty() ) {
            // not empty tel
            tmp_telephone = tmp_p.tel1 + " (" + tmp_p.tel1_type + ")";
        }

        //calculate the age of the patient
        int current_year = aTime->tm_year + 1900;
        int age = current_year - d.year();
        QString age_string =  QString::number(age);

        // create table row
        QStringList row;
        row << "" << QString::number(tmp_p.id) << tmp_p.surname << tmp_p.name << tmp_p.fathersName <<d.toString("MMM dd, yyyy")  << age_string << tmp_telephone
            << tmp_p.insurance << tmp_p.vat << tmp_p.cancer_trace << tmp_p.children << tmp_p.nationality << tmp_p.inheritance ;

        QTreeWidgetItem* item = new QTreeWidgetItem( ui->treeWidget, row);

        // fix size
        item->setSizeHint(0, QSize(5, 25) );
        // fix alignment
        item->setTextAlignment(1, Qt::AlignCenter);
        item->setTextAlignment(6, Qt::AlignCenter);
        // add row to table
        ui->treeWidget->addTopLevelItem(item);
    }

    // sort first time
    ui->treeWidget->sortByColumn( 2, Qt::AscendingOrder);
}

void ListPatients::loadPatientsfromList(QList<Patient> patients) {

    // clear the table every time
    ui->treeWidget->clear();

    // loop
    for (int i = 0; i < patients.size(); ++i) {
	Patient tmp_p = patients.at(i);

	// fix date
	QDate d = QDate::fromString( tmp_p.birthdate, "yyyy-MM-dd" );

	// create table row
	QStringList row;
    row << "" << QString::number(tmp_p.id) << tmp_p.surname << tmp_p.name << tmp_p.fathersName << d.toString("MMM dd, yyyy") << tmp_p.tel1 + " (" + tmp_p.tel1_type + ")"
        << tmp_p.insurance << tmp_p.vat  << tmp_p.cancer_trace << tmp_p.children << tmp_p.nationality << tmp_p.inheritance;

	QTreeWidgetItem* item = new QTreeWidgetItem( ui->treeWidget, row);
	// fix size
	item->setSizeHint(0, QSize(5, 25) );
	// fix alignment
	item->setTextAlignment(1, Qt::AlignCenter);
	item->setTextAlignment(6, Qt::AlignCenter);
	// add row to table
	ui->treeWidget->addTopLevelItem(item);
    }
}

void ListPatients::on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column) {
    // ok lets get the item's ID value
    QString patID = item->text(1);

    MainWindow::instance()->goViewPatient( patID.toInt() );
}

void ListPatients::treeWidget_sortChanged( int itemIndex, Qt::SortOrder order ) {
    // sort
    sortOrder = order;
}

void ListPatients::treeWidget_headerClicked( int itemIndex ) {
    // sort
    ui->treeWidget->sortByColumn(itemIndex, sortOrder );
}

ListPatients::~ListPatients()
{
    delete ui;
}

void ListPatients::on_filter_name_textChanged(QString ) {

   ListPatients:filter_items();
}

void ListPatients::hide_items(QList<QTreeWidgetItem *> found_items)
{

    for( int i = 0; i < ui->treeWidget->topLevelItemCount(); i++ ) {
        QTreeWidgetItem *it = ui->treeWidget->topLevelItem(i);

        if(it->isHidden())
            continue;

        if( !found_items.contains( it ) ){
            it->setHidden(true);
        }
        else{
            it->setHidden(false);
        }

    }
}

void ListPatients::filter_items(){
    QList<QTreeWidgetItem *> found_items;


    /* initialize items to be visible */
    for( int i = 0; i < ui->treeWidget->topLevelItemCount(); i++ ) {
        QTreeWidgetItem *it = ui->treeWidget->topLevelItem(i);
        it->setHidden(false);
    }

// qDebug()<<"--------------------"<<endl;

    /* surname */
    /* run the filter only if it is not empty */
    if( !ui->filter_name->text().isEmpty()){
        QList<QTreeWidgetItem *> found_items =  ui->treeWidget->findItems( ui->filter_name->text(), Qt::MatchStartsWith, 2 );
        hide_items(found_items);
        found_items.clear();
    }

    /* fathers name */
    if( !ui->filter_fathersName->text().isEmpty()){
        found_items =  ui->treeWidget->findItems( ui->filter_fathersName->text(), Qt::MatchStartsWith, 2 );
        hide_items(found_items);
        found_items.clear();
    }

    /* surname */
    if( !ui->filter_surname->text().isEmpty()){
       found_items =  ui->treeWidget->findItems( ui->filter_surname->text(), Qt::MatchStartsWith, 2 );
        hide_items(found_items);
        found_items.clear();
    }

    /* children */
    if( !ui->filter_children->currentText().isEmpty()){
        found_items =  ui->treeWidget->findItems( ui->filter_children->currentText(), Qt::MatchStartsWith, 11 );
        hide_items(found_items);
        found_items.clear();
    }

    /* telephone  */
    if( !ui->filter_telephone->text().isEmpty()){
        found_items =  ui->treeWidget->findItems( ui->filter_telephone->text(), Qt::MatchStartsWith, 6 );
        hide_items(found_items);
        found_items.clear();
    }



    /* cancer trace */
    if( !ui->filter_cancer->currentText().isEmpty()){
        found_items =  ui->treeWidget->findItems( ui->filter_cancer->currentText(), Qt::MatchStartsWith, 10 );
        hide_items(found_items);
        found_items.clear();
    }

    /* nationality */
    if( !ui->filter_nationality->currentText().isEmpty()){
         found_items =  ui->treeWidget->findItems( ui->filter_nationality->currentText(), Qt::MatchStartsWith, 12 );
        hide_items(found_items);
        found_items.clear();
    }

    /*age */
    if( !ui->filter_age->text().isEmpty()){
        found_items =  ui->treeWidget->findItems( ui->filter_age->text(), Qt::MatchStartsWith, 6 );
        hide_items(found_items);
        found_items.clear();
    }


    /* inheritance */

    if( !ui->filter_inheritance->currentText().isEmpty()){
        found_items =  ui->treeWidget->findItems( ui->filter_inheritance->currentText(), Qt::MatchStartsWith, 13 );
        hide_items(found_items);
        found_items.clear();
    }

}



void ListPatients::on_filter_surname_textChanged(QString ) {
    ListPatients:filter_items();
}

void ListPatients::on_filter_fathersName_textChanged(QString ) {
   ListPatients:filter_items();
}

void ListPatients::on_filter_telephone_textChanged(QString ) {
   ListPatients:filter_items();
}

void ListPatients::on_filter_children_currentIndexChanged(int index)
{
 ListPatients:filter_items();
}

void ListPatients::on_filter_cancer_currentIndexChanged(int index)
{
     ListPatients:filter_items();
}

void ListPatients::on_filter_nationality_currentIndexChanged(const QString &arg1)
{
     ListPatients:filter_items();
}


void ListPatients::on_filter_age_textChanged(const QString &arg1)
{
     ListPatients:filter_items();
}

void ListPatients::on_filter_inheritance_currentIndexChanged(int index)
{
    ListPatients:filter_items();
}


void ListPatients::on_pushButton_clicked() {
    // save as excel

    // get list of patients first (IF NO PATIENTS, ECHO WARNING AND RETURN)
//    QList<Patient> patients = DBConn::instance()->patients();

    QString filename = QFileDialog::getSaveFileName( this, tr("Save file..."), "/untitled.csv", tr("CSV Files (*.csv)") );

    // open the file
    QFile file( filename );

    if( file.open( QFile::WriteOnly | QIODevice::Text ) ) {
	// create streamer
	QTextStream ts( &file );
	ts.setAutoDetectUnicode(true);

    ts << "\"ID\"" << ";" << "\"Name\"" << ";" << "\"Surname\"" << ";" << "\"Father's Name\"" << ";" << "\"Birthdate\"" << ";" <<
          "\"Nationality\"" << ";" << "\"Cancer Trace\"" << ";" << "\"Family Inheriance\"" << ";" << "\"Children Number\"" << ";" <<  "\n";



    for( int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i )  {
       QTreeWidgetItem *item = ui->treeWidget->topLevelItem( i );

       //skip the items currently hidden
       if(item->isHidden())
           continue;

       ts << "\"" << item->text(1) << "\"" << ";" << "\"" << item->text(3) << "\"" << ";" << "\"" <<item->text(2) << "\"" << ";" << "\""
          << item->text(4) << "\"" << ";" << "\"" << item->text(5) << "\"" << ";" << "\"" << item->text(12) << "\"" << ";" << "\"" << item->text(10)
          << "\"" << ";" << "\"" <<  item->text(13) << "\"" << ";" << "\"" << item->text(11) << "\"" << "\n";
      //    }

       // Do something with item ...
    }

	// we finished, close file
	file.close();

	// echo success and return
	GeneralDialog *gd = new GeneralDialog( "info", tr("export patients to csv"), tr("Export Succeded!"), tr("Your export has finished successfully!") );
	gd->exec();
    }
}

void ListPatients::on_pushButton_2_clicked() {
    // print patients
    // create the print dialog
    PrintDialog *pd = new PrintDialog( this, true );

    QString printTemplate, printData;

    QSettings settings;

    // load template from file
    QFile file( settings.value("cwd").toString() + "templates/listpatients_en.tpl" );
    if( file.open( QFile::ReadOnly | QIODevice::Text ) ) { // File is Opened / Exists etc
	// move cursor 1 step ahead
	while( !file.atEnd() ) {
	    // get raw line
	    QByteArray tmp_line = file.readLine();
	    // make qstring
	    QString line = QString::fromUtf8( tmp_line );

	    // appent line to template
	    printTemplate += line;
	}
    }

    // load the template data
    pd->loadData(printTemplate);

    pd->subVar( "##title##", tr("list all patients in system") );

    // fix the date
    pd->subVar( "##date##", QDate().currentDate().toString("d MMM, yyyy") );

    // now load the patient data
    QList<Patient> patients = DBConn::instance()->patients();

    printData = "<table width=\"100%\" cellpadding=\"2\" cellspacing=\"0\">";
    printData +=    "<tr>";
    printData +=	"<th align=\"left\">" + tr("id") + "</th>";
    printData +=	"<th align=\"left\">" + tr("Surname") + "</th>";
    printData +=	"<th align=\"left\">" + tr("Name") + "</th>";
    printData +=	"<th align=\"left\">" + tr("Father's Name") + "</th>";
    printData +=	"<th align=\"left\">" + tr("Birth Date") + "</th>";
    printData +=	"<th align=\"left\">" + tr("Gender") + "</th>";
    printData +=	"<th align=\"left\">" + tr("Cancer trace") + "</th>";
    printData +=	"<th align=\"left\">" + tr("Children") + "</th>";
    printData +=	"<th align=\"left\">" + tr("Nationality") + "</th>";
     printData +=	"<th align=\"left\">" + tr("Inheritance") + "</th>";
    printData +=    "</tr>";

    // start writing - loop
    for (int i = 0; i < patients.size(); ++i) {
	Patient tmp_p = patients.at(i);

	printData += "<tr>";
	printData +=	"<td>" + QString::number(tmp_p.id) + "</td>";
	printData +=	"<td>" + tmp_p.surname + "</td>";
	printData +=	"<td>" + tmp_p.name + "</td>";
	printData +=	"<td>" + tmp_p.fathersName + "</td>";
	printData +=	"<td>" + tmp_p.birthdate + "</td>";
	printData +=	"<td>" + tmp_p.gender + "</td>";
    printData +=	"<td>" + tmp_p.cancer_trace + "</td>";
    printData +=	"<td>" + tmp_p.children + "</td>";
    printData +=	"<td>" + tmp_p.nationality + "</td>";
    printData +=	"<td>" + tmp_p.inheritance + "</td>";
	printData += "</tr>";
    }

    printData += "</table>";

    // sub
    pd->subVar( "##data##", printData );

    pd->exec();
}

void ListPatients::on_pushButton_3_clicked() {
    // open the CSV file and start import etc. - SKIP FIRST LINE (titles)
    // get the fileanme
    QString filename = QFileDialog::getOpenFileName( this, tr("Open file..."), "/", tr("CSV Files (*.csv)") );

    if( !filename.isEmpty() ) { // check filename
	// open file
	QFile file( filename );

	if( file.open( QFile::ReadOnly | QIODevice::Text ) ) { // File is Opened / Exists etc
	    // move cursor 1 step ahead
	    file.readLine();

	    // start traversing
	    while( !file.atEnd() ) { // iterate
		// get raw line
		QByteArray tmp_line = file.readLine();
		// make qstring
		QString line = QString::fromUtf8( tmp_line );
		// remove newline
		line = line.replace( "\n", "" );
		// split into fields
		QStringList splt_line = line.replace("\"", "").split( ";" );

		// check if the patient exists already
		QString pat_id = splt_line.at(0);
		Patient p = DBConn::instance()->patient( pat_id.toInt() );
		if( p.id > 0 ) {
		    qDebug() << "skipped patient with id:" << splt_line.at(0);
		} else { // patient doesnt exist - add
		    p.id = pat_id.toInt();
		    p.name = splt_line.at(1);
		    p.surname = splt_line.at(2);
		    p.fathersName = splt_line.at(3);
            p.birthdate = splt_line.at(4);
		    p.created_on = QDate().currentDate().toString("yyyy-MM-dd");

		    // add
		    DBConn::instance()->insertPatient(p);
		    qDebug() << "added patient with id:" << p.id;
		}
	    }

	    // close file
	    file.close();

	    // reload list
	    reload();
	}
    }
}

void ListPatients::on_pushButton_4_clicked()
{
    // reload list
    reload();

    /* clear filter text */
    /* surname */
    /* run the filter only if it is not empty */

     ui->filter_surname->setText("");
      ui->filter_name->setText("");
     ui->filter_fathersName->setText("");
     ui->filter_telephone->setText("");
     ui->filter_children->setCurrentIndex(0);
     ui->filter_cancer->setCurrentIndex(0);
     ui->filter_nationality->setCurrentIndex(0);
     ui->filter_age->setText("");
     ui->filter_inheritance->setCurrentIndex(0);





}
