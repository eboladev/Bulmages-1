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

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QString>
#include <QtWidgets/QFileDialog>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtWidgets/QWidget>

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
    BL_FUNC_DEBUG
    m_asiento1View = as;
    setBoton();
    
}


///
/**
**/
RegIVAQToolButton::~RegIVAQToolButton()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void RegIVAQToolButton::setBoton()
{
    BL_FUNC_DEBUG
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( _ ( "Registro de IVA" ) );
    setToolTip ( _ ( "Registro de IVA" ) );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/vat.png" ) ) );
    setIconSize ( QSize ( 22, 22 ) );
    
}


///
/**
**/
void RegIVAQToolButton::click()
{
    BL_FUNC_DEBUG

    BlSubForm *las = m_asiento1View->findChild<BlSubForm *> ("mui_list");
//    BcAsientoSubForm *las = m_asiento1View->mui_list;
    las->save();
    try {
        int idborrador = las->dbValue ( "idborrador" ).toInt();
        RegistroIvaView *nuevae = new RegistroIvaView ( ( BcCompany * ) las->mainCompany(), 0 );
        nuevae->inicializa1 ( idborrador );
        ( ( BcCompany * ) las->mainCompany() ) ->pWorkspace() ->addSubWindow ( nuevae );
        nuevae->show();
    } catch ( ... ) {
        blMsgInfo ( _("Debe seleccionar un apunte" ));
    } // end try

    
}

