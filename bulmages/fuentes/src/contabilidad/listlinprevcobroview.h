//
// C++ Interface: listlinprevcobroview
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLINPREVCOBROVIEW_H
#define LISTLINPREVCOBROVIEW_H

/**
@author Tomeu Borras
*/

#include <qtable.h>
#include "listlinprevcobro.h"
#include "empresa.h"
#include "linprevcobro.h"


class listlinprevcobroview : public QTable , public listlinprevcobro {
Q_OBJECT
public:
    listlinprevcobroview(QWidget *parent=0, const char *name=0);
    ~listlinprevcobroview();
    virtual void pintalistlinprevcobro();
    virtual void pintalinlistlinprevcobro(int);
    virtual bool eventFilter( QObject *obj, QEvent *ev );
    linprevcobro *lineaat(int);
    linprevcobro *lineaact();
public slots:
    virtual void valueLineChanged(int row, int col);
    virtual QString searchCuenta();
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void borralinprevcobroact();
};

#endif
