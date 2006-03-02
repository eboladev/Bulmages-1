/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C. (Porting to QT4)              *
 *   fcojavmc@todo-redes.com                                               *
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
#ifndef LISTSERIEVIEW_H
#define LISTSERIEVIEW_H

// SERIES DE FACTURA.

#include "listseriefacturabase.h"
#include "postgresiface2.h"
#include "cobro.h"
#include "busquedacliente.h"
#include "busquedafecha.h"


#include <QLineEdit>
#include <Q3TextEdit>
#include <QLabel>
#include <QCheckBox>

/** @author Tomeu Borras Riera & Fco. Javier M. C. */
class company;

class ListSerieFacturaView : public ListSerieFacturaBase  {
	Q_OBJECT
public:
	company *companyact;
public:
	ListSerieFacturaView(company *, QWidget *, const char *);
	~ListSerieFacturaView();

	void inicializa();
	int guardalinea(int);

public slots:
	virtual void s_new();
	virtual void s_save();
	virtual void s_delete();
};

#endif
