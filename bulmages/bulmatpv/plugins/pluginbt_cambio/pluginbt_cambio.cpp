/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include "pluginbt_cambio.h"
#include "blfunctions.h"
#include "btcompany.h"
#include "cambio.h"
#include "blplugins.h"
#include "btticket.h"
#include "blapplication.h"


///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbt_cambio", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    
    return 0;
}

int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{
    BL_FUNC_DEBUG
    
    return 0;
}


int BtCompany_cobrar ( BtCompany *etpv )
{
    BL_FUNC_DEBUG

    /// Comprueba que el ticket no este vacio.
    if (etpv->ticketActual()->listaLineas()->count() == 0) {
        blMsgWarning ( _( "El ticket esta vacio." ) );
        return -1;
    } // end if

    QDialog *diag = new QDialog;
    Cambio *camb = new Cambio ( etpv, diag );

    diag->setModal ( true );

    /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
    /// para que sea redimensionable y aparezca el titulo de la ventana.
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget ( camb );
    layout->setMargin ( 0 );
    layout->setSpacing ( 0 );
    diag->setLayout ( layout );
    diag->setWindowTitle ( _ ( "Cobro" ) );

    diag->exec();

    int valor = camb->m_value;

    delete diag;

    
    return valor;
}


int BtTicket_BtTicket_Post(BtTicket *tick) 
{
    BL_FUNC_DEBUG
    tick->addDbField ( "pagado", BlDbField::DbNumeric, BlDbField::DbNoSave, _( "Pagado" ) );
    tick->addDbField ( "cambio", BlDbField::DbNumeric, BlDbField::DbNoSave, _( "Cambio" ) );
    
    return 0;
}
