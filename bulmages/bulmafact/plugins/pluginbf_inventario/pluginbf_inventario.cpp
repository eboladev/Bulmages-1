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

#include "pluginbf_inventario.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "inventariosview.h"

BfBulmaFact *g_pluginbf_inventario = NULL;

///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_Inventario", "0.10.1-0002");

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_inventario", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );



    g_pluginbf_inventario = bges;

    /// Miramos si existe un menu Inventario
    QMenu *pPluginMenu = bges->newMenu ( _("&Articulos"), "menuArticulos", "menuMaestro" );
    pPluginMenu->addSeparator();


    BlAction *accionA = new BlAction ( _ ( "&Inventarios" ), 0 );
    accionA->setStatusTip ( _ ( "Inventarios" ) );
    accionA->setWhatsThis ( _ ( "Inventarios" ) );
    accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/product-list.png" ) ) );
    accionA->setObjectName("mui_actionInventario");

    pPluginMenu->addAction ( accionA );

    return 0;
}

int BlAction_actionTriggered(BlAction *accion) {
    BlDebug::blDebug ( Q_FUNC_INFO, 0, _("PluginBf_Inventario") );

    if (accion->objectName() == "mui_actionInventario") {
        InventariosView *tar = new InventariosView ( g_pluginbf_inventario->company(), NULL );
        g_pluginbf_inventario->company()->m_pWorkspace->addSubWindow ( tar );
        tar->show();
    } // end if

    
    return 0;
}


///
/**
\param art
\return
**/
int ArticuloView_ArticuloView ( ArticuloView *art )
{
    BL_FUNC_DEBUG
    BfSubForm *l = new BfSubForm ( art );
    l->setMainCompany ( art->mainCompany() );
    l->setObjectName ( QString::fromUtf8 ( "m_lmin" ) );
    art->mui_tab->addTab ( l, _ ( "Minimos almacen" ) );

    l->setDbTableName ( "minimsalmacen" );
    l->setDbFieldId ( "idminimsalmacen" );
    l->addSubFormHeader ( "nomalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Almacen" ) );
    l->addSubFormHeader ( "idalmacen", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite, _ ( "Id Almacen" ) );
    l->addSubFormHeader ( "valminimsalmacen", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Minimo" ) );
    l->addSubFormHeader ( "idminimsalmacen", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id minimo almacen" ) );
    l->addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id articulo" ) );
    l->setInsert ( TRUE );
    l->setDelete ( TRUE );
    l->setSortingEnabled ( FALSE );
    art->dialogChanges_setExcludedObject ( l->mui_list );
    
    return 0;
}


///
/**
\param fich
\return
**/
int BlForm_load ( BlForm *fich )
{
    BL_FUNC_DEBUG
    BlSubForm * form = fich->findChild<BlSubForm *> ( "m_lmin" );
    if ( form )
        form->load ( "SELECT * FROM minimsalmacen NATURAL LEFT JOIN almacen WHERE idarticulo = " + fich->dbValue ( "idarticulo" ) );
    return 0;
}



int BlForm_guardar_Post ( BlForm *fich )
{
    BL_FUNC_DEBUG
    BlSubForm * form = fich->findChild<BlSubForm *> ( "m_lmin" );
    if ( form ) {
        if (form->rowCount() > 0) {
	  form->setColumnValue ( "idarticulo", fich->dbValue ( "idarticulo" ) );
	  int i = form->rowCount();
	   while (--i >= 0) {
	    if ((form->lineaat(i)->dbValue("valminimsalmacen") == "0,00") && (form->lineaat(i)->dbValue("idalmacen") != "")) {
		//form->remove(i);
		form->lineaat(i)->setDbValue("idarticulo", "");
		form->lineaat(i)->setDbValue("idalmacen", "");
	    } // end if
	  } // end while
	  form->save();
	} // end if
    } // end if
    return 0;
}


int BfSubForm_on_mui_list_editFinished ( BfSubForm *subform )
{
    BL_FUNC_DEBUG

    BlDbSubFormField *camp = ( BlDbSubFormField * ) subform->item ( subform->currentRow(), subform->currentColumn()  );
    /// Si no hay campo actual salimos directamente.
    if (!camp) return 0;
    camp->refresh();
    if ( camp->fieldName() == "cant" + subform->tableName() ) {
      
        BlDbSubFormRecord * rec = subform->lineaat ( subform->currentRow() );
        QObject *wid = subform->parent();
        while ( wid
                && ( wid->objectName() != "PresupuestoClienteBase" )
                && ( wid->objectName() != "PedidoClienteBase" )
                && ( wid->objectName() != "AlbaranClienteBase" )
              ) {
            wid = wid->parent();
        } // end if

        if ( !wid ) return 0;

        BlForm *fich = ( BlForm * ) wid;
	BlComboBox *combo = wid->findChild<BlComboBox *>("mui_idalmacen");
	if (!combo) return 0;
	
        QString idalmacen = combo->id ();
        if ( idalmacen == "" ) return 0;

        if ( rec->dbValue ( "idarticulo" ) == "" ) return 0;

	
        QString query1 = "SELECT * FROM stock_almacen where idarticulo=" + rec->dbValue ( "idarticulo" ) + " AND idalmacen = " + idalmacen;
        BlDbRecordSet *cur1 = subform->mainCompany() ->loadQuery ( query1 );
        BlFixed stock ( "0" );
        if ( !cur1 ) return 0;
        if ( !cur1->eof() ) {
            stock = BlFixed ( cur1->value( "stock" ) );
        } // end if
        delete cur1;

        QString query = "SELECT * FROM minimsalmacen where idarticulo=" + rec->dbValue ( "idarticulo" ) + " AND idalmacen = " + idalmacen;
        BlDbRecordSet *cur = subform->mainCompany() ->loadQuery ( query );
        if ( !cur ) return 0;
        if ( !cur->eof() ) {
            BlFixed val = BlFixed ( cur->value( "valminimsalmacen" ) );
            BlFixed valb = BlFixed ( camp->fieldValue() );
            if ( stock - valb <= val )
                blMsgWarning ( _ ( "Stock minimo superado" ) );
        } // end if
        delete cur;
    } // end if
    return 0;
}



int BlSubForm_campoCompleto ( BfSubForm *subform )
{
    BL_FUNC_DEBUG
    
    /// Si el subformulario no tiene filas entonces no tiene sentido seguir. Y produce un error en la búsqueda de un dbValue.
    if (subform->rowCount() == 0 || subform->currentRow() < 0) {
      return 0;
    } // end if
  
  
    QString camp1 = subform->dbValue("cantlpresupuesto");
    if (camp1 == "")
	camp1 = subform->dbValue("cantlpedidocliente");
    if (camp1 == "")
	camp1 = subform->dbValue("cantlalbaran");

    if ( camp1 != "" ) {
      
        BlDbSubFormRecord * rec = subform->lineaat ( subform->currentRow() );
	
        QObject *wid = subform->parent();
        while ( wid
                && ( wid->objectName() != "PresupuestoClienteBase" )
                && ( wid->objectName() != "PedidoClienteBase" )
                && ( wid->objectName() != "AlbaranClienteBase" )
              ) {
            wid = wid->parent();
        } // end if

        if ( !wid ) return 0;

        BlForm *fich = ( BlForm * ) wid;
	BlComboBox *combo = wid->findChild<BlComboBox *>("mui_idalmacen");
	if (!combo) return 0;
	
        QString idalmacen = combo->id ();
        if ( idalmacen == "" ) return 0;

        if ( rec->dbValue ( "idarticulo" ) == "" ) return 0;

	
        QString query1 = "SELECT * FROM stock_almacen where idarticulo=" + rec->dbValue ( "idarticulo" ) + " AND idalmacen = " + idalmacen;
        BlDbRecordSet *cur1 = subform->mainCompany() ->loadQuery ( query1 );
        BlFixed stock ( "0" );
        if ( !cur1 ) return 0;
        if ( !cur1->eof() ) {
            stock = BlFixed ( cur1->value( "stock" ) );
        } // end if
        delete cur1;

        QString query = "SELECT * FROM minimsalmacen where idarticulo=" + rec->dbValue ( "idarticulo" ) + " AND idalmacen = " + idalmacen;
        BlDbRecordSet *cur = subform->mainCompany() ->loadQuery ( query );
        if ( !cur ) return 0;
        if ( !cur->eof() ) {
            BlFixed val = BlFixed ( cur->value( "valminimsalmacen" ) );
            BlFixed valb = BlFixed ( camp1 );
            if ( stock - valb <= val )
                blMsgWarning ( _ ( "Stock minimo superado" ) );
        } // end if
        delete cur;
    } // end if
    return 0;
}

