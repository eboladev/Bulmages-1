//
// C++ Interface: ListIvaView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTIVAVIEW_H
#define LISTIVAVIEW_H

/**
@author Tomeu Borras
*/

#include <Q3Table>
//Added by qt3to4:
#include <QEvent>

#include "listiva.h"
#include "iva.h"


class empresa;

class ListIvaView : public Q3Table , public ListIva {
    Q_OBJECT
public:
    ListIvaView(QWidget *parent=0, const char *name=0);
    ~ListIvaView();
    virtual void pintaListIva();
    virtual void pintaIva(int);
    virtual bool eventFilter( QObject *obj, QEvent *ev );
    Iva *lineaat(int);
    Iva *lineaact();
    void guardaconfig();
    void cargaconfig();
public slots:
    virtual void valueBudgetLineChanged(int row, int col);
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void borraIvaAct();
};

#endif
