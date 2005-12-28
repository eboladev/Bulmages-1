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


#include "listprovinciasview.h"
#include "company.h"
#include "configuracion.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include <qmessagebox.h>
#include <q3table.h>
#include <qwidget.h>
#include <qcombobox.h>
#include <q3popupmenu.h>
#include <qtoolbutton.h>
#include <qtextstream.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <fstream>
using namespace std;
#include "funcaux.h"

#define COL_PROVINCIA 0
#define COL_ORIGINALPROVINCIA 1


ListProvinciasView::ListProvinciasView( company *comp , QWidget *parent, const char *name) : ListProvinciasBase(parent, name, Qt::WDestructiveClose) {
	companyact = comp;
	m_listado->setNumCols(2);
	m_listado->horizontalHeader()->setLabel( COL_PROVINCIA, tr( "Provincia" ) );
	m_listado->horizontalHeader()->setLabel( COL_ORIGINALPROVINCIA, tr( "Provincia" ) );
	m_listado->setColumnWidth(COL_PROVINCIA,175);
	m_listado->hideColumn(COL_ORIGINALPROVINCIA);
	inicializa();
}// end ListSerieFacturaView


ListProvinciasView::~ListProvinciasView() {
}// end ~ListSerieFacturaView



void ListProvinciasView::inicializa() {
	m_listado->setNumRows(0);

	QString SQLQuery = "SELECT * FROM provincia";
	cursor2 *cur = companyact->cargacursor(SQLQuery);

	m_listado->setNumRows(cur->numregistros());
	int i=0;
	while (!cur->eof()) {
		m_listado->setText(i,COL_PROVINCIA,cur->valor("provincia"));
		m_listado->setText(i,COL_ORIGINALPROVINCIA,cur->valor("provincia"));
		i++;
		cur->siguienteregistro();
	}// end while
}// end inicializa


void ListProvinciasView::s_new() {
	QString SQLQuery = "INSERT INTO provincia (provincia) VALUES ('--')";
	int error = companyact->ejecuta(SQLQuery);
	if (error) {
		return;
	}// end if
	inicializa();
}// end s_new

void ListProvinciasView::s_save() {
	companyact->begin();
	int i = 0;
	while (i < m_listado->numRows()) {
		int error = guardalinea(i);
		if (error) {
			companyact->rollback();
			return;
		}// end if
		i++;
	}// end while
	companyact->commit();
	inicializa();
}// end s_save

void ListProvinciasView::s_delete() {
	int row = m_listado->currentRow();
	if (row < 0) return;
	QString codigooriginal = m_listado->text(row, COL_ORIGINALPROVINCIA);
	QString SQLQuery = "DELETE FROM provincia WHERE provincia='"+codigooriginal+"'";
	int error = companyact->ejecuta(SQLQuery);
	if (error) return;
	inicializa();
}// end s_delete


int ListProvinciasView::guardalinea(int row) {
	QString codigooriginal = m_listado->text(row, COL_ORIGINALPROVINCIA);
	QString codigo = m_listado->text(row, COL_PROVINCIA);

	QString SQLQuery = "UPDATE provincia SET provincia='"+codigo+"' WHERE provincia= '"+codigooriginal+"'";
	int error = companyact->ejecuta(SQLQuery);
	if (error) {
		return 1;
	}// end if
	return 0;
}// end guardalinea
