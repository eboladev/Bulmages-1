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

#include <QMenu>
#include <QAction>
#include <QObject>
#include <QMessageBox>
#include <QInputDialog>
#include <QStringList>
#include <QTranslator>
#include <QTextCodec>
#include <QLocale>

#include <stdio.h>

#include "plugincuadrante.h"
#include "company.h"
#include "funcaux.h"
#include "facturaview.h"
#include "presupuestoview.h"
#include "pedidoclienteview.h"
#include "albaranclienteview.h"
#include "pedidoproveedorview.h"

myplugin4::myplugin4() {}


myplugin4::~myplugin4() {}


void myplugin4::elslot() {
    bool ok;
    QString text = QInputDialog::getText(0, tr("QInputDialog::getText()"),
                                         tr("Introduzca codigo"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) {
        QStringList listaelem = text.split(" ");
        if (listaelem.at(0) == QString("FAC")) {
            FacturaView *prov = ((Company *)empresaBase())->newFacturaView();
            if (prov->cargar(listaelem.at(1))) {
                delete prov;
                return;
            } // end if
            empresaBase()->m_pWorkspace->addWindow(prov);
            prov->show();
        } else if (listaelem.at(0) == QString("PRE")) {
            PresupuestoView *prov = ((Company *)empresaBase())->nuevoPresupuestoView();
            if (prov->cargar(listaelem.at(1))) {
                delete prov;
                return;
            } // end if
            empresaBase()->m_pWorkspace->addWindow(prov);
            prov->show();
        } else if (listaelem.at(0) == QString("PED")) {
            PedidoClienteView *prov = new PedidoClienteView((Company *)empresaBase(), 0);
            if (prov->cargar(listaelem.at(1))) {
                delete prov;
                return;
            } // end if
            empresaBase()->m_pWorkspace->addWindow(prov);
            prov->show();
        } else if (listaelem.at(0) == QString("ALB")) {
            AlbaranClienteView *prov = ((Company *)empresaBase())->newAlbaranClienteView();
            if (prov->cargar(listaelem.at(1))) {
                delete prov;
                return;
            } // end if
            empresaBase()->m_pWorkspace->addWindow(prov);
            prov->show();
        } else if (listaelem.at(0) == QString("PEDP")) {
            PedidoProveedorView *prov = new PedidoProveedorView((Company *)empresaBase(), 0);
            if (prov->cargar(listaelem.at(1))) {
                delete prov;
                return;
            } // end if
            empresaBase()->m_pWorkspace->addWindow(prov);
            prov->show();
        } // end if
    } // end if
}


void myplugin4::inicializa(Bulmafact *bges) {
    /// Creamos el men&uacute;.
    setEmpresaBase(bges->getcompany());
    m_bulmafact = bges;
    QMenu *pPluginMenu;
    /// Miramos si existe un menu Herramientas
    pPluginMenu = bges->menuBar()->findChild<QMenu *>("Herramientas");

    /// Creamos el men&uacute;.
    if (!pPluginMenu) {
        pPluginMenu = new QMenu("&Herramientas", bges->menuBar());
        pPluginMenu->setObjectName(QString::fromUtf8("Herramientas"));
    } // end if

    QAction *accion = new QAction("&Cuadrantes", 0);
    accion->setStatusTip("Permite realizar Plannings Laborales");
    accion->setWhatsThis("Podra destinar los trabajadores a distintos almacenes");
    connect(accion, SIGNAL(activated()), this, SLOT(elslot()));
    pPluginMenu->addAction(accion);
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar()->insertMenu(bges->menuVentana->menuAction(), pPluginMenu);
}


void entryPoint(Bulmafact *bges) {
        /// Cargamos el sistema de traducciones una vez pasado por las configuraciones generales
        QTranslator *traductor = new QTranslator(0);
        if (confpr->valor(CONF_TRADUCCION) == "locales") {
            traductor->load(QString("pluginplanning_") + QLocale::system().name(),
                            confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());
        } else {
            QString archivo = "pluginplanning_" + confpr->valor(CONF_TRADUCCION);
           traductor->load(archivo, confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());
        } // end if
        theApp->installTranslator(traductor);

    myplugin4 *plug = new myplugin4();
    plug->inicializa(bges);
}

