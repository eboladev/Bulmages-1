//
// C++ Interface: ListControlStockView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTCONTROLSTOCKVIEW_H
#define LISTCONTROLSTOCKVIEW_H

/**
@author Tomeu Borras
*/

#include <q3table.h>
//Added by qt3to4:
#include <QEvent>

#include "listcontrolstock.h"
#include "company.h"
#include "controlstock.h"




class ListControlStockView : public Q3Table , public ListControlStock {
Q_OBJECT
public:
    ListControlStockView(QWidget *parent=0, const char *name=0);
    ~ListControlStockView();
    virtual void pintaListControlStock();
    virtual void pintalinListControlStock(int);
    virtual bool eventFilter( QObject *obj, QEvent *ev );
    ControlStock *lineaat(int);
    ControlStock *lineaact();
public slots:
    virtual void valueBudgetLineChanged(int row, int col);
    virtual QString searchArticle();
    virtual void manageArticle(int row);
    virtual void manageAlmacen(int row);
    virtual void contextMenu ( int , int , const QPoint &  );
//    virtual void borrar();
};

#endif
