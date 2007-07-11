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
#include <QClipboard>

#include <stdio.h>

#include "pluginclipboardbf.h"
#include "company.h"
#include "funcaux.h"
#include "facturaview.h"
#include "presupuestoview.h"
#include "pedidoclienteview.h"
#include "albaranclienteview.h"
#include "pedidoproveedorview.h"
#include "clipboardqtoolbutton.h"


myplugin::myplugin() {}


myplugin::~myplugin() {}


void myplugin::elslot() {
   _depura(theApp->clipboard()->text(), 2);
}


void myplugin::inicializa(bulmafact *bges) {
    /// Creamos el men&uacute;.
    setEmpresaBase(bges->getcompany());
    m_bulmafact = bges;
    QMenu *pPluginMenu;
    /// Miramos si existe un menu Herramientas
    pPluginMenu = bges->menuBar()->findChild<QMenu *>("Editar");

    /// Creamos el men&uacute;.
    if (!pPluginMenu) {
        pPluginMenu = new QMenu("&Editar", bges->menuBar());
        pPluginMenu->setObjectName(QString::fromUtf8("Editar"));
    } // end if

    QAction *accion = new QAction("&Pegado desde Hoja de Calculo", 0);
    accion->setStatusTip("Pega Contenidos de facturas desde una hoja de calculo");
    accion->setWhatsThis("Pega Contenidos de facturas desde una hoja de calculo");
    connect(accion, SIGNAL(activated()), this, SLOT(elslot()));
    pPluginMenu->addAction(accion);
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar()->insertMenu(bges->menuCompras->menuAction(), pPluginMenu);
}


void entryPoint(bulmafact *bges) {
    myplugin *plug = new myplugin();
    plug->inicializa(bges);
}

int FacturaView_FacturaView(FacturaView *l) {
       _depura("FacturaView_FacturaView", 0);
//================================
       ClipBoardQToolButton *mui_exporta_efactura2 = new ClipBoardQToolButton(NULL, NULL, NULL, l, l->mui_plugbotones);

       QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *>("hboxLayout1");
       if (!m_hboxLayout1) {
                m_hboxLayout1 = new QHBoxLayout(l->mui_plugbotones);
                m_hboxLayout1->setSpacing(5);
                m_hboxLayout1->setMargin(5);
                m_hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
       } // end if
       m_hboxLayout1->addWidget(mui_exporta_efactura2);
//================================
       _depura("END FacturaView_FacturaView", 0);
       return 0;
}