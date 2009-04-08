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

#include <QToolButton>
#include <QLineEdit>
#include <QTableWidget>

#include "pluginartsubform.h"
#include "blfunctions.h"
#include "ticket.h"
#include "btcompany.h"
#include "mticket.h"
#include "busquedaarticulo.h"


///
/**
\return
**/
int entryPoint ( BulmaTPV *tpv )
{
    _depura ( "entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginartsubform", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
    _depura ( "END entryPoint", 0 );
    return 0;
}


int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{

    ArticuloList1 *lan = new ArticuloList1 ( ( BfCompany * ) etpv, NULL, 0, BL_SELECT_MODE );
    g_main->setCentralWidget ( lan );

    return 0;
}
