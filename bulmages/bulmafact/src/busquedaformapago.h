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

#ifndef BUSQUEDAFORMA_PAGO_H
#define BUSQUEDAFORMA_PAGO_H

#include <Q3ComboBox>

#include "company.h"
#include "postgresiface2.h"
#include "funcaux.h"

class BusquedaFormaPago : public Q3ComboBox
{
	Q_OBJECT

private:
	company *companyact;
	cursor2 *m_cursorcombo;

public:
	BusquedaFormaPago(QWidget *parent = 0, const char *name = 0);
	~BusquedaFormaPago();
	void setcompany(company *comp)
	{
		companyact = comp;
	};
	virtual void setidforma_pago(QString idforma_pago);

public slots:
	void m_activated(int index)
	{
		_depura("Activado el combo box", 0);
		if (index > 0)
		{
			emit(valueChanged(m_cursorcombo->valor("idforma_pago", index - 1)));
		} else {
			emit(valueChanged(""));
		}
	};

signals:
	void valueChanged(QString);
};

#endif
