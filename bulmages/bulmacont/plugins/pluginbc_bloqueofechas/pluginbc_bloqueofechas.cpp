/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>

#include "pluginbc_bloqueofechas.h"
#include "bccompany.h"
#include "bcbloqueafechaview.h"



///
/**
**/
PluginBc_BloqueFechas::PluginBc_BloqueFechas()
{
    blDebug ( "PluginBc_BloqueFechas::PluginBc_BloqueFechas", 0 );
    blDebug ( "END PluginBc_BloqueFechas::PluginBc_BloqueFechas", 0 );
}


///
/**
**/
PluginBc_BloqueFechas::~PluginBc_BloqueFechas()
{
    blDebug ( "PluginBc_BloqueFechas::~PluginBc_BloqueFechas", 0 );
    blDebug ( "END PluginBc_BloqueFechas::~PluginBc_BloqueFechas", 0 );
}


///
/**
**/
void PluginBc_BloqueFechas::elslot()
{
    blDebug ( "PluginBc_BloqueFechas::elslot", 0 );
    BcBloqueaFechaView *tip = new BcBloqueaFechaView ( (BcCompany *)m_bulmacont->empresaactual(), 0 );
    m_bulmacont->workspace()->addSubWindow ( tip );
    tip->show();
    blDebug ( "END PluginBc_BloqueFechas::elslot", 0 );
}





///
/**
\param bges
**/
void PluginBc_BloqueFechas::inicializa ( BcBulmaCont *bges )
{
    /// Creamos el men&uacute;.
    m_bulmacont = bges;

    /// Miramos si existe un menu Herramientas
    QMenu *pPluginMenu = bges->newMenu(_("&Herramientas"), "menuHerramientas", "menuVentana");

    QAction *accion = new QAction ( _ ( "&Bloqueo de Fechas" ), 0 );
    accion->setStatusTip ( _ ( "Bloqueo de Fechas" ) );
    accion->setWhatsThis ( _ ( "Bloqueo de Fechas" ) );

    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( accion );

}


///
/**
\return
**/
int entryPoint ( BcBulmaCont *bcont )
{
    blDebug ( "Punto de entrada del plugin bloqueo de Fechas", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_bloqueofechas", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    PluginBc_BloqueFechas *my = new PluginBc_BloqueFechas();
    my->inicializa ( bcont );
    return 0;
}

