/***************************************************************************
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

#include <cstdio>

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>
#include <QTextCodec>
#include <QLocale>

#include "pluginbc_duplicarasiento.h"
#include "bcasientoduplicarview.h"
#include "bccompany.h"

///
/**
**/
MyPluginDuplicarAsiento::MyPluginDuplicarAsiento(BcAsientoView * as, BcCompany *comp, QWidget *parent ) : BlMainCompanyPointer(comp), QToolButton(parent)
{
    blDebug ( "MyPluginDuplicarAsiento::MyPluginDuplicarAsiento", 0 );
    setObjectName ( QString::fromUtf8 ( "MyPluginDuplicarAsiento" ) );
    setStatusTip ( _ ( "Duplicar Asiento" ) );
    setToolTip ( _ ( "Duplicar Asiento" ) );
    setMinimumSize ( QSize ( 32, 32 ) );
    setMaximumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( QString::fromUtf8 ( ":/BulmaCont32x32/images/png/igualant.xpm" ) ) );
    setIconSize ( QSize ( 32, 32 ) );
    m_asiento = as;
    connect (this, SIGNAL(released()), this, SLOT(elslot()));
    blDebug ( "END MyPluginDuplicarAsiento::MyPluginDuplicarAsiento", 0 );
}


///
/**
**/
MyPluginDuplicarAsiento::~MyPluginDuplicarAsiento()
{
    blDebug ( "MyPluginDuplicarAsiento::~MyPluginDuplicarAsiento", 0 );
    blDebug ( "END MyPluginDuplicarAsiento::~MyPluginDuplicarAsiento", 0 );
}


///
/**
**/
void MyPluginDuplicarAsiento::elslot()
{
    blDebug ( "MyPluginDuplicarAsiento::elslot", 0 );
    DuplicarAsientoView *dupli = new DuplicarAsientoView ( (BcCompany *)mainCompany(), 0 );
    /// Establecemos los par&aacute;metros para el nuevo asiento a duplicar.
    dupli->inicializa ( m_asiento->mui_ordenasiento->text(), m_asiento->mui_ordenasiento->text() );
    dupli->exec();
    m_asiento->cargaasientos();
    m_asiento->boton_fin();
    delete dupli;
    blDebug ( "END MyPluginDuplicarAsiento::elslot", 0 );
}


///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    blDebug ( "entryPoint::entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_duplicarasiento", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    blDebug ( "END entryPoint::entryPoint", 0 );
    return 0;
}

///
/**
\param l
\return
**/
int BcAsientoView_BcAsientoView ( BcAsientoView *l )
{
    blDebug ( "BcAsientoView_BcAsientoView", 0 );
    MyPluginDuplicarAsiento *mui_exporta_efactura2 = new MyPluginDuplicarAsiento ( l, l->mainCompany(),  l );
    mui_exporta_efactura2->setObjectName("m_duplicarasiento");
    
    QFrame *frame = l->findChild<QFrame *> ("mui_plugbotones");
    if (frame) {
	QHBoxLayout *m_hboxLayout1 = frame->findChild<QHBoxLayout *> ( "hboxLayout1" );
	if ( !m_hboxLayout1 ) {
	    m_hboxLayout1 = new QHBoxLayout ( frame );
	    m_hboxLayout1->setSpacing ( 5 );
	    m_hboxLayout1->setMargin ( 5 );
	    m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
	} // end if
	m_hboxLayout1->addWidget ( mui_exporta_efactura2 );
    } // end if

    blDebug ( "END BcAsientoView_BcAsientoView", 0 );
    return 0;
}



