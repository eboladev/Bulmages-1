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

#include "pluginresarticulos.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "listlresarticulo.h"


///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Punto de Entrada del plugin de Resumen de Articulos\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginresarticulos", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

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
    ListCompras *lc = new ListCompras ( art );
    lc->setObjectName ( QString::fromUtf8 ( "lcompras" ) );
    lc->setMainCompany ( art->mainCompany() );
    lc->cargar ( "0" );
    art->mui_tab->addTab ( lc, "Compras" );

    ListLResArticulo *l = new ListLResArticulo ( art );
    l->setObjectName ( QString::fromUtf8 ( "lresfacturacion" ) );
    l->setMainCompany ( art->mainCompany() );
    l->cargar ( "0" );
    art->mui_tab->addTab ( l, "Ventas" );

    ListEntregas *l1 = new ListEntregas ( art );
    l1->setObjectName ( QString::fromUtf8 ( "lentregas" ) );
    l1->setMainCompany ( art->mainCompany() );
    l1->cargar ( "0" );
    art->mui_tab->addTab ( l1, "Entregas" );

    ListRecogidas *lr = new ListRecogidas ( art );
    lr->setObjectName ( QString::fromUtf8 ( "lrecogidas" ) );
    lr->setMainCompany ( art->mainCompany() );
    lr->cargar ( "0" );
    art->mui_tab->addTab ( lr, "Recogidas" );

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
    ListLResArticulo *l = art->findChild<ListLResArticulo *> ( "lresfacturacion" );
    l->cargar ( art->dbValue ( "idarticulo" ) );

    ListEntregas *l1 = art->findChild<ListEntregas *> ( "lentregas" );
    l1->cargar ( art->dbValue ( "idarticulo" ) );

    ListCompras *lc = art->findChild<ListCompras *> ( "lcompras" );
    lc->cargar ( art->dbValue ( "idarticulo" ) );

    ListRecogidas *llr = art->findChild<ListRecogidas *> ( "lrecogidas" );
    llr->cargar ( art->dbValue ( "idarticulo" ) );

    _depura ( "END ArticuloView_cargar", 0 );
    return 0;
}

