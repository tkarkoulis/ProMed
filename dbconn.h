/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/12/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef DBCONN_H
#define DBCONN_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>
#include <QDate>
#include "obj_patient.h"
#include "obj_operation.h"
#include "obj_appointment.h"
#include "obj_setting.h"
#include "obj_payment.h"
#include "obj_image.h"
#include "obj_prescription.h"
#include "obj_patienthistory.h"

class DBConn : public QObject
{
    Q_OBJECT
    QSqlDatabase dbc;
    static DBConn *s_instance;

    public:
	explicit DBConn(QObject *parent = 0);
	~DBConn();
	// singleton instance
	static DBConn *instance() {
	    if (!s_instance)
	      s_instance = new DBConn;
	    return s_instance;
	}
	// patients
	int insertPatient( Patient &pat );
	bool updatePatient( Patient &pat );
	bool deletePatient( int id );
	QList<Patient> patients();
	QList<Patient> patients(QString name, QString surname, QString fathersName, QString telephone );
	QList<Patient> patients(QString created_from, QString created_to);
	int patientVisits(QString date_from, QString date_to);
	Patient patient(int patient_id);
	// operations
	bool insertOperation( Operation &op );
	bool updateOperation( Operation &op );
	QList<Operation> operations( int patient_id );
	QList<Operation> operations( QString date_from, QString date_to );
        QList<Patient> operationPatients( QString date_from, QString date_to );
	QStringList operationTypes();
	Operation operation( int id );
	bool deleteOperation( int id );
	// appointments
	bool insertAppointment( Appointment &ap );
	bool updateAppointment( Appointment &ap );
	bool deleteAppointment( int id );
	QList<Appointment> appointments();
	QList<Appointment> appointments( int patient_id );
	QList<Appointment> appointments( QString ap_date );
	QList<Appointment> appointments( QString ap_date_start, QString ap_date_end );
	Appointment appointment( int id );
        Appointment latestAppointment( int patient_id );
	// payments
	bool insertPayment( Payment &pay );
	bool updatePayment( Payment &pay );
	bool deletePayment( int id );
	QList<Payment> payments();
	QList<Payment> payments( int patient_id );
	QList<Payment> payments( QString pay_date );
	QList<Payment> payments( QString pay_date_start, QString pay_date_end );
	Payment payment( int id );
	// images
	bool insertImage( Image &img );
	bool updateImage( Image &img );
	bool deleteImage( int id );
	QList<Image> images();
	QList<Image> images( int patient_id );
	QList<Image> images( QString img_date );
	QList<Image> images( QString img_date_start, QString img_date_end );
	Image image( int id );
	// prescriptions
	bool insertPrescription( Prescription &pre );
	bool updatePrescription( Prescription &pre );
	bool deletePrescription( int id );
	QList<Prescription> prescriptions();
	QList<Prescription> prescriptions( int patient_id );
	QList<Prescription> prescriptions( QString pre_date );
	QList<Prescription> prescriptions( QString pre_date_start, QString pre_date_end );
	Prescription prescription( int id );
	// patient history
	bool insertPatientHistory( PatientHistoryObj &ph );
	bool updatePatientHistory( PatientHistoryObj &ph );
	bool deletePatientHistory( int id );
	PatientHistoryObj patienthistory( int patient_id );
	// settings
	bool insertSetting( Setting &set );
	bool updateSetting( Setting &set );
	int settingExists( QString setting );
	Setting setting( QString setting );
	QString settingValue( QString setting );
        // operation_associations
        bool linkOperation( QString type, int from_id, int to_id );
        QList<Appointment> linkedAppointments( int op_id );
        QList<Image> linkedImages( int op_id );
        QList<Prescription> linkedPrescriptions( int op_id );
        bool deleteLink( QString type, int from_id, int to_id );
        bool deleteLinksOfOperation( int operation_id );
        bool deleteLinksWhereAppointment( int appointment_id );
        bool deleteLinksWhereImage( int image_id );
        bool deleteLinksWherePrescription( int prescription_id );
        bool linkExists( QString type, int from_id, int to_id );
        // statistics
        QStringList operationTypes(QDate month);
        int operationsInMonth(QDate month);
        int appointmentsInDay( QString ap_date );
        int appointmentsInMonth( QDate month );
        int maxAppointmentsPerDayPerMonth( QString ap_date );
        QList<Patient> patientsFromDemo( QString name, QString surname, QString gender, QString insurance, QString city, QString birthdate);
	// general stuff
	QString defaultCurrency( bool symbol );
	QString databaseVersion();
    QStringList get_nationalities();
        void checkUpdateDatabase();
        bool demo();

    private:
        bool isDemo;
};

#endif // DBCONN_H
