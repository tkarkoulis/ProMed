/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/14/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef VIEWPATIENT_H
#define VIEWPATIENT_H

#include <QWidget>
#include <QTreeWidgetItem>

namespace Ui {
    class ViewPatient;
}

class ViewPatient : public QWidget
{
    Q_OBJECT

    public:
	explicit ViewPatient(QWidget *parent = 0, int pID=0);
	void reloadPatientImages();
	~ViewPatient();

    private:
	Ui::ViewPatient *ui;
	void loadPatientData(int patID);
	void loadPatientOperations(int patID);
	void loadPatientAppointments(int patID);
	void loadPatientPayments(int patID);
	void loadPatientImages(int patID);
	void loadPatientPrescriptions(int patID);
	void loadPatientHistory(int patID);
        void loadLatestAppointment();
	int patientID;

private slots:
    void on_toolButton_9_clicked();
    void on_pat_prescriptions_itemDoubleClicked(QTreeWidgetItem* item, int column);
    void on_toolButton_8_clicked();
    void on_toolButton_7_clicked();
    void on_pat_payments_itemDoubleClicked(QTreeWidgetItem* item, int column);
    void on_pat_images_itemDoubleClicked(QTreeWidgetItem* item, int column);
    void on_toolButton_6_clicked();
    void on_toolButton_5_clicked();
    void on_toolButton_4_clicked();
    void on_pat_appointments_itemDoubleClicked(QTreeWidgetItem* item, int column);
    void on_toolButton_3_clicked();
    void on_toolButton_2_clicked();
    void on_pat_operations_itemDoubleClicked(QTreeWidgetItem* item, int column);
    void on_toolButton_clicked();
};

#endif // VIEWPATIENT_H
