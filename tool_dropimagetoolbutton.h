/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/29/2010 2010
**   All Rights Reserved
**************************************************************************/

#include <QToolButton>
#include <QDropEvent>

#ifndef TOOL_DROPIMAGETOOLBUTTON_H
#define TOOL_DROPIMAGETOOLBUTTON_H

class DropImageToolButton : public QToolButton
{
    Q_OBJECT

public:
    explicit DropImageToolButton(QWidget *parent = 0);
    ~DropImageToolButton();

protected:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

signals:
    void fileDropped( QString fileURL );
};

#endif // TOOL_DROPIMAGETOOLBUTTON_H
