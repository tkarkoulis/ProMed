/**************************************************************************
**   Recompile IT Services, Development Dept
**   11/1/2010 2010
**   All Rights Reserved
**************************************************************************/

#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QPushButton>

#include "dialog_viewimage.h"
#include "dialog_generaldialog.h"
#include "ui_dialog_viewimage.h"
#include "dbconn.h"
#include "obj_image.h"

ViewImage::ViewImage(QWidget *parent,int image_id, ViewPatient *p ) :
    QDialog(parent),
    ui(new Ui::ViewImage)
{
    ui->setupUi(this);

    //this->setWindowFlags( Qt::SplashScreen );

    // load the qsetting
    QSettings setting;

    // flag
    i_action = 0;
    imageID = image_id;
    vp = p;
    aop = 0;

    // load the image from db and display it
    Image img = DBConn::instance()->image(image_id);

    QString img_url = setting.value( "data_path" ).toString() + "/" + img.img_name;

    QLabel *l = new QLabel();
    l->setPixmap( QPixmap(img_url) );

    // qimage
    ui->qImageLayout->addWidget( l );

    ui->img_descr->setText( img.img_notes );

    // fix button
    QPushButton *but_cancel = ui->buttonBox->button( QDialogButtonBox::Close );
    but_cancel->setIcon( QIcon(":/icons/Reset 16x16") );
    QPushButton *but_delete = ui->buttonBox->addButton( tr("Delete"), QDialogButtonBox::AcceptRole );
    but_delete->setIcon( QIcon(":/icons/Cancel 16x16") );

    but_cancel->setText( tr("Cancel") );

    connect( but_delete, SIGNAL(clicked()), this, SLOT(deleteClicked()) );
}

void ViewImage::deleteClicked() {
    // set flag
    i_action = 1; // delete

	GeneralDialog *gd2 = new GeneralDialog("question", tr("delete image"), tr("Delete Image?"), tr("Are you sure you wish to delete this image?") );

	if( gd2->exec() == QDialog::Accepted ) {

	Image i = DBConn::instance()->image(imageID);

	// delete the physical file
	QSettings setting;

	// DATA SECURITY CHECK
	if( setting.value("data_path").toString().isEmpty()  ) return;

	QString img_url = setting.value( "data_path" ).toString() + "/" + i.img_name;

	QFileInfo fi_file = img_url;
	QFile ft( setting.value("data_path").toString() + "/" + fi_file.baseName() + "-thumb." + fi_file.suffix() );

	QFile f( img_url );

	if( f.remove() && ft.remove() ) {

            DBConn::instance()->deleteLinksWhereImage( i.id );
	    DBConn::instance()->deleteImage( i.id );

            if( vp ) vp->reloadPatientImages();
            if( aop ) aop->loadAttachedImages();

	    GeneralDialog *gd = new GeneralDialog("info", tr("delete image"), tr("Delete Image"), tr("Image has been successfully Deleted!") );
	    gd->exec();
	}
    }
}

int ViewImage::action() {
    return i_action;
}

void ViewImage::setOperationDialog(AddOperation *ao) {
    aop = ao;
}

ViewImage::~ViewImage()
{
    delete ui;
}
