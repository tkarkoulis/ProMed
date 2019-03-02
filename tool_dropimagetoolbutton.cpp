/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/29/2010 2010
**   All Rights Reserved
**************************************************************************/

#include <QList>
#include <QUrl>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDragEnterEvent>
#include <QMimeData>

#include "tool_dropimagetoolbutton.h"

DropImageToolButton::DropImageToolButton(QWidget *parent) : QToolButton(parent)
{
    // start here
    setAcceptDrops(true);
}

void DropImageToolButton::dragEnterEvent(QDragEnterEvent *event) {
    // accept
    if( event->mimeData()->urls().size() == 1 ) {
	QFileInfo fi_file( event->mimeData()->urls().at(0).toLocalFile() );
	// if its a file
	if( fi_file.isFile() )
	    event->acceptProposedAction();
    }
}

void DropImageToolButton::dropEvent(QDropEvent *event) {
    // lets see
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
	qDebug() << "nothing here... :-(";
	return;
    }

    // emit signal
    emit fileDropped( event->mimeData()->urls().at(0).toLocalFile() );
}

DropImageToolButton::~DropImageToolButton() {
    // destructor
}
