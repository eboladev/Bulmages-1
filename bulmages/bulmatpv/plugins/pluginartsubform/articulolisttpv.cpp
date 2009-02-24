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
#include "bfcompany.h"
#include "busquedafamilia.h"
#include "busquedatipoarticulo.h"
#include "blfunctions.h"
#include "plugins.h"
#include "btcompany.h"


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
ArticuloList1::ArticuloList1 ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
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
    m_tipoarticulo->setMainCompany ( comp );
    m_familia->setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
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
    QString idarticulo =  mui_list->dbValue ( "idarticulo" );
    ( ( BtCompany * ) mainCompany() )->ticketActual() ->insertarArticulo ( idarticulo, BlFixed ( "1" ) );
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

    setDbTableName ( "articulo" );
    setDbFieldId ( "idarticulo" );

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ArticuloList1SubForm1_ArticuloList1SubForm1", this );
    if ( res != 0 )
        return;

    addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoView | BlSubFormHeader::DbNoWrite, _( "ID articulo" ) );
    addSubFormHeader ( "codigocompletoarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Codigo completo del articulo" ) );
    addSubFormHeader ( "nomarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Nombre del articulo" ) );
    addSubFormHeader ( "abrevarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Descripcion abreviada del articulo" ) );
    addSubFormHeader ( "obserarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Observaciones sobre el articulo" ) );
    addSubFormHeader ( "desctipo_articulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Descripcion del tipo de articulo" ) );
    addSubFormHeader ( "desctipo_iva", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Descripcion tipo de I.V.A." ) );
    addSubFormHeader ( "pvparticulo", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "P.V.P. articulo" ) );
    addSubFormHeader ( "stockarticulo", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Disponible en stock" ) );
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
