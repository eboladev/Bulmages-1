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
#include <QTime>
#include <QWidget>

#include "pluginalias.h"
#include "bfcompany.h"
#include "blfunctions.h"


QString g_minAlias=(QString)NULL;
QString g_maxAlias=(QString)NULL;

QTime g_ultimRefrescAlias;
int g_maxLenAlias=0;
int g_minLenAlias=0;
///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    
    _depura ( "Punto de entrada del plugin de alias\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginalias", g_confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    g_ultimRefrescAlias.start();
    return 0;
}


void invalidaEstadAlias(void) {
        g_minAlias = (QString)NULL;
        g_maxAlias = (QString)NULL;
        g_minLenAlias = 0;
        g_maxLenAlias = 0;
}

bool posibleAlias(QString alias, BlMainCompany *eb) {
   if ( MILISEG_REFRESCO_ESTAD_ALIAS &&       
       (g_ultimRefrescAlias.elapsed()>MILISEG_REFRESCO_ESTAD_ALIAS)) {
       invalidaEstadAlias();
   }
   if ((g_minAlias == (QString)NULL)&&(g_maxAlias==(QString)NULL)) {
       	BlDbRecordSet *cur = eb ->loadQuery (
        "SELECT \
   (SELECT cadalias FROM alias ORDER by cadalias  USING ~<~ LIMIT 1) as minalias,\
   (SELECT cadalias FROM alias ORDER by cadalias  USING ~>~ LIMIT 1) as maxalias,\
   (SELECT max(length(cadalias)) FROM alias ) as maxlenalias,\
   (SELECT min(length(cadalias)) FROM alias ) as minlenalias" );
        g_minAlias = cur->valor("minalias");
        g_maxAlias = cur->valor("maxalias");
        g_minLenAlias = cur->valorInt("minlenalias");
        g_maxLenAlias = cur->valorInt("maxlenalias");

        // si la taula alias es buida posem un max impossible per evitar tornar 
        // a buscar fins que en donin un d'alta (en l'alta posen NULL a g_maxAlias)
        if (g_maxAlias == (QString)NULL) {
            g_maxAlias="";
        }
        g_ultimRefrescAlias.restart();
   }
   return ((g_minAlias!=(QString)NULL) && (g_minAlias!=(QString)NULL)
           && (alias !=(QString)NULL)
           && (g_minAlias <= alias) && ( g_maxAlias >= alias )
           && (g_minLenAlias <= alias.length())
           && ( g_maxLenAlias >= alias.length() )
          );
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
    BfSubForm *l = new BfSubForm ( art );
    l->setObjectName ( QString::fromUtf8 ( "lalias" ) );
    l->setMainCompany ( art->mainCompany() );
    l->setDbTableName ( "alias" );
    l->setDbFieldId ( "idalias" );
    l->addSubFormHeader ( "cadalias", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone , _( "Alias" ) );
    l->addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite , _( "Id articulo" ) );
    l->addSubFormHeader ( "idalias", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _( "Id validacion" ) );
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
    BfSubForm *l = art->findChild<BfSubForm *> ( "lalias" );
    if ( l ) {
        l->cargar ( "SELECT * FROM alias WHERE idarticulo = " + art->dbValue ( "idarticulo" ) );
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
        BfSubForm *l = art->findChild<BfSubForm *> ( "lalias" );
        l->setColumnValue ( "idarticulo", art->dbValue ( "idarticulo" ) );
        l->guardar();
        invalidaEstadAlias();
        return 0;
    } catch ( ... ) {
        _depura ( "Hubo un error al guardar los alias", 2 );
        return 0;
    }
}


int Busqueda_on_m_inputBusqueda_textChanged ( BlSearchWidget *busc )
{
    bool encontrado = FALSE;

	if (busc->tableName() == "articulo") {
	QString val = busc->m_inputBusqueda->text();
	
	if ( posibleAlias ( val, busc->mainCompany() ) )
	{
                // busco amb l'operador que compara sense locale ~=~ per aprofitar 
                // els indexos insensibles a locale (si fossin sensibles a 
                // locale i no hi hagues el mateix locale al servidor PostgreSQL
                // que al client no funcionaria posibleAlias perquè calcularia 
                // els minims i màxims de cadalias amb un ordre de col·lacio 
                // i compararia el valor entrat amb un altre.
		QString valors[1] = {val};
		QString SQLQuery = "SELECT * FROM alias LEFT JOIN articulo ON alias.idarticulo = articulo.idarticulo WHERE cadalias ~=~ $1";
		BlDbRecordSet *cur = busc->mainCompany() ->loadQuery ( SQLQuery, 1, valors, NULL, NULL );
		if ( !cur->eof() )
		{
			busc->setId( cur->valor ( "idarticulo" ) );
			encontrado = TRUE;
		}
		delete cur;
	}

    if ( encontrado ) {
        return -1;
    } // end if

	} // end if
    return 0;
}


int BusquedaArticuloDelegate_textChanged_Post ( BusquedaArticuloDelegate *baDel )
{
      _depura ( "BusquedaArticuloDelegate_textChanged_Post", 0 );

	bool encontrado = FALSE;
	if ( posibleAlias ( baDel->entrada(), baDel->mainCompany() ) )
	{
                _depura("possible Alias ",0, baDel->entrada());
		QString SQLQuery = "SELECT codigocompletoarticulo,nomarticulo,cadalias FROM alias LEFT JOIN articulo ON alias.idarticulo = articulo.idarticulo WHERE cadalias ~=~ $1";
		QString valors[1] = {baDel->entrada() };
		BlDbRecordSet *cur = baDel->mainCompany() ->loadQuery ( SQLQuery, 1, valors );

		if ( !cur->eof() )
		{
			baDel->insertItem ( 0, cur->valor ( "codigocompletoarticulo" ) 
                                            + ".-" + cur->valor ( "nomarticulo" )
			                    + " (" + cur->valor ( "cadalias" ) + ") ", 
                                            QVariant(cur->valor ( "codigocompletoarticulo" )) 
                                          );
			baDel->setEditText(baDel->entrada());
			encontrado = TRUE;
		}
		delete cur;
	}
      _depura ( "END BusquedaArticuloDelegate_textChanged_Post", 0 );

	if ( encontrado )
	{
		return -1;
	} // end if
	return 0;

}

int BlSubForm_campoCompleto(BlSubForm *grid,void **resultat) {
   _depura("BlSubForm_campoCompleto de pluginalias",0);
   BusquedaArticuloDelegate *baDel = dynamic_cast<BusquedaArticuloDelegate *>( grid->mui_list->QAbstractItemView::indexWidget ( grid->mui_list->currentIndex() ));
    if (baDel) {
         _depura("camp de codi article/alias. count="+QString::number(baDel->count()),0);
         QString elec=baDel->eligeUnico();
         if (!elec.isNull()) {
          grid->item(grid->currentRow(), grid->currentColumn())->setText(elec);
         }
    }
  
   _depura("END BlSubForm_campoCompleto de pluginalias",0);
    
  return 0; // continua el processament normal
 }
