/***************************************************************************
 *   Copyright (C) 2013 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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
#include <QtNetwork/QNetworkAccessManager>
#include <QtCore/QFileInfo>

#include "blfunctions.h"
#include "blconfiguration.h"
#include "blfile.h"
#include "blmainwindow.h"
#include "blmaincompany.h"

QNetworkAccessManager *manager;


BlFile::BlFile() : QFile () {
  BL_FUNC_DEBUG
}

BlFile::BlFile(QString file) : QFile (file) {
  BL_FUNC_DEBUG
  m_file = file;
}

void BlFile::sync() {
  BL_FUNC_DEBUG 
  
  /// Esta clase esta clausurada
  return;

//  if (!exists() {
  
      QFileInfo fileInfo(m_file);
      QString filename(fileInfo.fileName());
      
      QString user = g_confpr->value(CONF_LOGIN_USER);
      QString dbname = g_confpr->value(CONF_DBNAME);
    #ifdef Q_OS_WIN32
      QString platform = "MS_WIN";
    #else
      QString platform = "LINUX";
    #endif
      
      QString url = "http://www.bulmages.com/bulmaincloud/"+platform+"/"+user+"/"+dbname+"/"+filename;
      fprintf(stderr, "Iniciando descarga %s\n", url.toLatin1().constData());
      
      manager = new QNetworkAccessManager(this);
      QNetworkRequest request;
      request.setUrl(QUrl(url));
      request.setRawHeader("User-Agent", "BgBrowser 1.0");
    
      QNetworkReply *reply = manager->get(request);
      connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
      connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));

      connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
	    this, SLOT(slotError(QNetworkReply::NetworkError)));
      connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
	    this, SLOT(slotSslErrors(QList<QSslError>)));
      connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));

      while (reply->isRunning()) {
	QApplication::processEvents();
      } // end while
 
//  }// end if
 
      url = "http://www.bulmages.com/bulmaincloud/ALL/"+user+"/"+dbname+"/"+filename;
      fprintf(stderr, "Iniciando descarga %s\n", url.toLatin1().constData());
      request.setUrl(QUrl(url));
      reply = manager->get(request);
      while (reply->isRunning()) {
	QApplication::processEvents();
      } // end while
      if (reply->error() == QNetworkReply::NoError) {
	return;
      } // end if

      url = "http://www.bulmages.com/bulmaincloud/ALL/ALL/"+filename;
      fprintf(stderr, "Iniciando descarga %s\n", url.toLatin1().constData());
      request.setUrl(QUrl(url));
      reply = manager->get(request);
      while (reply->isRunning()) {
	QApplication::processEvents();
      } // end while
    
}

void 	BlFile::setFileName(const QString & name) {
  BL_FUNC_DEBUG
  m_file = name;
  sync();
  QFile::setFileName(name);
}

BlFile::~BlFile() {
}

void BlFile::slotSslErrors(QList<QSslError>) {
  fprintf(stderr, "Error de autentificacion descargando %s\n", fileName().toLatin1().constData());
}

void BlFile::slotError(QNetworkReply::NetworkError) {
  fprintf(stderr,"Error descargando %s\n", fileName().toLatin1().constData());
}

void BlFile::slotReadyRead() {

}



void BlFile::replyFinished(QNetworkReply * reply) {
  
  if (reply->error() == QNetworkReply::NoError) {
      
      fprintf(stderr,"Descarga completa %s\n", m_file.toLatin1().constData());
      
      QFile localFile(m_file);
      if (!localFile.open(QIODevice::WriteOnly)) {
	  fprintf(stderr,"Error en la escritura del archivo %s\n", m_file.toLatin1().constData());
	  return;
      } // end if
      const QByteArray sdata = reply->readAll();
      localFile.write(sdata);
      localFile.close();
      fprintf(stderr, "Archivo escrito %s \n %s\n", m_file.toLatin1().constData(), sdata.data());
  } else {
      fprintf(stderr,"Error descargando %s\n", fileName().toLatin1().constData());
  }
  reply->deleteLater();
}

void BlFile::downloadProgress(qint64 a,qint64 b) {
  fprintf(stderr,"Descarga en proceso\n");
}