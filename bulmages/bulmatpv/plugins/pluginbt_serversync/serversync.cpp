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
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

#include "serversync.h"
#include "bldb.h"
#include "btbulmatpv.h"
#include "btsubform.h"


ServerSync::ServerSync ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    setFocusPolicy ( Qt::NoFocus );
    emp->pWorkspace()->addSubWindow ( this );
    setWindowTitle ( "Servidor" );

    m_tcpServer = new QTcpServer(this);
    if (!m_tcpServer->listen(QHostAddress::Any, 5899)) {
         blMsgInfo(m_tcpServer->errorString());
         close();
         return;
    }

    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(conection()));

    mui_plainText->setMaximumBlockCount(100);


    g_plugins->run ( "ServerSync_ServerSync_Post", this );
    
}

ServerSync::~ServerSync()
{
    BL_FUNC_DEBUG
    
}


void ServerSync::conection()
{
    BL_FUNC_DEBUG
    QTcpSocket *socket = m_tcpServer->nextPendingConnection();
    QHostAddress conectadofrom = socket->peerAddress();
    m_listaSockets.append(socket);
    
    mui_plainText->appendPlainText("Nueva Conexion: " + conectadofrom.toString() + "\n");

    QString texto = ((BtCompany *)mainCompany())->exportXML();
    send(texto);    
    
    connect (socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect (socket, SIGNAL(readChannelFinished()), this, SLOT(readChannelFinished()));
    
}

void ServerSync::readyRead() {
    BL_FUNC_DEBUG
    static QByteArray array = "";
    QTcpSocket *socket = (QTcpSocket *) sender();
    array += socket->readAll();
    QString mensaje = "Mensaje desde: "+ socket->peerAddress().toString() + "\n";
    
    if (array.contains("</BTCOMPANY>")) {
	mensaje = "Mensaje completado desde: "+ socket->peerAddress().toString() + "\n";
	/// Redirigimos el mensaje a todos los clientes conectados al servidor.
	QString texto(array);
	send(texto);
	mui_plainText->appendPlainText(mensaje);
	((BtCompany *)mainCompany())->syncXML(texto);
	array = "";
    }// end while
    
}

void ServerSync::readChannelFinished() {
    BL_FUNC_DEBUG
    QTcpSocket *socket = (QTcpSocket *) sender();
    QString mensaje = "Fin de la comunicacion: "+ socket->peerAddress().toString() + "\n";
    mui_plainText->appendPlainText(mensaje);
    m_listaSockets.removeAll( socket);
    
}

void ServerSync::send(const QString & texto) {
    BL_FUNC_DEBUG
    QTcpSocket *socket;
    /// Redirigimos el mensaje a todos los clientes conectados al servidor.
    for (int i = 0; i < m_listaSockets.size(); ++i) {
	socket = m_listaSockets.at(i);
	if (socket != (QTcpSocket *) sender()) {
	  mui_plainText->appendPlainText("Enviando mensaje a:" + socket->peerAddress().toString() + "\n");
	  socket->write(texto.toLatin1());
	} // end if
    } // end for
    
}