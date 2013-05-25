/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2012 by Fco. Javier M. C.                               *
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

#include "bcasientoview.h"
#include "correctorwidget.h"
#include "confreglasview.h"
#include "pluginbc_asiento.h"


/** Inicializacion de la clase.
    En el constructor del corrector no se hacen tareas especiales. */
/**
\param parent
\param fl
**/
CorrectorWidget::CorrectorWidget ( QWidget* parent, Qt::WindowFlags flags )
        : QWidget ( parent, flags )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    QObject::connect ( mui_browser, SIGNAL ( anchorClicked ( const QUrl ) ), this, SLOT ( alink ( const QUrl ) ) );    
}


///
/**
**/
CorrectorWidget::~CorrectorWidget()
{
    BL_FUNC_DEBUG
}


///
/**
**/
void CorrectorWidget::setCompany  ( BcCompany *company ) {
    m_company = company;
    m_dbConnection = company->bdempresa();
}


///
/**
**/
void CorrectorWidget::cambia ( bool a ) {
    if ( a ) {
	m_dockWidget->hide();
	m_dockWidget->show();
	m_dockWidget->showMaximized();
	m_viewCorrector->setChecked ( true );
    } else {
	m_dockWidget->hide();
	m_viewCorrector->setChecked ( false );
    } // end if
}



/** Se ha pulsado sobre el boton de configuracion de reglas.
    Activa la ventana de correcion de reglas \ref confreglasview */
/**
**/
void CorrectorWidget::on_mui_configurar_clicked()
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
void CorrectorWidget::on_mui_corregir_clicked()
{
    BL_FUNC_DEBUG
    m_textBrowser = "<HTML><BODY BGCOLOR='#FFFFFF'>";
    BlDbRecordSet *cur;
    BlDbRecordSet *cur2;

    /// Calculo de asientos abiertos.
    QString query;
    query.sprintf ( "SELECT *, asiento.idasiento AS idas FROM asiento LEFT JOIN (SELECT count(idborrador) AS numborr, idasiento FROM borrador GROUP BY idasiento) AS borr ON borr.idasiento = asiento.idasiento LEFT JOIN (SELECT count(idapunte) AS numap, idasiento FROM apunte GROUP BY idasiento) AS apunt ON apunt.idasiento = asiento.idasiento WHERE apunt.numap = 0 OR numap ISNULL" );
    cur = m_dbConnection->loadQuery ( query );
    while ( !cur->eof() ) {
        QString cadena;
        cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El asiento num. <B>" + cur->value( "ordenasiento" ) + "</B> con fecha <B>" + cur->value( "fecha" ) + "</B> esta abierto, esto causa que el asiento no modifique el estado de las cuentas.";
        agregarError ( cadena, "asiento", "idasiento=" + cur->value( "idas" ) );
        cur->nextRecord();
    } // end while
    delete cur;

    /// Calculo de insercion en cuentas intermedias (con hijos).
    /// --------------------------------------------------------
    query.sprintf ( "SELECT * FROM asiento, apunte, cuenta WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.idcuenta IN (SELECT padre FROM cuenta) AND apunte.idasiento = asiento.idasiento" );
    cur = m_dbConnection->loadQuery ( query );
    while ( !cur->eof() ) {
        QString cadena;
        cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_critical.png'>&nbsp;&nbsp;<B><I>Critial Error:</I></B><BR>El asiento num. <B>" + cur->value( "ordenasiento" ) + "</B> tiene un apunte con la cuenta <B>" + cur->value( "codigo" ) + "</B> no hija..";
        agregarError ( cadena, "asiento", "idasiento=" + cur->value( "idasiento" ) );
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
    cur = m_dbConnection->loadQuery ( query );
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
            agregarError ( cadena, "asiento", "idasiento=" + cur->value( "idasiento" ) );
        } // end if
        cur->nextRecord();
    } // end while
    delete cur;

    /// Calculo de cuentas con insercion en el debe que lo tienen bloqueado.
    /// --------------------------------------------------------------------
    query.sprintf ( "SELECT * FROM asiento, apunte, cuenta WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.nodebe AND apunte.idasiento = asiento.idasiento AND apunte.debe <> 0" );
    BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("Consulta: '%1'")).arg(query) );
    cur = m_dbConnection->loadQuery ( query, "hola1" );
    while ( !cur->eof() ) {
        QString cadena;
        cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El asiento num. <B>" + cur->value( "ordenasiento" ) + "</B> tiene una insercion en el debe de la cuenta <B>" + cur->value( "codigo" ) + "</B> que no permite inserciones en el debe de dicha cuenta.";
        agregarError ( cadena, "asiento", "idasiento=" + cur->value( "idasiento" ) );
        cur->nextRecord();
    } // end while
    delete cur;

    /// Calculo de cuentas con insercion en el haber que lo tienen bloqueado.
    /// ---------------------------------------------------------------------
    query.sprintf ( "SELECT * FROM asiento, apunte, cuenta WHERE apunte.idcuenta = cuenta.idcuenta AND cuenta.nohaber AND apunte.idasiento = asiento.idasiento AND apunte.haber <> 0" );
    cur = m_dbConnection->loadQuery ( query );
    while ( !cur->eof() ) {
        QString cadena;
        cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El asiento num. <B>" + cur->value( "ordenasiento" ) + "</B> tiene una insercion en el haber de la cuenta <B>" + cur->value( "codigo" ) + "</B> que no permite inserciones en el haber de dicha cuenta.";
        agregarError ( cadena, "asiento", "idasiento=" + cur->value( "idasiento" ) );
        cur->nextRecord();
    } // end while
    delete cur;

    /// Calculo de amortizaciones con plazos expirados.
    /// -----------------------------------------------
    query = "SELECT * FROM linamortizacion WHERE fechaprevista < now() AND idasiento IS NULL";
    cur = m_dbConnection->loadQuery ( query );
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
    cur = m_dbConnection->loadQuery ( query );
    while ( !cur->eof() ) {
        QString cadena;
        cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El asiento num. <B>" + cur->value( "orden" ) + "</B> tiene una insercion en cuentas de IVA (" + cur->value( "codigo" ) + ") sin que haya una factura asociada.";
        agregarError ( cadena, "asiento", "idasiento=" + cur->value( "idasiento" ) );
        cur->nextRecord();
    } // end while
    delete cur;


    /// Verifica la longitud de las cuentas.
    /// -----------------------------------------------------
    /// La longitud del codigo es valida si es mayor que 'x' e igual a 'x' + 'y'.
    query = "SELECT valor FROM configuracion WHERE nombre = 'CodCuenta'";
    cur = m_dbConnection->loadQuery ( query );

    /// Longitud de x.
    int longitudx = cur->value( "valor" ).replace(QString("y"), QString("")).trimmed().length();
    int totallongitud = cur->value( "valor" ).trimmed().length();

    /// Busca errores.
    query = "SELECT * FROM cuenta WHERE char_length(codigo) > '" + QString::number(longitudx) + "' AND char_length(codigo) != '" + QString::number(totallongitud) + "'";
    cur2 = m_dbConnection->loadQuery ( query );

    while ( !cur2->eof() ) {
	QString cadena;
	cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El codigo de la cuenta <B>" + cur2->value( "codigo" ) + "</B> no tiene la longitud adecuada.";
	agregarError ( cadena, "", "" );
	cur2->nextRecord();
    } // end while

    delete cur2;
    delete cur;

    
    /// Busca codigos de cuentas repetidos.
    /// -----------------------------------------------------
    query = "SELECT codigo, COUNT(codigo) AS repeticiones FROM cuenta GROUP BY codigo HAVING ( COUNT(codigo) > 1 )";
    cur = m_dbConnection->loadQuery ( query );

    while ( !cur->eof() ) {
	QString cadena;
	QString cadena2;
	
	cadena = "<img src='" + g_confpr->value( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El codigo de la cuenta <B>" + cur->value( "codigo" ) + "</B> esta repedido <B>" + cur->value( "repeticiones" ) + "</B> veces:<BR>";
	
	
	query = "SELECT idcuenta, codigo, descripcion FROM cuenta WHERE codigo = '" + cur->value( "codigo" ) + "'";
	cur2 = m_dbConnection->loadQuery ( query );
	while ( !cur2->eof() ) {
	    /// Muestra las 3 primeras letras de la descripcion de la cuenta.
	    cadena2 += "<tr><td>Cuenta: <B>'" + cur2->value("descripcion").left(7) + "...</B>' <a name='idcuenta' href='#idcuenta=" + cur2->value( "idcuenta" ) + "'>corregir</a></td></tr>";
	    cur2->nextRecord();
	} // end while
	delete cur2;
		
	//agregarError ( cadena, "idcuenta", "idcuenta=" + cur->value( "idcuenta" ) );
	m_textBrowser += "<HR><table><tr><td>" + cadena + "</td></tr>" + cadena2 + "</table>";

	cur->nextRecord();
    } // end while
    
    delete cur;
    
    m_textBrowser += "</BODY></HTML>";
    mui_browser->setHtml ( m_textBrowser );
    
}


/** Detecta la pulsacion de un enlace en el widget de resultados.
    Se activa esta funcion cuando se ha pulsado un link en el Widget de resultados
    QTextBrowser.
    De esta forma la aplicacion puede interactuar con la pagina web que se muestra. */
/**
\param url
**/
void CorrectorWidget::alink ( const QUrl &url )
{
    BL_FUNC_DEBUG

    QString linker = url.fragment();
    QStringList list = linker.split ( "=" );

    QString query;
    QString codigoOld;
    QString codigoNew;
    QString codigoX;
    BlDbRecordSet *cur;
    BlDbRecordSet *cur2;
    
    
    ///TODO: REVISAR ESTA FUNCION QUE NO HACE BIEN SU TRABAJO.
    if ( list[0] == "ver" ) {
	// TODO.
    } else if ( list[0] == "idasiento" ) {
//        BcAsientoView * view = m_company->intapuntsempresa();
        BcAsientoView * view = g_asiento;
        bool ok;
        view->muestraAsiento ( list[1].toInt ( &ok ) );
        view->hide();
        view->show();
    } else if ( list[0] == "idcuenta" ) {
      /// Recalcula un codigo de cuenta no duplicado.
      /// 0) Busca el codigo de la cuenta.
      query = "SELECT codigo FROM cuenta WHERE idcuenta = '" + list[1] + "';";
      cur = m_dbConnection->loadQuery ( query );
      
      codigoOld = cur->value( "codigo" );
      delete cur;

      /// 1) Coge el codigo padre 'las x'.      
      query = "SELECT valor FROM configuracion WHERE nombre = 'CodCuenta'";
      cur = m_dbConnection->loadQuery ( query );

      /// Longitud de x.
      int longitudx = cur->value( "valor" ).replace(QString("y"), QString("")).trimmed().length();
      int totallongitud = cur->value( "valor" ).trimmed().length();
      
      codigoX = codigoOld.left(longitudx);
      delete cur;

      
      /// 2) Va provando de forma secuencial hasta encontrar un codigo libre con la longitud adecuada segun configuracion.
      
      int i = 1;
      
      while(1) {
	codigoNew = codigoX + QString::number(i).rightJustified(totallongitud - longitudx, '0');
	
	/// Comprueba que el nuevo codigo no este en uso.
	query = "SELECT idcuenta, codigo FROM cuenta WHERE codigo = '" + codigoNew + "'";
	cur = m_dbConnection->loadQuery ( query );

	if (cur->numregistros() == 0) {
	    break;
	} // end if

	i++;
	delete cur;
      } // end while


      /// 3) Muestra informacion de codigo anterior y codigo nuevo para que el usuario acepte o cancele la accion.
      int res = QMessageBox::question ( this, _("Cambiar codigo de cuenta?"), _("El codigo de la cuenta: ") + list[1] + "\n" + _("se cambiara de ") + codigoOld + _(" a ") + codigoNew + ".", QMessageBox::Apply | QMessageBox::Cancel, QMessageBox::NoButton );

      
      /// 4) Renombra el codigo de la cuenta.
      if ( QMessageBox::Apply == res) {
	  query = "UPDATE cuenta SET codigo = '" + codigoNew + "' WHERE idcuenta = '" + list[1] + "'";
	  cur = m_dbConnection->loadQuery ( query );
	  blMsgInfo(_("Cambiado con exito."));
      } // end if
      
      
    } else {
        /*        BcAsientoView *view = m_company->intapuntsempresa();
                bool ok;
                view->muestraAsiento(linker.toInt(&ok));
        */
        QMessageBox::warning ( 0, _ ( "Opcion no implementada" ), _ ( "No se puede acceder al error" ), 0, 1, 2 );
    } // endif
    
}


/** Se encarga de agregar un error en el browser que los presenta.
    Sirve para para introducir los errores hayados en el corrector y que se vean por
    pantalla. */
/**
\param texto
\param texto1
\param texto2
**/
void CorrectorWidget::agregarError ( QString texto, QString texto1, QString texto2 )
{
    BL_FUNC_DEBUG
    m_textBrowser += "<HR><table><tr><td colspan=2>" + texto + "</td></tr><tr><td><!-- a name='masinfo' href='#" + texto1 + "'>+ info</a --></td><td><a name='" + texto1 + "' href='#" + texto2 + "'>ver error</a></td></tr></table>";
    
}
