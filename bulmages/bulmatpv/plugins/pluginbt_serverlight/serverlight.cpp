/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
 *                                                                         *
 *   Copyright (C) 2010 by Fco. Javier M. C.                               *
 *   comercial@todo-redes.com                                              *
 *   http://www.todo-redes.com                                             *
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
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtXml/QDomDocument>
#include <QtCore/QBuffer>

#include "serverlight.h"
#include "bldb.h"
#include "btbulmatpv.h"
#include "btsubform.h"
#include "btcompany.h"

QMap <QTcpSocket *, QByteArray> g_buffers;


ServerLight::ServerLight ( BtCompany *emp ) : BlWidget (emp, 0)
{
    BL_FUNC_DEBUG

    m_tcpServer = new QTcpServer(this);
    if (!m_tcpServer->listen(QHostAddress::Any, 11637)) {
         blMsgInfo(m_tcpServer->errorString());
         return;
    }

    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(conection()));

    g_plugins->run ( "ServerLight_ServerLight_Post", this );
    
}


ServerLight::~ServerLight()
{
    BL_FUNC_DEBUG
    
}


void ServerLight::conection()
{
    BL_FUNC_DEBUG
    QTcpSocket *socket = m_tcpServer->nextPendingConnection();
    QHostAddress conectadofrom = socket->peerAddress();
    m_listaSockets.append(socket);
    g_buffers[socket] = "";

    connect (socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    
}


void ServerLight::readyRead() {
    BL_FUNC_DEBUG
    static QByteArray array = "";
    QByteArray lectura = "";
    QTcpSocket *socket = (QTcpSocket *) sender();
    lectura = socket->readAll();
    g_buffers[socket]  += lectura;
    array += lectura;
    QString mensaje = _("Mensaje desde: ")+ socket->peerAddress().toString() + "\n";
    
    QString texto(g_buffers[socket]);

    fprintf(stderr, "\nPaquete recibido:\n");
    fprintf(stderr, array);
    fprintf(stderr, "\nPaquete finalizado :\n");
    fprintf(stderr, QString::number(array.size()).toLatin1());
    fprintf(stderr, "\nFin Paquete recibido :\n");
    
    /// Si se detecta el final de la transmision se procesa la informacion.
    if (texto.contains("</DOCUMENT>", Qt::CaseInsensitive)) {
    
	if (texto.contains("<GETCOMMAND>categoria_articulo</GETCOMMAND>", Qt::CaseInsensitive)) {
	    fprintf(stderr, categoryArticleXML().toLatin1() );
	    socket->write(categoryArticleXML().toLatin1());
	    /// Hay que cerrar el socket despues del envio.
	    g_buffers[socket] = "";
	    g_buffers.remove(socket);
	    socket->close();
	
        } else if (texto.contains("<PUTCOMMAND>ticket_data</PUTCOMMAND>", Qt::CaseInsensitive)) {
	    /// Nos han mandado la informacion de un ticket.
	    processTicketDataXML(texto);
	    g_buffers[socket] = "";
	    g_buffers.remove(socket);
	    socket->close();
        } else {
	    //fprintf(stderr, "--NO ENTIENDO--");
	} // end if

	/// Como se ha completado la recepcion de datos y se han procesado, se resetea el buffer de entrada.
	array = "";

    } // end if

    
}


void ServerLight::send(const QString & texto) {
    BL_FUNC_DEBUG
    QTcpSocket *socket;
    /// Redirigimos el mensaje a todos los clientes conectados al servidor.
    for (int i = 0; i < m_listaSockets.size(); ++i) {
	socket = m_listaSockets.at(i);
	if (socket != (QTcpSocket *) sender()) {
	  //mui_plainText->appendPlainText("Enviando mensaje a:" + socket->peerAddress().toString() + "\n");
	  QString txt = _("Enviando mensaje a:") + socket->peerAddress().toString() + "\n";
	  //fprintf(stderr, txt.toLatin1() );
	  socket->write(texto.toLatin1());
	} // end if
    } // end for
    //fprintf(stderr, "FIN ENVIO");
    
}


void ServerLight::processTicketDataXML(QString data)
{
    BtCompany *emp = (BtCompany *) mainCompany();
    BlDbRecord *rec;
    BtTicket *ticket;
    BtTicket *ticketActual;
    
    fprintf(stdout, "\n==========MENSAJE COMPLETO ===============\n");
    fprintf(stdout, data.toLatin1());
    fprintf(stdout, "\n=========================\n");
    
    ticketActual = emp->ticketActual();
  
  
    QString *er = new QString("");
    int *erline;
    int num = 0;
    erline = &num;
  
    QDomDocument doc ( "mydocument" );
    if ( !doc.setContent ( data, true, er, erline ) ) {
      
	//fprintf(stderr, er-.toLatin1());
	//fprintf(stderr, QString::number(*erline).toLatin1());
      
	fprintf(stderr, "Error en documento XML.\n");
        return;
    } // end if


    QDomElement docElem = doc.documentElement();
    QDomElement principal = docElem.firstChildElement ( "TICKET" );
    
    /// Cogemos la coordenada X
    QDomNodeList nodos = principal.elementsByTagName ( "LINEATICKET" );
    
    
    /// Crea un ticket.
    ticket = emp->newBtTicket();
    
    ticket->setDbValue("nomticket", principal.firstChildElement("NOMTICKET").text() );
    ticket->setDbValue("idtrabajador", "1");

	    
    int i = 0;
    while (i < nodos.count() ) {

        QDomNode ventana = nodos.item ( i++ );
		
        QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
        if ( !e1.isNull() ) { /// the node was really an element.

	    QString idarticulo = e1.firstChildElement("IDARTICULO").text();
	    QString cantarticulo = e1.firstChildElement("CANTARTICULO").text();
	    QString imagen = "";
	    QDomElement modificadores = e1.firstChildElement("MODIFICADORES");
	    if (!modificadores.isNull()) 
	      imagen = modificadores.firstChildElement("IMAGEN").text();
	    
	    
	    /// Inserta lineas.
	    BlDbRecord *linea = ticket->agregarLinea();
	    linea->setDbValue("idarticulo", idarticulo);
	    linea->setDbValue("cantlalbaran", cantarticulo);	    
	    
	    if (imagen != "") {
	      QByteArray bytes1 = QByteArray::fromBase64(imagen.toLatin1());
	      QByteArray bytes;
	      QBuffer buffer(&bytes);
	      QImage img;
	      buffer.open(QIODevice::WriteOnly);
	      img.loadFromData(bytes1, "BMP");
	      img.save("/tmp/imagen.bmp","BMP");
	      img.save(&buffer, "PNG");
	      QString text = bytes.toBase64();
	      linea->setDbValue ( "imglalbaran", text );
	    } else {
	      linea->setDbValue ( "imglalbaran", "" );
	    } // end if
	    


	    
	    /// El plugin de IVA incluido tiene que estar instalado.
	    
	    /// Buscamos los parametros en la base de datos.
	    QString query = "SELECT pvparticulo, pvpivaincarticulo, codigocompletoarticulo, nomarticulo, porcentasa_iva FROM articulo LEFT JOIN (SELECT idtipo_iva, porcentasa_iva, fechatasa_iva FROM tasa_iva ) AS t1 ON articulo.idtipo_iva = t1.idtipo_iva WHERE idarticulo = " + idarticulo + " ORDER BY t1.fechatasa_iva LIMIT 1";
  	    
	    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );

	    if ( !cur->eof() ) {
		linea->setDbValue ( "pvplalbaran", cur->value( "pvparticulo" ) );
		linea->setDbValue ( "pvpivainclalbaran", cur->value( "pvpivaincarticulo" ) );		
		linea->setDbValue ( "codigocompletoarticulo", cur->value( "codigocompletoarticulo" ) );
		linea->setDbValue ( "nomarticulo", cur->value( "nomarticulo" ) );
		linea->setDbValue ( "desclalbaran", cur->value( "nomarticulo" ) );
		linea->setDbValue ( "ivalalbaran", cur->value( "porcentasa_iva") );

	    } // end if
	    delete cur;

        } // end if
    } // end while


    /// Aparca el ticket.
    emp->listaTickets()->prepend(ticket);
    
    /// Hacemos una llamada a plugins para indicar que hay un ticket nuevo y que deben recoger los otros plugins.
    /// En este caso es una llamada extraña pq no se pasa la clase llamante sino que se pasa el ticket generado.
    g_plugins->run("ticket_aparcado_remotamente", ticket);
    
}


QString ServerLight::categoryArticleXML()
{
    QString textoXML = "";
    QString query;

    BlDbRecordSet *familias;
    BlDbRecordSet *articulos;
    
    query = "SELECT idfamilia, codigocompletofamilia, nombrefamilia FROM familia ORDER BY nombrefamilia";
    familias = mainCompany()->loadQuery(query);

    while (!familias->eof()) {
	textoXML += "<CATEGORIA>\\\n";

	  textoXML += "<CODCATEGORIA>";
	  textoXML += familias->value("codigocompletofamilia");
	  textoXML += "</CODCATEGORIA>\\\n";

	  textoXML += "<NOMCATEGORIA>";
	  textoXML += familias->value("nombrefamilia");
	  textoXML += "</NOMCATEGORIA>\\\n";

	      query = "SELECT idarticulo, nomarticulo, pvpivaincarticulo FROM articulo WHERE idfamilia = " + familias->value("idfamilia") + "ORDER BY nomarticulo";
	      articulos = mainCompany()->loadQuery(query);
	      
	      if (articulos->numregistros() > 0) {
		
		while (!articulos->eof()) {
		  
		      textoXML += "<ARTICULO>\\\n";

			  textoXML += "<IDARTICULO>";
			  textoXML += articulos->value("idarticulo");
			  textoXML += "</IDARTICULO>\\\n";

			  textoXML += "<NOMARTICULO>";
			  textoXML += articulos->value("nomarticulo");
			  textoXML += "</NOMARTICULO>\\\n";

			  textoXML += "<PVPARTICULO>";
			  textoXML += articulos->value("pvpivaincarticulo");
			  textoXML += "</PVPARTICULO>\\\n";
		      
		      textoXML += "</ARTICULO>\\\n";
		      
		      articulos->nextRecord();
		  
		} // end while
		
	      } // end if

	textoXML += "</CATEGORIA>\\\n\\\n\\\n";
	
	familias->nextRecord();

    } // end while
    
    return textoXML;
}

