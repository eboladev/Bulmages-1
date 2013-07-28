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

#include <QtWidgets/QToolButton>

#include "pluginbf_q19.h"
#include "blfunctions.h"
#include "q19qtoolbutton.h"
#include "q19qtoolbutton1.h"
#include "q19textcodec.h"
#include "bfbulmafact.h"


///
/**
\return
**/
int entryPoint ( BfBulmaFact * )
{
    BL_FUNC_DEBUG
    new Q19TextCodec(); // no us espanteu, els codecs els arxiva i destrueix QT (espero)
    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_q19", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    
    return 0;
}


///
/**
\param l
\return
**/
int CobrosList_CobrosList ( CobrosList *l )
{
    BL_FUNC_DEBUG

    Q19QToolButton *sel = new Q19QToolButton ( l, l->mui_plugbotones );
    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    m_hboxLayout1->addWidget ( sel );

    
    return 0;
}


///
/**
\param l
\return
**/
int FacturasList_FacturasList ( FacturasList *l )
{
    BL_FUNC_DEBUG

    Q19QToolButton1 *sel = new Q19QToolButton1 ( l, l->mui_plugbotones );
    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    m_hboxLayout1->addWidget ( sel );

    
    return 0;
}


///
/**
\param l
\return
**/
int RecibosList_RecibosList ( RecibosList *l )
{
    BL_FUNC_DEBUG

    Q19QToolButton *sel = new Q19QToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    m_hboxLayout1->addWidget ( sel );

    
    return 0;
}



