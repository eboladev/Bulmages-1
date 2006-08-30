
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
/*
    mypluginbf *plug= new mypluginbf( );
    plug->inicializa(bges);
*/
	/// Iniciamos las libreriasAboutView
//	Q_CLEANUP_RESOURCE(bulmages);
//	Q_INIT_RESOURCE(bulmages);

    _depura("END Punto de Entrada del plugin registroIVA", 0);
    return 0;
}


/**
  * Buscamos en el asiento si hay indicios de una factura y actuamos en consecuencia.
  */

int Asiento1_guardaAsiento1_post(Asiento1 *as) {
    _depura("Asiento1_guardaAsiento1_post",0);
    empresa *companyact = as->companyact();
    QString cuentas="";
    QString query = "SELECT valor FROM configuracion WHERE nombre='RegistroEmitida' OR nombre='RegistroSoportada'";
    cursor2 *curvalor = companyact->cargacursor(query);
    while (!curvalor->eof()) {
        cuentas += curvalor->valor("valor")+"%|"; // Preparamos una expresión regular para usar en la consulta
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
        reg->exec();
        delete reg;
        cursborr->siguienteregistro();
    }// end while
    delete cursborr;
    return 0;
}// end buscaFactura




int empresa_cobPag(empresa *emp) {
    cobropagoview *adoc= new cobropagoview(emp,0,"");
    adoc->exec();
    delete adoc;
    return 0;
}

int empresa_registroiva(empresa *emp) {
    ListRegistroIvaView *perd = new ListRegistroIvaView(emp, "0");
    perd->inicializa();
    perd->exec();
    delete perd;
    return 0;
}


int ListLinAsiento1View_boton_iva(ListLinAsiento1View *as) {
    as->guardar();
    if (as->DBvalue("idborrador") != "") {
        int idborrador = as->DBvalue("idborrador").toInt();
        RegistroIvaView *nuevae=new RegistroIvaView( (empresa *) as->companyact(), 0);
        nuevae->inicializa1(idborrador);
        nuevae->exec();
        delete nuevae;
        as->pintar();
    }// end if
    return 0;
}
