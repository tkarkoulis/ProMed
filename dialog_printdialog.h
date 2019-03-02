/**************************************************************************
**   Recompile IT Services, Development Dept
**   11/22/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef DIALOG_PRINTDIALOG_H
#define DIALOG_PRINTDIALOG_H

#include <QDialog>

namespace Ui {
    class PrintDialog;
}

class PrintDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrintDialog(QWidget *parent = 0, bool readOnly = true);
    void loadData(QString data);
    void subVar( QString var, QString data);
    void printNow();
    ~PrintDialog();

private:
    Ui::PrintDialog *ui;
    QString htmlData;

private slots:
    void on_pushButton_clicked();
};

#endif // DIALOG_PRINTDIALOG_H
