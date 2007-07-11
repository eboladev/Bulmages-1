/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#include <QWidget>
#include "clipboardqtoolbutton.h"
#include "funcaux.h"


#include <stdio.h>
#include <stdlib.h>

#include <QFile>
#include <QTextStream>
#include <QClipboard>

#include <QString>
#include <QFileDialog>
#include <QMap>
#include <QList>
#include "facturapview.h"
#include "presupuestoview.h"
#include "fixed.h"


#include "facturaview.h"
#include "company.h"
#include "dbrecord.h"


ClipBoardQToolButton::ClipBoardQToolButton(PresupuestoView *pres, PedidoClienteView *ped, AlbaranClienteView *alb, FacturaView *fac , QWidget *parent) : QToolButton(parent) {
    _depura("ClipBoardQToolButton::ClipBoardQToolButton", 0);
    m_presupuestoView = pres;
    m_pedidoClienteView = ped;
    m_albaranClienteView = alb;
    m_facturaView = fac;
    setBoton();
    _depura("END ClipBoardQToolButton::ClipBoardQToolButton", 0);
}

ClipBoardQToolButton::~ClipBoardQToolButton() {}

void ClipBoardQToolButton::setBoton() {
    _depura("ClipBoardQToolButton::setBoton", 0);
    connect(this, SIGNAL(clicked()), this, SLOT(click()));
    setObjectName(QString::fromUtf8("exporta"));
    setStatusTip("Importar desde ClipBoard");
    setToolTip("Importar desde ClipBoard");
    setMinimumSize(QSize(32, 32));
    setIcon(QIcon(QString::fromUtf8("/usr/share/bulmages/icons/ticket.png")));
    setIconSize(QSize(22, 22));
    _depura("END ClipBoardQToolButton::setBoton", 0);
}


void ClipBoardQToolButton::click() {
    _depura("ImpQToolButton::click", 0);

    if (m_albaranClienteView != NULL) {

    } // end if

    if (m_facturaView != NULL) {
        m_companyact = m_facturaView->getcompany();
	QString clipboard = theApp->clipboard()->text();

	QStringList lineas = clipboard.split("\n");

	/// La primera linea tiene los nombres de las columnas.
	QStringList campos = lineas.at(0).simplified().split(" ");

	/// Calculamos el tamanyo de cada campo.
	int numcampos = campos.size();
	int numchars  = lineas.at(0).size() / numcampos;

	/// Iteramos para cada linea
        for (int i = 1; i < lineas.size() -1 ; ++i) {
		QString cadena_valores = lineas.at(i);

		/// Creamos un elemento en la factura
		SDBRecord  *linea1;
		linea1 = m_facturaView->getlistalineas()->lineaat(m_facturaView->getlistalineas()->rowCount() - 1);
		/// Haciendo el nuevo registro antes nos evitamos problemas de foco.
		m_facturaView->getlistalineas()->nuevoRegistro();

		/// Iteramos para cada columna.
		for (int j = 0; j < campos.size(); ++j) {
			/// Cogemos un valor.
			QString valorcampo = cadena_valores.left(numchars).simplified();
			cadena_valores = cadena_valores.right(cadena_valores.size()- numchars);
			linea1->setDBvalue(campos.at(j), valorcampo);
		} // end for
	} // end for
    } // end if

_depura("END ImpQToolButton::click", 0);
}


