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


#include <stdio.h>
#include <stdlib.h>

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QFileDialog>
#include <QMap>
#include <QList>
#include <QWidget>

#include "regivaqtoolbutton.h"
#include "blfunctions.h"
#include "blfixed.h"
#include "bcasientoview.h"
#include "bccompany.h"
#include "bldb.h"
#include "registroivaview.h"


///
/**
\param as
\param parent
**/
RegIVAQToolButton::RegIVAQToolButton ( BcAsientoView *as , QWidget *parent ) : QToolButton ( parent )
{
    blDebug ( "RegIVAQToolButton::RegIVAQToolButton", 0 );
    m_asiento1View = as;
    setBoton();
    blDebug ( "END RegIVAQToolButton::RegIVAQToolButton", 0 );
}


///
/**
**/
RegIVAQToolButton::~RegIVAQToolButton()
{
    blDebug ( "RegIVAQToolButton::~RegIVAQToolButton", 0 );
    blDebug ( "END RegIVAQToolButton::~RegIVAQToolButton", 0 );
}


///
/**
**/
void RegIVAQToolButton::setBoton()
{
    blDebug ( "RegIVAQToolButton::setBoton", 0 );
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( _ ( "Registro de IVA" ) );
    setToolTip ( _ ( "Registro de IVA" ) );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/vat.png" ) ) );
    setIconSize ( QSize ( 32, 32 ) );
    blDebug ( "END RegIVAQToolButton::setBoton", 0 );
}


///
/**
**/
void RegIVAQToolButton::click()
{
    blDebug ( "ImpQToolButton::click", 0 );
    BcAsientoSubForm *las = m_asiento1View->mui_list;
    las->guardar();
    try {
        int idborrador = las->dbValue ( "idborrador" ).toInt();
        RegistroIvaView *nuevae = new RegistroIvaView ( ( BcCompany * ) las->mainCompany(), 0 );
        nuevae->inicializa1 ( idborrador );
        ( ( BcCompany * ) las->mainCompany() ) ->pWorkspace() ->addWindow ( nuevae );
        nuevae->show();
    } catch ( ... ) {
        mensajeInfo ( "Debe seleccionar un apunte" );
    } // end try

    blDebug ( "END ImpQToolButton::click", 0 );
}


