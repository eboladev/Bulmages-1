/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifdef TOMEU


#ifndef LISTLINPREVCOBROVIEW1_H
#define LISTLINPREVCOBROVIEW1_H

#include <QtCore/QEvent>
#include <QtWidgets/QTableWidget>

#include "listlinprevcobro.h"
#include "bccompany.h"
#include "linprevcobro.h"


/// TODO: Esta clase debe derivar de BcSubForm
/** Se encarga de la gestion del listado de lineas de cobro
*/
class ListLinPrevCobroView : public QTableWidget, public ListLinPrevCobro
{
    Q_OBJECT

public:
    ListLinPrevCobroView ( QWidget *parent = 0 );
    ~ListLinPrevCobroView();
    virtual void pintalistlinprevcobro();
    virtual void pintalinlistlinprevcobro ( int );
    linprevcobro *lineaat ( int );
    linprevcobro *lineaact();
    void arreglaPosicion ( int, int );
    void presentacionFactura();
    void presentacionListado();

private:
    virtual void pintalistlinprevcobro ( linprevcobro *, int );

public slots:
    virtual void valueLineChanged ( int row, int col );
    virtual QString searchCuenta();
    virtual void contextMenu ( int, int, const QPoint & );
    virtual void borralinprevcobroact();
    virtual void s_creaPago();
    /// Esto activa a las variables de filtro con lo que los nombres de las funciones no son
    /// muy acertados.
    virtual void s_settipoprevcobro ( QString tip ) {
        mfilt_tipoprevcobro = tip;
    };
    virtual void s_setcodigocuentaprevcobro ( QString tip ) {
        mfilt_codigocuentaprevcobro = tip;
    };
    virtual void s_setfinprevcobro ( QString tip ) {
        mfilt_finprevcobro = tip;
    };
    virtual void s_setffiprevcobro ( QString tip ) {
        mfilt_ffiprevcobro = tip;
    };
    virtual void s_setprocesado ( QString tip ) {
        mfilt_procesado = tip;
    };
    virtual void s_setidregistroiva ( QString tip ) {
        mfilt_idregistroiva = tip;
    };
};

#endif

#endif