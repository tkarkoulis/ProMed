/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/18/2010 2010
**   All Rights Reserved
**************************************************************************/
#include <QDate>
#include <QPushButton>

#include "dialog_viewoperation.h"
#include "ui_dialog_viewoperation.h"
#include "dbconn.h"
#include "obj_operation.h"

ViewOperation::ViewOperation(QWidget *parent, int operation_id) :
    QDialog(parent),
    ui(new Ui::ViewOperation)
{
    ui->setupUi(this);

    // we have the operation ID, so please get the object from DB
    Operation op = DBConn::instance()->operation(operation_id);

    // fix date
    QDate d = QDate::fromString( op.op_date, "yyyy-MM-dd" );

    // assign  
    ui->op_date->setText( d.toString("ddd, MMMM d, yyyy") );
    ui->op_type->setText(op.op_type);
    ui->op_notes->setText(op.op_notes);

    // get the 2 buttons
    QPushButton *but_cancel = ui->buttonBox->button( QDialogButtonBox::Close );
    QPushButton *but_delete =  ui->buttonBox->addButton( tr("Delete"), QDialogButtonBox::AcceptRole );
    but_cancel->setIcon( QIcon(":/icons/Reset 16x16") );
    but_delete->setIcon( QIcon(":/icons/Save 16x16") );

    but_cancel->setText( tr("Cancel") );
}

ViewOperation::~ViewOperation()
{
    delete ui;
}
