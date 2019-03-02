/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/29/2010 2010
**   All Rights Reserved
**************************************************************************/

#include <QPushButton>
#include <QFileDialog>

#include "dialog_addimage.h"
#include "ui_dialog_addimage.h"
#include "tool_dropimagetoolbutton.h"


AddImage::AddImage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddImage)
{
    ui->setupUi(this);

    // get the 2 buttons
    QPushButton *but_cancel = ui->buttonBox->button( QDialogButtonBox::Cancel );
    QPushButton *but_save = ui->buttonBox->button( QDialogButtonBox::Save );
    but_cancel->setIcon( QIcon(":/icons/Reset 16x16") );
    but_save->setIcon( QIcon(":/icons/Save 16x16") );
    but_cancel->setText( tr("Cancel") );
    but_save->setText( tr("Save") );

    connect( ui->toolButton, SIGNAL(fileDropped(QString)), this, SLOT(fileDrop(QString)) );

    // init
    fileLocation = "none";
}

void AddImage::fileDrop(QString filename) {
    // show file in area
    ui->fileLabel->setText( filename );
    ui->fileLabel->setFont( QFont( ui->fileLabel->font().family(), 7, QFont::Bold ) );
    // store the thing
    fileLocation = filename;
}

QString AddImage::file() {
    return fileLocation;
}

QString AddImage::description() {
    return ui->plainTextEdit->toPlainText();
}

AddImage::~AddImage()
{
    delete ui;
}

void AddImage::on_pushButton_clicked() {
    // open QFIleDialog
    QString filename = QFileDialog::getOpenFileName(this,
     tr("Browse Image"), "/home/test", tr("Image Files (*.png *.jpg *.bmp *.gif)"));

    ui->fileLabel->setText( filename );
    ui->fileLabel->setFont( QFont( ui->fileLabel->font().family(), 10, QFont::Bold ) );
    // store image
    fileLocation = filename;
}
