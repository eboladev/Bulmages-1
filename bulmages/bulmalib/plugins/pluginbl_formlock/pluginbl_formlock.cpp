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

#include "local_BlI18n.h"
#include "pluginbl_formlock.h"
#include "blform.h"
#include "blmaincompany.h"
#include "bloqmenu.h"


///
/**
\param bcont
\return
**/
int entryPoint ( QMainWindow *bcont )
{
    blDebug ( "Entrada del plugin Bloqueos", 0 );
    blDebug ( "Iniciado correctamente el plugin Bloqueos", 10 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbl_formlock", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    return ( 0 );
}


/// Bloquear la ficha si no lo estaba ya por otro usuario
/**
  Antes, en "fichabloqueo" s&oacute;lo se almacenaba el nombre del campo identificador.
  Como al usar esquemas de PostgreSQL la nomenclatura "idtabla" no es exacta,
  pongo 'nombre_tabla.nombre_campo_id' en "fichabloqueo".
  As&iacute; identificamos inequ&iacute;vocamente lo que se ha bloqueado.
\param ficha
\return
**/
int BlForm_cargar ( BlForm *ficha )
{
    blDebug ( "Plugin formlock::BlForm_cargar", 0 );

    QString table_dot_id = QString("%1.%2").arg(ficha->tableName()).arg(ficha->fieldId());
    QString query;

    query = "SELECT * FROM bloqueo WHERE fichabloqueo = $1 AND identificadorfichabloqueo= $2 AND usuariobloqueo <> $3";
    BlDbRecordSet *cur1 = ficha->mainCompany()->load ( query,
                                          table_dot_id,
                                          ficha->dbValue ( ficha->fieldId() ),
                                          ficha->mainCompany()->currentUser() );

    if ( !cur1->eof() ) {
       mensajeInfo ( _ ( "Esta ficha está siendo utilizada por el usuario \"%1\". Usted no podrá hacer cambios en este momento." )
               .arg( cur1->valor("usuariobloqueo") ) );

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

        query = "INSERT INTO bloqueo (fichabloqueo, identificadorfichabloqueo, usuariobloqueo) VALUES ($1,$2,$3)";
        ficha->mainCompany()->run ( query, 
                                    table_dot_id,
                                    ficha->dbValue ( ficha->fieldId() ), 
                                    ficha->mainCompany()->currentUser() );

    } // end if

    delete cur1;
    blDebug ( "END Plugin formlock::BlForm_cargar", 0 );
    return 0;
}


/// Dejar la ficha desbloqueada
/**
\param ficha
\return
**/
int BlForm_DesBlForm ( BlForm *ficha )
{
    blDebug ( "Plugin formlock::BlForm_DesBlForm", 0 );

    if (!ficha->fieldId().isEmpty()) {
    if ( ficha->dbValue(ficha->fieldId()) != "") {
      
	QString table_dot_id = QString("%1.%2").arg(ficha->tableName()).arg(ficha->fieldId());

	QString query = "";
	if ( ficha->dbValue ( ficha->fieldId() ) != "" ) {
	    QString usuario = "";
	    BlDbRecordSet *cur = ficha->mainCompany()->loadQuery ( "SELECT current_user" );
	    if ( !cur->eof() ) {
		usuario = cur->valor ( "current_user" );
	    } // end if
	    delete cur;
	    query = "SELECT * FROM bloqueo WHERE fichabloqueo = $1 AND identificadorfichabloqueo= $2 AND usuariobloqueo = $3";
	    BlDbRecordSet *cur1 = ficha->mainCompany()->load( query , 
							      table_dot_id,
							      ficha->dbValue ( ficha->fieldId() ) ,
							      usuario );
	    if ( !cur1->eof() ) {
		query = "DELETE FROM bloqueo WHERE fichabloqueo = $1 AND identificadorfichabloqueo= $2";
		ficha->mainCompany()->run ( query ,
					    table_dot_id,
					    ficha->dbValue ( ficha->fieldId() ));
	    } // end if
	    delete cur1;
	} // end if
    } // end if
    } // end if
    blDebug ( "END Plugin formlock::BlForm_DesBlForm", 0 );
    return 0;
}


///
/**
\param l
\return
**/
int BlForm_BlForm ( BlForm *l )
{
    blDebug ( "Plugin formlock::BlForm_BlForm", 0 );
    new BloqMenu ( l );
    blDebug ( "END Plugin formlock::BlForm_BlForm", 0 );
    return 0;
}
