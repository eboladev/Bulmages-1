/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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


#include "informeqtoolbutton.h"
#include "informeqtoolbutton1.h"


#include "pluginbf_informecliente2sxc.h"
#include "bfcompany.h"
#include "blfunctions.h"


///
/**
**/
int entryPoint ( BfBulmaFact * )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_informecliente2sxc", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    return 0;
}


///
/**
\param l
\return
**/
int ClientsList_ClientsList ( ClientsList *l )
{
    BL_FUNC_DEBUG
    InformeQToolButton *mui_exporta_efactura2 = new InformeQToolButton ( l,  l->mui_plugbotones );
    InformeQToolButton1 *mui_exporta_efactura3 = new InformeQToolButton1 ( l,  l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 5 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );
    m_hboxLayout1->addWidget ( mui_exporta_efactura3 );
    
    return 0;
}


///
/**
\param l
\return
**/
int ProveedorList_ProveedorList ( ProveedorList *l )
{
    BL_FUNC_DEBUG
    InformeProveedorQToolButton *mui_exporta_efactura2 = new InformeProveedorQToolButton ( l,  l->mui_plugbotones );
    InformeProveedorQToolButton1 *mui_exporta_efactura3 = new InformeProveedorQToolButton1 ( l,  l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 5 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );
    m_hboxLayout1->addWidget ( mui_exporta_efactura3 );
    
    return 0;
}


