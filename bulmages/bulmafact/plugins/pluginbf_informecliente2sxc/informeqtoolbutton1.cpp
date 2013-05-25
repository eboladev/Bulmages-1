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

#include <stdio.h>
#include <stdlib.h>

#include <QtWidgets/QWidget>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QString>
#include <QtWidgets/QFileDialog>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtWidgets/QInputDialog>

#include "informeqtoolbutton1.h"
#include "blfunctions.h"
#include "blfixed.h"
#include "bfcompany.h"
#include "bldb.h"
#include "blprogressbar.h"


///
/**
\param art
\param parent
**/
InformeQToolButton1::InformeQToolButton1 ( ClientsList *art , QWidget *parent ) : QToolButton ( parent ), BlMainCompanyPointer()
{
    BL_FUNC_DEBUG
    m_clientsList = art;
    setBoton();
    
}


///
/**
**/
InformeQToolButton1::~InformeQToolButton1()
{
    BL_FUNC_DEBUG
    
}

///
/**
**/
void InformeQToolButton1::setBoton()
{
    BL_FUNC_DEBUG
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( "Informe de Cobros en Hoja de Calculo" );
    setToolTip ( "Informe de Cobros en Hoja de Calculo" );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/informeclientessxc.png" ) );
    setIconSize ( QSize ( 22, 22 ) );
    
}


///
/**
**/
void InformeQToolButton1::click()
{
    BL_FUNC_DEBUG

    int y = 0;

    // Puede que no se haya actualizado bien el company
    setMainCompany ( m_clientsList->mainCompany() );


    // Solicitamos el ejercicio
    bool ok;
    int anyo = QInputDialog::getInt ( this, _ ( "Ejercicio del Informe" ),
                                          _ ( "Anyo:" ), 2008, 1900, 4000, 1, &ok );

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
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery );
    BlProgressBar barra;
    barra.setRange ( 0, cur->numregistros() );
    barra.show();
    barra.setText ( _ ( "Generando Informe" ) );

    while ( !cur->eof() ) {
        QString gen = generarCliente ( cur->value( "idcliente" ), y, anyo );
        if ( gen != "" ) {
            fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";
            fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
            fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 1, " + QString::number ( y ) + ");\n";
            fitxersortidatxt += "$doc->oooData(\"cell-text\", \"" + blScriptEscape ( cur->value( "cifcliente" ) ) + "\");\n";
//   fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 2, "+ QString::number ( y ) +");\n";
            fitxersortidatxt += "$doc->oooData(\"cell-text\", \"" + blScriptEscape ( cur->value( "nomcliente" ) ) + "\");\n";
            fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";
            fitxersortidatxt += gen;
            fitxersortidatxt += "\n";
            y++;
        } // end if
        cur->nextRecord();
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


    QString informeclientessxc = g_confpr->value( CONF_DIR_USER ) + "informeclientessxc.sxc";
    blRemove(informeclientessxc);
    QString informeclientessxcperl = g_confpr->value( CONF_DIR_USER ) + "informeclientessxc.perl";
    blRemove(informeclientessxcperl);

    QFile file ( informeclientessxcperl );
    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << fitxersortidatxt;
        file.close();
    } // end if

    QString cadena = " cd " + g_confpr->value( CONF_DIR_USER ) + "; perl " + informeclientessxcperl;
    system ( cadena.toLatin1() );
    cadena = "kspread " + g_confpr->value( CONF_DIR_USER ) + "informeclientessxc.sxc &";
    system ( cadena.toLatin1() );

    
}



///
/**
\param idcliente
\param row
\return
**/
QString InformeQToolButton1::generarCliente ( QString idcliente, int row, int anyo )
{
    BL_FUNC_DEBUG
    QString fitxersortidatxt = "";



    /// Sacamos todas las referencias de este cliente y las guardamos en el string referencias
    for ( int mes = 1; mes < 12; mes++ ) {
        QString SQLQuery = "select sum(cantcobro) AS total, idcliente from cobro ";
        SQLQuery += " WHERE fechacobro >= ('01/01/" + QString::number ( anyo ) + "')::DATE + '@" + QString::number ( mes - 1 ) + " mon'::INTERVAL ";
        SQLQuery += " AND fechacobro < ('01/01/" + QString::number ( anyo ) + "')::DATE + '@" + QString::number ( mes ) + " mon'::INTERVAL ";
        SQLQuery += " AND idcliente = " + idcliente + " AND previsioncobro = false GROUP BY idcliente";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery );
        if ( !cur->eof() ) {
            fitxersortidatxt += "$doc->oooSet(\"cell-loc\", " + QString::number ( mes + 3 ) + ", " + QString::number ( row ) + ");\n";
            fitxersortidatxt += "$doc->oooSet(\"text-color\", \"00FF\");\n";
            fitxersortidatxt += "$doc->oooData(\"cell-text\", \"" + cur->value( "total" ) + "\");\n";
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
    
    return fitxersortidatxt;
}


/// ===================================================================================
/// ===================================================================================


///
/**
\param art
\param parent
**/
InformeProveedorQToolButton1::InformeProveedorQToolButton1 ( ProveedorList *art , QWidget *parent ) : QToolButton ( parent ), BlMainCompanyPointer()
{
    BL_FUNC_DEBUG
    m_proveedorList = art;
    setBoton();
    
}


///
/**
**/
InformeProveedorQToolButton1::~InformeProveedorQToolButton1()
{
    BL_FUNC_DEBUG
    
}

///
/**
**/
void InformeProveedorQToolButton1::setBoton()
{
    BL_FUNC_DEBUG
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( "Informe de Pagos en Hoja de Calculo" );
    setToolTip ( "Informe de Pagos en Hoja de Calculo" );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/informeclientessxc.png" ) );
    setIconSize ( QSize ( 22, 22 ) );
    
}


///
/**
**/
void InformeProveedorQToolButton1::click()
{
    BL_FUNC_DEBUG

    int y = 0;

    // Puede que no se haya actualizado bien el company
    setMainCompany ( m_proveedorList->mainCompany() );


    // Solicitamos el ejercicio
    bool ok;
    int anyo = QInputDialog::getInt( this, _ ( "Ejercicio del Informe" ),
                                          _ ( "Anyo:" ), 2008, 1900, 4000, 1, &ok );


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
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery );

    /// Ponemos la barra de progreso
    BlProgressBar barra;
    barra.setRange ( 0, cur->numregistros() );
    barra.show();
    barra.setText ( _ ( "Generando informe" ) );

    while ( !cur->eof() ) {
        QString gen = generarProveedor ( cur->value( "idproveedor" ), y, anyo );
        if ( gen != "" ) {
            fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";
            fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
            fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 1, " + QString::number ( y ) + ");\n";
            fitxersortidatxt += "$doc->oooData(\"cell-text\", \"" + blScriptEscape ( cur->value( "cifproveedor" ) ) + "\");\n";
            fitxersortidatxt += "$doc->oooData(\"cell-text\", \"" + blScriptEscape ( cur->value( "nomproveedor" ) ) + "\");\n";
            fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";
            fitxersortidatxt += gen;
            fitxersortidatxt += "\n";
            y++;
        } // end if
        cur->nextRecord();
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
   
    QString informeproveedoressxc = g_confpr->value( CONF_DIR_USER ) + "informeproveedoressxc.sxc";
    blRemove(informeproveedoressxc);
    QString informeproveedoressxcperl = g_confpr->value( CONF_DIR_USER ) + "informeproveedoressxc.perl";
    blRemove(informeproveedoressxcperl);

    QFile file ( informeproveedoressxcperl );
    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << fitxersortidatxt;
        file.close();
    } // end if

    QString cadena = " cd " + g_confpr->value( CONF_DIR_USER ) + "; perl " + informeproveedoressxcperl;
    system ( cadena.toLatin1() );
    cadena = "kspread " + g_confpr->value( CONF_DIR_USER ) + "informeproveedoressxc.sxc &";
    system ( cadena.toLatin1() );

    
}


///
/**
\param idproveedor
\param row
\return
**/
QString InformeProveedorQToolButton1::generarProveedor ( QString idproveedor, int row, int anyo )
{
    BL_FUNC_DEBUG
    QString fitxersortidatxt = "";

    /// Sacamos todas las referencias de este cliente y las guardamos en el string referencias
    for ( int mes = 1; mes < 12; mes++ ) {
        QString SQLQuery = "select sum(cantpago) AS total, idproveedor from pago ";
        SQLQuery += " WHERE fechapago >= ('01/01/" + QString::number ( anyo ) + "')::DATE + '@" + QString::number ( mes - 1 ) + " mon'::INTERVAL ";
        SQLQuery += " AND fechapago < ('01/01/" + QString::number ( anyo ) + "')::DATE + '@" + QString::number ( mes ) + " mon'::INTERVAL ";
        SQLQuery += " AND idproveedor = " + idproveedor + " AND previsionpago = false GROUP BY idproveedor";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery );
        if ( !cur->eof() ) {
            fitxersortidatxt += "$doc->oooSet(\"cell-loc\", " + QString::number ( mes + 3 ) + ", " + QString::number ( row ) + ");\n";
            fitxersortidatxt += "$doc->oooSet(\"text-color\", \"00FF\");\n";
            fitxersortidatxt += "$doc->oooData(\"cell-text\", \"" + cur->value( "total" ) + "\");\n";
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
    
    return fitxersortidatxt;
}

