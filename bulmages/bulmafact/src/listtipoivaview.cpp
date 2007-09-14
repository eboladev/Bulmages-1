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

#include <QMessageBox>
#include <QWidget>
#include <QComboBox>
#include <QToolButton>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>

#include <fstream>

#include "listtipoivaview.h"
#include "company.h"
#include "configuracion.h"
#include "busquedacliente.h"
#include "busquedafecha.h"

#include "funcaux.h"


ListTipoIVAView::ListTipoIVAView(Company *comp, QWidget *parent)
        : FichaBf(comp, parent) {
    _depura("ListTipoIVAView::ListTipoIVAView", 1);
    setTitleName(tr("Tipo IVA"));
    setDBTableName("tipo_iva");
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    mui_listado->setEmpresaBase(comp);
    mui_listado->cargar();
    meteWindow(windowTitle(), this);
    _depura("END ListTipoIVAView::ListTipoIVAView", 1);
}


void ListTipoIVAView::on_mui_aceptar_clicked() {
    try {
        mui_listado->guardar();
        close();
    } catch (...) {
        mensajeInfo("Error al guardar los almacenes");
    } // end try
}


ListTipoIVAView::~ListTipoIVAView() {
    _depura("ListTipoIVAView::~ListTipoIVAView", 0);
    _depura("END ListTipoIVAView::~ListTipoIVAView", 0);
}

/*
int ListTipoIVAView::sacaWindow() {
    _depura("ListTipoIVAView::sacaWindow", 0);
    companyact->sacaWindow(this);
    _depura("END ListTipoIVAView::sacaWindow", 0);
    return 0;
}
*/

/// ===================================== SUBFORMULARIO ===============================================
ListTipoIVASubForm::ListTipoIVASubForm(QWidget *parent) : SubForm2Bf(parent) {
    _depura("ListTipoIVASubForm::ListTipoIVASubForm", 0);
    setDBTableName("tipo_iva");
    setDBCampoId("idtipo_iva");
    addSHeader("idtipo_iva", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, tr("Id Tipo IVA"));
    addSHeader("desctipo_iva", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, tr("Tipo IVA"));
    setinsercion(TRUE);
    _depura("END ListTipoIVASubForm::ListTipoIVASubForm", 0);
}

void ListTipoIVASubForm::cargar() {
        _depura("ListTipoIVASubForm::cargar", 0);
        cursor2 * cur= empresaBase()->cargacursor("SELECT * FROM tipo_iva");
        SubForm3::cargar(cur);
        delete cur;
        _depura("END ListTipoIVASubForm::cargar", 0);
}

