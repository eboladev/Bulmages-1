/***************************************************************************
 *   Copyright (C) 2004 by J. M. Torres Rigo                               *
 *   joanmi@bulma.net                                                      *
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
#ifndef PROVIDERSLIST_H
#define PROVIDERSLIST_H

#include "providerslistbase.h"


class company;


class providerslist : public providerslistbase {
 Q_OBJECT
public:
   company *companyact;
   
public:
    providerslist(company *, QWidget *parent = 0, const char *name = 0, int flag = 0);
    ~providerslist();
    void inicializa();
    
public slots:
    virtual void dobleclick(int, int, int, const QPoint &);
    virtual void contextMenu(int, int, const QPoint &);
    virtual void newprovider();
    
    /*
    virtual void boton_editar();
    
    virtual void boton_duplicar();
    virtual void boton_borrar();
    virtual void boton_imprimir();
    virtual void boton_filtrar();
*/
};

#endif
