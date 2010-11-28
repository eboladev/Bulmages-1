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
    blDebug ( _ ( "Estoy dentro del plugin de Abertura de Referencia" ), 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_abrereferencia", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    return 0;
}


///
/**
\param l
\return
**/
int BfBuscarReferencia_on_mui_rferencia_returnPressed ( BfBuscarReferencia *l )
{
    blDebug ( "BfBuscarReferencia_on_mui_rferencia_returnPressed", 0 );

    /// Buscamos la ficha que corresponde al widget.
    QList<BfForm *> lista = g_main->findChildren<BfForm *>();

    BfForm *ficha = ( BfForm * ) l->parentWidget();

    while ( ficha && !lista.contains ( ficha ) ) {
        ficha = ( BfForm * ) ficha->parentWidget();
    } // end while

    if ( ficha ) {
        QString query = "SELECT * FROM " + ficha->tableName() + " WHERE ref" + ficha->tableName() + " = '" + l->text() + "'";
        BlDbRecordSet *cur = l->mainCompany()->loadQuery ( query );
        if ( !cur->eof() ) {
            ficha->cargar ( cur->valor ( "id" + ficha->tableName() ) );
        } // end if
        delete cur;
    }// end if

    blDebug ( "END BfBuscarReferencia_on_mui_rferencia_returnPressed", 0 );
    return 0;
}

