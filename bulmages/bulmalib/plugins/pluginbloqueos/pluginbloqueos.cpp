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

#include "pluginbloqueos.h"
#include "blform.h"
#include "blmaincompany.h"
#include "bloqmenu.h"


int entryPoint ( QMainWindow *bcont )
{
    _depura ( "Entrada del plugin Bloqueos", 0 );
    _depura ( "Iniciado correctamente el plugin Bloqueos", 10 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginbloqueos", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    return ( 0 );
}


int BlForm_cargar ( BlForm *ficha )
{
    _depura ( "Plugin bloqueos::BlForm_cargar", 0 );
    QString query;

    query = "SELECT * FROM bloqueo WHERE fichabloqueo = '" + ficha->fieldId() + "' AND identificadorfichabloqueo= '" + ficha->dbValue ( ficha->fieldId() ) + "' AND usuariobloqueo <> '" + ficha->mainCompany()->currentUser() + "'";
    BlDbRecordSet *cur1 = ficha->mainCompany()->loadQuery ( query );

    if ( !cur1->eof() ) {
        mensajeInfo ( _("Ficha bloqueada por otro usuario, no podra hacer modificaciones") );

        /// Miramos si existe un boton de guardar, borrar y uno de aceptar y los desactivamos
        QToolButton *pguardar = ficha->findChild<QToolButton *> ( "mui_guardar" );
        if ( pguardar ) pguardar->setEnabled ( FALSE );

        QPushButton *paceptar = ficha->findChild<QPushButton *> ( "mui_aceptar" );
        if ( paceptar ) paceptar->setEnabled ( FALSE );

        QToolButton *pborrar = ficha->findChild<QToolButton *> ( "mui_borrar" );
        if ( pborrar ) pborrar->setEnabled ( FALSE );

        QToolButton *peliminar = ficha->findChild<QToolButton *> ( "mui_eliminar" );
        if ( peliminar ) peliminar->setEnabled ( FALSE );

    } else {

        query = "INSERT INTO bloqueo (fichabloqueo, identificadorfichabloqueo, usuariobloqueo) VALUES ('" + ficha->fieldId() + "','" + ficha->dbValue ( ficha->fieldId() ) + "','" + ficha->mainCompany()->currentUser() + "')";
        ficha->mainCompany()->runQuery ( query );

    } // end if

    delete cur1;
    return 0;
    _depura ( "END Plugin bloqueos::BlForm_cargar", 0 );
}


int BlForm_DesFicha ( BlForm *ficha )
{
    QString query = "";
    if ( ficha->dbValue ( ficha->fieldId() ) != "" ) {
        QString usuario = "";
        BlDbRecordSet *cur = ficha->mainCompany()->loadQuery ( "SELECT current_user" );
        if ( !cur->eof() ) {
            usuario = cur->valor ( "current_user" );
        } // end if
        delete cur;
        query = "SELECT * FROM bloqueo WHERE fichabloqueo = '" + ficha->fieldId() + "' AND identificadorfichabloqueo= '" + ficha->dbValue ( ficha->fieldId() ) + "' AND usuariobloqueo = '" + usuario + "'";
        BlDbRecordSet *cur1 = ficha->mainCompany()->loadQuery ( query );
        if ( !cur1->eof() ) {
            query = "DELETE FROM bloqueo WHERE fichabloqueo = '" + ficha->fieldId() + "' AND identificadorfichabloqueo= '" + ficha->dbValue ( ficha->fieldId() ) + "'";
            ficha->mainCompany()->runQuery ( query );
        } // end if
        delete cur1;
    } // end if
    return 0;
}


///
/**
\param l
\return
**/
int BlForm_BlForm ( BlForm *l )
{
    _depura ( "BlForm_BlForm", 0 );

    BloqMenu *mui_imp = new BloqMenu ( l );

    _depura ( "END BlForm_BlForm", 0 );
    return 0;
}
