/**************************************************************************
**   Recompile IT Services, Development Dept
**   12/14/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef OBJ_PATIENTHISTORYOBJ_H
#define OBJ_PATIENTHISTORYOBJ_H

#include <QString>

class PatientHistoryObj
{
public:
    PatientHistoryObj();
    int id;
    int patient_id;
    QString hist_diseases;
    QString hist_allergies;
    QString hist_surgeries;
    QString hist_other2;
    QString hist_family;
    QString hist_medications;
    QString hist_other;
    QString hist_habit_smoking;
    QString hist_habit_sports;
    QString hist_habit_alcohol;
    QString hist_habit_other;
};

#endif // OBJ_PATIENTHISTORY_H
