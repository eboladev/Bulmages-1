/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org Asociación Iglues -- Contabilidad Linux         *
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

#ifndef FPAGOVIEW_H
#define FPAGOVIEW_H

#include "blfunctions.h"
#include "ui_fpagobase.h"
#include "blpostgresqlclient.h"
#include "dialogchanges.h"
#include "fichabc.h"


class Empresa;


/// La clase fpagoview se encarga de la gesti&oacute;n de formas de pago.
/** Las formas de pago son unas plantillas que se usan para determinar las formas en
    que se pagan o se cobran las facturas emitidas o recibidas. */
class fpagoview : public FichaBc, public Ui_FPagoBase
{
    Q_OBJECT

private:
    BlDbRecordSet *m_curfpago;
    /// Indica la posici&oacute;n en el combobox que est&aacute; seleccionada.
    /// Se usa para guardar en caso de modificaciones.
    int m_posactual;

private:
    void pintar ( QString idtipoiva = "" );
    void mostrarplantilla ( int pos = 0 );
    void cambiacombo ( int );

public:
    fpagoview ( Empresa *, QWidget *parent = 0 );
    ~fpagoview();
    virtual int guardar();
    virtual void on_mui_borrar_clicked();

private slots:
    virtual void on_mui_crear_clicked();
    virtual void on_mui_comboFPago_currentIndexChanged ( int i );
};

#endif
