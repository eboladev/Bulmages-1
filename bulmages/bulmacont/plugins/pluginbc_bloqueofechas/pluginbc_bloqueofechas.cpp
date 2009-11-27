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
myBloqFechas::myBloqFechas()
{
    _depura ( "myBloqFechas::myBloqFechas", 0 );
    _depura ( "END myBloqFechas::myBloqFechas", 0 );
}


///
/**
**/
myBloqFechas::~myBloqFechas()
{
    _depura ( "myBloqFechas::~myBloqFechas", 0 );
    _depura ( "END myBloqFechas::~myBloqFechas", 0 );
}


///
/**
**/
void myBloqFechas::elslot()
{
    _depura ( "myBloqFechas::elslot", 0 );
    BcBloqueaFechaView *tip = new BcBloqueaFechaView ( (BcCompany *)m_bulmacont->empresaactual(), 0 );
    m_bulmacont->workspace()->addWindow ( tip );
    tip->show();
    _depura ( "END myBloqFechas::elslot", 0 );
}





///
/**
\param bges
**/
void myBloqFechas::inicializa ( BcBulmaCont *bges )
{
    /// Creamos el men&uacute;.
    m_bulmacont = bges;
    QAction *accion = new QAction ( _ ( "&Bloqueo de Fechas" ), 0 );
    accion->setStatusTip ( _ ( "Bloqueo de Fechas" ) );
    accion->setWhatsThis ( _ ( "Bloqueo de Fechas" ) );

    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuListados->insertAction ( bges->actionCuentas_Anuales, accion );

    bges->menuListados->insertSeparator ( bges->actionCuentas_Anuales );
}


///
/**
\return
**/
int entryPoint ( BcBulmaCont *bcont )
{
    _depura ( "Punto de entrada del plugin bloqueo de Fechas", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbc_bloqueofechas", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    myBloqFechas *my = new myBloqFechas();
    my->inicializa ( bcont );
    return 0;
}

