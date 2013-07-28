/***************************************************************************
 *   Copyright (C) 2013 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#ifndef BLFILE_H
#define BLFILE_H

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSslError>
#include <QtCore/QFile>

#include "blfunctions.h"



/** Sobrecarga los QFile para poder hacer busquedas por internet */
class BL_EXPORT  BlFile : public QFile
{
    Q_OBJECT
private:
    QString m_file;
public:
    BlFile ( );
    BlFile (QString );
    ~BlFile();
    void sync();
    virtual void setFileName(const QString &);
public slots:
  virtual void replyFinished(QNetworkReply *);
  virtual void slotSslErrors(QList<QSslError>);
  virtual void slotError(QNetworkReply::NetworkError);
  virtual void slotReadyRead();
  virtual void downloadProgress(qint64 ,qint64 );
};

#endif