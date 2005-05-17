//
// C++ Interface: confreglasview
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CONFREGLASVIEW_H
#define CONFREGLASVIEW_H

#include <confreglas.h>

/**
@author Tomeu Borras
*/
class confreglasview : public confreglas {
Q_OBJECT
public:
    confreglasview(QWidget *parent = 0, const char *name = 0);

    ~confreglasview();

};

#endif
