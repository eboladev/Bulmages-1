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

#include "cobroview.h"
#include "company.h"
#include "configuracion.h"
#include "busquedacliente.h"
#include "busquedafecha.h"

#include "funcaux.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
CobroView::CobroView ( Company *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "CobroView::CobroView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );
        centrarEnPantalla ( this );
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        mui_idcliente->setEmpresaBase ( comp );
        mui_refcobro->setEmpresaBase ( comp );
        mui_idbanco->setEmpresaBase ( comp );

        setTitleName ( tr ( "Cobro" ) );
        setDBTableName ( "cobro" );
        setDBCampoId ( "idcobro" );
        addDBCampo ( "idcobro", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate ( "Cobro", "ID cobro" ) );
        addDBCampo ( "idcliente", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate ( "Cobro", "Cliente" ) );
        addDBCampo ( "previsioncobro", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate ( "Cobro", "Prevision" ) );
        addDBCampo ( "fechacobro", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate ( "Cobro", "Fecha del cobro" ) );
        addDBCampo ( "fechavenccobro", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate ( "Cobro", "Fecha de vencimiento" ) );
        addDBCampo ( "refcobro", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cobro", "Referencia" ) );
        addDBCampo ( "cantcobro", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate ( "Cobro", "Total cobro" ) );
        addDBCampo ( "comentcobro", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cobro", "Descripcion del cobro" ) );
        addDBCampo ( "idbanco", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate ( "Cobro", "Banco" ) );

        meteWindow ( windowTitle(), this, FALSE );
        pintar();
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        mensajeInfo ( tr ( "Error al crear el cobro" ), this );
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
   // pillar los datos de cliente equivale a un <!--QUERY-->      implícito que contiene toda la plantilla.
   QString params[1] = {DBvalue ( "idcliente" )};
   buff = trataCursor(empresaBase() ->cargacursor ( 
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
        mensajeInfo ( tr ( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
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
	cursor2 *cur1 = empresaBase()->cargacursor(query1);

	QString query = "SELECT COALESCE(sum(totalfactura), 0) AS total FROM factura WHERE reffactura='" + DBvalue("refcobro") + "'";
	cursor2 *cur = empresaBase()->cargacursor(query);
	if (cur->valor("total") == cur1->valor("totalc") ) {
		mensajeInfo("Procesar todos los documentos con esta referencia ?");
		QString query2 = "UPDATE factura set procesadafactura = TRUE WHERE reffactura='" + DBvalue("refcobro") + "'";
		empresaBase()->ejecuta(query2);
	} // end if
	delete cur;
	delete cur1;
	_depura("END CobroView::guardarPost", 0);

}

