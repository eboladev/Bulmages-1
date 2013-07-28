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

#ifndef BLSYNC_H
#define BLSYNC_H

#include <QtWidgets/QDialog>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSslError>
#include <QtCore/QFile>
#include "ui_blsyncbase.h"
#include "blfunctions.h"

/** Sobrecarga los QFile para poder hacer busquedas por internet */
class BL_EXPORT BlSync :  public QDialog, public Ui_BlSyncBase
{
    Q_OBJECT
private:
    QString m_destfile;
public:
    BlSync ( QWidget *parent, const char *name );
    ~BlSync();
    void sync();
    void getFile(const QString &, const QString &) ;
    virtual int exec();

public slots:
  virtual void replyFinished(QNetworkReply *);
  virtual void slotSslErrors(QList<QSslError>);
  virtual void slotError(QNetworkReply::NetworkError);
  virtual void slotReadyRead();
  virtual void downloadProgress(qint64 ,qint64 );
};

#endif