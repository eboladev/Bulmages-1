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


#include "listseriefacturaview.h"
#include "company.h"
#include "configuracion.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include <QMessageBox>
#include <Q3Table>
#include <QWidget>
#include <QComboBox>
#include <Q3PopupMenu>
#include <QToolButton>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>
#include <fstream>
using namespace std;
#include "funcaux.h"

#define COL_CODIGOSERIE_FACTURA 0
#define COL_DESCSERIE_FACTURA 1
#define COL_ORIGINALSERIE_FACTURA 2

// DEBUGMODE => 0 = Disabled; 1 = Enabled;
#define DEBUGMODE 1

ListSerieFacturaView::ListSerieFacturaView( company *comp , QWidget *parent, const char *name) : ListSerieFacturaBase(parent, name, Qt::WDestructiveClose) {
	_depura("INIT_ListSerieFacturaView::ListSerieFacturaView", DEBUGMODE);

	companyact = comp;

	m_listado->setNumCols(3);
	m_listado->horizontalHeader()->setLabel( COL_CODIGOSERIE_FACTURA, tr( "Codigo" ) );
	m_listado->horizontalHeader()->setLabel( COL_DESCSERIE_FACTURA, tr( "Descripción" ) );
	m_listado->horizontalHeader()->setLabel( COL_ORIGINALSERIE_FACTURA, tr( "Codigo invisible" ) );

	m_listado->setColumnWidth(COL_DESCSERIE_FACTURA,175);

	m_listado->hideColumn(COL_ORIGINALSERIE_FACTURA);

	inicializa();

	_depura("END_ListSerieFacturaView::ListSerieFacturaView", DEBUGMODE);
}// end ListSerieFacturaView


ListSerieFacturaView::~ListSerieFacturaView() {
}// end ~ListSerieFacturaView



void ListSerieFacturaView::inicializa() {
	_depura("INIT_ListSerieFacturaView::inicializa", DEBUGMODE);

	m_listado->setNumRows(0);

	QString SQLQuery = "SELECT * FROM serie_factura";
	cursor2 *cur = companyact->cargacursor(SQLQuery);

	m_listado->setNumRows(cur->numregistros());
	int i=0;
	while (!cur->eof()) {
		m_listado->setText(i,COL_CODIGOSERIE_FACTURA,cur->valor("codigoserie_factura"));
		m_listado->setText(i,COL_ORIGINALSERIE_FACTURA,cur->valor("codigoserie_factura"));
		m_listado->setText(i,COL_DESCSERIE_FACTURA, cur->valor("descserie_factura"));
		i++;
		cur->siguienteregistro();
	}// end while

	_depura("END_ListSerieFacturaView::inicializa", DEBUGMODE);
}// end inicializa


void ListSerieFacturaView::s_new() {
	_depura("INIT_ListSerieFacturaView::s_new", DEBUGMODE);

	QString SQLQuery = "INSERT INTO serie_factura (codigoserie_factura,descserie_factura) VALUES ('--','--')";
	int error = companyact->ejecuta(SQLQuery);
	if (error) {
		return;
	}// end if
	inicializa();

	_depura("END_ListSerieFacturaView::s_new", DEBUGMODE);
}// end s_new

void ListSerieFacturaView::s_save() {
	_depura("INIT_ListSerieFacturaView::s_save", DEBUGMODE);

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

	_depura("END_ListSerieFacturaView::s_save", DEBUGMODE);
}// end s_save

void ListSerieFacturaView::s_delete() {
	_depura("INIT_ListSerieFacturaView::s_delete", DEBUGMODE);

	int row = m_listado->currentRow();
	if (row < 0) return;
	QString codigooriginal = m_listado->text(row, COL_ORIGINALSERIE_FACTURA);
	QString SQLQuery = "DELETE FROM serie_factura WHERE codigoserie_factura='"+codigooriginal+"'";
	int error = companyact->ejecuta(SQLQuery);
	if (error) return;
	inicializa();

	_depura("END_ListSerieFacturaView::s_delete", DEBUGMODE);
}// end s_delete


int ListSerieFacturaView::guardalinea(int row) {
	_depura("INIT_ListSerieFacturaView::guardalinea", DEBUGMODE);

	QString codigooriginal = m_listado->text(row, COL_ORIGINALSERIE_FACTURA);
	QString codigo = m_listado->text(row, COL_CODIGOSERIE_FACTURA);
	QString desc = m_listado->text(row, COL_DESCSERIE_FACTURA);
	QString SQLQuery = "UPDATE serie_factura SET codigoserie_factura='"+codigo+"', descserie_factura='"+desc+"' WHERE codigoserie_factura= '"+codigooriginal+"'";
	int error = companyact->ejecuta(SQLQuery);
	if (error) {
		return 1;
	}// end if
	return 0;

	_depura("END_ListSerieFacturaView::guardalinea", DEBUGMODE);
}// end guardalinea
