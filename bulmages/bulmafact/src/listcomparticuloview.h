//
// C++ Interface: ListCompArticuloView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTCOMPARTICULOVIEW_H
#define LISTCOMPARTICULOVIEW_H

/**
@author Tomeu Borras
*/

#include <Q3Table>
//Added by qt3to4:
#include <QEvent>

#include "listcomparticulo.h"
#include "company.h"
#include "comparticulo.h"




class ListCompArticuloView : public Q3Table , public ListCompArticulo {
Q_OBJECT
public:
    ListCompArticuloView(QWidget *parent=0, const char *name=0);
    ~ListCompArticuloView();
    virtual void pintaListCompArticulo();
    virtual void pintalinListCompArticulo(int);
    virtual bool eventFilter( QObject *obj, QEvent *ev );
    CompArticulo *lineaat(int);
    CompArticulo *lineaact();
public slots:
    virtual void valueListCompArticuloViewChanged(int row, int col);
    virtual QString searchArticle();
    virtual void manageArticle(int row);
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void borraCompArticuloact();
};

#endif
