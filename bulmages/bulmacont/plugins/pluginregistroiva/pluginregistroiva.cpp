/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include <stdio.h>

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>

#include "pluginregistroiva.h"
#include "empresa.h"
#include "registroivaview.h"
#include "cobropagoview.h"
#include "asiento1.h"
#include "listlinasiento1view.h"
#include "listregistroivaview.h"
#include "registroivaview.h"
#include "regivaqtoolbutton.h"



///
/**
**/
myRegIVA::myRegIVA() {
    _depura("myRegIVA::myRegIVA", 0);
    _depura("END myRegIVA::myRegIVA", 0);
}


///
/**
**/
myRegIVA::~myRegIVA() {
    _depura("myRegIVA::~myRegIVA", 0);
    _depura("END myRegIVA::~myRegIVA", 0);
}


///
/**
**/
void myRegIVA::elslot() {
    _depura("myRegIVA::elslot", 0);
    ListRegistroIvaView *perd = new ListRegistroIvaView(m_bulmacont->empresaactual(), "0");
    perd->inicializa();
    m_bulmacont->empresaactual()->pWorkspace()->addWindow(perd);
    perd->show();
    _depura("END empresa_registroiva", 0);
    _depura("END myRegIVA::elslot", 0);
}


///
/**
**/
void myRegIVA::elslot1() {
    _depura("myRegIVA::elslot1", 0);
    cobropagoview *adoc = new cobropagoview(m_bulmacont->empresaactual(), 0);
    m_bulmacont->empresaactual()->pWorkspace()->addWindow(adoc);
    adoc->show();
    _depura("END myRegIVA::elslot1", 0);
}


///
/**
\param bges
**/
void myRegIVA::inicializa(Bulmacont *bges) {
    /// Creamos el men&uacute;.
    m_bulmacont = bges;
    QAction *accion = new QAction("&Registro de IVA", 0);
    accion->setStatusTip("Registro de IVA");
    accion->setWhatsThis("Registro de IVA");

    QAction *accion1 = new QAction("&Cobros y Pagos", 0);
    accion1->setStatusTip("Cobros y Pagos");
    accion1->setWhatsThis("Cobros y Pagos");

    connect(accion, SIGNAL(activated()), this, SLOT(elslot()));
    connect(accion1, SIGNAL(activated()), this, SLOT(elslot1()));
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuListados->insertAction(bges->actionCuentas_Anuales, accion);
    bges->menuListados->insertAction(bges->actionCuentas_Anuales, accion1);
    bges->menuListados->insertSeparator(bges->actionCuentas_Anuales);
}


///
/**
\return
**/
int entryPoint(Bulmacont *bcont) {
    _depura("Punto de entrada del plugin registroIVA", 0);
    myRegIVA *my = new myRegIVA();
    my->inicializa(bcont);
    return 0;
}


///
/**
\param l
\return
**/
int Asiento1View_Asiento1View(Asiento1View *l) {
       _depura("Asiento1View_Asiento1View", 0);
//================================
       RegIVAQToolButton *mui_exporta_efactura2 = new RegIVAQToolButton(l,  l->mui_plugbotones);

       QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *>("hboxLayout1");
       if (!m_hboxLayout1) {
                m_hboxLayout1 = new QHBoxLayout(l->mui_plugbotones);
                m_hboxLayout1->setSpacing(5);
                m_hboxLayout1->setMargin(5);
                m_hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
       } // end if
       m_hboxLayout1->addWidget(mui_exporta_efactura2);
//================================
       _depura("END Asiento1View_Asiento1View", 0);
       return 0;
}


/// Buscamos en el asiento si hay indicios de una factura y actuamos en consecuencia.
/**
\param as
\return
**/
int Asiento1_guardaAsiento1_post(Asiento1 *as) {
    _depura("Asiento1_guardaAsiento1_post", 0);
    Empresa *companyact = as->companyact();
    QString cuentas = "";
    QString query = "SELECT valor FROM configuracion WHERE nombre = 'RegistroEmitida' OR nombre = 'RegistroSoportada'";
    cursor2 *curvalor = companyact->cargacursor(query);
    while (!curvalor->eof()) {
        /// Preparamos una expresi&oacute;n regular para usar en la consulta.
        cuentas += curvalor->valor("valor") + "%|";
        curvalor->siguienteregistro();
    } // end while
    delete curvalor;
    /// Le quitamos el &uacute;ltimo '|' que nos sobra.
    cuentas.truncate(cuentas.length() - 1);

    /// Recorremos la tabla en busca de entradas de factura no introducidas y las
    /// preguntamos antes de cerrar nada.
    QString SQLQuery = "SELECT bcontrapartidaborr(idborrador) AS contra FROM borrador LEFT JOIN cuenta ON borrador.idcuenta = cuenta.idcuenta WHERE idasiento = " + as->DBvalue("idasiento") + " AND codigo SIMILAR TO '" + companyact->sanearCadena(cuentas.toAscii().constData()) + "' GROUP BY contra";

    cursor2 *cursborr= companyact->cargacursor(SQLQuery);
    while (!cursborr->eof()) {
        int idborrador = cursborr->valor("contra").toInt();
        RegistroIvaView *reg = new RegistroIvaView(companyact, 0);
        reg->inicializa1(idborrador);
        companyact->pWorkspace()->addWindow(reg);
        reg->show();
        cursborr->siguienteregistro();
    } // end while
    delete cursborr;
    _depura("END Asiento1_guardaAsiento1_post", 0);
    return 0;
}



///
/**
\param as
\return
**/
int ListLinAsiento1View_boton_iva(ListLinAsiento1View *as) {
    _depura("ListLinAsiento1View_boton_iva", 0);
    as->guardar();
    try {
        int idborrador = as->DBvalue("idborrador").toInt();
        RegistroIvaView *nuevae = new RegistroIvaView((Empresa *) as->empresaBase(), 0);
        nuevae->inicializa1(idborrador);
        ((Empresa *)as->empresaBase())->pWorkspace()->addWindow(nuevae);
        nuevae->show();
    } catch (...) {
        mensajeInfo("Debe seleccionar un apunte");
        return 0;
    } // end try
    _depura("END ListLinAsiento1View_boton_iva", 0);
    return 0;
}

