//
// C++ Interface: ListLinAlbaranProveedorView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLINALBARANPROVEEDORVIEW_H
#define LISTLINALBARANPROVEEDORVIEW_H

/**
@author Tomeu Borras
*/

#include <Q3Table>
//Added by qt3to4:
#include <QEvent>
#include "linalbaranproveedor.h"
#include "listlinalbaranproveedor.h"
#include "company.h"



class ListLinAlbaranProveedorView : public Q3Table , public ListLinAlbaranProveedor {
Q_OBJECT
public:
    ListLinAlbaranProveedorView(QWidget *parent=0, const char *name=0);
    ~ListLinAlbaranProveedorView();
    virtual void pintaListLinAlbaranProveedor();
    virtual void pintalinListLinAlbaranProveedor(int);
    virtual void pintalinListLinAlbaranProveedor(LinAlbaranProveedor *, int);
    virtual bool eventFilter( QObject *obj, QEvent *ev );
    LinAlbaranProveedor *lineaat(int);
    LinAlbaranProveedor *lineaact();
	void cargaconfig();
	void guardaconfig();

public slots:
    virtual void valueBudgetLineChanged(int row, int col);
    virtual QString searchArticle();
    virtual void manageArticle(int row);
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void borraLinAlbaranProveedoract();
};

#endif
