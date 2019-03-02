/**************************************************************************
**   Recompile IT Services, Development Dept
**   12/13/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef DIALOG_PATIENTHISTORY_H
#define DIALOG_PATIENTHISTORY_H

#include <QDialog>
#include <QString>
#include "obj_patienthistory.h"

namespace Ui {
    class PatientHistory;
}

class PatientHistory : public QDialog
{
    Q_OBJECT

public:
    explicit PatientHistory(QWidget *parent = 0, int patient_id = 0 );
    QString diseases();
    QString surgeries();
    QString allergies();
    QString other2();
    QString family();
    QString habit_smoking();
    QString habit_sports();
    QString habit_alcohol();
    QString habit_other();
    QString medications();
    QString other();
    int action();
    ~PatientHistory();

private:
    Ui::PatientHistory *ui;
    void loadHistoryValues( PatientHistoryObj h );
    int patientID;
    int i_action;

private slots:
    void updateClicked();
};

#endif // DIALOG_PATIENTHISTORY_H
