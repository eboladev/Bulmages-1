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


QString g_minAlias =  "";
QString g_maxAlias =  "";

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
    BL_FUNC_DEBUG
    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_Alias", "0.11.2");

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_alias", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_ultimRefrescAlias.start();
    return 0;
}


void invalidaEstadAlias ( void )
{
    BL_FUNC_DEBUG
    g_minAlias = ( QString ) "";
    g_maxAlias = ( QString ) "";
    g_minLenAlias = 0;
    g_maxLenAlias = 0;
}


bool posibleAlias ( QString alias, BlMainCompany *eb )
{
    BL_FUNC_DEBUG
    if ( MILISEG_REFRESCO_ESTAD_ALIAS && ( g_ultimRefrescAlias.elapsed() > MILISEG_REFRESCO_ESTAD_ALIAS ) ) {
        invalidaEstadAlias();
    }// end if
    if ( ( g_minAlias ==  "" ) && ( g_maxAlias == "" ) ) {
        BlDbRecordSet *cur = eb ->loadQuery (
                                 "SELECT \
   (SELECT cadalias FROM alias ORDER by cadalias  ASC LIMIT 1) as minalias,\
   (SELECT cadalias FROM alias ORDER by cadalias  DESC LIMIT 1) as maxalias,\
   (SELECT max(length(cadalias)) FROM alias ) as maxlenalias,\
   (SELECT min(length(cadalias)) FROM alias ) as minlenalias" );
	
        g_minAlias = cur->value( "minalias" );
        g_maxAlias = cur->value( "maxalias" );
        g_minLenAlias = cur->valorInt ( "minlenalias" );
        g_maxLenAlias = cur->valorInt ( "maxlenalias" );

	delete cur;
        g_ultimRefrescAlias.restart();
    }// end if
    return ( ( g_minAlias !=  "" ) && ( g_maxAlias !=  "" )
             && ( alias != "" )
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
    BL_FUNC_DEBUG

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
    art->dialogChanges_setExcludedObject ( l->mui_list );

    art->mui_tab->addTab ( l, "Alias" );

    
    return 0;
}


///
/**
\param art
\return
**/
int ArticuloView_load ( ArticuloView *art )
{
    BL_FUNC_DEBUG
    BfSubForm *l = art->findChild<BfSubForm *> ( "lalias" );
    if ( l ) {
        l->load ( "SELECT * FROM alias WHERE idarticulo = " + art->dbValue ( "idarticulo" ) );
    } // end if
    
    return 0;
}



///
/**
\param art
\return
**/
int ArticuloView_guardar_post ( ArticuloView *art )
{
    BL_FUNC_DEBUG
    try {
        BfSubForm *l = art->findChild<BfSubForm *> ( "lalias" );
        l->setColumnValue ( "idarticulo", art->dbValue ( "idarticulo" ) );
        l->save();
        invalidaEstadAlias();
        return 0;
    } catch ( ... ) {
        
        return 0;
    } // end try

    
}


int Busqueda_on_m_inputBusqueda_textChanged ( BlSearchWidget *busc )
{
    BL_FUNC_DEBUG
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
                busc->setId ( cur->value( "idarticulo" ) );
                encontrado = TRUE;
            }// end if
            delete cur;
        }// end if

        if ( encontrado ) {
            return -1;
        } // end if

    } // end if
    return 0;
}


int BlSubForm_editFinished ( BlSubForm *sub )
{
    BL_FUNC_DEBUG
    if ( sub->m_campoactual->fieldName() == "codigocompletoarticulo" ) {
      /// Buscamos el codigo correspondiente.
        QString val = sub->m_campoactual->text();
        QString query = "SELECT idarticulo FROM articulo WHERE codigocompletoarticulo = '"+ sub->mainCompany() ->sanearCadena(val) + "'";
	BlDbSubFormField *camp = sub->m_campoactual;
        BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( query);

        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idarticulo", cur->value( "idarticulo" ) );
	} else {

           /// No hay codigos, pero puede que exista algun alias que se corresponda.
           if (posibleAlias(val, sub->mainCompany())) {
             QString SQLQuery = "SELECT articulo.idarticulo, codigocompletoarticulo FROM alias LEFT JOIN articulo ON alias.idarticulo = articulo.idarticulo WHERE cadalias LIKE '" + sub->mainCompany() ->sanearCadena(val) + "'||'%'";
             BlDbRecordSet *cur1 = sub->mainCompany() ->loadQuery ( SQLQuery );
          
             if ( !cur1->eof() ) {
                sub->m_registrolinea->setDbValue ( "idarticulo", cur1->value( "idarticulo" ) );
                sub->m_registrolinea->setDbValue ( "codigocompletoarticulo", cur1->value( "codigocompletoarticulo" ) );
                camp->setText( cur1->value( "codigocompletoarticulo" ) ); 
	        camp->refresh();
             } // end if

             delete cur1;

           } // end if

        } // end if

        delete cur;
	
	return 1;

    } // end if
    
    return 0;
}


int BlDbCompleterComboBox_textChanged (BlDbCompleterComboBox *bl)
{
  BL_FUNC_DEBUG

        if ( bl->m_entrada.size() >= g_confpr->value(CONF_NUMCHAR_RELOAD_FILTRO).toInt() && bl->m_tabla == "articulo") {
           // no se si es el autoComplete o què però em criden a
           // aquesta senyal quan omplo el combo, amb el primer valor
           // i si no m'aturo ara, recalcularia el combo amb nomes
           // aquest valor encara que l'usuari nomes hagi escrit
           // un prefix que permeti mes candidats
          if ( bl->entrada().indexOf ( ".-" ) < 0 )  {
		QString val = bl->m_entrada;
                /// Inicializamos los valores de vuelta a ""
                QString SQLQuery = "SELECT * FROM " + bl->m_tabla + " WHERE upper(codigocompletoarticulo) LIKE  upper('" + bl->mainCompany() ->sanearCadena(val) + "'||'%') OR upper(nomarticulo) LIKE  upper('" + bl->mainCompany() ->sanearCadena(val) + "'||'%') ORDER BY nomarticulo";
		bl->m_cursorcombo = bl->mainCompany() ->load ( SQLQuery );
                bl->clear();
                while ( !bl->m_cursorcombo->eof() ) {
                    QMapIterator<QString, QString> i ( bl->m_valores );
                    QString cad = "";
                    QString sep = "";
                    QString cad1 = "";
                    while ( i.hasNext() ) {
                        i.next();
                        cad = cad + sep + bl->m_cursorcombo->value( i.key() );
                        if ( sep == "" ) {
                            cad1 = i.key();
                            sep = ".-";
                        } // end if
                    } // end while
                    bl->addItem ( cad , QVariant ( bl->m_cursorcombo->value( cad1 ) ) );
                    bl->m_cursorcombo->nextRecord();
                } // end while
                delete bl->m_cursorcombo;
                if (posibleAlias(bl->m_entrada,bl->mainCompany())) {
		  /// Con los alias no vamos a hacer distincion de mayusculas  y minusculas.
                  QString SQLQuery = "SELECT articulo.idarticulo, codigocompletoarticulo, cadalias, nomarticulo FROM alias LEFT JOIN articulo ON alias.idarticulo = articulo.idarticulo WHERE cadalias LIKE '" + bl->mainCompany() ->sanearCadena(val) + "%' ORDER BY nomarticulo";

                  bl->m_cursorcombo = bl->mainCompany() ->load( SQLQuery );
                  while ( !bl->m_cursorcombo->eof() ) {
                    bl->addItem ( bl->m_cursorcombo->value( "codigocompletoarticulo" )
                                + ".-" + bl->m_cursorcombo->value( "nomarticulo" )
                                + " (" + bl->m_cursorcombo->value( "cadalias" ) + ") ",
                                QVariant ( bl->m_cursorcombo->value( "codigocompletoarticulo" ) )
                              );
                    bl->m_cursorcombo->nextRecord();
                  } // end while
                  delete bl->m_cursorcombo;
                }

	   return 1;
           } //end if te .- 

        } // end if

    return 0;
}

