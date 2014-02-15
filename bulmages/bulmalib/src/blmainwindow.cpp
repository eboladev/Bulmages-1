/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#include <QtCore/QString>
#include <QtWidgets/QMenuBar>

#include "blmainwindow.h"
#include "blfunctions.h"

/// Definimos aqui la variable global g_main para que sea accesible desde esta libreria.
QMainWindow *g_main = NULL;



BlMainWindow::BlMainWindow ( QWidget * parent, Qt::WindowFlags flags )
{
   BL_FUNC_DEBUG
}


BlMainWindow::~BlMainWindow()
{
   BL_FUNC_DEBUG
}


QMenu *BlMainWindow::newMenu ( const QString &name, const QString &objname, const QString &before )
{
    BL_FUNC_DEBUG
    /// Miramos si existe un menu Ventas
    QMenu *pPluginMenu = menuBar() ->findChild<QMenu *> ( objname );
    /// Creamos el men&uacute;.
    if ( !pPluginMenu || objname == "" ) {
        QMenu *pPluginMaestro = menuBar() ->findChild<QMenu *> ( before );
        pPluginMenu = new QMenu ( name, menuBar() );
        pPluginMenu->setObjectName (  objname );
        menuBar()->insertMenu ( pPluginMaestro->menuAction(), pPluginMenu );
    } // end if
    return pPluginMenu;
}


///
/**
\return
**/
BlMainCompany * BlMainWindow::company()
{
    BL_FUNC_DEBUG
    
    return m_pcompany;
}