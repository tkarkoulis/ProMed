/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/18/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef OPERATION_H
#define OPERATION_H

#include <QString>

class Operation
{
public:
    Operation();
    int id;
    int patient_id;
    QString op_date;
    QString op_type;
    QString op_notes;
};

#endif // OPERATION_H
