/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/18/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef DIALOG_ADDAPPOINTMENT_H
#define DIALOG_ADDAPPOINTMENT_H

#include <QDialog>
#include <QDate>
#include <QTime>
#include "obj_appointment.h"

namespace Ui {
    class AddAppointment;
}

class AddAppointment : public QDialog
{
    Q_OBJECT

public:
    explicit AddAppointment(QWidget *parent = 0, int appointment_id = 0);
    QDate getDate();
    QTime getTime();
    QString getNotes();
    QString getNotes2();
    int appointmentID;
    int action();
    ~AddAppointment();

private:
    Ui::AddAppointment *ui;
    void loadAppointmentValues(Appointment a);
    int i_action;

private slots:
    void deleteClicked();
    void updateClicked();
};

#endif // DIALOG_ADDAPPOINTMENT_H
