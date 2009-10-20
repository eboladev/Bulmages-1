/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#include "pluginbf_alumno.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "alumnoview.h"
#include "alumnoslist.h"
#include "listcuotasporalumnoview.h"
#include "claseview.h"

AlumnosList *g_alumnosList = NULL;

///
/**
**/
MyPlugProf::MyPlugProf()
{
    _depura ( "MyPlugProf::MyPlugProf", 0 );
    _depura ( "END MyPlugProf::MyPlugProf", 0 );
}

///
/**
**/
MyPlugProf::~MyPlugProf()
{
    _depura ( "MyPlugProf::~MyPlugProf", 0 );
    _depura ( "END MyPlugProf::~MyPlugProf", 0 );
}

///
/**
**/
void MyPlugProf::elslot()
{
    _depura ( "MyPlugProf::elslot", 0 );
    
    if ( g_alumnosList ) {
        g_alumnosList->hide();
        g_alumnosList->show();
    } // end if
    
    _depura ( "END MyPlugProf::elslot", 0 );
}

///
/**
**/
void MyPlugProf::elslot1()
{
    _depura ( "MyPlugProf::elslot1", 0 );
    
    AlumnoView * bud = new AlumnoView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany()->m_pWorkspace->addWindow ( bud );
    bud->show();
    
    _depura ( "END MyPlugProf::elslot1", 0 );
}

///
/**
**/
void MyPlugProf::elslot2()
{
    _depura ( "MyPlugProf::elslot2", 0 );
    
    ListCuotasPorAlumnoView * bud = new ListCuotasPorAlumnoView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany()->m_pWorkspace->addWindow ( bud );
    bud->show();
    
    _depura ( "END MyPlugProf::elslot2", 0 );
}

///
/**
**/
void MyPlugProf::elslot3()
{
    _depura ( "MyPlugProf::elslot3", 0 );
    ClaseView * bud = new ClaseView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    _depura ( "END MyPlugProf::elslot3", 0 );
}


///
/**
\param bges
**/
void MyPlugProf::inicializa ( BfBulmaFact *bges )
{
    _depura ( "MyPlugProf::inicializa", 0 );

    if ( bges->company()->hasTablePrivilege ( "alumno", "SELECT" ) ) {

        /// Miramos si existe un menu Docencia
        QMenu *pPluginMenu = bges->newMenu ( "&Docencia", "menuDocencia", "menuMaestro" );

        /// Agrego un separador
        pPluginMenu->addSeparator();

        m_bges = bges;
        setMainCompany ( bges->company() );
        
        QAction *alumnos = new QAction ( _ ( "&Alumnos" ), 0 );
        alumnos->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/alumno-list.png" ) ) );
        alumnos->setStatusTip ( _ ( "Alumnos" ) );
        alumnos->setWhatsThis ( _ ( "Alumnos" ) );
        pPluginMenu->addAction ( alumnos );
        bges->Listados->addAction ( alumnos );
        connect ( alumnos, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

        QAction *nalumno = new QAction ( _ ( "&Nuevo alumno" ), 0 );
        nalumno->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/alumno-new.png" ) ) );
        nalumno->setStatusTip ( _ ( "Nuevo alumno" ) );
        nalumno->setWhatsThis ( _ ( "Nuevo alumno" ) );
        pPluginMenu->addAction ( nalumno );
        bges->Fichas->addAction ( nalumno );
        connect ( nalumno, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );

        /// Miramos si existe un menu Docencia
        QMenu *pGesMenu = bges->newMenu ( "&Gestion", "menuGestion", "menuMaestro" );

        /// Agrego un separador
        pGesMenu->addSeparator();

        QAction *cuotaspalumno = new QAction ( _ ( "&Cuotas Socio" ), 0 );
        cuotaspalumno->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/mix_money_32.png" ) ) );
        cuotaspalumno->setStatusTip ( _ ( "Cuotas por alumno" ) );
        cuotaspalumno->setWhatsThis ( _ ( "Cuotas por alumno" ) );
        pGesMenu->addAction ( cuotaspalumno );
        bges->Fichas->addAction ( cuotaspalumno );
        connect ( cuotaspalumno, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );

        /// Agrego un separador
        pGesMenu->addSeparator();

        QAction *clases = new QAction ( _ ( "&Clases" ), 0 );
//        cuotaspalumno->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/mix_money_32.png" ) ) );
        clases->setStatusTip ( _ ( "Clases" ) );
        clases->setWhatsThis ( _ ( "Clases" ) );
        pGesMenu->addAction ( clases );
        bges->Fichas->addAction ( clases );
        connect ( clases, SIGNAL ( activated() ), this, SLOT ( elslot3() ) );

    } // end if
    
    _depura ( "END MyPlugProf::inicializa", 0 );
}

///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    _depura ( "Punto de entrada del plugin de alumnos\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_alumno", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    MyPlugProf *plug = new MyPlugProf();
    plug->inicializa ( bges );
    
    _depura ( "END Punto de entrada del plugin de alumnos\n", 0 );
    
    return 0;
}

int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    _depura ( "BfCompany_createMainWindows_Post", 0 );

    if ( comp->hasTablePrivilege ( "alumno", "SELECT" ) ) {
        g_alumnosList = new AlumnosList ( comp, NULL );
        comp->m_pWorkspace->addWindow ( g_alumnosList );
        g_alumnosList->hide();
    } // end if
    
    _depura ( "END BfCompany_createMainWindows_Post", 0 );

    return 0;
}

int BfSubForm_pressedAsterisk ( BfSubForm *sub )
{
    _depura ( "BfSubForm_pressedAsterisk" );

    if ( sub->m_campoactual->nomcampo() != "nombrealumno1" ) {
        _depura ( "END BfSubForm::pressedAsterisk", 0 );
        return 0;
    } // end if

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
    
    _depura ( "END BfSubForm_pressedAsterisk" );

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
MyPlugAl1::MyPlugAl1 ( BlSubForm *parent ) : QObject ( parent )
{
    _depura ( "MyPlugAl1::MyPlugAl1", 0 );
    _depura ( "END MyPlugAl1::MyPlugAl1", 0 );
}

///
/**
**/
MyPlugAl1::~MyPlugAl1()
{
    _depura ( "MyPlugAl1::~MyPlugAl1", 0 );
    _depura ( "END MyPlugAl1::~MyPlugAl1", 0 );
}


///
/**
\param menu
**/
void MyPlugAl1::s_pintaMenu ( QMenu *menu )
{
    _depura ( "MyPlugAl1::s_pintaMenu", 0 );
    BfSubForm *sub = ( BfSubForm * ) parent();
    BlSubFormHeader *header = sub->header ( "nombrealumno1" );
    if (!header) 
	header = sub->header ( "nombrealumno" );
    if ( header ) {
        menu->addSeparator();
        menu->addAction ( _ ( "Nuevo alumno" ) );
        QString idalumno = sub->dbValue ( "idalumno" );
        if ( idalumno != "" ) menu->addAction ( _ ( "Editar alumno" ) );
        if ( ! ( header->options() & BlSubFormHeader::DbNoWrite ) )  {
            menu->addAction ( _ ( "Seleccionar alumno" ) );
        } // end if
    } // end if
    _depura ( "END MyPlugAl1::s_pintaMenu", 0 );
}


///
/**
\param action
**/
void MyPlugAl1::s_trataMenu ( QAction *action )
{
    _depura ( "MyPlugAl1::s_trataMenu", 0 );
    BfSubForm *sub = ( BfSubForm * ) parent();
    if ( action->text() == _ ( "Editar alumno" ) ) {
        QString idalumno = sub->dbValue ( "idalumno" );
        if ( idalumno != "" )
            editarAlumno ( idalumno );
    } else if ( action->text() == _ ( "Seleccionar alumno" ) ) {
        seleccionarAlumno ( sub );
    } else if ( action->text() == _ ( "Nuevo alumno" )  ) {
        nuevoAlumno();
    } // end if

    _depura ( "END MyPlugAl1::s_trataMenu", 0 );
}


///
/**
**/
void MyPlugAl1::editarAlumno ( QString idalumno )
{
    _depura ( "MyPlugAl1::editarAlumno", 0 );
    BlSubForm * subf = ( BlSubForm * ) parent();
    AlumnoView * art = new AlumnoView ( ( BfCompany * ) subf->mainCompany(), 0 );
    subf->mainCompany() ->m_pWorkspace->addWindow ( art );
    /// Si la carga no va bien entonces terminamos.
    if ( art->cargar ( idalumno ) ) {
        delete art;
        _depura ( "END AlumnosList::editar", 0, "Carga Erronea" );
        return;
    } // end if
    art->hide();
    art->show();
    _depura ( "END MyPlugAl1::editarAlumno", 0 );
}



///
/**
**/
void MyPlugAl1::nuevoAlumno( )
{
    _depura ( "MyPlugAl1::editarAlumno", 0 );
    BlSubForm * subf = ( BlSubForm * ) parent();
    AlumnoView * art = new AlumnoView ( ( BfCompany * ) subf->mainCompany(), 0 );
    subf->mainCompany() ->m_pWorkspace->addWindow ( art );
    art->hide();
    art->show();
    _depura ( "END MyPlugAl1::editarAlumno", 0 );
}


///
/**
**/
void MyPlugAl1::seleccionarAlumno ( BfSubForm *sub )
{
    _depura ( "MyPlugAl1::editarAlumno", 0 );

    AlumnosList *artlist = new AlumnosList ( ( BfCompany * ) sub->mainCompany(), NULL, 0, BL_SELECT_MODE );
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    sub->setEnabled ( false );
    artlist->show();
    while ( !artlist->isHidden() )
        g_theApp->processEvents();
    sub->setEnabled ( true );
    QString idAlumno = artlist->idalumno();
    delete artlist;

    /// Si no tenemos un idalumno salimos ya que significa que no se ha seleccionado ninguno.
    if ( idAlumno == "" ) {
        _depura ( "END BfSubForm::pressedAsterisk", 0 );
        return;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM alumno WHERE idalumno = " + idAlumno );
    if ( !cur->eof() ) {
        sub->lineaact()->setDbValue ( "idalumno", idAlumno );
        sub->lineaact()->setDbValue ( "nombrealumno1", cur->valor ( "nombrealumno" ) );
    } // end if
    delete cur;

    _depura ( "END MyPlugAl1::editarAlumno", 0 );
}


///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    _depura ( "BlSubForm_BlSubForm_Post", 0 );
    MyPlugAl1 *subformods = new MyPlugAl1 ( sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    _depura ( "END BlSubForm_BlSubForm_Post", 0 );
    return 0;
}






