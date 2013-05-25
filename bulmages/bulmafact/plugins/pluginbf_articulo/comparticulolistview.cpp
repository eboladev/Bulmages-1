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
#include <QtWidgets/QMenu>
#include <QtGui/QKeyEvent>
#include <QtCore/QEvent>

#include "articulolist.h"
#include "comparticulolistview.h"
#include "blfunctions.h"


/** Prepara el subformulario para trabajar con la tabla comparticulo de la base de datos.
*/
/**
\param parent
**/
ListCompArticuloView::ListCompArticuloView ( QWidget *parent, const char * )
        : BfSubForm ( parent )
{
    BL_FUNC_DEBUG
    setDbTableName ( "comparticulo" );
    setDbFieldId ( "idcomponente" );
    addSubFormHeader ( "codigocompletoarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave | BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Codigo completo del articulo" ) );
    addSubFormHeader ( "nomarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "Nombre del articulo" ) );
    addSubFormHeader ( "cantcomparticulo", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Cantidad de componente de articulo" ) );
    addSubFormHeader ( "idcomponente", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView, _ ( "Id componente" ) );
    addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbPrimaryKey | BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id articulo" ) );
    setInsert ( true );
    setOrdenEnabled ( false );
    
}


/** SLOT que responde a la pulsacion de Ctrl + * o F2 en el subformulario.
    Dependiendo de la columna que este seleccionada saca el buscador de articulos.
*/
/**
\param row
\param col
\return
**/
void ListCompArticuloView::pressedAsterisk ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp )
{
    BL_FUNC_DEBUG
    if ( camp->fieldName() != "codigocompletoarticulo" ) {
	
        return;
    } // end if

    ArticuloList *artlist = new ArticuloList ( ( BfCompany * ) mainCompany(), NULL, 0, BL_SELECT_MODE );
    blCenterOnScreen(artlist);
    artlist->m_filtro->setFocus(Qt::PopupFocusReason);
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    this->setEnabled ( false );
    artlist->show();
    while ( !artlist->isHidden() )
        g_theApp->processEvents();
    this->setEnabled ( true );
    QString idArticle = artlist->idarticulo();
    delete artlist;
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM articulo WHERE idarticulo=" + idArticle );
    if ( !cur->eof() ) {
        rec->setDbValue ( "idcomponente", idArticle );
        rec->setDbValue ( "codigocompletoarticulo", cur->value( "codigocompletoarticulo" ) );
        rec->setDbValue ( "nomarticulo", cur->value( "nomarticulo" ) );
    } // end if
    
}


/** SLOT que responde a la finalizacion de edicion de una celda en el subformulario.
    Comprueba que sea la celda de codigo de articulo y si es asi pone el resto de componentes.
*/
/**
\param row
\param col
**/
void ListCompArticuloView::editFinished ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp )
{
    BL_FUNC_DEBUG
    camp->refresh();
    if ( camp->fieldName() == "codigocompletoarticulo" ) {
        BlDbRecordSet * cur = mainCompany() ->loadQuery ( "SELECT * FROM articulo WHERE codigocompletoarticulo='" + camp->text() + "'" );
        if ( !cur->eof() ) {
            rec->setDbValue ( "idcomponente", cur->value( "idarticulo" ) );
            rec->setDbValue ( "codigocompletoarticulo", cur->value( "codigocompletoarticulo" ) );
            rec->setDbValue ( "nomarticulo", cur->value( "nomarticulo" ) );
	    rec->setDbValue ( "cantcomparticulo", "1.00" );
        } // end if
    } // end if
    
}


/** Destructor de la clase
**/
/**
**/
ListCompArticuloView::~ListCompArticuloView()
{
    BL_FUNC_DEBUG
    
}


/** Realiza la carga del listado creando el query necesario para dicha carga
**/
/**
\param idarticulo
**/
void ListCompArticuloView::load ( QString idarticulo )
{
    BL_FUNC_DEBUG
    mdb_idarticulo = idarticulo;
    BlSubForm::load ( "SELECT * FROM comparticulo LEFT JOIN articulo ON comparticulo.idcomponente = articulo.idarticulo WHERE comparticulo.idarticulo=" + mdb_idarticulo );
    
}


