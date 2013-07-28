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

#include <QtWidgets/QAction>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtCore/QObject>
#include <QtCore/QTextCodec>
#include <QtCore/QLocale>

#include "pluginbc_duplicarasiento.h"
#include "bcasientoduplicarview.h"
#include "bccompany.h"

///
/**
**/
PluginBc_DuplicarAsiento::PluginBc_DuplicarAsiento(BcAsientoView * as, BcCompany *comp, QWidget *parent ) : BlMainCompanyPointer(comp), QToolButton(parent)
{
    BL_FUNC_DEBUG
    setObjectName ( QString::fromUtf8 ( "PluginBc_DuplicarAsiento" ) );
    setStatusTip ( _ ( "Duplicar asiento" ) );
    setToolTip ( _ ( "Duplicar asiento" ) );
    setMinimumSize ( QSize ( 32, 32 ) );
    setMaximumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( QString::fromUtf8 ( ":/BulmaCont32x32/images/png/igualant.xpm" ) ) );
    setIconSize ( QSize ( 22, 22 ) );
    m_asiento = as;
    connect (this, SIGNAL(released()), this, SLOT(elslot()));
    
}


///
/**
**/
PluginBc_DuplicarAsiento::~PluginBc_DuplicarAsiento()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void PluginBc_DuplicarAsiento::elslot()
{
    BL_FUNC_DEBUG
    DuplicarAsientoView *dupli = new DuplicarAsientoView ( (BcCompany *)mainCompany(), 0 );
    /// Establecemos los par&aacute;metros para el nuevo asiento a duplicar.
    dupli->inicializa ( m_asiento->mui_ordenAsiento->text(), m_asiento->mui_ordenAsiento->text() );
    dupli->exec();
    m_asiento->cargaAsientos();
    m_asiento->botonFin();
    delete dupli;
    
}


///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_duplicarasiento", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    
    return 0;
}

///
/**
\param l
\return
**/
int BcAsientoView_BcAsientoView ( BcAsientoView *l )
{
    BL_FUNC_DEBUG
    PluginBc_DuplicarAsiento *mui_exporta_efactura2 = new PluginBc_DuplicarAsiento ( l, l->mainCompany(),  l );
    mui_exporta_efactura2->setObjectName("m_duplicarasiento");

    QHBoxLayout *layout = l->findChild<QHBoxLayout *> ("mui_plugbotones");

    if (layout) {
        l->mui_plugbotones->addWidget ( mui_exporta_efactura2 );
    } // end if

    return 0;
}



