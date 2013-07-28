/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
 *   Contribution by Tomeu Borras tborras@conetxia.com                     *
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

#include "QString"

#include "efacturarecepcion.h"
#include "blfunctions.h"
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLineEdit>

#define EMAIL "efactura@conetxia.com"
#define DIREMAIL "/home/arturo/efacturamail"


///
/**
\param emp
\param parent
**/
EFacturaRecepcion::EFacturaRecepcion ( BfCompany *emp, QWidget *parent ) : BfForm ( emp, parent )
{
    BL_FUNC_DEBUG
    QString query;

    setupUi ( this );  // Para que el form se inicialice bien
    blScript(this);
    
}


///
/**
**/
EFacturaRecepcion::~EFacturaRecepcion()
{
    BL_FUNC_DEBUG
    
}


///
/**
\return
**/
void EFacturaRecepcion::on_mui_recibir_clicked()
{
    BL_FUNC_DEBUG

    /// Obtenemos los valores de configuracion necesarios para obtener eFacturas por e-mail

    QString query = "SELECT valor FROM configuracion WHERE nombre = 'eFactura_dirEmail'";
    BlDbRecordSet *dirEmail = mainCompany() ->loadQuery ( query );

    query = "SELECT valor FROM configuracion WHERE nombre = 'eFactura_serverRec'";
    BlDbRecordSet *serverRec = mainCompany() ->loadQuery ( query );

    query = "SELECT valor FROM configuracion WHERE nombre = 'eFactura_Email'";
    BlDbRecordSet *Email = mainCompany() ->loadQuery ( query );

    query = "SELECT valor FROM configuracion WHERE nombre = 'eFactura_Password'";
    BlDbRecordSet *Password = mainCompany() ->loadQuery ( query );

    /// Comprobamos que el directorio especificado existeAboutView

    QDir *direfactura = new QDir ( dirEmail->value( "valor" ) );

    if ( !direfactura->exists() ) {
	
	blMsgError(_("El directorio especificado en la configuracion para la eFactura no existe. Por favor, creelo e intentelo de nuevo"));
        return;
    }

    /// Llamamos al script para recibir e-mails con los parametros obtenidos

    QString llamada_qstring = "cd " + dirEmail->value( "valor" ) + " ; python /home/arturo/bulmages/trunk/bulmages/installbulmages/efactura/recibemail.py " + serverRec->value( "valor" ) + " " + Email->value( "valor" ) + " " + Password->value( "valor" );

    char llamada[512];

    strncpy ( llamada, llamada_qstring.toLatin1(), sizeof ( llamada ) - 1 );

    system ( llamada );

    /// Obtenemos los directorios alojados en nuestra carpeta de eFacturas recibidas

    // Filtramos solo por directorios que no sean enlaces
    direfactura->setFilter ( QDir::Dirs );
    // Ordenamos por fecha de modificacion
    direfactura->setSorting ( QDir::Name );

    QFileInfoList *listadirs = new QFileInfoList ( direfactura->entryInfoList() );

    mui_facturasRecibidas->setColumnCount ( 2 );

    int cont = 0;

    mui_facturasRecibidas->setRowCount ( 0 );

    QFileInfo *fileInfo = NULL;
    QTableWidgetItem *item = NULL;
    QFileInfoList *archivos = NULL;

    for ( int i = 0; i < listadirs->size(); i++ ) {

        fileInfo = new QFileInfo ( listadirs->at ( i ) );

        /// Evitamos los directorios "." y ".."

        if ( !fileInfo->fileName().startsWith ( "." ) ) {

            /// Entramos en el directorio

            if ( !direfactura->cd ( fileInfo->fileName() ) ) {
		BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("Error intentando entrar en '%1'.")).arg(fileInfo->fileName()) );
		blMsgError(QString(_("Error intentando entrar en '%1'")).arg(fileInfo->fileName()));
                continue;
            }

            /// Buscamos solo archivos

            direfactura->setFilter ( QDir::Files );

            /// Obtenemos la lista de archivos (en teoria solo debe de haber uno, el XML con la efactura)

            archivos = new QFileInfoList ( direfactura->entryInfoList() );

            if ( archivos->size() > 1 ) {
		BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("El directorio '%1' contiene mas de un archivo. Por favor, arreglelo dejando solo el fichero XML que contenga la e-factura.")).arg(direfactura->absolutePath()) );
		blMsgError(QString(_("El directorio '%1' contiene mas de un archivo. Por favor, arreglelo dejando solo el fichero XML que contenga la e-factura.")).arg(direfactura->absolutePath()));
                direfactura->cdUp();
                continue;
            }

            /// Obtenemos info del archivo

            fileInfo = new QFileInfo ( archivos->at ( 0 ) );

            /// Creamos una nueva fila y la usamos

            mui_facturasRecibidas->insertRow ( cont );

            /// Nombre del fichero

            item = new QTableWidgetItem ( fileInfo->fileName() );
            mui_facturasRecibidas->setItem ( cont, 0, item );

            /// Nombre del directorio que lo contiene

            item = new QTableWidgetItem ( direfactura->absolutePath() );
            mui_facturasRecibidas->setItem ( cont, 1, item );

            cont++;

            /// Volvemos al directorio de efacturas

            direfactura->cdUp();

        } // end if
    }

    /// Obtenemos el nombre del archivo XML incluido en cada uno de ellos y
    /// los mostramos en pantalla

    /// Liberamos memoria con los datos que ya no necesitamos

//  delete item;
    delete fileInfo;
    delete direfactura;
    delete dirEmail;
    delete serverRec;
    delete Email;
    delete Password;

    
}


///
/**
**/
void EFacturaRecepcion::on_mui_abrir_clicked()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void EFacturaRecepcion::on_mui_salir_clicked()
{
    BL_FUNC_DEBUG
    close();
    
}
