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

 
 
#include "visoralarm.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "bldockwidget.h"
#include "blplugins.h"

#define CREAVARIABLESWEB
#ifdef CREAVARIABLESWEB
#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebPage>
#include <QtWebKitWidgets/QWebFrame>
#endif
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>
#include <QtCore/QUrlQuery>
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

}



/** Inicializacion de la clase.
    En el constructor del corrector no se hacen tareas especiales. */
/**
\param parent
\param fl
**/
visoralarm::visoralarm ( QWidget* parent, Qt::WindowFlags fl )
        : BlWidget ( parent, fl )
{
    BL_FUNC_DEBUG
    setupUi ( this );

    BlDockWidget *par = (BlDockWidget *) parent;
    mui_browser->page()->setLinkDelegationPolicy ( QWebPage::DelegateAllLinks );

}


void visoralarm::on_mui_browser_linkClicked ( const QUrl & url  ) {
  BL_FUNC_DEBUG
    QUrlQuery uquery(url);
    
    QString op = uquery.queryItemValue("op");
    if (op == "borrar") {
      QString query = "DELETE FROM alarma WHERE idalarma = " + uquery.queryItemValue("idalarma");
      mainCompany()->runQuery(query);
      presentar();
    } else if (op == "abrir") {
        QString query = "SELECT * FROM alarma WHERE idalarma = " + uquery.queryItemValue("idalarma");
	BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
	if ( ! cur->eof() ) {
	    QString tabla = "open_"+cur->value("fichaalarma");
//	    QString id = cur->value("idfichaalarma");
	    /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
//	    g_plugParams = &id;
//	    int resur = g_plugins->run ( tabla.toLatin1().constData(), ( BlMainCompany * ) mainCompany() );
	    QString id = cur->value("fichaalarma") + "_" + cur->value("idfichaalarma");
	    g_plugParams = &id;
	    int resur = g_plugins->run ("Plugin_open", ( BlMainCompany * ) mainCompany());
	} // end if
	delete cur;
    } else {
      QString query = "UPDATE alarma SET fechaalarma = fechaalarma + '@1day'::interval WHERE idalarma = " + uquery.queryItemValue("idalarma");
      mainCompany()->runQuery(query);
      presentar();
    } // end if
    
}

void visoralarm::sloadFinished ( bool ok  ) {

}

void visoralarm::on_mui_browser_loadFinished ( bool ok  ) {
    mui_browser->page()->setLinkDelegationPolicy ( QWebPage::DelegateAllLinks );
}

void visoralarm::presentar() {
    BL_FUNC_DEBUG
    textBrowser = "<HTML><BODY BGCOLOR='#FFFFFF'>";
    QString cadena;

    QString query = "SELECT *, (fechaalarma + horaalarma <= now()) AS expirada FROM alarma ORDER BY fechaalarma, horaalarma";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
    while ( ! cur->eof() ) {
        if (cur->value("expirada") == "f") {
	#ifdef Q_OS_WIN32
	    QString cupath = QDir::currentPath().replace("program", "").replace(".bulmages","");
	    QString src= g_confpr->value( CONF_PROGDATA).replace("..",cupath);
	    cadena = "<img src='file:///" + src +"icons/chrono.png'>&nbsp;&nbsp;  <BR> " +cur->value( "fechaalarma" ) +" " + cur->value( "horaalarma" )  +" <br><b>" +cur->value( "subjectalarma" ) +"</b><BR>" + cur->value("textoalarma");
	#else
	    cadena = "<img src='file://" + g_confpr->value( CONF_PROGDATA ) +"icons/chrono.png'>&nbsp;&nbsp;  <BR> " +cur->value( "fechaalarma" ) +" " + cur->value( "horaalarma" )  +" <br><b>" +cur->value( "subjectalarma" ) +"</b><BR>" + cur->value("textoalarma");
	#endif

	} else {
	#ifdef Q_OS_WIN32
	    QString cupath = QDir::currentPath().replace("program", "").replace(".bulmages","");
	    QString src= g_confpr->value( CONF_PROGDATA).replace("..",cupath);
	  cadena = "<br><img src='file:///" + src +"icons/chrono.png'>&nbsp;&nbsp;  <BR> " +" <font color='red'>" + cur->value( "fechaalarma" )  + cur->value( "horaalarma" )  +" <br><b>" +cur->value( "subjectalarma" ) +"</b><BR>" + cur->value("textoalarma") + " </font>";
		
	#else
	  cadena = "<br><img src='file://" + g_confpr->value( CONF_PROGDATA ) +"icons/chrono.png'>&nbsp;&nbsp;  <BR> " +" <font color='red'>" + cur->value( "fechaalarma" )  + cur->value( "horaalarma" )  +" <br><b>" +cur->value( "subjectalarma" ) +"</b><BR>" + cur->value("textoalarma") + " </font>";
	#endif

	} // end if
        agregarError ( cadena, "alarma", cur->value( "idalarma" ) );
        cur->nextRecord();
    } // end while
    delete cur;

    
    textBrowser += "</BODY></HTML>";
    mui_browser->setHtml ( textBrowser );

    QTimer::singleShot(10000, this, SLOT(presentar()));
}


/** Se encarga de agregar un error en el browser que los presenta.
    Sirve para para introducir los errores hayados en el corrector y que se vean por
    pantalla. */
/**
\param texto
\param texto1
\param texto2
**/
void visoralarm::agregarError ( QString texto, QString texto1, QString texto2 )
{
    BL_FUNC_DEBUG
    textBrowser += "<HR><table><tr><td colspan=2>" + texto + "</td></tr><tr><td><a name='masinfo' href='abredoc?op=posponer&tabla="+texto1+"&idalarma=" + texto2 + "'>Posponer Alarma</a></td><td><a name='" + texto1 + "' href='abredoc?op=borrar&tabla="+texto1+"&idalarma=" + texto2 + "'>Borrar</a></td></tr><tr><td colspan='2'><a name='abredoc' href='abredoc?op=abrir&tabla="+texto1+"&idalarma=" + texto2 + "'>Abrir Documento</a></td></tr></table>";
    
}


///
/**
**/
visoralarm::~visoralarm()
{
    BL_FUNC_DEBUG
}



