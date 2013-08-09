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

#ifndef BLMAINWINDOW_H
#define BLMAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "bldefs.h"
#include "blworkspace.h"
#include "blmaincompany.h"

class BL_EXPORT BlMainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    /// El workSpace que se va a usar con la aplicacion.
    BlWorkspace *pWorkspace;
    /// El puntero el BlMainCompany para que se pueda usar esta superclase 
    BlMainCompany *m_pcompany;
    
public:
    BlMainWindow ( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
    ~BlMainWindow();
    QMenu *newMenu(const QString &name,const QString &objname, const QString &before="");
    BlMainCompany *company();
};


extern BL_EXPORT QMainWindow *g_main;

#endif

