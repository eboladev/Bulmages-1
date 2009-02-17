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

#include "bfcompany.h"
#include "blfunctions.h"
#include "plugins.h"
#include "provedit.h"


///
/**
\param comp
\param parent
**/
ProveedorView::ProveedorView ( BfCompany *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "ProveedorView::ProveedorView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setTitleName ( _( "Proveedor" ) );
        setDBTableName ( "proveedor" );
        setDBCampoId ( "idproveedor" );
        addDBCampo ( "idproveedor", BlDbField::DBint, BlDbField::DBPrimaryKey, _( "ID proveedor" ) );
        addDBCampo ( "nomproveedor", BlDbField::DBvarchar, BlDbField::DBNotNull, _( "Nombre del proveedor" ) );
        addDBCampo ( "nomaltproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Nombre alternativo del proveedor" ) );
        addDBCampo ( "cifproveedor", BlDbField::DBvarchar, BlDbField::DBNotNull, _( "C.I.F. del proveedor" ) );
        addDBCampo ( "codicliproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Codigo cliente" ) );
        addDBCampo ( "cbancproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Banco proveedor" ) );
        addDBCampo ( "dirproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Direccion" ) );
        addDBCampo ( "poblproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Poblacion" ) );
        addDBCampo ( "idprovincia", BlDbField::DBint, BlDbField::DBNothing, _( "Provincia" ) );
        addDBCampo ( "cpproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Codigo postal" ) );
        addDBCampo ( "telproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Numero de telefono" ) );
        addDBCampo ( "faxproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Numero de fax" ) );
        addDBCampo ( "emailproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Direccion electronica" ) );
        addDBCampo ( "urlproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _( "URL" ) );
        addDBCampo ( "comentproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Comentarios" ) );
        addDBCampo ( "codproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Codigo" ) );
        addDBCampo ( "regimenfiscalproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Regimen Fiscal" ) );
        addDBCampo ( "idforma_pago", BlDbField::DBint, BlDbField::DBNothing, _( "Forma_Pago" ) );
        addDBCampo ( "recargoeqproveedor", BlDbField::DBboolean, BlDbField::DBNothing, _( "Recargo de Equivalencia" ) );
        addDBCampo ( "irpfproveedor", BlDbField::DBnumeric, BlDbField::DBNothing, _( "IRPF" ) );

        setupUi ( this );

        /// Deshabilitamos los tabs que aun no se usan.
        mui_tab->setTabEnabled ( 5, FALSE );
        mui_tab->setTabEnabled ( 6, FALSE );
        mui_tab->setTabEnabled ( 7, FALSE );

        /// Cargamos algunos valores por defecto.
        mui_idforma_pago->setMainCompany ( empresaBase() );
        mui_idforma_pago->setId ( "0" );

        mui_idprovincia->setMainCompany ( empresaBase() );
    mui_idprovincia->setQuery("SELECT * FROM provincia LEFT JOIN pais ON provincia.idpais = pais.idpais ORDER BY descpais, provincia");
    mui_idprovincia->setTableName ("provincia");
    mui_idprovincia->setCampoId ("idprovincia");
    mui_idprovincia->m_valores["provincia"] = "";
    mui_idprovincia->m_valores["descpais"] = "";
        mui_idprovincia->setId ( "" );


        meteWindow ( windowTitle(), this, FALSE );
        dialogChanges_cargaInicial();

        /// Disparamos los plugins.
        g_plugins->lanza ( "ProveedorView_ProveedorView_Post", this );
    } catch ( ... ) {
        mensajeInfo ( _( "Error al crear el proveedor" ) );
    } // end try

    _depura ( "END ProveedorView::ProveedorView", 0 );
}

///
/**
**/
ProveedorView::~ProveedorView()
{
    _depura ( "ProveedorView::~ProveedorView", 0 );
    _depura ( "END ProveedorView::~ProveedorView", 0 );
}



/**
*/
/**
**/
void ProveedorView::on_mui_cifproveedor_lostFocus()
{
    _depura ( "ProveedorView::on_mui_cifproveedor_lostFocus", 0 );
	QChar digito;
	if (!validarCIFNIF(mui_cifproveedor->text(), digito)) {
		mensajeInfo("Error en el CIF del proveedor. Digito:" + QString(digito));
	} // end if
    _depura ( "END ProveedorView::on_mui_cifproveedor_lostFocus", 0 );
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
    /// Lanzamos los plugins de carga
    g_plugins->lanza("ProveedorView_cargarPost_Post", this);
    _depura ( "END ProveedorView::cargar", 0 );
    return 0;
}



