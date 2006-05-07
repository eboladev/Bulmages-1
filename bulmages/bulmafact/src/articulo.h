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

#ifndef ARTICULO_H
#define ARTICULO_H

#include <QString>

#include "company.h"
#include "dbrecord.h"


class Articulo : public DBRecord {

protected:
	company *m_companyact;

public:
	Articulo(company *);
	company* companyact() {return m_companyact;};
	virtual ~Articulo();
	virtual void pintar();

	void setidarticulo(QString val) 		{setDBvalue("idarticulo", val);};
	void setcodarticulo(QString val) 		{setDBvalue("codarticulo", val);};
	void setnomarticulo(QString val) 		{setDBvalue("nomarticulo", val);};
	void setabrevarticulo(QString val) 		{setDBvalue("abrevarticulo", val);};
	void setobservarticulo(QString val) 		{setDBvalue("observarticulo", val);};
	void setpresentablearticulo(QString val) 	{setDBvalue("presentablearticulo", val);};
	void setcontrolstockarticulo(QString val) 	{setDBvalue("controlstockarticulo", val);};
	void setidtipo_articulo(QString val) 		{setDBvalue("idtipo_articulo", val);};
	void setidtipo_iva(QString val) 		{setDBvalue("idtipo_iva", val);};
	void setcodigocompletoarticulo(QString val) 	{setDBvalue("codigocompletoarticulo", val);};
	void setidfamilia(QString val) 			{setDBvalue("idfamilia", val);};
	void setstockarticulo(QString val) 		{setDBvalue("stockarticulo", val);};
	void setinactivoarticulo(QString val) 		{setDBvalue("inactivoarticulo", val);};
	void setpvparticulo(QString val) 		{setDBvalue("pvparticulo", val);};

	virtual void pintaidarticulo (QString)	{};
	virtual void pintacodarticulo (QString)	{};
	virtual void pintanomarticulo (QString)	{};
	virtual void pintaabrevarticulo (QString)	{};
	virtual void pintaobservarticulo (QString)	{};
	virtual void pintapresentablearticulo (QString)	{};
	virtual void pintacontrolstockarticulo (QString)	{};
	virtual void pintaidtipo_articulo (QString)	{};
	virtual void pintaidtipo_iva (QString)	{};
	virtual void pintacodigocompletoarticulo (QString)	{};
	virtual void pintaidfamilia (QString)	{};
	virtual void pintastockarticulo (QString)	{};
	virtual void pintainactivoarticulo (QString)	{};
	virtual void pintapvparticulo (QString)	{};
};

#endif
