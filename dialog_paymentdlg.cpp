/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/29/2010 2010
**   All Rights Reserved
**************************************************************************/

#include <QPushButton>

#include "dialog_paymentdlg.h"
#include "ui_dialog_paymentdlg.h"
#include "dbconn.h"

PaymentDlg::PaymentDlg(QWidget *parent, int payment_id) :
    QDialog(parent),
    ui(new Ui::PaymentDlg)
{
    ui->setupUi(this);

    // set action flag action = 1 == delete, 2 == update
    i_action = 0;

    if( payment_id > 0 ) { // WE ARE UPDATING
	// set flag
	paymentID = payment_id;

	// get payment
	Payment p = DBConn::instance()->payment( paymentID );

        // set title
        ui->formTitle->setText( tr("View Payment") );

	// load values
	loadPaymentValues(p);

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
        // button text
        but_cancel->setText( tr("Cancel") );
        but_save->setText( tr("Save") );
        // button icons
	but_cancel->setIcon( QIcon(":/icons/Reset 16x16") );
	but_save->setIcon( QIcon(":/icons/Save 16x16") );
    }

    // fix default date
    ui->pay_date->setDate( QDate().currentDate() );
}

void PaymentDlg::loadPaymentValues(Payment p) {
    ui->pay_date->setDate( QDate().fromString(p.pay_date, "yyyy-MM-dd") );
    ui->pay_amount->setText( p.pay_amount );
    ui->pay_notes->setPlainText( p.pay_notes );
    ui->pay_status->setCurrentIndex( p.pay_status-1 );
}

QDate PaymentDlg::getDate() {
    return ui->pay_date->date();
}

QString PaymentDlg::getNotes() {
    return ui->pay_notes->toPlainText();
}

float PaymentDlg::getAmount() {
    return ui->pay_amount->text().toFloat();
}

int PaymentDlg::getStatus() {
    return ui->pay_status->currentIndex()+1;
}

void PaymentDlg::deleteClicked() {
    // set flag
    i_action = 1;
}

void PaymentDlg::updateClicked() {
    // set flag
    i_action = 2;
}

int PaymentDlg::action() {
    return i_action;
}

PaymentDlg::~PaymentDlg()
{
    delete ui;
}
