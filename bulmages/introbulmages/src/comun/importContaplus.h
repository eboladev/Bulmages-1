/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borrás                                    *
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
 ***************************************************************************/
#ifndef _IMPORTCONTAPLUS_H_
#define _IMPORTCONTAPLUS_H_
 
#include "importContaplusBase.h"
#include "postgresiface2.h" 
 
 
class importContaplus: public importContaplusBase {
Q_OBJECT
private:
	postgresiface2 *conexionbase;
public:
    importContaplus(postgresiface2 *, QWidget *, const char *, WFlags );
    ~importContaplus(){};
    void botonImportar();
    
public slots:
	void botonBuscarSubCta();
	void botonBuscarDiario();
};




#endif

