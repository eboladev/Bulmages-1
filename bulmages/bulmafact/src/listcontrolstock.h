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

#ifndef LISTCONTROLSTOCK_H
#define LISTCONTROLSTOCK_H

#include <QObject>
#include <Q3PtrList>

#include "funcaux.h"
#include "company.h"
#include "controlstock.h"


class ListControlStock
{

public:
	company *companyact;
	QString mdb_idinventario;
	Q3PtrList<ControlStock> m_lista;

public:
	ListControlStock(company *comp);
	ListControlStock();
	virtual ~ListControlStock();
	ControlStock *linpos(int);
	void cargaListControlStock(QString);
	void guardaListControlStock();
	void vaciar();
	void setidinventario(QString id)
	{
		mdb_idinventario = id;
		ControlStock *linea;
		uint i = 0;
		for (linea = m_lista.first(); linea; linea = m_lista.next())
		{
			linea->setidinventario(mdb_idinventario);
			i++;
		}
	};
	void setcompany(company *c)
	{
		_depura("ListControlStock setCompany." 0);
		companyact = c;
		_depura("ListControlStock fin de setCompany.", 0);
	};
	virtual void pintaListControlStock()
	{
		_depura("La funcion pintaListControlStock aun no ha sido implementada.", 0);
	};
	void nuevalinea(QString idalmacen, QString idarticulo, QString stockantcontrolstock,
				QString stocknewcontrolstock, QString punteocontrolstock,
				QString codigocompletoarticulo, QString nomarticulo,
				QString nomalmacen, QString codigoalmacen);
	virtual void borrarListControlStock();
	virtual void borrarControlStock(int);
	virtual void pregenerar();
};

#endif
