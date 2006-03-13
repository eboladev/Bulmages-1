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

#include <QEvent>
#include <QTableWidget>

#include "qtable2.h"
#include "listltarifa.h"
#include "ltarifa.h"

class ListLTarifaView : public QTableWidget2 , public ListLTarifa {
    Q_OBJECT
public:
    ListLTarifaView(QWidget *parent=0);
    ~ListLTarifaView();

    virtual void pintar();
    virtual void pintar(int);

    LTarifa *lineaat(int);
    LTarifa *lineaact();

    void guardaconfig();
    void cargaconfig();
    void borraLTarifaAct();

    
        virtual void contextMenuEvent (QContextMenuEvent *e);

public slots:
	virtual void s_cellChanged(int row, int col);

/**
        virtual void borraLTarifaAct();
    */
};

#endif
