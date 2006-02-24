//
//
// C++ Implementation: $MODULE$
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "correctorwidget.h"
#include "confreglasview.h"

#include <q3table.h>
#include <q3header.h>
#include <q3textbrowser.h>
#include "asiento1view.h"

/** \brief Inicializaci� de la clase,
  *
  * En el constructor del corrector no se hacen tareas especiales.
  */
correctorwidget::correctorwidget( QWidget* parent, const char* name, Qt::WFlags fl ):correctorwdt(parent, name, fl) {
    setCaption("Corrector");
}// end correctorwidget;


correctorwidget::~correctorwidget() {}


/** \brief Detecta la pulsaci� de un enlace en el widget de resultados
  * 
  * Se activa esta funci� cuando se ha pulsado un link en el Widget de resultados.
  * De esta forma la aplicaci� puede interactuar con la p�ina web que se muestra
  */
void correctorwidget::link(const QString &linker) {
    fprintf(stderr,"Link Pulsado: %s\n", linker.ascii());
}// end link


/** \brief Detecta la pulsaci� de un enlace en el widget de resultados
  * 
  * Se activa esta funci� cuando se ha pulsado un link en el Widget de resultados QTextBrowser.
  * De esta forma la aplicaci� puede interactuar con la p�ina web que se muestra
  */
void correctorwidget::alink(const QString &linker, const QString &l) {
    fprintf(stderr,"Anchor pulsado: %s, %s\n", linker.ascii(), l.ascii());
    if (linker == "ver") {
        empresaactual->muestracuentas();
    } else if (linker== "asiento") {
        QString ordenasiento = l.right(l.length()-2);
        Asiento1View *view = empresaactual->intapuntsempresa();
        bool ok;
        view->muestraasiento(ordenasiento.toInt(&ok));
    } else {
        QMessageBox::warning(0,tr("Opcion� no implementada"),tr("No se puede acceder al error"),0,1,2);
    }// endif
}// end link


/** \brief Se encarga de agregar un error en el browser que los presenta
  *
  * Sirve para para introducir los errores hayados en el corrector y que se vean por
  * pantalla.
  */
void correctorwidget::agregarError(QString texto, QString texto1, QString texto2) {
    textBrowser += "<HR><table><tr><td colspan=2>"+texto+"</td></tr><tr><td><a name='masinfo' href='#?"+texto1+"'>+ info</a></td><td><a name='"+texto1+"' href='#?"+texto2+"'>ver error</a></td></tr></table>";
}// end agregarError


/** \brief SE encarga de llevar a cabo las diferentes pruebas y generar el informe. 
  *Esta funci� es el disparador del corrector, que se encarga de hacer los tests a la empresa
  * Y sacar la ventana de resultados.
  */
void correctorwidget::corregir() {
    textBrowser = "<HTML><BODY BGCOLOR='#FFFFFF'>";
    cursor2 *cur;

    /// Calculo de asientos abiertos
    QString query;
    query.sprintf("SELECT * FROM asiento LEFT JOIN (SELECT count(idborrador) AS numborr, idasiento FROM borrador GROUP BY idasiento) AS borr ON borr.idasiento=asiento.idasiento LEFT JOIN (SELECT count(idapunte) AS numap, idasiento  FROM apunte GROUP BY idasiento) AS apunt ON apunt.idasiento=asiento.idasiento WHERE apunt.numap =0 OR numap ISNULL");
    cur = conexionbase->cargacursor(query);
    while (!cur->eof()) {
        QString cadena;
        cadena.sprintf("<font face='Arial' size=2><img src='/usr/share/bulmages/icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B></FONT><font face='Arial' size=1><BR>El asiento num. <B>%s</B> est�abierto, esto causa que el asiento no modifique el estado de las cuentas.</FONT>", cur->valor("ordenasiento").ascii());
        agregarError(cadena, "asiento",cur->valor("idasiento").ascii());
        cur->siguienteregistro();
    }// end while
    delete(cur);

    /// C�culo de inserci� en cuentas intermedias (con hijos)
    //--------------------------------------------------------
    query.sprintf("SELECT * FROM asiento, apunte, cuenta WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.idcuenta IN (SELECT padre FROM cuenta) AND apunte.idasiento=asiento.idasiento");
    cur = conexionbase->cargacursor(query);
    while (!cur->eof()) {
        QString cadena;
        cadena.sprintf("<font face='Arial' size=2><img src='/usr/share/bulmages/icons/messagebox_critical.png'>&nbsp;&nbsp;<B><I>Critial Error:</I></B></FONT><font face='Arial' size=1><BR>El asiento num. <B>%s</B> tiene un apunte con la cuenta <B>%s</B> no hija..</FONT>", cur->valor("ordenasiento").ascii(), cur->valor("codigo").ascii());
        agregarError(cadena, "asiento",cur->valor("idasiento").ascii());
        cur->siguienteregistro();
    }// end while
    delete(cur);

    /// C�culo de la ecuaci� fundamental contable A+I= P+N+G
    //--------------------------------------------------------
    query = " SELECT asiento.idasiento AS idasiento, asiento.ordenasiento AS ordenasiento, ingresos,activos, gastos, netos, pasivos FROM asiento ";
    query += " LEFT JOIN (SELECT idasiento, sum(apunte.debe)-sum(apunte.haber) AS ingresos  FROM cuenta,apunte WHERE  apunte.idcuenta=cuenta.idcuenta AND cuenta.tipocuenta=4 GROUP BY idasiento) AS ing ON asiento.idasiento = ing.idasiento ";
    query +=" LEFT JOIN (SELECT idasiento, sum(apunte.debe)-sum(apunte.haber) AS gastos  FROM cuenta,apunte WHERE  apunte.idcuenta=cuenta.idcuenta AND cuenta.tipocuenta=5 GROUP BY idasiento) AS gas ON asiento.idasiento= gas.idasiento ";
    query += " LEFT JOIN (SELECT idasiento, sum(apunte.debe)-sum(apunte.haber) AS activos  FROM cuenta,apunte WHERE  apunte.idcuenta=cuenta.idcuenta AND cuenta.tipocuenta=1 GROUP BY idasiento) AS act ON act.idasiento=asiento.idasiento ";
    query += " LEFT JOIN (SELECT idasiento, sum(apunte.debe)-sum(apunte.haber) AS pasivos  FROM cuenta,apunte WHERE  apunte.idcuenta=cuenta.idcuenta AND cuenta.tipocuenta=2 GROUP BY idasiento) AS pas ON pas.idasiento=asiento.idasiento ";
    query += " LEFT JOIN (SELECT idasiento, sum(apunte.debe)-sum(apunte.haber) AS netos  FROM cuenta,apunte WHERE  apunte.idcuenta=cuenta.idcuenta AND cuenta.tipocuenta=3 GROUP BY idasiento) AS net ON net.idasiento=asiento.idasiento ";
    query += " ORDER BY ordenasiento";
    cur = conexionbase->cargacursor(query,"hol");
    while (!cur->eof()) {
        float ing,gas,act,pas,net;
        ing = cur->valor("ingresos").toFloat();
        gas = cur->valor("gastos").toFloat();
        act = cur->valor("activos").toFloat();
        pas = cur->valor("pasivos").toFloat();
        net = cur->valor("netos").toFloat();
        if (-act-gas-pas-net+ing>0.01) {
            QString cadena;
            cadena.sprintf("<font face='Arial' size=2><img src='/usr/share/bulmages/icons/messagebox_critical.png'>&nbsp;&nbsp;<B><I>Critial Error:</I></B></FONT><font face='Arial' size=1><BR>El asiento num. <B>%s</B> no cumple la ecuaci� fundamental.%2.2f+%2.2f=%2.2f+%2.2f+%2.2f</FONT>", cur->valor("ordenasiento").ascii(), act,gas,pas,net,ing);
            agregarError(cadena, "asiento",cur->valor("idasiento").ascii());
        }// end if
        cur->siguienteregistro();
    }// end while
    delete(cur);

    /// Calculo de cuentas con insercion en el debe que lo tienen bloqueado
    //-----------------------------------------------------------------------
    query.sprintf("SELECT * FROM asiento, apunte, cuenta WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.nodebe AND apunte.idasiento=asiento.idasiento AND apunte.debe <> 0");
    fprintf(stderr,"%s\n",query.ascii());
    cur = conexionbase->cargacursor(query,"hola1");
    while (!cur->eof()) {
        QString cadena;
        cadena.sprintf("<font face='Arial' size=2><img src='/usr/share/bulmages/icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B></FONT><font face='Arial' size=1><BR>El asiento num. <B>%s</B> tiene una inserci� en el debe de la cuenta <B>%s</B> que no permite inserciones en el debe de dicha cuenta.</FONT>", cur->valor("ordenasiento").ascii(), cur->valor("codigo").ascii());
        agregarError(cadena, "asiento",cur->valor("idasiento").ascii());
        cur->siguienteregistro();
    }// end while
    delete(cur);


    /// Calculo de cuentas con insercion en el haber que lo tienen bloqueado
    //-----------------------------------------------------------------------
    query.sprintf("SELECT * FROM asiento, apunte, cuenta WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.nohaber AND apunte.idasiento=asiento.idasiento AND apunte.haber <> 0");
    cur = conexionbase->cargacursor(query);
    while (!cur->eof()) {
        QString cadena;
        cadena.sprintf("<font face='Arial' size=2><img src='/usr/share/bulmages/icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B></FONT><font face='Arial' size=1><BR>El asiento num. <B>%s</B> tiene una inserci� en el haber de la cuenta <B>%s</B> que no permite inserciones en el haber de dicha cuenta.</FONT>", cur->valor("ordenasiento").ascii(), cur->valor("codigo").ascii());
        agregarError(cadena, "asiento",cur->valor("idasiento").ascii());
        cur->siguienteregistro();
    }// end while
    delete(cur);

    /// Calculo de amortizaciones con plazos expirados
    // ----------------------------------------------
    query = "SELECT * FROM linamortizacion WHERE fechaprevista < now() AND idasiento IS NULL";
    cur = conexionbase->cargacursor(query);
    while (!cur->eof()) {
        QString cadena;
        cadena.sprintf("<font face='Arial' size=2><img src='/usr/share/bulmages/icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B></FONT><font face='Arial' size=1><BR>La amortizacion num. <B>%s</B> tiene un plazo expirado <B>%s</B>.</FONT>", cur->valor("idamortizacion").ascii(), cur->valor("fechaprevista").ascii());
        agregarError(cadena, "amortizacion",cur->valor("idamortizacion").ascii());
        cur->siguienteregistro();
    }// end while�    delete (cur);


    /// Calculo de asientos con IVA y sin facturas asociadas
    // ----------------------------------------------------
    query = "SELECT * FROM borrador, cuenta WHERE cuenta.idcuenta = borrador.idcuenta AND codigo LIKE '%47%' AND idasiento NOT IN (SELECT idasiento FROM registroiva)";
    cur = conexionbase->cargacursor(query);
    while (!cur->eof()) {
        QString cadena;
        cadena.sprintf("<font face='Arial' size=2><img src='/usr/share/bulmages/icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B></FONT><font face='Arial' size=1><BR>El asiento num. <B>%s</B> tiene una inserci� en cuentas de IVA (%s) sin que haya una factura asociada.</FONT>", cur->valor("ordenasiento").ascii(), cur->valor("codigo").ascii());
        agregarError(cadena, "asiento",cur->valor("idasiento").ascii());
        cur->siguienteregistro();
    }// end while
    delete(cur);

    textBrowser += "</BODY></HTML>";
    browser->setText(textBrowser);
}// end corregir


/** \brief Se ha pulsado sobre el bot� de configuraci� de reglas
  * 
  * Activa la ventana de correci� de reglas \ref confreglasview
  */
void correctorwidget::s_configurarReglas() {
    confreglasview *conf = new confreglasview(0,0);
    conf->exec();
    delete conf;
}

