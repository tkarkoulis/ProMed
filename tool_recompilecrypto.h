/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/18/2010 2010
**   All Rights Reserved
**************************************************************************/
#ifndef TOOL_RECOMPILECRYPTO_H
#define TOOL_RECOMPILECRYPTO_H

#include <QString>
#include <QMap>

class RecompileCrypto // cryptography class
{
public:
    RecompileCrypto();
    ~RecompileCrypto();
    QString encodeString( QString str );
    QString decodeString( QString str );
    bool validKey( QString email, QString key );

protected:
    QString public_key;
    QMap<QString, int> map_asciiTable;
};

#endif // TOOL_RECOMPILECRYPTO_H
