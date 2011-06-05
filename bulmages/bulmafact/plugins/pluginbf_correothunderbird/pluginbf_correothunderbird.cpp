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

#include "pluginbf_correothunderbird.h"
#include "blfunctions.h"
#include "facturaview.h"
#include "presupuestoview.h"
#include "impqtoolbutton.h"

///
/**
\return
**/
int entryPoint ( BfBulmaFact * )
{
    blDebug ( Q_FUNC_INFO, 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_correothunderbird", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    return 0;
}


///
/**
\param l
\return
**/
int PresupuestoView_PresupuestoView ( PresupuestoView *l )
{
    blDebug ( Q_FUNC_INFO, 0 );
    EmailThunderbirdQToolButton *mui_envia_email = new EmailThunderbirdQToolButton ( l, NULL, NULL,  NULL, l->mui_plugbotones );
    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }

    m_hboxLayout1->addWidget ( mui_envia_email );

    blDebug ( ("END ", Q_FUNC_INFO), 0 );

    return 0;
}


///
/**
\param l
\return
**/
int PedidoClienteView_PedidoClienteView ( PedidoClienteView *l )
{
    blDebug ( Q_FUNC_INFO, 0 );

    EmailThunderbirdQToolButton *mui_envia_email = new EmailThunderbirdQToolButton ( NULL, l, NULL, NULL, l->mui_plugbotones );
    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }

    m_hboxLayout1->addWidget ( mui_envia_email );

    blDebug ( ("END ", Q_FUNC_INFO), 0 );

    return 0;
}


///
/**
\param l
\return
**/
int AlbaranClienteView_AlbaranClienteView ( AlbaranClienteView *l )
{
    blDebug ( Q_FUNC_INFO, 0 );

    EmailThunderbirdQToolButton *mui_envia_email = new EmailThunderbirdQToolButton ( NULL, NULL, l, NULL, l->mui_plugbotones );
    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }

    m_hboxLayout1->addWidget ( mui_envia_email );

    blDebug ( ("END ", Q_FUNC_INFO), 0 );

    return 0;
}


///
/**
\param l
\return
**/
int FacturaView_FacturaView ( FacturaView *l )
{
    blDebug ( Q_FUNC_INFO, 0 );

    EmailThunderbirdQToolButton *mui_envia_email = new EmailThunderbirdQToolButton ( NULL, NULL, NULL, l, l->mui_plugbotones );
    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }

    m_hboxLayout1->addWidget ( mui_envia_email );

    blDebug ( ("END ", Q_FUNC_INFO), 0 );

    return 0;
}
