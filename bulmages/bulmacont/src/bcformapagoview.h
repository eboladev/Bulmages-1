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

#ifndef BCFORMAPAGOVIEW_H
#define BCFORMAPAGOVIEW_H

#include "blfunctions.h"
#include "ui_bcformapagobase.h"
#include "blpostgresqlclient.h"
#include "bldialogchanges.h"
#include "bcform.h"


class BcCompany;


/// La clase BcFormaPagoView se encarga de la gesti&oacute;n de formas de pago.
/** Las formas de pago son unas plantillas que se usan para determinar las formas en
    que se pagan o se cobran las facturas emitidas o recibidas. */
class BC_EXPORT BcFormaPagoView : public BcForm, public Ui_BcFormaPagoBase
{
    Q_OBJECT

private:
    BlDbRecordSet *m_curfpago;
    /// Indica la posici&oacute;n en el combobox que est&aacute; seleccionada.
    /// Se usa para guardar en caso de modificaciones.
    int m_posactual;

private:
    void pintar ( QString idtipoiva = "" );
    void mostrarPlantilla ( int pos = 0 );
    void cambiacombo ( int );

public:
    BcFormaPagoView ( BcCompany *, QWidget *parent = 0 );
    ~BcFormaPagoView();
    virtual int save();
    virtual void on_mui_borrar_clicked();

private slots:
    virtual void on_mui_crear_clicked();
    virtual void on_mui_comboFPago_currentIndexChanged ( int i );
};

#endif
