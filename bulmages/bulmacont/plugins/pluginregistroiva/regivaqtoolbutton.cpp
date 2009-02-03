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
#include "regivaqtoolbutton.h"
#include "funcaux.h"


#include <stdio.h>
#include <stdlib.h>

#include <QFile>
#include <QTextStream>


#include <QString>
#include <QFileDialog>
#include <QMap>
#include <QList>
#include "fixed.h"


#include "asiento1view.h"
#include "empresa.h"
#include "dbrecord.h"
#include "registroivaview.h"


///
/**
\param as
\param parent
**/
RegIVAQToolButton::RegIVAQToolButton ( Asiento1View *as , QWidget *parent ) : QToolButton ( parent )
{
    _depura ( "RegIVAQToolButton::RegIVAQToolButton", 0 );
    m_asiento1View = as;
    setBoton();
    _depura ( "END RegIVAQToolButton::RegIVAQToolButton", 0 );
}


///
/**
**/
RegIVAQToolButton::~RegIVAQToolButton()
{
    _depura ( "RegIVAQToolButton::~RegIVAQToolButton", 0 );
    _depura ( "END RegIVAQToolButton::~RegIVAQToolButton", 0 );
}


///
/**
**/
void RegIVAQToolButton::setBoton()
{
    _depura ( "RegIVAQToolButton::setBoton", 0 );
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( _( "Registro de IVA" ) );
    setToolTip ( _( "Registro de IVA" ) );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/vat.svg" ) ) );
    setIconSize ( QSize ( 32, 32 ) );
    _depura ( "END RegIVAQToolButton::setBoton", 0 );
}


///
/**
**/
void RegIVAQToolButton::click()
{
    _depura ( "ImpQToolButton::click", 0 );
    ListLinAsiento1View *las = m_asiento1View->mui_list;
    las->guardar();
    try {
        int idborrador = las->DBvalue ( "idborrador" ).toInt();
        RegistroIvaView *nuevae = new RegistroIvaView ( ( Empresa * ) las->empresaBase(), 0 );
        nuevae->inicializa1 ( idborrador );
        ( ( Empresa * ) las->empresaBase() ) ->pWorkspace() ->addWindow ( nuevae );
        nuevae->show();
    } catch ( ... ) {
        mensajeInfo ( "Debe seleccionar un apunte" );
    } // end try

    _depura ( "END ImpQToolButton::click", 0 );
}


