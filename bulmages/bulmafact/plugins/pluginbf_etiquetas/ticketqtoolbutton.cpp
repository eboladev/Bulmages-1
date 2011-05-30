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

#include <QWidget>
#include "ticketqtoolbutton.h"
#include "blfunctions.h"

#include <QString>
#include <QFileDialog>
#include <QMap>
#include <QList>
#include "facturapview.h"
#include "blfixed.h"
#include "facturaview.h"
#include "bfcompany.h"
#include "bldb.h"


///
/**
\param alb
\param parent
**/
TicketQToolButton::TicketQToolButton ( AlbaranProveedorView *alb, QWidget *parent ) : QToolButton ( parent )
{
    blDebug ( "TicketQToolButton::TicketQToolButton", 0 );
    m_albaranProveedorView = alb;
    setBoton();
    blDebug ( "END TicketQToolButton::TicketQToolButton", 0 );
}

///
TicketQToolButton::~TicketQToolButton()
{
    blDebug ( "TicketQToolButton::~TicketQToolButton", 0 );
    blDebug ( "END TicketQToolButton::~TicketQToolButton", 0 );
}


///
/**
**/
void TicketQToolButton::setBoton()
{
    blDebug ( "TicketQToolButton::setBoton", 0 );
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( _ ( "Imprimir Ticket" ) );
    setToolTip ( _ ( "Imprimir Ticket" ) );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/ticket.png" ) );
    setIconSize ( QSize ( 22, 22 ) );
    blDebug ( "END TicketQToolButton::setBoton", 0 );
}


///
/**
**/
void TicketQToolButton::click()
{
    blDebug ( "ImpQToolButton::click", 0 );



    if ( m_albaranProveedorView != NULL ) {
        blDebug ( "Impresion de ticket", 2 );
        m_companyact = m_albaranProveedorView->mainCompany();

    } // end if


    blDebug ( "END ImpQToolButton::click", 0 );
}


