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

#ifndef CLIENTE_H
#define CLIENTE_H

#include <QString>

#include "company.h"
#include "dbrecord.h"


class Cliente : public DBRecord
{

protected:
	company *m_companyact;

public:
	Cliente(company *);
	virtual ~Cliente();
	virtual void pintaCliente();
	virtual void guardaCliente();
	virtual void borraCliente();
	virtual void vaciaCliente();
	virtual void cargaCliente(QString);


	void setidcliente(QString val) {setDBvalue("idcliente",val);};
	void setnomcliente(QString val) {setDBvalue("nomcliente",val);};
	void setnomaltcliente(QString val) {setDBvalue("nomaltcliente",val);};
	void setcifcliente(QString val) {setDBvalue("cifcliente",val);};
	void setbancocliente(QString val) {setDBvalue("bancocliente",val);};
	void setdircliente(QString val) {setDBvalue("dircliente",val);};
	void setpoblcliente(QString val) {setDBvalue("poblcliente",val);};
	void setcpcliente(QString val) {setDBvalue("cpcliente",val);};
	void settelcliente(QString val) {setDBvalue("telcliente",val);};
	void setfaxcliente(QString val) {setDBvalue("faxcliente",val);};
	void setmailcliente(QString val) {setDBvalue("mailcliente",val);};
	void seturlcliente(QString val) {setDBvalue("urlcliente",val);};
	void setfaltacliente(QString val) {setDBvalue("faltacliente",val);};
	void setfbajacliente(QString val) {setDBvalue("fbajacliente",val);};
	void setcomentcliente(QString val) {setDBvalue("comentcliente",val);};
	void setinactivocliente(QString val) {setDBvalue("inactivocliente",val);};
	void setprovcliente(QString val) {setDBvalue("provcliente",val);};

	virtual void pintaidcliente (QString){};
	virtual void pintanomcliente (QString){};
	virtual void pintanomaltcliente (QString){};
	virtual void pintacifcliente (QString){};
	virtual void pintabancocliente (QString){};
	virtual void pintadircliente (QString){};
	virtual void pintapoblcliente (QString){};
	virtual void pintacpcliente (QString){};
	virtual void pintatelcliente (QString){};
	virtual void pintafaxcliente (QString){};
	virtual void pintamailcliente (QString){};
	virtual void pintaurlcliente (QString){};
	virtual void pintafaltacliente (QString){};
	virtual void pintafbajacliente (QString){};
	virtual void pintacomentcliente (QString){};
	virtual void pintainactivocliente (QString){};
	virtual void pintaprovcliente (QString){};
};

#endif
