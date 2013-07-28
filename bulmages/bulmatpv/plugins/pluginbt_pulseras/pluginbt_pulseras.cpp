/***************************************************************************
 *   Copyright (C) 2010 by Tomeu Borras Riera                              *
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

#include "pluginbt_pulseras.h"
#include "btcompany.h"
#include "blplugins.h"
#include "btticket.h"
#include "bldockwidget.h"
#include "blapplication.h"
#include "bltoolbutton.h"
#include "blfunctions.h"
#include "bldb.h"


#include "pulsera.h"

#include <QtWidgets/QHBoxLayout>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>

#define ARTICULO_HORA_COMPLETA "36"
#define ARTICULO_HORA_FRACCION "37"
#define MINUTOS_INICIALES 60
#define MINUTOS_FRACCION 15


BtCompany *g_pluginbt_pulseras_emp;

///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbt_pulseras", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    
    return 0;
}

///
/**
\return
**/
int exitPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG
    
    return 0;
}


int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{
    BL_FUNC_DEBUG
    /// Asignamos la variable global para usarla donde haga falta.
    g_pluginbt_pulseras_emp = etpv;
  
    BlToolButton *boton = new BlToolButton(etpv, etpv);
    boton->setObjectName(QString::fromUtf8("mui_pulseramas"));
    boton->setMinimumSize(QSize(72, 72));
    boton->setMaximumSize(QSize(200, 72));
    boton->setFocusPolicy(Qt::NoFocus);
    boton->setIcon(QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/pulseraplus.png"  ));
    boton->setIconSize(QSize(32, 32));
    boton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    boton->setText(N_("Agregar Pulsera", 0));
    
    BlToolButton *boton2 = new BlToolButton(etpv, etpv);
    boton2->setObjectName(QString::fromUtf8("mui_pulseraint"));
    boton2->setMinimumSize(QSize(72, 72));
    boton2->setMaximumSize(QSize(200, 72));
    boton2->setFocusPolicy(Qt::NoFocus);
    boton2->setIcon(QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/pulseraint.png"  ));
    boton2->setIconSize(QSize(32, 32));
    boton2->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    boton2->setText(N_("Buscar Pulsera", 0));
    
    /// Colocamos los botones en la botonera.
    QFrame *fr = g_main->findChild<QFrame *> ( "mui_frameabrevs" );
    if ( fr ) {
        QHBoxLayout *m_hboxLayout1 = fr->findChild<QHBoxLayout *> ( "hboxLayout1" );
        if ( !m_hboxLayout1 ) {
            m_hboxLayout1 = new QHBoxLayout ( fr );
            m_hboxLayout1->setSpacing ( 5 );
            m_hboxLayout1->setMargin ( 5 );
            m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
        } // end if
        m_hboxLayout1->addWidget ( boton );
        m_hboxLayout1->addWidget ( boton2 );
    } // end if

    return 0;
}


int BlToolButton_released(BlToolButton *toolbutton) {
    BL_FUNC_DEBUG
    QString nomticket = g_pluginbt_pulseras_emp->ticketActual() -> dbValue("nomticket"); 
    QString entrada = g_pluginbt_pulseras_emp->valorBtInput();
    if (nomticket != "") {
	if (toolbutton->objectName() == "mui_pulseramas") {
	    /// Agregar pulsera al ticket actual.
	    BlDbRecord *lineaticket = g_pluginbt_pulseras_emp->ticketActual()->insertarArticulo ( ARTICULO_HORA_COMPLETA, BlFixed("1"), true );
	    Pulsera *pul = new Pulsera(g_pluginbt_pulseras_emp->ticketActual(), entrada, lineaticket);
	    fprintf(stderr,"Pulsera Agregada, voy a pintar \n");
	    g_pluginbt_pulseras_emp->ticketActual()->pintar();
	    /// Borra el valor del Input.
	    g_pluginbt_pulseras_emp->pulsaTecla ( Qt::Key_F4, "" );
	    return 1;
	} else if (toolbutton->objectName() == "mui_pulseraint") {
	    for ( int i = 0; i < g_pulseras.size(); ++i ) {
	       Pulsera * pul = g_pulseras.at(i);
	       if (pul->m_nombrepulsera == entrada) {
		    BtTicket * ticket = pul->m_ticketpulsera;
		    g_pluginbt_pulseras_emp ->ticketActual()->setDbValue("bloqueadoticket", "false");
		    g_pluginbt_pulseras_emp ->setTicketActual ( ticket );
		    ticket->setDbValue("bloqueadoticket", "true");
		    ticket->pintar();
		    /// Borra el valor del Input.
		    g_pluginbt_pulseras_emp->pulsaTecla ( Qt::Key_F4, "" );
	       } // end if
	    } // end for
	    return 1;
	} else if (toolbutton->objectName() == "mui_pulserarayo") {
	    return 1;
	} // end if
    } // end if
    return 0;
} // end if


int BtCompany_cobrar(BtCompany *emp) {
    BL_FUNC_DEBUG
   BtTicket *ticketActual = g_pluginbt_pulseras_emp->ticketActual();
   
   QList <Pulsera *>pulserasaborrar;
   
   ticketActual->pintar();
    /// Facturamos todas las pulseras del ticket
    for ( int i = 0; i < g_pulseras.size(); ++i ) {
	Pulsera * pul = g_pulseras.at(i);
	if (pul->m_ticketpulsera == ticketActual) {
	    pulserasaborrar.append(pul);
	} // end if
    } // end for
    
    /// Eliminamos todas las pulseras del ticket ya que ya han sido facturadas
    for ( int i = 0; i < pulserasaborrar.size(); ++i ) {
	Pulsera * pul = pulserasaborrar.at(i);
	delete pul;
    } // end for
  
    
  return 0;
}


int BtTicket_pintar(BtTicket *tick) {
    BL_FUNC_DEBUG
    static int semaforo = 0;
  
    if (semaforo == 0) {
      	semaforo = 1;
	fprintf(stderr, "Pongo Semaforo\n");
	/// Facturamos todas las pulseras del ticket.
	for ( int i = 0; i < g_pulseras.size(); ++i ) {
	    Pulsera * pul = g_pulseras.at(i);
	    if (pul->m_ticketpulsera == tick) {
		if (!pul->m_lineaticket ) {
		    blMsgInfo("MENSAJE DE DEPURACION. Creo una linea de ticket que ya deberia existir.");
		    pul->m_lineaticket = tick->insertarArticulo ( ARTICULO_HORA_COMPLETA, BlFixed("1"), true );
		} // end if
		int minutos = (pul->m_horainicial.secsTo(QTime::currentTime ()) + 1) / 60;
		if (minutos < 0 ) minutos = minutos * -1; /* En el imposible caso de tiempos negativos se pone un absurdo */
		pul->m_lineaticket->setDbValue("desclalbaran", "p."+pul->m_nombrepulsera +" -- "+ pul->m_horainicial.toString("h:m") + "-" + QTime::currentTime().toString("h:m"));
		pul->m_lineaticket->setDbValue("nomarticulo", "p."+pul->m_nombrepulsera +" -- "+ pul->m_horainicial.toString("h:m") + "-" + QTime::currentTime().toString("h:m") );
		
		/// Superada la primera hora empiezan las fracciones de 15 minutos.
		if (minutos > MINUTOS_INICIALES && !pul->m_sinfracciones) {
		  if (!pul->m_lineaticketfraccion ) {
		    pul->m_lineaticketfraccion = tick->insertarArticulo ( ARTICULO_HORA_FRACCION, BlFixed("1"), true );
		  } // end if
		  int fraccionesminutos = (minutos - MINUTOS_INICIALES) / MINUTOS_FRACCION +1;
		  pul->m_lineaticketfraccion->setDbValue("cantlalbaran", QString::number(fraccionesminutos));	
		  pul->m_lineaticketfraccion->setDbValue("desclalbaran", "fp."+pul->m_nombrepulsera +" -- "+ pul->m_horainicial.toString("h:m") + "-" + QTime::currentTime().toString("h:m"));
		  pul->m_lineaticketfraccion->setDbValue("nomarticulo", "fp."+pul->m_nombrepulsera +" -- "+ pul->m_horainicial.toString("h:m") + "-" + QTime::currentTime().toString("h:m") );
		} // end if
	    } // end if
	} // end for  
	semaforo = 0;
	fprintf(stderr, "Quito Semaforo\n");
    } else {
	fprintf(stderr, "No se puede ejecutar por semaforo\n");
    } // end if

    
    return 0;
}


int BtTicket_borrarLinea(BtTicket *tick) {
    BL_FUNC_DEBUG
    /// Facturamos todas las pulseras del ticket
    int j= 0;
    while (j < g_pulseras.size()) {
	Pulsera * pul = g_pulseras.at(j);
	if (pul->m_lineaticketfraccion == tick->lineaActBtTicket()) {
	   pul->m_lineaticketfraccion = NULL;
	   pul->m_sinfracciones = true;
	} // end if
	if (pul->m_lineaticket == tick->lineaActBtTicket()) {
	   g_pulseras.removeAt(j);
	   delete pul;
	} else {
	   j ++;
	} // end if
    } // end while  
    return 0;
}


int BtTicket_exportXML_Post(BtTicket *tick) {
  BL_FUNC_DEBUG
  tick->m_textoXML += "\t<LISTAPULSERAS>\n";
  for (int i = 0; i < g_pulseras.size(); ++i) {
      Pulsera * pul = g_pulseras.at(i);
      if (pul->m_ticketpulsera == tick) {
	tick->m_textoXML += "\t\t<PULSERA>\n";
	tick->m_textoXML += "\t\t\t<NOMBREPULSERA>"+pul->m_nombrepulsera+"</NOMBREPULSERA>\n";
	tick->m_textoXML += "\t\t\t<HORAINICIALPULSERA>"+pul->m_horainicial.toString("h:m")+"</HORAINICIALPULSERA>\n";
	tick->m_textoXML += "\t\t\t<SINFRACCIONESPULSERA>"+(pul->m_sinfracciones? QString("true"):QString("false"))+"</SINFRACCIONESPULSERA>\n";
	tick->m_textoXML += "\t\t</PULSERA>\n";
      } // end if
  } // end for
  tick->m_textoXML += "\t</LISTAPULSERAS>\n";
  return 0;
}

int BtTicket_syncXML_Post(BtTicket *tick) {
    BL_FUNC_DEBUG
    QList<Pulsera *> pulserasusadas;
    QDomDocument doc ( "mydocument" );

    if ( !doc.setContent ( tick->m_textoXML ) ) {
	
        return 0;
    } // end if

    QDomElement docElem = doc.documentElement();
    QDomElement principal = docElem.firstChildElement ( "LISTAPULSERAS" );
    QDomNodeList nodos = principal.elementsByTagName ( "PULSERA" );
    
    int i = 0;
    while (i < nodos.count()) {
      
        QDomNode ventana = nodos.item ( i++ );
	QDomElement enombre = ventana.firstChildElement("NOMBREPULSERA");
	QDomElement ehora = ventana.firstChildElement("HORAINICIALPULSERA");
	QDomElement efracciones = ventana.firstChildElement("SINFRACCIONESPULSERA");
	
	bool encontrado = false;
	Pulsera * puls = NULL;
	
	/// Buscamos la pulsera correspondiente a esta.
	/// Si la encontramos no hacemos nada porque ya esta en el sistema y no requiere de accion alguna.
	for (int j = 0; j < g_pulseras.size(); ++j) {
	    Pulsera * pul = g_pulseras.at(j);
	    if (pul->m_ticketpulsera == tick) {
	      if (pul->m_nombrepulsera == enombre.text()) {
		 encontrado = true;
		 puls = pul;
	      } // end if
	    } // end if
	} // end for
	/// Si no la encontramos entonces si que hay que agregarla, pero seguramente ya tenga sus lineas correspondientes.
	BlDbRecord *lineaticket = NULL;
	BlDbRecord *lineafticket = NULL;
	for ( int j = 0; j < tick->listaLineas()->size(); ++j ) {
		BlDbRecord *item = tick->listaLineas()->at ( j );
		if (item->dbValue ( "desclalbaran" ).startsWith ( "p."+enombre.text() +" -- "+ ehora.text() + "-" )) {
			fprintf(stderr,"Encontrada la linea de ticket para la pulsera \n");
			lineaticket = item;
		} // end if
		if (item->dbValue ( "desclalbaran" ).startsWith ( "fp."+enombre.text() +" -- "+ ehora.text() + "-" )) {
			fprintf(stderr,"Encontrada la linea de parciales para la pulsera \n");
			lineafticket = item;
		} // end if
	} // end for
	if (! encontrado) {
	    puls = new Pulsera(tick, enombre.text(), lineaticket);
	}  // end if
	puls->m_lineaticket = lineaticket;
	puls->m_lineaticketfraccion = lineafticket;
	puls->m_horainicial = QTime::fromString(ehora.text(), "h:m");
	puls->m_sinfracciones = (efracciones.text() == "true");
	pulserasusadas.append(puls);

    } // end while


    /// Borramos todas las pulseras del sistema que no se han sincronizado porque significa que las han borrado en otro lado.

    int j=0;
    while (j < g_pulseras.size()) {
	Pulsera * pul = g_pulseras.at(j);
	if (pul->m_ticketpulsera == tick) {
	  if (!pulserasusadas.contains(pul)) {
	      g_pulseras.removeAt(j);
	      delete pul;
	  } else  {
	      j++;
	  } // end if
	} else {
	  j++;
	} // end if
	
    } // end while

    return 0;
} 



