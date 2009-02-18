/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <QMessageBox>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>

#include <fstream>

#include "zview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "plugins.h"
#include "albaranclienteview.h"
#include "blfunctions.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
/**
\param comp
\param parent
**/
ZView::ZView ( BfCompany *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "ZView::ZView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        /// Lanzamos los plugins.
        if ( g_plugins->lanza ( "ZView_ZView", this ) ) return;

        setTitleName ( _( "Cuadre de caja" ) );
        setDBTableName ( "z" );
        setDBCampoId ( "idz" );
        addDBCampo ( "idz", BlDbField::DBint, BlDbField::DBPrimaryKey, _( "Id" ) );
        addDBCampo ( "fechaz", BlDbField::DBdate, BlDbField::DBNotNull, _( "Fecha" ) );
        addDBCampo ( "horaz", BlDbField::DBvarchar, BlDbField::DBNotNull, _( "Hora" ) );
        addDBCampo ( "totalz", BlDbField::DBnumeric, BlDbField::DBNotNull, _( "Total" ) );
        addDBCampo ( "numtickets", BlDbField::DBint, BlDbField::DBNotNull, _( "Num. tickets" ) );
        addDBCampo ( "idalmacen", BlDbField::DBint, BlDbField::DBNothing, _( "Id. almacen" ) );
        addDBCampo ( "nomalmacen", BlDbField::DBvarchar, BlDbField::DBNoSave, _( "Almacen" ) );

        mui_idalmacen->setMainCompany ( comp );


        mui_list->setMainCompany ( comp );
        mui_list->setDBTableName ( "albaran" );
        mui_list->setDBCampoId ( "idalbaran" );
        mui_list->addSubFormHeader ( "idalbaran",  BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Id albaran" ) );
        mui_list->addSubFormHeader ( "numalbaran",  BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Numero" ) );
        mui_list->addSubFormHeader ( "descalbaran",  BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Descripcion" ) );
        mui_list->addSubFormHeader ( "refalbaran",  BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Referencia" ) );
        mui_list->addSubFormHeader ( "fechaalbaran",  BlDbField::DBdate, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Fecha" ) );
        mui_list->addSubFormHeader ( "procesadoalbaran",  BlDbField::DBboolean, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Procesado" ) );
        mui_list->addSubFormHeader ( "idforma_pago",  BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Id forma de pago" ) );
        mui_list->addSubFormHeader ( "descforma_pago",  BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite,  _( "Forma de pago" ) );
        mui_list->addSubFormHeader ( "anuladoalbaran",  BlDbField::DBboolean, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Anulado" ) );
        mui_list->addSubFormHeader ( "horaalbaran",  BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Hora" ) );
        mui_list->addSubFormHeader ( "totalalbaran",  BlDbField::DBnumeric, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Total" ) );
        mui_list->addSubFormHeader ( "bimpalbaran",  BlDbField::DBnumeric, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Base Imponible" ) );
        mui_list->addSubFormHeader ( "impalbaran",  BlDbField::DBnumeric, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Impuestos" ) );

        mui_list->setinsercion ( FALSE );
        mui_list->setDelete ( FALSE );
        mui_list->setSortingEnabled ( TRUE );

        dialogChanges_cargaInicial();
        meteWindow ( windowTitle(), this, FALSE );
    } catch ( ... ) {
        mensajeInfo ( _( "Error al crear el almacen" ) );
    } // end try
    _depura ( "END ZView::ZView", 0 );
}


/// No precisa acciones adicionales en el destructor.

/**
**/
ZView::~ZView()
{
    _depura ( "ZView::~ZView", 0 );
    _depura ( "END ZView::~ZView", 0 );
}


///
/**
\param idz
\return
**/
int ZView::cargarPost ( QString idz )
{
    mui_list->cargar ( "SELECT * FROM albaran NATURAL LEFT JOIN forma_pago WHERE idz=" + idz );
    return 0;
}




/**  Este metodo se activa cuando bien pulsando sobre el boton de editar
     o bien haciendo doble click en el modo de edicion se desea invocar la accion
     Editar el elemento si estamos en modo editmode o cerrar la ventana y emitir
     un signal selected() si estamos en el modo selector.

     Primero determina el idalbaran seleccionado, luego crea la instancia de
     la ventana de edicion AlbaranClienteView y lo mete en el workspace.
     Por ultimo hace que dicha ventana carge de la base de datos el idalbaran
     seleccionado.
*/
/**
\param row
\return
**/
void ZView::on_mui_list_cellDoubleClicked ( int row, int )
{
    _depura ( "ZView::on_mui_list_doubleClicked", 0 );

    QString idalbaran = mui_list->DBvalue ( QString ( "idalbaran" ), row );
	if (g_plugins->lanza("SNewAlbaranClienteView", empresaBase()) ) {
	
        AlbaranClienteView * prov = (AlbaranClienteView *) g_plugParams;
        if ( prov->cargar ( idalbaran ) ) {
            delete prov;
            return;
        } // end if
        empresaBase() ->m_pWorkspace->addWindow ( prov );
        prov->show();
	} // end if
    _depura ( "END ZView::on_mui_list_doubleClicked", 0 );
}


void ZView::on_mui_actualizar_released() {
	cargar(DBvalue("idz"));
} 

