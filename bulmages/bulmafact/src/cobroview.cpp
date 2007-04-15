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

/** Escribe en el QLineEdit de fecha la fecha indicada
**/
void CobroView::pintafechacobro(QString id) {
    _depura("CobroView::pintafechacobro", 0);
    mui_fechacobro->setText(id);
    _depura("END CobroView::pintafechacobro", 0);
}

/** Escribe en los comentarios del cobro los comentarios indicados
**/
void CobroView::pintacomentcobro(QString id) {
    _depura("CobroView::pintacomentcobro", 0);
    mui_comentcobro->setText(id);
    _depura("END CobroView::pintacomentcobro", 0);
}

/** Indica a la clase cual es el identificador del cliente del cobro
**/
void CobroView::pintaidcliente(QString id) {
    _depura("CobroView::pintaidcliente", 0);
    mui_cliente->setidcliente(id);
    _depura("END CobroView::pintaidcliente", 0);
}

/** Escribe en la referencia del cobro el parametro indicado
**/
void CobroView::pintarefcobro(QString id) {
    _depura("CobroView::pintarefcobro", 0);
    mui_refcobro->setText(id);
    _depura("END CobroView::pintarefcobro", 0);
}

/** EScribe en la cantidad del cobro el parametro indicado
**/
void CobroView::pintacantcobro(QString id) {
    _depura("CobroView::pintacantcobro", 0);
    mui_cantcobro->setText(id);
    _depura("END CobroView::pintacantcobro", 0);
}

/** Escribe en el QCheckBox de prevision si el parametro indicado
**/
void CobroView::pintaprevisioncobro(QString id) {
    _depura("CobroView::pintaprevisioncobro", 0);
    if (id == "t" || id == "TRUE") {
        mui_previsioncobro->setChecked(TRUE);
    } else {
        mui_previsioncobro->setChecked(FALSE);
    } // end if
    _depura("END CobroView::pintaprevisioncobro", 0);
}

/** Guarda en la base de datos el cobro especificado en la clase
**/
int CobroView::guardar() {
    _depura("CobroView::guardar", 0);
    _depura("END CobroView::guardar", 0);
    return Cobro::guardar();
}

/** Elimina el cobro que se esta viendo de la base de datos
**/
int CobroView::borrar()  {
    _depura("CobroView::borrar", 0);
    _depura("END CobroView::borrar", 0);
    return Cobro::borrar();
}

/** SLOT automatico que se ejecuta al cambiar algun texto en el campo de comentarios
    Actualiza los valores del cobro con los valores introducidos en el campo
**/
void CobroView::on_mui_comentcobro_textChanged(const QString &str) {
    _depura("CobroView::on_mui_comentcobro_textChanged", 0);
    setcomentcobro(str);
    _depura("END CobroView::on_mui_comentcobro_textChanged", 0);
}

/** SLOT automatico que se ejecuta al cambiar el texto del campo de referencia
    Actualiza los valores de la referencia con los valores introducidos en el campo
**/
void CobroView::on_mui_refcobro_valueChanged(const QString &str) {
    _depura("CobroView::on_mui_refcobro_valueChanged", 0);
    setrefcobro(str);
    _depura("END CobroView::on_mui_refcobro_valueChanged", 0);
}

/** SLOT automatico que se ejecuta al cambiar el texto del campo de cantidad
	Actualiza los valores de la cantidad en el cobro con los valores introducidos en el campo
**/
void CobroView::on_mui_cantcobro_textChanged(const QString &str) {
    _depura("CobroView::on_mui_cantcobro_textChanged", 0);
    setcantcobro(str);
    _depura("END CobroView::on_mui_cantcobro_textChanged", 0);
}

/** SLOT automatico que se ejecuta al cambiar el estado del QCheckBox de prevision
	Actualiza los valores de la prevision en el cobro con el valor que tiene el QComboBox
**/
void CobroView::on_mui_previsioncobro_stateChanged(int i) {
    _depura("CobroView::on_mui_previsioncobro_stateChanged", 0);
    if (i) {
        setprevisioncobro("TRUE");
    } else {
        setprevisioncobro("FALSE");
    } // end if
    _depura("END CobroView::on_mui_previsioncobro_stateChanged", 0);
}

/** SLOT automatico que se ejecuta al cambiar el selector de cliente
	Actualiza los valores de idcliente del cobro con el valor que se pasa
**/
void CobroView::on_mui_cliente_valueChanged(QString id) {
    _depura("CobroView::on_mui_cliente_valueChanged", 0);
    setidcliente(id);
    _depura("END CobroView::on_mui_cliente_valueChanged", 0);
}

/** SLOT automatico que se ejecuta al cambiar la fecha
	Actualiza los valores del cobro con la fecha indicada
**/
void CobroView::on_mui_fechacobro_valueChanged(QString id) {
    _depura("CobroView::on_mui_fechacobro_valueChanged", 0);
    setfechacobro(id);
    _depura("END CobroView::on_mui_fechacobro_valueChanged", 0);
}


