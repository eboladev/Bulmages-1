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
#include <QCloseEvent>

#include "incidenciacomercialview.h"
#include "company.h"
#include "configuracion.h"
#include "busquedacliente.h"
#include "busquedafecha.h"

#include <fstream>

#include "funcaux.h"
#define coma "'"


IncidenciaComercialView::IncidenciaComercialView(company *comp, QWidget *parent)
        : IncidenciaComercial(comp, parent) {
    _depura("IncidenciaComercialView::IncidenciaComercialView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    dialogChanges_cargaInicial();
    _depura("END IncidenciaComercialView::IncidenciaComercialView", 0);
}


IncidenciaComercialView::IncidenciaComercialView(QWidget *parent)
        :  IncidenciaComercial(NULL, parent) {
    _depura("IncidenciaComercialView::IncidenciaComercialView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    dialogChanges_cargaInicial();
    _depura("END IncidenciaComercialView::IncidenciaComercialView", 0);
}


IncidenciaComercialView::~IncidenciaComercialView() {
    _depura("END IncidenciaComercialView::IncidenciaComercialView", 0);
}


void IncidenciaComercialView::closeEvent(QCloseEvent *e) {
    _depura("closeEvent", 0);
    if (dialogChanges_hayCambios()) {
        int val = QMessageBox::warning(this,
                                       tr("Guardar la incidencia"),
                                       tr("Desea guardar los cambios?"), tr("&Si"), tr("&No"), tr("&Cancelar"), 0, 2);
        if (val == 0)
            guardar();
        if (val == 2)
            e->ignore();
    } // end if
}

void IncidenciaComercialView::setEmpresaBase(company *comp) {
    _depura("IncidenciaComercialView::setcompany", 0);
    PEmpresaBase::setEmpresaBase(comp);
    mui_idcliente->setEmpresaBase(comp);
    mui_idtrabajador->setEmpresaBase(comp);
    mui_idtrabajador->setidtrabajador("");
    _depura("END IncidenciaComercialView::setcompany", 0);

}

int IncidenciaComercialView::guardar() {
    _depura("IncidenciaComercialView::guardar", 0);
    if (mui_fechaincidenciacomercial->text() == "")
        return 0;
    setDBvalue("fechaincidenciacomercial", mui_fechaincidenciacomercial->text());
    setDBvalue("idcliente", mui_idcliente->idcliente());
    setDBvalue("comentincidenciacomercial", mui_comentincidenciacomercial->toPlainText());
    setDBvalue("idtrabajador", mui_idtrabajador->idtrabajador());
    setDBvalue("estadoincidenciacomercial", mui_estadoincidenciacomercial->estado());
    setDBvalue("horaincidenciacomercial", mui_horaincidenciacomercial->text());
    setDBvalue("refincidenciacomercial", mui_refincidenciacomercial->text());
    int err = IncidenciaComercial::guardar();
    _depura("END IncidenciaComercialView::guardar", 0);
    return err;
}


int IncidenciaComercialView::cargar(QString id) {
    _depura("IncidenciaComercialView::cargar", 0);
    int err = IncidenciaComercial::cargar(id);
    setWindowTitle(tr("Incidencia comercial") + " " + DBvalue("idincidenciacomercial"));
    mui_fechaincidenciacomercial->setText(DBvalue("fechaincidenciacomercial"));
    mui_idcliente->setidcliente(DBvalue("idcliente"));
    mui_comentincidenciacomercial->setPlainText(DBvalue("comentincidenciacomercial"));
    mui_idtrabajador->setidtrabajador(DBvalue("idtrabajador"));
    mui_estadoincidenciacomercial->setestado(DBvalue("estadoincidenciacomercial"));
    mui_horaincidenciacomercial->setText(DBvalue("horaincidenciacomercial"));
    mui_refincidenciacomercial->setText(DBvalue("refincidenciacomercial"));
    dialogChanges_cargaInicial();
    _depura("IncidenciaComercialView::cargar", 0);
    return err;
}

