/***************************************************************************
 *   Copyright (C) 2004 by Alvaro de Miguel                                *
 *   alvaro_demiguel@gmail.com                                             *
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

#ifndef ARTICLEEDIT_H
#define ARTICLEEDIT_H

#include "articleeditbase.h"
#include "postgresiface2.h"
#include "dialogchanges.h"


class company;

class articleedit : public articleeditbase, public dialogChanges
{
	Q_OBJECT

private:
	QString idArticle;
	QString m_archivoimagen;
	company *companyact;
	cursor2 *m_cursorcombo;

public:
	articleedit(company *emp, QWidget *parent = 0, const char *name = 0);
	~articleedit();

public:
	int chargeArticle(QString);
	int articleedit::cargarcomboiva(QString);
	void closeEvent( QCloseEvent *);

public slots:
	virtual void accept();
	virtual void boton_nuevo();
	virtual void boton_borrar();
	virtual void s_findArticulo();
	virtual void s_grabarClicked();
	virtual void s_cambiarimagen();
};

#endif
