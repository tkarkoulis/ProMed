/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/18/2010 2010
**   All Rights Reserved
**************************************************************************/
#include <QPushButton>
#include <QFileInfo>
#include <QFileDialog>
#include <QSettings>
#include <QDebug>
#include <QHBoxLayout>

#include "dialog_addoperation.h"
#include "dialog_addappointment.h"
#include "dialog_prescriptiondialog.h"
#include "dialog_viewimage.h"
#include "dialog_selectdialog.h"
#include "dialog_generaldialog.h"
#include "dialog_prescriptiondialog.h"
#include "dbconn.h"
#include "mainwindow.h"
#include "ui_dialog_addoperation.h"

AddOperation::AddOperation(QWidget *parent, int operation_id, int patient_id) :
    QDialog(parent),
    ui(new Ui::AddOperation)
{
    ui->setupUi(this);

    // set flag
    i_action = 0;
    patientID = patient_id;

    // fix title in case we are updating
    if( operation_id > 0 ) ui->formTitle->setText( tr("Update Operation") );

    // set current date
    ui->op_date->setDate( QDate().currentDate() );

    // load the operations
    ui->op_type->addItems( DBConn::instance()->operationTypes() );

    if( operation_id > 0 ) { // WE ARE UPDATING
        // set flag
        operationID = operation_id;

        // load attachments
        loadAttachedAppointments();
        loadAttachedPrescriptions();
        loadAttachedImages();

        // get operation
        Operation o = DBConn::instance()->operation( operationID );

        // load values
        loadOperationValues(o);

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

        // we are making a NEW Operation. create an object and save it temporarily. If it isnt needed, it will be deleted in time.
        Operation op;
        op.id = 0; // new operation
        op.op_date = opDate();
        op.op_type = "";
        op.patient_id = patientID;
        op.op_notes = "";

        DBConn::instance()->insertOperation(op);

        operationID = op.id;

        // get the 2 buttons
        QPushButton *but_cancel = ui->buttonBox->button( QDialogButtonBox::Cancel );
        QPushButton *but_save = ui->buttonBox->button( QDialogButtonBox::Save );
        but_cancel->setIcon( QIcon(":/icons/Reset 16x16") );
        but_save->setIcon( QIcon(":/icons/Save 16x16") );
        but_cancel->setText( tr("Cancel") );
        but_save->setText( tr("Save") );
    }
}

void AddOperation::loadOperationValues(Operation o) {
    ui->op_date->setDate( QDate().fromString(o.op_date, "yyyy-MM-dd") );
    ui->op_type->setCurrentIndex( ui->op_type->findText( o.op_type ) );
    ui->op_notes->setPlainText( o.op_notes );
}

QString AddOperation::opType() {
    return ui->op_type->currentText();
}

QString AddOperation::opNotes() {
    return ui->op_notes->toPlainText();
}

QString AddOperation::opDate() {
    return ui->op_date->date().toString("yyyy-MM-dd");
}

void AddOperation::deleteClicked() {
    // set flag
    i_action = 1;
}

void AddOperation::updateClicked() {
    // set flag
    i_action = 2;
}

int AddOperation::action() {
    return i_action;
}

AddOperation::~AddOperation()
{
    delete ui;
}

void AddOperation::on_btn_new_att_app_clicked()
{
    // open the new appointment dialog
    AddAppointment *ap = new AddAppointment(this);

    if( ap->exec() == QDialog::Accepted ) {

        Appointment a;
        a.ap_date = ap->getDate().toString("yyyy-MM-dd");
        a.ap_time = ap->getTime().toString("HH:mm");
        a.ap_notes = ap->getNotes();
        a.patient_id = patientID;
        a.id = 0; // new appointment

        // ok now lets save.
        if( DBConn::instance()->insertAppointment(a) ) {

            // insert the link
            DBConn::instance()->linkOperation( "appointment", operationID, a.id );

            loadAttachedAppointments();
        }

    } else {
        // echo error
    }
}

void AddOperation::loadAttachedAppointments() {
    // clear layout
    clearLayout( ui->app_container->layout() );

    lst_app = DBConn::instance()->linkedAppointments( operationID );

    // iterate
    for( int i = 0; i < lst_app.size(); i++ ) {
        Appointment a = (Appointment) lst_app.at(i);

        // create the widget
        QWidget *w = new QWidget();
        w->setObjectName("attach-appointment-row");
        w->setStyleSheet( "#attach-appointment-row { padding:0;margin:0; } QPushButton:pressed { background-color: none; border:none; }");


        // NEED CLASS

        if( MainWindow::instance()->getOSType() == "WIN" )
            w->setStyleSheet( w->styleSheet() + " attach-appointment-row QLabel{ font-size:8pt; }" );
        else
            w->setStyleSheet( w->styleSheet() + " attach-appointment-row QLabel{ font-size:11pt; }" );

        // END NEED CLASS

        w->setLayout( new QHBoxLayout() );
        w->setContentsMargins(0,0,0,0);
        w->layout()->setSpacing(0);
        w->layout()->setContentsMargins(0,0,0,0);
        w->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);

        QLabel *a_time = new QLabel(w);
        a_time->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
        a_time->setAlignment( Qt::AlignTop );
        a_time->setMaximumWidth(49);
        a_time->setMinimumWidth(49);
        a_time->setText( QTime().fromString(a.ap_time, "HH:mm").toString( "HH:mm" ) );

        QLabel *a_date = new QLabel(w);
        a_date->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
        a_date->setText( QDate().fromString(a.ap_date, "yyyy-MM-dd").toString("MMM dd, yyyy") );
        a_date->setToolTip( a.ap_notes );

        QPushButton *view_app = new QPushButton();
        view_app->setFlat(true);
        view_app->setIcon( QIcon(":/icons/Info 16x16") );
        view_app->setObjectName( QString::number(a.id) );

        connect( view_app, SIGNAL(clicked()), this, SLOT(appInfoClicked()) );

        QPushButton *del_app = new QPushButton();
        del_app->setFlat(true);
        del_app->setIcon( QIcon(":/icons/Delete 16x16") );
        del_app->setObjectName( QString::number(a.id) );

        connect( del_app, SIGNAL(clicked()), this, SLOT(appDeleteClicked()) );

        w->layout()->addWidget(a_time);
        w->layout()->addWidget(a_date);
        w->layout()->addWidget(new QWidget() );
        w->layout()->addWidget(view_app);
        w->layout()->addWidget(del_app);

        ui->app_container->layout()->addWidget(w);
    }
}

void AddOperation::clearLayout(QLayout *l) {
    // clear the appointments layout
    QLayoutItem *item;

    while((item = l->takeAt(0)) != 0) {

        if (item->layout()) {
                clearLayout((QLayout *)item->layout());
        }
        l->removeWidget(item->widget());
        delete item->widget();
    }
}

void AddOperation::on_btn_old_att_app_clicked()
{
    // create the list of the appointments of this patient and send it to the selector dialog
    QList<Appointment> apps = DBConn::instance()->appointments( patientID );

    SelectDialog *sd = new SelectDialog(this);
    sd->setData( apps );

    if( sd->exec() == QDialog::Accepted ) {
        // returned with success
        if( !sd->getSelectedID().isEmpty() ) {
            Appointment a = DBConn::instance()->appointment( sd->getSelectedID().toInt() );

            // insert the link IF it doesnt exist
            if( !DBConn::instance()->linkExists( "appointment", operationID, a.id ) ) {

                if( operationID )
                    DBConn::instance()->linkOperation( "appointment", operationID, a.id );

                // re-load
                loadAttachedAppointments();
            } else {
                // output error
                GeneralDialog *gd = new GeneralDialog( "error", tr("link exists"), tr("Link already exists"), tr( "This operation is already linked to the selected appointment. Linking Skipped." ), 0 );
                gd->exec();
            }
        }
    }
}

void AddOperation::on_btn_new_att_img_clicked()
{
    // open QFIleDialog
    QString filename = QFileDialog::getOpenFileName(this,
     tr("Browse Image"), "/home/test", tr("Image Files (*.png *.jpg *.bmp *.gif)"));

    if( filename.isEmpty() ) return;

    // now save the image itself
    // get file info
    QFileInfo fi_file( filename );
    // init settings object
    QSettings settings;
    // create destination filename
    QString dest_fileName = QString::number(patientID) + "_" + QString::number(QDateTime().currentDateTime().toTime_t ()) + "." + fi_file.suffix();
    QString dest_fileNameThumb = QString::number(patientID) + "_" + QString::number(QDateTime().currentDateTime().toTime_t ()) + "-thumb." + fi_file.suffix();
    QString dest_fullFileName = settings.value( "data_path").toString() + "/" + dest_fileName;
    QString dest_fullFileNameThumb = settings.value( "data_path").toString() + "/" + dest_fileNameThumb;

    // ok now lets create the image as a QImage
    QImage i_img( filename );
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
    QImage i_thumb( filename );

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
    img.patient_id = patientID;
    img.img_date = QDate().currentDate().toString("yyyy-MM-dd");
    img.img_notes = "";
    img.img_name = dest_fileName;

    if( DBConn::instance()->insertImage(img) ) {
        // echo success
        GeneralDialog *gd = new GeneralDialog( "info", tr("insert image"), "", tr("Image was successfully added!") );
        gd->exec();

    } else {
        // echo error
        GeneralDialog *gd = new GeneralDialog( "error", tr("insert image"), tr("Database Error"), tr("Image couldn't be stored in Database!") );
        gd->exec();
        return;
    }

    // insert the link
    DBConn::instance()->linkOperation( "image", operationID, img.id );

    loadAttachedImages();
}

void AddOperation::loadAttachedImages() {

    // clear layout
    clearLayout( ui->img_container->layout() );

    // init settings object
    QSettings settings;

    lst_img = DBConn::instance()->linkedImages( operationID );

    // iterate
    for( int z = 0; z < lst_img.size(); z++ ) {
        Image i = (Image) lst_img.at(z);

        // create the widget
        QWidget *w = new QWidget();
        w->setObjectName("attach-image-row");
        w->setStyleSheet( "#attach-image-row { padding:0;margin:0; } QPushButton:pressed { background-color: none; border:none; }");


        // NEED CLASS

        if( MainWindow::instance()->getOSType() == "WIN" )
            w->setStyleSheet( w->styleSheet() + " attach-image-row QLabel{ font-size:8pt; }" );
        else
            w->setStyleSheet( w->styleSheet() + " attach-image-row QLabel{ font-size:11pt; }" );

        // END NEED CLASS


        w->setLayout( new QHBoxLayout() );
        w->setContentsMargins(0,0,0,0);
        w->layout()->setSpacing(0);
        w->layout()->setContentsMargins(0,0,0,0);
        w->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);

        // analyze image data
        QFileInfo fi_file = settings.value("data_path").toString() + "/" + i.img_name;
        QPixmap pix = QPixmap( settings.value("data_path").toString() + "/" + fi_file.baseName() + "-thumb." + fi_file.suffix() );

        QPixmap cropped = pix.copy( 0, 0, 40, 30 );

        QLabel *a_thumb = new QLabel(w);
        a_thumb->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
        a_thumb->setPixmap(cropped);
        a_thumb->setStyleSheet( "border:1px solid #ccc; padding:3px; margin-right:5px;" );
        a_thumb->setAlignment( Qt::AlignTop );

        QLabel *a_date = new QLabel(w);
        a_date->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
        a_date->setText( QDate().fromString( i.img_date, "yyyy-MM-dd").toString("MMM dd, yyyy") );
        a_date->setToolTip( i.img_notes );

        QPushButton *view_img = new QPushButton();
        view_img->setFlat(true);
        view_img->setIcon( QIcon(":/icons/Info 16x16") );
        view_img->setObjectName( QString::number(i.id) );

        connect( view_img, SIGNAL(clicked()), this, SLOT(imgInfoClicked()) );

        QPushButton *del_img = new QPushButton();
        del_img->setFlat(true);
        del_img->setIcon( QIcon(":/icons/Delete 16x16") );
        del_img->setObjectName( QString::number(i.id) );

        connect( del_img, SIGNAL(clicked()), this, SLOT(imgDeleteClicked()) );

        w->layout()->addWidget(a_thumb);
        w->layout()->addWidget(a_date);
        w->layout()->addWidget(new QWidget() );
        w->layout()->addWidget(view_img);
        w->layout()->addWidget(del_img);

        ui->img_container->layout()->addWidget(w);
    }
}

void AddOperation::on_btn_old_att_img_clicked()
{
    // create the list of the images of this patient and send it to the selector dialog
    QList<Image> imgs = DBConn::instance()->images(patientID);

    SelectDialog *sd = new SelectDialog(this);
    sd->setData( imgs );

    if( sd->exec() == QDialog::Accepted ) {
        // returned with success
        if( !sd->getSelectedID().isEmpty() ) {
            Image i = DBConn::instance()->image( sd->getSelectedID().toInt() );

            // insert the link IF it doesnt exist
            if( !DBConn::instance()->linkExists( "image", operationID, i.id ) ) {

                DBConn::instance()->linkOperation( "image", operationID, i.id );

                // re-load
                loadAttachedImages();
            } else {
                // output error
                GeneralDialog *gd = new GeneralDialog( "error", tr("link exists"), tr("Link already exists"), tr( "This operations is already linked to the selected image. Linking Skipped." ), 0 );
                gd->exec();
            }
        }
    }
}

void AddOperation::on_btn_new_att_prescr_clicked()
{
    Patient pat = DBConn::instance()->patient(patientID);

    // open the new prescription dialog
    PrescriptionDialog *pd = new PrescriptionDialog(this, 0, pat.name + " " + pat.surname );

    if( pd->exec() == QDialog::Accepted ) {

        Prescription p;
        p.prescr_date = QDate().currentDate().toString("yyyy-MM-dd");
        p.prescr_notes = pd->getNotes();
        p.prescr_text = pd->getPrescriptionText();
        p.patient_id = patientID;

        // ok now lets save.
        if( DBConn::instance()->insertPrescription(p) ) {

            // craete link
            DBConn::instance()->linkOperation( "prescription", operationID, p.id );

            loadAttachedPrescriptions();
        }

    } else {
        // echo error
    }
}

void AddOperation::loadAttachedPrescriptions() {
    // clear layout
    clearLayout( ui->pre_container->layout() );

    lst_pre = DBConn::instance()->linkedPrescriptions( operationID );

    // iterate
    for( int i = 0; i < lst_pre.size(); i++ ) {
        Prescription p = (Prescription) lst_pre.at(i);

        // create the widget
        QWidget *w = new QWidget(this);
        w->setObjectName("attach-prescription-row");
        w->setStyleSheet( "#attach-prescription-row { padding:0;margin:0; } QPushButton:pressed { background-color: none; border:none; }");


        // NEED CLASS

        if( MainWindow::instance()->getOSType() == "WIN" )
            w->setStyleSheet( w->styleSheet() + " attach-prescription-row QLabel{ font-size:8pt; }" );
        else
            w->setStyleSheet( w->styleSheet() + " attach-prescription-row QLabel{ font-size:11pt; }" );

        // END NEED CLASS


        w->setLayout( new QHBoxLayout() );
        w->setContentsMargins(0,0,0,0);
        w->layout()->setSpacing(0);
        w->layout()->setContentsMargins(0,0,0,0);
        w->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);

        QLabel *a_date = new QLabel(w);
        a_date->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
        a_date->setAlignment( Qt::AlignTop );
        a_date->setMaximumWidth(80);
        a_date->setMinimumWidth(80);
        a_date->setText( QDate().fromString(p.prescr_date, "yyyy-MM-dd").toString("MMM dd, yyyy") );

        QLabel *a_notes = new QLabel(w);
        a_notes->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
        a_notes->setText( p.prescr_notes.left(12) );
        a_notes->setToolTip( p.prescr_text );

        QPushButton *view_pre = new QPushButton();
        view_pre->setFlat(true);
        view_pre->setIcon( QIcon(":/icons/Info 16x16") );
        view_pre->setObjectName( QString::number(p.id) );

        connect( view_pre, SIGNAL(clicked()), this, SLOT(preInfoClicked()) );

        QPushButton *del_pre = new QPushButton();
        del_pre->setFlat(true);
        del_pre->setIcon( QIcon(":/icons/Delete 16x16") );
        del_pre->setObjectName( QString::number(p.id) );

        connect( del_pre, SIGNAL(clicked()), this, SLOT(preDeleteClicked()) );

        w->layout()->addWidget(a_date);
        w->layout()->addWidget(a_notes);
        w->layout()->addWidget(new QWidget() );
        w->layout()->addWidget(view_pre);
        w->layout()->addWidget(del_pre);

        ui->pre_container->layout()->addWidget(w);
    }
}

void AddOperation::on_btn_old_att_prescr_clicked()
{
    // create the list of the prescriptions of this patient and send it to the selector dialog
    QList<Prescription> pres = DBConn::instance()->prescriptions(patientID);

    SelectDialog *sd = new SelectDialog(this);
    sd->setData( pres );

    if( sd->exec() == QDialog::Accepted ) {
        // returned with success
        if( !sd->getSelectedID().isEmpty() ) {
            Prescription p = DBConn::instance()->prescription( sd->getSelectedID().toInt() );

            // insert the link IF it doesnt exist
            if( !DBConn::instance()->linkExists( "prescription", operationID, p.id ) ) {

                DBConn::instance()->linkOperation( "prescription", operationID, p.id );

                // re-load
                loadAttachedPrescriptions();
            } else {
                // output error
                GeneralDialog *gd = new GeneralDialog( "error", tr("link exists"), tr("Link already exists"), tr( "This operations is already linked to the selected prescription. Linking Skipped." ), 0 );
                gd->exec();
            }
        }
    }
}

void AddOperation::appDeleteClicked() {

    // get the button that sent this signal
    QPushButton *b = (QPushButton *) QObject::sender();

    // get the appointment to delete
    Appointment a = DBConn::instance()->appointment( b->objectName().toInt() );

    // ask confirmation
    GeneralDialog *gd = new GeneralDialog( "question", tr("delete link"), tr("Delete link to Appointment"),
                                           tr("Are you sure you wish to delete this link?"
                                              "\n"
                                              "Type: Appointment \n"
                                              "Date: ") + QDate().fromString(a.ap_date, "yyyy-MM-dd").toString("MMM dd, yyyy") +
                                              "\n" + tr("Time: ") + QTime().fromString(a.ap_time, "HH:mm:ss a").toString( "HH:mm" ) +
                                              "\n" + tr("Symptoms:") + "\n" + a.ap_notes, 0 );

    if( gd->exec() == QDialog::Accepted ) {
        // delete and update list
        DBConn::instance()->deleteLink( "appointment", operationID, a.id );

        // refresh list
        loadAttachedAppointments();
    }
}

void AddOperation::imgDeleteClicked() {
    // get the button that sent this signal
    QPushButton *b = (QPushButton *) QObject::sender();

    // get the image to delete
    Image i = DBConn::instance()->image( b->objectName().toInt() );

    // ask confirmation
    GeneralDialog *gd = new GeneralDialog( "question", tr("delete link"), tr("Delete link to Image"),
                                           tr("Are you sure you wish to delete this link?"
                                              "\n"
                                              "Type: Image \n"
                                              "Date: ") + QDate().fromString(i.img_date, "yyyy-MM-dd").toString("MMM dd, yyyy") +
                                              "\n" + i.img_notes, 0 );

    if( gd->exec() == QDialog::Accepted ) {
        // delete and update list
        DBConn::instance()->deleteLink( "image", operationID, i.id );

        // refresh list
        loadAttachedImages();
    }
}

void AddOperation::appInfoClicked() {
    QPushButton *b = (QPushButton *) QObject::sender();

    Appointment a = DBConn::instance()->appointment( b->objectName().toInt() );

    GeneralDialog *gd = new GeneralDialog( "info", tr("appointment details"), tr("Appointment Details"),
                                           tr("Date: ") + QDate().fromString(a.ap_date, "yyyy-MM-dd").toString("MMM dd, yyyy") +
                                              "\n" + tr("Time: ") + QTime().fromString(a.ap_time, "HH:mm:ss a").toString( "HH:mm" ) +
                                              "\n" + tr("Symptoms:") + "\n" + a.ap_notes +
                                              "\n" + tr("Diagnosis:") + "\n" + a.ap_notes2, 0 );

    gd->exec();
}

void AddOperation::imgInfoClicked() {
    QPushButton *b = (QPushButton *) QObject::sender();

    Image i = DBConn::instance()->image( b->objectName().toInt() );

    ViewImage *vi = new ViewImage(this, i.id );
    vi->setOperationDialog( this );

    vi->show();
}

void AddOperation::preDeleteClicked() {
    // get the button that sent this signal
    QPushButton *b = (QPushButton *) QObject::sender();

    // get the prescription to delete
    Prescription p = DBConn::instance()->prescription( b->objectName().toInt() );

    // ask confirmation
    GeneralDialog *gd = new GeneralDialog( "question", tr("delete link"), tr("Delete link to Prescription"),
                                           tr("Are you sure you wish to delete this link?"
                                              "\n"
                                              "Type: Prescription \n"
                                              "Date: ") + QDate().fromString(p.prescr_date, "yyyy-MM-dd").toString("MMM dd, yyyy") +
                                              "\n" + tr("Notes:") + "\n" + p.prescr_notes, 0 );

    if( gd->exec() == QDialog::Accepted ) {
        // delete and update list
        DBConn::instance()->deleteLink( "prescription", operationID, p.id );

        // refresh list
        loadAttachedPrescriptions();
    }
}

void AddOperation::preInfoClicked() {
    QPushButton *b = (QPushButton *) QObject::sender();

    Prescription p = DBConn::instance()->prescription( b->objectName().toInt() );

    PrescriptionDialog *pd = new PrescriptionDialog( this, p.id, "" );

    if( pd->exec() == QDialog::Accepted ) {

        if( pd->action() == 1 ) {
            // confirm first
            GeneralDialog *gd = new GeneralDialog("question", tr("delete prescription"), tr("Deleting prescription..."), tr("Are you sure you wish to delete this prescription?") );

            if( gd->exec() == QDialog::Accepted ) {
                // delete & update list
                DBConn::instance()->deleteLinksWherePrescription( p.id );
                if( DBConn::instance()->deletePrescription(p.id) ) loadAttachedPrescriptions();
            }
        }

        if( pd->action() == 2 ) {
            // update
            p.prescr_text = pd->getPrescriptionText();
            p.prescr_notes = pd->getNotes();

            // perform action & update list
            if( DBConn::instance()->updatePrescription(p) ) {
                loadAttachedPrescriptions();
                // show message
                GeneralDialog *gd = new GeneralDialog("info", tr("change prescription"), QString::fromUtf8("Change Prescription"), QString::fromUtf8("Prescription has been successfully updated!") );
                gd->exec();
            }
        }
    }
}

void AddOperation::writeTempOperation() {
    // write the operation record to a normal object

    Operation op;
    op.id = operationID;
    op.op_date = opDate();
    op.op_notes = opNotes();
    op.op_type = opType();
    op.patient_id = patientID;

    // write data = update the temp operation with the normal data.
    DBConn::instance()->updateOperation( op );
}

void AddOperation::deleteTempOperation() {
    // delete the temporary operation
    DBConn::instance()->deleteLinksOfOperation( operationID );
    DBConn::instance()->deleteOperation( operationID );
}
