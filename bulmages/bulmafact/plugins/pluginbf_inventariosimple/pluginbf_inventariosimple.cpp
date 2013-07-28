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

#include "pluginbf_inventariosimple.h"
#include "listinventariosimpleview.h"
#include "bfcompany.h"
#include "blfunctions.h"

#include "actividadview.h"


BfBulmaFact *g_pluginbf_inventariosimple = NULL;

///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_InventarioSimple", "0.11.1-0001");

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_inventariosimple", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );
    g_pluginbf_inventariosimple = bges;


    QMenu *pPluginMenu = bges->newMenu ( _("&Articulos"), "menuArticulos", "menuMaestro" );
    pPluginMenu->addSeparator();


    BlAction *accionA = new BlAction ( _ ( "&Inventarios" ), 0 );
    accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/product-list.png" ) ) );
    accionA->setStatusTip ( _ ( "Inventarios" ) );
    accionA->setWhatsThis ( _ ( "Inventarios" ) );
    accionA->setObjectName("mui_actionInventarios");
    pPluginMenu->addAction ( accionA );

    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    if (accion->objectName() == "mui_actionInventarios") {
        ListInventarioSimpleView *tar = new ListInventarioSimpleView ( g_pluginbf_inventariosimple->company(), NULL );
        g_pluginbf_inventariosimple->company()->m_pWorkspace->addSubWindow ( tar );
        tar->show();
    } // end if
    return 0;
}



int ActividadView_ActividadView(ActividadView *act) {
    BL_FUNC_DEBUG

    /// Agregamos el subformulario de validaciones.
    BfSubForm *l = new BfSubForm ( act );
    l->setObjectName ( QString::fromUtf8 ( "prestamo" ) );
    l->setMainCompany ( act->mainCompany() );
    l->setDbTableName ( "prestamo" );
    l->setDbFieldId ( "idprestamo" );
    l->addSubFormHeader ( "idprestamo", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView , _( "Id prestamo"));
    l->addSubFormHeader ( "idactividad", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _( "Id Actividad" ) );
    l->addSubFormHeader ( "idinventariosimple", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite , _( "Id producto" ) );
    l->addSubFormHeader ( "nominventariosimple", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _( "Articulo" ) );
    l->addSubFormHeader ( "fechaprestamo", BlDbField::DbDate, BlDbField::DbNothing | BlDbField::DbRequired, BlSubFormHeader::DbNone, _( "Fecha Prestamo" ) );
    l->addSubFormHeader ( "cantprestamo", BlDbField::DbNumeric, BlDbField::DbNotNull | BlDbField::DbRequired, BlSubFormHeader::DbNone , _( "Cantidad" ) );
    l->setInsert ( true );
    l->setDelete ( true );
    l->setSortingEnabled ( false );
    act->dialogChanges_setExcludedObject ( l->mui_list );

    act->mui_tab->addTab ( l, "Material" );
    l->load("SELECT * FROM prestamo NATURAL LEFT JOIN inventariosimple WHERE idprestamo IS NULL");
    return 0;
}





///
/**
\param art
\return
**/
int BlForm_guardar_Post ( BlForm *art )
{
    BL_FUNC_DEBUG
    try {

        BfSubForm *l = art->findChild<BfSubForm *> ( "prestamo" );
        if (l) {
          l->setColumnValue ( "idactividad", art->dbValue ( "idactividad" ) );
          l->save();
        } // end if

        
        return 0;
    } catch ( ... ) {
	blMsgError(_("Error al guardar."));
	
        return 0;
    } // end try
}



///
/**
\param art
\return
**/
int BlForm_load ( BlForm *art )
{
    BL_FUNC_DEBUG
    try {

        BfSubForm *l = art->findChild<BfSubForm *> ( "prestamo" );
        if (l) {
          l->load("SELECT * FROM prestamo LEFT JOIN inventariosimple ON prestamo.idinventariosimple = inventariosimple.idinventariosimple WHERE idactividad = " + art->dbValue("idactividad"));
        } // end if

        
        return 0;
    } catch ( ... ) {
	blMsgError(_("Error en la carga."));
	
        return 0;
    } // end try
}





int BfSubForm_pressedAsterisk ( BfSubForm *sub )
{
    BL_FUNC_DEBUG

    if ( sub->m_campoactual->fieldName() != "nominventariosimple" ) {
        
        return 0;
    } // end if

    BlSubForm *list = new BlSubForm ( NULL);
    list->setMainCompany(( BfCompany * ) sub->mainCompany());

    list->setDbTableName ( "inventariosimple" );
    list->setDbFieldId ( "idinventariosimple" );
    list->addSubFormHeader ( "idinventariosimple", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoWrite, _ ( "Id Inventario" ) );
    list->addSubFormHeader ( "nominventariosimple", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite, _ ( "Nombre" ) );
    list->addSubFormHeader ( "stockinventariosimple", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite, _ ( "Stock" ) );
    list->setInsert ( false );
    list->setOrdenEnabled ( true );
    list->load("SELECT * FROM inventariosimple");
    list->setSelectMode();
    sub->connect(list, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), list, SLOT(hide()));
    sub->connect(list, SIGNAL(editFinish(int, int)), list, SLOT(hide()));

    sub->setEnabled ( false );
    list->show();

    while ( !list->isHidden() )
        g_theApp->processEvents();
        
    sub->setEnabled ( true );

    QString idinv= list->dbValue("idinventariosimple");
    
    delete list;

    if ( idinv == "" ) {
        
        return 0;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM inventariosimple WHERE idinventariosimple = " + idinv );
    if ( !cur->eof() ) {
        sub->m_registrolinea->setDbValue ( "idinventariosimple", idinv );
        sub->m_registrolinea->setDbValue ( "nominventariosimple", cur->value( "nominventariosimple") );
    } // end if
    
    delete cur;

    
    
    

    return 0;
}









/// --------------------------------------------------------------
/// --------- Implemento la edicion de alumnos -----------------
/// Con esta funcionalidad creamos menus contextuales en todos los subformularios donde
/// Aparezca el identificador de alumno como elemento y permite de forma sencilla
/// La creacion, la edicion, y la seleccion.
///


/**
\param parent
**/
SubForm_InventarioSimple::SubForm_InventarioSimple ( BlSubForm *parent ) : QObject ( parent )
{
    BL_FUNC_DEBUG
    
}

///
/**
**/
SubForm_InventarioSimple::~SubForm_InventarioSimple()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu
**/
void SubForm_InventarioSimple::s_pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
    BfSubForm *sub = ( BfSubForm * ) parent();
    BlSubFormHeader *header = sub->header ( "nominventariosimple" );
    if ( header ) {
        menu->addSeparator();
        if ( ! ( header->options() & BlSubFormHeader::DbNoWrite ) )  {
            menu->addAction ( QIcon ( ":/Images/product-family.png" ), _ ( "Seleccionar Material" ) );
        } // end if
    } // end if
    
}


///
/**
\param action
**/
void SubForm_InventarioSimple::s_trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG
    BfSubForm *sub = ( BfSubForm * ) parent();
    if ( action->text() == _ ( "Seleccionar Material" ) ) {
        seleccionarMaterial ();
    } // end if
    
    
}


///
/**
**/
void SubForm_InventarioSimple::seleccionarMaterial ( BfSubForm *sub )
{
    BL_FUNC_DEBUG
    
    if (!sub) sub= (BfSubForm *) parent();
    
    BlSubForm *list = new BlSubForm ( NULL);
    list->setMainCompany(( BfCompany * ) sub->mainCompany());

    list->setDbTableName ( "inventariosimple" );
    list->setDbFieldId ( "idinventariosimple" );
    list->addSubFormHeader ( "idinventariosimple", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoWrite, _ ( "Id Inventario" ) );
    list->addSubFormHeader ( "nominventariosimple", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite, _ ( "Nombre" ) );
    list->addSubFormHeader ( "stockinventariosimple", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite, _ ( "Stock" ) );
    list->setInsert ( false );
    list->setOrdenEnabled ( true );
    list->load("SELECT * FROM inventariosimple");
    list->setSelectMode();
    sub->connect(list, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), list, SLOT(hide()));
    sub->connect(list, SIGNAL(editFinish(int, int)), list, SLOT(hide()));

    sub->setEnabled ( false );
    list->show();

    while ( !list->isHidden() )
        g_theApp->processEvents();
        
    sub->setEnabled ( true );

    QString idinv= list->dbValue("idinventariosimple");
    
    delete list;
    
    if ( idinv == "" ) {
        
        return ;
    } // end if

    QString query = "SELECT * FROM inventariosimple WHERE idinventariosimple = " + idinv;
    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( query );
    if ( !cur->eof() ) {
        sub->lineaact()->setDbValue ( "idinventariosimple", idinv );
        sub->lineaact()->setDbValue ( "nominventariosimple", cur->value( "nominventariosimple") );
    } // end if
    
    delete cur;


    
}


///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    BL_FUNC_DEBUG
    SubForm_InventarioSimple *subformods = new SubForm_InventarioSimple ( (BfSubForm *) sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    
    return 0;
}



/// Miramos de poner los iconos del menu de subformularios
///
/**
\param sub
\return
**/
int BlSubForm_preparaMenu ( BlSubForm *sub ) {
    BL_FUNC_DEBUG

    BlSubFormHeader *header = sub->header ( "nominventariosimple" );
    if (!header) 
        header = sub->header ( "nominventariosimple" );
    if ( header ) {
        SubForm_InventarioSimple *subformods = new SubForm_InventarioSimple ( sub );
        
        QHBoxLayout *m_hboxLayout1 = sub->mui_menusubform->findChild<QHBoxLayout *> ( "hboxLayout1" );
        if ( !m_hboxLayout1 ) {
            m_hboxLayout1 = new QHBoxLayout ( sub->mui_menusubform );
            m_hboxLayout1->setSpacing (0 );
            m_hboxLayout1->setMargin ( 0 );
            m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
        } // end if
        
        if ( ! ( header->options() & BlSubFormHeader::DbNoWrite ) )  {
          QToolButton *sel1 = new QToolButton ( sub->mui_menusubform );
          sel1->setStatusTip ( "Seleccionar Material" );
          sel1->setToolTip ( "Seleccionar Material" );
          sel1->setMinimumSize ( QSize ( 18, 18 ) );
          sel1->setIcon ( QIcon ( ":/Images/product-family.png" ) );
          sel1->setIconSize ( QSize ( 18, 18 ) );    
          m_hboxLayout1->addWidget ( sel1 );
          sel1->connect (sel1, SIGNAL(released()), subformods, SLOT(seleccionarMaterial()));
        } // end if
    } // end if
    

    
    return 0;
}








