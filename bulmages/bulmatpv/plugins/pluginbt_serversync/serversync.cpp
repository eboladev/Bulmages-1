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


#include "serversync.h"
#include "bldb.h"
#include "btbulmatpv.h"
#include "btsubform.h"


ServerSync::ServerSync ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    _depura ( "ServerSync::ServerSync", 0 );
    setupUi ( this );
    setFocusPolicy ( Qt::NoFocus );
    emp->pWorkspace()->addWindow ( this );
    setWindowTitle ( "Servidor" );

     m_tcpServer = new QTcpServer(this);
     if (!m_tcpServer->listen()) {
         mensajeInfo(m_tcpServer->errorString());
         close();
         return;
     }

     mensajeInfo(QString::number(m_tcpServer->serverPort()));

     connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(conection()));


    g_plugins->lanza ( "ServerSync_ServerSync_Post", this );
    _depura ( "END ServerSync::ServerSync", 0 );
}

ServerSync::~ServerSync()
{
    _depura ( "ServerSync::~ServerSync", 0 );
    _depura ( "END ServerSync::~ServerSync", 0 );
}


void ServerSync::conection()
{
    _depura ( "ServerSync::conection", 0 );
    mui_plainText->setPlainText("hola conectado\n");
    _depura ( "END ServerSync::conection", 0 );
}
