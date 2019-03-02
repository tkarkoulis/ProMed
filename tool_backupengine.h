/**************************************************************************
**   Recompile IT Services, Development Dept
**   11/22/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef TOOL_BACKUPENGINE_H
#define TOOL_BACKUPENGINE_H

#include <QtXml/QDomDocument>
#include <QString>
#include <QLabel>
#include <QProgressBar>
#include <QObject>

class BackupEngine
{
    public:
	BackupEngine();
	BackupEngine( QLabel *label = 0, QProgressBar *pbar = 0);
	~BackupEngine();
	QString toPlainTextXML();
	bool createXMLBackup( QString filename = "" );
	bool importXMLBackup( QString filename = "" );
	bool uploadXMLBackup();

    private:
	QDomDocument b_doc;
	QLabel *label;
	QProgressBar *pbar;
	QString xmlBackup;
};

#endif // TOOL_BACKUPENGINE_H
