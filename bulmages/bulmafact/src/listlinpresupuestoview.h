//
// C++ Interface: listlinpresupuestoview
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLINPRESUPUESTOVIEW_H
#define LISTLINPRESUPUESTOVIEW_H

/**
@author Tomeu Borras
*/

#include <qtable.h>

#include "listlinpresupuesto.h"
#include "company.h"
#include "linpresupuesto.h"

#define COL_IDLPRESUPUESTO 0
#define COL_IDARTICULO 1
#define COL_CODARTICULO 2
#define COL_NOMARTICULO 3
#define COL_DESCLPRESUPUESTO 4
#define COL_CANTLPRESUPUESTO 5
#define COL_PVPLPRESUPUESTO 6
#define COL_DESCUENTOLPRESUPUESTO 7
#define COL_IDPRESUPUESTO 8
#define COL_REMOVE 9
#define COL_TASATIPO_IVA 10
#define COL_TIPO_IVA 11


class listlinpresupuestoview : public QTable, public listlinpresupuesto {
public:
listlinpresupuestoview(company *comp,  QWidget *parent=0, const char *name=0);
    ~listlinpresupuestoview();
    void pintalistlinpresupuesto();
};

#endif
