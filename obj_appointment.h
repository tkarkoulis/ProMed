/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/18/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <QString>

class Appointment
{
public:
    Appointment();
    int id;
    int patient_id;
    QString ap_date;
    QString ap_notes;
    QString ap_notes2;
    QString ap_time;
};

#endif // APPOINTMENT_H
