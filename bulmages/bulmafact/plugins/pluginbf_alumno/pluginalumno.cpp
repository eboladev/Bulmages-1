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

#include "pluginalumno.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "alumnoview.h"
#include "alumnoslist.h"
#include "listcuotasporalumnoview.h"


AlumnosList *g_alumnosList=NULL;

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
    if (g_alumnosList) {
	g_alumnosList->hide();
	g_alumnosList->show();
    }// end if
    _depura ( "END MyPlugProf::elslot", 0 );
}

///
/**
**/
void MyPlugProf::elslot1()
{
    _depura ( "MyPlugProf::elslot1", 0 );
        AlumnoView * bud = new AlumnoView((BfCompany *)mainCompany(), NULL);
        mainCompany() ->m_pWorkspace->addWindow ( bud );
        bud->show();
    _depura ( "END MyPlugProf::elslot1", 0 );
}


///
/**
**/
void MyPlugProf::elslot2()
{
    _depura ( "MyPlugProf::elslot2", 0 );
        ListCuotasPorAlumnoView * bud = new ListCuotasPorAlumnoView((BfCompany *)mainCompany(), NULL);
        mainCompany() ->m_pWorkspace->addWindow ( bud );
        bud->show();
    _depura ( "END MyPlugProf::elslot2", 0 );
}


///
/**
\param bges
**/
void MyPlugProf::inicializa ( Bulmafact *bges )
{
    _depura ( "MyPlugProf::inicializa", 0 );

    if ( bges->getcompany()->hasTablePrivilege ( "alumno", "SELECT" ) ) {

    /// Miramos si existe un menu Ventas
	QMenu *pPluginMenu = bges->newMenu("&Docencia", "menuDocencia", "menuMaestro");

	/// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
	m_bges = bges;
	setMainCompany ( bges->getcompany() );
	QAction *planCuentas = new QAction ( _( "&Alumnos" ), 0 );
	planCuentas->setIcon(QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/alumno.svg" ) ));
	planCuentas->setStatusTip ( _( "Alumnos" ) );
	planCuentas->setWhatsThis ( _( "Alumnos" ) );
	pPluginMenu->addAction ( planCuentas );
	bges->Listados->addAction (planCuentas);
	connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

	QAction *npago = new QAction ( _( "&Nuevo alumno" ), 0 );
	npago->setIcon(QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/alumno.svg" ) ));
	npago->setStatusTip ( _( "Nuevo alumno" ) );
	npago->setWhatsThis ( _( "Nuevo alumno" ) );
	pPluginMenu->addAction ( npago );
	bges->Fichas->addAction (npago);
	connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );
   
   
   QAction *cuotaspalumno = new QAction ( _( "&Cuotas por Alumno" ), 0 );
   cuotaspalumno->setIcon(QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/alumno.svg" ) ));
   cuotaspalumno->setStatusTip ( _( "Cuotas por alumno" ) );
   cuotaspalumno->setWhatsThis ( _( "Cuotas por alumno" ) );
   pPluginMenu->addAction ( cuotaspalumno );
   bges->Fichas->addAction (cuotaspalumno);
   connect ( cuotaspalumno, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );
   
    }// end if
    _depura ( "END MyPlugProf::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Punto de entrada del plugin de alumnos\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginalumno", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    MyPlugProf *plug = new MyPlugProf();
    plug->inicializa ( bges );
    return 0;
}


int BfCompany_createMainWindows_Post(BfCompany *comp) {
    if ( comp->hasTablePrivilege ( "cobro", "SELECT" ) ) {
	g_alumnosList = new AlumnosList( comp, NULL );	
	comp->m_pWorkspace->addWindow ( g_alumnosList );
	g_alumnosList->hide();
    }// end if
    return 0;
}
