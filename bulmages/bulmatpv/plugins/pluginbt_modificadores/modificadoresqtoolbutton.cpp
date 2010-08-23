/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#include <QWidget>

/// Necesarios para importacion de efactura
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNode>
#include <QString>
#include <QFileDialog>
#include <QMap>
#include <QList>

#include "modificadoresqtoolbutton.h"
#include "blfunctions.h"
#include "modificadores.h"

///
/**
\param cob
\param parent
**/
ModificadoresQToolButton::ModificadoresQToolButton ( BtCompany * emp, QWidget *parent ) : QToolButton ( parent )
{
    blDebug ( "ModificadoresQToolButton::ModificadoresQToolButton", 0 );
    m_companyact = emp;
    setBoton();
    blDebug ( "END ModificadoresQToolButton::ModificadoresQToolButton", 0 );
}


///
/**
**/
ModificadoresQToolButton::~ModificadoresQToolButton()
{
    blDebug ( "ModificadoresQToolButton::~ModificadoresQToolButton", 0 );
    blDebug ( "END ModificadoresQToolButton::~ModificadoresQToolButton", 0 );
}


///
/**
**/
void ModificadoresQToolButton::setBoton()
{
    blDebug ( "ModificadoresQToolButton::setBoton", 0 );
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( "Generar Q19" );
    setToolTip ( "Generar archivo Q19 de los elementos seleccionados" );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( g_confpr->valor ( CONF_PROGDATA ) + "icons/q19.png" ) );
    setIconSize ( QSize ( 22, 22 ) );
    blDebug ( "END ModificadoresQToolButton::setBoton", 0 );
}


///
/**
\return
**/
void ModificadoresQToolButton::click()
{
    blDebug ( "ModificadoresQToolButton::click", 0 );
    Modificadores * trab = new Modificadores ( m_companyact, 0 );
    trab->exec();
    blDebug ( "END ModificadoresQToolButton::click", 0 );

}

