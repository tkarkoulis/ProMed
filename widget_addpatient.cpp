#include <QMessageBox>
#include <QComboBox>
#include <QDateTime>
#include <time.h>
#include "widget_addpatient.h"
#include "ui_widget_addpatient.h"
#include "dbconn.h"
#include "mainwindow.h"
#include "dialog_generaldialog.h"
#include "obj_patient.h"

AddPatient::AddPatient(QWidget *parent, int patient_id) :
    QWidget(parent),
    ui(new Ui::AddPatient)
{    
    ui->setupUi(this);

    // set patient id and load values
    if( patient_id > 0 ) {
	patientID = patient_id;
	// get patient obj
	Patient p = DBConn::instance()->patient(patientID);
	// load values
	loadPatientValues(p);
	// hide the button for reset
	 ui->pushButton_2->hide();
	 ui->widget_5->layout()->removeWidget( ui->pushButton_2 );
	 //ui->widget_5->setMaximumWidth(180);
	 ui->formTitle->setText( tr("Edit Patient Record") );
    } else {
	// fix default date
	patientID = 0;
	ui->pat_bdate->setDate( QDate().currentDate() );
    }


    ui->pat_nationality->addItems(DBConn::instance()->get_nationalities());

}

AddPatient::~AddPatient()
{
    delete ui;
}

void AddPatient::loadPatientValues(Patient p) {
    // load values
    ui->pat_name->setText( p.name );
    ui->pat_surname->setText( p.surname );
    ui->pat_fatherName->setText( p.fathersName );
    ui->pat_address->setPlainText( p.address );
    ui->pat_email->setText( p.email );
    ui->pat_gender->setCurrentIndex( ui->pat_gender->findText(p.gender) );
    ui->pat_insurance->setCurrentIndex( ui->pat_insurance->findText(p.insurance) );
    ui->pat_vat->setText( p.vat );
    ui->pat_tel1->setText( p.tel1 );
    ui->pat_tel1type->setCurrentIndex( ui->pat_tel1type->findText( p.tel1_type ) );
    ui->pat_tel2->setText( p.tel2 );
    ui->pat_tel2type->setCurrentIndex( ui->pat_tel2type->findText( p.tel2_type ) );
    ui->pat_tel3->setText( p.tel3 );
    ui->pat_tel3type->setCurrentIndex( ui->pat_tel3type->findText( p.tel3_type ) );
    ui->pat_bdate->setDate( QDate().fromString( p.birthdate, "yyyy-MM-dd" ) );
    ui->pat_notes->setPlainText( p.notes );
    ui->pat_amka->setText( p.amka );

    ui->pat_cancer->setCurrentIndex( ui->pat_children->findText( p.cancer_trace ) );
    ui->pat_inheritance->setCurrentIndex( ui->pat_children->findText( p.inheritance ));
    ui->pat_children->setCurrentIndex( ui->pat_children->findText( p.children) );
    ui->pat_nationality->setCurrentIndex( ui->pat_nationality->findText( p.nationality) );
}

void AddPatient::on_pushButton_2_clicked()
{
    if( patientID > 0 ) {
    } else {
	// reset form
	ui->pat_name->clear();
	ui->pat_address->clear();
	ui->pat_email->clear();
	ui->pat_surname->clear();
	ui->pat_bdate->setDate(QDate().currentDate());;
	ui->pat_fatherName->clear();
	ui->pat_gender->setCurrentIndex(0);
	ui->pat_insurance->setCurrentIndex(0);
	ui->pat_vat->clear();
	ui->pat_tel1->clear();
	ui->pat_tel1type->clear();
	ui->pat_tel2->clear();
	ui->pat_tel2type->clear();
	ui->pat_tel3->clear();
	ui->pat_tel3type->clear();
	ui->pat_amka->clear();
    }
}

void AddPatient::on_pushButton_3_clicked()
{
    if( patientID > 0 )  // we are going back to the View Patient Screen
	MainWindow::instance()->goViewPatient(patientID);
    else
	MainWindow::instance()->goHome();
}

void AddPatient::on_pushButton_clicked()
{
    // write in the database! BUT FIRST CHECK IF NAME / SURNAME / FATHERS NAME are there!
    if( ui->pat_name->text().isEmpty() | ui->pat_surname->text().isEmpty() | ui->pat_fatherName->text().isEmpty() ) {
	// ECHO ERROR USING DIALOG
	GeneralDialog *gd = new GeneralDialog("error", tr("patient record error"), tr("Error"), QString::fromUtf8("You must fill in at least the patient's Name, Surname and Father's Name.") );
	gd->exec();
	return;
    }

    // create object to store
    Patient pat;
    pat.name = ui->pat_name->text();
    pat.surname = ui->pat_surname->text();
    pat.fathersName = ui->pat_fatherName->text();
    pat.vat = ui->pat_vat->text();
    pat.tel1 = ui->pat_tel1->text();
    pat.tel1_type = ui->pat_tel1type->currentText();
    pat.tel2 = ui->pat_tel2->text();
    pat.tel2_type = ui->pat_tel2type->currentText();
    pat.tel3 = ui->pat_tel3->text();
    pat.tel3_type = ui->pat_tel3type->currentText();
    pat.address = ui->pat_address->toPlainText();
    pat.gender = ui->pat_gender->currentText();
    pat.insurance = ui->pat_insurance->currentText();
    pat.email = ui->pat_email->text();
    pat.notes = ui->pat_notes->toPlainText();
    pat.birthdate = ui->pat_bdate->date().toString("yyyy-MM-dd");
    pat.amka = ui->pat_amka->text();
    pat.created_on = QDate().currentDate().toString("yyyy-MM-dd");

    pat.cancer_trace=ui->pat_cancer->currentText();
    pat.inheritance=ui->pat_inheritance->currentText();
    pat.nationality=ui->pat_nationality->currentText().trimmed();
    pat.children=ui->pat_children->currentText();


    if( patientID > 0 ) { // we are UPDATING
	pat.id = patientID;
	// do update
	if( DBConn::instance()->updatePatient( pat ) ) {
	    // output success!
	    GeneralDialog *gd = new GeneralDialog("info", tr("update patient record"), tr("Update Patient"), tr("Record ")+pat.name+" "+pat.surname+tr(" was successfully updated!") );
	    gd->exec();

	    // now go back to the root
	    MainWindow::instance()->goViewPatient(pat.id);
	    MainWindow::instance()->listPatientsScreen->reload();
	}
    } else { // we are INSERTING

        if( !DBConn::instance()->demo() ) {
            pat.id = 0;
            if( int id = DBConn::instance()->insertPatient( pat ) ) {
                // output success!
                GeneralDialog *gd = new GeneralDialog("info", tr("new patient record"), tr("New patient Record"), tr("Patient Record has been successfully saved with id: ")+QString::number(id) );
                gd->exec();

                // go to the patient we just added
                MainWindow::instance()->goViewPatient(id);
                MainWindow::instance()->listPatientsScreen->reload();
            }
        }
    }
}
void AddPatient::on_pat_bdate_dateTimeChanged(const QDateTime &dateTime)
{
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);
    int current_year = aTime->tm_year + 1900;


    QDateTime date = QDateTime::fromString( ui->pat_bdate->text(), "dd/M/yyyy");
    int age = current_year - dateTime.date().year();


    ui->pat_age->setText(QString::number(age));
}


void AddPatient::on_pat_age_textChanged(const QString &arg1)
{

    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);
    int current_year = aTime->tm_year + 1900;

    //get current date
    QDate date = QDate::currentDate();
//    QString dateString = date.toString();

    //block the signal and store its original state
    bool oldState = ui->pat_age->blockSignals(true);

    QString age = ui->pat_age->text();
    date =date.addYears( -age.toInt() );

    ui->pat_bdate->setDate(date);
    //restore it
    ui->pat_age->blockSignals(oldState);


//    QDateTime dateTime = QDateTime::fromString( ui->pat_bdate->text(), "dd/M/yyyy");
}


