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

#ifndef BUSQUEDAFAMILIA_H
#define BUSQUEDAFAMILIA_H

#include <QLineEdit>
#include <QLabel>

#include <busquedafamiliabase.h>


class company;

class BusquedaFamilia : public BusquedaFamiliaBase
{
	Q_OBJECT

private:
	company *companyact;
	QString mdb_idfamilia;
	QString mdb_nombrefamilia;
	QString mdb_codigocompletofamilia;

public:
	BusquedaFamilia(QWidget *parent = 0, const char *name = 0);
	~BusquedaFamilia();
	void setcompany(company *comp)
	{
		companyact = comp;
	};
	virtual QString codigocompletofamilia()
	{
		return m_codigocompletofamilia->text();
	};
	virtual QString idfamilia()
	{
		return mdb_idfamilia;
	};
	virtual QString nombrefamilia()
	{
		return nombrefamilia();
	};
	virtual void setidfamilia(QString val);
	virtual void setcodigocompletofamilia(QString val);

public slots:
	virtual void s_searchFamilia();
	virtual void s_codigofamiliatextChanged(const QString &);

signals:
	void valueChanged(QString);
};

#endif
