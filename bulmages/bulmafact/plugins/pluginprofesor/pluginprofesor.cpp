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

#include "pluginprofesor.h"
#include "company.h"
#include "blfunctions.h"
#include "profesorview.h"
#include "profesoreslist.h"



ProfesoresList *g_profesoresList=NULL;

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
    if (g_profesoresList) {
	g_profesoresList->hide();
	g_profesoresList->show();
    }// end if
    _depura ( "END MyPlugProf::elslot", 0 );
}

///
/**
**/
void MyPlugProf::elslot1()
{
    _depura ( "MyPlugProf::elslot1", 0 );
        ProfesorView * bud = new ProfesorView((Company *)empresaBase(), NULL);
        empresaBase() ->m_pWorkspace->addWindow ( bud );
        bud->show();
    _depura ( "END MyPlugProf::elslot1", 0 );
}



///
/**
\param bges
**/
void MyPlugProf::inicializa ( Bulmafact *bges )
{
    _depura ( "MyPlugProf::inicializa", 0 );

    if ( bges->getcompany()->has_table_privilege ( "cobro", "SELECT" ) ) {

    /// Miramos si existe un menu Ventas
	QMenu *pPluginMenu = bges->newMenu("&Docencia", "menuDocencia", "menuMaestro");

	/// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
	m_bges = bges;
	setEmpresaBase ( bges->getcompany() );
	QAction *planCuentas = new QAction ( _( "&Profesores" ), 0 );
	planCuentas->setIcon(QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/profesor.gif" ) ));
	planCuentas->setStatusTip ( _( "Profesores" ) );
	planCuentas->setWhatsThis ( _( "Profesores" ) );
	pPluginMenu->addAction ( planCuentas );
	bges->Listados->addAction (planCuentas);
	connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

	QAction *npago = new QAction ( _( "&Nuevo Profesor" ), 0 );
	npago->setIcon(QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/profesor.gif" ) ));
	npago->setStatusTip ( _( "Nuevo profesor" ) );
	npago->setWhatsThis ( _( "Nuevo profesor" ) );
	pPluginMenu->addAction ( npago );
	bges->Fichas->addAction (npago);
	connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );

	QAction *nfapac = new QAction ( _( "About FAPAC" ), 0 );
	bges->menuAcerca_de->addAction(nfapac);



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
    _depura ( "Punto de Entrada del plugin de Profesores\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginprofesor", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    MyPlugProf *plug = new MyPlugProf();
    plug->inicializa ( bges );
    return 0;
}


int Company_createMainWindows_Post(Company *comp) {
    if ( comp->has_table_privilege ( "profesor", "SELECT" ) ) {
	g_profesoresList = new ProfesoresList( comp, NULL );	
	comp->m_pWorkspace->addWindow ( g_profesoresList );
	g_profesoresList->hide();
    }// end if
    return 0;
}


int Busqueda_on_mui_buscar_clicked(Busqueda *busq) {
	if (busq->tableName() == "profesor") {


    QDialog *diag = new QDialog ( 0 );
    diag->setModal ( true );
    diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
    centrarEnPantalla ( diag );

    ProfesoresList *clients = new ProfesoresList ( ( Company * ) busq->empresaBase(), diag, 0, ProfesoresList::SelectMode );
    busq->connect ( clients, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

    /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
    /// para que sea redimensionable y aparezca el titulo de la ventana.
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget ( clients );
    layout->setMargin ( 0 );
    layout->setSpacing ( 0 );
    diag->setLayout ( layout );
    diag->setWindowTitle ( clients->windowTitle() );

    diag->exec();
    if ( clients->idprofesor() != "" ) {
        busq->setId ( clients->idprofesor() );
    } // end if
    delete diag;


		return 1;
	} // end if
	return 0;

}

