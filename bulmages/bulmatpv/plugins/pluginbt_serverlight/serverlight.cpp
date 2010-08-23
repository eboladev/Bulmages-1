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

#include <QLabel>
#include <QTextBrowser>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDomDocument>

#include "serverlight.h"
#include "bldb.h"
#include "btbulmatpv.h"
#include "btsubform.h"
#include "btcompany.h"


ServerLight::ServerLight ( BtCompany *emp ) : BlWidget (emp, 0)
{
    blDebug ( "ServerLight::ServerLight", 0 );

    m_tcpServer = new QTcpServer(this);
    if (!m_tcpServer->listen(QHostAddress::Any, 11637)) {
         blMsgInfo(m_tcpServer->errorString());
         return;
    }

    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(conection()));

    g_plugins->lanza ( "ServerLight_ServerLight_Post", this );
    blDebug ( "END ServerLight::ServerLight", 0 );
}


ServerLight::~ServerLight()
{
    blDebug ( "ServerLight::~ServerLight", 0 );
    blDebug ( "END ServerLight::~ServerLight", 0 );
}


void ServerLight::conection()
{
    fprintf(stderr, "conexion\n");
    blDebug ( "ServerLight::conection", 0 );
    QTcpSocket *socket = m_tcpServer->nextPendingConnection();
    QHostAddress conectadofrom = socket->peerAddress();
    m_listaSockets.append(socket);
    
    //QString texto = QString("Nueva Conexion: ") + conectadofrom.toString() + "\n";
    //fprintf(stderr, texto.toAscii());

    connect (socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    blDebug ( "END ServerLight::conection", 0 );
}


void ServerLight::readyRead() {
    blDebug ( "ServerLight::readyRead", 0 );
    static QByteArray array = "";
    QTcpSocket *socket = (QTcpSocket *) sender();
    array += socket->readAll();
    QString mensaje = "Mensaje desde: "+ socket->peerAddress().toString() + "\n";
    
    QString texto(array);

    /// Si se detecta el final de la transmision se procesa la informacion.
    if (texto.contains("</DOCUMENT>", Qt::CaseInsensitive)) {
    
	if (texto.contains("<GETCOMMAND>categoria_articulo</GETCOMMAND>", Qt::CaseInsensitive)) {
	    fprintf(stderr, categoryArticleXML().toAscii() );
	    socket->write(categoryArticleXML().toAscii());
	    /// Hay que cerrar el socket despues del envio.
	    socket->close();
	
        } else if (texto.contains("<PUTCOMMAND>ticket_data</PUTCOMMAND>", Qt::CaseInsensitive)) {
	    /// Nos han mandado la informacion de un ticket.
	    processTicketDataXML(texto);
	    socket->close();
        } else {
	    //fprintf(stderr, "--NO ENTIENDO--");
	} // end if

	/// Como se ha completado la recepcion de datos y se han procesado, se resetea el buffer de entrada.
	array = "";

    } // end if

    blDebug ( "END ServerLight::readyRead", 0 );
}


void ServerLight::send(const QString & texto) {
    blDebug ( "ServerLight::send", 0 );
    //fprintf(stderr, "ENVIANDO\n");
    QTcpSocket *socket;
    /// Redirigimos el mensaje a todos los clientes conectados al servidor.
    for (int i = 0; i < m_listaSockets.size(); ++i) {
	socket = m_listaSockets.at(i);
	if (socket != (QTcpSocket *) sender()) {
	  //mui_plainText->appendPlainText("Enviando mensaje a:" + socket->peerAddress().toString() + "\n");
	  QString txt = "Enviando mensaje a:" + socket->peerAddress().toString() + "\n";
	  //fprintf(stderr, txt.toAscii() );
	  socket->write(texto.toLatin1());
	} // end if
    } // end for
    //fprintf(stderr, "FIN ENVIO");
    blDebug ( "END ServerLight::send", 0 );
}


void ServerLight::processTicketDataXML(QString data)
{
    BtCompany *emp = (BtCompany *) mainCompany();
    BlDbRecord *rec;
    BtTicket *ticket;
    BtTicket *ticketActual;
    
    ticketActual = emp->ticketActual();
  
  
    QString *er = new QString("");
    int *erline;
    int num = 0;
    erline = &num;
  
    QDomDocument doc ( "mydocument" );
    if ( !doc.setContent ( data, true, er, erline ) ) {
      
	//fprintf(stderr, er->toAscii());
	//fprintf(stderr, QString::number(*erline).toAscii());
      
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

	    /// Inserta lineas.
	    BlDbRecord *linea = ticket->agregarLinea();
	    linea->setDbValue("idarticulo", idarticulo);
	    linea->setDbValue("cantlalbaran", cantarticulo);
	    
	    /// El plugin de IVA incluido tiene que estar instalado.
	    
	    /// Buscamos los parametros en la base de datos.
	    QString query = "SELECT pvparticulo, pvpivaincarticulo, codigocompletoarticulo, nomarticulo, porcentasa_iva FROM articulo LEFT JOIN (SELECT idtipo_iva, porcentasa_iva, fechatasa_iva FROM tasa_iva ) AS t1 ON articulo.idtipo_iva = t1.idtipo_iva WHERE idarticulo = " + idarticulo + " ORDER BY t1.fechatasa_iva LIMIT 1";
  	    
	    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );

	    if ( !cur->eof() ) {
		linea->setDbValue ( "pvplalbaran", cur->valor ( "pvparticulo" ) );
		linea->setDbValue ( "pvpivainclalbaran", cur->valor ( "pvpivaincarticulo" ) );		
		linea->setDbValue ( "codigocompletoarticulo", cur->valor ( "codigocompletoarticulo" ) );
		linea->setDbValue ( "nomarticulo", cur->valor ( "nomarticulo" ) );
		linea->setDbValue ( "desclalbaran", cur->valor ( "nomarticulo" ) );
		linea->setDbValue ( "ivalalbaran", cur->valor ( "porcentasa_iva") );

	    } // end if
	    delete cur;

        } // end if
    } // end while


    /// Aparca el ticket.
    emp->listaTickets()->prepend(ticket);
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
	  textoXML += familias->valor("codigocompletofamilia");
	  textoXML += "</CODCATEGORIA>\\\n";

	  textoXML += "<NOMCATEGORIA>";
	  textoXML += familias->valor("nombrefamilia");
	  textoXML += "</NOMCATEGORIA>\\\n";

	      query = "SELECT idarticulo, nomarticulo, pvpivaincarticulo FROM articulo WHERE idfamilia = " + familias->valor("idfamilia") + "ORDER BY nomarticulo";
	      articulos = mainCompany()->loadQuery(query);
	      
	      if (articulos->numregistros() > 0) {
		
		while (!articulos->eof()) {
		  
		      textoXML += "<ARTICULO>\\\n";

			  textoXML += "<IDARTICULO>";
			  textoXML += articulos->valor("idarticulo");
			  textoXML += "</IDARTICULO>\\\n";

			  textoXML += "<NOMARTICULO>";
			  textoXML += articulos->valor("nomarticulo");
			  textoXML += "</NOMARTICULO>\\\n";

			  textoXML += "<PVPARTICULO>";
			  textoXML += articulos->valor("pvpivaincarticulo");
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

