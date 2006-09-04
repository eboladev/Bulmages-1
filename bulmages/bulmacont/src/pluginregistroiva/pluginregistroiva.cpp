
#include "pluginregistroiva.h"
#include "empresa.h"
#include "registroivaview.h"
#include "cobropagoview.h"
#include "asiento1.h"
#include "listlinasiento1view.h"
#include "listregistroivaview.h"
#include "registroivaview.h"

#include <stdio.h>
#include <q3popupmenu.h>
#include <qaction.h>
#include <qobject.h>
#include <qmessagebox.h>
#include <q3dockwindow.h>
#include <q3mainwindow.h>

#include <qstringlist.h>
#include <qwidget.h>
#include <qicon.h>
#include <qapplication.h>
#include <qobject.h>
#include <q3dockwindow.h>

int entryPoint(Bulmages01 *bges) {
    _depura("Punto de Entrada del plugin registroIVA", 0);
    _depura("END Punto de Entrada del plugin registroIVA", 0);
    return 0;
}


/**
  * Buscamos en el asiento si hay indicios de una factura y actuamos en consecuencia.
  */

int Asiento1_guardaAsiento1_post(Asiento1 *as) {
    _depura("Asiento1_guardaAsiento1_post", 2);
    empresa *companyact = as->companyact();
    QString cuentas="";
    QString query = "SELECT valor FROM configuracion WHERE nombre='RegistroEmitida' OR nombre='RegistroSoportada'";
    cursor2 *curvalor = companyact->cargacursor(query);
    while (!curvalor->eof()) {
        /// Preparamos una expresión regular para usar en la consulta
        cuentas += curvalor->valor("valor")+"%|"; 
        curvalor->siguienteregistro();
    }// end while
    delete curvalor;
    cuentas.truncate(cuentas.length()-1); // Le quitamos el último '|' que nos sobra

    /// Recorremos la tabla en busca de entradas de factura no introducidas y las preguntamos antes de cerrar nada.
    QString SQLQuery = "SELECT bcontrapartidaborr(idborrador) AS contra FROM borrador LEFT JOIN cuenta ON borrador.idcuenta=cuenta.idcuenta WHERE idasiento="+as->DBvalue("idasiento")+" AND codigo SIMILAR TO '"+companyact->sanearCadena(cuentas.ascii())+"' GROUP BY contra";

    cursor2 *cursborr= companyact->cargacursor(SQLQuery);
    while (!cursborr->eof()) {
        int idborrador = cursborr->valor("contra").toInt();
        RegistroIvaView * reg = new RegistroIvaView(companyact,0);
        reg->inicializa1( idborrador);
        companyact->pWorkspace()->addWindow(reg);
	reg->show();
        cursborr->siguienteregistro();
    }// end while
    delete cursborr;
    _depura("END Asiento1_guardaAsiento1_post", 2);
    return 0;
}



int empresa_cobPag(empresa *emp) {
    _depura("empresa_cobPag", 0);
    cobropagoview *adoc= new cobropagoview(emp,0);
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
        RegistroIvaView *nuevae=new RegistroIvaView( (empresa *) as->companyact(), 0);
        nuevae->inicializa1(idborrador);
        ((empresa *) as->companyact())->pWorkspace()->addWindow(nuevae);
	nuevae->show();
    } catch(...) {
       mensajeInfo("ListLinAsiento1View_boton_iva::Debe seleccionar un apunte");
       return 0;
    } // end try
    _depura("END ListLinAsiento1View_boton_iva", 0);
    return 0;
}
