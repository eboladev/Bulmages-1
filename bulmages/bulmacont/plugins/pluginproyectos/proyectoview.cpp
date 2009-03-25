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
#include "bccompany.h"
#include "blconfiguration.h"
#include "blplugins.h"
#include "blfunctions.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
/**
\param comp
\param parent
**/
ProyectoView::ProyectoView ( BcCompany *comp, QWidget *parent )
        : BcForm ( comp, parent )
{
    _depura ( "ProyectoView::ProyectoView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        /// Lanzamos los plugins.
        if ( g_plugins->lanza ( "ProyectoView_ProyectoView", this ) ) return;

        setTitleName ( _( "Proyecto" ) );
        setDbTableName ( "presupuestoc" );
        setDbFieldId ( "idpresupuestoc" );
        addDbField ( "idpresupuestoc", BlDbField::DbInt, BlDbField::DbPrimaryKey, _( "ID almacen" ) );
        addDbField ( "idc_coste", BlDbField::DbInt, BlDbField::DbNotNull, _( "Id CCoste" ) );
        addDbField ( "nompresupuestoc", BlDbField::DbVarChar, BlDbField::DbNotNull, _( "Nom Presupuesto" ) );
        addDbField ( "fechapresupuestoc", BlDbField::DbDate, BlDbField::DbNothing, _( "fecha" ) );
        addDbField ( "comentpresupuestoc", BlDbField::DbVarChar, BlDbField::DbNothing, _( "Comentarios" ) );

        /// Inicializamos componentes.
        mui_idc_coste->setMainCompany ( comp );
        mui_idc_coste->setidc_coste ( "" );
        mui_ingresos->setMainCompany ( comp );
        mui_gastos->setMainCompany ( comp );

        /// Inicializamos el listado.
        mui_ingresos->setDbTableName ( "lingpresupuestoc" );
        mui_ingresos->setDbFieldId ( "idlingpresupuestoc" );
        mui_ingresos->addSubFormHeader ( "idcuenta", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView, _( "Id. Cuenta" ) );
        mui_ingresos->addSubFormHeader ( "codigo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _( "Codigo" ) );
        mui_ingresos->addSubFormHeader ( "descripcioncuenta", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _( "Cuenta" ) );
        mui_ingresos->addSubFormHeader ( "cantlingpresupuestoc", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _( "Cantidad" ) );
        mui_ingresos->addSubFormHeader ( "precioundlingpresupuestoc", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _( "Precio Und" ) );
        mui_ingresos->addSubFormHeader ( "ejerciciolingpresupuestoc", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbNone, _( "Ejercicio" ) );
        mui_ingresos->addSubFormHeader ( "comentlingpresupuestoc", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _( "Comentarios" ) );
        mui_ingresos->addSubFormHeader ( "desclingpresupuestoc", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _( "Descripcion" ) );
        mui_ingresos->addSubFormHeader ( "idlingpresupuestoc", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView , _( "Id linea de Ingreso Presupuesto" ) );
        mui_ingresos->addSubFormHeader ( "idpresupuestoc", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView, _( "Id. Proyecto" ) );
        mui_ingresos->addSubFormHeader ( "ordenlingpresupuestoc", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView, _( "Orden" ) );
        mui_ingresos->setinsercion ( TRUE );
        mui_ingresos->setOrdenEnabled ( TRUE );
        mui_ingresos->setOrdenPorQuery ( FALSE );


        /// Inicializamos el listado.
        mui_gastos->setDbTableName ( "lgaspresupuestoc" );
        mui_gastos->setDbFieldId ( "idlgaspresupuestoc" );
        mui_gastos->addSubFormHeader ( "idcuenta", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView, _( "Id. Cuenta" ) );
        mui_gastos->addSubFormHeader ( "codigo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _( "Codigo" ) );
        mui_gastos->addSubFormHeader ( "descripcioncuenta", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _( "Cuenta" ) );
        mui_gastos->addSubFormHeader ( "cantlgaspresupuestoc", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _( "Cantidad" ) );
        mui_gastos->addSubFormHeader ( "precioundlgaspresupuestoc", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _( "Precio Und" ) );
        mui_gastos->addSubFormHeader ( "ejerciciolgaspresupuestoc", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbNone, _( "Ejercicio" ) );
        mui_gastos->addSubFormHeader ( "comentlgaspresupuestoc", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _( "Comentarios" ) );
        mui_gastos->addSubFormHeader ( "desclgaspresupuestoc", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _( "Descripcion" ) );
        mui_gastos->addSubFormHeader ( "idlgaspresupuestoc", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView , _( "Id linea de Ingreso Presupuesto" ) );
        mui_gastos->addSubFormHeader ( "idpresupuestoc", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView, _( "Id. Proyecto" ) );
        mui_gastos->addSubFormHeader ( "ordenlgaspresupuestoc", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView, _( "Orden" ) );
        mui_gastos->setinsercion ( TRUE );
        mui_gastos->setOrdenEnabled ( TRUE );
        mui_gastos->setOrdenPorQuery ( FALSE );


        dialogChanges_cargaInicial();
        meteWindow ( windowTitle(), this, FALSE );
    } catch ( ... ) {
        mensajeInfo ( _( "Error al crear el almacen" ) );
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
    mui_ingresos->cargar ( "SELECT idlingpresupuestoc, idpresupuestoc, ordenlingpresupuestoc, lingpresupuestoc.idcuenta, cantlingpresupuestoc, precioundlingpresupuestoc, ejerciciolingpresupuestoc, comentlingpresupuestoc, desclingpresupuestoc, t1.codigo, t1.descripcion AS descripcioncuenta FROM lingpresupuestoc LEFT JOIN (SELECT *  FROM cuenta) AS t1 ON lingpresupuestoc.idcuenta = t1.idcuenta WHERE idpresupuestoc=" + dbValue ( "idpresupuestoc" ) );
    mui_gastos->cargar ( "SELECT idlgaspresupuestoc, idpresupuestoc, ordenlgaspresupuestoc, lgaspresupuestoc.idcuenta, cantlgaspresupuestoc, precioundlgaspresupuestoc, ejerciciolgaspresupuestoc, comentlgaspresupuestoc, desclgaspresupuestoc, t1.codigo, t1.descripcion AS descripcioncuenta  FROM lgaspresupuestoc LEFT JOIN (SELECT *  FROM cuenta) AS t1 ON lgaspresupuestoc.idcuenta = t1.idcuenta WHERE idpresupuestoc=" + dbValue ( "idpresupuestoc" ) );
    return 0;
}


///
/**
\return
**/
int ProyectoView::guardarPost()
{
    mui_ingresos->setColumnValue ( "idpresupuestoc", dbValue ( "idpresupuestoc" ) );
    mui_ingresos->guardar();
    mui_gastos->setColumnValue ( "idpresupuestoc", dbValue ( "idpresupuestoc" ) );
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
    QString queryej = "SELECT distinct ej from (select ejerciciolgaspresupuestoc AS ej FROM lgaspresupuestoc WHERE idpresupuestoc = " + dbValue ( "idpresupuestoc" ) + ")AS t1  UNION  (select ejerciciolingpresupuestoc as ej FROM lingpresupuestoc WHERE idpresupuestoc = " + dbValue ( "idpresupuestoc" ) + ")";
    BlDbRecordSet *curej = mainCompany()->loadQuery ( queryej );
    while ( !curej->eof() ) {
        fitxersortidatxt += "doc.set_cell_value( 1 ," + QString::number ( row++ ) + ", 'float' , '" + curej->valor ( "ej" ) + "')\n\n";

        QString querycta = "SELECT distinct idcuenta from ( SELECT idcuenta FROM lgaspresupuestoc WHERE idpresupuestoc = " + dbValue ( "idpresupuestoc" ) + " AND ejerciciolgaspresupuestoc = " + curej->valor ( "ej" ) + " ) AS t1 UNION (SELECT idcuenta FROM lingpresupuestoc WHERE idpresupuestoc = " + dbValue ( "idpresupuestoc" ) + " AND ejerciciolingpresupuestoc = " + curej->valor ( "ej" ) + " ) UNION (SELECT idcuenta FROM apunte WHERE fecha >= '01/01/" + curej->valor ( "ej" ) + "' AND fecha <= '31/12/" + curej->valor ( "ej" ) + "' AND idc_coste = " + dbValue ( "idc_coste" ) + " )";
        BlDbRecordSet *curcta = mainCompany()->loadQuery ( querycta );
        while ( !curcta->eof() ) {

            fitxersortidatxt += "doc.set_cell_value( 1 ," + QString::number ( row ) + ", 'float' , '" + curcta->valor ( "idcuenta" ) + "')\n\n";

            /// Ponemos el detalle de las cuentas para los datos contables.
            QString queryinc = "SELECT sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte WHERE idcuenta = " + curcta->valor ( "idcuenta" ) + " AND fecha >= '01/01/" + curej->valor ( "ej" ) + "' AND fecha <= '31/12/" + curej->valor ( "ej" ) + "' AND idc_coste = " + dbValue ( "idc_coste" );
            BlDbRecordSet * curing = mainCompany()->loadQuery ( queryinc );
            while ( !curing->eof() ) {
                fitxersortidatxt += "doc.set_cell_value( 2 ," + QString::number ( row ) + ", 'float' , '" + curing->valor ( "tdebe" ) + "')\n\n";
                fitxersortidatxt += "doc.set_cell_value( 3 ," + QString::number ( row ) + ", 'float' , '" + curing->valor ( "thaber" ) + "')\n\n";
                fitxersortidatxt += "doc.set_cell_value( 4 ," + QString::number ( row ) + ", 'float' , '" + curing->valor ( "tdebe" ) + "-" + curing->valor ( "thaber" ) + "')\n\n";
                curing->nextRecord();
            } // end while
            delete curing;

            QString queryling = "SELECT * FROM lingpresupuestoc WHERE idcuenta = " + curcta->valor ( "idcuenta" ) + " AND idpresupuestoc=" + dbValue ( "idpresupuestoc" ) + " AND ejerciciolingpresupuestoc = " + curej->valor ( "ej" );
            BlDbRecordSet *curling = mainCompany()->loadQuery ( queryling );
            while ( !curling->eof() ) {
                fitxersortidatxt += "doc.set_cell_value( 5 ," + QString::number ( row ) + ", 'float' , '" + curling->valor ( "cantlingpresupuestoc" ) + "*" + curling->valor ( "precioundlingpresupuestoc" ) + "')\n\n";
                curling->nextRecord();
            } // end while
            delete curling;

            QString querylgas = "SELECT * FROM lgaspresupuestoc WHERE idcuenta = " + curcta->valor ( "idcuenta" ) + " AND idpresupuestoc=" + dbValue ( "idpresupuestoc" ) + " AND ejerciciolgaspresupuestoc = " + curej->valor ( "ej" );
            BlDbRecordSet *curlgas = mainCompany()->loadQuery ( querylgas );
            while ( !curlgas->eof() ) {
                fitxersortidatxt += "doc.set_cell_value( 6 ," + QString::number ( row ) + ", 'float' , '" + curlgas->valor ( "cantlgaspresupuestoc" ) + "*" + curlgas->valor ( "precioundlgaspresupuestoc" ) + "')\n\n";
                curlgas->nextRecord();
            } // end while
            delete curlgas;

            row++;
            curcta->nextRecord();
        } // end while
        delete curcta;
        curej->nextRecord();

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

