/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/21/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef WIDGET_STATISTICS_H
#define WIDGET_STATISTICS_H

#include <QWidget>
#include <QDate>
#include <QTreeWidgetItem>

namespace Ui {
    class Statistics;
}

class Statistics : public QWidget
{
    Q_OBJECT

public:
    explicit Statistics(QWidget *parent = 0);
    void reloadAppointments();
    void reloadOperations();
    void reloadDemographic();
    ~Statistics();

private:
    Ui::Statistics *ui;
    QDate app_date;
    QDate op_date;

private slots:
    void on_tbl_demoData_itemDoubleClicked(QTreeWidgetItem* item, int column);
    void on_spin_demoYear_editingFinished();
    void on_txt_demoCity_returnPressed();
    void on_txt_demoName_returnPressed();
    void on_txt_demoSurname_returnPressed();
    void on_btn_demoSearch_clicked();
    void on_btn_opNext_clicked();
    void on_btn_opPrevious_clicked();
    void on_tbl_operationsNames_itemDoubleClicked(QTreeWidgetItem* item, int column);
    void on_btn_opRefresh_clicked();
    void on_btn_next_clicked();
    void on_btn_previous_clicked();
    void on_btn_refresh_clicked();
};

#endif // WIDGET_STATISTICS_H
