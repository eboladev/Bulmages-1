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

#include <cstdio>

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>
#include <QTextCodec>
#include <QLocale>

#include "pluginbc_amortizacion.h"
#include "bcamortizacionlistview.h"

BcBulmaCont *g_pluginbc_amortizacion = NULL;

///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_amortizacion", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_pluginbc_amortizacion = bcont;

    /// Creamos el men&uacute;.
    QMenu *pPluginMenu = bcont->newMenu(_("&Ver"), "menuVer", "menuMaestro");

    BlAction *accionA = new BlAction ( _ ( "&Amortizaciones" ), 0 );
    accionA->setStatusTip ( _ ( "Permite ver las amortizaciones" ) );
    accionA->setWhatsThis ( _ ( "Podra disponer de la informacion de las amortizaciones" ) );
    accionA->setIcon(QIcon(QString::fromUtf8(":/Genericos32x32/images/png/i_bulmacont_amortizaciones.png")));
    accionA->setObjectName("mui_actionAmortizaciones");
    pPluginMenu->addAction ( accionA );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bcont->toolBar->addAction ( accionA );
    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    if (accion->objectName() == "mui_actionAmortizaciones") {
	BcAmortizacionListView *amortizacionList = new BcAmortizacionListView ( g_pluginbc_amortizacion->company(), 0 );
	g_pluginbc_amortizacion->company()->pWorkspace()->addSubWindow ( amortizacionList );
	amortizacionList->show();
    } // end if

    return 0;
}

