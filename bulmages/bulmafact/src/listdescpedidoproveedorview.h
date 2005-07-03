//
// C++ Interface: ListDescuentoPedidoProveedorView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTDESCPEDIDOPROVEEDORVIEW_H
#define LISTDESCPEDIDOPROVEEDORVIEW_H

/**
@author Tomeu Borras
*/

#include <qtable.h>

#include "listdescpedidoproveedor.h"
#include "company.h"
#include "descpedidoproveedor.h"




class ListDescuentoPedidoProveedorView : public QTable , public ListDescuentoPedidoProveedor {
Q_OBJECT
public:
    ListDescuentoPedidoProveedorView(QWidget *parent=0, const char *name=0);
    ~ListDescuentoPedidoProveedorView();
    virtual void pintaListDescuentoPedidoProveedor();
    virtual void pintadescListDescuentoPedidoProveedor(int);
    DescuentoPedidoProveedor *lineaat(int);
    DescuentoPedidoProveedor *lineaact();
public slots:
    virtual void valueBudgetLineChanged(int row, int col);
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void borradescpedidoproveedoract();
};

#endif
