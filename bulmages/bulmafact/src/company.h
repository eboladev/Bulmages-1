/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borrás Riera                              *
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
#ifndef COMPANY_H
#define COMPANY_H

#include <qworkspace.h>
#include "postgresiface2.h"
#include <qobject.h>
/**
Company class gives the application something like a standard interface to access each company in the same way.
@author Tomeu Borrás
*/

class company : public postgresiface2 {
private:
   QWidget *m_pWorkspace;
public:
   company();
   ~company();
   void listproviders();
   void listclients();
   void listarticles();
   void setWorkspace(QWidget *qw) {m_pWorkspace=qw;}
   void provideraction();
};

#endif
