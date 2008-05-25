/***************************************************************************
 *   Copyright (C) 2004 by J. M. Torres Rigo                               *
 *   joanmi@bulma.net                                                      *
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

#include <QLineEdit>
#include <QMessageBox>
#include <QCloseEvent>

#include "company.h"
#include "funcaux.h"
#include "plugins.h"
#include "provedit.h"


///
/**
\param comp
\param parent
**/
ProveedorView::ProveedorView ( Company *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "ProveedorView::ProveedorView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setTitleName ( tr ( "Proveedor" ) );
        setDBTableName ( "proveedor" );
        setDBCampoId ( "idproveedor" );
        addDBCampo ( "idproveedor", DBCampo::DBint, DBCampo::DBPrimaryKey, tr ( "ID proveedor" ) );
        addDBCampo ( "nomproveedor", DBCampo::DBvarchar, DBCampo::DBNotNull, tr ( "Nombre del proveedor" ) );
        addDBCampo ( "nomaltproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Nombre alternativo del proveedor" ) );
        addDBCampo ( "cifproveedor", DBCampo::DBvarchar, DBCampo::DBNotNull, tr ( "C.I.F. del proveedor" ) );
        addDBCampo ( "codicliproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Codigo cliente" ) );
        addDBCampo ( "cbancproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Banco proveedor" ) );
        addDBCampo ( "dirproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Direccion" ) );
        addDBCampo ( "poblproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Poblacion" ) );
        addDBCampo ( "idprovincia", DBCampo::DBint, DBCampo::DBNothing, tr ( "Provincia" ) );
        addDBCampo ( "cpproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Codigo postal" ) );
        addDBCampo ( "telproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Numero de telefono" ) );
        addDBCampo ( "faxproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Numero de fax" ) );
        addDBCampo ( "emailproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Direccion electronica" ) );
        addDBCampo ( "urlproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "URL" ) );
        addDBCampo ( "comentproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Comentarios" ) );
        addDBCampo ( "codproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Codigo" ) );
        addDBCampo ( "regimenfiscalproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Proveedor", "Regimen Fiscal" ) );
        addDBCampo ( "idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate ( "Proveedor", "Forma_Pago" ) );
        addDBCampo ( "recargoeqproveedor", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate ( "Proveedor", "Recargo de Equivalencia" ) );
        addDBCampo ( "irpfproveedor", DBCampo::DBnumeric, DBCampo::DBNothing, QApplication::translate ( "Proveedor", "IRPF" ) );

        setupUi ( this );

        /// Deshabilitamos los tabs que aun no se usan.
        masdf->setTabEnabled ( 5, FALSE );
        masdf->setTabEnabled ( 6, FALSE );
        masdf->setTabEnabled ( 7, FALSE );

        /// Cargamos los documentos relacionados con el proveedor y dejamos presentable.
        m_listpedidosprov->setEmpresaBase ( empresaBase() );
        m_listpedidosprov->hideBusqueda();

        m_albaranesprov->setEmpresaBase ( empresaBase() );
        m_albaranesprov->hideBusqueda();

        m_listfacturasprov->setEmpresaBase ( empresaBase() );
        m_listfacturasprov->hideBusqueda();

        m_listpagosprov->setEmpresaBase ( empresaBase() );
        m_listpagosprov->hideBusqueda();

        /// Cargamos algunos valores por defecto.
        mui_idforma_pago->setEmpresaBase ( empresaBase() );
        mui_idforma_pago->setidforma_pago ( "0" );

        mui_idprovincia->setEmpresaBase ( empresaBase() );
        mui_idprovincia->setIdProvincia ( "" );


        meteWindow ( windowTitle(), this, FALSE );
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        mensajeInfo ( tr ( "Error al crear el proveedor" ) );
    } // end try

    _depura ( "END ProveedorView::ProveedorView", 0 );
}

///
/**
**/
ProveedorView::~ProveedorView()
{
    _depura ( "ProveedorView::~ProveedorView", 0 );
    empresaBase() ->refreshProveedores();
    _depura ( "END ProveedorView::~ProveedorView", 0 );
}



/// Esta funcion carga un proveedor de la base de datos y lo presenta.
/// Si el parametro pasado no es un identificador valido entonces se pone
/// la ventana de edicion en modo de insercion.
/**
\param idprov
\return
**/
int ProveedorView::cargarPost ( QString idprov )
{
    _depura ( "ProveedorView::cargar", 0, idprov );

    /// Cargamos las ventanas auxiliares.
    m_listpedidosprov->setidproveedor ( DBvalue ( "idproveedor" ) );
    m_listpedidosprov->presentar();

    m_albaranesprov->setidproveedor ( DBvalue ( "idproveedor" ) );
    m_albaranesprov->presentar();

    m_listfacturasprov->setidproveedor ( DBvalue ( "idproveedor" ) );
    m_listfacturasprov->presentar();

    m_listpagosprov->setidproveedor ( DBvalue ( "idproveedor" ) );
    m_listpagosprov->presentar();

    _depura ( "END ProveedorView::cargar", 0 );
    return 0;
}



