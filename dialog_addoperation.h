/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/18/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef ADDOPERATION_H
#define ADDOPERATION_H

#include <QDialog>
#include <QString>

#include "obj_operation.h"
#include "obj_appointment.h"
#include "obj_image.h"
#include "obj_prescription.h"
#include "obj_patient.h"

namespace Ui {
    class AddOperation;
}

class AddOperation : public QDialog
{
    Q_OBJECT

public:
    explicit AddOperation(QWidget *parent = 0, int operation_id = 0, int patient_id = 0);
    QString opType();
    QString opDate();
    QString opNotes();
    ~AddOperation();
    int operationID;
    int patientID;
    int action();
    void deleteTempOperation();
    void writeTempOperation();
    void loadAttachedImages();

private:
    Ui::AddOperation *ui;
    void loadOperationValues(Operation o);
    int i_action;
    void loadAttachedAppointments();
    void loadAttachedPrescriptions();
    QList<Appointment> lst_app;
    QList<Image> lst_img;
    QList<Prescription> lst_pre;
    void clearLayout(QLayout *l);

private slots:
    void on_btn_old_att_prescr_clicked();
    void on_btn_new_att_prescr_clicked();
    void on_btn_old_att_img_clicked();
    void on_btn_new_att_img_clicked();
    void on_btn_old_att_app_clicked();
    void on_btn_new_att_app_clicked();
    void appDeleteClicked();
    void appInfoClicked();
    void imgDeleteClicked();
    void imgInfoClicked();
    void preDeleteClicked();
    void preInfoClicked();
    void deleteClicked();
    void updateClicked();

};

#endif // ADDOPERATION_H
