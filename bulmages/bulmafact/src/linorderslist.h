/***************************************************************************
 *   Copyright (C) 2004 by J. M. Estopa Rey                                *
 *   pepma@telefonica.net                                                  *
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
#ifndef LINORDERSLIST_H
#define LINORDERSLIST_H

#include "linorderslistbase.h"
#include "postgresiface2.h"


class company;


class linorderslist : public linorderslistbase
{
 Q_OBJECT
public:
   company *companyact;
   QString idpedido;
   cursor2 *m_cursorcombo;
   
public:
    linorderslist(company *, QWidget *parent = 0, const char *name = 0, int flag = 0);
    ~linorderslist();
    

public:
    void chargelinorders(QString);
    void chargeorder(QString);
    
public slots:
    virtual void activated(int);
};

#endif
