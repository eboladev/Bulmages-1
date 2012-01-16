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
#include <QHostAddress>
#include <QTcpSocket>

#include "clientsync.h"
#include "bldb.h"
#include "btbulmatpv.h"
#include "btsubform.h"
#include "blconfiguration.h"


ClientSync::ClientSync ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    setFocusPolicy ( Qt::NoFocus );
    emp->pWorkspace()->addSubWindow ( this );
    setWindowTitle ( "Cliente" );
    
    m_socket = new QTcpSocket(this);
    m_socket->connectToHost(g_confpr->value(CONF_TPV_SERVER_SYNC_IP), 5899);

    QString mensaje = "Nueva Conexion: "+ m_socket->peerAddress().toString() + "\n";
    mui_plainText->appendPlainText(mensaje);
    mui_plainText->setMaximumBlockCount(100);
    
    
    connect (m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect (m_socket, SIGNAL(readChannelFinished()), this, SLOT(readChannelFinished()));

    g_plugins->run ( "ClientSync_ClientSync_Post", this );
    
}

ClientSync::~ClientSync()
{
    BL_FUNC_DEBUG
    
}



void ClientSync::readyRead() {
    BL_FUNC_DEBUG

    QTcpSocket *socket = (QTcpSocket *) sender();
    static QByteArray array="";
    array += socket->readAll();

    QString mensaje = "Mensaje desde: "+ socket->peerAddress().toString() + "\n";
    QString texto(array);

    if (array.contains("</BTCOMPANY>")) {
	mensaje = "Mensaje Completo desde: "+ socket->peerAddress().toString() + "\n";
	QString texto(array);
	((BtCompany *)mainCompany())->syncXML(texto);
	array = "";
    }// end while
    mui_plainText->appendPlainText(mensaje);    
    
}

void ClientSync::readChannelFinished() {
    BL_FUNC_DEBUG
    QTcpSocket *socket = (QTcpSocket *) sender();
    QString mensaje = "Fin de la comunicacion: "+ socket->peerAddress().toString() + "\n";
    mui_plainText->appendPlainText(mensaje);
    blMsgInfo("Error de comunicacion con el servidor");
    
}

void ClientSync::send(const QString & texto) {
    BL_FUNC_DEBUG
    m_socket->write(texto.toLatin1());
    mui_plainText->appendPlainText("Enviando mensaje a:" + m_socket->peerAddress().toString() + "\n" );  
    
}