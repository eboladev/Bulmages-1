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

#include <fstream>

#include <QMessageBox>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>

#include "cobroview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "busquedafecha.h"
#include "blfunctions.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
CobroView::CobroView ( BfCompany *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "CobroView::CobroView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );
        centrarEnPantalla ( this );
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        mui_idcliente->setMainCompany ( comp );
        mui_refcobro->setMainCompany ( comp );
        mui_idbanco->setMainCompany ( comp );

			/// Establecemos los parametros de busqueda del Cliente
		mui_idcliente->setLabel ( _( "Cliente:" ) );
		mui_idcliente->setTableName( "cliente" );
		mui_idcliente->m_valores["cifcliente"] = "";
		mui_idcliente->m_valores["nomcliente"] = "";

        setTitleName ( _( "Cobro" ) );
        setDbTableName ( "cobro" );
        setDbFieldId ( "idcobro" );
        addDbField ( "idcobro", BlDbField::DBint, BlDbField::DBPrimaryKey, _( "Id cobro" ) );
        addDbField ( "idcliente", BlDbField::DBint, BlDbField::DBNotNull, _( "Cliente" ) );
        addDbField ( "previsioncobro", BlDbField::DBboolean, BlDbField::DBNothing, _( "Prevision" ) );
        addDbField ( "fechacobro", BlDbField::DBdate, BlDbField::DBNothing, _( "Fecha del cobro" ) );
        addDbField ( "fechavenccobro", BlDbField::DBdate, BlDbField::DBNothing, _( "Fecha de vencimiento" ) );
        addDbField ( "refcobro", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Referencia" ) );
        addDbField ( "cantcobro", BlDbField::DBnumeric, BlDbField::DBNotNull, _( "Total cobro" ) );
        addDbField ( "comentcobro", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Descripcion del cobro" ) );
        addDbField ( "idbanco", BlDbField::DBint, BlDbField::DBNothing, _( "Banco" ) );

        meteWindow ( windowTitle(), this, FALSE );
        pintar();
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        mensajeInfo ( _( "Error al crear el cobro" ), this );
    } // end try
    _depura ( "END CobroView::CobroView", 0 );
}


/** No precisa acciones adicionales en el destructor.
*/
CobroView::~CobroView()
{
    _depura ( "CobroView::~CobroView", 0 );
    _depura ( "END CobroView::~CobroView", 0 );
}

int CobroView::trataTags ( QString &buff ) {
   // pillar los datos de cliente equivale a un <!--QUERY-->      implicito que contiene toda la plantilla.
   QString params[1] = {DBvalue ( "idcliente" )};
   buff = trataCursor(mainCompany() ->cargacursor ( 
          "SELECT * FROM cliente WHERE idcliente = $1" , 
           1, params ,NULL,0,0), buff);

    // mantenemos compatibilidad con RML preexistentes 
    // que usen estos nombres de variable
    buff.replace ( "[referencia]" , xmlEscape(DBvalue ( "refcobro" )) );
    buff.replace ( "[cantidad]" , xmlEscape(DBvalue ( "cantcobro" )) );
    buff.replace ( "[comentario]" , xmlEscape(DBvalue ( "comentcobro" )) );
    buff.replace ( "[fecha]" , xmlEscape(DBvalue ( "fechacobro" )) );

    // admitimos sustituciones , QUERY, IFACE, SETVAR... y los
    // nombres de campos de BD para cobros 
    FichaBf::trataTags(buff);
}

QString CobroView::nombrePlantilla(void) 
{
   return QString("recibo");
}

void CobroView::imprimir()
{
    _depura ( "CobroView::imprimir", 0 );
    /// Comprobamos que se disponen de los datos minimos para imprimir el recibo.
    QString SQLQuery = "";

    if ( DBvalue ( "idcliente" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        mensajeInfo ( _( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
        return;
    } 
    /// Disparamos los plugins
    int res = g_plugins->lanza ( "CoboView_on_mui_imprimir_clicked", this );
    if ( res != 0 ) {
        return;
    } // end if
    FichaBf::imprimir();

    _depura ( "END CobroView::imprimir", 0 );
}


int CobroView::guardarPost() {
	_depura(" CobroView::guardarPost", 0);
	QString query1 = "SELECT COALESCE(sum (cantcobro), 0) AS totalc FROM cobro WHERE refcobro='" + DBvalue("refcobro") + "'";
	BlDbRecordSet *cur1 = mainCompany()->cargacursor(query1);

	QString query = "SELECT COALESCE(sum(totalfactura), 0) AS total FROM factura WHERE reffactura='" + DBvalue("refcobro") + "'";
	BlDbRecordSet *cur = mainCompany()->cargacursor(query);
	if (cur->valor("total") == cur1->valor("totalc") ) {
		mensajeInfo("Procesar todos los documentos con esta referencia ?");
		QString query2 = "UPDATE factura set procesadafactura = TRUE WHERE reffactura='" + DBvalue("refcobro") + "'";
		mainCompany()->ejecuta(query2);
	} // end if
	delete cur;
	delete cur1;
	_depura("END CobroView::guardarPost", 0);

}

