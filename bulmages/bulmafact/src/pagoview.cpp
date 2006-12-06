/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QFile>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>
#include <QCloseEvent>

#include <fstream>

#include "pagoview.h"
#include "company.h"
#include "configuracion.h"
#include "busquedaproveedor.h"
#include "busquedafecha.h"

#include "funcaux.h"


PagoView::PagoView(company *comp, QWidget *parent)
        : Ficha(parent), Pago(comp) {
    try {
        setAttribute(Qt::WA_DeleteOnClose);
        setupUi(this);
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        mui_proveedor->setcompany(comp);
        mui_refpago->setcompany(comp);
        dialogChanges_cargaInicial();
        companyact->meteWindow(windowTitle(), this, FALSE);
    } catch (...) {
        mensajeInfo(tr("Error al crear el pago"));
    } // end try
    _depura("Fin de la inicializacion de PagoView\n", 0);
}


PagoView::~PagoView() {
    _depura("PagoView::~PagoView", 0);
    _depura("END PagoView::~PagoView", 0);
}


int PagoView::sacaWindow() {
    companyact->sacaWindow(this);
    return 0;
}

