/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/29/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef DIALOG_PAYMENTDLG_H
#define DIALOG_PAYMENTDLG_H

#include <QDialog>
#include <QDate>

#include "obj_payment.h"

namespace Ui {
    class PaymentDlg;
}

class PaymentDlg : public QDialog
{
    Q_OBJECT

public:
    explicit PaymentDlg(QWidget *parent = 0, int payment_id = 0);
    QDate getDate();
    QString getNotes();
    float getAmount();
    int getStatus();
    int paymentID;
    int action();
    ~PaymentDlg();

private:
    Ui::PaymentDlg *ui;
    void loadPaymentValues(Payment p);
    int i_action;

private slots:
    void deleteClicked();
    void updateClicked();
};

#endif // DIALOG_PAYMENTDLG_H
