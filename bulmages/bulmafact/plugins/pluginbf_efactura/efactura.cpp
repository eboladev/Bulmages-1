/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
 *            *
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

#include "efactura.h"
#include "blfunctions.h"
#include <QFileDialog>
#include <QLineEdit>
#include <QString>


/// Constructor de la clase EFactura (el form)
/// Pasamos como parametro la empresa para poder usar metodos de BD para
/// guardar los datos de configuracion
/**
\param emp
\param parent
**/
EFactura::EFactura ( BfCompany *emp, QWidget *parent ) : BfForm ( emp, parent )
{

    BL_FUNC_DEBUG

    QString query;

    setupUi ( this );  // Para que el form se inicialice bien

//  query = "SELECT valor FROM configuracion WHERE nombre = 'eFactura_server'";
//  BlDbRecordSet *cur1 = mainCompany()->loadQuery(query);
//  mui_URLServidorTiempo->setText(cur1->value("valor"));
//  delete cur1;
//
//  query = "SELECT valor FROM configuracion WHERE nombre = 'eFactura_certificado'";
//  BlDbRecordSet *cur2 = mainCompany()->loadQuery(query);
//  mui_ficheroECertificado->setText(cur2->value("valor"));
//  delete cur2;
//
//  query = "SELECT valor FROM configuracion WHERE nombre = 'eFactura_server_valida'";
//  BlDbRecordSet *cur3 = mainCompany()->loadQuery(query);
//  mui_URLServidorValidaCert->setText(cur3->value("valor"));
//  delete cur3;

    blScript(this);
    
}


/// Destructor
/**
**/
EFactura::~EFactura()
{
    BL_FUNC_DEBUG
    
}


/// Funcionalidad del boton guardar
/**
**/
void EFactura::on_mui_guardar_clicked()
{

    BL_FUNC_DEBUG

    QString query;

    if ( mui_URLServidorTiempo->isModified() ) {
        query = "DELETE FROM configuracion WHERE nombre = 'eFactura_server'";
        mainCompany() ->runQuery ( query );

        query = "INSERT INTO configuracion (nombre, valor) VALUES ('eFactura_server', '";
        query += mui_URLServidorTiempo->text();
        query += "')";
        mainCompany() ->runQuery ( query );
    }

    /// Como el setText() pone siempre a false el valor de retorno de isModified()
    /// hay que hacer siempre el update del campo que indica la ruta del fichero
    /// de certificado digital

    query = "DELETE FROM configuracion WHERE nombre = 'eFactura_certificado'";
    mainCompany() ->runQuery ( query );

    query = "INSERT INTO configuracion (nombre, valor) VALUES ('eFactura_certificado', '";
    query += mui_ficheroECertificado->text();
    query += "')";
    mainCompany() ->runQuery ( query );

    
}


/// Funcionalidad del boton Examinar en el apartado del certificado
/**
*/
void EFactura::on_mui_examinaECertificado_clicked()
{
    BL_FUNC_DEBUG

    QString s = QFileDialog::getOpenFileName (
                    this,
                    "Escoja un fichero por favor",
                    "/home",
                    "Todos los archivos (*)"
                );

    /// Si se le da a cancelar, s devuelve NULL y se queda el campo de texto vacio

    if ( s != "" )
        mui_ficheroECertificado->setText ( s );

    
}


/// Funcionalidad del boton cancelar
/**
**/
void EFactura::on_mui_cancelar_clicked()
{
    BL_FUNC_DEBUG

//  QString query = "SELECT * FROM configuracion";
//  BlDbRecordSet *cur = mainCompany()->loadQuery(query);
//
//  while(!cur->eof()) {
//   cur->nextRecord();
//  } // end while
//
//  delete cur;
//
//  query = "UPDATE configuracion SET nombre='nombre' WHRE nombre='elnombre'";
//  mainCompany()->begin();
//  mainCompany()->runQuery(query);
//  mainCompany()->rollback();
    
}
