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
#ifndef ARTICLESLIST_H
#define ARTICLESLIST_H

#include "articleslistbase.h"


class company;


class articleslist : public articleslistbase
{
 Q_OBJECT
public:
   company *companyact;
	int m_modo;	// == 0 es modo edición
					// ==1 es modo selector.
	QString m_idArticle;
   
public:
    articleslist(company *, QWidget *parent = 0, const char *name = 0, int flag = 0);
	 void inicializa();
    void modoseleccion() {m_modo=1;};
    void modoedicion() {m_modo=0;};
    QString idArticle() {return m_idArticle;};
    ~articleslist();
    
public slots:
	virtual void articleSelected(int, int, int, const QPoint &);
};

#endif
