/**************************************************************************
**   Recompile IT Services, Development Dept
**   11/22/2010 2010
**   All Rights Reserved
**************************************************************************/

#include <QFile>
#include <QDebug>
#include <QTextCodec>
#include "tool_backupengine.h"
#include "dialog_generaldialog.h"
#include "mainwindow.h"
#include "dbconn.h"

BackupEngine::BackupEngine() {
}

BackupEngine::BackupEngine(QLabel *label, QProgressBar *pbar) {
    // ok
    this->label = label;
    this->pbar = pbar;

    // go to +1%
    this->pbar->setValue( this->pbar->value()+1 );
    // fix label
    this->label->setText( "(##%) " + QObject::tr("Initializing backup engine...") );
    this->label->setText( this->label->text().replace("##", QString::number( this->pbar->value() )) );
}

QString BackupEngine::toPlainTextXML() {
    // return
}

bool BackupEngine::createXMLBackup( QString filename ) {
    // create backup

    // go to +1%
    this->pbar->setValue( 2 );
    // fix label
    this->label->setText( "(##%) " + QObject::tr("Beginning XML backup...") );
    this->label->setText( this->label->text().replace("##", QString::number( this->pbar->value() )) );

    xmlBackup = "";

    // create the document
    QDomDocument doc("ProMed");
    b_doc = doc;
    b_doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
    // create root element
    QDomElement root = b_doc.createElement("ProMed");
    b_doc.appendChild(root);

    // -- GATHER DATA -- //
    // go to +5%
    this->pbar->setValue( this->pbar->value()+5 );
    // fix label
    this->label->setText( "(##%) " + QObject::tr("Backing up patients...") );
    this->label->setText( this->label->text().replace("##", QString::number( this->pbar->value() )) );
    // patients
    QList<Patient> patients = DBConn::instance()->patients();

    // Loop through every patient, adding to the document
    for( int i=0; i < patients.size(); i++ ) {
	Patient p = patients.at(i); // get patient object

	// write the patient data inside the xml
	QDomElement tag_patient = b_doc.createElement("patient");
	root.appendChild(tag_patient);

	QDomElement tag = b_doc.createElement("id");
	QDomText val = b_doc.createTextNode( QString::number(p.id) );
	tag.appendChild(val);
	tag_patient.appendChild(tag);

	tag = b_doc.createElement("name");
	val = b_doc.createTextNode( p.name );
	tag.appendChild(val);
	tag_patient.appendChild(tag);

	tag = b_doc.createElement("surname");
	val = b_doc.createTextNode( p.surname );
	tag.appendChild(val);
	tag_patient.appendChild(tag);

	tag = b_doc.createElement("fathersName");
	val = b_doc.createTextNode( p.fathersName );
	tag.appendChild(val);
	tag_patient.appendChild(tag);

	tag = b_doc.createElement("birthdate");
	val = b_doc.createTextNode( p.birthdate );
	tag.appendChild(val);
	tag_patient.appendChild(tag);

	tag = b_doc.createElement("address");
	val = b_doc.createTextNode( p.address );
	tag.appendChild(val);
	tag_patient.appendChild(tag);

	tag = b_doc.createElement("amka");
	val = b_doc.createTextNode( p.amka );
	tag.appendChild(val);
	tag_patient.appendChild(tag);

	tag = b_doc.createElement("created_on");
	val = b_doc.createTextNode( p.created_on );
	tag.appendChild(val);
	tag_patient.appendChild(tag);

	tag = b_doc.createElement("email");
	val = b_doc.createTextNode( p.email );
	tag.appendChild(val);
	tag_patient.appendChild(tag);

	tag = b_doc.createElement("gender");
	val = b_doc.createTextNode( p.gender );
	tag.appendChild(val);
	tag_patient.appendChild(tag);

	tag = b_doc.createElement("insurance");
	val = b_doc.createTextNode( p.insurance );
	tag.appendChild(val);
	tag_patient.appendChild(tag);

	tag = b_doc.createElement("notes");
	val = b_doc.createTextNode( p.notes );
	tag.appendChild(val);
	tag_patient.appendChild(tag);

	tag = b_doc.createElement("tel1");
	val = b_doc.createTextNode( p.tel1 );
	tag.appendChild(val);
	tag_patient.appendChild(tag);

	tag = b_doc.createElement("tel2");
	val = b_doc.createTextNode( p.tel2 );
	tag.appendChild(val);
	tag_patient.appendChild(tag);

	tag = b_doc.createElement("tel3");
	val = b_doc.createTextNode( p.tel3 );
	tag.appendChild(val);
	tag_patient.appendChild(tag);

	tag = b_doc.createElement("tel1_type");
	val = b_doc.createTextNode( p.tel1_type );
	tag.appendChild(val);
	tag_patient.appendChild(tag);

	tag = b_doc.createElement("tel2_type");
	val = b_doc.createTextNode( p.tel2_type );
	tag.appendChild(val);
	tag_patient.appendChild(tag);

	tag = b_doc.createElement("tel3_type");
	val = b_doc.createTextNode( p.tel3_type );
	tag.appendChild(val);
	tag_patient.appendChild(tag);

	tag = b_doc.createElement("vat");
	val = b_doc.createTextNode( p.vat );
	tag.appendChild(val);
	tag_patient.appendChild(tag);

	// create operations for patient
	QList<Operation> operations = DBConn::instance()->operations( p.id ); // operations

	for( int z=0; z < operations.size(); z++ ) {
	    Operation o = operations.at(z);

	    // create operation element
	    QDomElement tag_operation = b_doc.createElement("operation");
	    tag_patient.appendChild( tag_operation );

	    // write the tags
	    tag = b_doc.createElement("id");
	    val = b_doc.createTextNode( QString::number(o.id) );
	    tag.appendChild(val);
	    tag_operation.appendChild(tag);

	    tag = b_doc.createElement("patient_id");
	    val = b_doc.createTextNode( QString::number(o.patient_id) );
	    tag.appendChild(val);
	    tag_operation.appendChild(tag);

	    tag = b_doc.createElement("op_date");
	    val = b_doc.createTextNode( o.op_date );
	    tag.appendChild(val);
	    tag_operation.appendChild(tag);

	    tag = b_doc.createElement("op_type");
	    val = b_doc.createTextNode( o.op_type );
	    tag.appendChild(val);
	    tag_operation.appendChild(tag);

	    tag = b_doc.createElement("op_notes");
	    val = b_doc.createTextNode( o.op_notes );
	    tag.appendChild(val);
	    tag_operation.appendChild(tag);
	}

	// create appointments for patient
	QList<Appointment> appointments = DBConn::instance()->appointments( p.id ); // appointments

	for( int z=0; z < appointments.size(); z++ ) {
	    Appointment a = appointments.at(z);

	    // create operation element
	    QDomElement tag_appointment = b_doc.createElement("appointment");
	    tag_patient.appendChild( tag_appointment );

	    // write the tags
	    tag = b_doc.createElement("id");
	    val = b_doc.createTextNode( QString::number(a.id) );
	    tag.appendChild(val);
	    tag_appointment.appendChild(tag);

	    tag = b_doc.createElement("patient_id");
	    val = b_doc.createTextNode( QString::number(a.patient_id) );
	    tag.appendChild(val);
	    tag_appointment.appendChild(tag);

	    tag = b_doc.createElement("ap_date");
	    val = b_doc.createTextNode( a.ap_date );
	    tag.appendChild(val);
	    tag_appointment.appendChild(tag);

	    tag = b_doc.createElement("ap_time");
	    val = b_doc.createTextNode( a.ap_time );
	    tag.appendChild(val);
	    tag_appointment.appendChild(tag);

	    tag = b_doc.createElement("ap_notes");
	    val = b_doc.createTextNode( a.ap_notes );
	    tag.appendChild(val);
	    tag_appointment.appendChild(tag);

            tag = b_doc.createElement("ap_notes2");
            val = b_doc.createTextNode( a.ap_notes2 );
            tag.appendChild(val);
            tag_appointment.appendChild(tag);
	}

	// create payments for patient
	QList<Payment> payments = DBConn::instance()->payments( p.id );

	for( int z=0; z < payments.size(); z++ ) {
	    Payment p = payments.at(z);

	    // create operation element
	    QDomElement tag_payment = b_doc.createElement("payment");
	    tag_patient.appendChild( tag_payment );

	    // write the tags
	    tag = b_doc.createElement("id");
	    val = b_doc.createTextNode( QString::number(p.id) );
	    tag.appendChild(val);
	    tag_payment.appendChild(tag);

	    tag = b_doc.createElement("patient_id");
	    val = b_doc.createTextNode( QString::number(p.patient_id) );
	    tag.appendChild(val);
	    tag_payment.appendChild(tag);

	    tag = b_doc.createElement("pay_date");
	    val = b_doc.createTextNode( p.pay_date );
	    tag.appendChild(val);
	    tag_payment.appendChild(tag);

	    tag = b_doc.createElement("pay_amount");
	    val = b_doc.createTextNode( p.pay_amount );
	    tag.appendChild(val);
	    tag_payment.appendChild(tag);

	    tag = b_doc.createElement("pay_status");
	    val = b_doc.createTextNode( QString::number(p.pay_status) );
	    tag.appendChild(val);
	    tag_payment.appendChild(tag);

	    tag = b_doc.createElement("pay_notes");
	    val = b_doc.createTextNode( p.pay_notes );
	    tag.appendChild(val);
	    tag_payment.appendChild(tag);
	}

	// create images for patient
	QList<Image> images = DBConn::instance()->images( p.id );

	for( int z=0; z < images.size(); z++ ) {
	    Image img = images.at(z);

	    // create operation element
	    QDomElement tag_image = b_doc.createElement("image");
	    tag_patient.appendChild( tag_image );

	    // write the tags
	    tag = b_doc.createElement("id");
	    val = b_doc.createTextNode( QString::number(img.id) );
	    tag.appendChild(val);
	    tag_image.appendChild(tag);

	    tag = b_doc.createElement("patient_id");
	    val = b_doc.createTextNode( QString::number(img.patient_id) );
	    tag.appendChild(val);
	    tag_image.appendChild(tag);

	    tag = b_doc.createElement("img_date");
	    val = b_doc.createTextNode( img.img_date );
	    tag.appendChild(val);
	    tag_image.appendChild(tag);

	    tag = b_doc.createElement("img_name");
	    val = b_doc.createTextNode( img.img_name );
	    tag.appendChild(val);
	    tag_image.appendChild(tag);

	    tag = b_doc.createElement("img_notes");
	    val = b_doc.createTextNode( img.img_notes );
	    tag.appendChild(val);
	    tag_image.appendChild(tag);

	    // write image data here
	}

	// create images for patient
        QList<Prescription> prescriptions = DBConn::instance()->prescriptions( p.id );

	for( int z=0; z < prescriptions.size(); z++ ) {
	    Prescription pre = prescriptions.at(z);

	    // create prescription element
	    QDomElement tag_prescr = b_doc.createElement("prescription");
	    tag_patient.appendChild( tag_prescr );

	    // write the tags
	    tag = b_doc.createElement("id");
	    val = b_doc.createTextNode( QString::number(pre.id) );
	    tag.appendChild(val);
	    tag_prescr.appendChild(tag);

	    tag = b_doc.createElement("patient_id");
	    val = b_doc.createTextNode( QString::number(pre.patient_id) );
	    tag.appendChild(val);
	    tag_prescr.appendChild(tag);

	    tag = b_doc.createElement("prescr_date");
	    val = b_doc.createTextNode( pre.prescr_date );
	    tag.appendChild(val);
	    tag_prescr.appendChild(tag);

	    tag = b_doc.createElement("prescr_notes");
	    val = b_doc.createTextNode( pre.prescr_notes );
	    tag.appendChild(val);
	    tag_prescr.appendChild(tag);

	    tag = b_doc.createElement("prescr_text");
	    val = b_doc.createTextNode( pre.prescr_text );
	    tag.appendChild(val);
	    tag_prescr.appendChild(tag);
	}

        // Backup Operation Links
        for( int z=0; z < operations.size(); z++ ) {
            Operation op = operations.at(z);

            QList<Appointment> apps = DBConn::instance()->linkedAppointments( op.id );

            for( int w=0; w < apps.size(); w++ ) {
                Appointment apoint = apps.at(w);

                // create link element
                QDomElement tag_prescr = b_doc.createElement("operation-assoc");
                tag_patient.appendChild( tag_prescr );

                // write the tags
                tag = b_doc.createElement("type");
                val = b_doc.createTextNode( "appointment" );
                tag.appendChild(val);
                tag_prescr.appendChild(tag);

                tag = b_doc.createElement("from_id");
                val = b_doc.createTextNode( QString::number(op.id) );
                tag.appendChild(val);
                tag_prescr.appendChild(tag);

                tag = b_doc.createElement("to_id");
                val = b_doc.createTextNode( QString::number(apoint.id) );
                tag.appendChild(val);
                tag_prescr.appendChild(tag);
            }

            QList<Prescription> pres = DBConn::instance()->linkedPrescriptions( op.id );

            for( int w=0; w < pres.size(); w++ ) {
                Prescription apoint = pres.at(w);

                // create link element
                QDomElement tag_prescr = b_doc.createElement("operation-assoc");
                tag_patient.appendChild( tag_prescr );

                // write the tags
                tag = b_doc.createElement("type");
                val = b_doc.createTextNode( "prescription" );
                tag.appendChild(val);
                tag_prescr.appendChild(tag);

                tag = b_doc.createElement("from_id");
                val = b_doc.createTextNode( QString::number(op.id) );
                tag.appendChild(val);
                tag_prescr.appendChild(tag);

                tag = b_doc.createElement("to_id");
                val = b_doc.createTextNode( QString::number(apoint.id) );
                tag.appendChild(val);
                tag_prescr.appendChild(tag);
            }

            QList<Image> imgs = DBConn::instance()->linkedImages( op.id );

            for( int w=0; w < imgs.size(); w++ ) {
                Image apoint = imgs.at(w);

                // create link element
                QDomElement tag_prescr = b_doc.createElement("operation-assoc");
                tag_patient.appendChild( tag_prescr );

                // write the tags
                tag = b_doc.createElement("type");
                val = b_doc.createTextNode( "image" );
                tag.appendChild(val);
                tag_prescr.appendChild(tag);

                tag = b_doc.createElement("from_id");
                val = b_doc.createTextNode( QString::number(op.id) );
                tag.appendChild(val);
                tag_prescr.appendChild(tag);

                tag = b_doc.createElement("to_id");
                val = b_doc.createTextNode( QString::number(apoint.id) );
                tag.appendChild(val);
                tag_prescr.appendChild(tag);
            }
        }

        PatientHistoryObj pho = DBConn::instance()->patienthistory( p.id );

        if( pho.id > 0 ) {

            // create link element
            QDomElement tag_prescr = b_doc.createElement("patienthistory");
            tag_patient.appendChild( tag_prescr );

            // write the tags
            tag = b_doc.createElement("id");
            val = b_doc.createTextNode( QString::number(pho.id) );
            tag.appendChild(val);
            tag_prescr.appendChild(tag);

            tag = b_doc.createElement("patient_id");
            val = b_doc.createTextNode( QString::number(pho.patient_id) );
            tag.appendChild(val);
            tag_prescr.appendChild(tag);

            // write the tags
            tag = b_doc.createElement( "hist_diseases" );
            val = b_doc.createTextNode( pho.hist_diseases );
            tag.appendChild(val);
            tag_prescr.appendChild(tag);

            tag = b_doc.createElement( "hist_allergies" );
            val = b_doc.createTextNode( pho.hist_allergies );
            tag.appendChild(val);
            tag_prescr.appendChild(tag);

            tag = b_doc.createElement( "hist_surgeries" );
            val = b_doc.createTextNode( pho.hist_surgeries );
            tag.appendChild(val);
            tag_prescr.appendChild(tag);

            tag = b_doc.createElement( "hist_other2" );
            val = b_doc.createTextNode( pho.hist_other2 );
            tag.appendChild(val);
            tag_prescr.appendChild(tag);

            tag = b_doc.createElement( "hist_family" );
            val = b_doc.createTextNode( pho.hist_family );
            tag.appendChild(val);
            tag_prescr.appendChild(tag);

            tag = b_doc.createElement( "hist_medications" );
            val = b_doc.createTextNode( pho.hist_medications );
            tag.appendChild(val);
            tag_prescr.appendChild(tag);

            tag = b_doc.createElement( "hist_other" );
            val = b_doc.createTextNode( pho.hist_other );
            tag.appendChild(val);
            tag_prescr.appendChild(tag);

            tag = b_doc.createElement( "hist_habit_smoking" );
            val = b_doc.createTextNode( pho.hist_habit_smoking );
            tag.appendChild(val);
            tag_prescr.appendChild(tag);

            tag = b_doc.createElement( "hist_habit_sports" );
            val = b_doc.createTextNode( pho.hist_habit_sports );
            tag.appendChild(val);
            tag_prescr.appendChild(tag);

            tag = b_doc.createElement( "hist_habit_alcohol" );
            val = b_doc.createTextNode( pho.hist_habit_alcohol );
            tag.appendChild(val);
            tag_prescr.appendChild(tag);

            tag = b_doc.createElement( "hist_habit_other" );
            val = b_doc.createTextNode( pho.hist_habit_other );
            tag.appendChild(val);
            tag_prescr.appendChild(tag);
        }

	// fix progressbar
	this->pbar->setValue( (i*100)/patients.size() );
	// fix label
	this->label->setText( "(##%) " + QObject::tr("Backing up patients...") );
	this->label->setText( this->label->text().replace("##", QString::number( this->pbar->value() )) );
    }

    // write the file
    if( filename.isEmpty() ) {
	// just assign
	xmlBackup = b_doc.toString();

	return true;
    } else {

	// fix label
	this->label->setText( "(##%) " + QObject::tr("Saving to Hard Disk...") );
	this->label->setText( this->label->text().replace("##", QString::number( this->pbar->value() )) );

	// save the file
	QFile file( filename );

	if( file.open( QFile::WriteOnly | QIODevice::Text ) ) {
	    // dump the data inside the file
	    QTextStream ts( &file );
	    ts.setAutoDetectUnicode(true);
	    // set codec
	    QTextCodec *utfcodec = QTextCodec::codecForName( "UTF-8" );
	    ts.setCodec( utfcodec );

	    // write
	    ts << b_doc.toString();

	    // close file
	    file.close();

	    // echo success
	    GeneralDialog *gd = new GeneralDialog( "info", QObject::tr("local backup"), QObject::tr("Backup Created"), QObject::tr("Your backup was created successully!") );

	    gd->exec();

	    return true;
	}
    }
}

bool BackupEngine::importXMLBackup(QString filename) {
    // start import

    // create docuemnt and open the file
    QDomDocument doc("ProMed");
    QFile file( filename );

    if ( !file.open(QFile::ReadOnly | QIODevice::Text ) )
	return false;

    QTextStream ts( &file );
    ts.setAutoDetectUnicode(true);

    QTextCodec *utfcodec = QTextCodec::codecForName( "UTF-8" );
    ts.setCodec( utfcodec );

    // set content fo file to content of xml
    if ( !doc.setContent( ts.readAll() ) ) {
	file.close();
	return false;
    }

    // close file - we're done with it
    file.close();

    qDebug() << doc.toString();

    // make list of patients
    QDomNodeList nl = doc.elementsByTagName("patient");

    // iterate
    for( int i=0; i < nl.size(); i++ ) {
	// get current element
	QDomElement n_patient = nl.at(i).toElement();

	QDomNode patEntries = n_patient.firstChild();

	Patient p;

	while( !patEntries.isNull() ) {
	    QDomElement peData = patEntries.toElement();

	    if( peData.tagName() == "id" )
		p.id = peData.text().toInt();
	    else if( peData.tagName() == "name" )
		p.name = peData.text();
	    else if( peData.tagName() == "surname" )
		p.surname = peData.text();
	    else if( peData.tagName() == "fathersName" )
		p.fathersName = peData.text();
	    else if( peData.tagName() == "birthdate" )
		p.birthdate = peData.text();
	    else if( peData.tagName() == "address" )
		p.address = peData.text();
	    else if( peData.tagName() == "amka" )
		p.amka = peData.text();
	    else if( peData.tagName() == "created_on" )
		p.created_on = peData.text();
	    else if( peData.tagName() == "email" )
		p.email = peData.text();
	    else if( peData.tagName() == "gender" )
		p.gender = peData.text();
	    else if( peData.tagName() == "insurance" )
		p.insurance = peData.text();
	    else if( peData.tagName() == "notes" )
		p.notes = peData.text();
	    else if( peData.tagName() == "tel1" )
		p.tel1 = peData.text();
	    else if( peData.tagName() == "tel2" )
		p.tel2 = peData.text();
	    else if( peData.tagName() == "tel3" )
		p.tel3 = peData.text();
	    else if( peData.tagName() == "tel1_type" )
		p.tel1_type = peData.text();
	    else if( peData.tagName() == "tel2_type" )
		p.tel2_type = peData.text();
	    else if( peData.tagName() == "tel3_type" )
		p.tel3_type = peData.text();
	    else if( peData.tagName() == "vat" )
		p.vat = peData.text();

	    patEntries = patEntries.nextSibling();
	}

	// now save the patient - CHECK IF EXISTS FIRST, THEN UPDATE NOT SAVE
	if( DBConn::instance()->patient( p.id ).id > 0 )
	    DBConn::instance()->updatePatient(p);
	else
	    DBConn::instance()->insertPatient(p);

	// list operations
	QDomNodeList nl_ops = n_patient.elementsByTagName("operation");
	for( int z=0; z < nl_ops.size(); z++ ) {
	    // get current operation
	    QDomElement n_operation = nl_ops.at(z).toElement();

	    QDomNode pEntries = n_operation.firstChild();

	    Operation op;

	    while( !pEntries.isNull() ) {
		QDomElement peData = pEntries.toElement();

		if( peData.tagName() == "id" )
		    op.id = peData.text().toInt();
		else if( peData.tagName() == "patient_id" )
		    op.patient_id = peData.text().toInt();
		else if( peData.tagName() == "op_date" )
		    op.op_date = peData.text();
		else if( peData.tagName() == "op_notes" )
		    op.op_notes = peData.text();
		else if( peData.tagName() == "op_type" )
		    op.op_type = peData.text();

		pEntries = pEntries.nextSibling();
	    }

	    // save operation - check if it exists already, in which case update it
	    if( DBConn::instance()->operation(op.id).id > 0 )
		DBConn::instance()->updateOperation(op);
	    else
		DBConn::instance()->insertOperation(op);
	}

	// list appointments
	QDomNodeList nl_aps = n_patient.elementsByTagName("appointment");
	for( int z=0; z < nl_aps.size(); z++ ) {
	    // get current appointment
	    QDomElement n_appointment = nl_aps.at(z).toElement();

	    QDomNode pEntries = n_appointment.firstChild();

	    Appointment ap;

	    while( !pEntries.isNull() ) {
		QDomElement peData = pEntries.toElement();

		if( peData.tagName() == "id" )
		    ap.id = peData.text().toInt();
		else if( peData.tagName() == "patient_id" )
		    ap.patient_id = peData.text().toInt();
		else if( peData.tagName() == "ap_date" )
		    ap.ap_date = peData.text();
		else if( peData.tagName() == "ap_notes" )
		    ap.ap_notes = peData.text();
		else if( peData.tagName() == "ap_time" )
		    ap.ap_time = peData.text();

		pEntries = pEntries.nextSibling();
	    }

	    // save appointment - check if it exists already, in which case update it
	    if( DBConn::instance()->appointment(ap.id).id > 0 )
		DBConn::instance()->updateAppointment(ap);
	    else
		DBConn::instance()->insertAppointment(ap);
	}

	// list payments
	QDomNodeList nl_pays = n_patient.elementsByTagName("payment");
	for( int z=0; z < nl_pays.size(); z++ ) {
	    // get current payment
	    QDomElement n_payment = nl_pays.at(z).toElement();

	    QDomNode pEntries = n_payment.firstChild();

	    Payment pay;

	    while( !pEntries.isNull() ) {
		QDomElement peData = pEntries.toElement();

		if( peData.tagName() == "id" )
		    pay.id = peData.text().toInt();
		else if( peData.tagName() == "patient_id" )
		    pay.patient_id = peData.text().toInt();
		else if( peData.tagName() == "pay_amount" )
		    pay.pay_amount = peData.text();
		else if( peData.tagName() == "pay_notes" )
		    pay.pay_notes = peData.text();
		else if( peData.tagName() == "pay_date" )
		    pay.pay_date = peData.text();
		else if( peData.tagName() == "pay_status" )
		    pay.pay_status = peData.text().toInt();

		pEntries = pEntries.nextSibling();
	    }

	    // save payment - check if it exists already, in which case update it
	    if( DBConn::instance()->payment(pay.id).id > 0 )
		DBConn::instance()->updatePayment(pay);
	    else
		DBConn::instance()->insertPayment(pay);
	}

	// list images
	QDomNodeList nl_img = n_patient.elementsByTagName("image");
	for( int z=0; z < nl_img.size(); z++ ) {
	    // get current image
	    QDomElement n_image = nl_img.at(z).toElement();

	    QDomNode pEntries = n_image.firstChild();

	    Image img;

	    while( !pEntries.isNull() ) {
		QDomElement peData = pEntries.toElement();

		if( peData.tagName() == "id" )
		    img.id = peData.text().toInt();
		else if( peData.tagName() == "patient_id" )
		    img.patient_id = peData.text().toInt();
		else if( peData.tagName() == "img_date" )
		    img.img_date = peData.text();
		else if( peData.tagName() == "img_name" )
		    img.img_name = peData.text();
		else if( peData.tagName() == "img_notes" )
		    img.img_notes = peData.text();

		pEntries = pEntries.nextSibling();
	    }

	    // save image - check if it exists already, in which case update it
	    if( DBConn::instance()->image(img.id).id > 0 )
		DBConn::instance()->updateImage(img);
	    else
		DBConn::instance()->insertImage(img);
	}

	// list prescriptions
	QDomNodeList nl_pre = n_patient.elementsByTagName("prescription");
	for( int z=0; z < nl_pre.size(); z++ ) {
	    // get current prescription
	    QDomElement n_prescription = nl_pre.at(z).toElement();

	    QDomNode pEntries = n_prescription.firstChild();

	    Prescription pre;

	    while( !pEntries.isNull() ) {
		QDomElement peData = pEntries.toElement();

		if( peData.tagName() == "id" )
		    pre.id = peData.text().toInt();
		else if( peData.tagName() == "patient_id" )
		    pre.patient_id = peData.text().toInt();
		else if( peData.tagName() == "prescr_date" )
		    pre.prescr_date = peData.text();
		else if( peData.tagName() == "prescr_notes" )
		    pre.prescr_notes = peData.text();
		else if( peData.tagName() == "prescr_text" )
		    pre.prescr_text = peData.text();

		pEntries = pEntries.nextSibling();
	    }

	    // save prescription - check if it exists already, in which case update it
	    if( DBConn::instance()->prescription(pre.id).id > 0 )
		DBConn::instance()->updatePrescription(pre);
	    else
		DBConn::instance()->insertPrescription(pre);
	}

        // list LINKS
        QDomNodeList nl_lnk = n_patient.elementsByTagName("operation-assoc");
        for( int z=0; z < nl_lnk.size(); z++ ) {
            // get current link
            QDomElement n_link = nl_lnk.at(z).toElement();

            QDomNode pEntries = n_link.firstChild();

            QString s_type; int s_from, s_to;

            while( !pEntries.isNull() ) {
                QDomElement peData = pEntries.toElement();

                if( peData.tagName() == "type" )
                    s_type = peData.text();
                else if( peData.tagName() == "from_id" )
                    s_from = peData.text().toInt();
                else if( peData.tagName() == "to_id" )
                    s_to = peData.text().toInt();

                pEntries = pEntries.nextSibling();
            }

            // save link
            DBConn::instance()->linkOperation( s_type, s_from, s_to );
        }

        // Insert PatientHistory
        QDomNodeList nl_his = n_patient.elementsByTagName("patienthistory");
        for( int z=0; z < nl_his.size(); z++ ) {
            // get current link
            QDomElement n_link = nl_his.at(z).toElement();

            QDomNode pEntries = n_link.firstChild();

            PatientHistoryObj pho;

            while( !pEntries.isNull() ) {
                QDomElement peData = pEntries.toElement();

                if( peData.tagName() == "id" )
                    pho.id = peData.text().toInt();
                if( peData.tagName() == "patient_id" )
                    pho.patient_id = peData.text().toInt();
                else if( peData.tagName() == "hist_diseases" )
                    pho.hist_diseases = peData.text();
                else if( peData.tagName() == "hist_allergies" )
                    pho.hist_allergies = peData.text();
                else if( peData.tagName() == "hist_surgeries" )
                    pho.hist_surgeries = peData.text();
                else if( peData.tagName() == "hist_other2" )
                    pho.hist_other2 = peData.text();
                else if( peData.tagName() == "hist_family" )
                    pho.hist_family = peData.text();
                else if( peData.tagName() == "hist_medications" )
                    pho.hist_medications = peData.text();
                else if( peData.tagName() == "hist_other" )
                    pho.hist_other = peData.text();
                else if( peData.tagName() == "hist_habit_smoking" )
                    pho.hist_habit_smoking = peData.text();
                else if( peData.tagName() == "hist_habit_sports" )
                    pho.hist_habit_sports = peData.text();
                else if( peData.tagName() == "hist_habit_alcohol" )
                    pho.hist_habit_alcohol = peData.text();
                else if( peData.tagName() == "hist_habit_other" )
                    pho.hist_habit_other = peData.text();

                pEntries = pEntries.nextSibling();
            }

            // save
            if( DBConn::instance()->patienthistory(p.id).id > 0 )
                DBConn::instance()->updatePatientHistory( pho );
            else
                DBConn::instance()->insertPatientHistory( pho );
        }

	// fix progressbar
	this->pbar->setValue( (i*100)/nl.size() );
	// fix label
	this->label->setText( "(##%) " + QObject::tr("Importing patients...") );
	this->label->setText( this->label->text().replace("##", QString::number( this->pbar->value() )) );
    }

    // before return, reload patients
    MainWindow::instance()->reloadPatientsList();

    return true;
}

bool BackupEngine::uploadXMLBackup() {
    // upload backup to server
}

BackupEngine::~BackupEngine() {
}
