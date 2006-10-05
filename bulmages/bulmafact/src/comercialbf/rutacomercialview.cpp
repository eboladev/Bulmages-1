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

using namespace std;

#include "funcaux.h"
#define coma "'"

RutaComercialView::RutaComercialView(company *comp, QWidget *parent)
        : QWidget(parent), RutaComercial(comp), dialogChanges(this) {
    _depura("RutaComercialView::RutaComercialView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    dialogChanges_cargaInicial();
    _depura("END RutaComercialView::RutaComercialView", 0);
}


RutaComercialView::RutaComercialView(QWidget *parent)
        : QWidget(parent), RutaComercial(NULL), dialogChanges(this) {
    _depura("RutaComercialView::RutaComercialView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    dialogChanges_cargaInicial();
    _depura("END RutaComercialView::RutaComercialView", 0);
}


RutaComercialView::~RutaComercialView() {
    _depura("END RutaComercialView::RutaComercialView", 0);
}


void RutaComercialView::closeEvent(QCloseEvent *e) {
    _depura("closeEvent", 0);
    if (dialogChanges_hayCambios())  {
        int val = QMessageBox::warning(this,
                                       tr("Guardar cobro"),
                                       tr("Desea guardar los cambios?"),
                                       tr("&Si"), tr("&No"), tr("&Cancelar"), 0, 2);
        if (val == 0)
            guardar();
        if (val == 2)
            e->ignore();
    } // end if
    _depura("END closeEvent", 0);
}

