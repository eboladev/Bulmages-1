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

#include "asiento1view.h"
#include "correctorwidget.h"
#include "confreglasview.h"


/** Inicializacion de la clase.
    En el constructor del corrector no se hacen tareas especiales. */
correctorwidget::correctorwidget(QWidget* parent, Qt::WFlags fl)
        : QWidget(parent, fl) {
    setupUi(this);
    QObject::connect(mui_browser, SIGNAL(anchorClicked(const QUrl)), this, SLOT(alink(const QUrl)));
}


correctorwidget::~correctorwidget() {
    _depura("Salida del plugin Corrector", 10);
}


/** Se ha pulsado sobre el boton de configuracion de reglas.
    Activa la ventana de correcion de reglas \ref confreglasview */
void correctorwidget::on_mui_configurar_clicked() {
    confreglasview *conf = new confreglasview();
    conf->exec();
    delete conf;
}


/** Se encarga de llevar a cabo las diferentes pruebas y generar el informe.
    Esta funcion es el disparador del corrector, que se encarga de hacer los tests a la
    empresa y sacar la ventana de resultados. */
void correctorwidget::on_mui_corregir_clicked() {
    textBrowser = "<HTML><BODY BGCOLOR='#FFFFFF'>";
    cursor2 *cur;

    /// Calculo de asientos abiertos.
    QString query;
    query.sprintf("SELECT * FROM asiento LEFT JOIN (SELECT count(idborrador) AS numborr, idasiento FROM borrador GROUP BY idasiento) AS borr ON borr.idasiento = asiento.idasiento LEFT JOIN (SELECT count(idapunte) AS numap, idasiento FROM apunte GROUP BY idasiento) AS apunt ON apunt.idasiento = asiento.idasiento WHERE apunt.numap = 0 OR numap ISNULL");
    cur = conexionbase->cargacursor(query);
    while (!cur->eof()) {
        QString cadena;
        cadena.sprintf("<img src='/usr/share/bulmages/icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El asiento num. <B>%s</B> con fecha <B>%s</B> esta abierto, esto causa que el asiento no modifique el estado de las cuentas.", cur->valor("ordenasiento").toAscii().constData(), cur->valor("fecha").toAscii().constData());
        agregarError(cadena, "asiento", cur->valor("idasiento").toAscii().constData());
        cur->siguienteregistro();
    } // end while
    delete cur;

    /// Calculo de insercion en cuentas intermedias (con hijos).
    /// --------------------------------------------------------
    query.sprintf("SELECT * FROM asiento, apunte, cuenta WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.idcuenta IN (SELECT padre FROM cuenta) AND apunte.idasiento = asiento.idasiento");
    cur = conexionbase->cargacursor(query);
    while (!cur->eof()) {
        QString cadena;
        cadena.sprintf("<img src='/usr/share/bulmages/icons/messagebox_critical.png'>&nbsp;&nbsp;<B><I>Critial Error:</I></B><BR>El asiento num. <B>%s</B> tiene un apunte con la cuenta <B>%s</B> no hija..", cur->valor("ordenasiento").toAscii().constData(), cur->valor("codigo").toAscii().constData());
        agregarError(cadena, "asiento", cur->valor("idasiento").toAscii().constData());
        cur->siguienteregistro();
    } // end while
    delete cur;

    /// Calculo de la ecuacion fundamental contable A+I = P+N+G
    /// -------------------------------------------------------
    query = " SELECT asiento.idasiento AS idasiento, asiento.ordenasiento AS ordenasiento, ingresos, activos, gastos, netos, pasivos FROM asiento ";
    query += " LEFT JOIN (SELECT idasiento, sum(apunte.debe) - sum(apunte.haber) AS ingresos FROM cuenta, apunte WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.tipocuenta = 4 GROUP BY idasiento) AS ing ON asiento.idasiento = ing.idasiento ";
    query +=" LEFT JOIN (SELECT idasiento, sum(apunte.debe) - sum(apunte.haber) AS gastos FROM cuenta, apunte WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.tipocuenta = 5 GROUP BY idasiento) AS gas ON asiento.idasiento = gas.idasiento ";
    query += " LEFT JOIN (SELECT idasiento, sum(apunte.debe) - sum(apunte.haber) AS activos FROM cuenta, apunte WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.tipocuenta = 1 GROUP BY idasiento) AS act ON act.idasiento = asiento.idasiento ";
    query += " LEFT JOIN (SELECT idasiento, sum(apunte.debe) - sum(apunte.haber) AS pasivos FROM cuenta, apunte WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.tipocuenta = 2 GROUP BY idasiento) AS pas ON pas.idasiento = asiento.idasiento ";
    query += " LEFT JOIN (SELECT idasiento, sum(apunte.debe) - sum(apunte.haber) AS netos FROM cuenta, apunte WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.tipocuenta = 3 GROUP BY idasiento) AS net ON net.idasiento = asiento.idasiento ";
    query += " ORDER BY ordenasiento";
    cur = conexionbase->cargacursor(query, "hol");
    while (!cur->eof()) {
        float ing, gas, act, pas, net;
        ing = cur->valor("ingresos").toFloat();
        gas = cur->valor("gastos").toFloat();
        act = cur->valor("activos").toFloat();
        pas = cur->valor("pasivos").toFloat();
        net = cur->valor("netos").toFloat();
        if ((-act - gas - pas - net + ing) > 0.01) {
            QString cadena;
            cadena.sprintf("<img src='/usr/share/bulmages/icons/messagebox_critical.png'>&nbsp;&nbsp;<B><I>Error critico:</I></B><BR>El asiento num. <B>%s</B> no cumple la ecuacion fundamental.%2.2f + %2.2f = %2.2f + %2.2f + %2.2f", cur->valor("ordenasiento").toAscii().constData(), act, gas, pas, net, ing);
            agregarError(cadena, "asiento", cur->valor("idasiento").toAscii().constData());
        } // end if
        cur->siguienteregistro();
    } // end while
    delete cur;

    /// Calculo de cuentas con insercion en el debe que lo tienen bloqueado.
    /// --------------------------------------------------------------------
    query.sprintf("SELECT * FROM asiento, apunte, cuenta WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.nodebe AND apunte.idasiento = asiento.idasiento AND apunte.debe <> 0");
    _depura(query, 10);
    cur = conexionbase->cargacursor(query, "hola1");
    while (!cur->eof()) {
        QString cadena;
        cadena.sprintf("<img src='/usr/share/bulmages/icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El asiento num. <B>%s</B> tiene una inserci� en el debe de la cuenta <B>%s</B> que no permite inserciones en el debe de dicha cuenta.", cur->valor("ordenasiento").toAscii().constData(), cur->valor("codigo").toAscii().constData());
        agregarError(cadena, "asiento", cur->valor("idasiento").toAscii().constData());
        cur->siguienteregistro();
    } // end while
    delete cur;

    /// Calculo de cuentas con insercion en el haber que lo tienen bloqueado.
    /// ---------------------------------------------------------------------
    query.sprintf("SELECT * FROM asiento, apunte, cuenta WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.nohaber AND apunte.idasiento = asiento.idasiento AND apunte.haber <> 0");
    cur = conexionbase->cargacursor(query);
    while (!cur->eof()) {
        QString cadena;
        cadena.sprintf("<img src='/usr/share/bulmages/icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El asiento num. <B>%s</B> tiene una insercion en el haber de la cuenta <B>%s</B> que no permite inserciones en el haber de dicha cuenta.", cur->valor("ordenasiento").toAscii().constData(), cur->valor("codigo").toAscii().constData());
        agregarError(cadena, "asiento", cur->valor("idasiento").toAscii().constData());
        cur->siguienteregistro();
    } // end while
    delete cur;

    /// Calculo de amortizaciones con plazos expirados.
    /// -----------------------------------------------
    query = "SELECT * FROM linamortizacion WHERE fechaprevista < now() AND idasiento IS NULL";
    cur = conexionbase->cargacursor(query);
    while (!cur->eof()) {
        QString cadena;
        cadena.sprintf("<img src='/usr/share/bulmages/icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>La amortizacion num. <B>%s</B> tiene un plazo expirado <B>%s</B>.", cur->valor("idamortizacion").toAscii().constData(), cur->valor("fechaprevista").toAscii().constData());
        agregarError(cadena, "amortizacion", cur->valor("idamortizacion").toAscii().constData());
        cur->siguienteregistro();
    } // end while
    delete cur;

    /// Calculo de asientos con IVA y sin facturas asociadas.
    /// -----------------------------------------------------
    query = "SELECT * FROM borrador, cuenta WHERE cuenta.idcuenta = borrador.idcuenta AND codigo LIKE '%47%' AND idasiento NOT IN (SELECT idasiento FROM registroiva)";
    cur = conexionbase->cargacursor(query);
    while (!cur->eof()) {
        QString cadena;
        cadena.sprintf("<img src='/usr/share/bulmages/icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El asiento num. <B>%s</B> tiene una insercion en cuentas de IVA (%s) sin que haya una factura asociada.", cur->valor("orden").toAscii().constData(), cur->valor("codigo").toAscii().constData());
        agregarError(cadena, "asiento", cur->valor("idasiento").toAscii().constData());
        cur->siguienteregistro();
    } // end while
    delete cur;

    textBrowser += "</BODY></HTML>";
    mui_browser->setHtml(textBrowser);
}


/** Detecta la pulsacion de un enlace en el widget de resultados.
    Se activa esta funcion cuando se ha pulsado un link en el Widget de resultados
    QTextBrowser.
    De esta forma la aplicacion puede interactuar con la pagina web que se muestra. */
void correctorwidget::alink(const QUrl &url) {
    QString linker = url.fragment();
   _depura(linker, 10);

    ///TODO: REVISAR ESTA FUNCION QUE NO HACE BIEN SU TRABAJO.
    if (linker == "ver") {
        empresaactual->muestracuentas();
    } else if (linker == "asiento") {
        //QString ordenasiento = l.right(l.length() - 2);
        Asiento1View *view = empresaactual->intapuntsempresa();
        bool ok;
        //view->muestraasiento(ordenasiento.toInt(&ok));
    } else {
        Asiento1View *view = empresaactual->intapuntsempresa();
        bool ok;
        view->muestraasiento(linker.toInt(&ok));
        //QMessageBox::warning(0, tr("Opcion no implementada"), tr("No se puede acceder al error"), 0, 1, 2);
    } // endif
}


/** Se encarga de agregar un error en el browser que los presenta.
    Sirve para para introducir los errores hayados en el corrector y que se vean por
    pantalla. */
void correctorwidget::agregarError(QString texto, QString texto1, QString texto2) {
    textBrowser += "<HR><table><tr><td colspan=2>" + texto + "</td></tr><tr><td><a name='masinfo' href='#" + texto1 + "'>+ info</a></td><td><a name='" + texto1 + "' href='#" + texto2 + "'>ver error</a></td></tr></table>";
}

