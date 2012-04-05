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

#include "local_blI18n.h"
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
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbl_formlock", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    return ( 0 );
}


int BlMainCompany_init(BlMainCompany * main) {
    BL_FUNC_DEBUG
    main->dbPatchVersionCheck("PluginBl_FormLock", "0.11.1-0001");
    return 0;
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
int BlForm_load ( BlForm *ficha )
{
    BL_FUNC_DEBUG

    /// Si el usuario s&oacute;lo tiene permisos de lectura sobre la ficha no es necesario bloquearla
    if ( !ficha->mainCompany() ->hasTablePrivilege ( ficha->tableName(), "UPDATE" )
      && !ficha->mainCompany() ->hasTablePrivilege ( ficha->tableName(), "DELETE" ) ) {
        return 0;
    }

    QString table_dot_id = QString("%1.%2").arg(ficha->tableName()).arg(ficha->fieldId());
    QString query;

    query = "SELECT * FROM bloqueo WHERE fichabloqueo = $1 AND identificadorfichabloqueo= $2 AND usuariobloqueo <> $3";
    BlDbRecordSet *cur1 = ficha->mainCompany()->load ( query,
                                          table_dot_id,
                                          ficha->dbValue ( ficha->fieldId() ),
                                          ficha->mainCompany()->currentUser() );

    if ( !cur1->eof() ) {
       blMsgInfo ( _ ( "Esta ficha está siendo utilizada por el usuario \"%1\". Usted no podrá hacer cambios en este momento." )
               .arg( cur1->value("usuariobloqueo") ) );

        /// Miramos si existe un boton de guardar, borrar y uno de aceptar y los desactivamos,
        /// y si hay uno de cancelar mostramos "Cerrar": as&iacute; tenemos una ficha de s&oacute;lo lectura
        QAbstractButton *pbut = NULL; /// Puntero para buscar y manipular botones
        pbut = ficha->findChild<QAbstractButton *> ( "mui_guardar" );
        if ( pbut ) pbut->setEnabled ( FALSE );
        pbut = ficha->findChild<QAbstractButton *> ( "mui_aceptar" );
        if ( pbut ) pbut->setEnabled ( FALSE );
        pbut = ficha->findChild<QAbstractButton *> ( "mui_cancelar" );
        if ( pbut ) pbut->setText("Cerrar");
        pbut = ficha->findChild<QAbstractButton *> ( "mui_borrar" );
        if ( pbut ) pbut->setEnabled ( FALSE );

    } else {

        query = "INSERT INTO bloqueo (fichabloqueo, identificadorfichabloqueo, usuariobloqueo) VALUES ($1,$2,$3)";
        ficha->mainCompany()->run ( query, 
                                    table_dot_id,
                                    ficha->dbValue ( ficha->fieldId() ), 
                                    ficha->mainCompany()->currentUser() );

    } // end if

    delete cur1;
    
    return 0;
}


/// Dejar la ficha desbloqueada
/**
\param ficha
\return
**/
int BlForm_DesBlForm ( BlForm *ficha )
{
    BL_FUNC_DEBUG

    if (!ficha->fieldId().isEmpty()) {
    if ( ficha->dbValue(ficha->fieldId()) != "") {
      
	QString table_dot_id = QString("%1.%2").arg(ficha->tableName()).arg(ficha->fieldId());

	QString query = "";
	if ( ficha->dbValue ( ficha->fieldId() ) != "" ) {
	    QString usuario = "";
	    BlDbRecordSet *cur = ficha->mainCompany()->loadQuery ( "SELECT current_user" );
	    if ( !cur->eof() ) {
		usuario = cur->value( "current_user" );
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
    
    return 0;
}


///
/**
\param l
\return
**/
int BlForm_BlForm ( BlForm *l )
{
    BL_FUNC_DEBUG
    
    new BloqMenu ( l );
    
    return 0;
}
