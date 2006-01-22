//
// C++ Interface: ListLinFacturaView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLINFACTURAVIEW_H
#define LISTLINFACTURAVIEW_H

/**
@author Tomeu Borras
*/

#include <q3table.h>
//Added by qt3to4:
#include <QEvent>

#include "listlinfactura.h"
#include "company.h"
#include "linfactura.h"




class ListLinFacturaView : public Q3Table , public ListLinFactura {
Q_OBJECT
public:
    ListLinFacturaView(QWidget *parent=0, const char *name=0);
    ~ListLinFacturaView();
    virtual void pintaListLinFactura();
    virtual void pintalinListLinFactura(int);
    virtual bool eventFilter( QObject *obj, QEvent *ev );
    LinFactura *lineaat(int);
    LinFactura *lineaact();
	void cargaconfig();
	void guardaconfig();
public slots:
    virtual void valueBudgetLineChanged(int row, int col);
    virtual QString searchArticle();
    virtual void manageArticle(int row);
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void borraLinFacturaact();
};

#endif
