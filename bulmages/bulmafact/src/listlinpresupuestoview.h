//
// C++ Interface: listlinpresupuestoview
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLINPRESUPUESTOVIEW_H
#define LISTLINPRESUPUESTOVIEW_H

/**
@author Tomeu Borras
*/

#include <Q3Table>
//Added by qt3to4:
#include <QEvent>

#include "listlinpresupuesto.h"
#include "company.h"
#include "linpresupuesto.h"




class listlinpresupuestoview : public Q3Table , public listlinpresupuesto {
    Q_OBJECT
public:
    listlinpresupuestoview(QWidget *parent=0, const char *name=0);
    ~listlinpresupuestoview();
    virtual void pintalistlinpresupuesto();
    virtual void pintalinlistlinpresupuesto(int);
    virtual bool eventFilter( QObject *obj, QEvent *ev );
    linpresupuesto *lineaat(int);
    linpresupuesto *lineaact();
    void guardaconfig();
    void cargaconfig();
public slots:
    virtual void valueBudgetLineChanged(int row, int col);
    virtual QString searchArticle();
    virtual void manageArticle(int row);
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void borralinpresupuestoact();
};

#endif
