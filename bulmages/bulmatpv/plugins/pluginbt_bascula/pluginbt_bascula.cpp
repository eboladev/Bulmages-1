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

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QToolButton>

#include "pluginbt_bascula.h"
#include "blfunctions.h"
#include "blplugins.h"
#include "btticket.h"
#include "blapplication.h"
#include "bltoolbutton.h"

#include "portlistener.h"

/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;
PortListener *g_listener = NULL;


///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbascula", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );


    QString portName = g_confpr->value( CONF_TPV_BASCULA_FILE );              // update this to use your port of choice
    g_listener = new PortListener(portName, tpv->company());
    
    
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

    BlToolButton *boton = new BlToolButton(etpv, etpv);
        boton->setObjectName(QString::fromUtf8("mui_bascula"));
        boton->setMinimumSize(QSize(72, 72));
        boton->setMaximumSize(QSize(200, 72));
        boton->setFocusPolicy(Qt::NoFocus);
        //QIcon icon;
        //icon.addFile(QString::fromUtf8(":/Images/employee-list.png"), QSize(), QIcon::Normal, QIcon::Off);
        //boton->setIcon(icon);
        //boton->setIconSize(QSize(32, 32));
        boton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        boton->setText(N_("Pesaje", 0));
	
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
	m_hboxLayout1->addWidget (boton);
    } // end if

    return 0;
}

int BlToolButton_released(BlToolButton *bot) {
  if (bot->objectName() == "mui_bascula") {
    g_listener->comm();
    return 1;
  } //end if
  return 0;
}

