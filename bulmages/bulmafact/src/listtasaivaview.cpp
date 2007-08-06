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

#include "listtasaivaview.h"
#include "company.h"
#include "configuracion.h"
#include "busquedacliente.h"
#include "busquedafecha.h"

#include "funcaux.h"


ListTasaIVAView::ListTasaIVAView(company *comp, QWidget *parent)
        : FichaBf(comp, parent) {
    _depura("ListTasaIVAView::ListTasaIVAView", 1);
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    mui_listado->setEmpresaBase(comp);
    mui_listado->cargar();
    meteWindow(windowTitle(), this);
    _depura("END ListTasaIVAView::ListTasaIVAView", 1);
}


void ListTasaIVAView::on_mui_aceptar_clicked() {
    try {
        mui_listado->guardar();
        close();
    } catch (...) {
        mensajeInfo("Error al guardar los almacenes");
    } // end try
}


ListTasaIVAView::~ListTasaIVAView() {
    _depura("ListTasaIVAView::~ListTasaIVAView", 0);
    _depura("END ListTasaIVAView::~ListTasaIVAView", 0);
}


/// ===================================== SUBFORMULARIO ===============================================
ListTasaIVASubForm::ListTasaIVASubForm(QWidget *parent) : SubForm2Bf(parent) {
    _depura("ListTasaIVASubForm::ListTasaIVASubForm", 0);
    setDBTableName("tasa_iva");
    setDBCampoId("idtasa_iva");
    addSHeader("idtasa_iva", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, tr("Id Tasa IVA"));
    addSHeader("desctipo_iva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Tipo IVA"));
    addSHeader("idtipo_iva", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id Tipo IVA"));
    addSHeader("porcentasa_iva", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Porcentaje IVA"));
    addSHeader("porcentretasa_iva", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Porcentaje Recargo Equivalencia"));
    addSHeader("fechatasa_iva", DBCampo::DBdate, DBCampo::DBNotNull, SHeader::DBNone, tr("Fecha Entrada en Vigor"));
    setinsercion(TRUE);
    _depura("END ListTasaIVASubForm::ListTasaIVASubForm", 0);
}


void ListTasaIVASubForm::cargar() {
        _depura("ListTasaIVASubForm::cargar", 0);
        cursor2 * cur= empresaBase()->cargacursor("SELECT * FROM tasa_iva LEFT JOIN tipo_iva on tasa_iva.idtipo_iva = tipo_iva.idtipo_iva");
        SubForm3::cargar(cur);
        delete cur;
        _depura("END ListTasaIVASubForm::cargar", 0);
}

