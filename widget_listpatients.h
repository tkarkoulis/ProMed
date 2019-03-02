/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/13/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef LISTPATIENTS_H
#define LISTPATIENTS_H

#include <QWidget>
#include <QTreeWidgetItem>
#include "obj_patient.h"

namespace Ui {
    class ListPatients;
}

class ListPatients : public QWidget
{
    Q_OBJECT

public:
    explicit ListPatients(QWidget *parent = 0);
    void reload();
    ~ListPatients();

private:
    Ui::ListPatients *ui;
    void loadPatientsfromDB();
    void loadPatientsfromList( QList<Patient> patients );
    Qt::SortOrder sortOrder;

    void hide_items(QList<QTreeWidgetItem *> found_items);
    void filter_items();

private slots:
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_filter_telephone_textChanged(QString );
    void on_filter_fathersName_textChanged(QString );
    void on_filter_surname_textChanged(QString );
    void on_filter_name_textChanged(QString );
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column);
    void treeWidget_sortChanged( int itemIndex, Qt::SortOrder order );
    void treeWidget_headerClicked( int itemIndex );
    void on_filter_age_textChanged(const QString &arg1);
    void on_filter_children_currentIndexChanged(int index);
    void on_filter_nationality_currentIndexChanged(const QString &arg1);
    void on_filter_inheritance_currentIndexChanged(int index);
    void on_filter_cancer_currentIndexChanged(int index);  
};

#endif // LISTPATIENTS_H
