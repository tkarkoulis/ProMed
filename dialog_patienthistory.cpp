/**************************************************************************
**   Recompile IT Services, Development Dept
**   12/13/2010 2010
**   All Rights Reserved
**************************************************************************/

#include <QPushButton>
#include "dialog_patienthistory.h"
#include "ui_dialog_patienthistory.h"
#include "dbconn.h"

PatientHistory::PatientHistory(QWidget *parent, int patient_id) :
    QDialog(parent),
    ui(new Ui::PatientHistory)
{
    ui->setupUi(this);

    // set action flag action = 1 == save
    i_action = 0;

    // set flag
    patientID = patient_id;

    // get appointment
    PatientHistoryObj h = DBConn::instance()->patienthistory( patientID );

    // load values
    loadHistoryValues(h);

    // we also need to fix the buttons
    // remote buttons
    ui->buttonBox->clear();
    QPushButton *but_cancel = ui->buttonBox->addButton( tr("Cancel"), QDialogButtonBox::RejectRole );
    QPushButton *but_update = ui->buttonBox->addButton( tr("Save"), QDialogButtonBox::AcceptRole );

    // test
    but_cancel->setIcon( QIcon(":/icons/Reset 16x16") );
    but_update->setIcon( QIcon(":/icons/Save 16x16") );

    connect( but_update, SIGNAL(clicked()), this, SLOT(updateClicked()) );
}

void PatientHistory::loadHistoryValues(PatientHistoryObj h) {
    // load values
    ui->history_diseases->setPlainText( h.hist_diseases );
    ui->history_medications->setPlainText( h.hist_medications );
    ui->history_other->setPlainText( h.hist_other );
    ui->history_other2->setPlainText( h.hist_other2 );
    ui->history_allergies->setPlainText( h.hist_allergies );
    ui->history_surgeries->setPlainText( h.hist_surgeries );
    ui->history_family->setPlainText( h.hist_family );
    ui->history_habit_smoking->setCurrentIndex( ui->history_habit_smoking->findText( h.hist_habit_smoking ) );
    ui->history_habit_sports->setCurrentIndex( ui->history_habit_sports->findText( h.hist_habit_sports ) );
    ui->history_habit_alcohol->setCurrentIndex( ui->history_habit_alcohol->findText( h.hist_habit_alcohol ) );
    ui->history_habit_other->setPlainText( h.hist_habit_other );
}

void PatientHistory::updateClicked() {
    i_action = 1;
}

QString PatientHistory::medications() {
    return ui->history_medications->toPlainText();
}

QString PatientHistory::diseases() {
    return ui->history_diseases->toPlainText();
}

QString PatientHistory::other() {
    return ui->history_other->toPlainText();
}

QString PatientHistory::allergies() {
    return ui->history_allergies->toPlainText();
}

QString PatientHistory::surgeries() {
    return ui->history_surgeries->toPlainText();
}

QString PatientHistory::other2() {
    return ui->history_other2->toPlainText();
}

QString PatientHistory::family() {
    return ui->history_family->toPlainText();
}

QString PatientHistory::habit_smoking() {
    return ui->history_habit_smoking->currentText();
}

QString PatientHistory::habit_sports() {
    return ui->history_habit_sports->currentText();
}

QString PatientHistory::habit_alcohol() {
    return ui->history_habit_alcohol->currentText();
}

QString PatientHistory::habit_other() {
    return ui->history_habit_other->toPlainText();
}

int PatientHistory::action() {
    return i_action;
}

PatientHistory::~PatientHistory()
{
    delete ui;
}
