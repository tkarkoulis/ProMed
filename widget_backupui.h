/**************************************************************************
**   Recompile IT Services, Development Dept
**   11/22/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef WIDGET_BACKUPUI_H
#define WIDGET_BACKUPUI_H

#include <QWidget>

namespace Ui {
    class BackupUI;
}

class BackupUI : public QWidget
{
    Q_OBJECT

public:
    explicit BackupUI(QWidget *parent = 0);
    ~BackupUI();

private:
    Ui::BackupUI *ui;

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // WIDGET_BACKUPUI_H
