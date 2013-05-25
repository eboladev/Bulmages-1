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

#include "pluginbt_camareros.h"

#include "bltoolbutton.h"

#include "btcompany.h"
#include "blplugins.h"
#include "btticket.h"
#include "bldockwidget.h"
#include "blapplication.h"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QToolButton>

#include "trabajadores.h"



///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbt_camareros", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    
    return 0;
}



int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{

    BlToolButton *boton = new BlToolButton(etpv, etpv);
        boton->setObjectName(QString::fromUtf8("mui_camareros1"));
        boton->setMinimumSize(QSize(72, 72));
        boton->setMaximumSize(QSize(200, 72));
        boton->setFocusPolicy(Qt::NoFocus);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Images/employee-list.png"), QSize(), QIcon::Normal, QIcon::Off);
        boton->setIcon(icon);
        boton->setIconSize(QSize(32, 32));
        boton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        boton->setText(N_("Camareros", 0));
	
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

    Trabajadores *trab = new Trabajadores ( etpv, 0, true );
    trab->exec();
    
    delete trab;
    return 0;
}

int BlToolButton_released(BlToolButton *bot) {
  if (bot->objectName() == "mui_camareros1") {
    Trabajadores * trab = new Trabajadores ( bot->mainCompany(), 0 );
    trab->exec();
    return 1;
  } //end if
  return 0;
}
