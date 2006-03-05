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

#ifndef BUSQUEDATIPOARTICULO_H
#define BUSQUEDATIPOARTICULO_H

#include <QLineEdit>
#include <QLabel>

#include <busquedatipoarticulobase.h>

class company;

class BusquedaTipoArticulo : public BusquedaTipoArticuloBase
{
	Q_OBJECT

private:
	company *companyact;
	QString mdb_idtipo_articulo;
	QString mdb_desctipo_articulo;
	QString mdb_codtipo_articulo;

public:
	BusquedaTipoArticulo(QWidget *parent = 0, const char *name = 0);
	~BusquedaTipoArticulo();
	void setcompany(company *comp)
	{
		companyact = comp;
	};
	virtual QString codtipo_articulo()
	{
		return m_codtipo_articulo->text();
	};
	virtual QString idtipo_articulo()
	{
		return mdb_idtipo_articulo;
	};
	virtual QString desctipo_articulo()
	{
		return desctipo_articulo();
	};
	virtual void setidtipo_articulo(QString val);
	virtual void setcodtipo_articulo(QString val);

public slots:
	virtual void s_searchTipoArticulo();
	virtual void s_codtipo_articulotextChanged(const QString &);

signals:
	void valueChanged(QString);
};

#endif
