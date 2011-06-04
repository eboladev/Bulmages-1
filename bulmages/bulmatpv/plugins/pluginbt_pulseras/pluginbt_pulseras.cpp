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

#include <QHBoxLayout>


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

    BlToolButton *boton = new BlToolButton(etpv, etpv);
    boton->setObjectName(QString::fromUtf8("mui_pulseramas"));
    boton->setMinimumSize(QSize(72, 72));
    boton->setMaximumSize(QSize(200, 72));
    boton->setFocusPolicy(Qt::NoFocus);
    boton->setIcon(QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/pulseraplus.png"  ));
    boton->setIconSize(QSize(32, 32));
    boton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    boton->setText(N_("Agregar Pulsera", 0));

    BlToolButton *boton1 = new BlToolButton(etpv, etpv);
    boton1->setObjectName(QString::fromUtf8("mui_pulserarayo"));
    boton1->setMinimumSize(QSize(72, 72));
    boton1->setMaximumSize(QSize(200, 72));
    boton1->setFocusPolicy(Qt::NoFocus);
    boton1->setIcon(QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/pulserarayo.png"  ));
    boton1->setIconSize(QSize(32, 32));
    boton1->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    boton1->setText(N_("Recuperar Pulsera", 0));

    BlToolButton *boton2 = new BlToolButton(etpv, etpv);
    boton2->setObjectName(QString::fromUtf8("mui_pulseraint"));
    boton2->setMinimumSize(QSize(72, 72));
    boton2->setMaximumSize(QSize(200, 72));
    boton2->setFocusPolicy(Qt::NoFocus);
    boton2->setIcon(QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/pulseraint.png"  ));
    boton2->setIconSize(QSize(32, 32));
    boton2->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    boton2->setText(N_("Buscar Pulsera", 0));
    
    // ============ Pruebas con abrevs
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
        m_hboxLayout1->addWidget ( boton1 );
        m_hboxLayout1->addWidget ( boton2 );
    } // end if

    return 0;
}
