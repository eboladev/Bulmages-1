/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#ifndef CONTRATOVIEW_H
#define CONTRATOVIEW_H

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>


#include "bldatesearch.h"
#include "bfbuscarformapago.h"
#include "bfbuscarseriefactura.h"
#include "bfbuscaralmacen.h"
#include "bldialogchanges.h"
#include "bfform.h"


class BfCompany;

/// Administra el detalle del listado de facturas a clientes.
/** */
class FacturasContratoListSubForm : public BfSubForm
{
    Q_OBJECT
public:
    FacturasContratoListSubForm ( QWidget *parent = 0, const char *name = 0 );
    ~FacturasContratoListSubForm() {}

public slots:
    virtual void load ( QString query ) {
        BlSubForm::load ( query );
    }
};


#include "ui_contratobase.h"
/// Muestra y administra la ventana de una contrato a cliente.
/** */
class ContratoView : public BfForm, public Ui_ContratoBase
{
    Q_OBJECT

public:
    ContratoView ( BfCompany *, QWidget *parent = 0 );
    ~ContratoView();
    void inicializar();
    virtual int afterSave();
    virtual int cargarPost ( QString id );
    virtual void pintarPost ( );

public slots:
    virtual void on_subform2_itemDoubleClicked ( QTableWidgetItem * );
    virtual void on_m_cliente_valueChanged ( QString id );
    virtual void on_mui_facturar_clicked();
};

#endif

