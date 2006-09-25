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

#include <QMenu>
#include <QAction>
#include <QObject>
#include <QMessageBox>

#include <stdio.h>

#include "efacturabf.h"
#include "efactura.h"
#include "company.h"
#include "funcaux.h"

efacturabf::efacturabf() {}

efacturabf::~efacturabf() {}

/// Esto es lo que se activa al hacer click sobre el menu
void efacturabf::elslot() {
	EFactura *ef = new EFactura();
	ef->show();
//     fprintf(stderr,"Sa ha activado el slot\n");
//     QMessageBox::warning(0,
//                          "Titulo de la ventana",
//                          "Mensaje.",
//                          QMessageBox::Ok,
//                          QMessageBox::Cancel);
}


void efacturabf::inicializa(bulmafact *bges) {
    /// Creamos el menu.
    QMenu *pPluginMenu = new QMenu("&Factura Electronica");
    QAction *accion = new QAction("&Prueba de e-factura", 0);
    accion->setStatusTip("Muestra statustip");
    accion->setWhatsThis("Muestra que es esto");
    connect(accion, SIGNAL(activated()), this, SLOT(elslot()));
    pPluginMenu->addAction(accion);
    /// Anyadimos la nueva opcion al menu principal del programa.
    bges->menuBar()->addMenu(pPluginMenu);
}


void entryPoint(bulmafact *bges) {
    _depura("Estoy dentro del plugin de e-factura\n", 0);

    efacturabf *efact = new efacturabf();
    efact->inicializa(bges);
    /// SOLO A MODO DE EJEMPLO: se modifica el titulo de la ventana principal
    /// del programa para indicar que el plugin se ha cargado.
    bges->setWindowTitle("Prueba de plugin e-factura.");
}

