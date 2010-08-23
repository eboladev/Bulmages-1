/***************************************************************************
 *   Copyright (C) 2004 by J. M. Estopa Rey                                *
 *   pepma@telefonica.net                                                  *
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
#include <QFile>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QTextStream>
#include <QMenu>
#include <QFileDialog>

#include "bccuentalistview.h"
#include "bccompany.h"
#include "bccuentaview.h"
#include "blfunctions.h"
#include "blplugins.h"


/** Constructor de la ventana de listado de articulos
    Inicializa todos los componentes, propaga el puntero a m_company
    Configura la pantalla para que haga una cosa u otra en funcion de m_modo.
    Si estamos en modo edicion mete la ventana en el workSpace. En caso contrario
    la presenta como un 'popup'.
    Hace la presentacion inicial.
*/
/**
\param comp
\param paren
\param flag
\param editmodo
\return
**/
BcCuentaListView::BcCuentaListView ( BcCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    blDebug ( "BcCuentaListView::BcCuentaListView", 0 );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "BcCuentaListView_BcCuentaListView", this );
    if ( res != 0 ) {
        blDebug ( "END BcCuentaListView::BcCuentaListView", 0, "Salida por plugin" );
        return;
    } // end if
    mui_list->setMainCompany ( comp );
    setSubForm ( mui_list );

    if ( modoEdicion() ) {
        mainCompany() ->meteWindow ( windowTitle(), this );
    } else {
        setWindowTitle ( _ ( "Selector de articulos" ) );
        mui_imprimir->setHidden ( TRUE );
    } // end if

    mui_nivel->clear();
    for ( int i = comp->numdigitosempresa(); i >= 2; i-- ) {
        /// Inicializamos la tabla de nivel.
        mui_nivel->insertItem ( i, QString::number ( i ) );
    } // end for


    presentar();
    hideBusqueda();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "cuenta" );
    blDebug ( "END BcCuentaListView::BcCuentaListView", 0 );
}


/// Hace la carga del subformulario para presentar el listado.
/**
**/
void BcCuentaListView::presentar()
{
    blDebug ( "BcCuentaListView::presentar", 0 );
    mui_list->cargar ( formaQuery() );
    blDebug ( "BcCuentaListView::END presentar", 0 );
}


/** Se encarga de la accion preseleccionada al hacer doble click o al darle
    al boton de editar.
    Si estamos en modo seleccion actualiza el valor de los campos de seleccion y
    cierra la ventana.
    Si estamos en modo edicion abre una instancia de ArticuloView y lo carga con el
    valor seleccionado.
*/
/// \TODO: este metodo deberia ser editar
/**
\param row
\return
**/
void BcCuentaListView::editar ( int row )
{
    blDebug ( "BcCuentaListView::editar", 0 );
    mdb_idcuenta = mui_list->dbValue ( "idcuenta", row );
    mdb_nombrecuenta = mui_list->dbValue ( "descripcion", row );
    mdb_codigocuenta = mui_list->dbValue ( "codigo", row );
    if ( modoEdicion() ) {
        /*
                ArticuloView * art = ( ( BcCompany * ) BlMainCompany() ) ->newArticuloView();
                BlMainCompany() ->m_pWorkspace->addWindow ( art );
                /// Si la carga no va bien entonces terminamos.
                if ( art->cargar ( mdb_idarticulo ) ) {
                    delete art;
                    blDebug ( "END BcCuentaListView::editar", 0, "Carga Erronea" );
                    return;
                } // end if
                art->hide();
                art->show();
        */
    } else {
        close();
        emit ( selected ( mdb_idcuenta ) );
    } // end if
    blDebug ( "END BcCuentaListView::editar", 0 );
}


/// No requiere de ninguna accion adicional
/**
**/
BcCuentaListView::~BcCuentaListView()
{
    blDebug ( "BcCuentaListView::~BcCuentaListView", 0 );
    blDebug ( "END BcCuentaListView::~BcCuentaListView", 0 );
}


/** SLOT que responde a la pulsacion del boton borrar
    Verifica que queremos borrar el articulo y lo borra tras preguntar.
*/
/// \TODO: Deberia crear una instancia del articulo e invocar a su metodo de borrado
/// ya que si hay algun plugin puede que no se vea afectado por este metodo.
/**
\return
**/
void BcCuentaListView::borrar()
{
    /*
        blDebug ( "BcCuentaListView::on_mui_borrar_clicked", 0 );
        int a = mui_list->currentRow();
        if ( a < 0 ) {
            mensajeInfo ( _( "Tiene que seleccionar un articulo" ), this );
            return;
        } // end if
        try {
            QString idarticulo = mui_list->dbValue ( "idarticulo" );
            if ( QMessageBox::Yes == QMessageBox::question ( this,
                    _( "Borrar articulo" ),
                    _( "Esta a punto de borrar un articulo. Estos datos pueden dar problemas." ),
                    QMessageBox::Yes, QMessageBox::No ) ) {
                QString SQLQuery = "DELETE FROM articulo WHERE idarticulo = " + idarticulo;
                int error = BlMainCompany() ->runQuery ( SQLQuery );
                if ( error )
                    throw - 1;
                presentar();
            } // end if
            blDebug ( "END BcCuentaListView::on_mui_borrar_clicked", 0 );
        } catch ( ... ) {
            mensajeInfo ( _( "Error al borrar el articulo" ), this );
        } // end try
    */
}


/** Metodo auxiliar que forma el query de presentacion a partir de los datos
    de filtrado.
    Es usado por presentar
*/
/**
**/
QString BcCuentaListView::formaQuery()
{
    blDebug ( "BcCuentaListView::formaQuery", 0 );
    QString query = "";

    query = "SELECT * FROM cuenta WHERE 1=1 ";
    if ( m_filtro->text() != "" ) {
        query += " AND  ( lower(descripcion) LIKE lower('%" + m_filtro->text() + "%') ";
        query += " OR lower(codigo) LIKE lower('%" + m_filtro->text() + "%')) ";
    } // end if

    if ( mui_nivel->currentText() != "" ) {
        query += " AND nivel(codigo) >= " + mui_nivel->currentText();
    } // end if

    /*
        query += "SELECT * FROM articulo NATURAL LEFT JOIN tipo_iva NATURAL LEFT JOIN tipo_articulo WHERE 1 = 1 ";
        if ( m_presentablearticulo->isChecked() )
            query += " AND presentablearticulo ";
        if ( m_usadoarticulo->isChecked() )
            query += " AND idarticulo IN (SELECT DISTINCT idarticulo FROM lpresupuesto"
                     " UNION SELECT DISTINCT idarticulo FROM lpedidocliente"
                     " UNION SELECT DISTINCT idarticulo FROM lalbaran"
                     " UNION SELECT DISTINCT idarticulo FROM lfactura"
                     " UNION SELECT DISTINCT idarticulo FROM lpedidoproveedor"
                     " UNION SELECT DISTINCT idarticulo FROM lalbaranp"
                     " UNION SELECT DISTINCT idarticulo FROM lfacturap"
                     ") ";
        if ( m_filtro->text() != "" )
            query += " AND lower(nomarticulo) LIKE lower('%" + m_filtro->text() + "%') ";
    */
//    if ( m_familia->idfamilia() != "" ) {
//        query += " AND idfamilia IN (SELECT idfamilia FROM familia WHERE codigocompletofamilia LIKE '" + m_familia->codigocompletofamilia() + "%')";
//    } // end if
//    if ( m_tipoarticulo->idtipo_articulo() != "" ) {
//        query += " AND idtipo_articulo = " + m_tipoarticulo->idtipo_articulo();
//    } // end if
//    query += " ORDER BY codigocompletoarticulo";
    return ( query );
    blDebug ( "BcCuentaListView::END_formaQuery()\n", 0 );
}


/// La impresion del listado esta completamente delegada en BlSubForm.
/**
**/
void BcCuentaListView::imprimir()
{
    blDebug ( "BcCuentaListView::s_imprimir1", 0 );
    mui_list->imprimirPDF ( "Listado de cuentas" );
    blDebug ( "END BcCuentaListView::s_imprimir1", 0 );
}


/// SLOT que exporta el listado de articulos a formato XML.
/**
**/
void BcCuentaListView::on_mui_exportar_clicked()
{
    /*
        blDebug ( "BcCuentaListView::on_mui_exportar_clicked", 0 );
        QFile filexml ( QFileDialog::getSaveFileName ( this,
                        _( "Elija el archivo" ),
                        g_confpr->valor ( CONF_DIR_USER ),
                        _( "Clientes (*.xml)" ) ) );

        if ( filexml.open ( QIODevice::WriteOnly ) ) {
            bulmafact2XML ( filexml, IMPORT_ARTICULOS );
            filexml.close();
        } else {
            blDebug ( "ERROR AL ABRIR EL ARCHIVO\n", 2 );
        } // end if

        blDebug ( "END BcCuentaListView::on_mui_exportar_clicked", 0 );
    */
}


/** SLOT que importa los articulos desde un fichero xml
    Pide al usuario que indique cual es el fichero
    Hace la importacion a traves de XML2BulmaFact.
*/
/**
**/
void BcCuentaListView::on_mui_importar_clicked()
{
    /*
        blDebug ( "BcCuentaListView::INIT_s_importar", 0 );
        QFile filexml ( QFileDialog::getOpenFileName ( this,
                        _( "Elija el archivo" ),
                        g_confpr->valor ( CONF_DIR_USER ),
                        _( "Clientes (*.xml)" ) ) );

        if ( filexml.open ( QIODevice::ReadOnly ) ) {
            XML2BulmaFact ( filexml, IMPORT_ARTICULOS );
            filexml.close();
            presentar();
        } else {
            blDebug ( "ERROR AL ABRIR EL ARCHIVO", 2 );
        } // end if
        blDebug ( "BcCuentaListView::END_s_importar", 0 );
    */
}


/** \TODO: REVISAR ESTE METODO YA QUE NO PARECE SER EL ADECUADO
    EN LA LLAMADA DE SUBMENUS
*/
/**
\return
**/
void BcCuentaListView::submenu ( const QPoint & )
{

    blDebug ( "BcCuentaListView::on_mui_list_customContextMenuRequested", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 )
        return;
    QMenu *popup = new QMenu ( this );
    QAction *edit = popup->addAction ( _ ( "Editar articulo" ) );
    QAction *del = popup->addAction ( _ ( "Borrar articulo" ) );
    QAction *opcion = popup->exec ( QCursor::pos() );
    if ( opcion == del )
        on_mui_borrar_clicked();
    if ( opcion == edit )
        on_mui_editar_clicked();
    delete popup;
}


///
/**
**/
void BcCuentaListView::crear()
{
    /*
        blDebug ( "BcCuentaListView::crear", 0 );
        ( ( BcCompany * ) BlMainCompany() ) ->s_newArticulo();
        blDebug ( "END BcCuentaListView::crear", 0 );
    */
}


///
/**
\return
**/
QString BcCuentaListView::idcuenta()
{
    blDebug ( "BcCuentaListView::idarticulo", 0 );
    blDebug ( "END BcCuentaListView::idarticulo", 0 );
    return mdb_idcuenta;
}


///
/**
\return
**/
QString BcCuentaListView::nombrecuenta()
{
    blDebug ( "BcCuentaListView::nombrecuenta", 0 );
    blDebug ( "END BcCuentaListView::nombrecuenta", 0 );
    return mdb_nombrecuenta;
}


///
/**
\return
**/
QString BcCuentaListView::codigocuenta()
{
    blDebug ( "BcCuentaListView::codigocompletoarticulo", 0 );
    blDebug ( "END BcCuentaListView::codigocompletoarticulo", 0 );
    return mdb_codigocuenta;
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Prepara el subformulario para que trabaje con la tabla articulo
*/
///
/**
\param parent
**/
BcCuentaListSubForm::BcCuentaListSubForm ( QWidget *parent, const char * )
        : BcSubForm ( parent )
{
    blDebug ( "BcCuentaListSubForm::BcCuentaListSubForm", 0 );
    setDbTableName ( "cuenta" );
    setDbFieldId ( "idcuenta" );


    addSubFormHeader ( "idcuenta", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID cuenta" ) );
    addSubFormHeader ( "codigo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Codigo Cuenta" ) );
    addSubFormHeader ( "descripcion", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre de la cuenta" ) );
    /*
        addSubFormHeader ( "abrevarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Descripcion abreviada del articulo" ) );
        addSubFormHeader ( "obserarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Observaciones sobre el articulo" ) );
        addSubFormHeader ( "desctipo_articulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Descripcion del tipo de articulo" ) );
        addSubFormHeader ( "desctipo_iva", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Descripcion tipo de I.V.A." ) );
        addSubFormHeader ( "pvparticulo", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "P.V.P. articulo" ) );
        addSubFormHeader ( "stockarticulo", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Disponible en stock" ) );
    */
    setInsert ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    /// Disparamos los plugins.
    g_plugins->lanza ( "BcCuentaListSubForm_BcCuentaListSubForm_Post", this );
    blDebug ( "END BcCuentaListSubForm::BcCuentaListSubForm", 0 );
}


///
/**
**/

BcCuentaListSubForm::~BcCuentaListSubForm()
{
    blDebug ( "BcCuentaListSubForm::~BcCuentaListSubForm", 0 );
    blDebug ( "END BcCuentaListSubForm::~BcCuentaListSubForm", 0 );

}

