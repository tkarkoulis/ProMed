/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/27/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef OBJ_PAYMENT_H
#define OBJ_PAYMENT_H

#include <QString>

#define PAY_PENDING 1
#define PAY_COMPLETED 2

class Payment
{
public:
    Payment();
    int id;
    int patient_id;
    QString pay_amount;
    int pay_status;
    QString pay_notes;
    QString pay_date;
};

#endif // OBJ_PAYMENT_H
