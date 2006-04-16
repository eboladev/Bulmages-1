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

#ifndef TIPOSARTICULOVIEW_H
#define TIPOSARTICULOVIEW_H

#include "ui_tiposarticulodlg.h"
#include "dialogchanges.h"


class company;

class tiposarticuloview : public QDialog, public Ui_tiposarticulodlg, dialogChanges
{
	Q_OBJECT

private:
	company *companyact;
	/// Indica cual es el registro que se esta modificando. Sirve para saber los
	/// cambios que hay que guardar cuando se ha modificado.
	QString m_idtipo;
	/// Indica si es modo consulta o modo edicion. (altera el comportamiento del
	/// doble click sobre la lista)
	bool m_modoConsulta;

public:
	tiposarticuloview(company *, QWidget *parent = 0, const char *name = 0);
	~tiposarticuloview();
	bool trataModificado();
	void mostrarplantilla();
	void setModoConsulta()
	{
		m_modoConsulta = TRUE;
	};
	void setModoEdicion()
	{
		m_modoConsulta = FALSE;
	};
	QString codtipo_articulo();
	QString idtipo_articulo();
	QString desctipo_articulo();

public slots:
	virtual void pintar();
	virtual void on_m_listTipos_itemDoubleClicked ( QTreeWidgetItem * item, int column );
	virtual void close();
	virtual void on_mui_guardar_clicked();
	virtual void on_mui_crear_clicked();
	virtual void on_mui_borrar_clicked();
	virtual void on_m_listTipos_currentItemChanged ( QTreeWidgetItem * current, QTreeWidgetItem * previous);
	virtual void on_mui_aceptar_clicked() {
		close();
	};
};

#endif
