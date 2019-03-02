/**************************************************************************
**   Recompile IT Services, Development Dept
**   11/1/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef DIALOG_VIEWIMAGE_H
#define DIALOG_VIEWIMAGE_H

#include <QDialog>
#include "widget_viewpatient.h"
#include "dialog_addoperation.h"

namespace Ui {
    class ViewImage;
}

class ViewImage : public QDialog
{
    Q_OBJECT

public:
    explicit ViewImage(QWidget *parent = 0, int image_id = 0, ViewPatient *p = 0 );
    int action();
    void setOperationDialog( AddOperation *ao );
    ~ViewImage();

private:
    Ui::ViewImage *ui;
    ViewPatient *vp;
    AddOperation *aop;
    int i_action;
    int imageID;

private slots:
    void deleteClicked();
};

#endif // DIALOG_VIEWIMAGE_H
