/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/18/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef DIALOG_GENERALDIALOG_H
#define DIALOG_GENERALDIALOG_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
    class GeneralDialog;
}

class GeneralDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GeneralDialog(QString type = "info", QString window_title = "Dialog", QString dlg_message_title = "Message Title", QString dlg_message = "Message", QWidget *parent = 0);
    ~GeneralDialog();
    void addButton( QPushButton *button );

private:
    Ui::GeneralDialog *ui;
};

#endif // DIALOG_GENERALDIALOG_H
