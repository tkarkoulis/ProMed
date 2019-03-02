/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/18/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef DIALOG_REGISTERDIALOG_H
#define DIALOG_REGISTERDIALOG_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
    class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = 0);
    QString getEmail();
    QString getCode();
    ~RegisterDialog();

private:
    Ui::RegisterDialog *ui;
    void apply();
    QPushButton *registerNow;
    QPushButton *cancel;
    QPushButton *processRegister;

private slots:
    void registerNowClicked();
};

#endif // DIALOG_REGISTERDIALOG_H
