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
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        mui_idcliente->setEmpresaBase ( comp );
        mui_refcobro->setEmpresaBase ( comp );
        mui_idbanco->setEmpresaBase ( comp );

        setTitleName ( tr ( "Cobro" ) );
        setDBTableName ( "cobro" );
        setDBCampoId ( "idcobro" );
        addDBCampo ( "idcobro", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate ( "Cobro", "ID cobro" ) );
        addDBCampo ( "idcliente", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate ( "Cobro", "ID cliente" ) );
        addDBCampo ( "previsioncobro", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate ( "Cobro", "Prevision de cobro" ) );
        addDBCampo ( "fechacobro", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate ( "Cobro", "Fecha de cobro" ) );
        addDBCampo ( "fechavenccobro", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate ( "Cobro", "Fecha de vencimiento" ) );
        addDBCampo ( "refcobro", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cobro", "Referencia del cobro" ) );
        addDBCampo ( "cantcobro", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate ( "Cobro", "Cantidad" ) );
        addDBCampo ( "comentcobro", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cobro", "Comentarios" ) );
        addDBCampo ( "idbanco", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate ( "Banco", "Comentarios" ) );

        meteWindow ( windowTitle(), this, FALSE );
        pintar();
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        mensajeInfo ( tr ( "Error al crear el cobro" ) );
    } // end try
    _depura ( "END CobroView::CobroView", 0 );
}


/** No precisa acciones adicionales en el destructor.
*/
CobroView::~CobroView()
{
    _depura ( "CobroView::~CobroView", 0 );
    /// ATENCION: Hacer esto es un error ya que puede machacar procesos dependientes del listado.
    // ((Company *)empresaBase())->refreshCobrosCliente();
    _depura ( "END CobroView::~CobroView", 0 );
}


void CobroView::imprimir()
{
    _depura ( "CobroView::imprimir", 0 );
    /// Comprobamos que se disponen de los datos minimos para imprimir el recibo.
    QString SQLQuery = "";

    if ( DBvalue ( "idcliente" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        mensajeInfo ( tr ( "Tiene que guardar el documento antes de poder imprimirlo." ) );
        return;
    } else {
        SQLQuery = "SELECT * FROM cliente WHERE idcliente = " + DBvalue ( "idcliente" );
    } // end if

    /// Disparamos los plugins
    int res = g_plugins->lanza ( "CoboView_on_mui_imprimir_clicked", this );
    if ( res != 0 ) {
        return;
    } // end if
    base basesimp;
    base basesimpreqeq;
    QString archivo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "recibo.rml";
    QString archivod = confpr->valor ( CONF_DIR_USER ) + "recibo.rml";
    QString archivologo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system ( archivo.toAscii().constData() );
    /// Copiamos el logo
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#endif

    system ( archivologo.toAscii().constData() );
    QFile file;
    file.setFileName ( archivod );
    file.open ( QIODevice::ReadOnly );
    QTextStream stream ( &file );
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt = "";

    /// Linea de totales del Presupuesto.
    cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );
    if ( !cur->eof() ) {
        buff.replace ( "[dircliente]", cur->valor ( "dircliente" ) );
        buff.replace ( "[poblcliente]", cur->valor ( "poblcliente" ) );
        buff.replace ( "[telcliente]", cur->valor ( "telcliente" ) );
        buff.replace ( "[nomcliente]", cur->valor ( "nomcliente" ) );
        buff.replace ( "[cifcliente]", cur->valor ( "cifcliente" ) );
        buff.replace ( "[idcliente]", cur->valor ( "idcliente" ) );
        buff.replace ( "[cpcliente]", cur->valor ( "cpcliente" ) );
        buff.replace ( "[codcliente]", cur->valor ( "codcliente" ) );
    } // end if
    delete cur;

    buff.replace ( "[referencia]" , DBvalue ( "refcobro" ) );
    buff.replace ( "[cantidad]" , DBvalue ( "cantcobro" ) );
    buff.replace ( "[comentario]" , DBvalue ( "comentcobro" ) );
    buff.replace ( "[fecha]" , DBvalue ( "fechacobro" ) );
    buff.replace ( "[story]", fitxersortidatxt );

    Fixed basei ( "0.00" );

    /// En la version para windows hay problemas con las imagenes,
    /// por eso de momento lo dejamos asi.
#ifndef WINDOWS
    //   buff.replace("[detallearticulos]", detalleArticulos());
#endif

    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << buff;
        file.close();
    } // end if

    _depura ( "FichaBf::imprimir", 0 );
    invocaPDF ( "recibo" );

    _depura ( "END CobroView::imprimir", 0 );
}


