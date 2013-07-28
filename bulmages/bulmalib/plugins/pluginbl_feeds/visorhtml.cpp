/***************************************************************************
 *   Copyright (C) 2011 by Tomeu Borras Riera                              *
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

 
 
#include "visorhtml.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "bldockwidget.h"

#define CREAVARIABLESWEB
#ifdef CREAVARIABLESWEB
#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebPage>
#include <QtWebKitWidgets/QWebFrame>
#endif
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>
#include <QtWidgets/QDockWidget>
#include <QtNetwork/QNetworkAccessManager>
#include <QtCore/QEventLoop>
#include <QtWidgets/QFileDialog>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QObject>

// Claves para el array de licencia
#define USUARIO 0
#define EMPRESA 1
#define LICENCIA 2
#define EJECUCIONES 3
#define EMAIL 4

#ifdef CREAVARIABLESWEB
QWebView *g_webViewModal = NULL;
QString g_valores[10];
QString g_retorno = "";
#endif


/// Funcion auxiliar
void download(const QUrl &url, const QString &filename)
{
    blMsgInfo("La descarga se efectuara en segundo plano. Una vez descargado el archivo aparecera en el destino");

    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    loop.exec();

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    file.write(reply->readAll());

    delete reply;
}



/** Inicializacion de la clase.
    En el constructor del corrector no se hacen tareas especiales. */
/**
\param parent
\param fl
**/
visorhtml::visorhtml ( QWidget* parent, Qt::WindowFlags fl )
        : QWidget ( parent, fl )
{
    BlDebug::blDebug ( "visorhtml::visorhtml", 0 );
    setupUi ( this );

    BlDockWidget *par = (BlDockWidget *) parent;
	
	webView->load( QUrl( g_confpr->value(CONF_URL_CONTENTS) ));


	webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    BlDebug::blDebug ( "END visorhtml::visorhtml", 0 );
}


void visorhtml::on_webView_linkClicked ( const QUrl & url  ) {
  if (url.path().contains("pdf") || url.path().contains("exe")) {
		 QString fileName = QFileDialog::getSaveFileName(this, "Save File", url.path(),  "*");
		 download(url, fileName);
  } else {
    webView->load(url);
  } // end if
  webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
}

void visorhtml::sloadFinished ( bool ok  ) {

}

void visorhtml::on_webView_loadFinished ( bool ok  ) {

}



///
/**
**/
visorhtml::~visorhtml()
{
    BlDebug::blDebug ( "visorhtml::~visorhtml", 0 );
    BlDebug::blDebug ( "END visorhtml::~visorhtml", 0 );
}



