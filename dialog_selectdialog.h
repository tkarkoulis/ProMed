#ifndef DIALOG_SELECTDIALOG_H
#define DIALOG_SELECTDIALOG_H

#include <QDialog>
#include <QList>
#include <QTreeWidgetItem>

#include "obj_appointment.h"
#include "obj_image.h"
#include "obj_prescription.h"

namespace Ui {
    class SelectDialog;
}

class SelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectDialog(QWidget *parent = 0);
    ~SelectDialog();
    void setData( QList<Appointment> list );
    void setData( QList<Image> list );
    void setData( QList<Prescription> list );
    QString getSelectedID();

private:
    Ui::SelectDialog *ui;
    QString selectedID;


private slots:
    void on_treeWidget_itemSelectionChanged();
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column);
};

#endif // DIALOG_SELECTDIALOG_H
