/**************************************************************************
**   Recompile IT Services, Development Dept
**   11/22/2010 2010
**   All Rights Reserved
**************************************************************************/

#include <QPrinter>
#include <QPrintDialog>

#include "mainwindow.h"
#include "dialog_printdialog.h"
#include "ui_dialog_printdialog.h"

PrintDialog::PrintDialog(QWidget *parent, bool readOnly) :
    QDialog(parent),
    ui(new Ui::PrintDialog)
{
    ui->setupUi(this);

    this->setWindowTitle( tr("print...") );
}

void PrintDialog::loadData(QString data) {
    // load data inside the textbrowser
    ui->widget_2->setHtml( data );
    htmlData = data;
}

// substitute variable inside the print dialog body
void PrintDialog::subVar(QString var, QString data) {

    htmlData = htmlData.replace( var, data );

    // substitute
    ui->widget_2->setHtml( htmlData );
}

PrintDialog::~PrintDialog()
{
    delete ui;
}

void PrintDialog::printNow() {
    on_pushButton_clicked();
}

void PrintDialog::on_pushButton_clicked()
{
    // Print
    QPrinter printer(QPrinter::HighResolution);

    // get dialog
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    // dialog title
    dialog->setWindowTitle( tr("Print Document") );
    // if user selected anything other than print
    if (dialog->exec() != QDialog::Accepted)
	return;

    // ## WINDOWS FIX ##
    if( MainWindow::instance()->getOSType() == "WIN" ) {
        QString newHTML = htmlData.replace( "font-size:16px", "font-size:20px" );
        newHTML = newHTML.replace( "font-size:12px", "font-size:16px" );
        newHTML = newHTML.replace( "Tahoma", "Arial" );

        QWebEngineView *wv = new QWebEngineView();
        wv->setHtml( newHTML );

        // print
        wv->page()->print( &printer, [=](bool){} );
    } else {
        // print!
        ui->widget_2->page()->print(&printer, [=](bool){});
    }
}
