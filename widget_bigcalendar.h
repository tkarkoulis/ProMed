/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/20/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef WIDGET_BIGCALENDAR_H
#define WIDGET_BIGCALENDAR_H

#include <QWidget>
#include <QDate>
#include <QLayout>

namespace Ui {
    class BigCalendar;
}

class BigCalendar : public QWidget
{
    Q_OBJECT

public:
    explicit BigCalendar(QWidget *parent = 0, QString os = "WIN");
    ~BigCalendar();
    void reloadMonth();
    void reloadWeek();

private:
    Ui::BigCalendar *ui;
    QDate selectedDate, selectedDateWeek;
    void clearLayout(QLayout *l);
    void createMonthHeaders();
    void createWeekHeaders();
    QString osType;

private slots:
    void on_toolButton_2_clicked();
    void on_toolButton_clicked();
    void on_cal_year_valueChanged(int );
    void on_cal_month_currentIndexChanged(int index);
    void on_cal_next_clicked();
    void on_cal_previous_clicked();
    void app_clicked(QString link);
    void day_clicked(QString link);
};

#endif // WIDGET_BIGCALENDAR_H
