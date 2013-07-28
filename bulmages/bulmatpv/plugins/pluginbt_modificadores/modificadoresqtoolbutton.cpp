/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#include <QtWidgets/QWidget>

/// Necesarios para importacion de efactura
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNode>
#include <QtCore/QString>
#include <QtWidgets/QFileDialog>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QBuffer>


#include "tabletcanvas.h"
#include "modificadoresqtoolbutton.h"
#include "blfunctions.h"
#include "modificadores.h"
#include "mticketivainc.h"

//extern TabletCanvas *g_tablet;

///
/**
\param cob
\param parent
**/
ModificadoresQToolButton::ModificadoresQToolButton ( BtCompany * emp, QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
    m_companyact = emp;
    setBoton();
    
}


///
/**
**/
ModificadoresQToolButton::~ModificadoresQToolButton()
{
    BL_FUNC_DEBUG
}


///
/**
**/
void ModificadoresQToolButton::setBoton()
{
    BL_FUNC_DEBUG
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( _("Generar Q19") );
    setToolTip ( _("Generar archivo Q19 de los elementos seleccionados") );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/q19.png" ) );
    setIconSize ( QSize ( 22, 22 ) );
    
}


///
/**
\return
**/
void ModificadoresQToolButton::click()
{
    BL_FUNC_DEBUG

    /// Solo muestra la ventana de modificadores si se ha seleccionado una linea del ticket que se
    /// pueda modificar.
    if (! ( ( BtCompany * ) m_companyact )->ticketActual()->lineaActBtTicket() ) {
	blMsgWarning(_("Primero seleccione una linea del ticket."));
    } else {
	BtTicket *ticket = ( ( BtCompany * ) m_companyact )->ticketActual();
	Modificadores * trab = new Modificadores ( m_companyact, 0 );
	trab->exec();
	ticket->pintar();      
    } // end if
    
    

}




/// =========================================
/// =========================================

///
/**
\param cob
\param parent
**/
MTabletQToolButton::MTabletQToolButton ( BtCompany * emp, QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
    m_companyact = emp;
    setBoton();
    
}


///
/**
**/
MTabletQToolButton::~MTabletQToolButton()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void MTabletQToolButton::setBoton()
{
    BL_FUNC_DEBUG
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( _("Generar Q19") );
    setToolTip ( _("Generar archivo Q19 de los elementos seleccionados") );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/q19.png" ) );
    setIconSize ( QSize ( 22, 22 ) );
    
}


///
/**
\return
**/
void MTabletQToolButton::click()
{
    BL_FUNC_DEBUG
    /*
	  BtTicket *ticket = ( ( BtCompany * ) m_companyact )->ticketActual();
	  if (g_tablet->m_vacio == true) {
	      ticket->lineaActBtTicket()->setDbValue ( "imglalbaran", "" );
	  } else {
	      QByteArray bytes;
	      QBuffer buffer(&bytes);
	      buffer.open(QIODevice::WriteOnly);
	      g_tablet->pixmap.save(&buffer, "PNG");
	      QString text = bytes.toBase64();
	      ticket->lineaActBtTicket()->setDbValue ( "imglalbaran", text );
	  } // end if
	 ticket->pintar();
    */
    

}

