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

#include <stdio.h>

#include "pluginalias.h"
#include "company.h"
#include "funcaux.h"


///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Punto de Entrada del plugin de Alias\n", 0 );
    return 0;
}


///
/**
\param art
\return
**/
int ArticuloView_ArticuloView ( ArticuloView *art )
{
    _depura ( "ArticuloView_ArticuloView", 0 );

    /// Agregamos el subformulario de validaciones.
    SubForm2Bf *l = new SubForm2Bf ( art );
    l->setObjectName ( QString::fromUtf8 ( "lalias" ) );
    l->setEmpresaBase ( art->empresaBase() );
    l->setDBTableName ( "alias" );
    l->setDBCampoId ( "idalias" );
    l->addSHeader ( "cadalias", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone , QApplication::translate ( "TrabajadorView", "Alias" ) );
    l->addSHeader ( "idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView | SHeader::DBNoWrite , QApplication::translate ( "TrabajadorView", "Id Articulo" ) );
    l->addSHeader ( "idalias", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, QApplication::translate ( "TrabajadorView", "ID Validacion" ) );
    l->setinsercion ( TRUE );
    l->setDelete ( TRUE );
    l->setSortingEnabled ( FALSE );
    art->dialogChanges_setQObjectExcluido ( l->mui_list );

    art->mui_tab->addTab ( l, "Alias" );

    _depura ( "END ArticuloView_ArticuloView", 0 );
    return 0;
}


///
/**
\param art
\return
**/
int ArticuloView_cargar ( ArticuloView *art )
{
    _depura ( "ArticuloView_cargar", 0 );
    SubForm2Bf *l = art->findChild<SubForm2Bf *> ( "lalias" );
    if ( l ) {
        l->cargar ( "SELECT * FROM alias WHERE idarticulo = " + art->DBvalue ( "idarticulo" ) );
    } // end if
    _depura ( "END ArticuloView_cargar", 0 );
    return 0;
}



///
/**
\param art
\return
**/
int ArticuloView_guardar_post ( ArticuloView *art )
{
    _depura ( "ArticuloView_guardar_post", 0 );
    try {
        SubForm2Bf *l = art->findChild<SubForm2Bf *> ( "lalias" );
        l->setColumnValue ( "idarticulo", art->DBvalue ( "idarticulo" ) );
        l->guardar();
        return 0;
    } catch ( ... ) {
        _depura ( "Hubo un error al guardar los alias", 2 );
        return 0;
    }
}


int BusquedaArticulo_on_m_codigocompletoarticulo_textChanged_Post ( BusquedaArticulo *busc )
{
    bool encontrado = FALSE;

    QString val = busc->m_codigocompletoarticulo->text();

    QString SQLQuery = "SELECT * FROM alias LEFT JOIN articulo ON alias.idarticulo = articulo.idarticulo WHERE cadalias = '" + val + "'";
    cursor2 *cur = busc->empresaBase() ->cargacursor ( SQLQuery );
    if ( !cur->eof() ) {
        busc->setidarticulo ( cur->valor ( "idarticulo" ) );
        encontrado = TRUE;
    }
    delete cur;

    if ( encontrado ) {
        return -1;
    } // end if
    return 0;
}
