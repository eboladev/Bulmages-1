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

#include "proyectoview.h"
#include "empresa.h"
#include "configuracion.h"
#include "plugins.h"
#include "funcaux.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
/**
\param comp
\param parent
**/
ProyectoView::ProyectoView ( Empresa *comp, QWidget *parent )
        : FichaBc ( comp, parent )
{
    _depura ( "ProyectoView::ProyectoView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        /// Lanzamos los plugins.
        if ( g_plugins->lanza ( "ProyectoView_ProyectoView", this ) ) return;

        setTitleName ( tr ( "Proyecto" ) );
        setDBTableName ( "presupuestoc" );
        setDBCampoId ( "idpresupuestoc" );
        addDBCampo ( "idpresupuestoc", DBCampo::DBint, DBCampo::DBPrimaryKey, tr ( "ID almacen" ) );
        addDBCampo ( "idc_coste", DBCampo::DBint, DBCampo::DBNotNull, tr ( "Id CCoste" ) );
        addDBCampo ( "nompresupuestoc", DBCampo::DBvarchar, DBCampo::DBNotNull, tr ( "Nom Presupuesto" ) );
        addDBCampo ( "fechapresupuestoc", DBCampo::DBdate, DBCampo::DBNothing, tr ( "fecha" ) );
        addDBCampo ( "comentpresupuestoc", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Comentarios" ) );

        /// Inicializamos componentes.
        mui_idc_coste->setEmpresaBase ( comp );
        mui_idc_coste->setidc_coste ( "" );
        mui_ingresos->setEmpresaBase ( comp );
        mui_gastos->setEmpresaBase ( comp );

        /// Inicializamos el listado.
        mui_ingresos->setDBTableName ( "lingpresupuestoc" );
        mui_ingresos->setDBCampoId ( "idlingpresupuestoc" );
        mui_ingresos->addSHeader ( "idcuenta", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNoView, tr ( "Id. Cuenta" ) );
        mui_ingresos->addSHeader ( "codigo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr ( "Codigo" ) );
        mui_ingresos->addSHeader ( "descripcioncuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr ( "Cuenta" ) );
        mui_ingresos->addSHeader ( "cantlingpresupuestoc", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr ( "Cantidad" ) );
        mui_ingresos->addSHeader ( "precioundlingpresupuestoc", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr ( "Precio Und" ) );
        mui_ingresos->addSHeader ( "ejerciciolingpresupuestoc", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNone, tr ( "Ejercicio" ) );
        mui_ingresos->addSHeader ( "comentlingpresupuestoc", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone, tr ( "Comentarios" ) );
        mui_ingresos->addSHeader ( "desclingpresupuestoc", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone, tr ( "Descripcion" ) );
        mui_ingresos->addSHeader ( "idlingpresupuestoc", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView , tr ( "Id linea de Ingreso Presupuesto" ) );
        mui_ingresos->addSHeader ( "idpresupuestoc", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr ( "Id. Proyecto" ) );
        mui_ingresos->addSHeader ( "ordenlingpresupuestoc", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNoView, tr ( "Orden" ) );
        mui_ingresos->setinsercion ( TRUE );
        mui_ingresos->setOrdenEnabled ( TRUE );
        mui_ingresos->setOrdenPorQuery ( FALSE );


        /// Inicializamos el listado.
        mui_gastos->setDBTableName ( "lgaspresupuestoc" );
        mui_gastos->setDBCampoId ( "idlgaspresupuestoc" );
        mui_gastos->addSHeader ( "idcuenta", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNoView, tr ( "Id. Cuenta" ) );
        mui_gastos->addSHeader ( "codigo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr ( "Codigo" ) );
        mui_gastos->addSHeader ( "descripcioncuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr ( "Cuenta" ) );
        mui_gastos->addSHeader ( "cantlgaspresupuestoc", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr ( "Cantidad" ) );
        mui_gastos->addSHeader ( "precioundlgaspresupuestoc", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr ( "Precio Und" ) );
        mui_gastos->addSHeader ( "ejerciciolgaspresupuestoc", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNone, tr ( "Ejercicio" ) );
        mui_gastos->addSHeader ( "comentlgaspresupuestoc", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone, tr ( "Comentarios" ) );
        mui_gastos->addSHeader ( "desclgaspresupuestoc", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone, tr ( "Descripcion" ) );
        mui_gastos->addSHeader ( "idlgaspresupuestoc", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView , tr ( "Id linea de Ingreso Presupuesto" ) );
        mui_gastos->addSHeader ( "idpresupuestoc", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr ( "Id. Proyecto" ) );
        mui_gastos->addSHeader ( "ordenlgaspresupuestoc", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNoView, tr ( "Orden" ) );
        mui_gastos->setinsercion ( TRUE );
        mui_gastos->setOrdenEnabled ( TRUE );
        mui_gastos->setOrdenPorQuery ( FALSE );


        dialogChanges_cargaInicial();
        meteWindow ( windowTitle(), this, FALSE );
    } catch ( ... ) {
        mensajeInfo ( tr ( "Error al crear el almacen" ) );
    } // end try
    _depura ( "END ProyectoView::ProyectoView", 0 );
}


/// No precisa acciones adicionales en el destructor.

/**
**/
ProyectoView::~ProyectoView()
{
    _depura ( "ProyectoView::~ProyectoView", 0 );
    _depura ( "END ProyectoView::~ProyectoView", 0 );
}



///
/**
\param idcuadrante
\return
**/
int ProyectoView::cargarPost ( QString idcuadrante )
{
    mui_ingresos->cargar ( "SELECT idlingpresupuestoc, idpresupuestoc, ordenlingpresupuestoc, lingpresupuestoc.idcuenta, cantlingpresupuestoc, precioundlingpresupuestoc, ejerciciolingpresupuestoc, comentlingpresupuestoc, desclingpresupuestoc, t1.codigo, t1.descripcion AS descripcioncuenta FROM lingpresupuestoc LEFT JOIN (SELECT *  FROM cuenta) AS t1 ON lingpresupuestoc.idcuenta = t1.idcuenta WHERE idpresupuestoc=" + DBvalue ( "idpresupuestoc" ) );
    mui_gastos->cargar ( "SELECT idlgaspresupuestoc, idpresupuestoc, ordenlgaspresupuestoc, lgaspresupuestoc.idcuenta, cantlgaspresupuestoc, precioundlgaspresupuestoc, ejerciciolgaspresupuestoc, comentlgaspresupuestoc, desclgaspresupuestoc, t1.codigo, t1.descripcion AS descripcioncuenta  FROM lgaspresupuestoc LEFT JOIN (SELECT *  FROM cuenta) AS t1 ON lgaspresupuestoc.idcuenta = t1.idcuenta WHERE idpresupuestoc=" + DBvalue ( "idpresupuestoc" ) );
    return 0;
}


///
/**
\return
**/
int ProyectoView::guardarPost()
{
    mui_ingresos->setColumnValue ( "idpresupuestoc", DBvalue ( "idpresupuestoc" ) );
    mui_ingresos->guardar();
    mui_gastos->setColumnValue ( "idpresupuestoc", DBvalue ( "idpresupuestoc" ) );
    mui_gastos->guardar();
    emit ( save() );
    return 0;
}


///
/**
\return
**/
void ProyectoView::on_mui_geninforme_clicked()
{
    int row = 1;

    QString archivod = confpr->valor ( CONF_DIR_USER ) + "informeproyectoc.py";
    QString fitxersortidatxt = "";

    fitxersortidatxt += "#!/usr/bin/python\n";
    fitxersortidatxt += "# -*- coding: utf8 -*-\n";
    fitxersortidatxt += "\n";
    fitxersortidatxt += "import ooolib\n";
    fitxersortidatxt += "\n";

    fitxersortidatxt += "# Crea el documento\n";
    fitxersortidatxt += "doc = ooolib.Calc(\"Listado\")\n";



//    fitxersortidatxt += "doc.set_cell_value(" + QString::number ( x++ ) + "," + QString::number ( y ) + ", 'float' , '" + textocontenido + "')\n\n";

//    fitxersortidatxt += "doc.set_cell_value(" + QString::number ( x++ ) + "," + QString::number ( y ) + ", 'string', '" + textocontenido + "')\n\n";

    /// Buscamos los ejercicios involucrados en el presupuesto
    QString queryej = "SELECT distinct ej from (select ejerciciolgaspresupuestoc AS ej FROM lgaspresupuestoc WHERE idpresupuestoc = " + DBvalue ( "idpresupuestoc" ) + ")AS t1  UNION  (select ejerciciolingpresupuestoc as ej FROM lingpresupuestoc WHERE idpresupuestoc = " + DBvalue ( "idpresupuestoc" ) + ")";
    cursor2 *curej = empresaBase()->cargacursor ( queryej );
    while ( !curej->eof() ) {
        fitxersortidatxt += "doc.set_cell_value( 1 ," + QString::number ( row++ ) + ", 'float' , '" + curej->valor ( "ej" ) + "')\n\n";

        QString querycta = "SELECT distinct idcuenta from ( SELECT idcuenta FROM lgaspresupuestoc WHERE idpresupuestoc = " + DBvalue ( "idpresupuestoc" ) + " AND ejerciciolgaspresupuestoc = " + curej->valor ( "ej" ) + " ) AS t1 UNION (SELECT idcuenta FROM lingpresupuestoc WHERE idpresupuestoc = " + DBvalue ( "idpresupuestoc" ) + " AND ejerciciolingpresupuestoc = " + curej->valor ( "ej" ) + " ) UNION (SELECT idcuenta FROM apunte WHERE fecha >= '01/01/" + curej->valor ( "ej" ) + "' AND fecha <= '31/12/" + curej->valor ( "ej" ) + "' AND idc_coste = " + DBvalue ( "idc_coste" ) + " )";
        cursor2 *curcta = empresaBase()->cargacursor ( querycta );
        while ( !curcta->eof() ) {

            fitxersortidatxt += "doc.set_cell_value( 1 ," + QString::number ( row ) + ", 'float' , '" + curcta->valor ( "idcuenta" ) + "')\n\n";

            /// Ponemos el detalle de las cuentas para los datos contables.
            QString queryinc = "SELECT sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte WHERE idcuenta = " + curcta->valor ( "idcuenta" ) + " AND fecha >= '01/01/" + curej->valor ( "ej" ) + "' AND fecha <= '31/12/" + curej->valor ( "ej" ) + "' AND idc_coste = " + DBvalue ( "idc_coste" );
            cursor2 * curing = empresaBase()->cargacursor ( queryinc );
            while ( !curing->eof() ) {
                fitxersortidatxt += "doc.set_cell_value( 2 ," + QString::number ( row ) + ", 'float' , '" + curing->valor ( "tdebe" ) + "')\n\n";
                fitxersortidatxt += "doc.set_cell_value( 3 ," + QString::number ( row ) + ", 'float' , '" + curing->valor ( "thaber" ) + "')\n\n";
                fitxersortidatxt += "doc.set_cell_value( 4 ," + QString::number ( row ) + ", 'float' , '" + curing->valor ( "tdebe" ) + "-" + curing->valor ( "thaber" ) + "')\n\n";
                curing->siguienteregistro();
            } // end while
            delete curing;

            QString queryling = "SELECT * FROM lingpresupuestoc WHERE idcuenta = " + curcta->valor ( "idcuenta" ) + " AND idpresupuestoc=" + DBvalue ( "idpresupuestoc" ) + " AND ejerciciolingpresupuestoc = " + curej->valor ( "ej" );
            cursor2 *curling = empresaBase()->cargacursor ( queryling );
            while ( !curling->eof() ) {
                fitxersortidatxt += "doc.set_cell_value( 5 ," + QString::number ( row ) + ", 'float' , '" + curling->valor ( "cantlingpresupuestoc" ) + "*" + curling->valor ( "precioundlingpresupuestoc" ) + "')\n\n";
                curling->siguienteregistro();
            } // end while
            delete curling;

            QString querylgas = "SELECT * FROM lgaspresupuestoc WHERE idcuenta = " + curcta->valor ( "idcuenta" ) + " AND idpresupuestoc=" + DBvalue ( "idpresupuestoc" ) + " AND ejerciciolgaspresupuestoc = " + curej->valor ( "ej" );
            cursor2 *curlgas = empresaBase()->cargacursor ( querylgas );
            while ( !curlgas->eof() ) {
                fitxersortidatxt += "doc.set_cell_value( 6 ," + QString::number ( row ) + ", 'float' , '" + curlgas->valor ( "cantlgaspresupuestoc" ) + "*" + curlgas->valor ( "precioundlgaspresupuestoc" ) + "')\n\n";
                curlgas->siguienteregistro();
            } // end while
            delete curlgas;

            row++;
            curcta->siguienteregistro();
        } // end while
        delete curcta;
        curej->siguienteregistro();

        row += 10;
    } // end while
    delete curej;


    fitxersortidatxt += "doc.save(\"informeproyectoc.ods\")\n";

    QString cadena = "rm " + confpr->valor ( CONF_DIR_USER ) + "informeproyectoc.ods";
    system ( cadena.toAscii() );
    cadena = "rm " + archivod;
    system ( cadena.toAscii() );

    QFile file ( archivod );
    if ( file.open ( QIODevice::WriteOnly ) )  {
        QTextStream stream ( &file );
        stream.setCodec ( "UTF-8" );
        stream << fitxersortidatxt;
        file.close();
    } // end if

    cadena = " cd " + confpr->valor ( CONF_DIR_USER ) + "; python " + archivod;
    system ( cadena.toAscii() );
    cadena = "oocalc " + confpr->valor ( CONF_DIR_USER ) + "informeproyectoc.ods &";
    system ( cadena.toAscii() );

}

