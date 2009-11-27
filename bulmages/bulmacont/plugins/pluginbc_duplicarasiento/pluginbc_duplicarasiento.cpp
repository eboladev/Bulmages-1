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
MyPluginDuplicarAsiento::MyPluginDuplicarAsiento(BcCompany *comp, QWidget *parent ) : BlMainCompanyPointer(comp), QToolButton(parent)
{
    _depura ( "MyPluginDuplicarAsiento::MyPluginDuplicarAsiento", 0 );
    connect (this, SIGNAL(released()), this, SLOT(elslot()));
    _depura ( "END MyPluginDuplicarAsiento::MyPluginDuplicarAsiento", 0 );
}


///
/**
**/
MyPluginDuplicarAsiento::~MyPluginDuplicarAsiento()
{
    _depura ( "MyPluginDuplicarAsiento::~MyPluginDuplicarAsiento", 0 );
    _depura ( "END MyPluginDuplicarAsiento::~MyPluginDuplicarAsiento", 0 );
}


///
/**
**/
void MyPluginDuplicarAsiento::elslot()
{
    _depura ( "MyPluginDuplicarAsiento::elslot", 0 );
  BcAsientoView *l = (BcAsientoView *) parent();

    DuplicarAsientoView *dupli = new DuplicarAsientoView ( (BcCompany *)mainCompany(), 0 );
    /// Establecemos los par&aacute;metros para el nuevo asiento a duplicar.
    dupli->inicializa ( l->mui_ordenasiento->text(), l->mui_ordenasiento->text() );
    dupli->exec();
    l->cargaasientos();
    l->boton_fin();
    delete dupli;
    _depura ( "END MyPluginDuplicarAsiento::elslot", 0 );
}


///
/**
\param bges
**/
void MyPluginDuplicarAsiento::inicializa ( BcBulmaCont *bges )
{
    _depura ( "MyPluginDuplicarAsiento::inicializa", 0 );
/*
    /// Creamos el men&uacute;.
    setMainCompany ( bges->empresaactual() );
    m_bulmacont = bges;
    QMenu *pPluginMenu;
    /// Miramos si existe un menu Herramientas
    pPluginMenu = bges->menuBar() ->findChild<QMenu *> ( _ ( "menuVentana" ) );

    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
        pPluginMenu = new QMenu ( _ ( "&Ver" ), bges->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuVentana" ) );
    } // end if

    QAction *accion = new QAction ( _ ( "&Libro DuplicarAsiento" ), 0 );
    accion->setStatusTip ( _ ( "Permite ver el diario" ) );
    accion->setWhatsThis ( _ ( "Podra disponer de la informacion del diario" ) );
    accion->setIcon(QIcon(QString::fromUtf8(":/Images/balance-sheet.png")));
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    pPluginMenu->addAction ( accion );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar() ->insertMenu ( bges->menuVentana->menuAction(), pPluginMenu );
*/
    _depura ( "END MyPluginDuplicarAsiento::inicializa", 0 );
}



///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    _depura ( "entryPoint::entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbc_diario", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    _depura ( "END entryPoint::entryPoint", 0 );
    return 0;
}

///
/**
\param l
\return
**/
int BcAsientoView_BcAsientoView ( BcAsientoView *l )
{
    _depura ( "BcAsientoView_BcAsientoView", 0 );
//================================
    MyPluginDuplicarAsiento *mui_exporta_efactura2 = new MyPluginDuplicarAsiento ( l->mainCompany(),  l );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 5 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );
//================================
    _depura ( "END BcAsientoView_BcAsientoView", 0 );
    return 0;
}



