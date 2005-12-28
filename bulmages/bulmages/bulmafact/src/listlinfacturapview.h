//
// C++ Interface: ListLinFacturaProveedorView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLINFACTURAPVIEW_H
#define LISTLINFACTURAPVIEW_H

/**
@author Tomeu Borras
*/

#include <q3table.h>
//Added by qt3to4:
#include <QEvent>

#include "listlinfacturap.h"
#include "company.h"
#include "linfacturap.h"




class ListLinFacturaProveedorView : public Q3Table , public ListLinFacturaProveedor {
Q_OBJECT
public:
    ListLinFacturaProveedorView(QWidget *parent=0, const char *name=0);
    ~ListLinFacturaProveedorView();
    virtual void pintaListLinFacturaProveedor();
    virtual void pintalinListLinFacturaProveedor(int);
    virtual bool eventFilter( QObject *obj, QEvent *ev );
    LinFacturaProveedor *lineaat(int);
    LinFacturaProveedor *lineaact();
public slots:
    virtual void valueBudgetLineChanged(int row, int col);
    virtual QString searchArticle();
    virtual void manageArticle(int row);
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void borraLinFacturaProveedoract();
};

#endif
