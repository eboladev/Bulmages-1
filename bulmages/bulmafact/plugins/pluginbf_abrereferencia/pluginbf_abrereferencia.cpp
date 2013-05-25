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

#include "pluginbf_abrereferencia.h"
#include "blfunctions.h"
#include "bfform.h"
#include "bfbuscarreferencia.h"


///
/**
\return
**/
int entryPoint ( BfBulmaFact * )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_abrereferencia", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    
    return 0;
}


///
/**
\param l
\return
**/
int BfBuscarReferencia_on_mui_rferencia_returnPressed ( BfBuscarReferencia *l )
{
    BL_FUNC_DEBUG

    /// Buscamos la ficha que corresponde al widget.
    QList<BfForm *> lista = g_main->findChildren<BfForm *>();

    BfForm *ficha = ( BfForm * ) l->parentWidget();

    while ( ficha && !lista.contains ( ficha ) ) {
        ficha = ( BfForm * ) ficha->parentWidget();
    } // end while

    if ( ficha ) {
        QString query = "SELECT * FROM " + ficha->tableName() + " WHERE ref" + ficha->tableName() + " = '" + l->text() + "'";
        BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("Consulta: '%1'.")).arg(query) );
        BlDbRecordSet *cur = l->mainCompany()->loadQuery ( query );
        if ( !cur->eof() ) {
            ficha->load ( cur->value( "id" + ficha->tableName() ) );
        } // end if
        delete cur;
    } // end if

    
    return 0;
}

