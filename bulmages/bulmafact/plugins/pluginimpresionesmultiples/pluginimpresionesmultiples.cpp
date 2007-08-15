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

#include "pluginimpresionesmultiples.h"
#include "funcaux.h"
#include "presupuestolist.h"
#include "impqtoolbutton.h"


int entryPoint(Bulmafact *) {
    _depura("Estoy dentro del plugin de impresiones multiples", 0);
    return 0;
}

int PedidosClienteListSubform_PedidosClienteListSubform(PedidosClienteListSubform *l) {
    l->addSHeader("selector", DBCampo::DBboolean, DBCampo::DBNoSave | DBCampo::DBNoLoad, SHeader::DBNone |SHeader::DBNoWrite, "selector");
    return 0;
}

int AlbaranClienteListSubform_AlbaranClienteListSubform(AlbaranClienteListSubform *l) {
    if (!l->existsHeader( "selector")) {
        l->addSHeader("selector", DBCampo::DBboolean, DBCampo::DBNoSave | DBCampo::DBNoLoad, SHeader::DBNone |SHeader::DBNoWrite, "selector");
    } // end if
    return 0;
}

int FacturasListSubform_FacturasListSubform(FacturasListSubform *l) {
    l->addSHeader("selector", DBCampo::DBboolean, DBCampo::DBNoSave | DBCampo::DBNoLoad, SHeader::DBNone | SHeader::DBNoWrite, "selector");
    return 0;
}

int PresupuestoListSubForm_PresupuestoListSubForm(PresupuestoListSubForm *l) {
    l->addSHeader("selector", DBCampo::DBboolean, DBCampo::DBNoSave | DBCampo::DBNoLoad, SHeader::DBNone | SHeader::DBNoWrite, "selector");
    return 0;
}

int CobrosListSubForm_CobrosListSubForm(CobrosListSubForm *l) {
    l->addSHeader("selector", DBCampo::DBboolean, DBCampo::DBNoSave | DBCampo::DBNoLoad, SHeader::DBNone | SHeader::DBNoWrite, "selector");
    return 0;
}


int CobrosList_CobrosList(CobrosList *l) {
    _depura("CobrosList_CobrosList", 0);

    SelQToolButton *sel = new SelQToolButton(NULL, NULL, NULL, NULL, l, l->mui_plugbotones);
    ImpQToolButton *imp = new ImpQToolButton(NULL, NULL, NULL, NULL, l, l->mui_plugbotones);
//    EmailQToolButton *email = new EmailQToolButton(NULL, NULL, NULL, NULL, l, l->mui_plugbotones);

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *>("hboxLayout1");
    if (!m_hboxLayout1) {
        m_hboxLayout1 = new QHBoxLayout(l->mui_plugbotones);
        m_hboxLayout1->setSpacing(5);
        m_hboxLayout1->setMargin(5);
        m_hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    } // end if
    m_hboxLayout1->addWidget(sel);
    m_hboxLayout1->addWidget(imp);
//    m_hboxLayout1->addWidget(email);

    _depura("END CobrosList_CobrosList", 0);
    return 0;
}

int PresupuestoList_PresupuestoList(PresupuestoList *l) {
    _depura("PresupuestoList_PresupuestoList", 0);

    SelQToolButton *sel = new SelQToolButton(l, NULL, NULL, NULL,NULL, l->mui_plugbotones);
    ImpQToolButton *imp = new ImpQToolButton(l, NULL, NULL, NULL,NULL, l->mui_plugbotones);
    EmailQToolButton *email = new EmailQToolButton(l, NULL, NULL, NULL,NULL, l->mui_plugbotones);

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *>("hboxLayout1");
    if (!m_hboxLayout1) {
        m_hboxLayout1 = new QHBoxLayout(l->mui_plugbotones);
        m_hboxLayout1->setSpacing(5);
        m_hboxLayout1->setMargin(5);
        m_hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    } // end if
    m_hboxLayout1->addWidget(sel);
    m_hboxLayout1->addWidget(imp);
    m_hboxLayout1->addWidget(email);

    _depura("END PresupuestoList_PresupuestoList", 0);
    return 0;
}

int PedidosClienteList_PedidosClienteList(PedidosClienteList *l) {
    _depura("PedidosClienteList_PedidosClienteList", 0);

    SelQToolButton *sel = new SelQToolButton(NULL, l, NULL, NULL,NULL, l->mui_plugbotones);
    ImpQToolButton *imp = new ImpQToolButton(NULL, l, NULL, NULL,NULL, l->mui_plugbotones);
    EmailQToolButton *email = new EmailQToolButton(NULL, l, NULL, NULL,NULL, l->mui_plugbotones);

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *>("hboxLayout1");
    if (!m_hboxLayout1) {
        m_hboxLayout1 = new QHBoxLayout(l->mui_plugbotones);
        m_hboxLayout1->setSpacing(5);
        m_hboxLayout1->setMargin(5);
        m_hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    } // end if

    m_hboxLayout1->addWidget(sel);
    m_hboxLayout1->addWidget(imp);
    m_hboxLayout1->addWidget(email);

    _depura("END PedidosClienteList_PedidosClienteList", 0);
    return 0;
}

int AlbaranClienteList_AlbaranClienteList(AlbaranClienteList *l) {
    _depura("PedidosClienteList_PedidosClienteList", 0);

    SelQToolButton *sel = new SelQToolButton(NULL,  NULL, l, NULL,NULL, l->mui_plugbotones);
    ImpQToolButton *imp = new ImpQToolButton(NULL,  NULL, l, NULL,NULL, l->mui_plugbotones);
    EmailQToolButton *email = new EmailQToolButton(NULL,  NULL, l, NULL,NULL, l->mui_plugbotones);

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *>("hboxLayout1");
    if (!m_hboxLayout1) {
        m_hboxLayout1 = new QHBoxLayout(l->mui_plugbotones);
        m_hboxLayout1->setSpacing(5);
        m_hboxLayout1->setMargin(5);
        m_hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    } // end if
    m_hboxLayout1->addWidget(sel);
    m_hboxLayout1->addWidget(imp);
    m_hboxLayout1->addWidget(email);

    _depura("END PedidosClienteList_PedidosClienteList", 0);
    return 0;
}


int FacturasList_FacturasList(FacturasList *l) {
    _depura("FacturasList_FacturasList", 0);

    SelQToolButton *sel = new SelQToolButton(NULL, NULL, NULL, l,NULL, l->mui_plugbotones);
    ImpQToolButton *imp = new ImpQToolButton(NULL, NULL, NULL, l,NULL, l->mui_plugbotones);
    EmailQToolButton *email = new EmailQToolButton(NULL, NULL, NULL, l,NULL, l->mui_plugbotones);

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *>("hboxLayout1");
    if (!m_hboxLayout1) {
        m_hboxLayout1 = new QHBoxLayout(l->mui_plugbotones);
        m_hboxLayout1->setSpacing(5);
        m_hboxLayout1->setMargin(5);
        m_hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    } // end if
    m_hboxLayout1->addWidget(sel);
    m_hboxLayout1->addWidget(imp);
    m_hboxLayout1->addWidget(email);

    _depura("END FacturasList_FacturasList", 0);
    return 0;
}
