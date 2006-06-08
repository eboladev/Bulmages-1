/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#ifndef RUTACOMERCIAL_H
#define RUTACOMERCIAL_H



#include <QString>

#include "company.h"
#include "dbrecord.h"


class RutaComercial : public DBRecord {

protected:
    company *m_companyact;

public:
    RutaComercial(company *);
    company* companyact() {
        return m_companyact;
    };
    void setcompany(company *comp) {
	m_companyact = comp;
	DBRecord::setconexionbase(comp);
	};
    virtual ~RutaComercial();
    virtual void pintar();
//    virtual int guardar();
    virtual int cargar(QString id) {
        _depura("RutaComercial::cargar",0);
	QString query = "SELECT * FROM rutacomercial WHERE idrutacomercial=" + id;
	cursor2 * cur= m_companyact->cargacursor(query);
	if (!cur->eof())  {
		DBload(cur);
	}
	delete cur;
        _depura("END RutaComercial::cargar",0);
        return 0;
    };
};


#endif
