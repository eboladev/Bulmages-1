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

#include "correctorwidget.h"
#include "confreglasview.h"
#include "clienteview.h"
#include "provedit.h"
#include "facturaview.h"


/** Inicializacion de la clase.
    En el constructor del corrector no se hacen tareas especiales. */
/**
\param parent
\param fl
**/
correctorwidget::correctorwidget ( QWidget* parent, Qt::WindowFlags fl )
        : BlWidget ( parent, fl )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    QObject::connect ( mui_browser, SIGNAL ( anchorClicked ( const QUrl ) ), this, SLOT ( alink ( const QUrl ) ) );
    
}


///
/**
**/
correctorwidget::~correctorwidget()
{
    BL_FUNC_DEBUG
    
}


/** Se ha pulsado sobre el boton de configuracion de reglas.
    Activa la ventana de correcion de reglas \ref confreglasview */
/**
**/
void correctorwidget::on_mui_configurar_clicked()
{
    BL_FUNC_DEBUG
    confreglasview *conf = new confreglasview();
    conf->exec();
    delete conf;
    
}


/** Se encarga de llevar a cabo las diferentes pruebas y generar el informe.
    Esta funcion es el disparador del corrector, que se encarga de hacer los tests a la
    empresa y sacar la ventana de resultados. */
/**
**/
void correctorwidget::on_mui_corregir_clicked()
{
    BL_FUNC_DEBUG
    textBrowser = "<HTML><BODY BGCOLOR='#FFFFFF'>";
    QString cadena;

    QString query = "SELECT * from factura WHERE reffactura NOT IN (SELECT refalbaran FROM albaran)";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
    while ( ! cur->eof() ) {
        cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>La factura num. <B>" + cur->value( "numfactura" ) + "</B> No esta avalada por ningun albaran, esto puede ser causa de descontrol en el stock.";
        agregarError ( cadena, "factura", cur->value( "idfactura" ) );
        cur->nextRecord();
    } // end while
    delete cur;

    query = "SELECT * from cliente WHERE length(cifcliente) < 6";
    cur = mainCompany() ->loadQuery ( query );
    while ( ! cur->eof() ) {
        cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El cliente <B>" + cur->value( "nomcliente" ) + "</B> no tiene CIF.";
        agregarError ( cadena, "cliente",  cur->value( "idcliente" ) );
        cur->nextRecord();
    } // end while
    delete cur;

    query = "SELECT * from proveedor WHERE length(cifproveedor) < 6";
    cur = mainCompany() ->loadQuery ( query );
    while ( ! cur->eof() ) {
        cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El proveedor <B>" + cur->value( "nomproveedor" ) + "</B> no tiene CIF.";
        agregarError ( cadena, "proveedor",  cur->value( "idproveedor" ) );
        cur->nextRecord();
    } // end while
    delete cur;


    query = "SELECT * FROM cliente";
    cur = mainCompany()->loadQuery ( query );
    while ( ! cur->eof() ) {
        QChar digito;
        if ( ! blValidateSpainCIFNIFCode ( cur->value( "cifcliente" ), digito ) ) {
            cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El cliente ," + cur->value( "cifcliente" ) + " <B>" + cur->value( "nomcliente" ) + "</B> tiene CIF invalido. Digito de Control:" + QString ( digito );
            agregarError ( cadena, "cliente", cur->value( "idcliente" ) );
        } // end if
        cur->nextRecord();
    } // end while
    delete cur;

    query = "SELECT * FROM proveedor";
    cur = mainCompany()->loadQuery ( query );
    while ( ! cur->eof() ) {
        QChar digito;
        if ( ! blValidateSpainCIFNIFCode ( cur->value( "cifproveedor" ), digito ) ) {
            cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El proveedor <B>" + cur->value( "nomproveedor" ) + "," + cur->value( "cifproveedor" ) + "</B> tiene CIF invalido. Digito de Control: " + QString ( digito );
            agregarError ( cadena, "proveedor",  cur->value( "idproveedor" ) );
        } // end if
        cur->nextRecord();
    } // end while
    delete cur;


    
    /// Calculo de asientos abiertos.
    query.sprintf ( "SELECT *, asiento.idasiento AS idas FROM asiento LEFT JOIN (SELECT count(idborrador) AS numborr, idasiento FROM borrador GROUP BY idasiento) AS borr ON borr.idasiento = asiento.idasiento LEFT JOIN (SELECT count(idapunte) AS numap, idasiento FROM apunte GROUP BY idasiento) AS apunt ON apunt.idasiento = asiento.idasiento WHERE apunt.numap = 0 OR numap ISNULL" );
    cur = mainCompany()->loadQuery ( query );
    while ( !cur->eof() ) {
        QString cadena;
        cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El asiento num. <B>" + cur->value( "ordenasiento" ) + "</B> con fecha <B>" + cur->value( "fecha" ) + "</B> esta abierto, esto causa que el asiento no modifique el estado de las cuentas.";
        agregarError ( cadena, "asiento",  cur->value( "idas" ) );
        cur->nextRecord();
    } // end while
    delete cur;

    /// Calculo de insercion en cuentas intermedias (con hijos).
    /// --------------------------------------------------------
    query.sprintf ( "SELECT * FROM asiento, apunte, cuenta WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.idcuenta IN (SELECT padre FROM cuenta) AND apunte.idasiento = asiento.idasiento" );
    cur = mainCompany()->loadQuery ( query );
    while ( !cur->eof() ) {
        QString cadena;
        cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_critical.png'>&nbsp;&nbsp;<B><I>Critial Error:</I></B><BR>El asiento num. <B>" + cur->value( "ordenasiento" ) + "</B> tiene un apunte con la cuenta <B>" + cur->value( "codigo" ) + "</B> no hija..";
        agregarError ( cadena, "asiento",  cur->value( "idasiento" ) );
        cur->nextRecord();
    } // end while
    delete cur;

    /// Calculo de la ecuacion fundamental contable A+G = P+N+I
    /// -------------------------------------------------------
    query = " SELECT asiento.idasiento AS idasiento, asiento.ordenasiento AS ordenasiento, ingresos, activos, gastos, netos, pasivos FROM asiento ";
    query += " LEFT JOIN (SELECT idasiento, COALESCE(sum(apunte.debe),0) - COALESCE(sum(apunte.haber),0) AS ingresos FROM cuenta, apunte WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.tipocuenta = 4 GROUP BY idasiento) AS ing ON asiento.idasiento = ing.idasiento ";
    query += " LEFT JOIN (SELECT idasiento, COALESCE(sum(apunte.debe),0) - COALESCE(sum(apunte.haber),0) AS gastos FROM cuenta, apunte WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.tipocuenta = 5 GROUP BY idasiento) AS gas ON asiento.idasiento = gas.idasiento ";
    query += " LEFT JOIN (SELECT idasiento, COALESCE(sum(apunte.debe),0) - COALESCE(sum(apunte.haber),0) AS activos FROM cuenta, apunte WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.tipocuenta = 1 GROUP BY idasiento) AS act ON act.idasiento = asiento.idasiento ";
    query += " LEFT JOIN (SELECT idasiento, COALESCE(sum(apunte.debe),0) - COALESCE(sum(apunte.haber),0) AS pasivos FROM cuenta, apunte WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.tipocuenta = 2 GROUP BY idasiento) AS pas ON pas.idasiento = asiento.idasiento ";
    query += " LEFT JOIN (SELECT idasiento, COALESCE(sum(apunte.debe),0) - COALESCE(sum(apunte.haber),0) AS netos FROM cuenta, apunte WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.tipocuenta = 3 GROUP BY idasiento) AS net ON net.idasiento = asiento.idasiento ";
    query += " ORDER BY ordenasiento";
    cur = mainCompany()->loadQuery ( query );
    while ( !cur->eof() ) {
        float ing, gas, act, pas, net;
        ing = cur->value( "ingresos" ).toFloat();
        gas = cur->value( "gastos" ).toFloat();
        act = cur->value( "activos" ).toFloat();
        pas = cur->value( "pasivos" ).toFloat();
        net = cur->value( "netos" ).toFloat();
        if ( abs( act + gas + pas + net + ing ) > 0.01 ) {
            QString cadena;
            cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_critical.png'>&nbsp;&nbsp;<B><I>Error critico:</I></B><BR>El asiento num. <B>" + cur->value( "ordenasiento" ) + "</B> no cumple la ecuacion fundamental." + QString::number ( act ) + " + " + QString::number ( gas ) + " = " + QString::number ( pas ) + " + " + QString::number ( net ) + " + " + QString::number ( ing );
            agregarError ( cadena, "asiento",  cur->value( "idasiento" ) );
        } // end if
        cur->nextRecord();
    } // end while
    delete cur;

    /// Calculo de cuentas con insercion en el debe que lo tienen bloqueado.
    /// --------------------------------------------------------------------
    query.sprintf ( "SELECT * FROM asiento, apunte, cuenta WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.nodebe AND apunte.idasiento = asiento.idasiento AND apunte.debe <> 0" );
    BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("Consulta: '%1'")).arg(query) );
    cur = mainCompany()->loadQuery ( query );
    while ( !cur->eof() ) {
        QString cadena;
        cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El asiento num. <B>" + cur->value( "ordenasiento" ) + "</B> tiene una insercion en el debe de la cuenta <B>" + cur->value( "codigo" ) + "</B> que no permite inserciones en el debe de dicha cuenta.";
        agregarError ( cadena, "asiento",  cur->value( "idasiento" ) );
        cur->nextRecord();
    } // end while
    delete cur;

    /// Calculo de cuentas con insercion en el haber que lo tienen bloqueado.
    /// ---------------------------------------------------------------------
    query.sprintf ( "SELECT * FROM asiento, apunte, cuenta WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.nohaber AND apunte.idasiento = asiento.idasiento AND apunte.haber <> 0" );
    cur = mainCompany()->loadQuery ( query );
    while ( !cur->eof() ) {
        QString cadena;
        cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El asiento num. <B>" + cur->value( "ordenasiento" ) + "</B> tiene una insercion en el haber de la cuenta <B>" + cur->value( "codigo" ) + "</B> que no permite inserciones en el haber de dicha cuenta.";
        agregarError ( cadena, "asiento", cur->value( "idasiento" ) );
        cur->nextRecord();
    } // end while
    delete cur;

    /// Calculo de amortizaciones con plazos expirados.
    /// -----------------------------------------------
    query = "SELECT * FROM linamortizacion WHERE fechaprevista < now() AND idasiento IS NULL";
    cur = mainCompany()->loadQuery ( query );
    while ( !cur->eof() ) {
        QString cadena;
        cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>La amortizacion num. <B>" + cur->value( "idamortizacion" ) + "</B> tiene un plazo expirado <B>" + cur->value( "fechaprevista" ) + "</B>.";
        agregarError ( cadena, "amortizacion", "idamortizacion=" + cur->value( "idamortizacion" ) );
        cur->nextRecord();
    } // end while
    delete cur;

    /// Calculo de asientos con IVA y sin facturas asociadas.
    /// -----------------------------------------------------
    query = "SELECT * FROM borrador, cuenta WHERE cuenta.idcuenta = borrador.idcuenta AND codigo LIKE '%47%' AND idasiento NOT IN (SELECT idborrador FROM registroiva)";
    cur = mainCompany()->loadQuery ( query );
    while ( !cur->eof() ) {
        QString cadena;
        cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El asiento num. <B>" + cur->value( "orden" ) + "</B> tiene una insercion en cuentas de IVA (" + cur->value( "codigo" ) + ") sin que haya una factura asociada.";
        agregarError ( cadena, "asiento",  cur->value( "idasiento" ) );
        cur->nextRecord();
    } // end while
    delete cur;


    /// Verifica la longitud de las cuentas.
    /// -----------------------------------------------------
    /// La longitud del codigo es valida si es mayor que 'x' e igual a 'x' + 'y'.
    query = "SELECT valor FROM configuracion WHERE nombre = 'CodCuenta'";
    cur = mainCompany()->loadQuery ( query );

    /// Longitud de x.
    int longitudx = cur->value( "valor" ).replace(QString("y"), QString("")).trimmed().length();
    int totallongitud = cur->value( "valor" ).trimmed().length();

    /// Busca errores.
    query = "SELECT * FROM cuenta WHERE char_length(codigo) > '" + QString::number(longitudx) + "' AND char_length(codigo) != '" + QString::number(totallongitud) + "'";
    BlDbRecordSet *cur2 = mainCompany()->loadQuery ( query );

    while ( !cur2->eof() ) {
	QString cadena;
	cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El codigo de la cuenta <B>" + cur2->value( "codigo" ) + "</B> no tiene la longitud adecuada.";
	agregarError ( cadena, "cuenta", cur2->value("idcuenta") );
	cur2->nextRecord();
    } // end while

    delete cur2;
    delete cur;

    
    /// Busca codigos de cuentas repetidos.
    /// -----------------------------------------------------
    query = "SELECT codigo, COUNT(codigo) AS repeticiones FROM cuenta GROUP BY codigo HAVING ( COUNT(codigo) > 1 )";
    cur = mainCompany()->loadQuery ( query );

    while ( !cur->eof() ) {
	QString cadena;
	QString cadena2;
	
	cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El codigo de la cuenta <B>" + cur->value( "codigo" ) + "</B> esta repedido <B>" + cur->value( "repeticiones" ) + "</B> veces:<BR>";
	
	
	query = "SELECT idcuenta, codigo, descripcion FROM cuenta WHERE codigo = '" + cur->value( "codigo" ) + "'";
	cur2 = mainCompany()->loadQuery ( query );
	while ( !cur2->eof() ) {
	    /// Muestra las 3 primeras letras de la descripcion de la cuenta.
	    cadena2 += "<tr><td>Cuenta: <B>'" + cur2->value("descripcion").left(7) + "...</B>' <a name='idcuenta' href='adf.html?op=abrir&tabla=cuenta&id=" + cur2->value( "idcuenta" ) + "'>corregir</a></td></tr>";
	    cur2->nextRecord();
	} // end while
	delete cur2;
		
	//agregarError ( cadena, "idcuenta", "idcuenta=" + cur->value( "idcuenta" ) );
	textBrowser += "<HR><table><tr><td>" + cadena + "</td></tr>" + cadena2 + "</table>";

	cur->nextRecord();
    } // end while
    
    delete cur;    
    
    
    
    g_plugParams = &textBrowser;
    g_plugins->run("CorrectorWidget_corregir", this);
    
    textBrowser += "</BODY></HTML>";
    mui_browser->setHtml ( textBrowser );
    
}


///
/**
\param mensaj
**/
void correctorwidget::setMensaje ( QString mensaje )
{
    BL_FUNC_DEBUG
    mui_browser->setHtml ( mensaje );
    
}



/** Detecta la pulsacion de un enlace en el widget de resultados.
    Se activa esta funcion cuando se ha pulsado un link en el Widget de resultados
    QTextBrowser.
    De esta forma la aplicacion puede interactuar con la pagina web que se muestra. */
/**
\param url
\return
**/
void correctorwidget::alink ( const QUrl &url )
{
    BL_FUNC_DEBUG
    QUrlQuery uquery(url);
	    QString id = uquery.queryItemValue("tabla") + "_" + uquery.queryItemValue("id");
	    g_plugParams = &id;
	    int resur = g_plugins->run ("Plugin_open", ( BlMainCompany * ) mainCompany());
}


/** Se encarga de agregar un error en el browser que los presenta.
    Sirve para para introducir los errores hayados en el corrector y que se vean por
    pantalla. */
/**
\param texto
\param texto1
\param texto2
**/
void correctorwidget::agregarError ( QString texto, QString texto1, QString texto2 )
{
    BL_FUNC_DEBUG
    
    textBrowser += "<HR><table><tr><td colspan=2>" + texto + "</td></tr><tr><td><a name='masinfo' href='abredoc?op=masinfo&tabla="+texto1+"&id=" + texto2 + "'>+info</a></td><td></td></tr></table>";
}


///
/**
\param a
**/
void correctorwidget::cambia ( bool a )
{
    BL_FUNC_DEBUG
    if ( a ) {
        dock->hide();
        dock->show();
        dock->showMaximized();
    } else {
        dock->hide();
    } // end if
    
}

