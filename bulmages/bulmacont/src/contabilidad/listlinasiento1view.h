//
// C++ Interface: ListLinAsiento1View
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLINASIENTO1VIEW_H
#define LISTLINASIENTO1VIEW_H

/**
@author Tomeu Borras
*/

#include <q3table.h>
//Added by qt3to4:
#include <QEvent>

#include "listlinasiento1.h"
#include "linasiento1.h"

#include "selectccosteview.h"
#include "selectcanalview.h"


class ListLinAsiento1View : public Q3Table , public ListLinAsiento1 {
    Q_OBJECT
public:
    ListLinAsiento1View(QWidget *parent=0, const char *name=0);
    ~ListLinAsiento1View();
    virtual void pintaListLinAsiento1();
    virtual void pintalinListLinAsiento1(int);
    virtual bool eventFilter( QObject *obj, QEvent *ev );
    LinAsiento1 *lineaat(int);
    LinAsiento1 *lineaact();
    void cargaconfig();
    void guardaconfig();
    void iniciar_asiento_nuevo(QString);
    void boton_iva();

public slots:
    virtual void valueBudgetLineChanged(int row, int col);
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void contextMenuCerrado ( int , int , const QPoint &  );
    virtual void borraLinAsiento1act();

    virtual void boton_extracto1(int);
    virtual void boton_diario1(int);
    virtual void boton_balance1(int);
};

#endif
