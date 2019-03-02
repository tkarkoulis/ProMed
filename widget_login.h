/**************************************************************************
**   Recompile IT Services, Development Dept
**   11/1/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef WIDGET_LOGIN_H
#define WIDGET_LOGIN_H

#include <QWidget>

namespace Ui {
    class LogIn;
}

class LogIn : public QWidget
{
    Q_OBJECT

public:
    explicit LogIn(QWidget *parent = 0);
    void clear();
    ~LogIn();

private:
    Ui::LogIn *ui;
    void startLogin();

private slots:
    void on_login_pass_returnPressed();
    void on_but_login_clicked();
};

#endif // WIDGET_LOGIN_H
