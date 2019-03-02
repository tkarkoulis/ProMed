/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/29/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef DIALOG_ADDIMAGE_H
#define DIALOG_ADDIMAGE_H

#include <QDialog>

namespace Ui {
    class AddImage;
}

class AddImage : public QDialog
{
    Q_OBJECT

public:
    explicit AddImage(QWidget *parent = 0);
    QString file();
    QString description();
    ~AddImage();

private:
    Ui::AddImage *ui;
    QString fileLocation;

private slots:
    void on_pushButton_clicked();
    void fileDrop( QString filename );
};

#endif // DIALOG_ADDIMAGE_H
