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
#include <QWidget>
#include <QFileDialog>

#include "blmaincompany.h"
#include "importcsv.h"
#include "blconfiguration.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
ImportCSV::ImportCSV ( BlMainCompany *comp, QWidget *parent )
        : BlWidget ( comp, parent )
{
    _depura ( "ImportCSV::ImportCSV", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );
        rellenarTablas();

        mainCompany()->meteWindow ( windowTitle(), this );
    } catch ( ... ) {
        mensajeInfo ( _( "Error al crear la ventana de importacion" ) );
    } // end try
    _depura ( "END ImportCSV::ImportCSV", 0 );
}

/** No precisa acciones adicionales en el destructor.
*/
ImportCSV::~ImportCSV()
{
    _depura ( "ImportCSV::~ImportCSV", 0 );
    /// ATENCION: Hacer esto es un error ya que puede machacar procesos dependientes del listado.
    // ((BfCompany *)mainCompany())->refreshCobrosCliente();
    _depura ( "END ImportCSV::~ImportCSV", 0 );
}

void ImportCSV::on_mui_buscarArchivo_clicked()
{

    QString fileName = QFileDialog::getOpenFileName ( this, _( "Open file" ), "", _( "All files (*)" ) );
    mui_archivo->setText ( fileName );
}

/** No precisa acciones adicionales en el destructor.
*/
void ImportCSV::on_mui_aceptar_clicked()
{
    _depura ( "ImportCSV::on_mui_aceptar_clicked", 0 );
    mensajeInfo ( "aceptar pulsado" );

    QFile file ( mui_archivo->text() );
    if ( !file.open ( QIODevice::ReadOnly | QIODevice::Text ) )
        return;

    if ( mui_cabeceras->isChecked() ) {
        file.readLine();
    } // end if

    mainCompany()->begin();
    while ( !file.atEnd() ) {
        QByteArray line = file.readLine();
        procesarLinea ( line );
    } // end while
    mainCompany()->commit();
    file.close();

    _depura ( "END ImportCSV::on_mui_aceptar_clicked", 0 );
}

void ImportCSV::on_mui_cabeceras_stateChanged ( int state )
{
    if ( state != 0 ) {
        m_claves.clear();
        QFile file ( mui_archivo->text() );
        if ( !file.open ( QIODevice::ReadOnly | QIODevice::Text ) )
            return;

        QString linea = file.readLine();
        QStringList list1;
        if ( mui_radioSeparadorTab->isChecked() ) {
            list1   = linea.split ( "\t" );
        } else {
            list1   = linea.split ( mui_separador->text() );
        } // end if

        QString text = "";

        for ( int i = 0; i < list1.size(); ++i ) {
            m_claves.append ( "[" + list1.at ( i ) + "]" );
            text += "[" + list1.at ( i ) + "] ";
        } // end for

        mui_cabecerasEdit->setText ( text );
        file.close();
    } // end if
}


void ImportCSV::procesarLinea ( const QString &linea )
{
    QStringList list1;
    if ( mui_radioSeparadorTab->isChecked() ) {
        list1   = linea.split ( "\t" );
    } else {
        list1   = linea.split ( mui_separador->text() );
    } // end if

    QString query = "INSERT INTO " + mui_combotablas->currentText() + "(";
    QString coma = "";
    for ( int i = 0; i < mui_list->rowCount(); ++i ) {
        if ( mui_list->item ( i, 1 ) )
            if ( mui_list->item ( i, 1 )->text() != "" ) {
                query += coma + mui_list->item ( i, 0 )->text();
                coma = ",";
            } // end if
    } // end for

    query += ") VALUES (";


    coma = "";
    for ( int i = 0; i < mui_list->rowCount(); ++i ) {
        if ( mui_list->item ( i, 1 ) )
            if ( mui_list->item ( i, 1 )->text() != "" ) {
                QString valor = mui_list->item ( i, 1 )->text();
                for ( int j = 0; j < m_claves.size() && j < list1.size(); ++j ) {
                    valor.replace ( m_claves[j], list1[j] );
                    valor.replace ( "[" + QString::number ( j ) + "]", list1[j] );
                } // end for
                query += coma + "'" + valor + "'";
                coma = ",";
            } // end if
    } // end for
    query += ")";

    mainCompany()->ejecuta ( query );
}


void ImportCSV::rellenarTablas()
{
    QString query = "select * from information_schema.tables where table_schema='public' and table_type='BASE TABLE' ORDER BY table_name";
    BlDbRecordSet *cur = mainCompany()->cargacursor ( query );
    mui_combotablas->clear();
    while ( !cur->eof() ) {
        mui_combotablas->addItem ( cur->valor ( "table_name" ) );
        cur->siguienteregistro();
    } // end while
    delete cur;
}


void ImportCSV::on_mui_combotablas_activated ( const QString & text )
{

    QString query = "SELECT a.attnum, a.attname AS field, t.typname AS type, a.attlen AS length, a.atttypmod AS lengthvar, a.attnotnull AS notnull FROM pg_class c, pg_attribute a, pg_type t WHERE c.relname = '" + text + "' and  a.attnum > 0 and a.attrelid = c.oid and a.atttypid = t.oid ORDER BY a.attnum";
    BlDbRecordSet *cur = mainCompany()->cargacursor ( query );
    mui_list->setRowCount ( cur->numregistros() );
    mui_list->setColumnCount ( 2 );
    int row = 0;
    while ( !cur->eof() ) {
        QTableWidgetItem *newItem = new QTableWidgetItem ( cur->valor ( "field" ) );
        mui_list->setItem ( row, 0, newItem );
        cur->siguienteregistro();
        row ++;
    } // end while
    delete cur;
}

