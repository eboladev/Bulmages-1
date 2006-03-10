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

#ifndef TRABAJADORVIEW_H
#define TRABAJADORVIEW_H

#include <ui_trabajadorbase.h>
#include "dialogchanges.h"
#include "postgresiface2.h"

class company;

class TrabajadorView : public QDialog, private Ui_TrabajadorBase, dialogChanges
{
	Q_OBJECT

private:
	company *m_companyact;
	cursor2 *m_cursortrabajadores;
	/// Indica cual es el objeto que se esta mostrando.
	QString mdb_idtrabajador;
	/// Indica si es modo consulta o modo edicion. (altera el comportamiento del
	/// doble click sobre la lista)
	bool m_modoConsulta;
	QString m_archivoimagen;
	QListWidgetItem *m_item;

private:
	/// Se encarga de hacer la carga del query inicial y de mostrar la lista bien
	/// y presentar el elemento que se especifique.
	void pintar();

public:
	TrabajadorView(company * emp, QWidget *parent = 0, const char *name = 0);
	~TrabajadorView();
	void setModoConsulta()
	{
		m_modoConsulta = TRUE;
	};
	void setModoEdicion()
	{
		m_modoConsulta = FALSE;
	};
	bool trataModificado();

private slots:
	virtual void on_mui_lista_currentItemChanged(QListWidgetItem *cur, QListWidgetItem *prev);
	virtual void on_mui_guardar_clicked();
	virtual void on_mui_nuevo_clicked();
	virtual void on_mui_borrar_clicked();
	virtual void on_mui_imagen_clicked();
};

#endif
