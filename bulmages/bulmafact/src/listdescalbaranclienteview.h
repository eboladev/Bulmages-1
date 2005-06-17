//
// C++ Interface: ListDescuentoAlbaranClienteView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTDESCALBARANCLIENTEVIEW_H
#define LISTDESCALBARANCLIENTEVIEW_H

/**
@author Tomeu Borras
*/

#include <qtable.h>

#include "listdescalbarancliente.h"
#include "company.h"
#include "descalbarancliente.h"




class ListDescuentoAlbaranClienteView : public QTable , public ListDescuentoAlbaranCliente {
Q_OBJECT
public:
    ListDescuentoAlbaranClienteView(QWidget *parent=0, const char *name=0);
    ~ListDescuentoAlbaranClienteView();
    virtual void pintaListDescuentoAlbaranCliente();
    virtual void pintadescListDescuentoAlbaranCliente(int);
    DescuentoAlbaranCliente *lineaat(int);
    DescuentoAlbaranCliente *lineaact();
public slots:
    virtual void valueBudgetLineChanged(int row, int col);
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void borradescalbaranact();
};

#endif
