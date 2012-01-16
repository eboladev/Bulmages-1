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

#include <QLabel>
#include <QTextBrowser>
#include <QBitmap>


#include "mticketivainc.h"
#include "bldb.h"
#include "btbulmatpv.h"
#include "btsubform.h"

/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;

MTicketIVAInc::MTicketIVAInc ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    setFocusPolicy ( Qt::NoFocus );
	
    emp->pWorkspace()->addSubWindow ( this );
    //setWindowTitle ( "Ticket" );
    m_parent = parent;
	
    /// Por defecto hacemos el browser invisible porque es leeeento
    mui_plainText->setVisible(FALSE);
    mui_frame->setVisible(FALSE);

    g_plugins->run ( "MTicketIVAInc_MTicketIVAInc_Post", this );
	
    pintar();
	
	
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

    BtTicket *ticket = ( ( BtCompany * ) mainCompany() )->ticketActual();

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
	    buscar = item->dbValue ( "nomarticulo" );
            bgColor = "#CCCCFF";
            plainTextContent += "> ";
        } else {
            plainTextContent += "  ";
        } // end if

        BlFixed cant ( item->dbValue ( "cantlalbaran" ) );
        BlFixed pvpund ( item->dbValue ( "pvpivainclalbaran" ) );
        BlFixed totalLinea = cant * pvpund;
        int precision = cant.precision() > pvpund.precision() ? cant.precision() : pvpund.precision();

        plainTextContent += item->dbValue("cantlalbaran").rightJustified ( 7, ' ', TRUE ) + " ";
        plainTextContent += item->dbValue("nomarticulo").leftJustified ( 20, ' ', TRUE ) + " ";
        plainTextContent += totalLinea.toQString('0', precision).rightJustified ( 9, ' ', TRUE ) + "\n";

        htmlContent += "<tr>";
        htmlContent += "<td bgcolor=\"" + bgColor + "\" align=\"right\" width=\"50\">" + item->dbValue ( "cantlalbaran" ) + "</td>";
        htmlContent += "<td bgcolor=\"" + bgColor + "\">" + item->dbValue ( "nomarticulo" ) + "</td>";
        htmlContent += "<td bgcolor=\"" + bgColor + "\" align=\"right\" width=\"50\">" + totalLinea.toQString('0', precision) + "</td>";
        htmlContent += "</tr>";

	
	/// SI HAY MODIFICADORES LOS PONEMOS. TENER EN CUENTA QUE ESTE CAMBIO SOLO SE TIENE EN CUENTA SI ESTA ACTIVADO
	/// EL PLUGIN DE MODIFICADORES. PERO PUESTO AQUI ES BASTANTE MAS EFICIENTE.
		
	if (item->exists("imgalbaran")) {
	    if (item->dbValue("imglalbaran") != "") {
		    htmlContent += "<tr>";
		    htmlContent += "<td colspan=\"3\" align=\"center\" bgcolor=\"" + bgColor + "\" >";
		    QString text1 = item->dbValue("imglalbaran");
		    QByteArray text = QByteArray::fromBase64(text1.toAscii());

				  
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
    ( ( BtCompany * ) mainCompany() )->pulsaTecla ( Qt::Key_Up );
}

void MTicketIVAInc::on_mui_bajar_clicked()
{
    /// Simulamos la pulsacion de la tecla abajo
    ( ( BtCompany * ) mainCompany() )->pulsaTecla ( Qt::Key_Down );
}

void MTicketIVAInc::on_mui_borrar_clicked()
{
    BtTicket * tick = ( ( BtCompany * ) mainCompany() )->ticketActual();
    tick->ponerCantidad ( "0" );

    pintar();
}

void MTicketIVAInc::on_mui_imprimir_clicked()
{
    /// Llamamos al atajo de teclado que llama a BtTicket::imprimir()
    ( ( BtCompany * ) mainCompany() )->pulsaTecla ( Qt::Key_F6 );
}

void MTicketIVAInc::on_mui_borrarticket_clicked()
{
    BtCompany *emp = ( ( BtCompany * ) mainCompany() );
    BtTicket *ticket;
    
    /// No permitimos bajo ningun concepto borrar un ticket que ya ha sido impreso.
    if (emp->ticketActual()->dbValue("numalbaran") != "") {
	blMsgInfo(_("Operacion no permitida. El ticket se ha imprimido. Debe cobrar el ticket."));
	return;
    } // end if
    
    QString nomticketactual = emp->ticketActual()->dbValue ( "nomticket" );
    QString idtrabajador = emp->ticketActual()->dbValue ( "idtrabajador" );
    QString nomticketdefecto = emp->ticketActual()->nomTicketDefecto();
    int i;


    /// Eliminamos el ticket actual y lo borra de la lista de tickets.
    for ( i = 0; i < emp->listaTickets() ->size(); ++i ) {
	ticket = emp->listaTickets() ->at ( i );
	if ( (nomticketactual == ticket->dbValue ( "nomticket" )) && (idtrabajador == ticket->dbValue ( "idtrabajador" )) ) {
	    emp->listaTickets()->removeAt(i);
	    delete ticket;
	} // end if
    } // end for

    
    /// Solo agregamos a la lista si es el ticket actual.
    if (nomticketactual == nomticketdefecto) {
        /// Creamos un nuevo ticket vacio.
	ticket = emp->newBtTicket();
	ticket->setDbValue("idtrabajador", idtrabajador);
	emp->setTicketActual(ticket);
	emp->listaTickets()->append(ticket);
    } else {
	/// Localizamos el ticket por defecto.
	for ( i = 0; i < emp->listaTickets() ->size(); ++i ) {
	    ticket = emp->listaTickets() ->at ( i );
	    if ( (nomticketdefecto == ticket->dbValue ( "nomticket" )) && (idtrabajador == ticket->dbValue ( "idtrabajador" )) ) {
		emp->setTicketActual(ticket);
	        break;
	    } // end if
	} // end for
    } // end if
    
    ticket->pintar();
}

