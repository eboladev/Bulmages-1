/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#include <QtWidgets/QMessageBox>
#include <QtGui/QCloseEvent>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QDrag>
#include <QtWidgets/QMenu>

#include "cuadranteview.h"
#include "cuadrante1view.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "blplugins.h"
#include "blfunctions.h"
#include "blprogressbar.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
/**
\param comp
\param parent
**/
CuadranteView::CuadranteView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        /// Lanzamos los plugins.
        if ( g_plugins->run ( "CuadranteView_CuadranteView", this ) ) return;

        setTitleName ( _ ( "Almacen" ) );
        setDbTableName ( "almacen" );

        mui_listtrabajadores->setDragEnabled ( true );
        mui_cuadrante->setAcceptDrops ( true );
// mui_cuadrante->verticalHeader()->hide();

        inicializaTrabajadores();
        inicializaCuadrante ( QDate::currentDate() );

        insertWindow ( windowTitle(), this, false );
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el almacen" ) );
    } // end try
    
}


/** No precisa acciones adicionales en el destructor.
*/
///
/**
**/
CuadranteView::~CuadranteView()
{
    BL_FUNC_DEBUG
    saveConfig();
    
}


/** Inicializa la seleccion de trabajadores
*/
/**
**/
void CuadranteView::inicializaTrabajadores()
{
    BL_FUNC_DEBUG
    mui_listtrabajadores->clear();
    mui_listtrabajadores->setColumnCount ( 2 );
    mui_listtrabajadores->hideColumn ( 1 );
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM tipotrabajo" );
    if ( cur ) {
        while ( !cur->eof() ) {
            QTreeWidgetItem * it = new QTreeWidgetItem ( mui_listtrabajadores );
            it->setText ( 0, cur->value( "nomtipotrabajo" ) );
            it->setTextColor ( 0, QColor ( "#FF0000" ) );

            /// Cargamos los trabajadores correspondientes a este tipo de trabajo.
            BlDbRecordSet *curtrab = mainCompany() ->loadQuery ( "SELECT * FROM trabajador WHERE idtipotrabajo = " + cur->value( "idtipotrabajo" ) + "ORDER BY nomtrabajador, apellidostrabajador" );
            if ( curtrab ) {
                while ( !curtrab->eof() ) {
                    QTreeWidgetItem * itt = new QTreeWidgetItem ( it );
                    itt->setText ( 0, curtrab->value( "nomtrabajador" ) + " " + curtrab->value( "apellidostrabajador" ) );
                    itt->setText ( 1, curtrab->value( "idtrabajador" ) );
                    itt->setTextColor ( 0, QColor ( "#000000" ) );
                    curtrab->nextRecord();
                } // end while
                delete curtrab;
            } // end if

            cur->nextRecord();
        } // end if
        delete cur;
    } // end if
    
}


/** Inicializa la seleccion de trabajadores
*/
/**
\param dateorig
\return
**/
void CuadranteView::inicializaCuadrante ( const QDate &dateorig )
{
    BL_FUNC_DEBUG
    try {
        /// Si el cuadrante ha sido manipulado guardamos las configuracion del mismo.
        if ( mui_cuadrante->rowCount() != 0 )
            saveConfig();

        mui_cuadrante->clear();

        BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT idalmacen, tipoalmacen, nomalmacen FROM almacen ORDER BY tipoalmacen DESC, nomalmacen" );
        if ( !cur ) throw - 1;


        mui_cuadrante->setRowCount ( cur->numregistros() );
        mui_cuadrante->setColumnCount ( 7 );

        QStringList headers;
        headers << _ ( "Lunes" ) << _ ( "Martes" ) << _ ( "Miercoles" ) << _ ( "Jueves" ) << _ ( "Viernes" ) << _ ( "Sabado" ) << _ ( "Domingo" );
        mui_cuadrante->setHorizontalHeaderLabels ( headers );


        int row = 0;
        BlProgressBar barra;
        barra.setRange ( 0, cur->numregistros() );
        barra.show();
        barra.setText ( _ ( "Generando Informe" ) );

        while ( !cur->eof() ) {

            /// Establecemos la altura de los campos.
            mui_cuadrante->setRowHeight ( row, 100 );

            /// Buscamos el Lunes de la Semana
            QDate date = dateorig.addDays ( -dateorig.dayOfWeek() + 1 );

            for ( int column = 0; column < 7; column ++ ) {

                mui_cuadrante->setColumnWidth ( column, 250 );

                CuadranteQTextDocument *newItem = new CuadranteQTextDocument ( mainCompany(), mui_cuadrante );

                newItem->setAlmFecha ( cur->value( "idalmacen" ), date );

                mui_cuadrante->setCellWidget ( row, column, newItem );

                date = date.addDays ( 1 );
            } // end for
            cur->nextRecord();
            row++;
            barra.setValue ( row );
        } // end while
        loadConfig();
    } catch ( ... ) {
	
        return;
    } // end try
}


///
/**
\param date
**/
void CuadranteView::on_mui_calendario_clicked ( const QDate &date )
{
    BL_FUNC_DEBUG
    inicializaCuadrante ( date );
    
}


///
/**
\param item
**/
void CuadranteView::on_mui_listtrabajadores_itemDoubleClicked ( QTreeWidgetItem *item, int )
{
    BL_FUNC_DEBUG
    QString item1 = item->text ( 1 );

    QList<QTableWidgetSelectionRange> selectionranges = mui_cuadrante->selectedRanges();
    for ( int i = 0; i < selectionranges.size(); ++i ) {
        for ( int x = selectionranges.at ( i ).topRow(); x <= selectionranges.at ( i ).bottomRow(); x++ ) {
            for ( int y = selectionranges.at ( i ).leftColumn(); y <= selectionranges.at ( i ).rightColumn(); y++ ) {
                CuadranteQTextDocument *newItem = ( CuadranteQTextDocument * ) mui_cuadrante->cellWidget ( x, y );
                newItem->addTrabajador ( item1 );
            } // end for
        } // end for
    }
}


///
/**
**/
void CuadranteView::on_mui_editar_clicked()
{
    BL_FUNC_DEBUG
    Cuadrante1View *cuad = new Cuadrante1View ( mainCompany(), 0 );
    mainCompany() ->pWorkspace() ->addSubWindow ( cuad );
    cuad->show();
    CuadranteQTextDocument *newItem = ( CuadranteQTextDocument * ) mui_cuadrante->cellWidget ( mui_cuadrante->currentRow(), mui_cuadrante->currentColumn() );
    newItem->connect ( cuad, SIGNAL ( saved() ), newItem, SLOT ( refresh() ) );
    cuad->load ( newItem->idcuadrante() );
}


///
/**
\param pos
**/
void CuadranteView::on_mui_calendario_customContextMenuRequested ( const QPoint & pos )
{
    BL_FUNC_DEBUG
    QMenu *popup = new QMenu ( mui_calendario );

    popup->addSeparator();
    QAction *norm = popup->addAction ( _ ( "Normal" ) );
    QAction *fiesta = popup->addAction ( _ ( "Fiesta general" ) );

    QAction *opcion = popup->exec ( mapToGlobal ( pos ) );
    if ( opcion == norm ) {
        mainCompany() ->begin();
        mainCompany() ->runQuery ( "UPDATE CUADRANTE SET fiestacuadrante = false WHERE fechacuadrante = '" + mui_calendario->selectedDate().toString ( "dd/MM/yyyy" ) + "'" );
        mainCompany() ->commit();
    } // end if

    if ( opcion == fiesta ) {
        mainCompany() ->begin();
        mainCompany() ->runQuery ( "UPDATE CUADRANTE SET fiestacuadrante = true WHERE fechacuadrante = '" + mui_calendario->selectedDate().toString ( "dd/MM/yyyy" ) + "'" );
        mainCompany() ->commit();
    } // end if

    inicializaCuadrante ( mui_calendario->selectedDate() );
    
}


///
/**
**/
void CuadranteView::on_mui_actualizar_clicked()
{
    BL_FUNC_DEBUG
    inicializaTrabajadores();
    inicializaCuadrante ( mui_calendario->selectedDate() );
    
}


///
/**
**/
void CuadranteView::on_mui_limpiar_clicked()
{
    BL_FUNC_DEBUG
    QDate date = mui_calendario->selectedDate().addDays ( -mui_calendario->selectedDate().dayOfWeek() + 1 );
    QDate datefin = date.addDays ( 6 );
    QString query = "DELETE FROM horario WHERE idcuadrante IN (SELECT idcuadrante FROM cuadrante WHERE fechacuadrante >= '" + date.toString ( "dd/MM/yyyy" ) + "' AND fechacuadrante <='" + datefin.toString ( "dd/MM/yyyy" ) + "')";
    mainCompany() ->runQuery ( query );
    query = "DELETE FROM cuadrante WHERE fechacuadrante >= '" + date.toString ( "dd/MM/yyyy" ) + "' AND fechacuadrante <='" + datefin.toString ( "dd/MM/yyyy" ) + "'";
    mainCompany() ->runQuery ( query );
    on_mui_actualizar_clicked();
    
}



///
/**
**/
void CuadranteView::on_mui_duplicar_clicked()
{
    BL_FUNC_DEBUG
    for ( QDate date = mui_calendario->selectedDate().addDays ( -mui_calendario->selectedDate().dayOfWeek() + 1 )
                       ; date <= mui_calendario->selectedDate().addDays ( -mui_calendario->selectedDate().dayOfWeek() + 7 )
            ; date = date.addDays ( 1 ) ) {
        QDate fechaant = date.addDays ( -7 );
        QString query = "SELECT * FROM cuadrante WHERE fechacuadrante = '" + fechaant.toString ( "dd/MM/yyyy" ) + "'";
	BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("Consulta: '%1'.")).arg(query) );
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
        while ( !cur->eof() ) {
            query = "UPDATE cuadrante SET ";
            query += " comentcuadrante = '" + cur->value( "comentcuadrante" ) + "'";
            query += ", aperturacuadrante = " + ( ( cur->value( "aperturacuadrante" ) == "" ) ? "NULL" : "'" + cur->value( "aperturacuadrante" ) + "'" );
            query += ", cierrecuadrante = " + ( ( cur->value( "cierrecuadrante" ) == "" ) ? "NULL" : "'" + cur->value( "cierrecuadrante" ) + "'" );
            query += ", apertura1cuadrante = " + ( ( cur->value( "apertura1cuadrante" ) == "" ) ? "NULL" : "'" + cur->value( "apertura1cuadrante" ) + "'" );
            query += ", cierre1cuadrante = " + ( ( cur->value( "cierre1cuadrante" ) == "" ) ? "NULL" : "'" + cur->value( "cierre1cuadrante" ) + "'" );
            query += ", fiestacuadrante = '" + cur->value( "fiestacuadrante" ) + "'";
            query += " WHERE fechacuadrante = '" + date.toString ( "dd/MM/yyyy" ) + "' AND idalmacen = " + cur->value( "idalmacen" );
            mainCompany() ->runQuery ( query );

            BlDbRecordSet *cur1 = mainCompany() ->loadQuery ( "SELECT * FROM cuadrante WHERE fechacuadrante = '" + date.toString ( "dd/MM/yyyy" ) + "' AND idalmacen = " + cur->value( "idalmacen" ) );
            QString idcuadrante = cur1->value( "idcuadrante" );
            delete cur1;

            BlDbRecordSet *cur2 = mainCompany() ->loadQuery ( "SELECT * FROM horario WHERE idcuadrante=" + cur->value( "idcuadrante" ) );
            while ( !cur2->eof() ) {
                query = "INSERT INTO horario (idtrabajador, idcuadrante, horainhorario, horafinhorario) VALUES (";
                query += cur2->value( "idtrabajador" );
                query += "," + idcuadrante;
                query += ",'" + cur2->value( "horainhorario" ) + "'";
                query += ",'" + cur2->value( "horafinhorario" ) + "'";
                query += ")";
                mainCompany() ->runQuery ( query );
                cur2->nextRecord();
            } // end while
            delete cur2;

            cur->nextRecord();
        } // end while
        delete cur;


    } // end for
    on_mui_actualizar_clicked();
    
}


///
/**
**/
void CuadranteView::on_mui_imprimir_clicked()
{
    BL_FUNC_DEBUG

    /// Copiamos el archivo.
    QString archivo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "cuadrante.rml";
    QString archivod = g_confpr->value( CONF_DIR_USER ) + "cuadrante.rml";
    blCopyFile(archivo, archivod);
    
    /// Copiamos el logo
    QString archivologo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "logo.jpg";
    QString logousuario = g_confpr->value( CONF_DIR_USER ) + "logo.jpg";
    blCopyFile(archivologo, logousuario);

    QFile file;
    file.setFileName ( archivod );
    file.open ( QIODevice::ReadOnly );
    QTextStream stream ( &file );
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt = "";


    ///Ponemos la tabla
    fitxersortidatxt += "<para>Cuadrante</para>\n";
    fitxersortidatxt += "<blockTable style=\"tablacontenido\" colWidths=\"6cm, 6cm, 6cm, 6cm, 6cm, 6cm, 4cm\" repeatRows=\"1\">\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "    <td>Lunes</td>\n";
    fitxersortidatxt += "    <td>Martes</td>\n";
    fitxersortidatxt += "    <td>Miercoles</td>\n";
    fitxersortidatxt += "    <td>Jueves</td>\n";
    fitxersortidatxt += "    <td>Viernes</td>\n";
    fitxersortidatxt += "    <td>Sabado</td>\n";
    fitxersortidatxt += "    <td>Domingo</td>\n";
    fitxersortidatxt += "</tr>\n";


// ================================================================

    /// Buscamos el Lunes de la Semana
//    QDate date = mui_calendario->selectedDate().addDays(-mui_calendario->selectedDate().dayOfWeek() + 1);

    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT idalmacen FROM almacen" );
    if ( !cur ) throw - 1;

    mui_cuadrante->setRowCount ( cur->numregistros() );
    mui_cuadrante->setColumnCount ( 7 );

    int row = 0;
    while ( !cur->eof() ) {
        fitxersortidatxt += "<tr>\n";
        for ( int column = 0; column < 7; column ++ ) {
            CuadranteQTextDocument *newItem = ( CuadranteQTextDocument * ) mui_cuadrante->cellWidget ( row, column );
            fitxersortidatxt += newItem->impresion();
        } // end for
        cur->nextRecord();
        row++;
        fitxersortidatxt += "</tr>\n";
    } // end while

    fitxersortidatxt += "</blockTable>\n";

    buff.replace ( "[story]", fitxersortidatxt );

    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << buff;
        file.close();
    } // end if
    blCreateAndLoadPDF ( "cuadrante" );
    

}


/// Guardamos el archivo de configuracion.
/**
**/
void CuadranteView::saveConfig()
{
    BL_FUNC_DEBUG
    QString aux = "";
    QFile file ( g_confpr->value( CONF_DIR_USER ) + "cuadrantecfn.cfn" );
    /// Guardado del orden y de configuraciones varias.
    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );

        /// Guardado del ancho y alto de las columnas
        for ( int i = 0; i < mui_cuadrante->columnCount(); i++ ) {
            stream << mui_cuadrante->columnWidth ( i ) << "\n";
        } // end for

        /// Guardado del ancho y alto de las columnas
        for ( int i = 0; i < mui_cuadrante->rowCount(); i++ ) {
            stream << mui_cuadrante->rowHeight ( i ) << "\n";
        } // end for

        file.close();
    } // end if
    
}


///
/**
**/
void CuadranteView::loadConfig()
{
    BL_FUNC_DEBUG
    QFile file ( g_confpr->value( CONF_DIR_USER ) + "cuadrantecfn.cfn" );
    QString line;
    int error = 1;
    if ( file.open ( QIODevice::ReadOnly ) ) {
        error = 0;
        QTextStream stream ( &file );
        /// Establecemos la columna de ordenaci&oacute;n
        QString linea = "";


        /// Establecemos el ancho de las columnas.
        for ( int i = 0; i < mui_cuadrante->columnCount(); i++ ) {
            linea = stream.readLine();
            if ( linea.toInt() > 0 ) {
                mui_cuadrante->setColumnWidth ( i, linea.toInt() );
            } else {
                mui_cuadrante->setColumnWidth ( i, 400 );
                error = 1;
            } // end if
        } // end for

        /// Establecemos el ancho de las columnas.
        for ( int i = 0; i < mui_cuadrante->rowCount(); i++ ) {
            linea = stream.readLine();
            if ( linea.toInt() > 0 ) {
                mui_cuadrante->setRowHeight ( i, linea.toInt() );
            } else {
                mui_cuadrante->setRowHeight ( i, 250 );
                error = 1;
            } // end if
        } // end for
    } // end if
    /// Si se ha producido alg&uacute;n error en la carga hacemos un maquetado autom&aacute;tico.
    if ( error ) {
        mui_cuadrante->resizeColumnsToContents();
        mui_cuadrante->resizeRowsToContents();
    }

    file.close();
    
}

