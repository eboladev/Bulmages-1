/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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


#include "pluginabrevs.h"
#include "funcaux.h"
#include "empresatpv.h"
#include "abrevs.h"
#include "plugins.h"
#include "qapplication2.h"
#include "trabajadores.h"
#include "bdockwidget.h"
#include <QX11EmbedContainer>
#include <QProcess>

Abrevs *g_tot;
BDockWidget *g_doc1;

///
/**
\return
**/
int entryPoint ( BulmaTPV *tpv )
{
    _depura ( "entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginabrevs", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    /// Vamos a probar con un docwindow.
    g_doc1 = new BDockWidget ( "Abreviaturas", tpv, "abrevsdock" );
    g_doc1->setObjectName ( "abrevsdock" );
    g_doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );
    tpv->addDockWidget ( Qt::BottomDockWidgetArea, g_doc1 );
    g_doc1->show();
    g_doc1->cargaconf();
    _depura ( "END entryPoint", 0 );
    return 0;
}

///
/**
\return
**/
int exitPoint ( BulmaTPV *tpv )
{
    _depura ( "pluginabrevs::exitPoint", 0 );

    /// Vamos a probar con un docwindow.
    delete g_doc1;
    _depura ( "END pluginabrevs::exitPoint", 0 );
    return 0;
}

int EmpresaTPV_createMainWindows_Post ( EmpresaTPV *etpv )
{
    g_tot = new Abrevs ( etpv, g_doc1 );
    g_tot->setObjectName ( "abrevs" );
    g_doc1->setWidget ( g_tot );

    Trabajadores *trab = new Trabajadores ( etpv, 0 );
    trab->exec();
    return 0;
}



