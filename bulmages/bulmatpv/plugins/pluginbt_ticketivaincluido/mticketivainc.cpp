/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#include <QtWidgets/QLabel>
#include <QtWidgets/QTextBrowser>
#include <QtGui/QBitmap>

#include "mticketivainc.h"
#include "bldb.h"
#include "btbulmatpv.h"
#include "btsubform.h"


/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;

MTicketIVAInc::MTicketIVAInc ( BtCompany *btCompany, QWidget *parent ) : BlWidget ( btCompany, parent )
{
    BL_FUNC_DEBUG

    m_btCompany = btCompany;

    setupUi ( this );
    setFocusPolicy ( Qt::NoFocus );

    m_btCompany->pWorkspace()->addSubWindow ( this );
    m_parent = parent;

    /// Por defecto hacemos el browser invisible porque es leeeento
    mui_plainText->setVisible(false);
    mui_frame->setVisible(false);

    pintar();

    g_plugins->run ( "MTicketIVAInc_MTicketIVAInc_Post", this );
}


MTicketIVAInc::~MTicketIVAInc()
{
    BL_FUNC_DEBUG
}


void MTicketIVAInc::pintar()
{
    BL_FUNC_DEBUG
	
    QString query;
    BlDbRecord *item;

    if ( g_plugins->run ( "MTicketIVAInc_pintar", this ) ) {
        
        return;
    } // end if
	
    QString buscar;

    BtTicket *ticket = m_btCompany->ticketActual();
    
    /// Establecemos la condicion de que la sigueinte linea es una insercion segun el ticket actual.
    mui_nextLineIsInsert->setChecked(ticket->nextLineIsInsert());

    QString plainTextContent = "";
    QString htmlContent = "<p style=\"font-family:monospace; font-size: 12pt;\">";

    QString tituloTicket = "Ticket: " + ticket->dbValue ( "nomticket" ) + " " + _("(I.V.A. inc.)");
	
    m_parent->setWindowTitle(tituloTicket);

    query = "SELECT idtrabajador, nomtrabajador FROM trabajador WHERE idtrabajador = " + ticket->dbValue ( "idtrabajador" );
    BlDbRecordSet *rsTrabajador = mainCompany()->loadQuery ( query );
	if (rsTrabajador) {
		plainTextContent += "Trabajador: " + rsTrabajador->value ( "nomtrabajador" ) + "\n";
		htmlContent += "Trabajador: " + rsTrabajador->value ( "nomtrabajador" ) + "<br>";
		delete rsTrabajador;
	} // end if
	
    query = "SELECT idcliente, nomcliente FROM cliente WHERE idcliente = " + ticket->dbValue ( "idcliente" );
    BlDbRecordSet *rsCliente = mainCompany()->loadQuery ( query );

	if (rsCliente) {
		plainTextContent += "Cliente: " + rsCliente->value ( "nomcliente" ) + "\n";
		htmlContent += "Cliente: " + rsCliente->value ( "nomcliente" ) + "<br>";
		delete rsCliente;
	} // end if

    htmlContent += "<table border=\"0\" width=\"100%\">";

    plainTextContent += "  " + QString(_("CANT:")) + "  " + QString(_("ARTICULO:")) + "  " + QString(_("PRECIO:")) + "\n";
    plainTextContent += "----------------------------------------\n";

    htmlContent += "<tr><td width=\"10%\">" + QString(_("CANT:")) + "</td><td width=\"80%\">" + QString(_("ARTICULO:")) + "</td><td width=\"10%\">" + QString(_("PRECIO:")) + "</td></tr>";
    htmlContent += "<tr><td colspan=\"3\" style=\"font-size:1px;\"><hr></td></tr>";
	
    for ( int i = 0; i < ticket->listaLineas()->size(); ++i ) {
        item = ticket->listaLineas()->at ( i );
        QString bgColor = "#FFFFFF";
        if ( item == ticket->lineaActBtTicket() ) {
	    buscar = item->dbValue ( "desclalbaran" );
            bgColor = "#CCCCFF";
            plainTextContent += "> ";
        } else {
            plainTextContent += "  ";
        } // end if

        BlFixed cant ( item->dbValue ( "cantlalbaran" ) );
        BlFixed pvpund ( item->dbValue ( "pvpivainclalbaran" ) );
        BlFixed totalLinea = cant * pvpund;
        int precision = cant.precision() > pvpund.precision() ? cant.precision() : pvpund.precision();

        plainTextContent += item->dbValue("cantlalbaran").rightJustified ( 7, ' ', true ) + " ";
        plainTextContent += item->dbValue("desclalbaran").leftJustified ( 20, ' ', true ) + " ";
        plainTextContent += totalLinea.toQString('0', precision).rightJustified ( 9, ' ', true ) + "\n";

        htmlContent += "<tr>";
        htmlContent += "<td bgcolor=\"" + bgColor + "\" align=\"right\" width=\"50\">" + item->dbValue ( "cantlalbaran" ) + "</td>";
        htmlContent += "<td bgcolor=\"" + bgColor + "\">" + item->dbValue ( "desclalbaran" ) + "</td>";
        htmlContent += "<td bgcolor=\"" + bgColor + "\" align=\"right\" width=\"50\">" + totalLinea.toQString('0', precision) + "</td>";
        htmlContent += "</tr>";

	
	/// SI HAY MODIFICADORES LOS PONEMOS. TENER EN CUENTA QUE ESTE CAMBIO SOLO SE TIENE EN CUENTA SI ESTA ACTIVADO
	/// EL PLUGIN DE MODIFICADORES. PERO PUESTO AQUI ES BASTANTE MAS EFICIENTE.
		
	if (item->exists("imgalbaran")) {
	    if (item->dbValue("imglalbaran") != "") {
		    htmlContent += "<tr>";
		    htmlContent += "<td colspan=\"3\" align=\"center\" bgcolor=\"" + bgColor + "\" >";
		    QString text1 = item->dbValue("imglalbaran");
		    QByteArray text = QByteArray::fromBase64(text1.toLatin1());

				  
		    QFile file("/tmp/imagen"+QString::number(i)+".png");
		      if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			  return;

		      file.write(text);
		      file.close();
		    htmlContent += "<img src=\"/tmp/imagen"+QString::number(i)+".png\" width=\"215\" height=\"170\"></td>";
		    htmlContent += "</tr>";
	    } // end if
	    
	    for (int i = 3; i <10; i++) {
		if (item-> dbValue("idmodificador" + QString::number(i)) != "") {
		    htmlContent += "<tr>";
		    query = "SELECT nombremodificador FROM modificador WHERE idmodificador = " + item-> dbValue("idmodificador" + QString::number(i));
		    BlDbRecordSet *rsModificador = mainCompany()->loadQuery ( query );
		    htmlContent += "<td colspan=\"3\" align=\"center\"  bgcolor=\"" + bgColor + "\" >" + rsModificador->value("nombremodificador") + "</TD>";
		    delete rsModificador;
		    htmlContent += "</tr>";
		} // end if
	    } // end for
	} // end if
	/// SIENDO PURISTAS ESTA PARTE DEBERIA ESTAR EN EL pluginbt_modificadores pero aqui tampoco va a molestar mucho.
    } // end for

    plainTextContent += "----------------------------------------\n";
    htmlContent += "<tr><td colspan=\"3\" style=\"font-size:1px;\"><hr></td></tr>";
    htmlContent += "</table>";
    htmlContent += "</p>";
	
    mui_browser->setText ( htmlContent );
    mui_plainText->setPlainText ( plainTextContent );
    
    mui_browser->find ( buscar );
    QTextCursor cursor = mui_browser->textCursor();
    cursor.clearSelection();
    mui_browser->setTextCursor( cursor );
}


void MTicketIVAInc::on_mui_subir_clicked()
{
    /// Simulamos la pulsacion de la tecla arriba
    m_btCompany->pulsaTecla ( Qt::Key_Up );
}


void MTicketIVAInc::on_mui_bajar_clicked()
{
    /// Simulamos la pulsacion de la tecla abajo
    m_btCompany->pulsaTecla ( Qt::Key_Down );
}


void MTicketIVAInc::on_mui_borrar_clicked()
{
    BtTicket * tick = m_btCompany->ticketActual();
    tick->ponerCantidad ( "0" );

    pintar();
}


void MTicketIVAInc::on_mui_imprimir_clicked()
{
    /// Llamamos al atajo de teclado que llama a BtTicket::imprimir()
    m_btCompany->pulsaTecla ( Qt::Key_F6 );
}


void MTicketIVAInc::on_mui_borrarticket_clicked()
{
    BL_FUNC_DEBUG
  
    BtTicket *ticket;
    
    /// No permitimos bajo ningun concepto borrar un ticket que ya ha sido impreso.
    if (m_btCompany->ticketActual()->dbValue("numalbaran") != "") {
	blMsgInfo(_("Operacion no permitida. El ticket se ha imprimido. Debe cobrar el ticket."));
	return;
    } // end if
    
    QString nomticketactual = m_btCompany->ticketActual()->dbValue ( "nomticket" );
    QString idtrabajador = m_btCompany->ticketActual()->dbValue ( "idtrabajador" );
    QString nomticketdefecto = m_btCompany->ticketActual()->nomTicketDefecto();
    int i;


    /// Eliminamos el ticket actual y lo borra de la lista de tickets.
    for ( i = 0; i < m_btCompany->listaTickets() ->size(); ++i ) {
	ticket = m_btCompany->listaTickets() ->at ( i );
	if ( (nomticketactual == ticket->dbValue ( "nomticket" )) && (idtrabajador == ticket->dbValue ( "idtrabajador" )) ) {
	    m_btCompany->listaTickets()->removeAt(i);
	    delete ticket;
	} // end if
    } // end for

    
    /// Solo agregamos a la lista si es el ticket actual.
    if (nomticketactual == nomticketdefecto) {
        /// Creamos un nuevo ticket vacio.
	ticket = m_btCompany->newBtTicket();
	ticket->setDbValue("idtrabajador", idtrabajador);
	m_btCompany->setTicketActual(ticket);
	m_btCompany->listaTickets()->append(ticket);
    } else {
	/// Localizamos el ticket por defecto.
	for ( i = 0; i < m_btCompany->listaTickets() ->size(); ++i ) {
	    ticket = m_btCompany->listaTickets() ->at ( i );
	    if ( (nomticketdefecto == ticket->dbValue ( "nomticket" )) && (idtrabajador == ticket->dbValue ( "idtrabajador" )) ) {
		m_btCompany->setTicketActual(ticket);
	        break;
	    } // end if
	} // end for
    } // end if
    
    ticket->pintar();
}


void MTicketIVAInc::on_mui_nextLineIsInsert_toggled(bool checked)
{
    BL_FUNC_DEBUG

    if (checked){
        m_btCompany->ticketActual()->setNextLineIsInsert(true);
    } else {
	m_btCompany->ticketActual()->setNextLineIsInsert(false);
    } // end if
}

