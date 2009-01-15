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

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>

#include "pluginz2z.h"
#include "configuracion.h"
#include "bulmafact.h"
#include "company.h"
#include "z2zview.h"

Company *g_bges;

///
/**
**/
mypluginzz::mypluginzz()
{
    _depura ( "mypluginzz::mypluginzz", 0 );
    _depura ( "END mypluginzz::mypluginzz", 0 );
}


///
/**
**/
mypluginzz::~mypluginzz()
{
    _depura ( "mypluginzz::~mypluginzz", 0 );
    _depura ( "END mypluginzz::~mypluginzz", 0 );
}


///
/**
\return
**/
void mypluginzz::elslot (  )
{
    _depura ( "mypluginzz::cambia", 0 );
        Z2ZView * bud = new Z2ZView(g_bges, NULL);
        g_bges ->m_pWorkspace->addWindow ( bud );
        bud->show();
    _depura ( "END mypluginzz::cambia", 0 );
}



///
/**
\param bcont
**/
void entryPoint ( Bulmafact *bges )
{
    _depura ( "Entrada del plugin Corrector", 10 );
    g_bges = bges->getcompany();

    mypluginzz *corr = new mypluginzz();

    /// A&ntilde;ade en el men&uacute; del programa la opci&oacuteMn para
    /// acceder al corrector.
    QAction *viewCorrector = new QAction ( "&Traspaso de Z", 0 );

    /// Miramos si existe un menu Herramientas
    QMenu *pPluginMenu = bges->newMenu("&Herramientas", "menuHerramientas", "menuAcerca_de");


    viewCorrector->setStatusTip ( "Traspaso de Z" );
    viewCorrector->setWhatsThis ( "Traspaso de Z" );
    QObject::connect ( viewCorrector, SIGNAL ( activated (  ) ), corr, SLOT ( elslot ( ) ) );
    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( viewCorrector );
    _depura ( "Iniciado correctamente el plugin de Traspaso de Z", 10 );
}

