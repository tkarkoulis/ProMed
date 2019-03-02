/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/12/2010 2010
**   All Rights Reserved
**************************************************************************/

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QDate>
#include <QMultiMap>
#include <QSettings>
#include "dbconn.h"

// start database connection
DBConn *DBConn::s_instance = 0;

DBConn::DBConn(QObject *parent) :
    QObject(parent)
{

    QSettings settings;
    QString db_driver = "QSQLITE";

    if( db_driver == "QMYSQL" ) {

        dbc = QSqlDatabase::addDatabase("QMYSQL");
        dbc.setHostName("10.0.0.4");
        dbc.setDatabaseName("testDB");
        dbc.setUserName("promed_u1");
        dbc.setPassword("promed1234");

    } else {

        dbc = QSqlDatabase::addDatabase("QSQLITE");
        dbc.setDatabaseName( settings.value("resources_path").toString() + "/db/ProMed.dpdata" );

        qDebug() << "Database should be located at " << settings.value("resources_path").toString() + "/db/ProMed.dpdata";
    }

    // check status
    if( dbc.open() ) {

	// database has been opened, CHECK FOR DB UPDATES
	checkUpdateDatabase();
	qDebug() << "DB File Loaded - at latest Version";

    QSqlQuery query;
    query.exec("SELECT * FROM appointments");

    while (query.next()) {
             qDebug() << query.value(0) << query.value(1) << query.value(2) << query.value(3);
         }

    } else {
         qDebug() << "file NOT opened! - should be at: ";
    }

    this->isDemo = false;
}

bool DBConn::demo() {
    return this->isDemo;
}

int DBConn::insertPatient( Patient &pat ) {

    qDebug() << "adding patient...";

    // Insert the patient record here
    QSqlQuery query( QSqlDatabase::database() );

    if( pat.id > 0 ) {
	query.prepare( "insert into patients ("
			"id,"
			"name,"
			"surname,"
			"fathersName,"
			"vat,"
			"tel1,"
			"tel1_type,"
			"tel2,"
			"tel2_type,"
			"tel3,"
			"tel3_type,"
			"address,"
			"birthdate,"
			"gender,"
			"insurance,"
			"email,"
			"notes,"
			"amka,"
            "created_on,"
           "cancer_trace,"
           "children,"
           "nationality,"
           "inheritance"


			") values ( :id, :name, :surname, :fathersName, :vat, :tel1, :tel1_type, :tel2, :tel2_type, :tel3, :tel3_type, "
            ":address, :birthdate, :gender, :insurance, :email, :notes, :amka, :created_on, :cancer_trace, :children, :nationality ,:inheritance  )" );

	query.bindValue( ":id", pat.id );
    } else {
	query.prepare( "insert into patients ("
			"name,"
			"surname,"
			"fathersName,"
			"vat,"
			"tel1,"
			"tel1_type,"
			"tel2,"
			"tel2_type,"
			"tel3,"
			"tel3_type,"
			"address,"
			"birthdate,"
			"gender,"
			"insurance,"
			"email,"
			"notes,"
			"amka,"
            "created_on,"
           "cancer_trace,"
           "children,"
           "nationality,"
           "inheritance"

			") values ( :name, :surname, :fathersName, :vat, :tel1, :tel1_type, :tel2, :tel2_type, :tel3, :tel3_type, "
            ":address, :birthdate, :gender, :insurance, :email, :notes, :amka, :created_on , :cancer_trace, :children, :nationality ,:inheritance )" );
    }

    query.bindValue( ":name", pat.name );
    query.bindValue( ":surname", pat.surname );
    query.bindValue( ":fathersName", pat.fathersName );
    query.bindValue( ":vat", pat.vat );
    query.bindValue( ":tel1", pat.tel1 );
    query.bindValue( ":tel1_type", pat.tel1_type );
    query.bindValue( ":tel2", pat.tel2 );
    query.bindValue( ":tel2_type", pat.tel2_type );
    query.bindValue( ":tel3", pat.tel3 );
    query.bindValue( ":tel3_type", pat.tel3_type );
    query.bindValue( ":address", pat.address );
    query.bindValue( ":birthdate", pat.birthdate );
    query.bindValue( ":gender", pat.gender );
    query.bindValue( ":insurance", pat.insurance );
    query.bindValue( ":email", pat.email );
    query.bindValue( ":notes", pat.notes );
    query.bindValue( ":amka", pat.amka );
    query.bindValue( ":created_on", pat.created_on );

    query.bindValue( ":cancer_trace", pat.cancer_trace );
    query.bindValue( ":children", pat.children );
    query.bindValue( ":nationality", pat.nationality );
    query.bindValue( ":inheritance", pat.inheritance );

    if( query.exec() ) {
	return query.lastInsertId().toInt();
    } else {
        qDebug() << "failed: " << query.lastError();
	return 0;
    }
}

bool DBConn::updatePatient( Patient &pat ) {

    qDebug() << "updating patient...";

    // Insert the patient record here
    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "update patients set"
		    " name=:name, surname=:surname, fathersName=:fathersName, vat=:vat, tel1=:tel1, tel1_type=:tel1_type, tel2=:tel2, tel2_type=:tel2_type, "
		    "tel3=:tel3, tel3_type=:tel3_type, address=:address, birthdate=:birthdate, gender=:gender, insurance=:insurance, email=:email, notes=:notes, amka=:amka"
            "nationality=:nationality, children=:children, inheritance=:inheritance, cancer_trace=:cancer_trace"
		    " where id=:id" );

    query.bindValue( ":id", pat.id );
    query.bindValue( ":name", pat.name );
    query.bindValue( ":surname", pat.surname );
    query.bindValue( ":fathersName", pat.fathersName );
    query.bindValue( ":vat", pat.vat );
    query.bindValue( ":tel1", pat.tel1 );
    query.bindValue( ":tel1_type", pat.tel1_type );
    query.bindValue( ":tel2", pat.tel2 );
    query.bindValue( ":tel2_type", pat.tel2_type );
    query.bindValue( ":tel3", pat.tel3 );
    query.bindValue( ":tel3_type", pat.tel3_type );
    query.bindValue( ":address", pat.address );
    query.bindValue( ":birthdate", pat.birthdate );
    query.bindValue( ":gender", pat.gender );
    query.bindValue( ":insurance", pat.insurance );
    query.bindValue( ":email", pat.email );
    query.bindValue( ":notes", pat.notes );
    query.bindValue( ":amka", pat.amka );

    query.bindValue( ":cancer_trace", pat.cancer_trace );
    query.bindValue( ":inheritance", pat.inheritance );
    query.bindValue( ":children", pat.children );
    query.bindValue( ":nationality", pat.nationality );

    if( query.exec() ) {
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }
}

QList<Patient> DBConn::patients() {
    // init
    QList<Patient> patients;

    // get the list of patients and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select name,surname,fathersName,vat,tel1,tel1_type,tel2,tel2_type,tel3,tel3_type,address,birthdate,gender,insurance,email,notes,id,created_on,amka,"
               "cancer_trace,children,nationality,inheritance from patients");

    // loop
    while( query.next() ) {
        // create the Patient Record
        Patient p;
        p.name = query.value(0).toString();
        p.surname = query.value(1).toString();
        p.fathersName = query.value(2).toString();
        p.vat = query.value(3).toString();
        p.tel1 = query.value(4).toString();
        p.tel1_type = query.value(5).toString();
        p.tel2 = query.value(6).toString();
        p.tel2_type = query.value(7).toString();
        p.tel3 = query.value(8).toString();
        p.tel3_type = query.value(9).toString();
        p.address = query.value(10).toString();
        p.birthdate = query.value(11).toString();
        p.gender = query.value(12).toString();
        p.insurance = query.value(13).toString();
        p.email = query.value(14).toString();
        p.notes = query.value(15).toString();
        p.id = query.value(16).toInt();
        p.created_on = query.value(17).toString();
        p.amka = query.value(18).toString();

        p.cancer_trace = query.value(19).toString();
        p.children = query.value(20).toString();
        p.nationality = query.value(21).toString();
        p.inheritance = query.value(22).toString();



        // add to stack
        patients.append( p );
    }

    // return
    return patients;
}

QList<Patient> DBConn::patients(QString name, QString surname, QString fathersName, QString telephone) {
    // init
    QList<Patient> patients;

    // get the list of patients and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // create the WHERE clause
    QString s_where = "where ";
    bool b_hasPrev = false; // flag for AND  keyword

    // if the name is there
    if( !name.isEmpty() ) {
	s_where += "upper(name) like upper('%" + name + "%') ";
	b_hasPrev = true; // set flag
    }

    QMessageBox::warning(0, "", surname.toLower() );

    // if the surname is there
    if( !surname.isEmpty() ) {
	if( b_hasPrev ) s_where += "and ";
	s_where += "lower(surname) like '%" + surname.toLower() + "%' ";
	b_hasPrev = true;
    }

    // if fathersname isthere
    if( !fathersName.isEmpty() ) {
	if( b_hasPrev ) s_where += "and ";
	s_where += "fathersName like '%" + fathersName + "%' ";
	b_hasPrev = true;
    }

    // if telephone is there
    if( !telephone.isEmpty() ) {
	if( b_hasPrev ) s_where += "and ";
	s_where += "( lower(tel1) like lower('" + telephone + "%') or lower(tel2) like lower('" + telephone + "%') or lower(tel3) like lower('" + telephone + "%') ) ";
	b_hasPrev = true;
    }

    // clean up
    if( s_where == "where " ) s_where = "";

    // execute select statement
    query.exec(  QString::fromUtf8( "select name,upper(surname),fathersName,vat,tel1,tel1_type,tel2,tel2_type,tel3,tel3_type,"
		    "address,birthdate,gender,insurance,email,notes,id,created_on,amka from patients " ) + s_where );

    // loop
    while( query.next() ) {
	// create the Patient Record
	Patient p;
	p.name = query.value(0).toString();
	p.surname = query.value(1).toString();
	p.fathersName = query.value(2).toString();
	p.vat = query.value(3).toString();
	p.tel1 = query.value(4).toString();
	p.tel1_type = query.value(5).toString();
	p.tel2 = query.value(6).toString();
	p.tel2_type = query.value(7).toString();
	p.tel3 = query.value(8).toString();
	p.tel3_type = query.value(9).toString();
	p.address = query.value(10).toString();
	p.birthdate = query.value(11).toString();
	p.gender = query.value(12).toString();
	p.insurance = query.value(13).toString();
	p.email = query.value(14).toString();
	p.notes = query.value(15).toString();
	p.id = query.value(16).toInt();
	p.created_on = query.value(17).toString();
	p.amka = query.value(18).toString();

	// add to stack
	patients.append( p );
    }

    // return
    return patients;
}

QList<Patient> DBConn::patients(QString created_from, QString created_to) {
    // init
    QList<Patient> patients;

    // get the list of patients and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select name,surname,fathersName,vat,tel1,tel1_type,tel2,tel2_type,tel3,tel3_type,address,birthdate,gender,insurance,email,notes,id,created_on,amka from patients"
		" where (created_on > '" + created_from + "' and created_on < '" + created_to + "' ) or (created_on = '"+created_from+"') or (created_on = '"+created_to+"' )" );

    // loop
    while( query.next() ) {
	// create the Patient Record
	Patient p;
	p.name = query.value(0).toString();
	p.surname = query.value(1).toString();
	p.fathersName = query.value(2).toString();
	p.vat = query.value(3).toString();
	p.tel1 = query.value(4).toString();
	p.tel1_type = query.value(5).toString();
	p.tel2 = query.value(6).toString();
	p.tel2_type = query.value(7).toString();
	p.tel3 = query.value(8).toString();
	p.tel3_type = query.value(9).toString();
	p.address = query.value(10).toString();
	p.birthdate = query.value(11).toString();
	p.gender = query.value(12).toString();
	p.insurance = query.value(13).toString();
	p.email = query.value(14).toString();
	p.notes = query.value(15).toString();
	p.id = query.value(16).toInt();
	p.created_on = query.value(17).toString();
	p.amka = query.value(18).toString();

	// add to stack
	patients.append( p );
    }

    // return
    return patients;
}

int DBConn::patientVisits(QString date_from, QString date_to) {
    // create SQL
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select patient_id,op_date from operations"
		" where (op_date > '" + date_from + "' and op_date < '" + date_to + "' ) or (op_date = '"+date_from+"') or (op_date = '"+date_to+"' )" );

    // prepare list
    QMultiMap<int,QString> tmplist;

    // move cursor
    while( query.next() ) {
	// get the id
	int pat_id = query.value(0).toInt();
	// get the date
	QString pat_date = query.value(1).toString();

	// if the id & date exists in our array, dont insert
	tmplist.insert( pat_id, pat_date );
    }

    // get unique keys
    QList<int> u_keys = tmplist.uniqueKeys();
    QList<QString> final_apps;

    for( int i = 0; i < u_keys.size(); i++ ) {
	// get the unique values
	QSet<QString> u_dates = tmplist.values( u_keys.at(i) ).toSet();
	// append to total
	final_apps.append( u_dates.toList() );
    }

    // get the count value
    return final_apps.size();
}

Patient DBConn::patient(int patient_id) {
    // return the patiend for this id, or false;

    // get the list of patients and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select name,surname,fathersName,vat,tel1,tel1_type,tel2,tel2_type,tel3,tel3_type,address,birthdate,gender,insurance,email,notes,id,created_on,amka,"
        "cancer_trace,children,nationality,inheritance"
               " from patients "
		" where id=" + QString::number(patient_id) + " limit 1" );

    // get record
    query.next();



    // assign values
    Patient p;
    p.name = query.value(0).toString();
    p.surname = query.value(1).toString();
    p.fathersName = query.value(2).toString();
    p.vat = query.value(3).toString();
    p.tel1 = query.value(4).toString();
    p.tel1_type = query.value(5).toString();
    p.tel2 = query.value(6).toString();
    p.tel2_type = query.value(7).toString();
    p.tel3 = query.value(8).toString();
    p.tel3_type = query.value(9).toString();
    p.address = query.value(10).toString();
    p.birthdate = query.value(11).toString();
    p.gender = query.value(12).toString();
    p.insurance = query.value(13).toString();
    p.email = query.value(14).toString();
    p.notes = query.value(15).toString();
    p.id = query.value(16).toInt();
    p.created_on = query.value(17).toString();
    p.amka = query.value(18).toString();

    p.cancer_trace = query.value(19).toString();
    p.children = query.value(20).toString();
    p.nationality = query.value(21).toString();
    p.inheritance = query.value(22).toString();

    for(int i=0;i<23;i++)
         qDebug() <<i<<':' << query.value(i).toString() <<endl;

    return p;
}

QList<Patient> DBConn::patientsFromDemo(QString name, QString surname, QString gender, QString insurance, QString city, QString birthdate) {

    // prepare the List
    QList<Patient> patients;

    // create the query
    QSqlQuery query( QSqlDatabase::database() );

    // prepare sql

    QString sql = "select id from patients where lower(name) like '%" + name
                   + "%' and lower(surname) like '%" + surname
                   + "%' and gender like '%" + gender
                   + "%' and insurance like '%" + insurance
                   + "%' and address like '%" + city
                   + "%' and birthdate like '%" + birthdate + "%' order by name,surname asc";

    query.exec( sql );

    while( query.next() ) {
        Patient p = patient( query.value(0).toInt() );

        patients.append( p );
    }

    return patients;
}

bool DBConn::deletePatient( int id ) {
    // delete patient

    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "delete from patients where id=:id" );

    query.bindValue( ":id", id );

    if( query.exec() ) {
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }
}

bool DBConn::insertOperation( Operation &op ) {
    qDebug() << "adding operation...";

    // Insert the operation record here
    QSqlQuery query( QSqlDatabase::database() );

    if( op.id > 0 ) {
        query.prepare( "insert into operations ("
                           "id,"
                            "patient_id,"
                            "op_date,"
                            "op_type,"
                            "op_notes"
                            ") values ( :id, :patient_id, :op_date, :op_type, :op_notes )" );

        query.bindValue( ":id", op.id );

    } else {
        query.prepare( "insert into operations ("
                            "patient_id,"
                            "op_date,"
                            "op_type,"
                            "op_notes"
                            ") values ( :patient_id, :op_date, :op_type, :op_notes )" );
    }

    query.bindValue( ":patient_id", op.patient_id );
    query.bindValue( ":op_date", op.op_date );
    query.bindValue( ":op_type", op.op_type );
    query.bindValue( ":op_notes", op.op_notes );

    if( query.exec() ) {
        op.id = query.lastInsertId().toInt();
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }
}

bool DBConn::updateOperation( Operation &op ) {
    qDebug() << "updating operation...";

    // Update the operation record here
    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "update operations set "
		    "patient_id=:patient_id,"
		    "op_date=:op_date,"
		    "op_type=:op_type,"
		    "op_notes=:op_notes"
		    " where id=:id" );

    query.bindValue( ":id", op.id );
    query.bindValue( ":patient_id", op.patient_id );
    query.bindValue( ":op_date", op.op_date );
    query.bindValue( ":op_type", op.op_type );
    query.bindValue( ":op_notes", op.op_notes );

    if( query.exec() ) {
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }
}

QList<Operation> DBConn::operations( int patient_id ) {
    // init
    QList<Operation> operations;

    // get the list of patients and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,op_date,op_type,op_notes from operations where patient_id=" + QString::number(patient_id) + " order by op_date desc" );

    // loop
    while( query.next() ) {
	// create the Operation Record
	Operation p;
	p.id = query.value(0).toInt();
	p.patient_id = query.value(1).toInt();
	p.op_date = query.value(2).toString();
	p.op_type = query.value(3).toString();
	p.op_notes = query.value(4).toString();

	// add to stack
	operations.append( p );
    }

    // return
    return operations;
}

QList<Operation> DBConn::operations(QString date_from, QString date_to) {
    // init
    QList<Operation> operations;

    // get the list of patients and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,op_date,op_type,op_notes from operations where op_date > '" + date_from + "' and op_date < '" + date_to + "' or (op_date = '"+date_from+"') or (op_date = '"+date_to+"' )" );

    // loop
    while( query.next() ) {
	// create the Operation Record
	Operation p;
	p.id = query.value(0).toInt();
	p.patient_id = query.value(1).toInt();
	p.op_date = query.value(2).toString();
	p.op_type = query.value(3).toString();
	p.op_notes = query.value(4).toString();

	// add to stack
	operations.append( p );
    }

    // return
    return operations;
}

QList<Patient> DBConn::operationPatients( QString date_from, QString date_to ) {
    // init
    QList<Patient> operations;

    // get the list of patients and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select distinct patient_id from operations where op_date > '" + date_from + "' and op_date < '" + date_to + "' or (op_date = '"+date_from+"') or (op_date = '"+date_to+"' )" );

    // loop
    while( query.next() ) {
        // create the Operation Record
        Patient p = patient( query.value(0).toInt() );

        // add to stack
        operations.append( p );
    }

    // return
    return operations;
}

QStringList DBConn::operationTypes() {
    // get the list of distinct operation types and return it
    QStringList operations;

    // load first value empty
    operations.append("");

    // get the list of patients and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select distinct op_type from operations" );

    // loop
    while( query.next() ) {
	// create the Operation Record
	QString p;
	p = query.value(0).toString();

	// add to stack
	operations.append( p );
    }

    // return
    return operations;
}

QStringList DBConn::operationTypes(QDate month) {
    // get the list of distinct operation types and return it
    QStringList operations;

    // get the list of patients and return in a QList
    QSqlQuery query( QSqlDatabase::database() );
    QSqlQuery query_count( QSqlDatabase::database() );

    // execute select statement
    query.prepare("select distinct op_type from operations where op_date >= :date_from and op_date <= :date_to order by count(op_type) desc" );
    query_count.prepare("select count(id) as cc from operations where op_date >= :date_from and op_date <= :date_to and op_type = :op_type" );

    query.bindValue( ":date_from", month.toString("yyyy-MM-01") );
    query.bindValue( ":date_to", month.toString("yyyy-MM-") + QString::number( month.daysInMonth() ) );
    query_count.bindValue( ":date_from", month.toString("yyyy-MM-01") );
    query_count.bindValue( ":date_to", month.toString("yyyy-MM-") + QString::number( month.daysInMonth() ) );

    query.exec();

    // loop
    while( query.next() ) {
        // create the Operation Record
        QString p;
        p = query.value(0).toString();

        query_count.bindValue(":op_type", p );
        query_count.exec();
        query_count.next();

        // add to stack
        operations.append( p + " (" + query_count.value(0).toString() + ")" );
    }

    // return
    return operations;
}

int DBConn::operationsInMonth(QDate month) {

    // get the list of patients and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.prepare("select count(id) as cc from operations where op_date >= :date_from and op_date <= :date_to" );

    query.bindValue( ":date_from", month.toString("yyyy-MM-01") );
    query.bindValue( ":date_to", month.toString("yyyy-MM-") + QString::number( month.daysInMonth() ) );

    query.exec();

    query.next();

    return query.value(0).toInt();
}

Operation DBConn::operation( int id ) {
    // return the operation for this id, or false;
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,op_date,op_type,op_notes from operations"
		" where id=" + QString::number(id) + " limit 1" );

    // get record
    query.next();

    // assign values
    Operation p;
    p.id = query.value(0).toInt();
    p.patient_id = query.value(1).toInt();
    p.op_date = query.value(2).toString();
    p.op_type = query.value(3).toString();
    p.op_notes = query.value(4).toString();

    return p;
}

bool DBConn::deleteOperation(int id) {
    // delete operation

    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "delete from operations where id=:id" );

    query.bindValue( ":id", id );

    if( query.exec() ) {
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }

}

bool DBConn::insertAppointment( Appointment &ap ) {
    qDebug() << "adding appointment...";

    // Insert the appointment record here
    QSqlQuery query( QSqlDatabase::database() );
    if( ap.id > 0 ) {
        query.prepare( "insert into appointments ("
                       "id,"
                        "patient_id,"
                        "ap_date,"
                        "ap_time,"
                        "ap_notes,"
                        "ap_notes2"
                        ") values ( :id, :patient_id, :ap_date, :ap_time, :ap_notes, :ap_notes2 )" );

        query.bindValue( ":id", ap.id );

    } else {
        query.prepare( "insert into appointments ("
                        "patient_id,"
                        "ap_date,"
                        "ap_time,"
                        "ap_notes,"
                        "ap_notes2"
                        ") values ( :patient_id, :ap_date, :ap_time, :ap_notes, :ap_notes2 )" );
    }

    query.bindValue( ":patient_id", ap.patient_id );
    query.bindValue( ":ap_date", ap.ap_date );
    query.bindValue( ":ap_time", ap.ap_time );
    query.bindValue( ":ap_notes", ap.ap_notes );
    query.bindValue( ":ap_notes2", ap.ap_notes2 );

    if( query.exec() ) {
        ap.id = query.lastInsertId().toInt();
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }
}

bool DBConn::updateAppointment( Appointment &ap ) {
    qDebug() << "updating appointment...";

    // Update the appointment record here
    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "update appointments set "
		    "ap_date=:ap_date,"
		    "ap_time=:ap_time,"
                    "ap_notes=:ap_notes,"
                    "ap_notes2=:ap_notes2"
		    " where id=:id" );

    query.bindValue( ":id", ap.id );
    query.bindValue( ":ap_date", ap.ap_date );
    query.bindValue( ":ap_time", ap.ap_time );
    query.bindValue( ":ap_notes", ap.ap_notes );
    query.bindValue( ":ap_notes2", ap.ap_notes2 );

    if( query.exec() ) {
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }
}

bool DBConn::deleteAppointment( int id ) {
    // delete appointment

    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "delete from appointments where id=:id" );

    query.bindValue( ":id", id );

    if( query.exec() ) {
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }
}

QList<Appointment> DBConn::appointments( int patient_id ) {
    // init
    QList<Appointment> appointments;

    // get the list of patients and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,ap_date,ap_time,ap_notes,ap_notes2 from appointments where patient_id=" + QString::number(patient_id) + " order by ap_date desc" );

    // loop
    while( query.next() ) {
	// create the Appointment Record
	Appointment p;
	p.id = query.value(0).toInt();
	p.patient_id = query.value(1).toInt();
	p.ap_date = query.value(2).toString();
	p.ap_time = query.value(3).toString();
	p.ap_notes = query.value(4).toString();
        p.ap_notes2 = query.value(5).toString();

	// add to stack
	appointments.append( p );
    }

    // return
    return appointments;
}

QList<Appointment> DBConn::appointments( QString ap_date ) {
    // init
    QList<Appointment> appointments;

    // get the list of patients and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.prepare("select id,patient_id,ap_date,ap_time,ap_notes, ap_notes2 from appointments where ap_date=:ap_date order by ap_time asc" );
    query.bindValue( ":ap_date", ap_date );

    query.exec();

    // loop
    while( query.next() ) {
	// create the Appointment Record
	Appointment p;
	p.id = query.value(0).toInt();
	p.patient_id = query.value(1).toInt();
	p.ap_date = query.value(2).toString();
	p.ap_time = query.value(3).toString();
	p.ap_notes = query.value(4).toString();
        p.ap_notes2 = query.value(5).toString();

	// add to stack
	appointments.append( p );
    }

    // return
    return appointments;
}

QList<Appointment> DBConn::appointments( QString ap_date_start, QString ap_date_end ) {
    // init
    QList<Appointment> appointments;

    // get the list of patients and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,ap_date,ap_time,ap_notes,ap_notes2 from appointments where ap_date > '" + ap_date_start + "' and ap_date < '" + ap_date_end + "' or (ap_date = '"+ap_date_start+"') or (ap_date = '"+ap_date_end+"' )" );

    // loop
    while( query.next() ) {
	// create the Appointment Record
	Appointment p;
	p.id = query.value(0).toInt();
	p.patient_id = query.value(1).toInt();
	p.ap_date = query.value(2).toString();
	p.ap_time = query.value(3).toString();
	p.ap_notes = query.value(4).toString();
        p.ap_notes2 = query.value(5).toString();

	// add to stack
	appointments.append( p );
    }

    // return
    return appointments;
}

Appointment DBConn::latestAppointment( int patient_id ) {
    // return the appointment for this id, or false;
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,ap_date,ap_time,ap_notes,ap_notes2 from appointments"
                " where patient_id=" + QString::number(patient_id) + " order by ap_date desc limit 1" );

    // get record
    query.next();

    // assign values
    Appointment p;
    p.id = query.value(0).toInt();
    p.patient_id = query.value(1).toInt();
    p.ap_date = query.value(2).toString();
    p.ap_time = query.value(3).toString();
    p.ap_notes = query.value(4).toString();
    p.ap_notes2 = query.value(5).toString();

    return p;
}

Appointment DBConn::appointment( int id ) {
    // return the appointment for this id, or false;
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,ap_date,ap_time,ap_notes,ap_notes2 from appointments"
		" where id=" + QString::number(id) + " limit 1" );

    // get record
    query.next();

    // assign values
    Appointment p;
    p.id = query.value(0).toInt();
    p.patient_id = query.value(1).toInt();
    p.ap_date = query.value(2).toString();
    p.ap_time = query.value(3).toString();
    p.ap_notes = query.value(4).toString();
    p.ap_notes2 = query.value(5).toString();

    return p;
}

int DBConn::appointmentsInDay(QString ap_date) {

    // get the list of patients and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.prepare("select count(id) as id from appointments where ap_date=:ap_date" );
    query.bindValue( ":ap_date", ap_date );

    query.exec();

    query.next();

    if( query.value(0).toInt() > 0 ) return query.value(0).toInt();
    else return 0;

}

int DBConn::appointmentsInMonth(QDate month) {

    // get the list of patients and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.prepare("select count(id) as id from appointments where ap_date >= :start and ap_date <= :stop" );
    query.bindValue( ":start", month.toString("yyyy-MM-01") );
    query.bindValue( ":stop", month.toString("yyyy-MM-") + QString::number( month.daysInMonth() ) );

    query.exec();

    query.next();

    if( query.value(0).toInt() > 0 ) return query.value(0).toInt();
    else return 0;

}

int DBConn::maxAppointmentsPerDayPerMonth(QString ap_date) {

    // prepare the dates
    QDate d, t_d;
    d = QDate().fromString( ap_date, "yyyy-MM-dd" );

    int max = 0;

    for( int i = 1; i <= d.daysInMonth(); i++ ) {

        int t_max = DBConn::instance()->appointmentsInDay( QDate(d.year(), d.month(), i).toString("yyyy-MM-dd") );

        if( t_max > max ) max = t_max;
    }

    return max;
}

bool DBConn::insertPayment(Payment &pay) {
    qDebug() << "adding payment...";

    // Insert the payment record here
    QSqlQuery query( QSqlDatabase::database() );

    if( pay.id > 0 ) {
        query.prepare( "insert into payments ("
                       "id,"
                        "patient_id,"
                        "pay_date,"
                        "pay_amount,"
                        "pay_status,"
                        "pay_notes"
                        ") values ( :id, :patient_id, :pay_date, :pay_amount, :pay_status, :pay_notes )" );

       query.bindValue( ":id", pay.id );

    } else {
        query.prepare( "insert into payments ("
                        "patient_id,"
                        "pay_date,"
                        "pay_amount,"
                        "pay_status,"
                        "pay_notes"
                        ") values ( :patient_id, :pay_date, :pay_amount, :pay_status, :pay_notes )" );
    }

    query.bindValue( ":patient_id", pay.patient_id );
    query.bindValue( ":pay_date", pay.pay_date );
    query.bindValue( ":pay_amount", pay.pay_amount );
    query.bindValue( ":pay_status", pay.pay_status );
    query.bindValue( ":pay_notes", pay.pay_notes );

    if( query.exec() ) {
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }
}

bool DBConn::updatePayment(Payment &pay) {
    qDebug() << "updating payment...";

    // Update the payment record here
    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "update payments set "
		    "pay_date=:pay_date,"
		    "pay_amount=:pay_amount,"
		    "pay_status=:pay_status,"
		    "pay_notes=:pay_notes"
		    " where id=:id" );

    query.bindValue( ":id", pay.id );
    query.bindValue( ":pay_date", pay.pay_date );
    query.bindValue( ":pay_amount", pay.pay_amount );
    query.bindValue( ":pay_status", pay.pay_status );
    query.bindValue( ":pay_notes", pay.pay_notes );

    if( query.exec() ) {
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }
}

bool DBConn::deletePayment(int id) {
    // delete payment

    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "delete from payments where id=:id" );

    query.bindValue( ":id", id );

    if( query.exec() ) {
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }
}

QList<Payment> DBConn::payments() {
    // init
    QList<Payment> payments;

    // get the list of payments and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,pay_date,pay_amount,pay_status,pay_notes from payments order by pay_date desc" );

    // loop
    while( query.next() ) {
	// create the payments Record
	Payment p;
	p.id = query.value(0).toInt();
	p.patient_id = query.value(1).toInt();
	p.pay_date = query.value(2).toString();
	p.pay_amount = query.value(3).toString();
	p.pay_status = query.value(4).toInt();
	p.pay_notes = query.value(5).toString();

	// add to stack
	payments.append( p );
    }

    // return
    return payments;
}

QList<Payment> DBConn::payments(int patient_id) {
    // init
    QList<Payment> payments;

    // get the list of payments and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,pay_date,pay_amount,pay_status,pay_notes from payments where patient_id=" + QString::number(patient_id) + " order by pay_date desc" );

    // loop
    while( query.next() ) {
	// create the payments Record
	Payment p;
	p.id = query.value(0).toInt();
	p.patient_id = query.value(1).toInt();
	p.pay_date = query.value(2).toString();
	p.pay_amount = query.value(3).toString();
	p.pay_status = query.value(4).toInt();
	p.pay_notes = query.value(5).toString();

	// add to stack
	payments.append( p );
    }

    // return
    return payments;
}

QList<Payment> DBConn::payments(QString pay_date) {
    // init
    QList<Payment> payments;

    // get the list of payments and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.prepare("select id,patient_id,pay_date,pay_amount,pay_status,pay_notes from payments where pay_date=:pay_date" );
    query.bindValue( ":pay_date", pay_date );

    query.exec();

    // loop
    while( query.next() ) {
	// create the payments Record
	Payment p;
	p.id = query.value(0).toInt();
	p.patient_id = query.value(1).toInt();
	p.pay_date = query.value(2).toString();
	p.pay_amount = query.value(3).toString();
	p.pay_status = query.value(4).toInt();
	p.pay_notes = query.value(5).toString();

	// add to stack
	payments.append( p );
    }

    // return
    return payments;
}

QList<Payment> DBConn::payments(QString pay_date_start, QString pay_date_end) {
    // init
    QList<Payment> payments;

    // get the list of payments and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,pay_date,pay_amount,pay_status,pay_notes from payments where pay_date > '" + pay_date_start + "' and pay_date < '" + pay_date_end + "'" );

    // loop
    while( query.next() ) {
	// create the payments Record
	Payment p;
	p.id = query.value(0).toInt();
	p.patient_id = query.value(1).toInt();
	p.pay_date = query.value(2).toString();
	p.pay_amount = query.value(3).toString();
	p.pay_status = query.value(4).toInt();
	p.pay_notes = query.value(5).toString();

	// add to stack
	payments.append( p );
    }

    // return
    return payments;
}

Payment DBConn::payment( int id ) {
    // return the payment for this id, or false;
    QSqlQuery query( QSqlDatabase::database() );

    qDebug() << "select id,patient_id,pay_date,pay_amount,pay_status,pay_notes from payments"
		" where id=" + QString::number(id) + " limit 1";

    // execute select statement
    query.exec("select id,patient_id,pay_date,pay_amount,pay_status,pay_notes from payments"
		" where id=" + QString::number(id) + " limit 1" );

    // get record
    query.next();

    // assign values
    Payment p;
    p.id = query.value(0).toInt();
    p.patient_id = query.value(1).toInt();
    p.pay_date = query.value(2).toString();
    p.pay_amount = query.value(3).toString();
    p.pay_status = query.value(4).toInt();
    p.pay_notes = query.value(5).toString();

    return p;
}

bool DBConn::insertImage(Image &img) {
    qDebug() << "adding image...";

    // Insert the image record here
    QSqlQuery query( QSqlDatabase::database() );

    if( img.id > 0 ) {
        query.prepare( "insert into images ("
                       "id,"
                        "patient_id,"
                        "img_date,"
                        "img_name,"
                        "img_notes"
                        ") values ( :id, :patient_id, :img_date, :img_name, :img_notes )" );

        query.bindValue( ":id", img.id );

    } else {
        query.prepare( "insert into images ("
                        "patient_id,"
                        "img_date,"
                        "img_name,"
                        "img_notes"
                        ") values ( :patient_id, :img_date, :img_name, :img_notes )" );
    }

    query.bindValue( ":patient_id", img.patient_id );
    query.bindValue( ":img_date", img.img_date );
    query.bindValue( ":img_name", img.img_name );
    query.bindValue( ":img_notes", img.img_notes );

    if( query.exec() ) {
        img.id = query.lastInsertId().toInt();
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }
}

bool DBConn::updateImage(Image &img) {
    qDebug() << "updating image...";

    // Update the image record here
    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "update images set "
		    "img_notes=:img_notes"
		    " where id=:id" );

    query.bindValue( ":id", img.id );
    query.bindValue( ":img_notes", img.img_notes );

    if( query.exec() ) {
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }
}

bool DBConn::deleteImage(int id) {
    // delete image

    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "delete from images where id=:id" );

    query.bindValue( ":id", id );

    if( query.exec() ) {
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }
}

QList<Image> DBConn::images() {
    // init
    QList<Image> images;

    // get the list of images and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,img_date,img_name,img_notes from images order by img_date desc" );

    // loop
    while( query.next() ) {
	// create the images Record
	Image p;
	p.id = query.value(0).toInt();
	p.patient_id = query.value(1).toInt();
	p.img_date = query.value(2).toString();
	p.img_name = query.value(3).toString();
	p.img_notes = query.value(4).toString();

	// add to stack
	images.append( p );
    }

    // return
    return images;
}

QList<Image> DBConn::images(int patient_id) {
    // init
    QList<Image> images;

    // get the list of images and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,img_date,img_name,img_notes from images where patient_id=" + QString::number(patient_id) + " order by img_date desc" );

    // loop
    while( query.next() ) {
	// create the images Record
	Image p;
	p.id = query.value(0).toInt();
	p.patient_id = query.value(1).toInt();
	p.img_date = query.value(2).toString();
	p.img_name = query.value(3).toString();
	p.img_notes = query.value(4).toString();

	// add to stack
	images.append( p );
    }

    // return
    return images;
}

QList<Image> DBConn::images(QString img_date) {
    // init
    QList<Image> images;

    // get the list of images and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.prepare("select id,patient_id,img_date,img_name,img_notes from images where img_date=:img_date" );
    query.bindValue( ":img_date", img_date );

    query.exec();

    // loop
    while( query.next() ) {
	// create the images Record
	Image p;
	p.id = query.value(0).toInt();
	p.patient_id = query.value(1).toInt();
	p.img_date = query.value(2).toString();
	p.img_name = query.value(3).toString();
	p.img_notes = query.value(4).toString();

	// add to stack
	images.append( p );
    }

    // return
    return images;
}

QList<Image> DBConn::images(QString img_date_start, QString img_date_end) {
    // init
    QList<Image> images;

    // get the list of images and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,img_date,img_name,img_notes from images where img_date > '" + img_date_start + "' and img_date < '" + img_date_end + "'" );

    // loop
    while( query.next() ) {
	// create the images Record
	Image p;
	p.id = query.value(0).toInt();
	p.patient_id = query.value(1).toInt();
	p.img_date = query.value(2).toString();
	p.img_name = query.value(3).toString();
	p.img_notes = query.value(4).toString();

	// add to stack
	images.append( p );
    }

    // return
    return images;
}

Image DBConn::image( int id ) {
    // return the image for this id, or false;
    QSqlQuery query( QSqlDatabase::database() );

    qDebug() << "select id,patient_id,img_date,img_name,img_notes from images"
		" where id=" + QString::number(id) + " limit 1";

    // execute select statement
    query.exec("select id,patient_id,img_date,img_name,img_notes from images"
		" where id=" + QString::number(id) + " limit 1" );

    // get record
    query.next();

    // assign values
    Image p;
    p.id = query.value(0).toInt();
    p.patient_id = query.value(1).toInt();
    p.img_date = query.value(2).toString();
    p.img_name = query.value(3).toString();
    p.img_notes = query.value(4).toString();

    return p;
}

bool DBConn::insertPrescription(Prescription &pre) {
    qDebug() << "adding prescription...";

    // Insert the prescription record here
    QSqlQuery query( QSqlDatabase::database() );

    if( pre.id > 0 ) {
        query.prepare( "insert into prescriptions ("
                       "id,"
                        "patient_id,"
                        "prescr_date,"
                        "prescr_text,"
                        "prescr_notes"
                        ") values ( :id, :patient_id, :prescr_date, :prescr_text, :prescr_notes )" );

        query.bindValue( ":id", pre.id );

    } else {
        query.prepare( "insert into prescriptions ("
                        "patient_id,"
                        "prescr_date,"
                        "prescr_text,"
                        "prescr_notes"
                        ") values ( :patient_id, :prescr_date, :prescr_text, :prescr_notes )" );
    }

    query.bindValue( ":patient_id", pre.patient_id );
    query.bindValue( ":prescr_date", pre.prescr_date );
    query.bindValue( ":prescr_text", pre.prescr_text );
    query.bindValue( ":prescr_notes", pre.prescr_notes );

    if( query.exec() ) {
        pre.id = query.lastInsertId().toInt();
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }
}

bool DBConn::deletePrescription(int id) {
    // delete prescription
    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "delete from prescriptions where id=:id" );

    query.bindValue( ":id", id );

    if( query.exec() ) {
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }
}

QList<Prescription> DBConn::prescriptions() {
    // return all
    QList<Prescription> prescriptions;

    // get the list of prescriptions and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,prescr_date,prescr_text,prescr_notes from prescriptions order by prescr_date desc" );

    // loop
    while( query.next() ) {
	// create the images Record
	Prescription p;
	p.id = query.value(0).toInt();
	p.patient_id = query.value(1).toInt();
	p.prescr_date = query.value(2).toString();
	p.prescr_text = query.value(3).toString();
	p.prescr_notes = query.value(4).toString();

	// add to stack
	prescriptions.append( p );
    }

    // return
    return prescriptions;
}

QList<Prescription> DBConn::prescriptions(int patient_id) {
    // return all for patient
    QList<Prescription> prescriptions;

    // get the list of prescriptions and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,prescr_date,prescr_text,prescr_notes from prescriptions where patient_id = " + QString::number(patient_id) + " order by prescr_date desc" );

    // loop
    while( query.next() ) {
	// create the images Record
	Prescription p;
	p.id = query.value(0).toInt();
	p.patient_id = query.value(1).toInt();
	p.prescr_date = query.value(2).toString();
	p.prescr_text = query.value(3).toString();
	p.prescr_notes = query.value(4).toString();

	// add to stack
	prescriptions.append( p );
    }

    // return
    return prescriptions;
}

QList<Prescription> DBConn::prescriptions(QString pre_date) {
    // return all for given date
    QList<Prescription> prescriptions;

    // get the list of prescriptions and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.prepare("select id,patient_id,prescr_date,prescr_text,prescr_notes from prescriptions where prescr_date=:prescr_date" );
    query.bindValue( ":prescr_date", pre_date );

    query.exec();

    // loop
    while( query.next() ) {
	// create the images Record
	Prescription p;
	p.id = query.value(0).toInt();
	p.patient_id = query.value(1).toInt();
	p.prescr_date = query.value(2).toString();
	p.prescr_text = query.value(3).toString();
	p.prescr_notes = query.value(4).toString();

	// add to stack
	prescriptions.append( p );
    }

    // return
    return prescriptions;
}

QList<Prescription> DBConn::prescriptions(QString pre_date_start, QString pre_date_end) {
    // return all from start to end
    QList<Prescription> prescriptions;

    // get the list of prescriptions and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,prescr_date,prescr_text,prescr_notes from prescriptions where prescr_date > '" + pre_date_start + "' and prescr_date < '" + pre_date_end + "'" );

    // loop
    while( query.next() ) {
	// create the images Record
	Prescription p;
	p.id = query.value(0).toInt();
	p.patient_id = query.value(1).toInt();
	p.prescr_date = query.value(2).toString();
	p.prescr_text = query.value(3).toString();
	p.prescr_notes = query.value(4).toString();

	// add to stack
	prescriptions.append( p );
    }

    // return
    return prescriptions;
}

Prescription DBConn::prescription(int id) {
    // return current prescription
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,prescr_date,prescr_text,prescr_notes from prescriptions"
		" where id=" + QString::number(id) + " limit 1" );

    // get record
    query.next();

    // assign values
    Prescription p;
    p.id = query.value(0).toInt();
    p.patient_id = query.value(1).toInt();
    p.prescr_date = query.value(2).toString();
    p.prescr_text = query.value(3).toString();
    p.prescr_notes = query.value(4).toString();

    return p;
}

bool DBConn::updatePrescription(Prescription &pre) {
    qDebug() << "updating prescription...";

    // Update the prescription record here
    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "update prescriptions set "
		    "prescr_text=:prescr_text,"
		    "prescr_notes=:prescr_notes"
		    " where id=:id" );

    query.bindValue( ":id", pre.id );
    query.bindValue( ":prescr_text", pre.prescr_text );
    query.bindValue( ":prescr_notes", pre.prescr_notes );

    if( query.exec() ) {
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }
}

bool DBConn::insertPatientHistory(PatientHistoryObj &ph) {
    // Insert the history record here
    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "insert into patient_history ("
		    "patient_id,"
		    "hist_diseases,"
                    "hist_allergies,"
                    "hist_surgeries,"
                    "hist_other2,"
                    "hist_family,"
		    "hist_medications,"
                    "hist_other,"
                    "hist_habit_smoking,"
                    "hist_habit_sports,"
                    "hist_habit_alcohol,"
                    "hist_habit_other"
                    ") values ( :patient_id, :hist_diseases, :hist_allergies, :hist_surgeries, :hist_other2, :hist_family, :hist_medications, :hist_other, :hist_habit_smoking, :hist_habit_sports, :hist_habit_alcohol, :hist_habit_other )" );

    query.bindValue( ":patient_id", ph.patient_id );
    query.bindValue( ":hist_diseases", ph.hist_diseases );
    query.bindValue( ":hist_allergies", ph.hist_allergies );
    query.bindValue( ":hist_surgeries", ph.hist_surgeries );
    query.bindValue( ":hist_other2", ph.hist_other2 );
    query.bindValue( ":hist_family", ph.hist_family );
    query.bindValue( ":hist_medications", ph.hist_medications );
    query.bindValue( ":hist_other", ph.hist_other );
    query.bindValue( ":hist_habit_smoking", ph.hist_habit_smoking );
    query.bindValue( ":hist_habit_sports", ph.hist_habit_sports );
    query.bindValue( ":hist_habit_alcohol", ph.hist_habit_alcohol );
    query.bindValue( ":hist_habit_other", ph.hist_habit_other );

    if( query.exec() ) {
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }
}

bool DBConn::updatePatientHistory(PatientHistoryObj &ph) {

    // Update the history record here
    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "update patient_history set "
		    "hist_diseases=:hist_diseases,"
                    "hist_allergies=:hist_allergies,"
                    "hist_surgeries=:hist_surgeries,"
                    "hist_other2=:hist_other2,"
                    "hist_family=:hist_family,"
		    "hist_medications=:hist_medications,"
                    "hist_other=:hist_other,"
                    "hist_habit_smoking=:hist_habit_smoking,"
                    "hist_habit_sports=:hist_habit_sports,"
                    "hist_habit_alcohol=:hist_habit_alcohol,"
                    "hist_habit_other=:hist_habit_other"
		    " where id=:id" );

    query.bindValue( ":id", ph.id );
    query.bindValue( ":hist_diseases", ph.hist_diseases );
    query.bindValue( ":hist_allergies", ph.hist_allergies );
    query.bindValue( ":hist_surgeries", ph.hist_surgeries );
    query.bindValue( ":hist_other2", ph.hist_other2 );
    query.bindValue( ":hist_family", ph.hist_family );
    query.bindValue( ":hist_medications", ph.hist_medications );
    query.bindValue( ":hist_other", ph.hist_other );
    query.bindValue( ":hist_habit_smoking", ph.hist_habit_smoking );
    query.bindValue( ":hist_habit_sports", ph.hist_habit_sports );
    query.bindValue( ":hist_habit_alcohol", ph.hist_habit_alcohol );
    query.bindValue( ":hist_habit_other", ph.hist_habit_other );

    if( query.exec() ) {
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }
}

bool DBConn::deletePatientHistory(int id) {
    // delete prescription
    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "delete from patient_history where id=:id" );

    query.bindValue( ":id", id );

    if( query.exec() ) {
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }
}

PatientHistoryObj DBConn::patienthistory(int patient_id) {
    // get the history for a given patient
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,hist_diseases,hist_medications,hist_other,hist_surgeries,hist_allergies,hist_other2,hist_family,hist_habit_smoking,hist_habit_sports,hist_habit_alcohol,hist_habit_other from patient_history"
		" where patient_id=" + QString::number(patient_id) + " limit 1" );

    // get record
    query.next();

    // assign values
    PatientHistoryObj p;
    p.id = query.value(0).toInt();
    p.patient_id = query.value(1).toInt();
    p.hist_diseases = query.value(2).toString();
    p.hist_medications = query.value(3).toString();
    p.hist_other = query.value(4).toString();
    p.hist_surgeries = query.value(5).toString();
    p.hist_allergies = query.value(6).toString();
    p.hist_other2 = query.value(7).toString();
    p.hist_family = query.value(8).toString();
    p.hist_habit_smoking = query.value(9).toString();
    p.hist_habit_sports = query.value(10).toString();
    p.hist_habit_alcohol = query.value(11).toString();
    p.hist_habit_other = query.value(12).toString();


    return p;
}

bool DBConn::insertSetting( Setting &set ) {
    qDebug() << "adding/updating setting...";

    if( settingExists(set.setting) ) { // EXISTS - UPDATE
	// update and return
	if( updateSetting(set) )
	    return true;
	else return false;

    } else { // CREATE NEW
	// Insert the setting record here
	QSqlQuery query( QSqlDatabase::database() );
	query.prepare( "insert into settings ("
			"setting,"
			"value"
			") values ( :setting, :value )" );

	query.bindValue( ":setting", set.setting );
	query.bindValue( ":value", set.value );

	if( query.exec() ) {
	    return true;
	} else {
	    qDebug() << "failed: " << query.lastError();
	    return false;
	}
    }
}

bool DBConn::updateSetting( Setting &set ) {
    if( !settingExists(set.setting) ) return false;

    set.id = settingExists(set.setting);

    qDebug() << "updating setting...";

    // Insert the appointment record here
    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "update settings set "
		    "setting=:setting,"
		    "value=:value"
		    " where id=:id" );

    query.bindValue( ":id", set.id );
    query.bindValue( ":setting", set.setting );
    query.bindValue( ":value", set.value );

    if( query.exec() ) {
	return true;
    } else {
	qDebug() << "failed: " << query.lastError();
	return false;
    }
}

Setting DBConn::setting( QString setting ) {
    // return the setting for this id, or false;
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.prepare("select id,setting,value from settings"
		" where setting=:setting limit 1" );
    query.bindValue( ":setting", setting );

    // execute
    query.exec();

    // get record
    if( query.next() ) {
	// assign values
	Setting p;
	p.id = query.value(0).toInt();
	p.setting = query.value(1).toInt();
	p.value = query.value(2).toString();

	return p;
    } else {
	Setting p;
	return p;
    }
}

int DBConn::settingExists( QString setting ) {
    // return the setting for this id, or false;
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.prepare("select id,setting,value from settings"
		" where setting=:setting limit 1" );
    query.bindValue( ":setting", setting );

    // execute
    query.exec();

    // get record
    if( query.next() ) {
	return query.value(0).toInt();
    } else {
	return false;
    }
}

QString DBConn::settingValue(QString setting) {
    // return the value string of the current setting
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.prepare("select id,setting,value from settings"
		" where setting=:setting limit 1" );
    query.bindValue( ":setting", setting );

    // execute
    query.exec();

    // get record
    if( query.next() ) {
	// assign values
	Setting p;
	p.id = query.value(0).toInt();
	p.setting = query.value(1).toInt();
	p.value = query.value(2).toString();

	return p.value;
    } else {
	return "";
    }
}

// OPERATION LINKS START
bool DBConn::linkOperation(QString type, int from_id, int to_id) {
    // we need to create the link

    qDebug() << "linking " << type;

    if( type == "appointment" ) {
        Appointment a = DBConn::instance()->appointment( to_id );
        if( !a.id ) return false;
    } else if( type == "image" ) {
        Image i = DBConn::instance()->image( to_id );
        if( !i.id ) return false;
    } else if ( type == "prescription" ) {
        Prescription p = DBConn::instance()->prescription( to_id );
        if( !p.id ) return false;
    } else return false;

    // type is correct, check that both id are valid
    Operation o = DBConn::instance()->operation( from_id );

    // not valid, return false;
    if( !o.id ) return false;

    // everything is valid, insert
    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "insert into operation_assoc ("
                    "type,"
                    "from_id,"
                    "to_id"
                    ") values ( :type, :from_id, :to_id )" );

    query.bindValue( ":type", type );
    query.bindValue( ":from_id", from_id );
    query.bindValue( ":to_id", to_id );

    if( query.exec() ) {
        return true;
    } else {
        qDebug() << "failed: " << query.lastError();
        return false;
    }
}

QStringList DBConn::get_nationalities(){

    QStringList nationalities;

    // get the list of nationalities
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select distinct nationality from patients order by nationality") ;

    //insert empty string
    nationalities << "";

    // loop
    while( query.next() ) {
        // append nationality to list
        if( query.value(0).toString() != "")
            nationalities << query.value(0).toString();
    }

    // return
    return nationalities;

}


QList<Appointment> DBConn::linkedAppointments(int op_id) {

    // return all from start to end
    QList<Appointment> appointments;

    // get the list of appointments and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,ap_date,ap_time,ap_notes,ap_notes2 from appointments where id in ( select to_id from operation_assoc where type = 'appointment' and from_id = " + QString::number(op_id) + " ) order by ap_date desc" );

    // loop
    while( query.next() ) {
        // create the appointments record
        Appointment a;
        a.id = query.value(0).toInt();
        a.patient_id = query.value(1).toInt();
        a.ap_date = query.value(2).toString();
        a.ap_time = query.value(3).toString();
        a.ap_notes = query.value(4).toString();
        a.ap_notes2 = query.value(5).toString();

        // add to stack
        appointments.append( a );
    }

    // return
    return appointments;
}

QList<Prescription> DBConn::linkedPrescriptions(int op_id) {
    // return all from start to end
    QList<Prescription> prescriptions;

    // get the list of appointments and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,prescr_date,prescr_text,prescr_notes from prescriptions where id in ( select to_id from operation_assoc where type = 'prescription' and from_id = " + QString::number(op_id) + " ) order by prescr_date desc" );

    // loop
    while( query.next() ) {
        // create the prescriptions record
        Prescription p;
        p.id = query.value(0).toInt();
        p.patient_id = query.value(1).toInt();
        p.prescr_date = query.value(2).toString();
        p.prescr_text = query.value(3).toString();
        p.prescr_notes = query.value(4).toString();

        // add to stack
        prescriptions.append( p );
    }

    // return
    return prescriptions;
}

QList<Image> DBConn::linkedImages(int op_id) {
    // return all from start to end
    QList<Image> images;

    // get the list of appointments and return in a QList
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id,patient_id,img_date,img_name,img_notes from images where id in ( select to_id from operation_assoc where type = 'image' and from_id = " + QString::number(op_id) + " ) order by img_date desc" );

    // loop
    while( query.next() ) {
        // create the images record
        Image p;
        p.id = query.value(0).toInt();
        p.patient_id = query.value(1).toInt();
        p.img_date = query.value(2).toString();
        p.img_name = query.value(3).toString();
        p.img_notes = query.value(4).toString();

        // add to stack
        images.append( p );
    }

    // return
    return images;
}

bool DBConn::deleteLink(QString type, int from_id, int to_id) {

    // delete prescription
    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "delete from operation_assoc where type=:type and from_id=:from_id and to_id=:to_id" );

    query.bindValue( ":type", type );
    query.bindValue( ":from_id", from_id );
    query.bindValue( ":to_id", to_id );

    if( query.exec() ) {
        return true;
    } else {
        qDebug() << "failed: " << query.lastError();
        return false;
    }
}

bool DBConn::deleteLinksOfOperation( int operation_id ) {

    // delete prescription
    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "delete from operation_assoc where from_id=:from_id" );

    query.bindValue( ":from_id", operation_id );

    if( query.exec() ) {
        return true;
    } else {
        qDebug() << "failed: " << query.lastError();
        return false;
    }
}

bool DBConn::deleteLinksWhereAppointment(int appointment_id) {

    // delete prescription
    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "delete from operation_assoc where type = 'appointment' and to_id=:to_id" );

    query.bindValue( ":to_id", appointment_id );

    if( query.exec() ) {
        return true;
    } else {
        qDebug() << "failed: " << query.lastError();
        return false;
    }
}

bool DBConn::deleteLinksWhereImage(int image_id ) {

    // delete prescription
    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "delete from operation_assoc where type = 'image' and to_id=:to_id" );

    query.bindValue( ":to_id", image_id );

    if( query.exec() ) {
        return true;
    } else {
        qDebug() << "failed: " << query.lastError();
        return false;
    }
}

bool DBConn::deleteLinksWherePrescription(int prescription_id) {

    // delete prescription
    QSqlQuery query( QSqlDatabase::database() );
    query.prepare( "delete from operation_assoc where type = 'prescription' and to_id=:to_id" );

    query.bindValue( ":to_id", prescription_id );

    if( query.exec() ) {
        return true;
    } else {
        qDebug() << "failed: " << query.lastError();
        return false;
    }
}

bool DBConn::linkExists(QString type, int from_id, int to_id) {

    // get the link see if it exists
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select id from operation_assoc where type = '" + type + "' and from_id = " + QString::number(from_id) + " and to_id = " + QString::number(to_id) );

    query.next();

    if( query.value(0).toInt() >  0 ) return true;
    else return false;
}

// OPERATION LINKS END

QString DBConn::defaultCurrency( bool symbol = true ) {
    // check the currency
    QString curr = settingValue("dental_office_currency");

    if( curr == "Euros" ) return QString::fromUtf8("€");
    if( curr == "US Dollars" ) return "$";
    if( curr == "British Pounds" ) return QString::fromUtf8("£");

    return "";
}

QString DBConn::databaseVersion() {
    // get the database version from table dbVer and column db_version with id = 1;

    // return the value string of the current setting
    QSqlQuery query( QSqlDatabase::database() );

    // execute select statement
    query.exec("select db_version from dbVer where id=1 limit 1" );

    // get record
    if( query.next() ) {
	// return version
	return query.value(0).toString();
    } else {
	return "1.5";
    }
}

void DBConn::checkUpdateDatabase() {
    // get the version and check against this version.
    // update as necessary
    QString dbVersion = databaseVersion();

    if( dbVersion == "1.5" ) {
	// create connection
	QSqlQuery query( QSqlDatabase::database() );

	// create table dbVer to store version
	query.exec( "CREATE TABLE dbVer (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL , db_version VARCHAR)" );
	query.exec( "CREATE TABLE patient_history (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL , patient_id INTEGER, hist_diseases TEXT, hist_medications TEXT, hist_other TEXT )" );
	query.exec( "insert into dbVer ( db_version ) values ( '1.6' )" );
    }

    dbVersion = databaseVersion();

    if( dbVersion == "1.6" ) {
        // create connection
        QSqlQuery query( QSqlDatabase::database() );

        // make table changes
        query.exec( "CREATE TABLE operation_assoc (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, type VARCHAR, from_id INTEGER, to_id INTEGER )" );
        query.exec( "update dbVer set db_version = '1.7' where db_version = '1.6'" );
    }

    dbVersion = databaseVersion();

    if( dbVersion == "1.7" ) {
        // create connection
        QSqlQuery query( QSqlDatabase::database() );

        // make table changes
        query.exec( "ALTER TABLE appointments ADD COLUMN ap_notes2 VARCHAR" );
        query.exec( "ALTER TABLE patient_history ADD COLUMN hist_allergies TEXT" );
        query.exec( "ALTER TABLE patient_history ADD COLUMN hist_surgeries TEXT" );
        query.exec( "ALTER TABLE patient_history ADD COLUMN hist_other2 TEXT" );
        query.exec( "ALTER TABLE patient_history ADD COLUMN hist_family TEXT" );
        query.exec( "ALTER TABLE patient_history ADD COLUMN hist_habit_smoking VARCHAR" );
        query.exec( "ALTER TABLE patient_history ADD COLUMN hist_habit_sports VARCHAR" );
        query.exec( "ALTER TABLE patient_history ADD COLUMN hist_habit_alcohol VARCHAR" );
        query.exec( "ALTER TABLE patient_history ADD COLUMN hist_habit_other TEXT" );
        query.exec( "update dbVer set db_version = '1.8' where db_version = '1.7'" );
    }
}


DBConn::~DBConn() {
    dbc.close();
}
