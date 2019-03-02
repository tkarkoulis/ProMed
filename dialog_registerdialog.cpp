/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/18/2010 2010
**   All Rights Reserved
**************************************************************************/

#include <QPushButton>

#include "dialog_registerdialog.h"
#include "ui_dialog_registerdialog.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);

    // add Register Now button
    ui->buttonBox->clear();
    cancel = new QPushButton( tr("Cancel") );
    ui->buttonBox->addButton( cancel, QDialogButtonBox::RejectRole );
    registerNow = new QPushButton( tr("Activate Now") );
    ui->buttonBox->addButton( registerNow, QDialogButtonBox::ApplyRole);

    cancel->setIcon( QIcon(":/icons/Reset 16x16") );
    registerNow->setIcon( QIcon(":/icons/Forward 24x24") );

    ui->entercode->hide();

    this->setMaximumHeight(130);

    connect( registerNow, SIGNAL(clicked()), this, SLOT(registerNowClicked()) );
}

void RegisterDialog::registerNowClicked() {
    // do things
    ui->registerintro->hide();
    ui->entercode->show();

    // create registration button
    processRegister = new QPushButton( tr("Activate!") );
    ui->buttonBox->addButton( processRegister, QDialogButtonBox::AcceptRole );
    processRegister->setIcon( QIcon(":/icons/Save 16x16") );

    // hide the other thing.
    registerNow->hide();
}

void RegisterDialog::apply() {
    // do nothing
}

QString RegisterDialog::getEmail(){
    return ui->reg_email->text();
}

QString RegisterDialog::getCode() {
    return ui->reg_code->text();
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}
