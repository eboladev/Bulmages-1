/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#include <QWidget>
#include <QToolButton>
#include <QString>
#include "company.h"
#include "funcaux.h"
#include <QtXml/QDomDocument>

class EFQToolButtonImportar : public QToolButton {
	Q_OBJECT

private:
	company *m_companyact;
	FacturasProveedorList *m_faclistado;
	
public:
	EFQToolButtonImportar(FacturasProveedorList *faclistado, QWidget *parent = 0);
	~EFQToolButtonImportar();
	void importa_factura_ubl();
	QString obten_valor_nodo(QString nombre, QDomDocument *doc);
	
public slots:
	virtual void click();

};
