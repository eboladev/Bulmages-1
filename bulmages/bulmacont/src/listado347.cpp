/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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

#include <errno.h>
#include <QLocale>
#include <QFile>
#include <QTextStream>

#include "listado347.h"
#include "blfunctions.h"


///
/**
\param emp
\param ejerActual
\param parent
\param f
**/
Listado347::Listado347 ( Empresa *emp, QString ejerActual, QWidget *parent, Qt::WFlags f )
        : QDialog ( parent, f ), BlMainCompanyPointer ( emp )
{
    _depura ( "Listado347::Listado347", 0 );
    setupUi ( this );
    importe->setText ( "3005.06" );
    finicial->setText ( normalizafecha ( "01/01/" + ejerActual ).toString ( "dd/MM/yyyy" ) );
    ffinal->setText ( normalizafecha ( "31/12/" + ejerActual ).toString ( "dd/MM/yyyy" ) );
    /// Carga las tablas en pantalla.
    on_m_boton_recalcular_clicked();
    centrarEnPantalla ( this );
    _depura ( "END Listado347::Listado347", 0 );
}


///
/**
**/
Listado347::~Listado347()
{
    _depura ( "Listado347::~Listado347", 0 );
    centrarEnPantalla ( this );
    _depura ( "END Listado347::~Listado347", 0 );
}


///
/**
**/
void Listado347::on_m_boton_recalcular_clicked()
{
    _depura ( "Listado347::click_recargar", 0 );

    QLocale::setDefault ( QLocale ( QLocale::Spanish, QLocale::Spain ) );
    QLocale spanish;

    /// A por la tabla de Ventas...pedazo de consulta SQL.
    QString query =  "SELECT codigo, descripcion, cifent_cuenta as cif, cpent_cuenta as cp, importe FROM cuenta ";
    query += "INNER JOIN (SELECT idcuenta, sum(apunte.debe) as importe FROM apunte ";
    query += "WHERE conceptocontable NOT SIMILAR TO '%(Asiento de Cierre|Asiento de Apertura)%' AND ";
    query += "idasiento IN (SELECT idasiento FROM (SELECT idcuenta FROM cuenta ";
    query += "WHERE codigo LIKE '4770%') AS iva INNER JOIN apunte USING (idcuenta) ";
    query += "WHERE fecha <= '" + ffinal->text() + "' AND fecha >= '" + finicial->text() + "' ";
    query += "GROUP BY idasiento) AND idcuenta IN (SELECT idcuenta FROM cuenta ";
    query += "WHERE codigo LIKE '4300%') GROUP BY idcuenta) AS facturado USING(idcuenta) ";
    query += "WHERE importe > " + importe->text() + " ORDER BY descripcion";

    empresaBase() ->begin();
    BlDbRecordSet *recordSet = empresaBase() ->cargacursor ( query, "recordSet" );
    empresaBase() ->commit();

    tablaventas->setColumnCount ( 5 );
    tablaventas->setRowCount ( recordSet->numregistros() );
    QStringList cabecera;
    cabecera  << "codigo" << "descripcion" << "cif/nif" << "cp" << "importe";
    tablaventas->setHorizontalHeaderLabels ( cabecera );
    int i = 0;
    QTableWidgetItem *item;
    while ( !recordSet->eof() ) {
        item = new QTableWidgetItem ( recordSet->valor ( "codigo" ) );
        tablaventas->setItem ( i, 0, item );
        item = new QTableWidgetItem ( recordSet->valor ( "descripcion" ) );
        tablaventas->setItem ( i, 1, item );
        item = new QTableWidgetItem ( recordSet->valor ( "cif" ) );
        tablaventas->setItem ( i, 2, item );
        item = new QTableWidgetItem ( recordSet->valor ( "cp" ) );
        tablaventas->setItem ( i, 3, item );
        query = spanish.toString ( recordSet->valor ( "importe" ).toDouble(), 'f', 2 );
        item = new QTableWidgetItem ( query );
        item->setTextAlignment ( Qt::AlignRight | Qt::AlignVCenter );
        tablaventas->setItem ( i, 4, item );
        ++i;
        recordSet->siguienteregistro();
    } // end while
    /// A por la tabla de Compras...
    query =  "SELECT codigo, descripcion, cifent_cuenta as cif, cpent_cuenta as cp, importe FROM cuenta ";
    query += "INNER JOIN (SELECT idcuenta, sum(apunte.haber) as importe FROM apunte ";
    query += "WHERE conceptocontable NOT SIMILAR TO '%(Asiento de Cierre|Asiento de Apertura)%' ";
    query += "AND idasiento IN (SELECT idasiento FROM (SELECT idcuenta FROM cuenta ";
    query += "WHERE codigo LIKE '4720%') AS iva ";
    query += "INNER JOIN apunte USING (idcuenta) ";
    query += "WHERE fecha <= '" + ffinal->text() + "' AND fecha >= '" + finicial->text() + "' ";
    query += "GROUP BY idasiento) AND idcuenta IN (SELECT idcuenta FROM cuenta ";
    query += "WHERE codigo SIMILAR TO '4(0|1)00%') GROUP BY idcuenta) AS facturado USING(idcuenta) ";
    query += "WHERE importe > " + importe->text() + " ORDER BY descripcion";

    empresaBase() ->begin();
    recordSet = empresaBase() ->cargacursor ( query, "recordSet" );
    empresaBase() ->commit();

    tablacompras->setColumnCount ( 5 );
    tablacompras->setRowCount ( recordSet->numregistros() );
    tablacompras->setHorizontalHeaderLabels ( cabecera );
    i = 0;
    while ( !recordSet->eof() ) {
        item = new QTableWidgetItem ( recordSet->valor ( "codigo" ) );
        tablacompras->setItem ( i, 0, item );
        item = new QTableWidgetItem ( recordSet->valor ( "descripcion" ) );
        tablacompras->setItem ( i, 1, item );
        item = new QTableWidgetItem ( recordSet->valor ( "cif" ) );
        tablacompras->setItem ( i, 2, item );
        item = new QTableWidgetItem ( recordSet->valor ( "cp" ) );
        tablacompras->setItem ( i, 3, item );
        query = spanish.toString ( recordSet->valor ( "importe" ).toDouble(), 'f', 2 );
        item = new QTableWidgetItem ( query );
        item->setTextAlignment ( Qt::AlignRight | Qt::AlignVCenter );
        tablacompras->setItem ( i, 4, item );
        ++i;
        recordSet->siguienteregistro();
    } // end while
    tablacompras->resizeColumnsToContents();
    tablaventas->resizeColumnsToContents();

    QLocale::setDefault ( QLocale::C );

    _depura ( "END Listado347::click_recargar", 0 );
}


///
/**
**/
void Listado347::on_m_boton_imprimir_clicked()
{
    _depura ( "Listado347::click_imprimir", 0 );
    int i, numventas, numcompras;
    QString codigo, descripcion, cif, importe, cp;
    QFile fichero ( confpr->valor ( CONF_DIR_USER ) + "listado347.txt" );
    if ( fichero.open ( QIODevice::WriteOnly | QIODevice::Text ) ) {
        QTextStream listado ( &fichero );
        listado << "LISTADO 347\n";
        listado << "\nCuenta     Deudor                         CIF/NIF     CP  Importe\n";
        listado << "________________________________________________________________________________________\n";
        numventas = tablaventas->rowCount();
        for ( i = 0; i < numventas; i++ ) {
            codigo = tablaventas->item ( i, 0 ) ->text();
            descripcion = tablaventas->item ( i, 1 ) ->text();
            cif = tablaventas->item ( i, 2 ) ->text();
            cp = tablaventas->item ( i, 3 ) ->text();
            importe = tablaventas->item ( i, 4 ) ->text();
            listado << qSetFieldWidth ( 9 ) << left << codigo.toAscii().constData() << qSetFieldWidth ( 50 ) << descripcion.toAscii().constData() << qSetFieldWidth ( 11 ) << cif.toAscii().constData() << qSetFieldWidth ( 6 ) << cp.toAscii().constData() << qSetFieldWidth ( 12 ) << right << importe.toAscii() << endl;
        } // end for
        numcompras = tablacompras->rowCount();
        listado << "\nCuenta     Acreedor                       CIF/NIF     CP  Importe\n";
        listado << "________________________________________________________________________________________\n";
        numcompras = tablacompras->rowCount();
        for ( i = 0; i < numcompras; i++ ) {
            codigo = tablacompras->item ( i, 0 ) ->text();
            descripcion = tablacompras->item ( i, 1 ) ->text();
            cif = tablacompras->item ( i, 2 ) ->text();
            cp = tablacompras->item ( i, 3 ) ->text();
            importe = tablacompras->item ( i, 4 ) ->text();
            listado << qSetFieldWidth ( 9 ) << left << codigo.toAscii().constData() << qSetFieldWidth ( 50 ) << descripcion.toAscii().constData() << qSetFieldWidth ( 11 ) << cif.toAscii().constData() << qSetFieldWidth ( 6 ) << cp.toAscii().constData() << qSetFieldWidth ( 12 ) << right << importe.toAscii() << endl;
        } // end for
        fichero.close();
    } // end if
    QString comando = confpr->valor ( CONF_EDITOR ).toAscii() + " " + fichero.fileName();
    system ( comando.toAscii().constData() );

    _depura ( "END Listado347::click_imprimir", 0 );
}


///
/**
**/
void Listado347::on_finicial_editingFinished()
{
    _depura ( "Listado347::finicial_exit", 0 );
    finicial->setText ( normalizafecha ( finicial->text() ).toString ( "dd/MM/yyyy" ) );
    _depura ( "END Listado347::finicial_exit", 0 );
}


///
/**
**/
void Listado347::on_ffinal_editingFinished()
{
    _depura ( "Listado347::ffinal_exit", 0 );
    ffinal->setText ( normalizafecha ( ffinal->text() ).toString ( "dd/MM/yyyy" ) );
    _depura ( "END Listado347::ffinal_exit", 0 );
}

