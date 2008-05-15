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

#include "articulolisttpv.h"
#include "pgimportfiles.h"
#include "company.h"
#include "articuloview.h"
#include "busquedafamilia.h"
#include "busquedatipoarticulo.h"
#include "funcaux.h"
#include "plugins.h"


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
ArticuloList1::ArticuloList1 ( Company *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : Listado ( comp, parent, flag, editmodo )
{
    _depura ( "ArticuloList1::ArticuloList1", 0 );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ArticuloList1_ArticuloList1", this );
    if ( res != 0 ) {
        _depura ( "END ArticuloList1::ArticuloList1", 0, "Salida por plugin" );
        return;
    } // end if
    m_tipoarticulo->setEmpresaBase ( comp );
    m_familia->setEmpresaBase ( comp );
    mui_list->setEmpresaBase ( comp );
    setSubForm ( mui_list );
    m_usadoarticulo->setCheckState ( Qt::Unchecked );

    if ( modoEdicion() ) {
//        empresaBase() ->meteWindow ( windowTitle(), this );
    } else {
        setWindowTitle ( tr ( "Selector de articulos" ) );
        mui_editar->setHidden ( TRUE );
        mui_crear->setHidden ( TRUE );
        mui_borrar->setHidden ( TRUE );
        mui_exportar->setHidden ( TRUE );
        mui_importar->setHidden ( TRUE );
        mui_imprimir->setHidden ( TRUE );
    } // end if
    presentar();
    hideBusqueda();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "articulo" );
    _depura ( "END ArticuloList1::ArticuloList1", 0 );
}


/// Hace la carga del subformulario para presentar el listado.
/**
**/
void ArticuloList1::presentar()
{
    _depura ( "ArticuloList1::INIT_presenta", 0 );
    mui_list->cargar ( formaQuery() );
    _depura ( "ArticuloList1::END_presenta", 0 );
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
void ArticuloList1::editar ( int row )
{
    _depura ( "ArticuloList1::editar", 0 );
    mdb_idarticulo = mui_list->DBvalue ( "idarticulo", row );
    mdb_nomarticulo = mui_list->DBvalue ( "nomarticulo", row );
    mdb_codigocompletoarticulo = mui_list->DBvalue ( "codigocompletoarticulo", row );
    if ( modoEdicion() ) {
        ArticuloView * art = ( ( Company * ) empresaBase() ) ->newArticuloView();
        empresaBase() ->m_pWorkspace->addWindow ( art );
        /// Si la carga no va bien entonces terminamos.
        if ( art->cargar ( mdb_idarticulo ) ) {
            delete art;
            _depura ( "END ArticuloList1::editar", 0, "Carga Erronea" );
            return;
        } // end if
        art->hide();
        art->show();
    } else {
        close();
        emit ( selected ( mdb_idarticulo ) );
    } // end if
    _depura ( "END ArticuloList1::editar", 0 );
}


/// No requiere de ninguna accion adicional
/**
**/
ArticuloList1::~ArticuloList1()
{
    _depura ( "ArticuloList1::~ArticuloList1", 0 );
    _depura ( "END ArticuloList1::~ArticuloList1", 0 );
}


/** SLOT que responde a la pulsacion del boton borrar
    Verifica que queremos borrar el articulo y lo borra tras preguntar.
*/
/// \TODO: Deberia crear una instancia del articulo e invocar a su metodo de borrado
/// ya que si hay algun plugin puede que no se vea afectado por este metodo.
/**
\return
**/
void ArticuloList1::borrar()
{
    _depura ( "ArticuloList1::on_mui_borrar_clicked", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        mensajeInfo ( tr ( "Tiene que seleccionar un articulo" ) );
        return;
    } // end if
    try {
        QString idarticulo = mui_list->DBvalue ( "idarticulo" );
        if ( QMessageBox::Yes == QMessageBox::question ( this,
                tr ( "Borrar articulo" ),
                tr ( "Esta a punto de borrar un articulo. Estos datos pueden dar problemas." ),
                QMessageBox::Yes, QMessageBox::No ) ) {
            QString SQLQuery = "DELETE FROM articulo WHERE idarticulo = " + idarticulo;
            int error = empresaBase() ->ejecuta ( SQLQuery );
            if ( error )
                throw - 1;
            presentar();
        } // end if
        _depura ( "END ArticuloList1::on_mui_borrar_clicked", 0 );
    } catch ( ... ) {
        mensajeInfo ( tr ( "Error al borrar el articulo" ) );
    } // end try
}


/** Metodo auxiliar que forma el query de presentacion a partir de los datos
    de filtrado.
    Es usado por presentar
*/
/**
**/
QString ArticuloList1::formaQuery()
{
    _depura ( "ArticuloList1::formaQuery", 0 );
    QString query = "";
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
    if ( m_familia->idfamilia() != "" ) {
        query += " AND idfamilia IN (SELECT idfamilia FROM familia WHERE codigocompletofamilia LIKE '" + m_familia->codigocompletofamilia() + "%')";
    } // end if
    if ( m_tipoarticulo->idtipo_articulo() != "" ) {
        query += " AND idtipo_articulo = " + m_tipoarticulo->idtipo_articulo();
    } // end if
    query += " ORDER BY codigocompletoarticulo";
    return ( query );
    _depura ( "ArticuloList1::END_formaQuery()\n", 0 );
}


/// La impresion del listado esta completamente delegada en SubForm3.
/**
**/
void ArticuloList1::imprimir()
{
    _depura ( "ArticuloList1::s_imprimir1", 0 );
    mui_list->imprimirPDF ( "Listado de artículos" );
    _depura ( "END ArticuloList1::s_imprimir1", 0 );
}



/** \TODO: REVISAR ESTE METODO YA QUE NO PARECE SER EL ADECUADO
    EN LA LLAMADA DE SUBMENUS
*/
/**
\return
**/
void ArticuloList1::submenu ( const QPoint & )
{
    _depura ( "ArticuloList1::on_mui_list_customContextMenuRequested", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 )
        return;
    QMenu *popup = new QMenu ( this );
    QAction *edit = popup->addAction ( tr ( "Editar articulo" ) );
    QAction *del = popup->addAction ( tr ( "Borrar articulo" ) );
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
void ArticuloList1::crear()
{
    _depura ( "ArticuloList1::crear", 0 );
    ( ( Company * ) empresaBase() ) ->s_newArticulo();
    _depura ( "END ArticuloList1::crear", 0 );
}


///
/**
\return
**/
QString ArticuloList1::idarticulo()
{
    _depura ( "ArticuloList1::idarticulo", 0 );
    _depura ( "END ArticuloList1::idarticulo", 0 );
    return mdb_idarticulo;
}


///
/**
\return
**/
QString ArticuloList1::nomarticulo()
{
    _depura ( "ArticuloList1::nomarticulo", 0 );
    _depura ( "END ArticuloList1::nomarticulo", 0 );
    return mdb_nomarticulo;
}


///
/**
\return
**/
QString ArticuloList1::codigocompletoarticulo()
{
    _depura ( "ArticuloList1::codigocompletoarticulo", 0 );
    _depura ( "END ArticuloList1::codigocompletoarticulo", 0 );
    return mdb_codigocompletoarticulo;
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
ArticuloList1SubForm1::ArticuloList1SubForm1 ( QWidget *parent, const char * )
        : SubForm2Bf ( parent )
{
    _depura ( "ArticuloList1SubForm1::ArticuloList1SubForm1", 0 );
    setDBTableName ( "articulo" );
    setDBCampoId ( "idarticulo" );
    addSHeader ( "idarticulo", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr ( "ID articulo" ) );
    addSHeader ( "codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Codigo completo del articulo" ) );
    addSHeader ( "nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Nombre del articulo" ) );
    addSHeader ( "abrevarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Descripcion abreviada del articulo" ) );
    addSHeader ( "obserarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Observaciones sobre el articulo" ) );
    addSHeader ( "desctipo_articulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Descripcion del tipo de articulo" ) );
    addSHeader ( "desctipo_iva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Descripcion tipo de I.V.A." ) );
    addSHeader ( "pvparticulo", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "P.V.P. articulo" ) );
    addSHeader ( "stockarticulo", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Disponible en stock" ) );
    setinsercion ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    /// Disparamos los plugins.
    g_plugins->lanza ( "ArticuloList1SubForm1_ArticuloList1SubForm1_Post", this );
    _depura ( "END ArticuloList1SubForm1::ArticuloList1SubForm1", 0 );
}


///
/**
**/
ArticuloList1SubForm1::~ArticuloList1SubForm1()
{
    _depura ( "ArticuloList1SubForm1::~ArticuloList1SubForm1", 0 );
    _depura ( "END ArticuloList1SubForm1::~ArticuloList1SubForm1", 0 );

}
