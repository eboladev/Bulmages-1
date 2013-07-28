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
#include <QtCore/QLocale>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "bcmodelo347listview.h"
#include "blfunctions.h"


///
/**
\param emp
\param ejerActual
\param parent
\param f
**/
BcModelo347ListView::BcModelo347ListView ( BcCompany *emp, QString ejerActual, QWidget *parent, Qt::WindowFlags f )
        : QDialog ( parent, f ), BlMainCompanyPointer ( emp )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    importe->setText ( "3005.06" );
    finicial->setText ( blNormalizeDate ( "01/01/" + ejerActual ).toString ( "dd/MM/yyyy" ) );
    ffinal->setText ( blNormalizeDate ( "31/12/" + ejerActual ).toString ( "dd/MM/yyyy" ) );
    /// Carga las tablas en pantalla.
    on_m_boton_recalcular_clicked();
    blCenterOnScreen ( this );
    
}


///
/**
**/
BcModelo347ListView::~BcModelo347ListView()
{
    BL_FUNC_DEBUG
    blCenterOnScreen ( this );
    
}


///
/**
**/
void BcModelo347ListView::on_m_boton_recalcular_clicked()
{
    BL_FUNC_DEBUG

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

    mainCompany() ->begin();
    BlDbRecordSet *recordSet = mainCompany() ->loadQuery ( query, "recordSet" );
    mainCompany() ->commit();

    tablaventas->setColumnCount ( 5 );
    tablaventas->setRowCount ( recordSet->numregistros() );
    QStringList cabecera;
    cabecera  << "codigo" << "descripcion" << "cif/nif" << "cp" << "importe";
    tablaventas->setHorizontalHeaderLabels ( cabecera );
    int i = 0;
    QTableWidgetItem *item;
    while ( !recordSet->eof() ) {
        item = new QTableWidgetItem ( recordSet->value( "codigo" ) );
        tablaventas->setItem ( i, 0, item );
        item = new QTableWidgetItem ( recordSet->value( "descripcion" ) );
        tablaventas->setItem ( i, 1, item );
        item = new QTableWidgetItem ( recordSet->value( "cif" ) );
        tablaventas->setItem ( i, 2, item );
        item = new QTableWidgetItem ( recordSet->value( "cp" ) );
        tablaventas->setItem ( i, 3, item );
        query = spanish.toString ( recordSet->value( "importe" ).toDouble(), 'f', 2 );
        item = new QTableWidgetItem ( query );
        item->setTextAlignment ( Qt::AlignRight | Qt::AlignVCenter );
        tablaventas->setItem ( i, 4, item );
        ++i;
        recordSet->nextRecord();
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

    mainCompany() ->begin();
    recordSet = mainCompany() ->loadQuery ( query, "recordSet" );
    mainCompany() ->commit();

    tablacompras->setColumnCount ( 5 );
    tablacompras->setRowCount ( recordSet->numregistros() );
    tablacompras->setHorizontalHeaderLabels ( cabecera );
    i = 0;
    while ( !recordSet->eof() ) {
        item = new QTableWidgetItem ( recordSet->value( "codigo" ) );
        tablacompras->setItem ( i, 0, item );
        item = new QTableWidgetItem ( recordSet->value( "descripcion" ) );
        tablacompras->setItem ( i, 1, item );
        item = new QTableWidgetItem ( recordSet->value( "cif" ) );
        tablacompras->setItem ( i, 2, item );
        item = new QTableWidgetItem ( recordSet->value( "cp" ) );
        tablacompras->setItem ( i, 3, item );
        query = spanish.toString ( recordSet->value( "importe" ).toDouble(), 'f', 2 );
        item = new QTableWidgetItem ( query );
        item->setTextAlignment ( Qt::AlignRight | Qt::AlignVCenter );
        tablacompras->setItem ( i, 4, item );
        ++i;
        recordSet->nextRecord();
    } // end while
    tablacompras->resizeColumnsToContents();
    tablaventas->resizeColumnsToContents();

    QLocale::setDefault ( QLocale::C );

    
}


///
/**
**/
void BcModelo347ListView::on_m_boton_imprimir_clicked()
{
    BL_FUNC_DEBUG
    int i, numventas, numcompras;
    QString codigo, descripcion, cif, importe, cp;
    QFile fichero ( g_confpr->value( CONF_DIR_USER ) + "listado347.txt" );
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
            listado << qSetFieldWidth ( 9 ) << left << codigo.toLatin1().constData() << qSetFieldWidth ( 50 ) << descripcion.toLatin1().constData() << qSetFieldWidth ( 11 ) << cif.toLatin1().constData() << qSetFieldWidth ( 6 ) << cp.toLatin1().constData() << qSetFieldWidth ( 12 ) << right << importe.toLatin1() << endl;
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
            listado << qSetFieldWidth ( 9 ) << left << codigo.toLatin1().constData() << qSetFieldWidth ( 50 ) << descripcion.toLatin1().constData() << qSetFieldWidth ( 11 ) << cif.toLatin1().constData() << qSetFieldWidth ( 6 ) << cp.toLatin1().constData() << qSetFieldWidth ( 12 ) << right << importe.toLatin1() << endl;
        } // end for
        fichero.close();
    } // end if
    QString comando = g_confpr->value( CONF_EDITOR ).toLatin1() + " " + fichero.fileName();
    system ( comando.toLatin1().constData() );

    
}


///
/**
**/
void BcModelo347ListView::on_finicial_editingFinished()
{
    BL_FUNC_DEBUG
    finicial->setText ( blNormalizeDate ( finicial->text() ).toString ( "dd/MM/yyyy" ) );
    
}


///
/**
**/
void BcModelo347ListView::on_ffinal_editingFinished()
{
    BL_FUNC_DEBUG
    ffinal->setText ( blNormalizeDate ( ffinal->text() ).toString ( "dd/MM/yyyy" ) );
    
}

