/***************************************************************************
 *   Copyright (C) 2008 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>
#include <QFile>
#include <QMenu>
#include <QMenuBar>
#include <QToolButton>
#include <QPushButton>

#include "pluginarchivos.h"
#include "funcaux.h"
#include "ficha.h"
#include "empresabase.h"
#include "archmenu.h"

int entryPoint ( QMainWindow *bcont )
{
    _depura ( "Entrada del plugin Bloqueos", 0 );
    _depura ( "Iniciado correctamente el plugin Bloqueos", 10 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginarchivos", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    return ( 0 );
}

int Ficha_cargar ( Ficha *ficha )
{
    _depura ( "PBloqueos::Ficha_cargar", 0 );
    return 0;
    _depura ( "END PBloqueos::Ficha_cargar", 0 );
}


int Ficha_DesFicha ( Ficha *ficha )
{
/*
    QString query = "";
    if ( ficha->DBvalue ( ficha->campoId() ) != "" ) {
        QString usuario = "";
        cursor2 *cur = ficha->empresaBase()->cargacursor ( "SELECT current_user" );
        if ( !cur->eof() ) {
            usuario = cur->valor ( "current_user" );
        } // end if
        delete cur;
        query = "SELECT * FROM bloqueo WHERE fichabloqueo = '" + ficha->campoId() + "' AND identificadorfichabloqueo= '" + ficha->DBvalue ( ficha->campoId() ) + "' AND usuariobloqueo = '" + usuario + "'";
        cursor2 *cur1 = ficha->empresaBase()->cargacursor ( query );
        if ( !cur1->eof() ) {
            query = "DELETE FROM bloqueo WHERE fichabloqueo = '" + ficha->campoId() + "' AND identificadorfichabloqueo= '" + ficha->DBvalue ( ficha->campoId() ) + "'";
            ficha->empresaBase()->ejecuta ( query );
        } // end if
        delete cur1;
    } // end if
*/
    return 0;
}

///
/**
\param l
\return
**/
int Ficha_Ficha ( Ficha *l )
{
    _depura ( "Ficha_Ficha", 0 );

    ArchMenu *mui_imp = new ArchMenu ( l );

    _depura ( "END Ficha_Ficha", 0 );
    return 0;
}
