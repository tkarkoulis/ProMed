/**************************************************************************
**   Recompile IT Services, Development Dept
**   11/19/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef OBJ_PRESCRIPTION_H
#define OBJ_PRESCRIPTION_H

#include <QString>

class Prescription
{
public:
    Prescription();
    int id;
    int patient_id;
    QString prescr_date;
    QString prescr_text;
    QString prescr_notes;
};

#endif // OBJ_PRESCRIPTION_H
