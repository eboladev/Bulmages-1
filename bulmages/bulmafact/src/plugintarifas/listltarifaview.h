//
// C++ Interface: ListLTarifaView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLTARIFAVIEW_H
#define LISTLTARIFAVIEW_H

/**
@author Tomeu Borras
*/

#include <Q3Table>
//Added by qt3to4:
#include <QEvent>

#include "listltarifa.h"
#include "ltarifa.h"

class ListLTarifaView : public Q3Table , public ListLTarifa {
Q_OBJECT
public:
    ListLTarifaView(QWidget *parent=0, const char *name=0);
    ~ListLTarifaView();
    virtual void pintar();
    virtual void pintar(int);
    virtual bool eventFilter( QObject *obj, QEvent *ev );
    LTarifa *lineaat(int);
    LTarifa *lineaact();
    void guardaconfig();
    void cargaconfig();
public slots:
    virtual void valueBudgetLineChanged(int row, int col);
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void borraLTarifaAct();
};

#endif
