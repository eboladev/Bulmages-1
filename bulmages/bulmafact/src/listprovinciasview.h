/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borr� Riera                              *
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
#ifndef LISTPROVINCIASVIEW_H
#define LISTPROVINCIASVIEW_H

// PROVINCIAS

#include "listprovinciasbase.h"
#include "postgresiface2.h"
#include "cobro.h"


#include <qlineedit.h>
#include <q3textedit.h>
#include <qlabel.h>
#include <qcheckbox.h>

/** @author Tomeu Borr� Riera */
class company;

class ListProvinciasView : public ListProvinciasBase  {
    Q_OBJECT
public:
	company *companyact;
public:
    ListProvinciasView(company *, QWidget *, const char *);
    ~ListProvinciasView();

	void inicializa();
	int guardalinea(int);

public slots:
	virtual void s_new();
	virtual void s_save();
	virtual void s_delete();
};

#endif
