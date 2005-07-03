//
// C++ Interface: ListLinPedidoProveedorView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLINPEDIDOPROVEEDORVIEW_H
#define LISTLINPEDIDOPROVEEDORVIEW_H

/** @author Tomeu Borras */

#include <qtable.h>

#include "listlinpedidoproveedor.h"
#include "company.h"
#include "linpedidoproveedor.h"




class ListLinPedidoProveedorView : public QTable , public ListLinPedidoProveedor {
Q_OBJECT
public:
    ListLinPedidoProveedorView(QWidget *parent=0, const char *name=0);
    ~ListLinPedidoProveedorView();
    virtual void pintaListLinPedidoProveedor();
    virtual void pintalinListLinPedidoProveedor(int);
    virtual bool eventFilter( QObject *obj, QEvent *ev );
    LinPedidoProveedor *lineaat(int);
    LinPedidoProveedor *lineaact();
public slots:
    virtual void valueBudgetLineChanged(int row, int col);
    virtual QString searchArticle();
    virtual void manageArticle(int row);
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void borraLinPedidoProveedoract();
};

#endif
