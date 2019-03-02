/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/18/2010 2010
**   All Rights Reserved
**************************************************************************/
#include <QPushButton>

#include "dialog_addappointment.h"
#include "ui_dialog_addappointment.h"
#include "dbconn.h"

AddAppointment::AddAppointment(QWidget *parent, int appointment_id) :
    QDialog(parent),
    ui(new Ui::AddAppointment)
{
    ui->setupUi(this);

    // set action flag action = 1 == delete, 2 == update
    i_action = 0;

    // fix title in case we are updating
    if( appointment_id > 0 ) ui->formTitle->setText( tr("Update Appointment") );

    if( appointment_id > 0 ) { // WE ARE UPDATING
	// set flag
	appointmentID = appointment_id;

	// get appointment
	Appointment a = DBConn::instance()->appointment( appointmentID );

	// load values
	loadAppointmentValues(a);

	// we also need to fix the buttons
	// remote buttons
	ui->buttonBox->clear();
	QPushButton *but_delete = ui->buttonBox->addButton( tr("Delete"), QDialogButtonBox::AcceptRole );
	QPushButton *but_cancel = ui->buttonBox->addButton( tr("Cancel"), QDialogButtonBox::RejectRole );
	QPushButton *but_update = ui->buttonBox->addButton( tr("Update"), QDialogButtonBox::AcceptRole );

	// test
	but_cancel->setIcon( QIcon(":/icons/Reset 16x16") );
	but_delete->setIcon( QIcon(":/icons/Cancel 16x16") );
	but_update->setIcon( QIcon(":/icons/Save 16x16") );

	connect( but_delete, SIGNAL(clicked()), this, SLOT(deleteClicked()) );
	connect( but_update, SIGNAL(clicked()), this, SLOT(updateClicked()) );

    } else {
	// get the 2 buttons
	QPushButton *but_cancel = ui->buttonBox->button( QDialogButtonBox::Cancel );
	QPushButton *but_save = ui->buttonBox->button( QDialogButtonBox::Save );
	but_cancel->setIcon( QIcon(":/icons/Reset 16x16") );
	but_save->setIcon( QIcon(":/icons/Save 16x16") );
        but_cancel->setText( tr("Cancel") );
        but_save->setText( tr("Save") );


        // fix default date
        ui->ap_date->setDate( QDate().currentDate() );
    }
}

void AddAppointment::loadAppointmentValues(Appointment a) {
    // load values
    ui->ap_date->setDate( QDate().fromString(a.ap_date, "yyyy-MM-dd") );
    ui->ap_time->setTime( QTime().fromString(a.ap_time, "HH:mm") );
    ui->ap_notes->setPlainText( a.ap_notes );
    ui->ap_notes2->setPlainText( a.ap_notes2 );
}

void AddAppointment::deleteClicked() {
    // set flag
    i_action = 1;
}

void AddAppointment::updateClicked() {
    // set flag
    i_action = 2;
}

int AddAppointment::action() {
    return i_action;
}

QDate AddAppointment::getDate() {
    return ui->ap_date->date();
}

QTime AddAppointment::getTime() {
    return ui->ap_time->time();
}

QString AddAppointment::getNotes() {
    return ui->ap_notes->toPlainText();
}

QString AddAppointment::getNotes2() {
    return ui->ap_notes2->toPlainText();
}

AddAppointment::~AddAppointment()
{
    delete ui;
}
