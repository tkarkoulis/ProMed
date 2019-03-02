
#include <QFileInfo>
#include <QSettings>

#include "dialog_selectdialog.h"
#include "ui_dialog_selectdialog.h"

SelectDialog::SelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectDialog)
{
    ui->setupUi(this);
}

void SelectDialog::setData(QList<Appointment> list) {
    // do something

    for( int i=0; i < list.size(); i++ ) {
        // get the appointment
        Appointment a = (Appointment) list.at(i);

        // ok now make the listwidgetitem
        QTreeWidgetItem *it = new QTreeWidgetItem();
        it->setData( 0, 0, a.ap_date );
        it->setData( 1, 0, a.ap_time );
        it->setData( 2, 0, a.ap_notes );
        it->setToolTip( 0, QString::number(a.id) );

        ui->treeWidget->addTopLevelItem(it);
    }
}

void SelectDialog::setData(QList<Image> list) {
    // do something

    QSettings settings;

    // init column names
    QStringList columns;
    columns << tr("Preview") << tr("Date") << tr("Notes");
    ui->treeWidget->setHeaderLabels( columns );

    for( int z=0; z < list.size(); z++ ) {
        // get the appointment
        Image i = (Image) list.at(z);

        QFileInfo fi_file = settings.value("data_path").toString() + "/" + i.img_name;
        QPixmap pix = QPixmap( settings.value("data_path").toString() + "/" + fi_file.baseName() + "-thumb." + fi_file.suffix() );

        // ok now make the listwidgetitem
        QTreeWidgetItem *it = new QTreeWidgetItem();
        it->setData( 0, Qt::DecorationRole, pix );
        it->setData( 1, 0, i.img_date );
        it->setData( 2, 0, i.img_notes );
        it->setToolTip( 0, QString::number(i.id) );

        ui->treeWidget->addTopLevelItem(it);
    }
}

void SelectDialog::setData(QList<Prescription> list) {
    // do something

    // set the right column numbers
    ui->treeWidget->setColumnCount(2);
    // init column names
    QStringList columns;
    columns << tr("Date") << tr("Notes");
    ui->treeWidget->setHeaderLabels( columns );

    for( int i=0; i < list.size(); i++ ) {
        // get the prescription
        Prescription p = (Prescription) list.at(i);

        // ok now make the listwidgetitem
        QTreeWidgetItem *it = new QTreeWidgetItem();
        it->setData( 0, 0, p.prescr_date );
        it->setData( 1, 0, p.prescr_notes );
        it->setToolTip( 0, QString::number(p.id) );

        ui->treeWidget->addTopLevelItem(it);
    }
}

QString SelectDialog::getSelectedID() {
    return selectedID;
}

SelectDialog::~SelectDialog()
{
    delete ui;
}

void SelectDialog::on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
    // accept the dialog
    this->accept();
}

void SelectDialog::on_treeWidget_itemSelectionChanged()
{
    selectedID = ui->treeWidget->currentItem()->toolTip(0);
}
