/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#include "mticket.h"
#include "bldb.h"
#include "btbulmatpv.h"
#include "btsubform.h"


/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;


MTicket::MTicket ( BtCompany *company, QWidget *parent ) : BlWidget ( company, parent )
{
    BL_FUNC_DEBUG
    
    m_btCompany = company;
    
    setupUi ( this );
    setFocusPolicy ( Qt::NoFocus );
    m_btCompany->pWorkspace() ->addSubWindow ( this );
    g_plugins->run ( "MTicket_MTicket_Post", this );
    
    m_parent = parent;
    
    mui_plainText->setVisible(FALSE);
    mui_frame->setVisible(FALSE);

    pintar();
}


MTicket::~MTicket()
{
    BL_FUNC_DEBUG
    
}


void MTicket::pintar()
{
    BL_FUNC_DEBUG

    if ( g_plugins->run ( "MTicket_pintar", this ) ) {
        
        return;
    } // end if

    QString buscar;

    BtTicket *ticket = m_btCompany->ticketActual();

    /// Establecemos la condicion de que la sigueinte linea es una insercion segun el ticket actual.
    mui_nextLineIsInsert->setChecked(ticket->nextLineIsInsert());


    QString htmlContent = "<p style=\"font-family:monospace; font-size: 12pt;\">";
    QString plainTextContent = "";
    QString query;

    QString tituloTicket = "Ticket: " + ticket->dbValue ( "nomticket" ) + " " + _("(sin I.V.A.)");
    
    m_parent->setWindowTitle(tituloTicket);

    query = "SELECT * FROM trabajador WHERE idtrabajador = " + ticket->dbValue ( "idtrabajador" );
    BlDbRecordSet *rsTrabajador = mainCompany() ->loadQuery ( query );
    plainTextContent += "Trabajador: " + rsTrabajador->value( "nomtrabajador" ) + "\n";
    htmlContent += "Trabajador: " + rsTrabajador->value( "nomtrabajador" ) + "<BR>";
    delete rsTrabajador;

    query = "SELECT * FROM cliente WHERE idcliente = " + ticket->dbValue ( "idcliente" );
    BlDbRecordSet *rsCliente = mainCompany() ->loadQuery ( query );
    plainTextContent += "Cliente: " + rsCliente->value( "nomcliente" ) + "\n";
    htmlContent += "Cliente: " + rsCliente->value( "nomcliente" ) + "<BR>";
    delete rsCliente;
    


    htmlContent += "<TABLE border=\"0\" width=\"100%\">";

//     if (ticket->dbValue("nomticket") != "") {
//       htmlContent += "<TR><TD colspan=\"3\" align=\"center\"><B>" + ticket->dbValue ( "nomticket" ) + "</B></TD></tr>";
//     } // end if

    
    htmlContent += "<TR>";
    htmlContent += "<TD WIDTH=\"10%\">" + QString(_("CANT:")) + "</TD><TD WIDTH=\"80%\">" + QString(_("ARTICULO:")) + "</TD><TD WIDTH=\"10%\">" + QString(_("PRECIO:")) + "</TD>";
    htmlContent += "</TR>";
    htmlContent += "<TR><TD COLSPAN=\"3\"><HR></TD></TR>";
    
    plainTextContent += "\n";
    plainTextContent += "  " + QString(_("CANT:")) + "  " + QString(_("ARTICULO:")) + "  " + QString(_("PRECIO:")) + "\n";
    plainTextContent += "----------------------------------------\n";

    BlDbRecord *item;

    for ( int i = 0; i < ticket->listaLineas() ->size(); ++i ) {
        item = ticket->listaLineas() ->at ( i );
        QString bgcolor = "#FFFFFF";
        if ( item == ticket->lineaActBtTicket() ) {
    	  buscar = item->dbValue ( "desclalbaran" );
	  bgcolor = "#CCCCFF";
          plainTextContent += "> ";
	} else {
            plainTextContent += "  ";
	} // end if
       
        htmlContent += "<TR>";
        htmlContent += "<TD bgcolor=\"" + bgcolor + "\" align=\"right\">" + item->dbValue ( "cantlalbaran" ) + "</TD>";
        htmlContent += "<TD bgcolor=\"" + bgcolor + "\">" + item->dbValue ( "desclalbaran" ) + "</TD>";
	
	BlFixed totalLinea ( "0.00" );
	totalLinea = BlFixed ( item->dbValue ( "cantlalbaran" ) ) * BlFixed ( item->dbValue ( "pvplalbaran" ) );
	htmlContent += "<TD bgcolor=\"" + bgcolor + "\" align=\"right\">" + totalLinea.toQString() + "</TD>";
	
	plainTextContent += item->dbValue("cantlalbaran").rightJustified ( 7, ' ', TRUE ) + " ";
        plainTextContent += item->dbValue("desclalbaran").leftJustified ( 20, ' ', TRUE ) + " ";
        plainTextContent += totalLinea.toQString().rightJustified ( 9, ' ', TRUE ) + "\n";
	
        htmlContent += "</TR>";
    } // end for

    htmlContent += "</TABLE>";
    htmlContent += "<BR><HR><BR>";
    
    /// Pintamos el HTML en el textBrowser
    mui_browser->setText ( htmlContent );
    mui_plainText->setText ( plainTextContent );

    mui_browser->find ( buscar );
    QTextCursor cursor = mui_browser->textCursor();
    cursor.clearSelection();
    mui_browser->setTextCursor( cursor );
    
    
}


void MTicket::on_mui_subir_clicked()
{
    BL_FUNC_DEBUG

    /// Simulamos la pulsacion de la tecla arriba
    m_btCompany->pulsaTecla ( Qt::Key_Up );
}


void MTicket::on_mui_bajar_clicked()
{
     BL_FUNC_DEBUG

    /// Simulamos la pulsacion de la tecla abajo
    m_btCompany->pulsaTecla ( Qt::Key_Down );
}


void MTicket::on_mui_borrar_clicked()
{
    BL_FUNC_DEBUG

    BtTicket * tick = m_btCompany->ticketActual();
    tick->ponerCantidad ( "0" );

    pintar();
}


void MTicket::on_mui_imprimir_clicked()
{
     BL_FUNC_DEBUG

    /// Llamamos al atajo de teclado que llama a BtTicket::imprimir()
    m_btCompany->pulsaTecla ( Qt::Key_F6 );
}


void MTicket::on_mui_reimprimir_clicked()
{
    BL_FUNC_DEBUG

    BtTicket *previousTicket = new BtTicket( m_btCompany );
    BlDbRecordSet *cur = mainCompany()->loadQuery ( "SELECT * FROM albaran WHERE ticketalbaran = TRUE ORDER BY idalbaran DESC LIMIT 1" );

    /// Si el numero de resultados devuelto = 0 entonces no existe ticket previo.
    if (cur->numregistros() == 0) {

	blMsgInfo(_("No existe ningun ticket anterior para imprimir."));

    } else {

        previousTicket->setDbValue("idalbaran", cur->value("idalbaran"));

	/// Cargamos las lineas de albaran
        cur = mainCompany()->loadQuery ( "SELECT * FROM lalbaran LEFT JOIN articulo ON lalbaran.idarticulo = articulo.idarticulo WHERE idalbaran = " + cur->value("idalbaran") );

	while ( !cur->eof() ) {
    	    BlDbRecord *l = previousTicket->agregarLinea();
    	    l->DBload( cur );
    	    cur->nextRecord();
        } // end while

	previousTicket->imprimir(FALSE);

	delete previousTicket;

    } // end if

    delete cur;

    
}

void MTicket::on_mui_borrarticket_clicked()
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


void MTicket::on_mui_formatear_clicked()
{
    BL_FUNC_DEBUG
}


void MTicket::on_mui_nextLineIsInsert_toggled(bool checked)
{
    BL_FUNC_DEBUG

    if (checked) {
	m_btCompany->ticketActual()->setNextLineIsInsert(true);
    } else {
	m_btCompany->ticketActual()->setNextLineIsInsert(false);
    } // end if
}

