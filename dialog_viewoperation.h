/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/18/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef DIALOG_VIEWOPERATION_H
#define DIALOG_VIEWOPERATION_H

#include <QDialog>

namespace Ui {
    class ViewOperation;
}

class ViewOperation : public QDialog
{
    Q_OBJECT

public:
    explicit ViewOperation(QWidget *parent = 0, int operation_id = 0);
    ~ViewOperation();

private:
    Ui::ViewOperation *ui;
};

#endif // DIALOG_VIEWOPERATION_H
