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

#ifndef ARTICULOVIEW_H
#define ARTICULOVIEW_H

#include "articleeditbase.h"
#include "postgresiface2.h"
#include "dialogchanges.h"


class company;

class ArticuloView : public articleeditbase, public dialogChanges
{
	Q_OBJECT

private:
	QString mdb_idarticulo;
	QString m_archivoimagen;
	company *m_companyact;
	cursor2 *m_cursorcombo;

public:
	ArticuloView(company *emp, QWidget *parent = 0, const char *name = 0);
	~ArticuloView();
	company *companyact() {return m_companyact;};
	QString idarticulo() {return mdb_idarticulo;};

public:
	int cargar(QString);
	int ArticuloView::cargarcomboiva(QString);
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
