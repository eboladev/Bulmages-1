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

///
/**
**/
myplugininvsimple::myplugininvsimple()
{
    blDebug ( "myplugininvsimple::myplugininvsimple", 0 );
    blDebug ( "END myplugininvsimple::myplugininvsimple", 0 );
}


///
/**
**/
myplugininvsimple::~myplugininvsimple()
{
    blDebug ( "myplugininvsimple::~myplugininvsimple", 0 );
    blDebug ( "END myplugininvsimple::~myplugininvsimple", 0 );
}


///
/**
**/
void myplugininvsimple::elslot()
{
    blDebug ( "myplugininvsimple::elslot", 0 );

    ListInventarioSimpleView *tar = new ListInventarioSimpleView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( tar );
    tar->show();

    blDebug ( "END myplugininvsimple::elslot", 0 );
}


///
/**
\param bges
**/
void myplugininvsimple::inicializa ( BfBulmaFact *bges )
{
    blDebug ( "myplugininvsimple::inicializa", 0 );
    /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
    m_bges = bges;
    setMainCompany ( bges->company() );

    /// Miramos si existe un menu Articulos
    QMenu *pPluginMenu = bges->newMenu ( _("&Inventari"), "menuInventari", "menuMaestro" );
    pPluginMenu->addSeparator();


    QAction *planCuentas = new QAction ( _ ( "&Inventarios" ), 0 );
    planCuentas->setStatusTip ( _ ( "Inventarios" ) );
    planCuentas->setWhatsThis ( _ ( "Inventarios" ) );

    pPluginMenu->addAction ( planCuentas );
    connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    blDebug ( "END myplugininvsimple::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de Entrada del plugin de Inventario Simple\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_inventariosimple", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    myplugininvsimple *plug = new myplugininvsimple();
    plug->inicializa ( bges );
    return 0;
}

int ActividadView_ActividadView(ActividadView *act) {
  blDebug("pluginbf_inventariosimple::ActividadView_ActividadView", 0);

    /// Agregamos el subformulario de validaciones.
    BfSubForm *l = new BfSubForm ( act );
    l->setObjectName ( QString::fromUtf8 ( "prestamo" ) );
    l->setMainCompany ( act->mainCompany() );
    l->setDbTableName ( "prestamo" );
    l->setDbFieldId ( "idprestamo" );
    l->addSubFormHeader ( "idprestamo", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView , _( "Id prestamo"));
    l->addSubFormHeader ( "idactividad", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _( "Id Actividad" ) );
    l->addSubFormHeader ( "idinventariosimple", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite , _( "Id producto" ) );
    l->addSubFormHeader ( "nominventariosimple", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _( "Articulo" ) );
    l->addSubFormHeader ( "fechaprestamo", BlDbField::DbDate, BlDbField::DbNothing, BlSubFormHeader::DbNone, _( "Fecha Prestamo" ) );
    l->addSubFormHeader ( "cantprestamo", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone , _( "Cantidad" ) );
    l->setInsert ( TRUE );
    l->setDelete ( TRUE );
    l->setSortingEnabled ( FALSE );
    act->dialogChanges_setQObjectExcluido ( l->mui_list );

    act->mui_tab->addTab ( l, "Material" );
    l->cargar("SELECT * FROM prestamo NATURAL LEFT JOIN inventariosimple WHERE idprestamo IS NULL");
    blDebug("END pluginbf_inventariosimple::ActividadView_ActividadView", 0);
    return 0;
}





///
/**
\param art
\return
**/
int BlForm_guardar_Post ( BlForm *art )
{
    blDebug ( "pluginbf_inventariosimple::BlForm_guardar_Post", 0 );
    try {

        BfSubForm *l = art->findChild<BfSubForm *> ( "prestamo" );
        if (l) {
          l->setColumnValue ( "idactividad", art->dbValue ( "idactividad" ) );
          l->guardar();
        } // end if

       blDebug ( "END pluginbf_inventariosimple::BlForm_guardar_Post", 0 );
        return 0;
    } catch ( ... ) {
        blDebug ( "Hubo un error al guardar los vencimientos", 2 );
        return 0;
    } // end try
}



///
/**
\param art
\return
**/
int BlForm_cargar ( BlForm *art )
{
    blDebug ( "pluginbf_inventariosimple::BlForm_cargar", 0 );
    try {

        BfSubForm *l = art->findChild<BfSubForm *> ( "prestamo" );
        if (l) {
          l->cargar("SELECT * FROM prestamo LEFT JOIN inventariosimple ON prestamo.idinventariosimple = inventariosimple.idinventariosimple WHERE idactividad = " + art->dbValue("idactividad"));
        } // end if

       blDebug ( "END pluginbf_inventariosimple::BlForm_cargar", 0 );
        return 0;
    } catch ( ... ) {
        blDebug ( "Hubo un error al guardar los vencimientos", 2 );
        return 0;
    } // end try
}





int BfSubForm_pressedAsterisk ( BfSubForm *sub )
{
    blDebug ( "BfSubForm_pressedAsterisk" );

    if ( sub->m_campoactual->nomcampo() != "nominventariosimple" ) {
        blDebug ( "END BfSubForm::pressedAsterisk", 0 );
        return 0;
    } // end if

    BlSubForm *list = new BlSubForm ( NULL);
    list->setMainCompany(( BfCompany * ) sub->mainCompany());

    list->setDbTableName ( "inventariosimple" );
    list->setDbFieldId ( "idinventariosimple" );
    list->addSubFormHeader ( "idinventariosimple", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoWrite, _ ( "Id Inventario" ) );
    list->addSubFormHeader ( "nominventariosimple", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite, _ ( "Nombre" ) );
    list->addSubFormHeader ( "stockinventariosimple", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite, _ ( "Stock" ) );
    list->setInsert ( FALSE );
    list->setOrdenEnabled ( TRUE );
    list->cargar("SELECT * FROM inventariosimple");
    list->setModoConsulta();
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
        blDebug ( "END BfSubForm_pressedAsterisk" );
        return 0;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM inventariosimple WHERE idinventariosimple = " + idinv );
    if ( !cur->eof() ) {
        sub->m_registrolinea->setDbValue ( "idinventariosimple", idinv );
        sub->m_registrolinea->setDbValue ( "nominventariosimple", cur->valor( "nominventariosimple") );
    } // end if
    
    delete cur;

    
    
    blDebug ( "END BfSubForm_pressedAsterisk" );

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
MyPlugInv1::MyPlugInv1 ( BlSubForm *parent ) : QObject ( parent )
{
    blDebug ( "MyPlugInv1::MyPlugInv1", 0 );
    blDebug ( "END MyPlugInv1::MyPlugInv1", 0 );
}

///
/**
**/
MyPlugInv1::~MyPlugInv1()
{
    blDebug ( "MyPlugInv1::~MyPlugInv1", 0 );
    blDebug ( "END MyPlugInv1::~MyPlugInv1", 0 );
}


///
/**
\param menu
**/
void MyPlugInv1::s_pintaMenu ( QMenu *menu )
{
    blDebug ( "MyPlugInv1::s_pintaMenu", 0 );
    BfSubForm *sub = ( BfSubForm * ) parent();
    BlSubFormHeader *header = sub->header ( "nominventariosimple" );
    if ( header ) {
        menu->addSeparator();
        if ( ! ( header->options() & BlSubFormHeader::DbNoWrite ) )  {
            menu->addAction ( QIcon ( ":/Images/product-family.png" ), _ ( "Seleccionar Material" ) );
        } // end if
    } // end if
    blDebug ( "END MyPlugInv1::s_pintaMenu", 0 );
}


///
/**
\param action
**/
void MyPlugInv1::s_trataMenu ( QAction *action )
{
    blDebug ( "MyPlugInv1::s_trataMenu", 0 );
    BfSubForm *sub = ( BfSubForm * ) parent();
    if ( action->text() == _ ( "Seleccionar Material" ) ) {
        seleccionarMaterial ();
    } // end if
    
    blDebug ( "END MyPlugInv1::s_trataMenu", 0 );
}


///
/**
**/
void MyPlugInv1::seleccionarMaterial ( BfSubForm *sub )
{
    blDebug ( metaObject()->className()+ QString("::seleccionarMaterial"), 0 );
    
    if (!sub) sub= (BfSubForm *) parent();
    
    
    
    BlSubForm *list = new BlSubForm ( NULL);
    list->setMainCompany(( BfCompany * ) sub->mainCompany());

    list->setDbTableName ( "inventariosimple" );
    list->setDbFieldId ( "idinventariosimple" );
    list->addSubFormHeader ( "idinventariosimple", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoWrite, _ ( "Id Inventario" ) );
    list->addSubFormHeader ( "nominventariosimple", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite, _ ( "Nombre" ) );
    list->addSubFormHeader ( "stockinventariosimple", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite, _ ( "Stock" ) );
    list->setInsert ( FALSE );
    list->setOrdenEnabled ( TRUE );
    list->cargar("SELECT * FROM inventariosimple");
    list->setModoConsulta();
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
        blDebug ( "END MyPlugInv1::editarMaterial" );
        return ;
    } // end if

    QString query = "SELECT * FROM inventariosimple WHERE idinventariosimple = " + idinv;
    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( query );
    if ( !cur->eof() ) {
        sub->lineaact()->setDbValue ( "idinventariosimple", idinv );
        sub->lineaact()->setDbValue ( "nominventariosimple", cur->valor( "nominventariosimple") );
    } // end if
    
    delete cur;


    blDebug ( "END MyPlugInv1::seleccionarMaterial", 0 );
}


///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    blDebug ( "BlSubForm_BlSubForm_Post", 0 );
    MyPlugInv1 *subformods = new MyPlugInv1 ( (BfSubForm *) sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    blDebug ( "END BlSubForm_BlSubForm_Post", 0 );
    return 0;
}



/// Miramos de poner los iconos del menu de subformularios
///
/**
\param sub
\return
**/
int BlSubForm_preparaMenu ( BlSubForm *sub ) {
    blDebug ( "BlSubForm_preparaMenu", 0 );

    BlSubFormHeader *header = sub->header ( "nominventariosimple" );
    if (!header) 
        header = sub->header ( "nominventariosimple" );
    if ( header ) {
        MyPlugInv1 *subformods = new MyPlugInv1 ( sub );
        
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
    

    blDebug ( "END BlSubForm_preparaMenu", 0 );
    return 0;
}








