//
// C++ Interface: ListDescuentoPedidoClienteView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTDESCPEDIDOCLIENTEVIEW_H
#define LISTDESCPEDIDOCLIENTEVIEW_H

/**
@author Tomeu Borras
*/

#include <Q3Table>

#include "listdescpedidocliente.h"
#include "company.h"
#include "descpedidocliente.h"




class ListDescuentoPedidoClienteView : public Q3Table , public ListDescuentoPedidoCliente {
Q_OBJECT
public:
    ListDescuentoPedidoClienteView(QWidget *parent=0, const char *name=0);
    ~ListDescuentoPedidoClienteView();
    virtual void pintaListDescuentoPedidoCliente();
    virtual void pintadescListDescuentoPedidoCliente(int);
    DescuentoPedidoCliente *lineaat(int);
    DescuentoPedidoCliente *lineaact();
public slots:
    virtual void valueBudgetLineChanged(int row, int col);
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void borradescpedidoclienteact();
};

#endif
