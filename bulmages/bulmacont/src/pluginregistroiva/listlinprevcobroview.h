//
// C++ Interface: ListLinPrevCobroView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLINPREVCOBROVIEW1_H
#define LISTLINPREVCOBROVIEW1_H

/**
@author Tomeu Borras
*/

#include <q3table.h>
#include "qtable1.h"
//Added by qt3to4:
#include <QEvent>
#include "listlinprevcobro.h"
#include "empresa.h"
#include "linprevcobro.h"


class ListLinPrevCobroView : public Q3Table , public ListLinPrevCobro {
    Q_OBJECT
public:
    ListLinPrevCobroView(QWidget *parent=0, const char *name=0);
    ~ListLinPrevCobroView();
    virtual void pintalistlinprevcobro();
    virtual void pintalinlistlinprevcobro(int);
    virtual bool eventFilter( QObject *obj, QEvent *ev );
    linprevcobro *lineaat(int);
    linprevcobro *lineaact();
    void arreglaPosicion(int, int);
    void presentacionFactura();
    void presentacionListado();

private:
    virtual void pintalistlinprevcobro(linprevcobro *, int);
public slots:
    virtual void valueLineChanged(int row, int col);
    virtual QString searchCuenta();
    virtual void contextMenu ( int , int , const QPoint &  );
    virtual void borralinprevcobroact();
    virtual void s_creaPago();

    /// Esto activa a las variables de filtro con lo que los nombres de las funciones no son
    /// muy acertados.
    virtual void s_settipoprevcobro(QString tip) {
        mfilt_tipoprevcobro = tip;
    };
    virtual void s_setcodigocuentaprevcobro(QString tip) {
        mfilt_codigocuentaprevcobro = tip;
    };
    virtual void s_setfinprevcobro(QString tip) {
        mfilt_finprevcobro = tip;
    };
    virtual void s_setffiprevcobro(QString tip) {
        mfilt_ffiprevcobro = tip;
    };
    virtual void s_setprocesado(QString tip) {
        mfilt_procesado = tip;
    };
    virtual void s_setidregistroiva(QString tip) {
        mfilt_idregistroiva = tip;
    };
};

#endif
