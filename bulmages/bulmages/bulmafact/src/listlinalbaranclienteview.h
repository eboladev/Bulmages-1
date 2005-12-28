//
// C++ Interface: ListLinAlbaranClienteView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLINALBARANCLIENTEVIEW_H
#define LISTLINALBARANCLIENTEVIEW_H

/**
@author Tomeu Borras
*/

#include <q3table.h>
//Added by qt3to4:
#include <QEvent>
#include "linalbarancliente.h"
#include "listlinalbarancliente.h"
#include "company.h"



class ListLinAlbaranClienteView : public Q3Table , public ListLinAlbaranCliente {
Q_OBJECT
public:
    ListLinAlbaranClienteView(QWidget *parent=0, const char *name=0);
    ~ListLinAlbaranClienteView();
    virtual void pintaListLinAlbaranCliente();
    virtual void pintalinListLinAlbaranCliente(int);
    virtual bool eventFilter( QObject *obj, QEvent *ev );
    LinAlbaranCliente *lineaat(int);
    LinAlbaranCliente *lineaact();
public slots:
    virtual void valueBudgetLineChanged(int row, int col);
    virtual QString searchArticle();
    virtual void manageArticle(int row);
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void borraLinAlbaranClienteact();
};

#endif
