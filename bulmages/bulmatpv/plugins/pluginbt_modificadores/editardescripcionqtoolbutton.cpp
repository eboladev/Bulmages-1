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

#include <QtWidgets/QWidget>

#include "editardescripcionqtoolbutton.h"
#include "blfunctions.h"
#include "modificadores.h"


///
/**
\param cob
\param parent
**/
EditarDescripcionQToolButton::EditarDescripcionQToolButton ( BtCompany * emp, QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
    m_companyact = emp;
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
}


///
/**
**/
EditarDescripcionQToolButton::~EditarDescripcionQToolButton()
{
    BL_FUNC_DEBUG
}



///
/**
\return
**/
void EditarDescripcionQToolButton::click()
{
    BL_FUNC_DEBUG

    /// Solo muestra la ventana de modificadores si se ha seleccionado una linea del ticket que se
    /// pueda modificar.
    if (! ( ( BtCompany * ) m_companyact )->ticketActual()->lineaActBtTicket() ) {
	blMsgWarning(_("Primero seleccione una linea del ticket."));
    } else {
	BtTicket *ticket = ( ( BtCompany * ) m_companyact )->ticketActual();
	Modificadores * trab = new Modificadores ( m_companyact, 0, true, "desclalbaran" );
	trab->exec();
	ticket->pintar();      
    } // end if    

}
