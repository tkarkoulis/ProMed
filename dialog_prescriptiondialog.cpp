/**************************************************************************
**   Recompile IT Services, Development Dept
**   12/1/2010 2010
**   All Rights Reserved
**************************************************************************/

#include <QPushButton>
#include <QPrintDialog>
#include <QPrinter>
#include <QFile>
#include <QDate>
#include <QDebug>
#include <QSettings>

#include "dialog_prescriptiondialog.h"
#include "dialog_printdialog.h"
#include "ui_dialog_prescriptiondialog.h"
#include "dbconn.h"

PrescriptionDialog::PrescriptionDialog(QWidget *parent, int prescription_id, QString name) :
    QDialog(parent),
    ui(new Ui::PrescriptionDialog)
{
    ui->setupUi(this);

    ui->pre_name->setText( name );

   if( prescription_id > 0 ) { // WE ARE UPDATING
	// set flag
	prescriptionID = prescription_id;

	// get appointment
	Prescription p = DBConn::instance()->prescription( prescription_id );

	// load values
	loadPrescriptionData(p);

	// we also need to fix the buttons
	// remove buttons
	ui->buttonBox->clear();
	QPushButton *but_delete = ui->buttonBox->addButton( tr("Delete"), QDialogButtonBox::AcceptRole );
	QPushButton *but_cancel = ui->buttonBox->addButton( tr("Cancel"), QDialogButtonBox::RejectRole );
	QPushButton *but_update = ui->buttonBox->addButton( tr("Update"), QDialogButtonBox::AcceptRole );
        QPushButton *but_print = ui->buttonBox->addButton( tr("Print"), QDialogButtonBox::AcceptRole );

	// test
        but_print->setIcon( QIcon(":/icons/Print 32x32") );
        but_update->setIcon( QIcon(":/icons/Save 16x16") );
	but_cancel->setIcon( QIcon(":/icons/Reset 16x16") );
	but_delete->setIcon( QIcon(":/icons/Cancel 16x16") );

	connect( but_delete, SIGNAL(clicked()), this, SLOT(deleteClicked()) );
	connect( but_update, SIGNAL(clicked()), this, SLOT(updateClicked()) );
	connect( but_print, SIGNAL(clicked()), this, SLOT(printClicked()) );

    } else {
	// get the 2 buttons
	QPushButton *but_cancel = ui->buttonBox->button( QDialogButtonBox::Cancel );
	QPushButton *but_save = ui->buttonBox->button( QDialogButtonBox::Save );
	but_cancel->setIcon( QIcon(":/icons/Reset 16x16") );
	but_save->setIcon( QIcon(":/icons/Save 16x16") );
        but_cancel->setText( tr("Cancel") );
        but_save->setText( tr("Save") );
    }
}

QString PrescriptionDialog::getPrescriptionText() {
    // return text
    return ui->textEdit->toHtml();
}

QString PrescriptionDialog::getNotes() {
    // return text
    return ui->plainTextEdit->toPlainText();
}

int PrescriptionDialog::action() {
    // return action
    return i_action;
}

void PrescriptionDialog::loadPrescriptionData(Prescription p) {
    // load

    ui->textEdit->setHtml( p.prescr_text );
    ui->plainTextEdit->setPlainText( p.prescr_notes );
}

void PrescriptionDialog::deleteClicked() {
    // delete
    i_action = 1;
}

void PrescriptionDialog::updateClicked() {
    // update
    i_action = 2;
}

void PrescriptionDialog::printClicked() {
    // print
    i_action = 3;

    QSettings settings;

    // this is the new prescription form, so load the template from the file
    QFile file( settings.value("cwd").toString() + "templates/prescription.tpl" );
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

    printTemplate = printTemplate.replace( "\t", "" );

    printTemplate = printTemplate.replace( "##officename##", DBConn::instance()->settingValue("dental_office_name") );
    printTemplate = printTemplate.replace( "##officename2##", DBConn::instance()->settingValue("dental_office_name2") );
    printTemplate = printTemplate.replace( "##address##", DBConn::instance()->settingValue("dental_office_address").replace("\n", "<br/>") );
    printTemplate = printTemplate.replace( "##date##", QDate().currentDate().toString("MMM dd, yyyy") );
    printTemplate = printTemplate.replace( "##content##", ui->textEdit->toPlainText().replace( "\n", "<br/>" ) );
}

void PrescriptionDialog::print() {
    // print
    PrintDialog *pd = new PrintDialog(this, true);
    pd->loadData( printTemplate );

    // pd->printNow();
    pd->exec();
}

PrescriptionDialog::~PrescriptionDialog()
{
    delete ui;
}
