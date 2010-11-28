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

#include "pluginbf_alias.h"
#include "bfcompany.h"
#include "blfunctions.h"


QString g_minAlias = ( QString ) "";
QString g_maxAlias = ( QString ) "";

QTime g_ultimRefrescAlias;
int g_maxLenAlias = 0;
int g_minLenAlias = 0;
///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{

    blDebug ( "Punto de entrada del plugin de alias\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_alias", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_ultimRefrescAlias.start();
    return 0;
}


void invalidaEstadAlias ( void )
{
    g_minAlias = ( QString ) "";
    g_maxAlias = ( QString ) "";
    g_minLenAlias = 0;
    g_maxLenAlias = 0;
}


bool posibleAlias ( QString alias, BlMainCompany *eb )
{
    if ( MILISEG_REFRESCO_ESTAD_ALIAS &&
            ( g_ultimRefrescAlias.elapsed() > MILISEG_REFRESCO_ESTAD_ALIAS ) ) {
        invalidaEstadAlias();
    }
    if ( ( g_minAlias == ( QString ) "" ) && ( g_maxAlias == ( QString ) "" ) ) {
        BlDbRecordSet *cur = eb ->loadQuery (
                                 "SELECT \
   (SELECT cadalias FROM alias ORDER by cadalias  USING ~<~ LIMIT 1) as minalias,\
   (SELECT cadalias FROM alias ORDER by cadalias  USING ~>~ LIMIT 1) as maxalias,\
   (SELECT max(length(cadalias)) FROM alias ) as maxlenalias,\
   (SELECT min(length(cadalias)) FROM alias ) as minlenalias" );
        g_minAlias = cur->valor ( "minalias" );
        g_maxAlias = cur->valor ( "maxalias" );
        g_minLenAlias = cur->valorInt ( "minlenalias" );
        g_maxLenAlias = cur->valorInt ( "maxlenalias" );

        // si la taula alias es buida posem un max impossible per evitar tornar
        // a buscar fins que en donin un d'alta (en l'alta posen NULL a g_maxAlias)
        if ( g_maxAlias == ( QString ) "" ) {
            g_maxAlias = "";
        }
        g_ultimRefrescAlias.restart();
    }
    return ( ( g_minAlias != ( QString ) "" ) && ( g_minAlias != ( QString ) "" )
             && ( alias != ( QString ) "" )
             && ( g_minAlias <= alias ) && ( g_maxAlias >= alias )
             && ( g_minLenAlias <= alias.length() )
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
    blDebug ( "ArticuloView_ArticuloView", 0 );

    /// Agregamos el subformulario de validaciones.
    BfSubForm *l = new BfSubForm ( art );
    l->setObjectName ( QString::fromUtf8 ( "lalias" ) );
    l->setMainCompany ( art->mainCompany() );
    l->setDbTableName ( "alias" );
    l->setDbFieldId ( "idalias" );
    l->addSubFormHeader ( "cadalias", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone , _ ( "Alias" ) );
    l->addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite , _ ( "Id articulo" ) );
    l->addSubFormHeader ( "idalias", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id validacion" ) );
    l->setInsert ( TRUE );
    l->setDelete ( TRUE );
    l->setSortingEnabled ( FALSE );
    art->dialogChanges_setQObjectExcluido ( l->mui_list );

    art->mui_tab->addTab ( l, "Alias" );

    blDebug ( "END ArticuloView_ArticuloView", 0 );
    return 0;
}


///
/**
\param art
\return
**/
int ArticuloView_cargar ( ArticuloView *art )
{
    blDebug ( "ArticuloView_cargar", 0 );
    BfSubForm *l = art->findChild<BfSubForm *> ( "lalias" );
    if ( l ) {
        l->cargar ( "SELECT * FROM alias WHERE idarticulo = " + art->dbValue ( "idarticulo" ) );
    } // end if
    blDebug ( "END ArticuloView_cargar", 0 );
    return 0;
}



///
/**
\param art
\return
**/
int ArticuloView_guardar_post ( ArticuloView *art )
{
    blDebug ( "ArticuloView_guardar_post", 0 );
    try {
        BfSubForm *l = art->findChild<BfSubForm *> ( "lalias" );
        l->setColumnValue ( "idarticulo", art->dbValue ( "idarticulo" ) );
        l->guardar();
        invalidaEstadAlias();
        return 0;
    } catch ( ... ) {
        blDebug ( "Hubo un error al guardar los alias", 2 );
        return 0;
    }
}


int Busqueda_on_m_inputBusqueda_textChanged ( BlSearchWidget *busc )
{
    bool encontrado = FALSE;

    if ( busc->tableName() == "articulo" ) {
        QString val = busc->m_inputBusqueda->text();

        if ( posibleAlias ( val, busc->mainCompany() ) ) {
            // busco amb l'operador que compara sense locale ~~* o ILIKE per aprofitar
            // els indexos insensibles a locale (si fossin sensibles a
            // locale i no hi hagues el mateix locale al servidor PostgreSQL
            // que al client no funcionaria posibleAlias perquè calcularia
            // els minims i màxims de cadalias amb un ordre de col·lacio
            // i compararia el valor entrat amb un altre.
            QString SQLQuery = "SELECT * FROM alias LEFT JOIN articulo ON alias.idarticulo = articulo.idarticulo WHERE cadalias ILIKE '" + busc->mainCompany() ->sanearCadena(val) + "'";
            BlDbRecordSet *cur = busc->mainCompany() ->loadQuery ( SQLQuery );
            if ( !cur->eof() ) {
                busc->setId ( cur->valor ( "idarticulo" ) );
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

/*

int BfBuscarArticuloDelegate_textChanged_Post ( BfBuscarArticuloDelegate *baDel )
{
    blDebug ( "BfBuscarArticuloDelegate_textChanged_Post", 0 );

    bool encontrado = FALSE;
    if ( posibleAlias ( baDel->entrada(), baDel->mainCompany() ) ) {
        blDebug ( "possible Alias ", 0, baDel->entrada() );
        QString SQLQuery = "SELECT codigocompletoarticulo,nomarticulo,cadalias FROM alias LEFT JOIN articulo ON alias.idarticulo = articulo.idarticulo WHERE cadalias ~=~ $1";
        QString valors[1] = {baDel->entrada() };
        BlDbRecordSet *cur = baDel->mainCompany() ->loadQuery ( SQLQuery, 1, valors );

        if ( !cur->eof() ) {
            baDel->insertItem ( 0, cur->valor ( "codigocompletoarticulo" )
                                + ".-" + cur->valor ( "nomarticulo" )
                                + " (" + cur->valor ( "cadalias" ) + ") ",
                                QVariant ( cur->valor ( "codigocompletoarticulo" ) )
                              );
            baDel->setEditText ( baDel->entrada() );
            encontrado = TRUE;
        }
        delete cur;
    }
    blDebug ( "END BfBuscarArticuloDelegate_textChanged_Post", 0 );

    if ( encontrado ) {
        return -1;
    } // end if
    return 0;

}


int BfSubForm_on_mui_list_editFinished ( BfSubForm *sf )
{
    blDebug ( "BfSubForm_on_mui_list_editFinished", 0 );
    BlDbSubFormField *camp = sf->m_campoactual;
    if  ( camp->nomcampo() == "codigocompletoarticulo" ) {
      if ( posibleAlias ( camp->text(), sf->mainCompany() ) ) {
        blDebug ( "possible Alias ", 0, camp->text() );
        QString SQLQuery = "SELECT codigocompletoarticulo FROM alias LEFT JOIN articulo ON alias.idarticulo = articulo.idarticulo WHERE cadalias ~=~ $1";
        QString valors[1] = { camp->text() };
        BlDbRecordSet *cur = sf->mainCompany() ->loadQuery ( SQLQuery, 1, valors );
        if ( !cur->eof() ) {
            camp->setText( cur->valor ( "codigocompletoarticulo" ) ); 
        }
        delete cur;
      }
    }
    blDebug ( "END BfSubForm_on_mui_list_editFinished", 0 );

    return 0;

}

*/




int BlSubForm_editFinished ( BlSubForm *sub )
{
    blDebug ( "pluginbf_alias::BlSubForm_editFinished", 0 );
    if ( sub->m_campoactual->nomcampo() == "codigocompletoarticulo" ) {
      QString val = sub->m_campoactual->text();
        QString valors[1] = {val};
        QString query = "SELECT idarticulo FROM articulo WHERE codigocompletoarticulo = $1";
	BlDbSubFormField *camp = sub->m_campoactual;
        BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( query,1, valors);
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idarticulo", cur->valor ( "idarticulo" ) );
	} else {
           /// No hay codigos y buscamos alias
           if (posibleAlias(val, sub->mainCompany())) {
             QString SQLQuery = "SELECT articulo.idarticulo, codigocompletoarticulo FROM alias LEFT JOIN articulo ON alias.idarticulo = articulo.idarticulo WHERE cadalias ~=~ $1";
             BlDbRecordSet *cur1 = sub->mainCompany() ->loadQuery ( SQLQuery, 1, valors );
          
             if ( !cur1->eof() ) {
                sub->m_registrolinea->setDbValue ( "idarticulo", cur1->valor ( "idarticulo" ) );
                sub->m_registrolinea->setDbValue ( "codigocompletoarticulo", cur1->valor ( "codigocompletoarticulo" ) );
                camp->setText( cur1->valor ( "codigocompletoarticulo" ) ); 
	        camp->refresh();
             } // end if
             delete cur1;
           } // end if possible alias
        } // end if és codi d'article complet
        delete cur;
	blDebug ( "END pluginbf_alias::BlSubForm_editFinished", 0 );
	return 1;
    } // end if és buscador d'article
    blDebug ( "END pluginbf_alias::BlSubForm_editFinished", 0 );
    return 0;
}


int BlDbCompleterComboBox_textChanged (BlDbCompleterComboBox *bl) {
  blDebug("BlDbCompleterComboBox_textChanged", 0);

        if ( bl->m_entrada.size() >= 3 && bl->m_tabla == "articulo") {
           // no se si es el autoComplete o què però em criden a
           // aquesta senyal quan omplo el combo, amb el primer valor
           // i si no m'aturo ara, recalcularia el combo amb nomes
           // aquest valor encara que l'usuari nomes hagi escrit
           // un prefix que permeti mes candidats
          if ( bl->entrada().indexOf ( ".-" ) < 0 )  {
                QString cadwhere = "";
                /// Inicializamos los valores de vuelta a ""
                QString SQLQuery = "SELECT * FROM " + bl->m_tabla + " WHERE upper(codigocompletoarticulo) LIKE  upper($1||'%')";
                bl->m_cursorcombo = bl->mainCompany() ->load ( SQLQuery, bl->m_entrada  );
                bl->clear();
                while ( !bl->m_cursorcombo->eof() ) {
                    QMapIterator<QString, QString> i ( bl->m_valores );
                    QString cad = "";
                    QString sep = "";
                    QString cad1 = "";
                    while ( i.hasNext() ) {
                        i.next();
                        cad = cad + sep + bl->m_cursorcombo->valor ( i.key() );
                        if ( sep == "" ) {
                            cad1 = i.key();
                            sep = ".-";
                        } // end if
                    } // end while
                    bl->addItem ( cad , QVariant ( bl->m_cursorcombo->valor ( cad1 ) ) );
                    bl->m_cursorcombo->nextRecord();
                } // end while
                delete bl->m_cursorcombo;
                if (posibleAlias(bl->m_entrada,bl->mainCompany())) {
                  QString SQLQuery = "SELECT articulo.idarticulo, codigocompletoarticulo, cadalias, nomarticulo FROM alias LEFT JOIN articulo ON alias.idarticulo = articulo.idarticulo WHERE cadalias ~=~ $1";
                  bl->m_cursorcombo = bl->mainCompany() ->load( SQLQuery, bl->m_entrada );
                  while ( !bl->m_cursorcombo->eof() ) {
                    bl->addItem ( bl->m_cursorcombo->valor ( "codigocompletoarticulo" )
                                + ".-" + bl->m_cursorcombo->valor ( "nomarticulo" )
                                + " (" + bl->m_cursorcombo->valor ( "cadalias" ) + ") ",
                                QVariant ( bl->m_cursorcombo->valor ( "codigocompletoarticulo" ) )
                              );
                    bl->m_cursorcombo->nextRecord();
                  } // end while
                  delete bl->m_cursorcombo;
                }
           blDebug("END BlDbCompleterComboBox_textChanged", 0);

	   return 1;
           } //end if te .- 

        } // end if
  blDebug("END BlDbCompleterComboBox_textChanged", 0);

    return 0;
}






