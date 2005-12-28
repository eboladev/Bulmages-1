//
// C++ Interface: ListDescuentoAlbaranProvView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTDESCALBARANPROVVIEW_H
#define LISTDESCALBARANPROVVIEW_H

/**
@author Tomeu Borras
*/

#include <q3table.h>

#include "listdescalbaranprov.h"
#include "company.h"
#include "descalbaranprov.h"




class ListDescuentoAlbaranProvView : public Q3Table , public ListDescuentoAlbaranProv {
Q_OBJECT
public:
    ListDescuentoAlbaranProvView(QWidget *parent=0, const char *name=0);
    ~ListDescuentoAlbaranProvView();
    virtual void pintaListDescuentoAlbaranProv();
    virtual void pintadescListDescuentoAlbaranProv(int);
    DescuentoAlbaranProv *lineaat(int);
    DescuentoAlbaranProv *lineaact();
public slots:
    virtual void valueBudgetLineChanged(int row, int col);
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void borradescalbaranact();
};

#endif
