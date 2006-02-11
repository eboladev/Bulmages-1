//
// C++ Interface: ListDescuentoFacturaProvView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTDESCFACTURAPROVVIEW_H
#define LISTDESCFACTURAPROVVIEW_H

/**
@author Tomeu Borras
*/

#include <Q3Table>

#include "listdescfacturaprov.h"
#include "company.h"
#include "descfacturaprov.h"




class ListDescuentoFacturaProvView : public Q3Table , public ListDescuentoFacturaProv {
Q_OBJECT
public:
    ListDescuentoFacturaProvView(QWidget *parent=0, const char *name=0);
    ~ListDescuentoFacturaProvView();
    virtual void pintaListDescuentoFacturaProv();
    virtual void pintadescListDescuentoFacturaProv(int);
    DescuentoFacturaProv *lineaat(int);
    DescuentoFacturaProv *lineaact();
public slots:
    virtual void valueBudgetLineChanged(int row, int col);
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void borradescfacturaact();
};

#endif
