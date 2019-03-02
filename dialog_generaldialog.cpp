/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/18/2010 2010
**   All Rights Reserved
**************************************************************************/
#include <QDebug>
#include <QPushButton>

#include "dialog_generaldialog.h"
#include "ui_dialog_generaldialog.h"
#include "mainwindow.h"

GeneralDialog::GeneralDialog(QString type, QString window_title, QString dlg_message_title, QString dlg_message, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralDialog)
{
    // init ui
    ui->setupUi(this);

    // fix style from main window
    this->setStyleSheet( MainWindow::instance()->styleSheet() );

    // fix data
    this->setWindowTitle( window_title );
    ui->dlg_text->setText( dlg_message );
    ui->dlg_text_title->setText( dlg_message_title );

    // depending on the dialog Type, we need to create the various types.
    if( type == "info" ) {

	setWindowIcon( QIcon(":/icons/Info 48x48") );

	ui->dlg_pixmap->setPixmap( QPixmap(":/icons/Info 48x48") );
        ui->dlg_pixmap->setAlignment( Qt::AlignTop );

	// remove cancel button
	ui->buttonBox->setStandardButtons( QDialogButtonBox::Ok );

	// fix pixmap
	QPushButton *but_save = ui->buttonBox->button( QDialogButtonBox::Ok );
	but_save->setIcon( QIcon(":/icons/Save 16x16") );

    } else if( type == "warning" ) {

	setWindowIcon( QIcon(":/icons/Warning 48x48") );

	ui->dlg_pixmap->setPixmap( QPixmap(":/icons/Warning 48x48") );
        ui->dlg_pixmap->setAlignment( Qt::AlignTop );

	// remove cancel button
	ui->buttonBox->setStandardButtons( QDialogButtonBox::Ok );

	// fix pixmap
	QPushButton *but_save = ui->buttonBox->button( QDialogButtonBox::Ok );
	but_save->setIcon( QIcon(":/icons/Save 16x16") );

    } else if( type == "error" ) {

	setWindowIcon( QIcon(":/icons/Error 48x48") );

	ui->dlg_pixmap->setPixmap( QPixmap(":/icons/Error 48x48") );
        ui->dlg_pixmap->setAlignment( Qt::AlignTop );

	// remove cancel button
	ui->buttonBox->setStandardButtons( QDialogButtonBox::Ok );

	// fix pixmap
	QPushButton *but_save = ui->buttonBox->button( QDialogButtonBox::Ok );
	but_save->setIcon( QIcon(":/icons/Save 16x16") );

    } else if( type == "question" ) {

	setWindowIcon( QIcon(":/icons/Question 48x48") );

	ui->dlg_pixmap->setPixmap( QPixmap(":/icons/Question 48x48") );
        ui->dlg_pixmap->setAlignment( Qt::AlignTop );

	// fix pixmap
	QPushButton *but_save = ui->buttonBox->button( QDialogButtonBox::Ok );
	but_save->setIcon( QIcon(":/icons/Save 16x16") );
	QPushButton *but_cancel = ui->buttonBox->button( QDialogButtonBox::Cancel );
	but_cancel->setIcon( QIcon(":/icons/Reset 16x16") );
        but_cancel->setText( tr("Cancel") );

    }
}

void GeneralDialog::addButton(QPushButton *button) {
    // add the button to the layout.
    ui->widget_2->layout()->addWidget( button );
}

GeneralDialog::~GeneralDialog()
{
    delete ui;
}
