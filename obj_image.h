/**************************************************************************
**   Recompile IT Services, Development Dept
**   10/27/2010 2010
**   All Rights Reserved
**************************************************************************/

#ifndef OBJ_IMAGE_H
#define OBJ_IMAGE_H

#include <QString>

class Image
{
public:
    Image();
    int id;
    int patient_id;
    QString img_name;
    QString img_notes;
    QString img_date;
};

#endif // OBJ_IMAGE_H
