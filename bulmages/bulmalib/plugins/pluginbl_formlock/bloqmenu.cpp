/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#include <QtWidgets/QWidget>
#include <QtCore/QString>
#include <QtWidgets/QFileDialog>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QPushButton>

#include "local_blI18n.h"
#include "blfixed.h"
#include "blmaincompany.h"
#include "bldb.h"
#include "bloqmenu.h"


///
/**
\param parent
**/
BloqMenu::BloqMenu ( QWidget *parent ) : QWidget ( parent )
{
    BL_FUNC_DEBUG
    connect ( parent, SIGNAL ( pintaMenu ( QMenu * ) ), this, SLOT ( pintaMenu ( QMenu * ) ) );
    connect ( parent, SIGNAL ( trataMenu ( QAction * ) ), this, SLOT ( trataMenu ( QAction * ) ) );
    m_BlForm = ( BlForm * ) parent;
    
}


///
/**
**/
BloqMenu::~BloqMenu()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void BloqMenu::pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG

    if ( m_BlForm->mainCompany() ->hasTablePrivilege ( m_BlForm->tableName(), "UPDATE" )
      || m_BlForm->mainCompany() ->hasTablePrivilege ( m_BlForm->tableName(), "DELETE" ) ) {
       QAction *ajust = menu->addAction ( _ ( "Desbloquear ficha" ) );
       ajust->setObjectName ( "desbloquearficha" );
    }
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void BloqMenu::trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG
    if ( action->objectName() == "desbloquearficha" ) {
        QString table_dot_id = QString("%1.%2").arg(m_BlForm->tableName()).arg(m_BlForm->fieldId());
        QString query = "DELETE FROM bloqueo WHERE fichabloqueo = $1 AND identificadorfichabloqueo= $2";
        m_BlForm->mainCompany()->run ( query , table_dot_id, m_BlForm->dbValue ( m_BlForm->fieldId() ));

        /// Teniendo en cuenta los privilegios del usuario sobre la tabla de la ficha,
        /// miramos si existe un boton de guardar, borrar y uno de aceptar y los reactivamos,
        /// y si hay uno de cancelar ajusto el texto: as&iacute; tenemos una ficha modificable de nuevo
        QAbstractButton *pbut = NULL; /// Puntero para buscar y manipular botones
        if ( m_BlForm->mainCompany() ->hasTablePrivilege ( m_BlForm->tableName(), "UPDATE" ) ) {
           pbut = m_BlForm->findChild<QAbstractButton *> ( "mui_guardar" );
           if ( pbut ) pbut->setEnabled ( true );
           pbut = m_BlForm->findChild<QAbstractButton *> ( "mui_aceptar" );
           if ( pbut ) pbut->setEnabled ( true );
           pbut = m_BlForm->findChild<QAbstractButton *> ( "mui_cancelar" );
           if ( pbut ) pbut->setText("Cancelar");
        }
        if ( m_BlForm->mainCompany() ->hasTablePrivilege ( m_BlForm->tableName(), "DELETE" ) ) {
           pbut = m_BlForm->findChild<QAbstractButton *> ( "mui_borrar" );
           if ( pbut ) pbut->setEnabled ( true );
        }
    } // end if
}
