/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#ifndef BUSQUEDATARIFA_H
#define BUSQUEDATARIFA_H

#include <Q3ComboBox>

#include "company.h"
#include "postgresiface2.h"


class BusquedaTarifa : public Q3ComboBox
{
	Q_OBJECT

private:
	company *companyact;
	cursor2 *m_cursorcombo;

public:
	BusquedaTarifa(QWidget *parent = 0, const char *name = 0);
	~BusquedaTarifa();
	void setcompany(company *comp)
	{
		companyact = comp;
	};
	virtual void setidtarifa(QString idtarifa);

public slots:
	void m_activated(int index)
	{
		if (index > 0)
		{
			emit(valueChanged(m_cursorcombo->valor("idtarifa", index - 1)));
		} else {
			emit(valueChanged(""));
		}
	};

signals:
	void valueChanged(QString);
};

#endif
