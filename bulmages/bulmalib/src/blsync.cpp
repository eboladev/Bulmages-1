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
#include "blsync.h"
#include "blmainwindow.h"
#include "blmaincompany.h"

QNetworkAccessManager *syncManager;


BlSync::BlSync ( QWidget *parent, const char *name ) : QDialog ( parent ) {
  BL_FUNC_DEBUG
    setWindowTitle ( name );
    setupUi ( this );
}



void BlSync::sync() {
      BL_FUNC_DEBUG  
  
      QString user = g_confpr->value(CONF_LOGIN_USER);
      QString dbname = g_confpr->value(CONF_DBNAME);
    #ifdef Q_OS_WIN32
      QString platform = "MS_WIN";
    #else
      QString platform = "LINUX";
    #endif
      
      bool reinicio = false;
      
      
      // Cogemos la lista de ficheros
      
      QString url = "http://www.bulmages.com/bulmaincloud/files.txt";
      
      syncManager = new QNetworkAccessManager(this);
      connect(syncManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
      
      getFile("files.txt", g_confpr->value(CONF_DIR_USER));

      QFile file ( g_confpr->value(CONF_DIR_USER)+"files.txt" );
      /// If not exists return.
      if ( !file.open ( QIODevice::ReadOnly ) ) {
	  fprintf(stderr,"No se pudo abrir archivo de indices\n");
	  return;
      } // end if
      QTextStream in(&file);
      QString line = in.readLine();
      while (!line.isNull()) {
	  QStringList arch = line.split(" ");
	  QString filename = arch[0];
	  QString dest = arch[1];
	  QString date= arch[2];
	  
	  bool descarga = true;
	  
	  QFile file1 ( g_confpr->value(CONF_DIR_USER)+"lfiles.txt");
	  if (file1.open(QIODevice::ReadOnly)) {
	      QTextStream lin(&file1);
	      QString lline = lin.readLine();
	      while (!lline.isNull()) {
		  QStringList larch = lline.split(" ");
		  if (filename == larch[0] && date == larch[2]) {
		     descarga = false;
		     break;
		  } // end if
		  lline = lin.readLine();
	      } // end while
	      file1.close();
	  } // end if
	  if (descarga) {
	      dest.replace("[CONF_DIR_OPENREPORTS]", g_confpr->value(CONF_DIR_OPENREPORTS));
	      dest.replace("[CONFIG_DIR_CONFIG]", CONFIG_DIR_CONFIG);
	      getFile(filename, dest);
	      if (arch[3] == "T") {
		reinicio = true;
	      }// end if
	  } // end if
	  line = in.readLine();
      } // end while
 
      blCopyFile(g_confpr->value(CONF_DIR_USER)+"files.txt", g_confpr->value(CONF_DIR_USER)+"lfiles.txt");
      
/// Configuraciones personalizadas
      url = "http://www.bulmages.com/bulmaincloud/filesp.txt";
      
      getFile("filesp.txt", g_confpr->value(CONF_DIR_USER));

      QFile pfile ( g_confpr->value(CONF_DIR_USER)+"filesp.txt" );
      /// If not exists return.
      if ( !pfile.open ( QIODevice::ReadOnly ) ) {
	  fprintf(stderr,"No se pudo abrir archivo de indices\n");
	  return;
      } // end if
      QTextStream pin(&pfile);
      QString pline = pin.readLine();
      while (!pline.isNull()) {
	  QStringList arch = pline.split(" ");
	  QString filename = arch[0];
	  QString dest = arch[1];
	  QString date= arch[2];
	  
	  bool descarga = true;
	  
	  QFile file1 ( g_confpr->value(CONF_DIR_USER)+"lfilesp.txt");
	  if (file1.open(QIODevice::ReadOnly)) {
	      QTextStream lin(&file1);
	      QString lline = lin.readLine();
	      while (!lline.isNull()) {
		  QStringList larch = lline.split(" ");
		  if (filename == larch[0] && date == larch[2]) {
		     descarga = false;
		     break;
		  } // end if
		  lline = lin.readLine();
	      } // end while
	      file1.close();
	  } // end if
	  if (descarga) {
	      dest.replace("[CONF_DIR_OPENREPORTS]", g_confpr->value(CONF_DIR_OPENREPORTS));
	      dest.replace("[CONFIG_DIR_CONFIG]", CONFIG_DIR_CONFIG);
	      getFile(filename, dest);
	      if (arch[3] == "T") {
		reinicio = true;
	      }// end if
	  } // end if
	  pline = pin.readLine();
      } // end while
 
      blCopyFile(g_confpr->value(CONF_DIR_USER)+"filesp.txt", g_confpr->value(CONF_DIR_USER)+"lfilesp.txt");
      
      if (reinicio) {
	blMsgInfo("Debe reiniciar el programa para que los cambios surjan efecto");
	exit(1);
      } // end if
}

void BlSync::getFile(const QString & name, const QString & dest) {
  BL_FUNC_DEBUG
  
  
      m_destfile = dest+name;

      QString user = g_confpr->value(CONF_LOGIN_USER);
      QString dbname = g_confpr->value(CONF_DBNAME);
    #ifdef Q_OS_WIN32
      QString platform = "MS_WIN";
    #else
      QString platform = "LINUX";
    #endif
  
      QString url = "http://www.bulmages.com/bulmaincloud/"+platform+"/"+dbname+"/"+name;
      QNetworkRequest request;
      request.setUrl(QUrl(url));
      request.setRawHeader("User-Agent", "BgBrowser 1.0");
    

      
      QNetworkReply *reply = syncManager->get(request);
      
/*      
      connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
      connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
      connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
	    this, SLOT(slotError(QNetworkReply::NetworkError)));
      connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
	    this, SLOT(slotSslErrors(QList<QSslError>)));
*/

      fprintf(stderr, "Iniciando descarga %s\n", url.toLatin1().constData());
      request.setUrl(QUrl(url));
      reply = syncManager->get(request);
      while (reply->isRunning()) {
	QApplication::processEvents();
      } // end while
      
      if (reply->error() == QNetworkReply::NoError) {
	  delete reply;
	  return;
      } // end if
      delete reply;
      
      
      
      
      url = "http://www.bulmages.com/bulmaincloud/ALL/"+dbname+"/"+name;
      request.setUrl(QUrl(url));
      reply = syncManager->get(request);
      while (reply->isRunning()) {
	QApplication::processEvents();
      } // end while
      
      if (reply->error() == QNetworkReply::NoError) {
	  delete reply;
	  return;
      } // end if
      delete reply;
      
      url = "http://www.bulmages.com/bulmaincloud/"+platform+"/ALL/"+name;
      request.setUrl(QUrl(url));
      reply = syncManager->get(request);
      while (reply->isRunning()) {
	QApplication::processEvents();
      } // end while
      
      if (reply->error() == QNetworkReply::NoError) {
	  delete reply;
	  return;
      } // end if
      delete reply;
      
      
      
      url = "http://www.bulmages.com/bulmaincloud/ALL/ALL/"+name;
      request.setUrl(QUrl(url));
      reply = syncManager->get(request);
      while (reply->isRunning()) {
	QApplication::processEvents();
      } // end while
      
      if (reply->error() == QNetworkReply::NoError) {
	  delete reply;
	  return;
      } // end if
      delete reply;

}




BlSync::~BlSync() {
}

void BlSync::slotSslErrors(QList<QSslError>) {
//  fprintf(stderr, "Error de autentificacion descargando %s\n", fileName().toLatin1().constData());
}

void BlSync::slotError(QNetworkReply::NetworkError) {
//  fprintf(stderr,"Error descargando %s\n", fileName().toLatin1().constData());
}

void BlSync::slotReadyRead() {

}



void BlSync::replyFinished(QNetworkReply * reply) {
  
  if (reply->error() == QNetworkReply::NoError) {
      
      fprintf(stderr,"Descarga completa %s\n", m_file.toLatin1().constData());
      
      QFile localFile(m_destfile);
      if (!localFile.open(QIODevice::WriteOnly)) {
	  fprintf(stderr,"Error en la escritura del archivo %s\n", m_destfile.toLatin1().constData());
	  return;
      } // end if
      const QByteArray sdata = reply->readAll();
      localFile.write(sdata);
      localFile.close();
      fprintf(stderr, "Archivo escrito %s \n %s\n", m_destfile.toLatin1().constData(), sdata.data());
  } else {
      fprintf(stderr,"Error descargando %s\n", m_destfile.toLatin1().constData());
  }
  //reply->deleteLater();
}

void BlSync::downloadProgress(qint64 a,qint64 b) {
  fprintf(stderr,"Descarga en proceso\n");
}

int BlSync::exec() {
  BL_FUNC_DEBUG
  sync();
}