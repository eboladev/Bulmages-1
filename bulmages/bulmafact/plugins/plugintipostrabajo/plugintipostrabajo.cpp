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
#include <QToolButton>

#include "plugintipostrabajo.h"
#include "listtipostrabajoview.h"
#include "tiptrab.h"
#include "busquedatipotrabajo.h"

mytiptrab::mytiptrab() {}


mytiptrab::~mytiptrab() {}


void mytiptrab::elslot() {
    ListTiposTrabajoView *l = new ListTiposTrabajoView((Company *)m_bulmafact->getcompany(), 0);
    m_bulmafact->workspace()->addWindow(l);
    l->show();
}


void mytiptrab::inicializa(Bulmafact *bges) {
    /// Creamos el men&uacute;.
    m_bulmafact = bges;
    QAction *accion = new QAction("&Tipos de Trabajo", 0);
    accion->setStatusTip("Tipos de Trabajo");
    accion->setWhatsThis("Tipos de Trabajo");
    connect(accion, SIGNAL(activated()), this, SLOT(elslot()));
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuMaestro->insertAction(bges->actionTrabajadores, accion);
}


int entryPoint(Bulmafact *bges) {
    _depura("Estoy dentro del plugin de tipos de trabajo", 0);
    mytiptrab *plug = new mytiptrab();
    plug->inicializa(bges);
    return 0;
}

/// Al crear la ventana de trabajadores tambien creamos un combo box para el tipo de trabajador.
int TrabajadorView_TrabajadorView_Post (TrabajadorView *trab) {
    _depura("TrabajadorView_TrabajadorView_Post", 0);

    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing(2);
    hboxLayout160->setMargin(0);
    hboxLayout160->setObjectName(QString::fromUtf8("hboxLayout16"));

    QLabel *textLabel2_9_26 = new QLabel(trab->m_frameplugin);
    textLabel2_9_26->setObjectName(QString::fromUtf8("textLabel2_9_2"));
    hboxLayout160->addWidget(textLabel2_9_26);
    textLabel2_9_26->setText("Tipo Trabajo");

    BusquedaTipoTrabajo *tipotraba = new BusquedaTipoTrabajo(trab->m_frameplugin);
    tipotraba->setEmpresaBase(trab->empresaBase());
    tipotraba->setidtipotrabajo("");
    tipotraba->setObjectName(QString::fromUtf8("tipotraba"));
    hboxLayout160->addWidget(tipotraba);

   trab->m_frameplugin->setLayout(hboxLayout160);
   return 0;
}


int TrabajadorView_on_mui_guardar_clicked(TrabajadorView *trab) {

    BusquedaTipoTrabajo *l = trab->findChild<BusquedaTipoTrabajo *>("tipotraba");
    QString query = "UPDATE trabajador SET ";
    query += " idtipotrabajo = " + l->idtipotrabajo();
    query += " WHERE idtrabajador=" + trab->empresaBase()->sanearCadena(trab->idtrabajador());
    trab->empresaBase()->begin();
    trab->empresaBase()->ejecuta(query);
    trab->empresaBase()->commit();
    return 0;
}

int TrabajadorView_on_mui_lista_currentItemChanged_Post(TrabajadorView *trab) {
    BusquedaTipoTrabajo *l = trab->findChild<BusquedaTipoTrabajo *>("tipotraba");

    cursor2 *cur = trab->empresaBase()->cargacursor("SELECT idtipotrabajo FROM trabajador WHERE idtrabajador = " + trab->idtrabajador());
    if (!cur->eof()) {
	l->setidtipotrabajo(cur->valor("idtipotrabajo")); 
    }
    delete cur;
    return 0;
}



