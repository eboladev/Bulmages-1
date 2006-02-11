//
// C++ Interface: ListDescuentoFacturaView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTDESCFACTURAVIEW_H
#define LISTDESCFACTURAVIEW_H

/**
@author Tomeu Borras
*/

#include <Q3Table>

#include "listdescfactura.h"
#include "company.h"
#include "descfactura.h"




class ListDescuentoFacturaView : public Q3Table , public ListDescuentoFactura {
Q_OBJECT
public:
    ListDescuentoFacturaView(QWidget *parent=0, const char *name=0);
    ~ListDescuentoFacturaView();
    virtual void pintaListDescuentoFactura();
    virtual void pintadescListDescuentoFactura(int);
    DescuentoFactura *lineaat(int);
    DescuentoFactura *lineaact();
public slots:
    virtual void valueBudgetLineChanged(int row, int col);
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void borradescfacturaact();
};

#endif
