/**************************************************************************
**   Recompile IT Services, Development Dept
**   12/1/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef DIALOG_PRESCRIPTIONDIALOG_H
#define DIALOG_PRESCRIPTIONDIALOG_H

#include <QDialog>
#include "obj_prescription.h"

namespace Ui {
    class PrescriptionDialog;
}

class PrescriptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrescriptionDialog(QWidget *parent = 0, int prescription_id = 0, QString name = "" );
    QString getPrescriptionText();
    QString getNotes();
    int action();
    void print();
    ~PrescriptionDialog();

private:
    Ui::PrescriptionDialog *ui;
    int prescriptionID;
    int i_action; //flag
    void loadPrescriptionData( Prescription p );
    QString printTemplate;

private slots:
    void deleteClicked();
    void updateClicked();
    void printClicked();
};

#endif // DIALOG_PRESCRIPTIONDIALOG_H
