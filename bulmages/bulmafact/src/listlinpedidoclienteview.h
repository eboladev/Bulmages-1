//
// C++ Interface: ListLinPedidoClienteView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLINPEDIDOCLIENTEVIEW_H
#define LISTLINPEDIDOCLIENTEVIEW_H

/** @author Tomeu Borras */

#include <qtable.h>

#include "listlinpedidocliente.h"
#include "company.h"
#include "linpedidocliente.h"




class ListLinPedidoClienteView : public QTable , public ListLinPedidoCliente {
Q_OBJECT
public:
    ListLinPedidoClienteView(QWidget *parent=0, const char *name=0);
    ~ListLinPedidoClienteView();
    virtual void pintaListLinPedidoCliente();
    virtual void pintalinListLinPedidoCliente(int);
    virtual bool eventFilter( QObject *obj, QEvent *ev );
    LinPedidoCliente *lineaat(int);
    LinPedidoCliente *lineaact();
public slots:
    virtual void valueBudgetLineChanged(int row, int col);
    virtual QString searchArticle();
    virtual void manageArticle(int row);
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void borraLinPedidoClienteact();
};

#endif
