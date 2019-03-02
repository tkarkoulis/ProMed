/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/22/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef WIDGET_DENTALSETTINGS_H
#define WIDGET_DENTALSETTINGS_H

#include <QWidget>

namespace Ui {
    class DentalSettings;
}

class DentalSettings : public QWidget
{
    Q_OBJECT

public:
    explicit DentalSettings(QWidget *parent = 0);
    void reload();
    ~DentalSettings();

private:
    Ui::DentalSettings *ui;

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // WIDGET_DENTALSETTINGS_H
