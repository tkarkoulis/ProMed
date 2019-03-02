/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/12/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef PATIENT_H
#define PATIENT_H

#include <QString>

class Patient
{
public:
    Patient();
    QString name;
    QString surname;
    QString fathersName;
    QString vat;
    QString tel1;
    QString tel1_type;
    QString tel2;
    QString tel2_type;
    QString tel3;
    QString tel3_type;
    QString address;
    QString birthdate;
    QString gender;
    QString insurance;
    QString email;
    QString notes;
    int id;
    QString created_on;
    QString amka;
    QString cancer_trace;
    QString children;
    QString nationality;
    QString inheritance;
};


#endif // PATIENT_H
