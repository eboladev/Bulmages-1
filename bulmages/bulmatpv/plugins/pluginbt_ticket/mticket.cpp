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


MTicket::MTicket ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    setFocusPolicy ( Qt::NoFocus );
    emp->pWorkspace() ->addSubWindow ( this );
    setWindowTitle ( "Ticket" );
    g_plugins->run ( "MTicket_MTicket_Post", this );
    
    m_parent = parent;
    
    mui_plainText->setVisible(FALSE);
    mui_frame->setVisible(FALSE);

    
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

    BtTicket *tick = ( ( BtCompany * ) mainCompany() ) ->ticketActual();
    QString htmlContent = "<p style=\"font-family:monospace; font-size: 12pt;\">";
    QString plainTextContent = "";
    QString query;

    QString tituloTicket = "Ticket: " + tick->dbValue ( "nomticket" ) + " " + _("(sin I.V.A.)");
    
    m_parent->setWindowTitle(tituloTicket);

    query = "SELECT * FROM trabajador WHERE idtrabajador = " + tick->dbValue ( "idtrabajador" );
    BlDbRecordSet *rsTrabajador = mainCompany() ->loadQuery ( query );
    plainTextContent += "Trabajador: " + rsTrabajador->value( "nomtrabajador" ) + "\n";
    htmlContent += "Trabajador: " + rsTrabajador->value( "nomtrabajador" ) + "<BR>";
    delete rsTrabajador;

    query = "SELECT * FROM cliente WHERE idcliente = " + tick->dbValue ( "idcliente" );
    BlDbRecordSet *rsCliente = mainCompany() ->loadQuery ( query );
    plainTextContent += "Cliente: " + rsCliente->value( "nomcliente" ) + "\n";
    htmlContent += "Cliente: " + rsCliente->value( "nomcliente" ) + "<BR>";
    delete rsCliente;
    


    htmlContent += "<TABLE border=\"0\" width=\"100%\">";

//     if (tick->dbValue("nomticket") != "") {
//       htmlContent += "<TR><TD colspan=\"3\" align=\"center\"><B>" + tick->dbValue ( "nomticket" ) + "</B></TD></tr>";
//     } // end if

    
    htmlContent += "<TR>";
    htmlContent += "<TD WIDTH=\"10%\">" + QString(_("CANT:")) + "</TD><TD WIDTH=\"80%\">" + QString(_("ARTICULO:")) + "</TD><TD WIDTH=\"10%\">" + QString(_("PRECIO:")) + "</TD>";
    htmlContent += "</TR>";
    htmlContent += "<TR><TD COLSPAN=\"3\"><HR></TD></TR>";
    
    plainTextContent += "\n";
    plainTextContent += "  " + QString(_("CANT:")) + "  " + QString(_("ARTICULO:")) + "  " + QString(_("PRECIO:")) + "\n";
    plainTextContent += "----------------------------------------\n";

    BlDbRecord *item;

    for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
        item = tick->listaLineas() ->at ( i );
        QString bgcolor = "#FFFFFF";
        if ( item == tick->lineaActBtTicket() ) {
    	  buscar = item->dbValue ( "nomarticulo" );
	  bgcolor = "#CCCCFF";
          plainTextContent += "> ";
	} else {
            plainTextContent += "  ";
	} // end if
       
        htmlContent += "<TR>";
        htmlContent += "<TD bgcolor=\"" + bgcolor + "\" align=\"right\">" + item->dbValue ( "cantlalbaran" ) + "</TD>";
        htmlContent += "<TD bgcolor=\"" + bgcolor + "\">" + item->dbValue ( "nomarticulo" ) + "</TD>";
	
	BlFixed totalLinea ( "0.00" );
	totalLinea = BlFixed ( item->dbValue ( "cantlalbaran" ) ) * BlFixed ( item->dbValue ( "pvpivainclalbaran" ) );
	htmlContent += "<TD bgcolor=\"" + bgcolor + "\" align=\"right\">" + totalLinea.toQString() + "</TD>";
	
	plainTextContent += item->dbValue("cantlalbaran").rightJustified ( 7, ' ', TRUE ) + " ";
        plainTextContent += item->dbValue("nomarticulo").leftJustified ( 20, ' ', TRUE ) + " ";
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
    ( ( BtCompany * ) mainCompany() )->pulsaTecla ( Qt::Key_Up );

    
}


void MTicket::on_mui_bajar_clicked()
{
     BL_FUNC_DEBUG

    /// Simulamos la pulsacion de la tecla abajo
    ( ( BtCompany * ) mainCompany() )->pulsaTecla ( Qt::Key_Down );

    
}


void MTicket::on_mui_borrar_clicked()
{
    BL_FUNC_DEBUG

    BtTicket * tick = ( ( BtCompany * ) mainCompany() )->ticketActual();
    tick->ponerCantidad ( "0" );

    pintar();

    
}


void MTicket::on_mui_imprimir_clicked()
{
     BL_FUNC_DEBUG

    /// Llamamos al atajo de teclado que llama a BtTicket::imprimir()
    ( ( BtCompany * ) mainCompany() )->pulsaTecla ( Qt::Key_F6 );

    
}


void MTicket::on_mui_reimprimir_clicked()
{
    BL_FUNC_DEBUG

    BtTicket *previousTicket = new BtTicket( ( BtCompany * ) mainCompany() );
    BlDbRecordSet *cur = mainCompany()->loadQuery ( "SELECT * FROM albaran WHERE ticketalbaran = TRUE ORDER BY idalbaran DESC LIMIT 1" );

    // Si el numero de resultados devuelto = 0 entonces no existe ticket previo.
    if (cur->numregistros() == 0) {

	blMsgInfo(_("No existe ningun ticket anterior para imprimir."));

    } else {

        previousTicket->setDbValue("idalbaran", cur->value("idalbaran"));

	// Cargamos las lineas de albaran
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
    
    BtCompany *emp = ( ( BtCompany * ) mainCompany() );
    BtTicket *ticket;
    QString idtrabajador = emp->ticketActual()->dbValue ( "idtrabajador" );
    QString nomticketactual = emp->ticketActual()->dbValue ( "nomticket" );

   // if (emp->ticketActual()->dbValue ( "nomticket" ).isEmpty()) {
      for ( int i = 0; i < emp->listaTickets() ->size(); ++i ) {
	  ticket = emp->listaTickets() ->at ( i );
	  if ( nomticketactual == ticket->dbValue ( "nomticket" ) ) {
	    emp->listaTickets()->removeAt(i);
	    
	  } // end if
      } // end for
    //} // end if
    
    /// Eliminamos el ticket actual.
    delete emp->ticketActual();
    /// Creamos un nuevo ticket vacio.
    ticket = emp-> newBtTicket();
    ticket->setDbValue("idtrabajador", idtrabajador);
    emp->setTicketActual(ticket);
    emp->listaTickets()->append(ticket);
    /// Solo agregamos a la lista si es el ticket actual.
    if (nomticketactual == emp->ticketActual()->nomTicketDefecto()) {
	emp->listaTickets()->append(ticket);
    } // end if
    
    ticket->pintar();
    
    
}

void MTicket::on_mui_formatear_clicked()
{
    BL_FUNC_DEBUG
}

