//
// C++ Interface: ListDescuentoPresupuestoView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTDESCPRESUPUESTOVIEW_H
#define LISTDESCPRESUPUESTOVIEW_H

/**
@author Tomeu Borras
*/

#include <q3table.h>

#include "listdescpresupuesto.h"
#include "company.h"
#include "descpresupuesto.h"




class ListDescuentoPresupuestoView : public Q3Table , public ListDescuentoPresupuesto {
Q_OBJECT
public:
    ListDescuentoPresupuestoView(QWidget *parent=0, const char *name=0);
    ~ListDescuentoPresupuestoView();
    virtual void pintaListDescuentoPresupuesto();
    virtual void pintadescListDescuentoPresupuesto(int);
    DescuentoPresupuesto *lineaat(int);
    DescuentoPresupuesto *lineaact();
public slots:
    virtual void valueBudgetLineChanged(int row, int col);
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void borradescpresupuestoact();
};

#endif
