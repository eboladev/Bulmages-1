/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#ifndef FACTURAPVIEW_H
#define FACTURAPVIEW_H

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>


#include "bfform.h"
#include "bldatesearch.h"
#include "bfbuscarformapago.h"
#include "bfbuscaralmacen.h"
#include "blfunctions.h"
#include "pdefs_pluginbf_proveedorfactura.h"
#include "ui_facturapbase.h"



/** Implementa la pantalla de Facturas de Proveedores.
    Deriva de la clase ficha para heredar metodos comunes a las fichas.
    Tambien deriva de la clase \ref FacturaProveedor para trabajar con la tabla facturap. */
class PLUGINBF_PROVEEDORFACTURA_EXPORT FacturaProveedorView : public BfForm, public Ui_FacturaProveedorBase
{
    Q_OBJECT

public:
    FacturaProveedorView ( BfCompany *, QWidget *parent = 0 );
    virtual ~FacturaProveedorView();
    void inicializar();

    virtual void pintatotales ( BlFixed iva, BlFixed base, BlFixed total, BlFixed desc, BlFixed irpf, BlFixed reqeq );
    /// Estos m&eacute;todos deben existir para poder trabajar con la clase Ficha.
    virtual int afterSave();
    virtual int cargarPost ( QString id );
    virtual int beforeDelete();
    virtual void pintarPost();

public slots:
    virtual void on_m_descuentos_editFinish ( int, int );
    virtual void on_subform2_editFinish ( int, int );
    virtual void on_mui_veralbaranes_clicked();
    virtual void on_mui_idproveedor_valueChanged ( QString );
    virtual void on_mui_duplicar_released();
};

#endif

