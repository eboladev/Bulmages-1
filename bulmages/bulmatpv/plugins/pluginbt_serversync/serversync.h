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

#ifndef SERVERSYNC_H
#define SERVERSYNC_H

#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>
 #include <QtNetwork/QTcpServer>

#include "btticket.h"
#include "btcompany.h"
#include "ui_serversyncbase.h"

class QTcpServer;

class ServerSync : public BlWidget, public Ui_ServerSyncBase {
Q_OBJECT

public:
    QTcpServer *m_tcpServer;
    QList <QTcpSocket *> m_listaSockets;
    
public:
    ServerSync ( BtCompany *emp, QWidget *parent );
    virtual ~ServerSync();
    virtual void send(const QString &);

  public slots:
    virtual void conection();
    virtual void readyRead();
    virtual void readChannelFinished();
};

#endif
