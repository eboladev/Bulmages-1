/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include "myplugin.h"
#include "rutacomerciallist.h"
#include "listzonacomercialview.h"
#include "funcaux.h"


mypluginbf::mypluginbf() : QObject() {}


mypluginbf::~mypluginbf() {}


void mypluginbf::elslot() {
    _depura("mypluginbf::elslot", 0);
    RutaComercialList *list  = new RutaComercialList(m_companyact, NULL);
    m_companyact->m_pWorkspace->addWindow(list);
    list->show();
    _depura("END mypluginbf::elslot", 0);
}


void mypluginbf::elslot1() {
    _depura("mypluginbf::elslot1", 0);
    ListZonaComercialView *list = new ListZonaComercialView(m_companyact, NULL);
    m_companyact->m_pWorkspace->addWindow(list);
    list->show();
    _depura("END mypluginbf::elslot1", 0);
}


void mypluginbf::inicializa(bulmafact *bges) {
    /// El men&uacute; de empresa.
    m_bges = bges;
    /// Cogemos la 'company' para tener acceso a todo y sobretodo a BD.
    m_companyact = bges->getcompany();
    /// Insertamos un separador en el men&uacute;.
    bges->menuMaestro->addSeparator();
    /// Creamos la opci&oacute;n para Zonas Comerciales que dispare el m&eacute;todo adecuado.
    QAction *planCuentas1 = new QAction(tr("&Zonas comerciales"), 0);
    planCuentas1->setStatusTip(tr("Zonas comerciales"));
    planCuentas1->setWhatsThis(tr("Zonas comerciales"));
    bges->menuMaestro->addAction(planCuentas1);
    connect(planCuentas1, SIGNAL(activated()), this, SLOT(elslot1()));
    /// Creamos la opci&oacute;n para Rutas Comerciales que dispare el m&eacute;todo adecuado.
    QAction *planCuentas = new QAction(tr("&Rutas comerciales"), 0);
    planCuentas->setStatusTip(tr("Rutas comerciales"));
    planCuentas->setWhatsThis(tr("Rutas comerciales"));
    bges->menuMaestro->addAction(planCuentas);
    connect(planCuentas, SIGNAL(activated()), this, SLOT(elslot()));

}

