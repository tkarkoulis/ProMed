/**************************************************************************
**   Recompile IT Services, Development Dept
**   11/3/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef DIALOG_ABOUTDIALOG_H
#define DIALOG_ABOUTDIALOG_H

#include <QDialog>
#include <QUrl>

namespace Ui {
    class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0, QString osType = "WIN" );
    ~AboutDialog();

private:
    Ui::AboutDialog *ui;
};

#endif // DIALOG_ABOUTDIALOG_H
