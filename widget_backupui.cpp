/**************************************************************************
**   Recompile IT Services, Development Dept
**   11/22/2010 2010
**   All Rights Reserved
**************************************************************************/

#include <QFileDialog>
#include <QDebug>
#include <QDate>

#include "widget_backupui.h"
#include "ui_widget_backupui.h"
#include "tool_backupengine.h"

BackupUI::BackupUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BackupUI)
{
    ui->setupUi(this);

    ui->horizontalLayout_4->setAlignment( ui->pushButton, Qt::AlignRight );
    ui->horizontalLayout_4->setAlignment( ui->pushButton_2, Qt::AlignLeft );

    // TODO - Implement REMOTE backup
    ui->radioButton->hide();
    ui->radioButton_2->hide();
    ui->radioButton_2->setChecked(true);

    // you must hide the label for the status and the progressbar
    ui->bak_label->hide();
    ui->bak_progress->hide();
    ui->bak_progress->setValue(0); // set default value
}

BackupUI::~BackupUI()
{
    delete ui;
}

void BackupUI::on_pushButton_clicked() {
    // start backup

    // show bar & label
    ui->bak_label->setText( "(##%) " + tr("Starting backup...") );
    ui->bak_label->setText( ui->bak_label->text().replace("##", QString::number( ui->bak_progress->value() )) );

    ui->bak_label->show();
    ui->bak_progress->show();

    // make checkboxes & button disabled
    //ui->radioButton->setEnabled(false);
    //ui->radioButton_2->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);

    if( ui->radioButton->isChecked() ) {
	// Online Backup
	qDebug() << "Online Backup";
    }

    if( ui->radioButton_2->isChecked() ) {
	// Local Backup
	ui->bak_progress->setValue(1);

	// step progress bar
	ui->bak_progress->setValue( ui->bak_progress->value()+1 );
	// step label
	ui->bak_label->setText( "(##%) " + tr("Choosing destination file...") );
	ui->bak_label->setText( ui->bak_label->text().replace("##", QString::number( ui->bak_progress->value() )) );

	// select the file where we will write the XML data
	QString filename = QFileDialog::getSaveFileName(this,
						tr("save backup file"),
                                                "/promed_backup-"+QDate().currentDate().toString("yyyy-MM-dd") + " " +
                                                QTime().currentTime().toString( "HH-mm" ),
						tr("ProMed Backup Files (*.pdk)" ) );

	if( !filename.isEmpty() ) { // check given file
	    // create the xml

	    // instantiate the backupengine tool, pass it the progressbar and label, and let it create the backup.
	    BackupEngine *beng = new BackupEngine( ui->bak_label, ui->bak_progress );

	    if( beng->createXMLBackup(filename) ) {
		// backup has been created
		ui->bak_progress->setValue(100);
		ui->bak_label->setText( "(##%) " + tr("Done!") );
		ui->bak_label->setText( ui->bak_label->text().replace("##", QString::number( ui->bak_progress->value() )) );

		// re-enable backup buttons
		ui->pushButton->setEnabled(true);
		ui->pushButton_2->setEnabled(true);
		//ui->radioButton->setEnabled(true);
		//ui->radioButton_2->setEnabled(true);
	    }
	} else {
	    // reset things
	    ui->bak_progress->setValue(1);
	    ui->bak_progress->hide();
	    ui->bak_label->hide();
	    ui->pushButton->setEnabled(true);
	    ui->pushButton_2->setEnabled(true);
	}
    }
}

void BackupUI::on_pushButton_2_clicked() {
    // open xml file to restore and restore.

    // show bar & label
    ui->bak_progress->setValue(1);

    ui->bak_label->setText( "(##%) " + tr("Starting backup import...") );
    ui->bak_label->setText( ui->bak_label->text().replace("##", QString::number( ui->bak_progress->value() )) );

    ui->bak_label->show();
    ui->bak_progress->show();

    // make checkboxes & button disabled
    //ui->radioButton->setEnabled(false);
    //ui->radioButton_2->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);

    // step progress bar
    ui->bak_progress->setValue( ui->bak_progress->value()+1 );
    // step label
    ui->bak_label->setText( "(##%) " + tr("Import backup file...") );
    ui->bak_label->setText( ui->bak_label->text().replace("##", QString::number( ui->bak_progress->value() )) );

    // select the file where we will write the XML data
    QString filename = QFileDialog::getOpenFileName(this,
					    tr("open backup file"),
					    "/",
					    tr("ProMed Backup Files (*.pdk)" ) );

    if( !filename.isEmpty() ) { // check given file
	// create the xml

	// instantiate the backupengine tool, pass it the progressbar and label, and let it import the backup.
	BackupEngine *beng = new BackupEngine( ui->bak_label, ui->bak_progress );

	if( beng->importXMLBackup(filename) ) {
		// backup has been created
		ui->bak_progress->setValue(100);
		ui->bak_label->setText( "(##%) " + tr("Done!") );
		ui->bak_label->setText( ui->bak_label->text().replace("##", QString::number( ui->bak_progress->value() )) );

		// re-enable backup buttons
		ui->pushButton->setEnabled(true);
		ui->pushButton_2->setEnabled(true);
		//ui->radioButton->setEnabled(true);
		//ui->radioButton_2->setEnabled(true);
	    }

    } else {
	    // reset things
	    ui->bak_progress->setValue(1);
	    ui->bak_progress->hide();
	    ui->bak_label->hide();
	    ui->pushButton->setEnabled(true);
	    ui->pushButton_2->setEnabled(true);
	}
}
