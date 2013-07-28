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

#include <QtWidgets/QAction>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QPushButton>

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
int entryPoint ( QMainWindow *bges )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbl_formlock", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

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

    /// No seguir con el bloqueo si la ficha es visible (que significa que estamos haciendo la carga post-guardado de BlForm::save())
    /// o si el usuario s&oacute;lo tiene permisos de lectura
    if ( ficha->isVisible()
      || ( !ficha->mainCompany() ->hasTablePrivilege ( ficha->tableName(), "UPDATE" )
        && !ficha->mainCompany() ->hasTablePrivilege ( ficha->tableName(), "DELETE" ) ) ) {
        return 0;
    }

    QString table_dot_id = QString("%1.%2").arg(ficha->tableName()).arg(ficha->fieldId());
    QString query;

    query = "SELECT * FROM bloqueo WHERE fichabloqueo = $1 AND identificadorfichabloqueo= $2 AND usuariobloqueo <> $3";
    BlDbRecordSet *cur1 = ficha->mainCompany()->load ( query,
                                          table_dot_id,
                                          ficha->dbValue ( ficha->fieldId() ),
                                          ficha->mainCompany()->currentUser() );

    int res = 0;

    if ( !cur1->eof() ) {

       /// Ofrecer al usuario la oportunidad de desbloquear la ficha
       QString msg = _ ( "La ficha parece estar en uso por otra persona.\nSe recomienda no hacer cambios en este momento.\nFicha bloqueada para \"%1\" el %2 a las %3." )
                   .arg( cur1->value("usuariobloqueo") ).arg( cur1->value("fechabloqueo") ).arg( cur1->value("horabloqueo").left(5) );
       res = QMessageBox::question ( NULL,
                         _ ( "Ficha en uso" ),
                         msg,
                         _ ( "&Ver en sólo lectura" ),
                         _ ( "&Desbloquear" ),
                         0, 1 );

       /// El usuario decide ver la ficha en s&oacute;lo lectura
       if ( res == 0 ) {

           /// Miramos si existe un boton de guardar, borrar y uno de aceptar y los desactivamos,
           /// y si hay uno de cancelar mostramos "Cerrar": as&iacute; tenemos una ficha de s&oacute;lo lectura
           QAbstractButton *pbut = NULL; /// Puntero para buscar y manipular botones
           pbut = ficha->findChild<QAbstractButton *> ( "mui_guardar" );
           if ( pbut ) pbut->setEnabled ( false );
           pbut = ficha->findChild<QAbstractButton *> ( "mui_aceptar" );
           if ( pbut ) pbut->setEnabled ( false );
           pbut = ficha->findChild<QAbstractButton *> ( "mui_cancelar" );
           if ( pbut ) pbut->setText("Cerrar");
           pbut = ficha->findChild<QAbstractButton *> ( "mui_borrar" );
           if ( pbut ) pbut->setEnabled ( false );
       } // end if

       /// El usuario decide desbloquear de todos modos
       else if ( res == 1 ) {

           BlForm_DesBlForm ( ficha );
       }
    }

    /// Si no hab&iacute;a un bloqueo activado o el usuario actual ha desbloqueado el bloqueo de otra persona, bloquear en su nombre
    if ( cur1->eof() || res == 1 ) {

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

    /// Eliminar los bloqueos de usuarios no conectados (son restos de un cierre precipitado por alg&uacute;n fallo)
    QString query = QString ( "DELETE FROM bloqueo WHERE usuariobloqueo NOT IN (SELECT usename FROM pg_stat_activity WHERE datname = '%1');" ).arg ( l->mainCompany()->dbName() );
    l->mainCompany()->runQuery(query);
    
    return 0;
}
