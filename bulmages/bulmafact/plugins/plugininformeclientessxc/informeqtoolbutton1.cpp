/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#include <QWidget>
#include "informeqtoolbutton1.h"
#include "funcaux.h"


#include <stdio.h>
#include <stdlib.h>

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QFileDialog>
#include <QMap>
#include <QList>
#include <QInputDialog>

#include "fixed.h"


#include "company.h"
#include "dbrecord.h"
#include "blprogressbar.h"

///
/**
\param art
\param parent
**/
InformeQToolButton1::InformeQToolButton1 ( ClientsList *art , QWidget *parent ) : QToolButton ( parent ), PEmpresaBase()
{
    _depura ( "InformeQToolButton1::InformeQToolButton1", 0 );
    m_clientsList = art;
    setBoton();
    _depura ( "END InformeQToolButton1::InformeQToolButton1", 0 );
}


///
/**
**/
InformeQToolButton1::~InformeQToolButton1()
{
    _depura ( "InformeQToolButton1::~InformeQToolButton1", 0 );
    _depura ( "END InformeQToolButton1::~InformeQToolButton1", 0 );
}

///
/**
**/
void InformeQToolButton1::setBoton()
{
    _depura ( "InformeQToolButton1::setBoton", 0 );
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( "Informe de Cobros en Hoja de Calculo" );
    setToolTip ( "Informe de Cobros en Hoja de Calculo" );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( confpr->valor ( CONF_PROGDATA ) + "icons/informeclientessxc.png" ) );
    setIconSize ( QSize ( 22, 22 ) );
    _depura ( "END InformeQToolButton1::setBoton", 0 );
}


///
/**
**/
void InformeQToolButton1::click()
{
    _depura ( "InformeQToolButton1::click", 0 );

    int y = 0;

    // Puede que no se haya actualizado bien el company
    setEmpresaBase ( m_clientsList->empresaBase() );


    // Solicitamos el ejercicio
    bool ok;
    int anyo = QInputDialog::getInteger ( this, _( "Ejercicio del Informe" ),
                                          _( "Anyo:" ), 2008, 1900, 4000, 1, &ok );


    QString archivod = confpr->valor ( CONF_DIR_USER ) + "informeclientessxc.perl";
    QString archivologo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "logo.jpg";


    /// Copiamos el logo.
#ifdef WINDOWS
    archivologo = "copy " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#else
    archivologo = "cp " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#endif


    QString fitxersortidatxt = "";

    fitxersortidatxt += "#!/usr/bin/perl\n";
    fitxersortidatxt += "# calc-example5 - Set Column sizes\n";
    fitxersortidatxt += "\n";
    fitxersortidatxt += "use ooolib;\n";
    fitxersortidatxt += "\n";
    fitxersortidatxt += "# Set variables\n";
    fitxersortidatxt += "$doc = new ooolib(\"sxc\");\n";
    fitxersortidatxt += "$doc->oooSet(\"builddir\", \".\");\n";
    fitxersortidatxt += "$doc->oooSet(\"title\", \"Informe de Clientes\");\n";
    fitxersortidatxt += "$doc->oooSet(\"subject\", \"Clientes\");\n";
    fitxersortidatxt += "$doc->oooSet(\"comments\", \"Informe para Clientes\");\n";
    fitxersortidatxt += "$doc->oooSet(\"author\", \"ooolib Example\");\n";
    fitxersortidatxt += "$doc->oooSet(\"meta1-name\", \"Programmer\");\n";
    fitxersortidatxt += "$doc->oooSet(\"meta1-value\", \"Tomeu Borras\");\n";
    fitxersortidatxt += "\n";

    fitxersortidatxt += "$doc->oooSet(\"cell-auto\", \"1,0\");\n";
    fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
    fitxersortidatxt += "$doc->oooSet(\"text-size\", \"18\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 4, 1);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"INFORME ANUAL DE COBROS POR CLIENTE\");\n";
    fitxersortidatxt += "$doc->oooSet(\"text-size\", \"10\");\n";
    fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";

    /// Escribimos los meses.
    fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 4, 3);\n";
    fitxersortidatxt += "$doc->oooSet(\"text-bgcolor\", \"CCCCCC\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Enero\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 5, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Febrero\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 6, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Marzo\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 7, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Abril\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 8, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Mayo\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 9, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Junio\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 10, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Julio\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 11, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Agosto\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 12, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Septiembre\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 13, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Octubre\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 14, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Noviembre\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 15, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Diciembre\");\n";

    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 17, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"TOTAL\");\n";
    fitxersortidatxt += "$doc->oooSet(\"text-bgcolor\", \"FFFFFF\");\n";
    fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";


    y = 5;
    /// Sacamos los datos del cliente.
    QString SQLQuery = "SELECT * FROM cliente ORDER BY nomcliente";
    cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );
    BLProgressBar barra;
    barra.setRange ( 0, cur->numregistros() );
    barra.show();
    barra.setText ( _( "Generando Informe" ) );

    while ( !cur->eof() ) {
        QString gen = generarCliente ( cur->valor ( "idcliente" ), y, anyo );
        if ( gen != "" ) {
            fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";
            fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
            fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 1, " + QString::number ( y ) + ");\n";
            fitxersortidatxt += "$doc->oooData(\"cell-text\", \"" + parsearCode ( cur->valor ( "cifcliente" ) ) + "\");\n";
//   fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 2, "+ QString::number ( y ) +");\n";
            fitxersortidatxt += "$doc->oooData(\"cell-text\", \"" + parsearCode ( cur->valor ( "nomcliente" ) ) + "\");\n";
            fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";
            fitxersortidatxt += gen;
            fitxersortidatxt += "\n";
            y++;
        } // end if
        cur->siguienteregistro();
        barra.setValue ( barra.value() + 1 );
    } // end while
    delete cur;


    fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";
    fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
    fitxersortidatxt += "$doc->oooSet(\"italic\", \"on\");\n";

    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 4, " + QString::number ( y + 1 ) + ");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(D5:D" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(E5:E" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(F5:F" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(G5:G" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(H5:H" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(I5:I" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(J5:J" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(K5:K" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(L5:L" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(M5:M" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(N5:N" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(O5:O" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";
    fitxersortidatxt += "$doc->oooSet(\"italic\", \"off\");\n";

    fitxersortidatxt += "my($filename) = $doc->oooGenerate(\"informeclientessxc.sxc\");\n";
    fitxersortidatxt += "\n";
    fitxersortidatxt += "exit;\n";


    QString cadena = "rm " + confpr->valor ( CONF_DIR_USER ) + "informeclientessxc.sxc";
    system ( cadena.toAscii() );
    cadena = "rm " + archivod;
    system ( cadena.toAscii() );

    QFile file ( archivod );
    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << fitxersortidatxt;
        file.close();
    } // end if

    cadena = " cd " + confpr->valor ( CONF_DIR_USER ) + "; perl " + archivod;
    system ( cadena.toAscii() );
    cadena = "kspread " + confpr->valor ( CONF_DIR_USER ) + "informeclientessxc.sxc &";
    system ( cadena.toAscii() );

    _depura ( "END InformeQToolButton1::click", 0 );
}



///
/**
\param idcliente
\param row
\return
**/
QString InformeQToolButton1::generarCliente ( QString idcliente, int row, int anyo )
{
    _depura ( "InformeQToolButton1::generarCliente", 0 );
    QString fitxersortidatxt = "";



    /// Sacamos todas las referencias de este cliente y las guardamos en el string referencias
    for ( int mes = 1; mes < 12; mes++ ) {
        QString SQLQuery = "select sum(cantcobro) AS total, idcliente from cobro ";
        SQLQuery += " WHERE fechacobro >= ('01/01/" + QString::number ( anyo ) + "')::DATE + '@" + QString::number ( mes - 1 ) + " mon'::INTERVAL ";
        SQLQuery += " AND fechacobro < ('01/01/" + QString::number ( anyo ) + "')::DATE + '@" + QString::number ( mes ) + " mon'::INTERVAL ";
        SQLQuery += " AND idcliente = " + idcliente + " AND previsioncobro = FALSE GROUP BY idcliente";
        cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );
        if ( !cur->eof() ) {
            fitxersortidatxt += "$doc->oooSet(\"cell-loc\", " + QString::number ( mes + 3 ) + ", " + QString::number ( row ) + ");\n";
            fitxersortidatxt += "$doc->oooSet(\"text-color\", \"00FF\");\n";
            fitxersortidatxt += "$doc->oooData(\"cell-text\", \"" + cur->valor ( "total" ) + "\");\n";
            fitxersortidatxt += "\n";
        } // end if
        delete cur;
    } // end for

    if ( fitxersortidatxt != "" ) {
        fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
        fitxersortidatxt += "$doc->oooSet(\"italic\", \"on\");\n";
        fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 17, " + QString::number ( row ) + ");\n";
        fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(D" + QString::number ( row ) + ":O" + QString::number ( row ) + ")\");\n";
        fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";
        fitxersortidatxt += "$doc->oooSet(\"italic\", \"off\");\n";
    } // end if
    _depura ( "END InformeQToolButton1::generarCliente", 0 );
    return fitxersortidatxt;
}


/// ===================================================================================
/// ===================================================================================


///
/**
\param art
\param parent
**/
InformeProveedorQToolButton1::InformeProveedorQToolButton1 ( ProveedorList *art , QWidget *parent ) : QToolButton ( parent ), PEmpresaBase()
{
    _depura ( "InformeProveedorQToolButton1::InformeProveedorQToolButton1", 0 );
    m_proveedorList = art;
    setBoton();
    _depura ( "END InformeProveedorQToolButton1::InformeProveedorQToolButton1", 0 );
}


///
/**
**/
InformeProveedorQToolButton1::~InformeProveedorQToolButton1()
{
    _depura ( "InformeProveedorQToolButton1::~InformeProveedorQToolButton1", 0 );
    _depura ( "END InformeProveedorQToolButton1::~InformeProveedorQToolButton1", 0 );
}

///
/**
**/
void InformeProveedorQToolButton1::setBoton()
{
    _depura ( "InformeProveedorQToolButton1::setBoton", 0 );
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( "Informe de Pagos en Hoja de Calculo" );
    setToolTip ( "Informe de Pagos en Hoja de Calculo" );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( confpr->valor ( CONF_PROGDATA ) + "icons/informeclientessxc.png" ) );
    setIconSize ( QSize ( 22, 22 ) );
    _depura ( "END InformeProveedorQToolButton1::setBoton", 0 );
}


///
/**
**/
void InformeProveedorQToolButton1::click()
{
    _depura ( "InformeProveedorQToolButton1::click", 0 );

    int y = 0;

    // Puede que no se haya actualizado bien el company
    setEmpresaBase ( m_proveedorList->empresaBase() );


    // Solicitamos el ejercicio
    bool ok;
    int anyo = QInputDialog::getInteger ( this, _( "Ejercicio del Informe" ),
                                          _( "Anyo:" ), 2008, 1900, 4000, 1, &ok );


    QString archivod = confpr->valor ( CONF_DIR_USER ) + "informeproveedoressxc.perl";
    QString archivologo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "logo.jpg";


    /// Copiamos el logo.
#ifdef WINDOWS
    archivologo = "copy " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#else
    archivologo = "cp " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#endif


    QString fitxersortidatxt = "";

    fitxersortidatxt += "#!/usr/bin/perl\n";
    fitxersortidatxt += "# calc-example5 - Set Column sizes\n";
    fitxersortidatxt += "\n";
    fitxersortidatxt += "use ooolib;\n";
    fitxersortidatxt += "\n";
    fitxersortidatxt += "# Set variables\n";
    fitxersortidatxt += "$doc = new ooolib(\"sxc\");\n";
    fitxersortidatxt += "$doc->oooSet(\"builddir\", \".\");\n";
    fitxersortidatxt += "$doc->oooSet(\"title\", \"Informe de Proveedores\");\n";
    fitxersortidatxt += "$doc->oooSet(\"subject\", \"Proveedores\");\n";
    fitxersortidatxt += "$doc->oooSet(\"comments\", \"Informe de Proveedores\");\n";
    fitxersortidatxt += "$doc->oooSet(\"author\", \"ooolib Example\");\n";
    fitxersortidatxt += "$doc->oooSet(\"meta1-name\", \"Programmer\");\n";
    fitxersortidatxt += "$doc->oooSet(\"meta1-value\", \"Tomeu Borras\");\n";
    fitxersortidatxt += "\n";

    fitxersortidatxt += "$doc->oooSet(\"cell-auto\", \"1,0\");\n";
    fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
    fitxersortidatxt += "$doc->oooSet(\"text-size\", \"18\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 4, 1);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"INFORME ANUAL DE COMPRAS POR PROVEEDOR\");\n";
    fitxersortidatxt += "$doc->oooSet(\"text-size\", \"10\");\n";
    fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";

    /// Escribimos los meses.
    fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 4, 3);\n";
    fitxersortidatxt += "$doc->oooSet(\"text-bgcolor\", \"CCCCCC\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Enero\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 5, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Febrero\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 6, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Marzo\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 7, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Abril\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 8, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Mayo\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 9, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Junio\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 10, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Julio\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 11, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Agosto\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 12, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Septiembre\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 13, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Octubre\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 14, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Noviembre\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 15, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"Diciembre\");\n";

    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 17, 3);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"TOTAL\");\n";
    fitxersortidatxt += "$doc->oooSet(\"text-bgcolor\", \"FFFFFF\");\n";
    fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";


    y = 5;
    /// Sacamos los datos del cliente.
    QString SQLQuery = "SELECT * FROM proveedor ORDER BY nomproveedor";
    cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );

    /// Ponemos la barra de progreso
    BLProgressBar barra;
    barra.setRange ( 0, cur->numregistros() );
    barra.show();
    barra.setText ( _( "Generando informe" ) );

    while ( !cur->eof() ) {
        QString gen = generarProveedor ( cur->valor ( "idproveedor" ), y, anyo );
        if ( gen != "" ) {
            fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";
            fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
            fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 1, " + QString::number ( y ) + ");\n";
            fitxersortidatxt += "$doc->oooData(\"cell-text\", \"" + parsearCode ( cur->valor ( "cifproveedor" ) ) + "\");\n";
            fitxersortidatxt += "$doc->oooData(\"cell-text\", \"" + parsearCode ( cur->valor ( "nomproveedor" ) ) + "\");\n";
            fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";
            fitxersortidatxt += gen;
            fitxersortidatxt += "\n";
            y++;
        } // end if
        cur->siguienteregistro();
        barra.setValue ( barra.value() + 1 );
    } // end while
    delete cur;


    fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";
    fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
    fitxersortidatxt += "$doc->oooSet(\"italic\", \"on\");\n";

    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 4, " + QString::number ( y + 1 ) + ");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(D5:D" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(E5:E" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(F5:F" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(G5:G" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(H5:H" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(I5:I" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(J5:J" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(K5:K" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(L5:L" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(M5:M" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(N5:N" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(O5:O" + QString::number ( y - 1 ) + ")\");\n";
    fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";
    fitxersortidatxt += "$doc->oooSet(\"italic\", \"off\");\n";

    fitxersortidatxt += "my($filename) = $doc->oooGenerate(\"informeproveedoressxc.sxc\");\n";
    fitxersortidatxt += "\n";
    fitxersortidatxt += "exit;\n";


    QString cadena = "rm " + confpr->valor ( CONF_DIR_USER ) + "informeproveedoressxc.sxc";
    system ( cadena.toAscii() );
    cadena = "rm " + archivod;
    system ( cadena.toAscii() );

    QFile file ( archivod );
    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << fitxersortidatxt;
        file.close();
    } // end if

    cadena = " cd " + confpr->valor ( CONF_DIR_USER ) + "; perl " + archivod;
    system ( cadena.toAscii() );
    cadena = "kspread " + confpr->valor ( CONF_DIR_USER ) + "informeproveedoressxc.sxc &";
    system ( cadena.toAscii() );

    _depura ( "END InformeProveedorQToolButton1::click", 0 );
}


///
/**
\param idproveedor
\param row
\return
**/
QString InformeProveedorQToolButton1::generarProveedor ( QString idproveedor, int row, int anyo )
{
    _depura ( "InformeProveedorQToolButton1::generarProveedor", 0 );
    QString fitxersortidatxt = "";

    /// Sacamos todas las referencias de este cliente y las guardamos en el string referencias
    for ( int mes = 1; mes < 12; mes++ ) {
        QString SQLQuery = "select sum(cantpago) AS total, idproveedor from pago ";
        SQLQuery += " WHERE fechapago >= ('01/01/" + QString::number ( anyo ) + "')::DATE + '@" + QString::number ( mes - 1 ) + " mon'::INTERVAL ";
        SQLQuery += " AND fechapago < ('01/01/" + QString::number ( anyo ) + "')::DATE + '@" + QString::number ( mes ) + " mon'::INTERVAL ";
        SQLQuery += " AND idproveedor = " + idproveedor + " AND previsionpago = FALSE GROUP BY idproveedor";
        cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );
        if ( !cur->eof() ) {
            fitxersortidatxt += "$doc->oooSet(\"cell-loc\", " + QString::number ( mes + 3 ) + ", " + QString::number ( row ) + ");\n";
            fitxersortidatxt += "$doc->oooSet(\"text-color\", \"00FF\");\n";
            fitxersortidatxt += "$doc->oooData(\"cell-text\", \"" + cur->valor ( "total" ) + "\");\n";
            fitxersortidatxt += "\n";
        } // end if
        delete cur;
    } // end for

    if ( fitxersortidatxt != "" ) {
        fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
        fitxersortidatxt += "$doc->oooSet(\"italic\", \"on\");\n";
        fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 17, " + QString::number ( row ) + ");\n";
        fitxersortidatxt += "$doc->oooData(\"cell-text\", \"=SUM(D" + QString::number ( row ) + ":O" + QString::number ( row ) + ")\");\n";
        fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";
        fitxersortidatxt += "$doc->oooSet(\"italic\", \"off\");\n";
    } // end if
    _depura ( "END InformeProveedorQToolButton1::generarProveedor", 0 );
    return fitxersortidatxt;
}

