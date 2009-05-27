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
\param bges
**/
void MyPlugProf::inicializa ( BfBulmaFact *bges )
{
    _depura ( "MyPlugProf::inicializa", 0 );

    if ( bges->getcompany()->hasTablePrivilege ( "alumno", "SELECT" ) ) {

        /// Miramos si existe un menu Docencia
        QMenu *pPluginMenu = bges->newMenu ( "&Docencia", "menuDocencia", "menuMaestro" );

        /// Agrego un separador
        pPluginMenu->addSeparator();

        m_bges = bges;
        setMainCompany ( bges->getcompany() );
        
        QAction *alumnos = new QAction ( _ ( "&Alumnos" ), 0 );
        alumnos->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/alumno.svg" ) ) );
        alumnos->setStatusTip ( _ ( "Alumnos" ) );
        alumnos->setWhatsThis ( _ ( "Alumnos" ) );
        pPluginMenu->addAction ( alumnos );
        bges->Listados->addAction ( alumnos );
        connect ( alumnos, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

        QAction *nalumno = new QAction ( _ ( "&Nuevo alumno" ), 0 );
        nalumno->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/alumno.svg" ) ) );
        nalumno->setStatusTip ( _ ( "Nuevo alumno" ) );
        nalumno->setWhatsThis ( _ ( "Nuevo alumno" ) );
        pPluginMenu->addAction ( nalumno );
        bges->Fichas->addAction ( nalumno );
        connect ( nalumno, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );

        QAction *cuotaspalumno = new QAction ( _ ( "&Cuotas por Alumno" ), 0 );
        cuotaspalumno->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/mix_money_32.png" ) ) );
        cuotaspalumno->setStatusTip ( _ ( "Cuotas por alumno" ) );
        cuotaspalumno->setWhatsThis ( _ ( "Cuotas por alumno" ) );
        pPluginMenu->addAction ( cuotaspalumno );
        bges->Fichas->addAction ( cuotaspalumno );
        connect ( cuotaspalumno, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );

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
    bindtextdomain ( "pluginalumno", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

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

    if ( sub->m_campoactual->nomcampo() != "nombrealumno" ) {
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
        sub->m_registrolinea->setDbValue ( "nombrealumno", cur->valor ( "nombrealumno" ) );
    } // end if
    
    delete cur;
    
    _depura ( "END BfSubForm_pressedAsterisk" );

    return 0;
}
