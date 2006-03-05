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

#ifndef FPAGO_H
#define FPAGO_H

#include <fpagobase.h>
#include "dialogchanges.h"


class company;

class fpago : public fpagobase, dialogChanges
{
	Q_OBJECT

private:
	company *companyact;
	/// Indica cual es el objeto que se esta mostrando.
	QString m_idforma_pago;
	/// Indica si es modo consulta o modo edicion. (altera el comportamiento
	/// del doble click sobre la lista)
	bool m_modoConsulta;

private:
	/// Se encarga de hacer la carga de la consulta (query) inicial y de mostrar la
	/// lista bien y presentar el elemento que se especifique.
	void pintar();

public:
	fpago(company * emp, QWidget *parent = 0, const char *name = 0);
	~fpago();
	void setModoConsulta()
	{
		m_modoConsulta = TRUE;
	};
	void setModoEdicion()
	{
		m_modoConsulta = FALSE;
	};
	bool trataModificado(); 
	virtual void closeEvent(QCloseEvent *);

private slots:
	virtual void s_lista(Q3ListViewItem *);
	virtual void s_saveFPago();
	virtual void s_newFPago();
	virtual void s_deleteFPago();
};

#endif
