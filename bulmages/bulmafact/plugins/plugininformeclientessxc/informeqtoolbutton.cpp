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
#include "informeqtoolbutton.h"
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
InformeQToolButton::InformeQToolButton ( ClientsList *art , QWidget *parent ) : QToolButton ( parent ), PEmpresaBase()
{
    _depura ( "InformeQToolButton::InformeQToolButton", 0 );
    m_clientsList = art;
    setBoton();
    _depura ( "END InformeQToolButton::InformeQToolButton", 0 );
}


///
/**
**/
InformeQToolButton::~InformeQToolButton()
{
    _depura ( "InformeQToolButton::~InformeQToolButton", 0 );
    _depura ( "END InformeQToolButton::~InformeQToolButton", 0 );
}

///
/**
**/
void InformeQToolButton::setBoton()
{
    _depura ( "InformeQToolButton::setBoton", 0 );
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( "Informe en Hoja de Cálculo" );
    setToolTip ( "Informe en Hoja de Cálculo" );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( confpr->valor ( CONF_PROGDATA ) + "icons/informeclientessxc.png" ) );
    setIconSize ( QSize ( 22, 22 ) );
    _depura ( "END InformeQToolButton::setBoton", 0 );
}


///
/**
**/
void InformeQToolButton::click()
{
    _depura ( "InformeQToolButton::click", 0 );

    int y = 0;

    // Puede que no se haya actualizado bien el company
    setEmpresaBase ( m_clientsList->empresaBase() );


    // Solicitamos el ejercicio
    bool ok;
    int anyo = QInputDialog::getInteger ( this, tr ( "Ejercicio del Informe" ),
                                          tr ( "Anyo:" ), 2008, 1900, 4000, 1, &ok );


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
    fitxersortidatxt += "$doc->oooSet(\"meta1-value\", \"Tomeu Borrás\");\n";
    fitxersortidatxt += "\n";

    fitxersortidatxt += "$doc->oooSet(\"cell-auto\", \"1,0\");\n";
    fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
    fitxersortidatxt += "$doc->oooSet(\"text-size\", \"18\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 4, 1);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"INFORME ANUAL DE VENTAS POR CLIENTE\");\n";
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
    /// Tratamos la barra de progreso
    BLProgressBar barra;
    barra.setRange ( 0, cur->numregistros() );
    barra.show();
    barra.setText ( tr ( "Generando Informe" ) );

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

    _depura ( "END InformeQToolButton::click", 0 );
}



///
/**
\param idcliente
\param row
\return
**/
QString InformeQToolButton::generarCliente ( QString idcliente, int row, int anyo )
{
    _depura ( "InformeQToolButton::generarCliente", 0 );
    QString fitxersortidatxt = "";



    /// Sacamos todas las referencias de este cliente y las guardamos en el string referencias
    for ( int mes = 1; mes < 12; mes++ ) {
        QString SQLQuery = "select sum(totalfactura) AS total, idcliente from factura ";
        SQLQuery += " WHERE ffactura >= ('01/01/" + QString::number ( anyo ) + "')::DATE + '@" + QString::number ( mes - 1 ) + " mon'::INTERVAL ";
        SQLQuery += " AND ffactura < ('01/01/" + QString::number ( anyo ) + "')::DATE + '@" + QString::number ( mes ) + " mon'::INTERVAL ";
        SQLQuery += " AND idcliente = " + idcliente + " GROUP BY idcliente";
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
    _depura ( "END InformeQToolButton::generarCliente", 0 );
    return fitxersortidatxt;
}


/// ===================================================================================
/// ===================================================================================


///
/**
\param art
\param parent
**/
InformeProveedorQToolButton::InformeProveedorQToolButton ( ProveedorList *art , QWidget *parent ) : QToolButton ( parent ), PEmpresaBase()
{
    _depura ( "InformeProveedorQToolButton::InformeProveedorQToolButton", 0 );
    m_proveedorList = art;
    setBoton();
    _depura ( "END InformeProveedorQToolButton::InformeProveedorQToolButton", 0 );
}


///
/**
**/
InformeProveedorQToolButton::~InformeProveedorQToolButton()
{
    _depura ( "InformeProveedorQToolButton::~InformeProveedorQToolButton", 0 );
    _depura ( "END InformeProveedorQToolButton::~InformeProveedorQToolButton", 0 );
}

///
/**
**/
void InformeProveedorQToolButton::setBoton()
{
    _depura ( "InformeProveedorQToolButton::setBoton", 0 );
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( "Informe en Hoja de Cálculo" );
    setToolTip ( "Informe en Hoja de Cálculo" );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( confpr->valor ( CONF_PROGDATA ) + "icons/informeclientessxc.png" ) );
    setIconSize ( QSize ( 22, 22 ) );
    _depura ( "END InformeProveedorQToolButton::setBoton", 0 );
}


///
/**
**/
void InformeProveedorQToolButton::click()
{
    _depura ( "InformeProveedorQToolButton::click", 0 );

    int y = 0;

    // Puede que no se haya actualizado bien el company
    setEmpresaBase ( m_proveedorList->empresaBase() );


    // Solicitamos el ejercicio
    bool ok;
    int anyo = QInputDialog::getInteger ( this, tr ( "Ejercicio del Informe" ),
                                          tr ( "Anyo:" ), 2008, 1900, 4000, 1, &ok );


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
    fitxersortidatxt += "$doc->oooSet(\"meta1-value\", \"Tomeu Borrás\");\n";
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
    BLProgressBar barra;
    barra.setRange ( 0, cur->numregistros() );
    barra.show();
    barra.setText ( tr ( "Generando Informe" ) );

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

    _depura ( "END InformeProveedorQToolButton::click", 0 );
}


///
/**
\param idproveedor
\param row
\return
**/
QString InformeProveedorQToolButton::generarProveedor ( QString idproveedor, int row, int anyo )
{
    _depura ( "InformeProveedorQToolButton::generarProveedor", 0 );
    QString fitxersortidatxt = "";

    /// Sacamos todas las referencias de este cliente y las guardamos en el string referencias
    for ( int mes = 1; mes < 12; mes++ ) {
        QString SQLQuery = "select sum(totalfacturap) AS total, idproveedor from facturap ";
        SQLQuery += " WHERE ffacturap >= ('01/01/" + QString::number ( anyo ) + "')::DATE + '@" + QString::number ( mes - 1 ) + " mon'::INTERVAL ";
        SQLQuery += " AND ffacturap < ('01/01/" + QString::number ( anyo ) + "')::DATE + '@" + QString::number ( mes ) + " mon'::INTERVAL ";
        SQLQuery += " AND idproveedor = " + idproveedor + " GROUP BY idproveedor";
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
    _depura ( "END InformeProveedorQToolButton::generarProveedor", 0 );
    return fitxersortidatxt;
}

