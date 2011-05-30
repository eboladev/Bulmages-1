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

#include "pluginbc_ccaa2ods.h"
#include "bccompany.h"
#include "blfunctions.h"
#include "datosview.h"
#include "blprogressbar.h"


BcCompany *g_comp = NULL;

///
/**
**/

PluginBc_ccaa2ods::PluginBc_ccaa2ods()
{
    blDebug ( "PluginBc_ccaa2ods::PluginBc_ccaa2ods", 0 );
    blDebug ( "END PluginBc_ccaa2ods::PluginBc_ccaa2ods", 0 );
}


///
/**
**/

PluginBc_ccaa2ods::~PluginBc_ccaa2ods()
{
    blDebug ( "PluginBc_ccaa2ods::~PluginBc_ccaa2ods", 0 );
    blDebug ( "END PluginBc_ccaa2ods::~PluginBc_ccaa2ods", 0 );
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


void PluginBc_ccaa2ods::formDatosBalance ( )
{
      blDebug ( "PluginBc_ccaa2ods::formDatosBalance", 0 );

      QDialog *diag = new QDialog ( 0 );
      diag->setModal ( true );
      diag->setGeometry ( QRect ( 0, 0, 450, 350 ) );
      blCenterOnScreen ( diag );

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

      blDebug ( "END PluginBc_ccaa2ods::formDatosBalance", 0 );
}



///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    blDebug ( "Estoy dentro del plugin\n", 0 );

    g_comp = bcont->company();

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_ccaa2ods", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    PluginBc_ccaa2ods *plug = new PluginBc_ccaa2ods();

    /// S&Oacute;LO A MODO DE EJEMPLO: se modifica el t&iacute;tulo de la ventana principal
    /// del programa para indicar que el plugin se ha cargado.

// =========================================================


    QMenu *pPluginMenu = NULL;
    QMenu *pPluginVer = NULL;
    if ( !pPluginMenu ) {
        pPluginVer = bcont->menuBar()->findChild<QMenu *> ( "menuVentana" );
    } // end if

    /// Buscamos ficheros que tengan el nombre de la tabla
    QDir dir ( g_confpr->value( CONF_DIR_OPENREPORTS ) );
    dir.setFilter ( QDir::Files | QDir::NoSymLinks );
    dir.setSorting ( QDir::Size | QDir::Reversed );
    /// Hacemos un filtrado de busqueda
    QStringList filters;
    filters << "inf_ccaa*.pys";
    dir.setNameFilters ( filters );

    QFileInfoList list = dir.entryInfoList();

    for ( int i = 0; i < list.size(); ++i ) {
        QFileInfo fileInfo = list.at ( i );

        QFile file;
        file.setFileName ( g_confpr->value( CONF_DIR_OPENREPORTS ) + fileInfo.fileName() );
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
		menu->addMenu ( pPluginMenu1 );
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

