#ifndef ADDPATIENT_H
#define ADDPATIENT_H

#include <QWidget>
#include "obj_patient.h"

namespace Ui {
    class AddPatient;
}

class AddPatient : public QWidget
{
    Q_OBJECT

public:
    explicit AddPatient(QWidget *parent = 0, int patient_id = 0);
    ~AddPatient();

private:
    Ui::AddPatient *ui;
    int patientID;
    void loadPatientValues(Patient p);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pat_age_textChanged(const QString &arg1);
    void on_pat_bdate_dateTimeChanged(const QDateTime &dateTime);
};

#endif // ADDPATIENT_H
