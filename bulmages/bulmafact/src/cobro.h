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

#ifndef COBRO_H
#define COBRO_H

#include <QString>
#include <QObject>

#include "company.h"
#include "dbrecord.h"


class Cobro : public DBRecord {

protected:
	company *companyact;

public:
	Cobro(company *);
	virtual ~Cobro();
	virtual void pintar();
	void setidcobro(QString val)
	{
		setDBvalue("idcobro",val);
	};
	void setidcliente(QString val)
	{
		setDBvalue("idcliente",val);
	};
	void setfechacobro(QString val)
	{
		setDBvalue("fechacobro",val);
	};
	void setcantcobro(QString val)
	{
		setDBvalue("cantcobro",val);
	};
	void setrefcobro(QString val)
	{
		setDBvalue("refcobro",val);
	};
	void setprevisioncobro(QString val)
	{
		setDBvalue("previsioncobro",val);
	};
	void setcomentcobro(QString val)
	{
		setDBvalue("comentcobro",val);
	};

	virtual void pintaidcobro(QString)
	{
	};
	virtual void pintaidcliente(QString)
	{
	};
	virtual void pintafechacobro(QString)
	{
	};
	virtual void pintacantcobro(QString)
	{
	};
	virtual void pintarefcobro(QString)
	{
	};
	virtual void pintaprevisioncobro(QString)
	{
	};
	virtual void pintacomentcobro(QString)
	{
	};
};

#endif
