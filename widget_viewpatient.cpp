/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/14/2010 2010
**   All Rights Reserved
**************************************************************************/

#include <QDebug>
#include <QDate>
#include <QDialogButtonBox>
#include <QDir>
#include <QSettings>
#include <QTextEdit>
#include "widget_viewpatient.h"
#include "ui_widget_viewpatient.h"
#include "mainwindow.h"
#include "dbconn.h"
#include "dialog_addoperation.h"
#include "dialog_viewoperation.h"
#include "dialog_addappointment.h"
#include "dialog_generaldialog.h"
#include "dialog_paymentdlg.h"
#include "dialog_addimage.h"
#include "dialog_viewimage.h"
#include "dialog_printdialog.h"
#include "dialog_prescriptiondialog.h"
#include "dialog_patienthistory.h"

ViewPatient::ViewPatient(QWidget *parent, int pID) :
    QWidget(parent),
    ui(new Ui::ViewPatient)
{
    if( ! pID > 0 ) ;//generate error & return

    ui->setupUi(this);

    ui->widget_4->setMinimumWidth( ui->label_15->size().width()+ui->pat_fathersName->size().width()+45 );

    // fix tabs
    ui->tabWidget->setTabText(0,  tr("History") );
    ui->tabWidget->setTabText(1,  tr("Appointments") );
    ui->tabWidget->setTabText(2,  tr("Operations") );
    ui->tabWidget->setTabText(3,  tr("Images") );
    ui->tabWidget->setTabText(4,  tr("Payments") );
    ui->tabWidget->setTabText(5,  tr("Prescriptions") );

    // store patient ID
    patientID = pID;

    // load patient Data
    loadPatientData(pID);

    // load the latest appointment of the patient
    loadLatestAppointment();
}

void ViewPatient::loadPatientData(int patID) {
    // load data here

    // get patient object from database
    Patient pat = DBConn::instance()->patient(patID);

    // fill data
    ui->formTitle->setText(  tr("Patient Record #") + QString::number(pat.id) );
    ui->pat_fullName->setText( pat.name + " " + pat.surname);
    ui->pat_address->setText( pat.address );
    ui->pat_fathersName->setText( pat.fathersName );
    ui->pat_email->setText( pat.email );
    ui->pat_gender->setText( pat.gender );
    ui->pat_insurance->setText( pat.insurance );
    ui->pat_notes->setText( pat.notes );
    // tel 1
    if( pat.tel1.isEmpty() ) ui->pat_tel1->setText( "" );
    else ui->pat_tel1->setText( pat.tel1 + " (" + pat.tel1_type + ")" );
    // tel 2
    if( pat.tel2.isEmpty() ) ui->pat_tel2->setText( "" );
    else ui->pat_tel2->setText( pat.tel2 + " (" + pat.tel2_type + ")" );
    // tel 3
    if( pat.tel3.isEmpty() ) ui->pat_tel3->setText( "" );
    else ui->pat_tel3->setText( pat.tel3 + " (" + pat.tel3_type + ")" );
    ui->pat_vat->setText( pat.vat );
    ui->pat_amka->setText( pat.amka );

    ui->pat_cancer->setText(pat.cancer_trace);
    ui->pat_nationality->setText(pat.nationality);
    ui->pat_children->setText(pat.children);
    ui->pat_inheritance->setText(pat.inheritance);

    // fix date
    QDate d = QDate::fromString( pat.birthdate, "yyyy-MM-dd" );
    ui->pat_birthdate->setText( d.toString("ddd, MMM dd, yyyy") );

    ui->widget_4->setMinimumWidth( ui->label_15->size().width()+ui->pat_fathersName->size().width()+80 );

    // now we need to get the operations and select the top 5 by date, and display
    // but before we do that, we need to fix the table.
    ui->pat_operations->setAlternatingRowColors(true);

    // create table header
    QHeaderView *hv = new QHeaderView(Qt::Horizontal);
    hv->setDefaultAlignment( Qt::AlignCenter );
//    hv->setResizeMode( QHeaderView::Interactive );
    hv->setStretchLastSection(true);
    // set Header
    ui->pat_operations->setHeader(hv);

    QStringList columns;
    columns << "" <<  tr("Date") <<  tr("Type") <<  tr("Notes");
    ui->pat_operations->setHeaderLabels(columns);

    // resize sections
    hv->resizeSection(0, 5);
    hv->resizeSection(1, 100);
    hv->resizeSection(2, 150);

    // load the patient operations
    loadPatientOperations(patID);

    // Load Appointments!
    // now we need to get the appointments and select the top 5 by date, and display
    // but before we do that, we need to fix the table.
    ui->pat_appointments->setAlternatingRowColors(true);

    // create table header
    QHeaderView *hv2 = new QHeaderView(Qt::Horizontal);
    hv2->setDefaultAlignment( Qt::AlignCenter );
//    hv2->setResizeMode( QHeaderView::Interactive );
    hv2->setStretchLastSection(true);
    // set Header
    ui->pat_appointments->setHeader(hv2);

    QStringList columns2;
    columns2 << "" <<  tr("Date") <<  tr("Time") <<  tr("Symptoms");
    ui->pat_appointments->setHeaderLabels(columns2);

    // resize sections
    hv2->resizeSection(0, 5);
    hv2->resizeSection(1, 100);

    // load the patient appointments
    loadPatientAppointments(patID);

    // Load Images!
    // now we need to get the Images and select the top 5 by date, and display
    // but before we do that, we need to fix the table.
    ui->pat_images->setAlternatingRowColors(true);

    // create table header
    QHeaderView *hv3 = new QHeaderView(Qt::Horizontal);
    hv3->setDefaultAlignment( Qt::AlignCenter );
//    hv3->setResizeMode( QHeaderView::Interactive );
    hv3->setStretchLastSection(true);
    // set Header
    ui->pat_images->setHeader(hv3);

    QStringList columns3;
    columns3 << "" <<  tr("Preview") << tr("Description");
    ui->pat_images->setHeaderLabels(columns3);

    // resize sections
    hv3->resizeSection(0, 5);
    hv3->resizeSection(1, 100);

    // load the patient images
    loadPatientImages(patID);

    // Load Payments!
    // now we need to get the Payments and select the top 5 by date, and display
    // but before we do that, we need to fix the table.
    ui->pat_payments->setAlternatingRowColors(true);

    // create table header
    QHeaderView *hv4 = new QHeaderView(Qt::Horizontal);
    hv4->setDefaultAlignment( Qt::AlignCenter );
//    hv4->setResizeMode( QHeaderView::Interactive );
    hv4->setStretchLastSection(true);
    // set Header
    ui->pat_payments->setHeader(hv4);

    QStringList columns4;
    columns4 << "" <<  tr("Date") <<  tr("Amount") <<  tr("Status") <<  tr("Notes");
    ui->pat_payments->setHeaderLabels(columns4);

    // resize sections
    hv4->resizeSection(0, 5);
    hv4->resizeSection(1, 100);

    // load the patient payments
    loadPatientPayments(patID);

    // Load Prescriptions!
    // now we need to get the Prescriptions and select the top 5 by date, and display
    // but before we do that, we need to fix the table.
    ui->pat_prescriptions->setAlternatingRowColors(true);

    // create table header
    QHeaderView *hv5 = new QHeaderView(Qt::Horizontal);
    hv5->setDefaultAlignment( Qt::AlignCenter );
//    hv5->setResizeMode( QHeaderView::Interactive );
    hv5->setStretchLastSection(true);
    // set Header
    ui->pat_prescriptions->setHeader(hv5);

    QStringList columns5;
    columns5 << "" <<  tr("Date") <<  tr("Prescription (trimmed)");
    ui->pat_prescriptions->setHeaderLabels(columns5);

    // resize sections
    hv5->resizeSection(0, 5);
    hv5->resizeSection(1, 100);

    // load the patient payments
    loadPatientPrescriptions(patID);

    // populate the history widget
    loadPatientHistory(patID);
}

void ViewPatient::loadPatientOperations(int patID) {
    // load the patients from the database
    QList<Operation> operations = DBConn::instance()->operations(patID);

    // clear table
    ui->pat_operations->clear();

    // loop
    for (int i = 0; i < operations.size(); ++i) {
	Operation tmp_p = operations.at(i);

	// fix date
	QDate d = QDate::fromString( tmp_p.op_date, "yyyy-MM-dd" );

	// create table row
	QStringList row;
	row << QString::number(tmp_p.id) << d.toString("MMM dd, yyyy") << tmp_p.op_type << tmp_p.op_notes;
	QTreeWidgetItem* item = new QTreeWidgetItem( ui->pat_operations, row);
	// fix size
	item->setSizeHint(0, QSize(5, 25) );
	// fix alignment
	item->setTextAlignment(1, Qt::AlignCenter);
	item->setTextAlignment(2, Qt::AlignCenter);
	// add row to table
	ui->pat_operations->addTopLevelItem(item);
    }
}

void ViewPatient::loadPatientAppointments(int patID) {
    // load the patients from the database
    QList<Appointment> appointments = DBConn::instance()->appointments(patID);

    // clear table
    ui->pat_appointments->clear();

    // loop
    for (int i = 0; i < appointments.size(); ++i) {
	Appointment tmp_p = appointments.at(i);

	// fix date
	QDate d = QDate::fromString( tmp_p.ap_date, "yyyy-MM-dd" );

	// create table row
	QStringList row;
        row << QString::number(tmp_p.id) << d.toString("MMM dd, yyyy") << tmp_p.ap_time << tmp_p.ap_notes;
	QTreeWidgetItem* item = new QTreeWidgetItem( ui->pat_appointments, row);
	// fix size
	item->setSizeHint(0, QSize(5, 25) );
	// fix alignment
	item->setTextAlignment(1, Qt::AlignCenter);
	item->setTextAlignment(2, Qt::AlignCenter);
	// add row to table
	ui->pat_appointments->addTopLevelItem(item);
    }
}

void ViewPatient::loadLatestAppointment() {
    // load the latest appointment of the user
    Appointment a = DBConn::instance()->latestAppointment( patientID );

    // load the values in the table
    QDate d = QDate::fromString( a.ap_date, "yyyy-MM-dd" );

    QString tmp = ui->latest_app_title->text();
    tmp.replace( "##", d.toString("MMM dd, yyyy") );
    ui->latest_app_title->setText( tmp );

    ui->latest_app_symptoms->setText( a.ap_notes );
    ui->latest_app_diagnosis->setText( a.ap_notes2 );
}

void ViewPatient::loadPatientPayments(int patID) {
    // load the payments from the database
    QList<Payment> payments = DBConn::instance()->payments(patID);

    // clear table
    ui->pat_payments->clear();

    // set up sum variables;
    float f_pending = 0;
    float f_completed = 0;

    // loop
    for (int i = 0; i < payments.size(); ++i) {
	Payment tmp_p = payments.at(i);

	// fix date
	QDate d = QDate::fromString( tmp_p.pay_date, "yyyy-MM-dd" );

	// create table row
	QStringList row;

	// status
	QString p_status;
	if( tmp_p.pay_status == 1 ) {
	    // add amount
	    f_pending += tmp_p.pay_amount.toFloat();
	    // set label text
	    p_status = tr("Pending");
	}

	if( tmp_p.pay_status == 2 ) {
	    f_completed += tmp_p.pay_amount.toFloat();
	    p_status = tr("Completed");
	}

        row << QString::number(tmp_p.id) << d.toString("MMM dd, yyyy") << DBConn::instance()->defaultCurrency(true) + QString::number(tmp_p.pay_amount.toFloat(), 'f', 2) << p_status << tmp_p.pay_notes;
	QTreeWidgetItem* item = new QTreeWidgetItem( ui->pat_payments, row);
	// fix size
	item->setSizeHint(0, QSize(5, 25) );
	// fix alignment
	item->setTextAlignment(1, Qt::AlignCenter);
	item->setTextAlignment(2, Qt::AlignCenter);
	// add row to table
	ui->pat_payments->addTopLevelItem(item);
    }

    // set amounts on top of list
    ui->pay_pending->setText( DBConn::instance()->defaultCurrency(true) + QString::number( f_pending, 'f', 2 ) );
    ui->pay_completed->setText( DBConn::instance()->defaultCurrency(true) + QString::number( f_completed, 'f', 2 ) );
}

void ViewPatient::loadPatientImages(int patID) {
    // load the images from the database
    QList<Image> images = DBConn::instance()->images(patID);

    // clear table
    ui->pat_images->clear();

    // load settings
    QSettings settings;

    // loop
    for (int i = 0; i < images.size(); ++i) {
	Image tmp_p = images.at(i);

	// fix date
	QDate d = QDate::fromString( tmp_p.img_date, "yyyy-MM-dd" );

	//QString img_url = settings

	// create table row
	QTreeWidgetItem* item = new QTreeWidgetItem( ui->pat_images);
	item->setData(0, Qt::DisplayRole, QString::number(tmp_p.id) );

	// create image preview
	QFileInfo fi_file = settings.value("data_path").toString() + "/" + tmp_p.img_name;
	QPixmap pix = QPixmap( settings.value("data_path").toString() + "/" + fi_file.baseName() + "-thumb." + fi_file.suffix() );
	item->setData(1, Qt::DecorationRole, pix );

	//item->setData(2, Qt::DisplayRole, d.toString("MMM dd, yyyy") );
	item->setData(2, Qt::DisplayRole, tmp_p.img_notes );

	// fix size
	item->setSizeHint(0, QSize(5, 25) );
	// fix alignment
	item->setTextAlignment(1, Qt::AlignCenter);
	//item->setTextAlignment(2, Qt::AlignCenter);
	// add row to table
	ui->pat_appointments->addTopLevelItem(item);
    }
}

void ViewPatient::loadPatientPrescriptions(int patID) {
    // load the patients from the database
    QList<Prescription> prescriptions = DBConn::instance()->prescriptions(patID);

    // clear table
    ui->pat_prescriptions->clear();

    // loop
    for (int i = 0; i < prescriptions.size(); ++i) {
	Prescription tmp_p = prescriptions.at(i);

	// fix date
	QDate d = QDate::fromString( tmp_p.prescr_date, "yyyy-MM-dd" );

	// create table row
	QStringList row;

	// retrieve the part of the prescription string using temp carrier.
	QTextEdit *tmp_edit = new QTextEdit();
	tmp_edit->setHtml( tmp_p.prescr_text );

	// write row
	row << QString::number(tmp_p.id) << d.toString("MMM dd, yyyy") << tmp_edit->toPlainText().mid(0, 250);
	QTreeWidgetItem* item = new QTreeWidgetItem( ui->pat_prescriptions, row);

	// fix size
	item->setSizeHint(0, QSize(5, 25) );
	// fix alignment
	item->setTextAlignment(1, Qt::AlignCenter);
	//item->setTextAlignment(2, Qt::AlignCenter);

	// add row to table
	ui->pat_prescriptions->addTopLevelItem(item);
    }

    // set total number
    ui->prescr_total->setText( QString::number( prescriptions.size() ) );
}

void ViewPatient::loadPatientHistory(int patID) {
    // load
    PatientHistoryObj h = DBConn::instance()->patienthistory( patID );
    // assign
    ui->history_diseases->setText( h.hist_diseases );
    ui->history_medications->setText( h.hist_medications );
    ui->history_other->setText( h.hist_other );
    ui->history_allergies->setText( h.hist_allergies );
    ui->history_family->setText( h.hist_family );
    ui->history_other2->setText( h.hist_other2 );
    ui->history_surgeries->setText( h.hist_surgeries );
    ui->history_habit_smoking->setText( h.hist_habit_smoking );
    ui->history_habit_sports->setText( h.hist_habit_sports );
    ui->history_habits_alcohol->setText( h.hist_habit_alcohol );
    ui->history_habit_other->setText( h.hist_habit_other );
}

ViewPatient::~ViewPatient()
{
    delete ui;
}

// ADD OPERATION
void ViewPatient::on_toolButton_clicked()
{
    // show add operation dialog
    AddOperation *adop = new AddOperation(this, 0, patientID);
    if( adop->exec() == QDialog::Accepted ) {
        adop->writeTempOperation();
        loadPatientOperations( patientID );
    } else {
        adop->deleteTempOperation();
    }
}

void ViewPatient::on_pat_operations_itemDoubleClicked(QTreeWidgetItem* item, int column) {
    // get the operation ID
    int operationID = item->text(0).toInt();

    // show the add dialog but pass the ID
    AddOperation *vo = new AddOperation(this, operationID, patientID);
    if( vo->exec() == QDialog::Accepted ) {

        if( vo->action() == 1 ) {
	    // confirm first
	    GeneralDialog *gd = new GeneralDialog("question", tr("delete operation"), tr("Delete Operation"), tr("Are you sure you wish to delete this operation?") );

	    if( gd->exec() == QDialog::Accepted ) {
                // delete the links first, if any
                DBConn::instance()->deleteLinksOfOperation( operationID );
		if( DBConn::instance()->deleteOperation( operationID ) ) loadPatientOperations(patientID); // delete
	    }
        }

        if( vo->action() == 2 ) { // update

            Operation o = DBConn::instance()->operation(operationID);
            o.op_date = vo->opDate();
            o.op_type = vo->opType();
            o.op_notes = vo->opNotes();

            // perform action & update list
            if( DBConn::instance()->updateOperation(o) ) {
                loadPatientOperations(patientID);
                // show message
                GeneralDialog *gd = new GeneralDialog("info", tr("update operation"), tr("Update Operation"), tr("This operation was updated successfully!") );
                gd->exec();
            }
        }
    }
}

void ViewPatient::on_toolButton_2_clicked() {
    // open the new appointment dialog
    AddAppointment *ap = new AddAppointment(this);

    if( ap->exec() == QDialog::Accepted ) {
	qDebug() << "Saving Appointment";

	Appointment a;
	a.ap_date = ap->getDate().toString("yyyy-MM-dd");
        a.ap_time = ap->getTime().toString("HH:mm");
	a.ap_notes = ap->getNotes();
        a.ap_notes2 = ap->getNotes2();
	a.patient_id = patientID;
        a.id = 0; // new Appointment

	// ok now lets save.
	if( DBConn::instance()->insertAppointment(a) ) {
	    // update the list
	    loadPatientAppointments(patientID);
	}

    } else {
	// echo error
    }
}

void ViewPatient::on_toolButton_3_clicked() {
    // go to edit patient screen (this is the add widget but with the flags properly set)
    MainWindow::instance()->goAddPatient(patientID);
}

void ViewPatient::on_pat_appointments_itemDoubleClicked(QTreeWidgetItem* item, int column) {
    // get the appointment ID
    int appointmentID = item->text(0).toInt();

    // show the viewoperation dialog
    AddAppointment *vo = new AddAppointment(this, appointmentID);

    if( vo->exec() == QDialog::Accepted ) {

	if( vo->action() == 1 ) {
	    // confirm first
	    GeneralDialog *gd = new GeneralDialog("question", tr("delete appointment"), tr("Delete Appointment"), tr("Are you sure you wish to delete this appointment?") );

	    if( gd->exec() == QDialog::Accepted ) {
		// delete & update list
                DBConn::instance()->deleteLinksWhereAppointment( appointmentID );
		if( DBConn::instance()->deleteAppointment(appointmentID) ) loadPatientAppointments(patientID);
	    }
	}

	if( vo->action() == 2 ) {
	    // update
	    Appointment a = DBConn::instance()->appointment(appointmentID);
	    a.ap_date = vo->getDate().toString("yyyy-MM-dd");
            a.ap_time = vo->getTime().toString("HH:mm");
	    a.ap_notes = vo->getNotes();
            a.ap_notes2 = vo->getNotes2();

	    // perform action & update list
	    if( DBConn::instance()->updateAppointment(a) ) {
		loadPatientAppointments(patientID);
		// show message
		GeneralDialog *gd = new GeneralDialog("info", tr("change appointment"), tr("Change Appointment"), tr("This appointment changed successfully!") );
		gd->exec();
	    }
	}
    }

}

void ViewPatient::on_toolButton_4_clicked() {
    // confirm first
    GeneralDialog *gd = new GeneralDialog("question", tr("delete patient"), tr("Delete Patient"), tr("Are you sure you wish to delete this patient?") );

    if( gd->exec() == QDialog::Accepted ) {
	// operations
	QList<Operation> ops = DBConn::instance()->operations( patientID );
	// loop
	for( int i = 0; i < ops.size(); i++ ) {
	    // instance
	    Operation o = ops.at(i);

            // delete links
            DBConn::instance()->deleteLinksOfOperation( o.id );

	    // delete
	    DBConn::instance()->deleteOperation( o.id );
	}

	// appointments
	QList<Appointment> aps = DBConn::instance()->appointments( patientID );
	// loop
	for( int i = 0; i < aps.size(); i++ ) {
	    // instance
	    Appointment a = aps.at(i);
	    // delete
	    DBConn::instance()->deleteAppointment( a.id );
	}

        // images
        QList<Image> img = DBConn::instance()->images( patientID );
        // loop
        for( int i = 0; i < img.size(); i++ ) {
            // instance
            Image a = img.at(i);
            // delete
            DBConn::instance()->deleteImage( a.id );
        }

        // prescriptions
        QList<Prescription> pres = DBConn::instance()->prescriptions( patientID );
        // loop
        for( int i = 0; i < pres.size(); i++ ) {
            // instance
            Prescription a = pres.at(i);
            // delete
            DBConn::instance()->deletePrescription( a.id );
        }

        // payments
        QList<Payment> pay = DBConn::instance()->payments( patientID );
        // loop
        for( int i = 0; i < pay.size(); i++ ) {
            // instance
            Payment a = pay.at(i);
            // delete
            DBConn::instance()->deletePayment( a.id );
        }

        // patient history
        PatientHistoryObj ph = DBConn::instance()->patienthistory( patientID );
        // delete
        DBConn::instance()->deletePatientHistory( ph.id );

	// perform delete
	DBConn::instance()->deletePatient( patientID );

	// go back to list patients
	MainWindow::instance()->goPatientList(true);
    }
}

void ViewPatient::on_toolButton_5_clicked() {
    // NEW IMAGE
    AddImage *i = new AddImage(this);
    if( i->exec() == QDialog::Accepted ) {
	qDebug() << "saving image...";

	qDebug() << "saving image " << i->file() << " with description " << i->description();

	// get file info
	QFileInfo fi_file( i->file() );
	// init settings object
	QSettings settings;
	// create destination filename
	QString dest_fileName = QString::number(patientID) + "_" + QString::number(QDateTime().currentDateTime().toTime_t ()) + "." + fi_file.suffix();
	QString dest_fileNameThumb = QString::number(patientID) + "_" + QString::number(QDateTime().currentDateTime().toTime_t ()) + "-thumb." + fi_file.suffix();
	QString dest_fullFileName = settings.value( "data_path").toString() + "/" + dest_fileName;
	QString dest_fullFileNameThumb = settings.value( "data_path").toString() + "/" + dest_fileNameThumb;

	// ok now lets create the image as a QImage
	QImage i_img( i->file() );
	QSize size_img = i_img.size();

	// check image dimensions, see if it exceeds 800x600 and then scale
	if( size_img.width() > 800 | size_img.height() > 600 ) { // scale

	    if( size_img.width() > 800 ) { // scale to width 800
		// but first check what will happen to the height
		if( (size_img.height()*800)/size_img.width() > 600 )
		    i_img = i_img.scaledToHeight( 600, Qt::SmoothTransformation ); // height will take care of width
		else
		    i_img = i_img.scaledToWidth( 800, Qt::SmoothTransformation );

		size_img = i_img.size();
	    }

	    if( size_img.height() > 600 ) { // scale to height 600
		i_img = i_img.scaledToHeight( 600, Qt::SmoothTransformation ); // height will take care of width
	    }
	}

	// now make the thumbnail
	QImage i_thumb( i->file() );

	// try to copy
	if( !i_img.save( dest_fullFileName ) ) {
	    // echo error
	    GeneralDialog *gd = new GeneralDialog( "error", tr("insert image"), tr("Copy Error"), tr("Image copy failed!") );
	    gd->exec();
	    return;
	}

	// save thumb
	i_thumb = i_thumb.scaledToWidth(80, Qt::SmoothTransformation);
	i_thumb.save( dest_fullFileNameThumb );

	// image should be ok, lets continue, create the Image object
	Image img;
        img.id = 0; // new image
	img.patient_id = patientID;
	img.img_date = QDate().currentDate().toString("yyyy-MM-dd");
	img.img_notes = i->description();
	img.img_name = dest_fileName;

	if( DBConn::instance()->insertImage(img) ) {
	    // echo success
	    GeneralDialog *gd = new GeneralDialog( "info", tr("insert image"), "", tr("Image was successfully added!") );
	    gd->exec();

	    // re-load list
	    loadPatientImages( patientID );
	} else {
	    // echo error
	    GeneralDialog *gd = new GeneralDialog( "error", tr("insert image"), tr("Database Error"), tr("Image couldn't be stored in Database!") );
	    gd->exec();
	    return;
	}

	qDebug() << "copied to ... " << settings.value( "data_path").toString() + "/" + dest_fileName;

    } else {
	// do nothing
    }
}

void ViewPatient::on_toolButton_6_clicked() {
    // NEW PAYMENT
    // open the new payment dialog
    PaymentDlg *pd = new PaymentDlg(this);

    if( pd->exec() == QDialog::Accepted ) {
	qDebug() << "Saving Payment";

	Payment p;
        p.id = 0; // new payment
	p.pay_date = pd->getDate().toString("yyyy-MM-dd");
	p.pay_amount = QString::number( pd->getAmount() );
	p.pay_notes = pd->getNotes();
	p.pay_status = pd->getStatus();
	p.patient_id = patientID;

	// ok now lets save.
	if( DBConn::instance()->insertPayment(p) ) {
	    // update the list
	    loadPatientPayments(patientID);
	}

    } else {
	// echo error
    }
}

void ViewPatient::on_pat_images_itemDoubleClicked(QTreeWidgetItem* item, int column) {
    // get the image ID
    int imageID = item->text(0).toInt();

    ViewImage *vi = new ViewImage(this, imageID, this);
    vi->show();
}

void ViewPatient::on_pat_payments_itemDoubleClicked(QTreeWidgetItem* item, int column) {
    // get the image ID
    int paymentID = item->text(0).toInt();

    // show the viewoperation dialog
    PaymentDlg *pd = new PaymentDlg(this, paymentID);

    if( pd->exec() == QDialog::Accepted ) {

	if( pd->action() == 1 ) {
	    // confirm first
	    GeneralDialog *gd = new GeneralDialog("question", tr("delete payment"), tr("Deleting payment..."), tr("Are you sure you wish to delete this payment?") );

	    if( gd->exec() == QDialog::Accepted ) {
		// delete & update list
		if( DBConn::instance()->deletePayment(paymentID) ) loadPatientPayments(patientID);
	    }
	}

	if( pd->action() == 2 ) {
	    // update
	    Payment p = DBConn::instance()->payment(paymentID);
	    p.pay_date = pd->getDate().toString("yyyy-MM-dd");
	    p.pay_amount = QString::number( pd->getAmount() );
	    p.pay_status = pd->getStatus();
	    p.pay_notes = pd->getNotes();

	    // perform action & update list
	    if( DBConn::instance()->updatePayment(p) ) {
		loadPatientPayments(patientID);
		// show message
		GeneralDialog *gd = new GeneralDialog("info", tr("change payment"), QString::fromUtf8("Change Payment"), QString::fromUtf8("Payment has been successfully updated!") );
		gd->exec();
	    }
	}
    }
}

void ViewPatient::on_toolButton_7_clicked() {
    // print patient record
    // create the print dialog
    PrintDialog *pd = new PrintDialog( this, true );

    // get settings
    QSettings settings;

    QString printTemplate, printData;

    // load template from file
    QFile file( settings.value("cwd").toString() + "templates/patient_en.tpl" );
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

    Patient p = DBConn::instance()->patient(patientID);

    pd->subVar( "##ttl_fathername##", tr("Father's Name:") );
    pd->subVar( "##ttl_email##", tr("Email:") );
    pd->subVar( "##ttl_bdate##", tr("Date of Birth:") );
    pd->subVar( "##ttl_tel1##", tr("Telephone 1:") );
    pd->subVar( "##ttl_insurance##", tr("Insurance:") );
    pd->subVar( "##ttl_vat##", tr("VAT #:") );
    pd->subVar( "##ttl_tel2##", tr("Telephone 2:") );
    pd->subVar( "##ttl_amka##", tr("Social Security #:") );
    pd->subVar( "##ttl_gender##", tr("Gender:") );
    pd->subVar( "##ttl_email##", tr("Email:") );
    pd->subVar( "##ttl_tel3##", tr("Telephone 3:") );
    pd->subVar( "##ttl_address##", tr("Address:") );
    pd->subVar( "##ttl_notes##", tr("Notes:") );
    pd->subVar( "##ttl_history##", tr("History") );
    pd->subVar( "##ttl_apps##", tr("Appointments") );
    pd->subVar( "##ttl_ops##", tr("Operations") );
    pd->subVar( "##ttl_prescriptions##", tr("Prescriptions") );

    pd->subVar( "##title##", tr("Patient File") );
    pd->subVar( "##fullname##", p.name + " " + p.surname );
    pd->subVar( "##fathername##", p.fathersName );
    pd->subVar( "##email##", p.email );
    pd->subVar( "##gender##", p.gender );
    pd->subVar( "##bdate##", p.birthdate );
    pd->subVar( "##tel1##", p.tel1 + " (" + p.tel1_type + ")" );
    pd->subVar( "##tel2##", p.tel2 + " (" + p.tel2_type + ")" );
    pd->subVar( "##tel3##", p.tel3 + " (" + p.tel3_type + ")" );
    pd->subVar( "##insurance##", p.insurance );
    pd->subVar( "##vat##", p.vat );
    pd->subVar( "##amka##", p.amka );
    pd->subVar( "##address##", p.address );
    pd->subVar( "##notes##", p.notes );

    printData = "";

    // construct the operations list
    QList<Operation> ops = DBConn::instance()->operations( patientID );

    if( ops.size() > 0 ) {
        // prepare table
        printData = "<table class=\"data\" width=\"100%\" cellpadding=\"2\" cellspacing=\"0\">";
        printData += "<tr>"
                    "<th>"+ tr("Date") +"</th>"
                    "<th>"+ tr("Type") +"</th>"
                    "<th>"+ tr("Notes") +"</th>"
                    "</tr>";

        // loop
        for (int i = 0; i < ops.size(); ++i) {
            Operation tmp_p = ops.at(i);

            printData +=    "<tr>";
            printData +=	"<td width=\"95\" style=\"font-weight:bold;\">" + QDate().fromString(tmp_p.op_date, "yyyy-MM-dd").toString("dd MMM, yyyy") + "</td>";
            printData +=	"<td align=\"left\">" + tmp_p.op_type + "</td>";
            printData +=	"<td align=\"left\">" + tmp_p.op_notes + "</td>";
            printData +=    "</tr>";
        }

        // close table
        printData += "</table>";
    }

    // assign
    pd->subVar( "##ops##", printData );

    // clear buffer
    printData = "";

    // construct the appointments list
    QList<Appointment> apps = DBConn::instance()->appointments( patientID );

    if( apps.size() > 0 ) {
        // prepare table
        printData = "<table class=\"data\" width=\"100%\" cellpadding=\"2\" cellspacing=\"0\">";
        printData += "<tr>"
                    "<th>"+ tr("Date") +"</td>"
                    "<th>"+ tr("Time") +"</th>"
                    "<th>"+ tr("Symptoms") +"</th>"
                    "<th>"+ tr("Diagnosis") +"</th>"
                    "</tr>";

        // loop
        for (int i = 0; i < apps.size(); ++i) {
            Appointment tmp_p = apps.at(i);

            printData +=    "<tr>";
            printData +=	"<td width=\"95\" style=\"font-weight:bold;\">" + QDate().fromString(tmp_p.ap_date, "yyyy-MM-dd").toString("dd MMM, yyyy") + "</td>";
            printData +=	"<td width=\"95\" align=\"left\">" + tmp_p.ap_time + "</td>";
            printData +=	"<td align=\"left\">" + tmp_p.ap_notes + "</td>";
            printData +=	"<td align=\"left\">" + tmp_p.ap_notes2 + "</td>";
            printData +=    "</tr>";
        }

        // close table
        printData += "</table>";
    }

    // assign
    pd->subVar( "##apps##", printData );

    // clear buffer
    printData = "";

    // construct the Prescription list
    QList<Prescription> pres = DBConn::instance()->prescriptions( patientID );

    if( pres.size() > 0 ) {
        // prepare table
        printData = "<table class=\"data\" width=\"100%\" cellpadding=\"2\" cellspacing=\"0\">";
        printData += "<tr>"
                    "<th>"+ tr("Date") +"</td>"
                    "<th>"+ tr("Prescription") +"</th>"
                    "</tr>";

        // loop
        for (int i = 0; i < pres.size(); ++i) {
            Prescription tmp_p = pres.at(i);

            QTextEdit *te = new QTextEdit();
            te->setHtml( tmp_p.prescr_text );

            printData +=    "<tr>";
            printData +=	"<td valign=\"top\" width=\"95\" style=\"font-weight:bold;\">" + QDate().fromString(tmp_p.prescr_date, "yyyy-MM-dd").toString("dd MMM, yyyy") + "</td>";
            printData +=	"<td align=\"left\">" + te->toPlainText().replace( "\n", "<br/>" ) + "</td>";
            printData +=    "</tr>";

            delete te;
        }

        // close table
        printData += "</table>";
    }

    // assign
    pd->subVar( "##prescriptions##", printData );

    // history
    pd->subVar( "##ttl_personal_history##", tr("Personal History") );
    pd->subVar( "##ttl_surgeries##", tr("Surgeries") );
    pd->subVar( "##ttl_diseases##", tr("Diseases") );
    pd->subVar( "##ttl_allergies##", tr("Allergies") );
    pd->subVar( "##ttl_personal_other##", tr("Other") );
    pd->subVar( "##ttl_habits##", tr("Habits") );
    pd->subVar( "##ttl_smoking##", tr("Smoking") );
    pd->subVar( "##ttl_sports##", tr("Sports") );
    pd->subVar( "##ttl_alcohol##", tr("Alcohol") );
    pd->subVar( "##ttl_habit_other##", tr("Other") );
    pd->subVar( "##ttl_family_history##", tr("Family History") );
    pd->subVar( "##ttl_medications##", tr("Medications") );
    pd->subVar( "##ttl_other##", tr("Other") );

    PatientHistoryObj ph = DBConn::instance()->patienthistory( patientID );

    pd->subVar( "##surgeries##", ph.hist_surgeries );
    pd->subVar( "##diseases##", ph.hist_diseases );
    pd->subVar( "##allergies##", ph.hist_allergies );
    pd->subVar( "##personal_other##", ph.hist_other2 );
    pd->subVar( "##smoking##", ph.hist_habit_smoking );
    pd->subVar( "##sports##", ph.hist_habit_sports );
    pd->subVar( "##alcohol##", ph.hist_habit_alcohol );
    pd->subVar( "##habit_other##", ph.hist_habit_other );
    pd->subVar( "##family_history##", ph.hist_family );
    pd->subVar( "##medications##", ph.hist_medications );
    pd->subVar( "##other##", ph.hist_other );


    pd->subVar( "##officename##", DBConn::instance()->settingValue("dental_office_name") );
    pd->subVar( "##officespec##", DBConn::instance()->settingValue("dental_office_name2") );
    pd->subVar( "##officeaddress##", DBConn::instance()->settingValue("dental_office_address") );
    pd->subVar( "##officetel##", DBConn::instance()->settingValue("dental_office_telephone") );
    pd->subVar( "##date##", QDate().currentDate().toString("dd MMM, yyyy") );

    pd->exec();
}

void ViewPatient::on_toolButton_8_clicked()
{
    Patient pat = DBConn::instance()->patient(patientID);

    // show the viewprescription dialog
    PrescriptionDialog *pd = new PrescriptionDialog(this, 0, pat.name + " " + pat.surname );

    if( pd->exec() == QDialog::Accepted ) {
	qDebug() << "Saving Prescription";

	Prescription p;
        p.id = 0; // new prescription
	p.prescr_date = QDate().currentDate().toString("yyyy-MM-dd");
	p.prescr_notes = pd->getNotes();
	p.prescr_text = pd->getPrescriptionText();
	p.patient_id = patientID;

	// ok now lets save.
	if( DBConn::instance()->insertPrescription(p) ) {
	    // update the list
	    loadPatientPrescriptions(patientID);
	}

    } else {
	// echo error
    }
}

void ViewPatient::on_pat_prescriptions_itemDoubleClicked(QTreeWidgetItem* item, int column) {
    // get the prescription ID
    int prescriptionID = item->text(0).toInt();
    Patient pat = DBConn::instance()->patient(patientID);

    // show the viewprescription dialog
    PrescriptionDialog *pd = new PrescriptionDialog(this, prescriptionID, pat.name + " " + pat.surname );

    if( pd->exec() == QDialog::Accepted ) {

	if( pd->action() == 1 ) {
	    // confirm first
	    GeneralDialog *gd = new GeneralDialog("question", tr("delete prescription"), tr("Deleting prescription..."), tr("Are you sure you wish to delete this prescription?") );

	    if( gd->exec() == QDialog::Accepted ) {
		// delete & update list
                DBConn::instance()->deleteLinksWherePrescription( prescriptionID );
		if( DBConn::instance()->deletePrescription(prescriptionID) ) loadPatientPrescriptions(patientID);
	    }
	}

	if( pd->action() == 2 ) {
	    // update
	    Prescription p = DBConn::instance()->prescription(prescriptionID);
	    // p.pay_date = pd->getDate().toString("yyyy-MM-dd");
	    //p.pay_amount = QString::number( pd->getAmount() );
	    p.prescr_text = pd->getPrescriptionText();
	    p.prescr_notes = pd->getNotes();

	    // perform action & update list
	    if( DBConn::instance()->updatePrescription(p) ) {
		loadPatientPrescriptions(patientID);
		// show message
		GeneralDialog *gd = new GeneralDialog("info", tr("change prescription"), QString::fromUtf8("Change Prescription"), QString::fromUtf8("Prescription has been successfully updated!") );
		gd->exec();
	    }
	}

	if( pd->action() == 3 ) {
	    // re-open the dialog

            pd->print();

            on_pat_prescriptions_itemDoubleClicked(item, column);
            return;
	}
    }
}

void ViewPatient::on_toolButton_9_clicked()
{
    // changes
    PatientHistory *ph = new PatientHistory(this, patientID);
    ph->exec();

    if( ph->action() == 1 ) { // save
	// try to get the history for this patient, if found then update, if not then add
	PatientHistoryObj pho = DBConn::instance()->patienthistory( patientID );

	if( pho.id > 0 ) { // history exists
	    pho.hist_diseases = ph->diseases();
            pho.hist_allergies = ph->allergies();
            pho.hist_family = ph->family();
            pho.hist_habit_alcohol = ph->habit_alcohol();
            pho.hist_habit_other = ph->habit_other();
            pho.hist_habit_smoking = ph->habit_smoking();
            pho.hist_habit_sports = ph->habit_sports();
            pho.hist_other2 = ph->other2();
	    pho.hist_medications = ph->medications();
	    pho.hist_other = ph->other();
            pho.hist_surgeries = ph->surgeries();
	    // update
	    if( DBConn::instance()->updatePatientHistory(pho) ) {
		GeneralDialog *gd = new GeneralDialog("info", tr("save patient history"), tr("History Updated"), tr("Patient history was successfully updated!") );
		gd->exec();
	    } else {
		GeneralDialog *gd = new GeneralDialog("error", tr("save patient history"), tr("History Update Error"), tr("Patient history update FAILED!") );
		gd->exec();
	    }
	} else { // history record doesnt exist
	    pho.patient_id = patientID;
            pho.hist_diseases = ph->diseases();
            pho.hist_allergies = ph->allergies();
            pho.hist_family = ph->family();
            pho.hist_habit_alcohol = ph->habit_alcohol();
            pho.hist_habit_other = ph->habit_other();
            pho.hist_habit_smoking = ph->habit_smoking();
            pho.hist_habit_sports = ph->habit_sports();
            pho.hist_other2 = ph->other2();
            pho.hist_medications = ph->medications();
            pho.hist_other = ph->other();
            pho.hist_surgeries = ph->surgeries();
	    // insert
	    if( DBConn::instance()->insertPatientHistory(pho) ) {
		GeneralDialog *gd = new GeneralDialog("info", tr("save patient history"), tr("History Updated"), tr("Patient history was successfully updated!") );
		gd->exec();
	    } else {
		GeneralDialog *gd = new GeneralDialog("error", tr("save patient history"), tr("History Update Error"), tr("Patient history update FAILED!") );
		gd->exec();
	    }
	}

	// reload
	loadPatientHistory(patientID);
    }
}

void ViewPatient::reloadPatientImages() {
    loadPatientImages(patientID);
}
