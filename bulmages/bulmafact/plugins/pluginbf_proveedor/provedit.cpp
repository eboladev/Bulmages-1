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
#include "blplugins.h"
#include "provedit.h"


///
/**
\param comp
\param parent
**/
ProveedorView::ProveedorView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setTitleName ( _ ( "Proveedor" ) );
        setDbTableName ( "proveedor" );
        setDbFieldId ( "idproveedor" );
        addDbField ( "idproveedor", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID proveedor" ) );
        addDbField ( "nomproveedor", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Nombre del proveedor" ) );
        addDbField ( "nomaltproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre alternativo del proveedor" ) );
        addDbField ( "cifproveedor", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "C.I.F. del proveedor" ) );
        addDbField ( "codicliproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Codigo cliente" ) );
        addDbField ( "cbancproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Banco proveedor" ) );
        addDbField ( "dirproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Direccion" ) );
        addDbField ( "poblproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Poblacion" ) );
        addDbField ( "idprovincia", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Provincia" ) );
        addDbField ( "cpproveedor", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Codigo postal" ) );
        addDbField ( "telproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de telefono" ) );
        addDbField ( "faxproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de fax" ) );
        addDbField ( "emailproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Direccion electronica" ) );
        addDbField ( "urlproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "URL" ) );
        addDbField ( "comentproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentarios" ) );
        addDbField ( "clavewebproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Datos de comercio electronico" ) );
        addDbField ( "codproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Codigo" ) );
        addDbField ( "regimenfiscalproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Regimen Fiscal" ) );
        addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Forma_Pago" ) );
        addDbField ( "recargoeqproveedor", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Recargo de Equivalencia" ) );
        addDbField ( "irpfproveedor", BlDbField::DbNumeric, BlDbField::DbNothing, _ ( "IRPF" ) );

        setupUi ( this );

        /// Deshabilitamos los tabs que aun no se usan.
	int i;
	
	for (i = 0; i < mui_tab->count(); i++) {
	
	    if (mui_tab->widget(i)->objectName() == "tabDivisiones") {
		mui_tab->setTabEnabled(i, FALSE);
	    } else if (mui_tab->widget(i)->objectName() == "tabProductosSuministrados") {
		mui_tab->setTabEnabled(i, FALSE);    
	    } else if (mui_tab->widget(i)->objectName() == "tabContratos") {
		mui_tab->setTabEnabled(i, FALSE);
	    } // end if
	    	    
	} // end for
	

        /// Cargamos algunos valores por defecto.
        mui_idforma_pago->setMainCompany ( mainCompany() );
        mui_idforma_pago->setId ( "0" );

        mui_idprovincia->setMainCompany ( mainCompany() );
        mui_idprovincia->setQuery ( "SELECT * FROM provincia LEFT JOIN pais ON provincia.idpais = pais.idpais ORDER BY descpais, provincia" );
        mui_idprovincia->setTableName ( "provincia" );
        mui_idprovincia->setFieldId ( "idprovincia" );
        mui_idprovincia->m_valores["provincia"] = "";
        mui_idprovincia->m_valores["descpais"] = "";
        mui_idprovincia->setId ( "" );


        insertWindow ( windowTitle(), this, FALSE );
        dialogChanges_readValues();

        /// Disparamos los plugins.
        g_plugins->run ( "ProveedorView_ProveedorView_Post", this );
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _( "Error al crear el proveedor" ) );
    } // end try

    
}

///
/**
**/
ProveedorView::~ProveedorView()
{
    BL_FUNC_DEBUG
    
}


int ProveedorView::beforeSave()
{
	int res1;

        /// Disparamos los plugins.
        res1 = g_plugins->run ( "ProveedorView_Guardar_Pre", this );
	if ( res1 != 0 ) {
	    throw -1;
	} // end if

}


int ProveedorView::afterSave()
{
 
	int res1;

        /// Disparamos los plugins.
        res1 = g_plugins->run ( "ProveedorView_Guardar_Post", this );
	if ( res1 != 0 ) {
	    throw -1;
	} // end if
	
}


/**
*/
/**
**/
void ProveedorView::on_mui_cifproveedor_lostFocus()
{
    BL_FUNC_DEBUG
    QChar digito;
    if ( !blValidateSpainCIFNIFCode ( mui_cifproveedor->text(), digito ) ) {
        blMsgInfo ( _("El CIF del proveedor no parece ser valido. Digito:") + " " + QString ( digito ) );
    } // end if
    
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
    BL_FUNC_DEBUG
    /// Lanzamos los plugins de carga
    g_plugins->run ( "ProveedorView_cargarPost_Post", this );
    
    return 0;
}

/** Pintar un proveedor
*/
/**
\param idbudget
\return
**/
void ProveedorView::pintarPost ( )
{
    BL_FUNC_DEBUG
    /// Escribimos como descripcion el nombre del proveedor para que aparezca en el titulo y en el dockwidget
    setDescripcion( "\n" + dbValue("nomproveedor"));
}
