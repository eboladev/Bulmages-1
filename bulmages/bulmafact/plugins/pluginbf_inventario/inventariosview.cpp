/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include <QtWidgets/QMessageBox>
#include "blfile.h"
#include <QtCore/QTextStream>

#include "inventariosview.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "inventarioview.h"


///
/**
**/
void InventariosView::on_mui_listado_itemDoubleClicked ( QTableWidgetItem * )
{
    BL_FUNC_DEBUG
    on_mui_editar_clicked();
    
}


///
/**
\return
**/
void InventariosView::on_mui_crear_clicked()
{
    BL_FUNC_DEBUG
    InventarioView *bud = new InventarioView ( ( BfCompany * ) mainCompany(), 0 );
    /// No se hace la carga inicial. Se espera a que el usuario establezca los datos de filtrado.
    //if ( bud->load ( "" ) )
    //    return;
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->show();
    bud->mui_nominventario->setFocus();
    
}


///
/**
**/
void InventariosView::on_mui_listado_itemDoubleClicked()
{
    BL_FUNC_DEBUG
    on_mui_editar_clicked();
    
}


///
/**
**/
void InventariosView::presentar()
{
    BL_FUNC_DEBUG
    mui_listado->load();
    
}

///
/**
\param comp
\param parent
**/
InventariosView::InventariosView ( BfCompany *comp, QWidget *parent, Qt::WindowFlags flag, edmode editmode )
        : BlFormList ( comp, parent, flag, editmode )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_listado->setMainCompany ( comp );
    mui_listado->load();
    /// Si estamos en el modo edici&oacute;n metemos la ventana en el workSpace.
    if ( editMode() ) {
        mainCompany() ->insertWindow ( windowTitle(), this );
    } else {
        setWindowTitle ( _ ( "Selector de Inventarios" ) );
    } // end if
    /// Llamamos a los scripts
    blScript(this);
    
}


///
/**
**/
InventariosView::~InventariosView()
{
    BL_FUNC_DEBUG
    
}


///
/**
\return
**/
void InventariosView::on_mui_editar_clicked()
{
    BL_FUNC_DEBUG
    int a = mui_listado->currentRow();
    if ( a < 0 ) {
        blMsgInfo ( _ ( "Tiene que seleccionar un inventario" ) );
        return;
    } else {
        QString idinventario = mui_listado->dbValue ( "idinventario" );
        if ( idinventario != "" ) {
            InventarioView * bud = new InventarioView ( ( BfCompany * ) mainCompany(), 0 );
            if ( bud->load ( idinventario ) )
                return;
            mainCompany() ->m_pWorkspace->addSubWindow ( bud );
            bud->show();
            bud->mui_nominventario->setFocus();
            presentar();
        } // end if
    } // end if
    
}


///
/**
\return
**/
void InventariosView::on_mui_borrar2_clicked()
{
    BL_FUNC_DEBUG
    int a = mui_listado->currentRow();
    if ( a < 0 ) {
        blMsgInfo ( _ ( "Tiene que seleccionar un inventario" ) );
        return;
    } else {
        QString idinventario = mui_listado->dbValue ( "idinventario" );
        if ( idinventario != "" ) {
            InventarioView * inv = new InventarioView ( ( BfCompany * ) mainCompany(), 0 );
            mainCompany() ->m_pWorkspace->addSubWindow ( inv );
            inv->load ( idinventario );
            /// Hacemos el borrado sin mostrar pantalla ni nada.
            inv->on_mui_borrar2_clicked();
            mui_listado->load();
        } // end if
    } // end if
    presentar();
    
}


///
/**
**/
void InventariosView::on_mui_imprimir_clicked()
{
    BL_FUNC_DEBUG

    /// Copiamos el archivo.
    QString archivo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "listado.rml";
    QString archivod = g_confpr->value( CONF_DIR_USER ) + "listado.rml";
    blCopyFile(archivo, archivod);
    
    /// Copiamos el logo
    QString archivologo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "logo.jpg";
    QString logousuario = g_confpr->value( CONF_DIR_USER ) + "logo.jpg";
    blCopyFile(archivologo, logousuario);


    BlFile file;
    file.setFileName ( archivod );
    file.open ( QIODevice::ReadOnly );
    QTextStream stream ( &file );
    QString buff = stream.readAll();
    file.close();


    QString txt = "<blockTable style=\"tabla\" repeatRows=\"1\">\n";
    txt += "<tr>\n\t<td></td>\n";

    QString query = "SELECT * FROM articulo ";
    BlDbRecordSet *almacenes = mainCompany() ->loadQuery ( "SELECT * FROM almacen" );
    while ( !almacenes->eof() ) {
        QString idalmacen = almacenes->value( "idalmacen" );
        query += " LEFT JOIN ( SELECT stock AS stock" + idalmacen + ", idarticulo FROM stock_almacen WHERE idalmacen=" + almacenes->value( "idalmacen" ) + ") AS t" + idalmacen + " ON " + " t" + idalmacen + ".idarticulo = articulo.idarticulo";

        txt += "\t<td>" + almacenes->value( "nomalmacen" ) + "</td>\n";
        almacenes->nextRecord();
    } // end while
    txt += "</tr>\n";
    query += " WHERE articulo.stockarticulo <> 0";


    BlDbRecordSet *cstock = mainCompany() ->loadQuery ( query );
    while ( !cstock->eof() ) {
        txt += "<tr>\n";
        txt += "\t<td>" + cstock->value( "nomarticulo" ) + "</td>\n";
        almacenes->firstRecord();
        while ( !almacenes->eof() ) {
            txt += "\t<td>" + cstock->value( "stock" + almacenes->value( "idalmacen" ) ) + "</td>\n";
            almacenes->nextRecord();
        } // end while
        cstock->nextRecord();
        txt += "</tr>\n";
    } // end while
    delete cstock;
    delete almacenes;
    txt += "</blockTable>\n";

    buff.replace ( "[story]", txt );
    buff.replace ( "[titulo]", "Listado de Stocks" );


    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << buff;
        file.close();
    } // end if

    blCreateAndLoadPDF ( "listado" );
    
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================


///
/**
**/
void InventariosSubForm::load()
{
    BL_FUNC_DEBUG
    QString SQLQuery = "SELECT * FROM inventario";
    BlSubForm::load ( SQLQuery );
    
}


///
/**
\param parent
**/
InventariosSubForm::InventariosSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    BL_FUNC_DEBUG
    setDbTableName ( "inventario" );
    setDbFieldId ( "idinventario" );
    addSubFormHeader ( "idinventario", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbHideView, _ ( "Id inventario" ) );
    addSubFormHeader ( "nominventario", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre del inventario" ) );
    addSubFormHeader ( "fechainventario", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha del inventario" ) );
    setInsert ( false );
    setDelete ( false );
    setSortingEnabled ( true );
    
}

