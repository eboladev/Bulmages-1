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

#include <QMessageBox>
#include <QMenu>
#include <QKeyEvent>
#include <QEvent>

#include "articulolist.h"
#include "comparticulolistview.h"
#include "funcaux.h"


/** Prepara el subformulario para trabajar con la tabla comparticulo de la base de datos.
*/
/**
\param parent
**/
ListCompArticuloView::ListCompArticuloView ( QWidget *parent, const char * )
        : SubForm2Bf ( parent )
{
    _depura ( "ListCompArticuloView::ListCompArticuloView", 0 );
    setDBTableName ( "comparticulo" );
    setDBCampoId ( "idcomponente" );
    addSHeader ( "codigocompletoarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave | BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Codigo completo del articulo" ) );
    addSHeader ( "nomarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Nombre del articulo" ) );
    addSHeader ( "cantcomparticulo", BlDbField::DBnumeric, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Cantidad de componente de articulo" ) );
    addSHeader ( "idcomponente", BlDbField::DBint, BlDbField::DBPrimaryKey | BlDbField::DBNotNull, BlSubFormHeader::DBNoWrite, _( "ID componente" ) );
    addSHeader ( "idarticulo", BlDbField::DBint, BlDbField::DBPrimaryKey | BlDbField::DBNotNull, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "ID articulo" ) );
    setinsercion ( TRUE );
    setOrdenEnabled ( FALSE );
    _depura ( "END ListCompArticuloView::ListCompArticuloView", 0 );
}


/** SLOT que responde a la pulsacion de Ctrl + * en el subformulario.
    Dependiendo de la columna que este seleccionada saca el buscador de articulos.
*/
/**
\param row
\param col
\return
**/
void ListCompArticuloView::pressedAsterisk ( int row, int col )
{
    _depura ( "ListCompArticuloView::pressedAsterisk", 0 );
    SDBRecord *rec = lineaat ( row );
    SDBCampo *camp = ( SDBCampo * ) item ( row, col );
    if ( camp->nomcampo() != "codigocompletoarticulo" )
        return;
    _depura ( "ListCompArticuloView::searchArticle", 0 );
    ArticuloList *artlist = new ArticuloList ( ( Company * ) empresaBase(), NULL, 0, ArticuloList::SelectMode );
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    this->setEnabled ( false );
    artlist->show();
    while ( !artlist->isHidden() )
        theApp->processEvents();
    this->setEnabled ( true );
    QString idArticle = artlist->idarticulo();
    delete artlist;
    BlDbRecordSet *cur = empresaBase() ->cargacursor ( "SELECT * FROM articulo WHERE idarticulo=" + idArticle );
    if ( !cur->eof() ) {
        rec->setDBvalue ( "idcomponente", idArticle );
        rec->setDBvalue ( "codigocompletoarticulo", cur->valor ( "codigocompletoarticulo" ) );
        rec->setDBvalue ( "nomarticulo", cur->valor ( "nomarticulo" ) );
    } // end if
    _depura ( "END ListCompArticuloView::pressedAsterisk", 0 );
}


/** SLOT que responde a la finalizacion de edicion de una celda en el subformulario.
    Comprueba que sea la celda de codigo de articulo y si es asi pone el resto de componentes.
*/
/**
\param row
\param col
**/
void ListCompArticuloView::editFinished ( int row, int col )
{
    _depura ( "ListCompArticuloView::editFinished", 0 );
    SDBRecord *rec = lineaat ( row );
    SDBCampo *camp = ( SDBCampo * ) item ( row, col );
    camp->refresh();
    if ( camp->nomcampo() == "codigocompletoarticulo" ) {
        BlDbRecordSet * cur = empresaBase() ->cargacursor ( "SELECT * FROM articulo WHERE codigocompletoarticulo='" + camp->text() + "'" );
        if ( !cur->eof() ) {
            rec->setDBvalue ( "idcomponente", cur->valor ( "idarticulo" ) );
            rec->setDBvalue ( "codigocompletoarticulo", cur->valor ( "codigocompletoarticulo" ) );
            rec->setDBvalue ( "nomarticulo", cur->valor ( "nomarticulo" ) );
        } // end if
    } // end if
    _depura ( "END ListCompArticuloView::editFinished", 0 );
}


/** Destructor de la clase
**/
/**
**/
ListCompArticuloView::~ListCompArticuloView()
{
    _depura ( "ListCompArticuloView::~ListCompArticuloView", 0 );
    _depura ( "END ListCompArticuloView::~ListCompArticuloView", 0 );
}


/** Realiza la carga del listado creando el query necesario para dicha carga
**/
/**
\param idarticulo
**/
void ListCompArticuloView::cargar ( QString idarticulo )
{
    _depura ( "ListCompActiculo::cargar", 0 );
    mdb_idarticulo = idarticulo;
    BlSubForm::cargar ( "SELECT * FROM comparticulo, articulo WHERE comparticulo.idarticulo=" + mdb_idarticulo + " AND articulo.idarticulo = comparticulo.idcomponente" );
    _depura ( "END ListCompActiculo::cargar", 0 );
}


