/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/18/2010 2010
**   All Rights Reserved
**************************************************************************/

#include <QDebug>
#include <QStringList>
#include <QRegExp>
#include <QMessageBox>

#include "tool_recompilecrypto.h"

RecompileCrypto::RecompileCrypto() {
    // Initialize the public key
    public_key = "reprodentcompileutilityprotectionkey";

    // init table
    map_asciiTable.insert( QString::fromUtf8("α"), 1000);
    map_asciiTable.insert( QString::fromUtf8("β"), 1001);
    map_asciiTable.insert( QString::fromUtf8("γ"), 1002);
    map_asciiTable.insert( QString::fromUtf8("δ"), 1003);
    map_asciiTable.insert( QString::fromUtf8("ε"), 1004);
    map_asciiTable.insert( QString::fromUtf8("ζ"), 1005);
    map_asciiTable.insert( QString::fromUtf8("η"), 1006);
    map_asciiTable.insert( QString::fromUtf8("θ"), 1007);
    map_asciiTable.insert( QString::fromUtf8("ι"), 1008);
    map_asciiTable.insert( QString::fromUtf8("κ"), 1009);
    map_asciiTable.insert( QString::fromUtf8("λ"), 1010);
    map_asciiTable.insert( QString::fromUtf8("μ"), 1011);
    map_asciiTable.insert( QString::fromUtf8("ν"), 1012);
    map_asciiTable.insert( QString::fromUtf8("ξ"), 1013);
    map_asciiTable.insert( QString::fromUtf8("ο"), 1014);
    map_asciiTable.insert( QString::fromUtf8("π"), 1015);
    map_asciiTable.insert( QString::fromUtf8("ρ"), 1016);
    map_asciiTable.insert( QString::fromUtf8("σ"), 1017);
    map_asciiTable.insert( QString::fromUtf8("τ"), 1018);
    map_asciiTable.insert( QString::fromUtf8("υ"), 1019);
    map_asciiTable.insert( QString::fromUtf8("φ"), 1020);
    map_asciiTable.insert( QString::fromUtf8("χ"), 1021);
    map_asciiTable.insert( QString::fromUtf8("ψ"), 1022);
    map_asciiTable.insert( QString::fromUtf8("ω"), 1023);
    map_asciiTable.insert( QString::fromUtf8("ς"), 1024);
    map_asciiTable.insert( QString::fromUtf8("ά"), 1025);
    map_asciiTable.insert( QString::fromUtf8("έ"), 1026);
    map_asciiTable.insert( QString::fromUtf8("ή"), 1027);
    map_asciiTable.insert( QString::fromUtf8("ί"), 1028);
    map_asciiTable.insert( QString::fromUtf8("ϊ"), 1029);
    map_asciiTable.insert( QString::fromUtf8("ό"), 1030);
    map_asciiTable.insert( QString::fromUtf8("ύ"), 1031);
    map_asciiTable.insert( QString::fromUtf8("ϋ"), 1032);
    map_asciiTable.insert( QString::fromUtf8("ώ"), 1033);
    // caps
    map_asciiTable.insert( QString::fromUtf8("Α"), 1034);
    map_asciiTable.insert( QString::fromUtf8("Β"), 1035);
    map_asciiTable.insert( QString::fromUtf8("Γ"), 1036);
    map_asciiTable.insert( QString::fromUtf8("Δ"), 1037);
    map_asciiTable.insert( QString::fromUtf8("Ε"), 1038);
    map_asciiTable.insert( QString::fromUtf8("Ζ"), 1039);
    map_asciiTable.insert( QString::fromUtf8("Η"), 1040);
    map_asciiTable.insert( QString::fromUtf8("Θ"), 1041);
    map_asciiTable.insert( QString::fromUtf8("Ι"), 1042);
    map_asciiTable.insert( QString::fromUtf8("Κ"), 1043);
    map_asciiTable.insert( QString::fromUtf8("Λ"), 1044);
    map_asciiTable.insert( QString::fromUtf8("Μ"), 1045);
    map_asciiTable.insert( QString::fromUtf8("Ν"), 1046);
    map_asciiTable.insert( QString::fromUtf8("Ξ"), 1047);
    map_asciiTable.insert( QString::fromUtf8("Ο"), 1048);
    map_asciiTable.insert( QString::fromUtf8("Π"), 1049);
    map_asciiTable.insert( QString::fromUtf8("Ρ"), 1050);
    map_asciiTable.insert( QString::fromUtf8("Σ"), 1051);
    map_asciiTable.insert( QString::fromUtf8("Τ"), 1052);
    map_asciiTable.insert( QString::fromUtf8("Υ"), 1053);
    map_asciiTable.insert( QString::fromUtf8("Φ"), 1054);
    map_asciiTable.insert( QString::fromUtf8("Χ"), 1055);
    map_asciiTable.insert( QString::fromUtf8("Ψ"), 1056);
    map_asciiTable.insert( QString::fromUtf8("Ω"), 1057);
    map_asciiTable.insert( QString::fromUtf8("Ά"), 1059);
    map_asciiTable.insert( QString::fromUtf8("Έ"), 1060);
    map_asciiTable.insert( QString::fromUtf8("Ή"), 1061);
    map_asciiTable.insert( QString::fromUtf8("Ί"), 1062);
    map_asciiTable.insert( QString::fromUtf8("Ϊ"), 1063);
    map_asciiTable.insert( QString::fromUtf8("Ό"), 1064);
    map_asciiTable.insert( QString::fromUtf8("Ύ"), 1065);
    map_asciiTable.insert( QString::fromUtf8("Ϋ"), 1066);
    map_asciiTable.insert( QString::fromUtf8("Ώ"), 1067);
}

RecompileCrypto::~RecompileCrypto() {
    // destructor
}

QString RecompileCrypto::encodeString(QString str) {
    // start encryption
    QString str_encrypted;

    if( public_key == "" ) return str;
    else {
        // init variables
        unsigned int i,j;

        for(i=0,j=0;i<str.length();i++,j++) {
            // inside encryption loop
            if(j>=public_key.length())
            {
                j=0; // rotate the key
            }

        int value = str.at(i).toLatin1() + public_key.at(j).toLatin1() - 128;

            // return result
	    str_encrypted.append( value + "#");
        }

        return str_encrypted;
    }
}

QString RecompileCrypto::decodeString(QString str) {
    // start decryption
    unsigned int i,j;
    int value;

    QString str_decrypted;

    QStringList strl = str.split( "#" );

    for(i=0,j=0;i< (strl.size()-1) ;i++,j++)
    {
	QString s = strl.at(i);

	// if value is larger than 1000 that means char from the ascii tble
	if( s.toInt() >= 1000 ) { // ascii table char
	    str_decrypted.append( map_asciiTable.key( s.toInt(), "-" ) );
	    continue;
	}

        //repeat the key if you are at end of it.
        if(j>=public_key.length())
        {
           j=0;
        }

        //similar to encipher only difference is you need to subtract
    value = ( s.toInt()+128) - ( public_key.at(j).toLatin1() );

        // decrypt
	str_decrypted.append( value );
    }

    return str_decrypted;
}

bool RecompileCrypto::validKey(QString email, QString key) {
    return true;
}
