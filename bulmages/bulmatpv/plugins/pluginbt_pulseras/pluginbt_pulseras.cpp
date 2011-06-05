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

#include <QHBoxLayout>

#define ARTICULO_FRACCION_HORA "32"


BtCompany *g_emp;

///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    blDebug ( "pluginbt_pulseras::entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbt_pulseras", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    blDebug ( "END pluginbt_pulseras::entryPoint", 0 );
    return 0;
}

///
/**
\return
**/
int exitPoint ( BtBulmaTPV *tpv )
{
    blDebug ( "pluginbt_pulseras::exitPoint", 0 );
    blDebug ( "END pluginbt_pulseras::exitPoint", 0 );
    return 0;
}


int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{

    /// Asignamos la variable global para usarla donde haga falta.
    g_emp = etpv;
  
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
  
    QString nomticket = g_emp->ticketActual() -> dbValue("nomticket"); 
    QString entrada = g_emp->valorBtInput();
    if (nomticket != "") {
	if (toolbutton->objectName() == "mui_pulseramas") {
	    /// Agregar pulsera al ticket actual.
	    BlDbRecord *lineaticket = g_emp->ticketActual()->insertarArticulo ( ARTICULO_FRACCION_HORA, BlFixed("1"), TRUE );
	    Pulsera *pul = new Pulsera(g_emp->ticketActual(), entrada, lineaticket);
	    g_emp->ticketActual()->pintar();
	    /// Borra el valor del Input.
	    g_emp->pulsaTecla ( Qt::Key_F4, "" );
	    return 1;
	} else if (toolbutton->objectName() == "mui_pulseraint") {
	    for ( int i = 0; i < g_pulseras.size(); ++i ) {
	       Pulsera * pul = g_pulseras.at(i);
	       if (pul->m_nombrepulsera == entrada) {
		    BtTicket * ticket = pul->m_ticketpulsera;
		    g_emp ->ticketActual()->setDbValue("bloqueadoticket", "FALSE");
		    g_emp ->setTicketActual ( ticket );
		    ticket->setDbValue("bloqueadoticket", "TRUE");
		    ticket->pintar();
		    /// Borra el valor del Input.
		    g_emp->pulsaTecla ( Qt::Key_F4, "" );
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
   BtTicket *ticketActual = g_emp->ticketActual();
   
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
    /// Facturamos todas las pulseras del ticket
    for ( int i = 0; i < g_pulseras.size(); ++i ) {
	Pulsera * pul = g_pulseras.at(i);
	if (pul->m_ticketpulsera == tick) {
	    int fracciones15minutos = (pul->m_horainicial.secsTo(QDateTime::currentDateTime ()) + 1) / (60 * 15) + 1;
	    pul->m_lineaticket->setDbValue("cantlalbaran", QString::number(fracciones15minutos));	    
	    pul->m_lineaticket->setDbValue("desclalbaran", "p."+pul->m_nombrepulsera +" -- "+ pul->m_horainicial.toString("h:m") + "-" + QDateTime::currentDateTime().toString("h:m"));
	    pul->m_lineaticket->setDbValue("nomarticulo", "p."+pul->m_nombrepulsera +" -- "+ pul->m_horainicial.toString("h:m") + "-" + QDateTime::currentDateTime().toString("h:m") );
	} // end if
    } // end for  
}


int BtTicket_borrarArticulo(BtTicket *tick) {
    /// Facturamos todas las pulseras del ticket
    for ( int i = 0; i < g_pulseras.size(); ++i ) {
	Pulsera * pul = g_pulseras.at(i);
	if (pul->m_lineaticket == tick->lineaActBtTicket()) {
	   delete pul;
	} // end if
    } // end for  
}
