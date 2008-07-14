/***************************************************************************
 *   Copyright (C) 2008 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
 *                                                                         *
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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
#include <QToolButton>

#include "pluginmailthunderbird.h"
#include "funcaux.h"
#include "facturaview.h"
#include "presupuestoview.h"
#include "impqtoolbutton.h"


///
/**
\return
**/
int entryPoint ( Bulmafact * )
{
    _depura ( "Estoy dentro del plugin de envio de e-mail", 0 );

    return 0;
}


///
/**
\param l
\return
**/
int PresupuestoView_PresupuestoView ( PresupuestoView *l )
{
    _depura ( "PresupuestoView_PresupuestoView", 0 );

    EmailThunderbirdQToolButton *mui_envia_email = new EmailThunderbirdQToolButton ( l, NULL, NULL,  NULL, l->mui_plugbotones );
    QHBoxLayout *m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );

    m_hboxLayout1->setSpacing ( 5 );
    m_hboxLayout1->setMargin ( 5 );
    m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    m_hboxLayout1->addWidget ( mui_envia_email );

    _depura ( "END PresupuestoView_PresupuestoView", 0 );

    return 0;
}


///
/**
\param l
\return
**/
int PedidoClienteView_PedidoClienteView ( PedidoClienteView *l )
{
    _depura ( "PedidoClienteView_PedidoClienteView", 0 );

    EmailThunderbirdQToolButton *mui_envia_email = new EmailThunderbirdQToolButton ( NULL, l, NULL, NULL, l->mui_plugbotones );
    QHBoxLayout *m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );

    m_hboxLayout1->setSpacing ( 5 );
    m_hboxLayout1->setMargin ( 5 );
    m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    m_hboxLayout1->addWidget ( mui_envia_email );

    _depura ( "END PedidoClienteView_PedidoClienteView", 0 );

    return 0;
}


///
/**
\param l
\return
**/
int AlbaranClienteView_AlbaranClienteView ( AlbaranClienteView *l )
{
    _depura ( "FacturaView_FacturaView", 0 );

    EmailThunderbirdQToolButton *mui_envia_email = new EmailThunderbirdQToolButton ( NULL, NULL, l, NULL, l->mui_plugbotones );
    QHBoxLayout *m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );

    m_hboxLayout1->setSpacing ( 5 );
    m_hboxLayout1->setMargin ( 5 );
    m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    m_hboxLayout1->addWidget ( mui_envia_email );

    _depura ( "END FacturaView_FacturaView", 0 );

    return 0;
}


///
/**
\param l
\return
**/
int FacturaView_FacturaView ( FacturaView *l )
{
    _depura ( "FacturaView_FacturaView", 0 );

    EmailThunderbirdQToolButton *mui_envia_email = new EmailThunderbirdQToolButton ( NULL, NULL, NULL, l, l->mui_plugbotones );
    QHBoxLayout *m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );

    m_hboxLayout1->setSpacing ( 5 );
    m_hboxLayout1->setMargin ( 5 );
    m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    m_hboxLayout1->addWidget ( mui_envia_email );

    _depura ( "END FacturaView_FacturaView", 0 );

    return 0;
}
