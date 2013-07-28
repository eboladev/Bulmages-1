/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <stdio.h>

#include <QtWidgets/QAction>
#include <QtCore/QObject>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QToolButton>
#include <QtGui/QImage>
#include <QtWidgets/QLabel>
#include <QtGui/QPixmap>
#include <QtWidgets/QFrame>

#include "pluginbc_documentoadjunto.h"
#include "bccompany.h"
#include "bcasientoview.h"
#include "adocumental.h"

/// Inclusion de imagenes.
#include "napuntedoc.xpm"
#include "ndoc.xpm"


///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_documentoadjunto", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    QPixmap *img = new QPixmap ( napuntedoc );
    QPixmap *img1 = new QPixmap ( ndoc );

    BcCompany *emp = bcont->company();
    BcAsientoView *intapunts = emp->intapuntsempresa();

    myplugin1 *pub = new myplugin1 ( emp );
    intapunts->hide();

    QToolButton *m_adocumental = new QToolButton ( intapunts );
    m_adocumental->setIcon ( QIcon ( *img ) );
    m_adocumental->setMinimumSize ( QSize ( 22, 22 ) );
    m_adocumental->setSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed );
    QObject::connect ( m_adocumental, SIGNAL ( clicked() ), pub, SLOT ( boton_adjuntar() ) );

    intapunts->hboxLayout1->addWidget ( m_adocumental );

    QToolButton *m_adocumental1 = new QToolButton ( intapunts );
    m_adocumental1->setIcon ( QIcon ( *img1 ) );
    m_adocumental1->setMinimumSize ( QSize ( 22, 22 ) );
    m_adocumental1->setSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed );
    QObject::connect ( m_adocumental1, SIGNAL ( clicked() ), pub, SLOT ( boton_nuevoasientodocumental() ) );

    intapunts->hboxLayout1->addWidget ( m_adocumental1 );

    /// Hacemos la entrada de menu.
    QAction *ArchDoc = new QAction ( _ ( "&Archivo documental" ), 0 );
    ArchDoc->setStatusTip ( _ ( "Archivo documental" ) );
    ArchDoc->setWhatsThis ( _ ( "Archivo documental" ) );
    QObject::connect ( ArchDoc, SIGNAL ( activated() ), pub, SLOT ( archDoc() ) );

    /// Pone la opcion dentro del menu HERRAMIENTAS.
    bcont->mui_MenuHerramientas() ->addSeparator();
    bcont->mui_MenuHerramientas() ->addAction ( ArchDoc );

    
    return 0;
}

