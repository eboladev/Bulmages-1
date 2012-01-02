/***************************************************************************
 *   Copyright (C) 2011 by Fco. Javier M. C.                               *
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

#include "cobrarparcialview.h"

typedef QMap<QString, BlFixed> base;



CobrarParcialView::CobrarParcialView(BtCompany *emp, QWidget *parent) : BlWidget (emp, parent)
{
    BlDebug::blDebug( Q_FUNC_INFO);
    
    ticketDestino = ( ( BtCompany * ) mainCompany() )->newBtTicket();
    ticketOrigen = ( ( BtCompany * ) mainCompany() )->newBtTicket();

    BtTicket *actTicket = ( ( BtCompany * ) mainCompany() )->ticketActual();
    
    /// Establece el nombre del ticket. Le pongo ':' para diferenciar el nombre del ticket.
    ticketDestino->setDbValue("nomticket", actTicket->dbValue("nomticket") + ":" );
    ticketOrigen->setDbValue("nomticket", actTicket->dbValue("nomticket") );

    /// Coge el ticket actual y va transcribiendo la informacion de las lineas en el ticket origen.
    for (int i = 0; i < actTicket->listaLineas()->size(); ++i) {

	ticketOrigen->agregarLinea();
	/// Recorre cada campo.
	for (int j = 0; j < actTicket->listaLineas()->at(i)->lista()->size(); ++j) {
	    QString campo = actTicket->listaLineas()->at(i)->lista()->at(j)->fieldName();
	    QString valor = actTicket->listaLineas()->at(i)->lista()->at(j)->fieldValue();
	    ticketOrigen->listaLineas()->at(i)->setDbValue(campo, valor);
	} // end for
    } // end for

    setupUi(this);
    
    /// Establece la linea seleccionada inicial a la primera del ticket.
    if (ticketOrigen->listaLineas()->size() > 0) ticketOrigen->setLineaActual(ticketOrigen->listaLineas()->at(0));
    if (ticketDestino->listaLineas()->size() > 0) ticketDestino->setLineaActual(ticketDestino->listaLineas()->at(0));
    
    pintar();
        
}


void CobrarParcialView::pintarTotal(BtTicket *tick, QLineEdit *lineEdit)
{
    BlDebug::blDebug( Q_FUNC_INFO);
    base basesimp;
    base basesimpreqeq;
    BlDbRecord *linea;
    int precision = 0;
    int maxprecision = 0;
    
    /// Impresion de los contenidos.
    QString l;
    BlFixed total ( "0.00" );
    BlFixed descuentolinea ( "0.00" );
    for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
        if (linea = tick->listaLineas() ->at ( i ) ) {
            BlFixed cant ( linea->dbValue ( "cantlalbaran" ) );
	    cant.setPrecision(((BtCompany *)tick->mainCompany())->decimalesCantidad());
            BlFixed pvpund ( linea->dbValue ( "pvpivainclalbaran" ) );
	    pvpund.setPrecision(((BtCompany *)tick->mainCompany())->decimalesPrecio());
            BlFixed cantpvp = cant * pvpund;
            precision = cant.precision() > pvpund.precision() ? cant.precision() : pvpund.precision();
            maxprecision = precision > maxprecision ? precision : maxprecision;
            total = total + cantpvp;
        } // end if
    } // end for

    /// Pintamos el total
    maxprecision = 2;
    lineEdit->setText ( total.toQString('0', maxprecision) );

}


void CobrarParcialView::pintarOrigen()
{
    BlDebug::blDebug( Q_FUNC_INFO);
    QString query;
    BlDbRecord *item;

    QString buscar;
    

    QString plainTextContent = "";
    QString htmlContent = "<p style=\"font-family:monospace; font-size: 12pt;\">";

    plainTextContent += "Ticket: " + ticketOrigen->dbValue ( "nomticket" ) + " " + _("(I.V.A. inc.)") + "\n";
    htmlContent += "Ticket: " + ticketOrigen->dbValue ( "nomticket" ) + " " + _("(I.V.A. inc.)") + "<br>";

    htmlContent += "<table border=\"0\" width=\"100%\">";

    plainTextContent += "  " + QString(_("CANT:")) + "  " + QString(_("ARTICULO:")) + "  " + QString(_("PRECIO:")) + "\n";
    plainTextContent += "----------------------------------------\n";

    htmlContent += "<tr><td width=\"10%\">" + QString(_("CANT:")) + "</td><td width=\"80%\">" + QString(_("ARTICULO:")) + "</td><td width=\"10%\">" + QString(_("PRECIO:")) + "</td></tr>";
    htmlContent += "<tr><td colspan=\"3\" style=\"font-size:1px;\"><hr></td></tr>";

    for ( int i = 0; i < ticketOrigen->listaLineas()->size(); ++i ) {
        item = ticketOrigen->listaLineas()->at ( i );
        QString bgColor = "#FFFFFF";
        if ( item == ticketOrigen->lineaActBtTicket() ) {
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
	if (item->exists("imglalbaran")) {
		
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

    mui_browser_origen->setText ( htmlContent );
    //mui_plainText->setPlainText ( plainTextContent );
    
    mui_browser_origen->find ( buscar );
    QTextCursor cursor = mui_browser_origen->textCursor();
    cursor.clearSelection();
    mui_browser_origen->setTextCursor( cursor );
    
    /// Pinta total.
    pintarTotal(ticketOrigen, mui_total_ticket_origen);
    
    
}


void CobrarParcialView::pintarDestino()
{
    BlDebug::blDebug( Q_FUNC_INFO);
    QString query;
    BlDbRecord *item;

    QString buscar;

    QString plainTextContent = "";
    QString htmlContent = "<p style=\"font-family:monospace; font-size: 12pt;\">";

    plainTextContent += "Ticket: " + ticketDestino->dbValue ( "nomticket" ) + " " + _("(I.V.A. inc.)") + "\n";
    htmlContent += "Ticket: " + ticketDestino->dbValue ( "nomticket" ) + " " + _("(I.V.A. inc.)") + "<br>";

    htmlContent += "<table border=\"0\" width=\"100%\">";

    plainTextContent += "  " + QString(_("CANT:")) + "  " + QString(_("ARTICULO:")) + "  " + QString(_("PRECIO:")) + "\n";
    plainTextContent += "----------------------------------------\n";

    htmlContent += "<tr><td width=\"10%\">" + QString(_("CANT:")) + "</td><td width=\"80%\">" + QString(_("ARTICULO:")) + "</td><td width=\"10%\">" + QString(_("PRECIO:")) + "</td></tr>";
    htmlContent += "<tr><td colspan=\"3\" style=\"font-size:1px;\"><hr></td></tr>";

    for ( int i = 0; i < ticketDestino->listaLineas()->size(); ++i ) {
        item = ticketDestino->listaLineas()->at ( i );
        QString bgColor = "#FFFFFF";
        if ( item == ticketDestino->lineaActBtTicket() ) {
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
	if (item->exists("imglalbaran")) {
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

    mui_browser_destino->setText ( htmlContent );
    //mui_plainText->setPlainText ( plainTextContent );
    
    mui_browser_destino->find ( buscar );
    QTextCursor cursor = mui_browser_destino->textCursor();
    cursor.clearSelection();
    mui_browser_destino->setTextCursor( cursor );
    
    /// Pinta total.
    pintarTotal(ticketDestino, mui_total_ticket_destino);
    
    
}


void CobrarParcialView::pintar()
{
    BlDebug::blDebug( Q_FUNC_INFO);

    pintarOrigen();
    pintarDestino();
    
    
}



CobrarParcialView::~CobrarParcialView()
{
    BlDebug::blDebug( Q_FUNC_INFO);
}


void CobrarParcialView::on_mui_aceptar_clicked()
{
    BlDebug::blDebug( Q_FUNC_INFO);
  
    BtTicket *actTicket = ( ( BtCompany * ) mainCompany() )->ticketActual();

    /// El ticket origen no puede estar vacio. Es un cobro parcial no mover ticket.
    if (ticketOrigen->listaLineas()->size() < 1) {
      	blMsgWarning (_("El ticket origen no puede estar vacio. Deje al menos 1 linea."));
	return;
    } // end if
    
    /// Comprueba que el ticket destino no este vacio. En ese caso no se acepta el ticket.
    if (ticketDestino->listaLineas()->size() > 0) {
	/// Pone el ticket destino en la lista de tickets.
	( ( BtCompany * ) mainCompany() )->listaTickets()->prepend(ticketDestino);

	/// Modifica el ticket actual para reflejar los cambios de 'origen'.
	/// Al modificar no tiene que imprimir cambios porque no existen al encontrarse en el
	/// ticket 'destino'.
	/// Si el ticket 'origen' esta vacio entonces se tiene que borrar el ticket actual.

	/// Vacia el ticket actual de lineas.
	int nLineas = actTicket->listaLineas()->size();
	for (int i = 0; i < nLineas; ++i) {
	    actTicket->listaLineas()->takeFirst()->remove();
	} // end for
	
	/// Coge el ticket origen y va transcribiendo la informacion de las lineas en el ticket actual.
	bool necesitaGuardar = FALSE;
	for (int i = 0; i < ticketOrigen->listaLineas()->size(); ++i) {
	  
	    actTicket->agregarLinea();
	    /// Recorre cada campo.
	    for (int j = 0; j < ticketOrigen->listaLineas()->at(i)->lista()->size(); ++j) {
		
		QString campo = ticketOrigen->listaLineas()->at(i)->lista()->at(j)->fieldName();
		QString valor = ticketOrigen->listaLineas()->at(i)->lista()->at(j)->fieldValue();

		if ((campo == "idalbaran") && (!valor.isEmpty())) {
		    necesitaGuardar = TRUE;
		} // end if

		/// Transcribe.
		actTicket->listaLineas()->at(i)->setDbValue(campo, valor);
		
	    } // end for

	    /// Borra informacion de albaran.
	    actTicket->listaLineas()->at(i)->setDbValue("idalbaran", "");
	    actTicket->listaLineas()->at(i)->setDbValue("numlalbaran", "");

	} // end for
	
	/// Agrega log.
	actTicket->agregarLog("COBRAR PARCIAL");
	ticketDestino->agregarLog("COBRAR PARCIAL");

	/// Si el ticket actual se imprimio entonces tiene informacion del albaran
	/// 'idalbaran' e 'numlalbaran'. Si es asi se tiene que guardar el nuevo ticket en
	/// un nuevo albaran. Llamando a 'save()' es suficiente para tener informacion actualizada.
	if (necesitaGuardar) {
	    ticketDestino->save();
	    actTicket->save();
	} // end if


	/// Selecciona ticket.
	( ( BtCompany * ) mainCompany() )->ticketActual()->setDbValue("bloqueadoticket", "FALSE");
	( ( BtCompany * ) mainCompany() )->setTicketActual(ticketDestino);
	ticketDestino->pintar();
	
	/// Cierra ventana.
	( ( QDialog * ) parent() )->close();
	
    } else {
	blMsgWarning (_("El nuevo ticket no puede estar vacio para crearlo."));
    } // end if
  
}


void CobrarParcialView::on_mui_cancelar_clicked()
{
    BlDebug::blDebug( Q_FUNC_INFO);
    ( ( QDialog * ) parent() )->close();
}


void CobrarParcialView::intercambiarLineasTicket(BtTicket *origen, BtTicket *destino, bool lineaCompleta, float cantidad)
{
  BlDebug::blDebug( Q_FUNC_INFO);
  if (origen->listaLineas()->size() == 0) return;
  
  bool destinoExiste = false;
  int i; /// Se utiliza para seleccionar la linea destino cuando exista.
  for (i = 0; i < destino->listaLineas()->size(); ++i) {
    if (destino->listaLineas()->at(i)->dbValue("idarticulo") == origen->lineaActBtTicket()->dbValue("idarticulo")) {
	destinoExiste = true;
	break;
    } // end if
  } // end for


  if (!destinoExiste) {

    /// Crea linea.
    BlDbRecord *r = destino->agregarLinea();
    destino->setLineaActual(r);
    /// Copia linea de origen a destino. Recorre cada campo.
    for (int j = 0; j < origen->lineaActBtTicket()->lista()->size(); ++j) {
      
	QString campo = origen->lineaActBtTicket()->lista()->at(j)->fieldName();
	QString valor = origen->lineaActBtTicket()->lista()->at(j)->fieldValue();
	
	destino->lineaActBtTicket()->setDbValue(campo, valor);
	
    } // end for
    destino->lineaActBtTicket()->setDbValue("cantlalbaran", QString::number(0));

  } else {
    
    destino->setLineaActual(destino->listaLineas()->at(i));
    
  } // end if
  
    /// Establece cantidad en destino.
    /// Si la cantidad de origen es 'cantidad' o inferior entonces traspasa toda la linea.
    if (origen->lineaActBtTicket()->dbValue("cantlalbaran").toFloat() <= cantidad ) {
      
      destino->lineaActBtTicket()->setDbValue("cantlalbaran", QString::number(destino->lineaActBtTicket()->dbValue("cantlalbaran").toFloat() + origen->lineaActBtTicket()->dbValue("cantlalbaran").toFloat()));
      
      /// Borra la linea de origen.
      origen->borrarLinea(origen->lineaActBtTicket());
      
    } else {
      
      float cantidadMover;
      
      if (lineaCompleta) {
	  /// Mueve toda la linea.
	  cantidadMover = origen->lineaActBtTicket()->dbValue("cantlalbaran").toFloat();

      } else {
	  cantidadMover = cantidad;
	
      } // end if

      destino->lineaActBtTicket()->setDbValue("cantlalbaran", QString::number(destino->lineaActBtTicket()->dbValue("cantlalbaran").toFloat() + cantidadMover));
      
      /// Establece cantidad en origen.
      origen->lineaActBtTicket()->setDbValue("cantlalbaran", QString::number(origen->lineaActBtTicket()->dbValue("cantlalbaran").toFloat() - cantidadMover));

      if (lineaCompleta) {

	  /// Borra la linea de origen.
	  origen->borrarLinea(origen->lineaActBtTicket());
	  
      } // end if
      
    } // end if
    
    pintar();

}


void CobrarParcialView::on_mui_unidad2destino_clicked()
{
  BlDebug::blDebug( Q_FUNC_INFO);
  intercambiarLineasTicket(ticketOrigen, ticketDestino);
  
}


void CobrarParcialView::on_mui_unidad2origen_clicked()
{
    BlDebug::blDebug( Q_FUNC_INFO);
    intercambiarLineasTicket(ticketDestino, ticketOrigen);
  
}


void CobrarParcialView::on_mui_linea2destino_clicked()
{
      BlDebug::blDebug( Q_FUNC_INFO);
      intercambiarLineasTicket(ticketOrigen, ticketDestino, true);
  
}


void CobrarParcialView::on_mui_linea2origen_clicked()
{
    BlDebug::blDebug( Q_FUNC_INFO);  
    intercambiarLineasTicket(ticketDestino, ticketOrigen, true);
}


void CobrarParcialView::on_mui_destino_arriba_clicked()
{
    BlDebug::blDebug( Q_FUNC_INFO);
    /// Simulamos la pulsacion de la tecla arriba
    ticketDestino->subir();
    pintarDestino();
}


void CobrarParcialView::on_mui_destino_abajo_clicked()
{
    BlDebug::blDebug( Q_FUNC_INFO);
    /// Simulamos la pulsacion de la tecla abajo
    ticketDestino->bajar();
    pintarDestino();
}


void CobrarParcialView::on_mui_origen_arriba_clicked()
{
    BlDebug::blDebug( Q_FUNC_INFO);
    /// Simulamos la pulsacion de la tecla arriba
    ticketOrigen->subir();
    pintarOrigen();
}


void CobrarParcialView::on_mui_origen_abajo_clicked()
{
    BlDebug::blDebug( Q_FUNC_INFO);
    /// Simulamos la pulsacion de la tecla abajo
    ticketOrigen->bajar();
    pintarOrigen();
}
