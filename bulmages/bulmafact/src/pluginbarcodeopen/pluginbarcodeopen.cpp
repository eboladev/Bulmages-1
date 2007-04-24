/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
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

#include <QMenu>
#include <QAction>
#include <QObject>
#include <QMessageBox>
#include <QInputDialog>
#include <QStringList>

#include <stdio.h>

#include "pluginbarcodeopen.h"
#include "company.h"
#include "funcaux.h"
#include "facturaview.h"

myplugin::myplugin() {}


myplugin::~myplugin() {}


void myplugin::elslot() {
        bool ok;
	QString text = QInputDialog::getText(0, tr("QInputDialog::getText()"),
	tr("Introduzca codigo"), QLineEdit::Normal, "", &ok);
	if (ok && !text.isEmpty()) {
	    QStringList listaelem = text.split(" ");
	    if (listaelem.at(0) == QString("FAC")) {
		FacturaView *prov = m_companyact->newFacturaView();
		if (prov->cargar(listaelem.at(1))) {
			delete prov;
			return;
		} // end if
		m_companyact->m_pWorkspace->addWindow(prov);
		prov->show();
	    } else if (listaelem.at(0) == QString("PRE")) {
		    _depura("Abriendo Presupuesto", 2);
	    }
	} // end if
}


void myplugin::inicializa(bulmafact *bges) {
    /// Creamos el men&uacute;.
    m_companyact = bges->getcompany();
    m_bulmafact = bges;
    /// Creamos el men&uacute;.
    QMenu *pPluginMenu = new QMenu("&Herramientas");
    QAction *accion = new QAction("&Apertura Rapida", 0);
    accion->setStatusTip("Abre documentos a partir del codigo de barras");
    accion->setWhatsThis("Abre documentos a partir del codigo de barras");
    connect(accion, SIGNAL(activated()), this, SLOT(elslot()));
    pPluginMenu->addAction(accion);
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar()->insertMenu(bges->menuVentana->menuAction(), pPluginMenu);
}


void entryPoint(bulmafact *bges) {
    myplugin *plug = new myplugin();
    plug->inicializa(bges);
    /// S&Oacute;LO A MODO DE EJEMPLO: se modifica el t&iacute;tulo de la ventana principal
    /// del programa para indicar que el plugin se ha cargado.
    //bges->setWindowTitle("Prueba de plugin.");
}

