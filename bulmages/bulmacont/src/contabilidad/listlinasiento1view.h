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
#include "empresa.h"
#include "linasiento1.h"




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
public slots:
    virtual void valueBudgetLineChanged(int row, int col);
//    virtual QString searchArticle();
//    virtual void manageArticle(int row);
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void borraLinAsiento1act();
};

#endif
