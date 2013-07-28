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

#include <QtWidgets/QToolButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTableWidget>

#include "pluginbt_ticketdesglose.h"
#include "btticket.h"
#include "btcompany.h"
#include "mticketdesglose.h"
#include "bldockwidget.h"


BlDockWidget *g_doc1;
MTicketDesglose *g_bud;


///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbt_ticketdesglose", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    /// Vamos a probar con un docwindow.
    g_doc1 = new BlDockWidget ( _ ( "Total" ), tpv, "plugindesglosedocked" );
    g_doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );
    g_doc1->setGeometry ( 100, 100, 100, 500 );
    g_doc1->resize ( 330, 400 );
    tpv->addDockWidget ( Qt::RightDockWidgetArea, g_doc1 );
    g_doc1->loadConfig();
    g_doc1->show();

    
    return 0;
}


int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{
    g_bud =  new MTicketDesglose ( etpv, g_doc1 );
    g_bud->setFocusPolicy ( Qt::NoFocus );
    g_doc1->setWidget ( ( QWidget * ) g_bud );


    return 0;
}


int Ticket_pintar ( BtTicket * )
{
    g_bud->pintar();
    return 0;
}

