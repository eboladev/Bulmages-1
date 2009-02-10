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
#include "company.h"
#include "blconfiguration.h"
#include "plugins.h"
#include "albaranclienteview.h"
#include "funcaux.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
/**
\param comp
\param parent
**/
ZView::ZView ( Company *comp, QWidget *parent )
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
        addDBCampo ( "idz", DBCampo::DBint, DBCampo::DBPrimaryKey, _( "Id" ) );
        addDBCampo ( "fechaz", DBCampo::DBdate, DBCampo::DBNotNull, _( "Fecha" ) );
        addDBCampo ( "horaz", DBCampo::DBvarchar, DBCampo::DBNotNull, _( "Hora" ) );
        addDBCampo ( "totalz", DBCampo::DBnumeric, DBCampo::DBNotNull, _( "Total" ) );
        addDBCampo ( "numtickets", DBCampo::DBint, DBCampo::DBNotNull, _( "Num. tickets" ) );
        addDBCampo ( "idalmacen", DBCampo::DBint, DBCampo::DBNothing, _( "Id. almacen" ) );
        addDBCampo ( "nomalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, _( "Almacen" ) );

        mui_idalmacen->setEmpresaBase ( comp );


        mui_list->setEmpresaBase ( comp );
        mui_list->setDBTableName ( "albaran" );
        mui_list->setDBCampoId ( "idalbaran" );
        mui_list->addSHeader ( "idalbaran",  DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "Id albaran" ) );
        mui_list->addSHeader ( "numalbaran",  DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "Numero" ) );
        mui_list->addSHeader ( "descalbaran",  DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "Descripcion" ) );
        mui_list->addSHeader ( "refalbaran",  DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "Referencia" ) );
        mui_list->addSHeader ( "fechaalbaran",  DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "Fecha" ) );
        mui_list->addSHeader ( "procesadoalbaran",  DBCampo::DBboolean, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "Procesado" ) );
        mui_list->addSHeader ( "idforma_pago",  DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "Id forma de pago" ) );
        mui_list->addSHeader ( "descforma_pago",  DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNoWrite,  _( "Forma de pago" ) );
        mui_list->addSHeader ( "anuladoalbaran",  DBCampo::DBboolean, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "Anulado" ) );
        mui_list->addSHeader ( "horaalbaran",  DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "Hora" ) );
        mui_list->addSHeader ( "totalalbaran",  DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "Total" ) );
        mui_list->addSHeader ( "bimpalbaran",  DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "Base Imponible" ) );
        mui_list->addSHeader ( "impalbaran",  DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "Impuestos" ) );

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

