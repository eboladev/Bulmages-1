/***************************************************************************
 *   Copyright (C) 2008 by Ricardo Diaz de la Calle <richard@galdi.es>     *
 *   Copyright (C) 2008 by Adelaida Clavaguera Mora <laida@alaxarxa.net>   *
 *   Copyright (C) 2008 by Leopold Palomo Avellaneda <leo@alaxarxa.net>    *
 *   Copyright (C) 2006 by Fco. Javier M. C. >fcojavmc@todo-redes.com>     *
 *   Copyright (C) 2005 by Tomeu Borras Riera <tborras@conetxia.com>       *
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

#include <QMenu>
#include <QAction>
#include <QObject>
#include <QMessageBox>

#include <cstdio>

#include "pluginbc_caaslods.h"
#include "bccompany.h"
#include "blfunctions.h"
#include "datosview.h"
#include "blprogressbar.h"


BcCompany *g_comp = NULL;

///
/**
**/

pluginBC_caaslODS::pluginBC_caaslODS()
{
    blDebug ( "pluginBC_caaslODS::pluginBC_caaslODS", 0 );
    blDebug ( "END pluginBC_caaslODS::pluginBC_caaslODS", 0 );
}


///
/**
**/

pluginBC_caaslODS::~pluginBC_caaslODS()
{
    blDebug ( "pluginBC_caaslODS::~pluginBC_caaslODS", 0 );
    blDebug ( "END pluginBC_caaslODS::~pluginBC_caaslODS", 0 );
}





//Cuentas Anuales Abreviadas Asociacion Sin Lucro CAAASL
//Cuentas Anuales Plan General Contable hasta 2007 CAPGC07
//Cuentas Anuales PYMES 2008 CAPYMES08
//Cuentas Anuales Plan General Contable 2008 CAPGC08
//Cuentas Anuales Abreviadas Plan General Contable 2008 CAAPGC08

///
/**
\param bcont
**/


void pluginBC_caaslODS::inicializa ( BcBulmaCont *bcont )
{
    blDebug ( "pluginBC_caaslODS::inicializa", 0 );

/*
    /// Creamos el men&uacute;.
    QMenu *pPluginMenu = new QMenu ( _ ( "&Cuentas Anuales" ) );

    QMenu *pgc08 = new QMenu ( _ ( "&PGC 2008" ) );
    QMenu *pgc07 = new QMenu ( _ ( "PGC &Anterior 2008" ) );

    QAction *accion4 = new QAction ( _ ( "&Cuentas Anuales PGC" ), 0 );
    accion4->setStatusTip ( _ ( "Cuentas Anuales Plan General Contable 2008" ) );
    accion4->setWhatsThis ( _ ( "Cuentas Anuales Plan General Contable 2008" ) );
    connect ( accion4, SIGNAL ( activated() ), this, SLOT ( balsitCAPGC08() ) );

    QAction *accion5 = new QAction ( _ ( "Cuentas Anuales &Abreviadas PGC" ), 0 );
    accion4->setStatusTip ( _ ( "Cuentas Anuales Abreviadas Plan General Contable 2008" ) );
    accion4->setWhatsThis ( _ ( "Cuentas Anuales Abreviadas Plan General Contable 2008" ) );
    connect ( accion5, SIGNAL ( activated() ), this, SLOT ( balsitCAAPGC08() ) );

    QAction *accion3 = new QAction ( _ ( "Cuentas Anuales &PYMES" ), 0 );
    accion3->setStatusTip ( _ ( "Cuentas Anuales PYMES 2008" ) );
    accion3->setWhatsThis ( _ ( "Cuentas Anuales PYMES 2008" ) );
    connect ( accion3, SIGNAL ( activated() ), this, SLOT ( balsitCAPYMES08() ) );

    QAction *accion6 = new QAction ( _ ( "CA simplicat Fund. &Ass Cat" ), 0 );
    accion6->setStatusTip ( _ ( "Comptes anuals simplificat fundacions i associacions catalanes 2008" ) );
    accion6->setWhatsThis ( _ ( "Comptes anuals simplificat fundacions i associacions catalanes 2008") );
    connect ( accion6, SIGNAL ( activated() ), this, SLOT ( balsitCASFAC08() ) );



    QAction *accion2 = new QAction ( _ ( "&Cuentas Anuales PGC" ), 0 );
    accion2->setStatusTip ( _ ( "Cuentas Anuales Plan General Contable hasta 2007" ) );
    accion2->setWhatsThis ( _ ( "Cuentas Anuales Plan General Contable hasta 2007" ) );
    connect ( accion2, SIGNAL ( activated() ), this, SLOT ( balsitCAPGC07() ) );

    QAction *accion1 = new QAction ( _ ( "Cuentas A. Abreviadas &Sin lucro (Nuevo)" ), 0 );
    accion1->setStatusTip ( _ ( "Cuentas Anuales Abreviadas Asociacion Sin animo de lucro" ) );
    accion1->setWhatsThis ( _ ( "Cuentas Anuales Abreviadas Asociacion Sin animo de lucro" ) );
    connect ( accion1, SIGNAL ( activated() ), this, SLOT ( balsitCAAASL() ) );


    pPluginMenu->addMenu ( pgc08 );
    pPluginMenu->addSeparator();
    pPluginMenu->addMenu ( pgc07 );

    pgc08->addAction ( accion4 ); //PGC
    pgc08->addAction ( accion5 );
    pgc08->addAction ( accion3 );
    pgc08->addAction ( accion6 );
    pgc07->addAction ( accion2 );
    pgc07->addAction ( accion1 );


    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bcont->menuBar() ->addMenu ( pPluginMenu );

    empresaact = bcont->empresaactual();
    dbConnection = empresaact->bdempresa();
*/
    blDebug ( "END pluginBC_caaslODS::inicializa", 0 );
}


void pluginBC_caaslODS::formDatosBalance ( )
{
      blDebug ( "pluginBC_caaslODS::formDatosBalance", 0 );

      QDialog *diag = new QDialog ( 0 );
      diag->setModal ( true );
      diag->setGeometry ( QRect ( 0, 0, 450, 350 ) );
      centrarEnPantalla ( diag );

      DatosView *pidefechas = new DatosView ( g_comp, diag, 0, BL_SELECT_MODE );

      pidefechas->m_informe = sender()->objectName();

      /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
      /// para que sea redimensionable y aparezca el titulo de la ventana.
      QHBoxLayout *layout = new QHBoxLayout;
      layout->addWidget ( pidefechas );
      layout->setMargin ( 0 );
      layout->setSpacing ( 0 );
      diag->setLayout ( layout );
      diag->setWindowTitle ( pidefechas->windowTitle() );
      
      pidefechas->connect ( pidefechas->mui_aceptar, SIGNAL ( clicked () ), diag, SLOT ( accept() ) );
      pidefechas->connect ( pidefechas->mui_cancelar, SIGNAL ( clicked () ), diag, SLOT ( accept() ) );
      
      while(!pidefechas->resultado)
      {
         diag->exec();
      }
      
      delete diag;

      blDebug ( "END pluginBC_caaslODS::formDatosBalance", 0 );
}



///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    blDebug ( "Estoy dentro del plugin\n", 0 );

    g_comp = bcont->empresaactual();

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbc_caaslods", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    pluginBC_caaslODS *plug = new pluginBC_caaslODS();
    plug->inicializa ( bcont );

    /// S&Oacute;LO A MODO DE EJEMPLO: se modifica el t&iacute;tulo de la ventana principal
    /// del programa para indicar que el plugin se ha cargado.

// =========================================================


    QMenu *pPluginMenu = NULL;
    QMenu *pPluginVer = NULL;
    if ( !pPluginMenu ) {
        pPluginVer = bcont->menuBar()->findChild<QMenu *> ( "menuVentana" );
    } // end if

    /// Buscamos ficheros que tengan el nombre de la tabla
    QDir dir ( g_confpr->valor ( CONF_DIR_OPENREPORTS ) );
    dir.setFilter ( QDir::Files | QDir::NoSymLinks );
    dir.setSorting ( QDir::Size | QDir::Reversed );
    /// Hacemos un filtrado de busqueda
    QStringList filters;
    filters << "inf_ca*.pys";
    dir.setNameFilters ( filters );

    QFileInfoList list = dir.entryInfoList();

    for ( int i = 0; i < list.size(); ++i ) {
        QFileInfo fileInfo = list.at ( i );

        QFile file;
        file.setFileName ( g_confpr->valor ( CONF_DIR_OPENREPORTS ) + fileInfo.fileName() );
        file.open ( QIODevice::ReadOnly );
        QTextStream stream ( &file );
        QString buff = stream.readAll();
        file.close();

        /// Buscamos el titulo
        QString titulo = fileInfo.fileName();
        QRegExp rx3 ( "title\\s*=\\s*\"(.*)\"" );
        rx3.setMinimal ( TRUE );
        if ( rx3.indexIn ( buff, 0 )  != -1 ) {
            titulo = rx3.cap ( 1 );
        } // end while

        QString pathtitulo = fileInfo.fileName();
        QRegExp rx1 ( "pathtitle\\s*=\\s*\"(.*)\"" );
        rx1.setMinimal ( TRUE );
        if ( rx1.indexIn ( buff, 0 )  != -1 ) {
            pathtitulo = rx1.cap ( 1 );
        } // end while


	QMenuBar *menubar =bcont->menuBar();
	QMenu *menu = NULL;
	QStringList path = pathtitulo.split("\\");


	if (path.size() > 1) {
		    // QMenu *pPluginVer = bcont->menuBar()->findChild<QMenu *> ( "menuVentana" );
		    QList<QMenu *> allPButtons = menubar->findChildren<QMenu *>();
		    bool encontrado = FALSE;
		    for (int j = 0; j < allPButtons.size(); ++j) {
			if (allPButtons.at(j)->title() == path[0]) {
			    encontrado = TRUE;
			    menu = allPButtons.at(j);
			} // end if
		    } // end for

		    if (!encontrado) {
			QMenu *pPluginMenu1 = new QMenu (  path[0] , menubar );
	//		pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuCuentasAnualesODS" ) );
			menubar->insertMenu ( pPluginVer->menuAction(), pPluginMenu1 );
			menu = pPluginMenu1;
		    } // end if
	} else {

		    if (!pPluginMenu) {
			    pPluginMenu = new QMenu ( _ ( "Cuentas Anuales &ODS" ), bcont->menuBar() );
			    pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuCuentasAnualesODS" ) );
			    bcont->menuBar()->insertMenu ( pPluginVer->menuAction(), pPluginMenu );
		    } // end if
		    menu = pPluginMenu;
	} // end if
	


	for (int i = 1; i < path.size()-1; ++i) {
	    QList<QMenu *> allPButtons = menu->findChildren<QMenu *>();
	    bool encontrado = FALSE;
	    for (int j = 0; j < allPButtons.size(); ++j) {
		if (allPButtons.at(j)->title() == path[i]) {
		    encontrado = TRUE;
		    menu = allPButtons.at(j);
		} // end if
	    } // end for

	    if (!encontrado) {
		QMenu *pPluginMenu1 = new QMenu ( path[i] , menu );
		menu->addMenu (  pPluginMenu1 );
		menu = pPluginMenu1;
	    } // end if

	} // end for

        /// Creamos el men&uacute;.
        QAction *accion = new QAction ( path[path.size()-1], 0 );
        accion->setObjectName ( fileInfo.fileName() );
        accion->setStatusTip ( titulo);
        accion->setWhatsThis ( titulo );
        bcont->connect ( accion, SIGNAL ( activated() ), plug, SLOT ( formDatosBalance() ) );
        menu->addAction ( accion );
    } // end for







// ==========================================================

    return 0;
}

