/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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

#ifndef ALBARANPROVEEDORVIEW_H
#define ALBARANPROVEEDORVIEW_H

#include <QLineEdit>
#include <QLabel>

#include "bldatesearch.h"
#include "bfbuscarformapago.h"
#include "bfbuscaralmacen.h"
#include "ui_albaranproveedorbase.h"
#include "bfform.h"
#include "pdefs_pluginbf_proveedoralbaran.h"


class BfCompany;


/** Se encarga de la ventana de albar&aacute;n de proveedor.
    Deriva de Ficha para temas de visualizacion y de AlbaranProveedor para cuestiones
    de manejo de la base de datos.
 */
class PLUGINBF_PROVEEDORALBARAN_EXPORT AlbaranProveedorView : public BfForm, public Ui_AlbaranProveedorBase
{
    Q_OBJECT

public:
    AlbaranProveedorView ( BfCompany *, QWidget * );
    ~AlbaranProveedorView();
    void inicializar();
    void pintatotales ( BlFixed, BlFixed, BlFixed, BlFixed, BlFixed, BlFixed );
    /// Estos metodos deben existir para poder trabajar con la clase Ficha
    virtual int afterSave();
    virtual int cargarPost ( QString id );
    virtual int beforeDelete();

public slots:
    virtual void on_m_descuentos_editFinish ( int, int );
    virtual void on_subform2_editFinish ( int, int );
    virtual void on_mui_verpedidosproveedor_clicked();
    virtual void on_mui_idproveedor_valueChanged ( QString );
    virtual void on_mui_duplicar_released();
};

#endif

