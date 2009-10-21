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
    _depura ( "myplugininvsimple::myplugininvsimple", 0 );
    _depura ( "END myplugininvsimple::myplugininvsimple", 0 );
}


///
/**
**/
myplugininvsimple::~myplugininvsimple()
{
    _depura ( "myplugininvsimple::~myplugininvsimple", 0 );
    _depura ( "END myplugininvsimple::~myplugininvsimple", 0 );
}


///
/**
**/
void myplugininvsimple::elslot()
{
    _depura ( "myplugininvsimple::elslot", 0 );

    ListInventarioSimpleView *tar = new ListInventarioSimpleView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addWindow ( tar );
    tar->show();

    _depura ( "END myplugininvsimple::elslot", 0 );
}


///
/**
\param bges
**/
void myplugininvsimple::inicializa ( BfBulmaFact *bges )
{
    _depura ( "myplugininvsimple::inicializa", 0 );
    /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
    m_bges = bges;
    setMainCompany ( bges->company() );

    /// Miramos si existe un menu Articulos
    QMenu *pPluginMenu = bges->newMenu ( _("&Articulos"), "menuArticulos", "menuMaestro" );
    pPluginMenu->addSeparator();


    QAction *planCuentas = new QAction ( _ ( "&Inventarios" ), 0 );
    planCuentas->setStatusTip ( _ ( "Inventarios" ) );
    planCuentas->setWhatsThis ( _ ( "Inventarios" ) );

    pPluginMenu->addAction ( planCuentas );
    connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    _depura ( "END myplugininvsimple::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    _depura ( "Punto de Entrada del plugin de Inventario Simple\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_inventariosimple", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    myplugininvsimple *plug = new myplugininvsimple();
    plug->inicializa ( bges );
    return 0;
}

int ActividadView_ActividadView(ActividadView *act) {
  _depura("pluginbf_inventariosimple::ActividadView_ActividadView", 0);

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
    _depura("END pluginbf_inventariosimple::ActividadView_ActividadView", 0);
    return 0;
}





///
/**
\param art
\return
**/
int BlForm_guardar_Post ( BlForm *art )
{
    _depura ( "pluginbf_inventariosimple::BlForm_guardar_Post", 0 );
    try {

        BfSubForm *l = art->findChild<BfSubForm *> ( "prestamo" );
        if (l) {
          l->setColumnValue ( "idactividad", art->dbValue ( "idactividad" ) );
          l->guardar();
        } // end if

       _depura ( "END pluginbf_inventariosimple::BlForm_guardar_Post", 0 );
        return 0;
    } catch ( ... ) {
        _depura ( "Hubo un error al guardar los vencimientos", 2 );
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
    _depura ( "pluginbf_inventariosimple::BlForm_cargar", 0 );
    try {

        BfSubForm *l = art->findChild<BfSubForm *> ( "prestamo" );
        if (l) {
          l->cargar("SELECT * FROM prestamo LEFT JOIN inventariosimple ON prestamo.idinventariosimple = inventariosimple.idinventariosimple WHERE idactividad = " + art->dbValue("idactividad"));
        } // end if

       _depura ( "END pluginbf_inventariosimple::BlForm_cargar", 0 );
        return 0;
    } catch ( ... ) {
        _depura ( "Hubo un error al guardar los vencimientos", 2 );
        return 0;
    } // end try
}





int BfSubForm_pressedAsterisk ( BfSubForm *sub )
{
    _depura ( "BfSubForm_pressedAsterisk" );

    if ( sub->m_campoactual->nomcampo() != "nominventariosimple" ) {
        _depura ( "END BfSubForm::pressedAsterisk", 0 );
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
        _depura ( "END BfSubForm_pressedAsterisk" );
        return 0;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM inventariosimple WHERE idinventariosimple = " + idinv );
    if ( !cur->eof() ) {
        sub->m_registrolinea->setDbValue ( "idinventariosimple", idinv );
        sub->m_registrolinea->setDbValue ( "nominventariosimple", cur->valor( "nominventariosimple") );
    } // end if
    
    delete cur;




/*
    AlumnosList *alumnoslist = new AlumnosList ( ( BfCompany * ) sub->mainCompany(), NULL, 0, BL_SELECT_MODE );
    
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    sub->setEnabled ( false );
    alumnoslist->show();
    
    while ( !alumnoslist->isHidden() )
        g_theApp->processEvents();
        
    sub->setEnabled ( true );
    QString idAlumno = alumnoslist->idalumno();
    
    delete alumnoslist;

    /// Si no tenemos un idarticulo salimos ya que significa que no se ha seleccionado ninguno.
    if ( idAlumno == "" ) {
        _depura ( "END BfSubForm::pressedAsterisk", 0 );
        return 0;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM alumno WHERE idalumno = " + idAlumno );
    if ( !cur->eof() ) {
        sub->m_registrolinea->setDbValue ( "idalumno", idAlumno );
        sub->m_registrolinea->setDbValue ( "nombrealumno1", cur->valor( "apellido1alumno") + " " + cur->valor("apellido2alumno") + ", " + cur->valor ( "nombrealumno" ));
    } // end if
    
    delete cur;
*/    
    _depura ( "END BfSubForm_pressedAsterisk" );

    return 0;
}

