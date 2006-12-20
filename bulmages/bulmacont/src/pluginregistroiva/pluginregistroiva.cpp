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


int entryPoint(Bulmacont *) {
    _depura("Punto de entrada del plugin registroIVA", 10);
    return 0;
}


/// Buscamos en el asiento si hay indicios de una factura y actuamos en consecuencia.
int Asiento1_guardaAsiento1_post(Asiento1 *as) {
    _depura("Asiento1_guardaAsiento1_post", 0);
    empresa *companyact = as->companyact();
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


int empresa_cobPag(empresa *emp) {
    _depura("empresa_cobPag", 0);
    cobropagoview *adoc = new cobropagoview(emp, 0);
    emp->pWorkspace()->addWindow(adoc);
    adoc->show();
    _depura("END empresa_cobPag", 0);
    return 0;
}


int empresa_registroiva(empresa *emp) {
    _depura("empresa_registroiva", 0);
    ListRegistroIvaView *perd = new ListRegistroIvaView(emp, "0");
    perd->inicializa();
    emp->pWorkspace()->addWindow(perd);
    perd->show();
    _depura("END empresa_registroiva", 0);
    return 0;
}


int ListLinAsiento1View_boton_iva(ListLinAsiento1View *as) {
    _depura("ListLinAsiento1View_boton_iva", 0);
    as->guardar();
    try {
        int idborrador = as->DBvalue("idborrador").toInt();
        RegistroIvaView *nuevae = new RegistroIvaView( (empresa *) as->companyact(), 0);
        nuevae->inicializa1(idborrador);
        ((empresa *)as->companyact())->pWorkspace()->addWindow(nuevae);
        nuevae->show();
    } catch (...) {
        mensajeInfo("ListLinAsiento1View_boton_iva::Debe seleccionar un apunte");
        return 0;
    } // end try
    _depura("END ListLinAsiento1View_boton_iva", 0);
    return 0;
}

