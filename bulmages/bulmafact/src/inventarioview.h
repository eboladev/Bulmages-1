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

#ifndef INVENTARIOVIEW_H
#define INVENTARIOVIEW_H

#include <QLineEdit>
#include <Q3TextEdit>
#include <QLabel>
#include <QCheckBox>

#include "inventariobase.h"
#include "inventario.h"
#include "busquedafecha.h"
#include "postgresiface2.h"


class InventarioView : public InventarioBase, public Inventario
{
	Q_OBJECT

public:
	InventarioView(company *, QWidget *parent = 0, const char *name = 0);
	~InventarioView();
	void pintaidinventario(QString)
	{
	};
	void pintafechainventario(QString id)
	{
		m_fechainventario->setText(id);
	};
	void pintanominventario(QString id)
	{
		m_nominventario->setText(id);
	};

public slots:
	virtual void s_fechainventariovalueChanged(QString val)
	{
		setfechainventario(val);
	};
	virtual void s_nominventariotextChanged(const QString &val)
	{
		setnominventario(val);
	};
	virtual void s_save()
	{
		guardaInventario();
	};
	virtual void s_delete();
	virtual void s_pregenerar()
	{
		pregenerar();
	};
};

#endif
