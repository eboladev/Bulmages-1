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
#include <QCloseEvent>

#include <fstream>

#include "cobroview.h"
#include "company.h"
#include "configuracion.h"
#include "busquedacliente.h"
#include "busquedafecha.h"

#include "funcaux.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
CobroView::CobroView(company *comp, QWidget *parent)
        : Ficha(parent), Cobro(comp) {
    _depura("CobroView::CobroView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    try {
        setupUi(this);
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        mui_cliente->setcompany(comp);
        mui_refcobro->setcompany(comp);
        dialogChanges_cargaInicial();
        companyact->meteWindow(windowTitle(), this, FALSE);
    } catch (...) {
        mensajeInfo(tr("Error al crear el cobro"));
    } // end try
    _depura("END CobroView::CobroView", 0);
}


/** No precisa acciones adicionales en el destructor. 
*/
CobroView::~CobroView() {
    _depura("CobroView::~CobroView", 0);
    companyact->refreshCobrosCliente();
    _depura("END CobroView::~CobroView", 0);
}


/** Saca la ventana del workSpace.
    Este metodo es invocado desde la clase Ficha.
*/
int CobroView::sacaWindow() {
    _depura("CobroView::sacaWindow", 0);
    companyact->sacaWindow(this);
    _depura("END CobroView::sacaWindow", 0);
    return 0;
}

/** Metodo de carga de un cobro.
    Delega el meanejo con la base de datos a la clase Cobro.
    Sin embargo se encarga de cambiar el titulo de la ventana y de refrescar.
    
    Si todo va bien devuelve 0.
    Si se producen errores devuelve -1.
*/
int CobroView::cargar(QString id) {
    _depura("CobroView::cargar", 0);
    try {
        if (Cobro::cargar(id))
            throw -1;
        setWindowTitle(tr("Cobro") + " " + DBvalue("refcobro") + " " + DBvalue("idcobro"));
        pintar();
        dialogChanges_cargaInicial();
        companyact->meteWindow(windowTitle(), this);
    } catch (...) {
        return -1;
    } // end try
    _depura("END CobroView::cargar", 0);
    return 0;
}

