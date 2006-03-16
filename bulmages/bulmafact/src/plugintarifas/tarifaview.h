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

#ifndef TARIFAVIEW_H
#define TARIFAVIEW_H

#include "ui_tarifabase.h"
#include "postgresiface2.h"
#include "dialogchanges.h"
#include "dbrecord.h"

class company;

class TarifaView : public QWidget, public Ui_TarifaBase, public dialogChanges, public DBRecord
{
	Q_OBJECT

private:
	QString m_idtarifa;
	QString m_nomtarifa;
	company *m_companyact;
	cursor2 *m_cursorcombo;

public:
	TarifaView(company *emp, QWidget *parent = 0, const char *name = 0);
	~TarifaView();
	company *companyact() {return m_companyact;};
public:
	void pintar();
	int cargar(QString);
	int guardar();
	int TarifaView::cargarcomboiva(QString);
	void closeEvent( QCloseEvent *);
	QString formaQuery(QString);

public slots:
	virtual void on_mui_guardar_clicked(){guardar();};
	virtual void on_mui_crear_clicked();
	virtual void on_mui_borrar_clicked();
	virtual void on_mui_aceptar_clicked();
	virtual void on_mui_actualizar_clicked();
};




#endif
