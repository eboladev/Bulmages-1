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

#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QFile>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>
#include <QCloseEvent>

#include "rutacomercialview.h"
#include "company.h"
#include "configuracion.h"
#include "busquedacliente.h"
#include "busquedafecha.h"


#include <fstream>

#include "funcaux.h"
#define coma "'"

RutaComercialView::RutaComercialView(company *comp, QWidget *parent)
        :  RutaComercial(comp, parent) {
    _depura("RutaComercialView::RutaComercialView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    dialogChanges_cargaInicial();
    _depura("END RutaComercialView::RutaComercialView", 0);
}


RutaComercialView::RutaComercialView(QWidget *parent)
        : RutaComercial(NULL, parent) {
    _depura("RutaComercialView::RutaComercialView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    dialogChanges_cargaInicial();
    _depura("END RutaComercialView::RutaComercialView", 0);
}


RutaComercialView::~RutaComercialView() {
    _depura("END RutaComercialView::RutaComercialView", 0);
}



void RutaComercialView::setEmpresaBase(company *comp) {
    _depura("RutaComercialView::setEmpresaBase", 0);
    PEmpresaBase::setEmpresaBase(comp);
    mui_idcliente->setEmpresaBase(comp);
    _depura("END RutaComercialView::setEmpresaBase", 0);
}

int RutaComercialView::guardar() {
    _depura("RutaComercialView::guardar", 0);
    if (mui_fecharutacomercial->text() == "")
        return 0;
    setDBvalue("fecharutacomercial", mui_fecharutacomercial->text());
    setDBvalue("idcliente", mui_idcliente->idcliente());
    setDBvalue("comentariosrutacomercial", mui_comentrutacomercial->toPlainText());
    setDBvalue("horarutacomercial", mui_horarutacomercial->text());
    setDBvalue("refrutacomercial", mui_refrutacomercial->text());
    int err = RutaComercial::guardar();
    _depura("END RutaComercialView::guardar", 0);
    return err;
}

int RutaComercialView::cargar(QString id) {
    _depura("RutaComercialView::cargar", 0);
    int err = RutaComercial::cargar(id);
    setWindowTitle(tr("Ruta comercial") + " " + DBvalue("idrutacomercial"));
    mui_fecharutacomercial->setText(DBvalue("fecharutacomercial"));
    mui_idcliente->setidcliente(DBvalue("idcliente"));
    mui_comentrutacomercial->setPlainText(DBvalue("comentariosrutacomercial"));
    mui_horarutacomercial->setText(DBvalue("horarutacomercial"));
    mui_refrutacomercial->setText(DBvalue("refrutacomercial"));
    dialogChanges_cargaInicial();
    _depura("END RutaComercialView::cargar", 0);
    return err;
}




