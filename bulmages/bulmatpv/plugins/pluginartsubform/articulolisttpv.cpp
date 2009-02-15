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
#include "blfunctions.h"
#include "plugins.h"
#include "empresatpv.h"


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
        : BlFormList ( comp, parent, flag, editmodo )
{
    _depura ( "ArticuloList1::ArticuloList1", 0 );
    setupUi ( this );

    setObjectName ( "ArticuloList1" );

    showFullScreen();
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
    mui_list->setFocusPolicy ( Qt::NoFocus );
    m_usadoarticulo->setCheckState ( Qt::Unchecked );


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



/// No requiere de ninguna accion adicional
/**
**/
ArticuloList1::~ArticuloList1()
{
    _depura ( "ArticuloList1::~ArticuloList1", 0 );
    _depura ( "END ArticuloList1::~ArticuloList1", 0 );
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
    QAction *edit = popup->addAction ( _( "Editar articulo" ) );
    QAction *del = popup->addAction ( _( "Borrar articulo" ) );
    QAction *opcion = popup->exec ( QCursor::pos() );
    if ( opcion == del )
        on_mui_borrar_clicked();
    if ( opcion == edit )
        on_mui_editar_clicked();
    delete popup;
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


///
/**
**/
void ArticuloList1::on_mui_list_cellDoubleClicked ( int, int )
{
    _depura ( "myplugin::elslot", 0 );
    QString idarticulo =  mui_list->DBvalue ( "idarticulo" );
    ( ( EmpresaTPV * ) empresaBase() )->ticketActual() ->insertarArticulo ( idarticulo, BlFixed ( "1" ) );
    _depura ( "END myplugin::elslot", 0 );
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
        : BfSubForm ( parent )
{
    _depura ( "ArticuloList1SubForm1::ArticuloList1SubForm1", 0 );

    setDBTableName ( "articulo" );
    setDBCampoId ( "idarticulo" );

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ArticuloList1SubForm1_ArticuloList1SubForm1", this );
    if ( res != 0 )
        return;

    addSHeader ( "idarticulo", BlDbField::DBint, BlDbField::DBNotNull | BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "ID articulo" ) );
    addSHeader ( "codigocompletoarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Codigo completo del articulo" ) );
    addSHeader ( "nomarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Nombre del articulo" ) );
    addSHeader ( "abrevarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Descripcion abreviada del articulo" ) );
    addSHeader ( "obserarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Observaciones sobre el articulo" ) );
    addSHeader ( "desctipo_articulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Descripcion del tipo de articulo" ) );
    addSHeader ( "desctipo_iva", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Descripcion tipo de I.V.A." ) );
    addSHeader ( "pvparticulo", BlDbField::DBnumeric, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "P.V.P. articulo" ) );
    addSHeader ( "stockarticulo", BlDbField::DBnumeric, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Disponible en stock" ) );
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
