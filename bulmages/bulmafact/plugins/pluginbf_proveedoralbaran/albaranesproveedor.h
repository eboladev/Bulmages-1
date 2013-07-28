/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#ifndef ALBARANESPROVEEDOR_H
#define ALBARANESPROVEEDOR_H

#include <QtWidgets/QLineEdit>

#include "bfcompany.h"
#include "bfbuscararticulo.h"
#include "blfunctions.h"
#include "bfsubform.h"
#include "blformlist.h"
#include "pdefs_pluginbf_proveedoralbaran.h"


/// Clase que maneja el subformulario de los albaranes de proveedor.
/** Deriva de la clase BfSubForm.
    Reimplementa los metodos de carga para que trabajan con la tabla albaranp.
*/
class PLUGINBF_PROVEEDORALBARAN_EXPORT AlbaranesProveedorListSubform : public BfSubForm
{
    Q_OBJECT

public:
    AlbaranesProveedorListSubform ( QWidget *parent = 0 );
    ~AlbaranesProveedorListSubform();

public slots:
    virtual void load();
    virtual void load ( QString query );
};


#include "ui_albaranesproveedorlistbase.h"


/// Clase que controla la pantalla de listado de albaranes a proveedor.
class PLUGINBF_PROVEEDORALBARAN_EXPORT AlbaranesProveedor : public BlFormList, public Ui_AlbaranesProveedorListBase
{
    Q_OBJECT

private:
    /// En el caso de estar en modo seleccion esta variable de clase indicara el identificador seleccionado.
    QString mdb_idalbaranp;

public:
    AlbaranesProveedor ( QWidget *parent = 0, Qt::WindowFlags flag = 0, edmode editmodo = BL_EDIT_MODE );
    AlbaranesProveedor ( BfCompany *comp = NULL, QWidget *parent = 0, Qt::WindowFlags flag = 0, edmode editmodo = BL_EDIT_MODE );
    ~AlbaranesProveedor();
    void presentar();
    void setMainCompany ( BfCompany *comp );
    QString idalbaranp();
    void imprimir();
    void setidproveedor ( QString val );
    void setidarticulo ( QString val );
    const QString generaFiltro();
    void editar ( int );
    void remove();
    void crear();
    void iniciaForm();
};

#endif

