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

#include <QWidget>
#include <QString>
#include <QFileDialog>
#include <QMap>
#include <QList>
#include <QMenu>
#include <QAction>
#include <QTextStream>
#include <QFile>
#include <QToolButton>
#include <QPushButton>

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
    _depura ( "BloqMenu::BloqMenu", 0 );
    connect ( parent, SIGNAL ( pintaMenu ( QMenu * ) ), this, SLOT ( pintaMenu ( QMenu * ) ) );
    connect ( parent, SIGNAL ( trataMenu ( QAction * ) ), this, SLOT ( trataMenu ( QAction * ) ) );
    m_BlForm = ( BlForm * ) parent;
    _depura ( "END BloqMenu::BloqMenu", 0 );
}


///
/**
**/
BloqMenu::~BloqMenu()
{
    _depura ( "BloqMenu::~BloqMenu", 0 );
    _depura ( "END BloqMenu::~BloqMenu", 0 );
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void BloqMenu::pintaMenu ( QMenu *menu )
{
    _depura ( "BloqMenu::pintaMenu", 0 );
    QAction *ajust = menu->addAction ( _( "Desbloquear ficha" ) );
    ajust->setObjectName ( "desbloquearficha" );

    _depura ( "END BloqMenu::pintaMenu", 0 );
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void BloqMenu::trataMenu ( QAction *action )
{
    _depura ( "BloqMenu::trataMenu", 0 );
    if ( action->objectName() == "desbloquearficha" ) {
        QString query = "DELETE FROM bloqueo WHERE fichabloqueo = '" + m_BlForm->campoId() + "' AND identificadorfichabloqueo= '" + m_BlForm->DBvalue ( m_BlForm->campoId() ) + "'";
        m_BlForm->empresaBase()->ejecuta ( query );

        /// Miramos si existe un boton de guardar, borrar y uno de aceptar y los desactivamos
        QToolButton *pguardar = m_BlForm->findChild<QToolButton *> ( "mui_guardar" );
        if ( pguardar ) pguardar->setEnabled ( TRUE );

        QPushButton *paceptar = m_BlForm->findChild<QPushButton *> ( "mui_aceptar" );
        if ( paceptar ) paceptar->setEnabled ( TRUE );

        QToolButton *pborrar = m_BlForm->findChild<QToolButton *> ( "mui_borrar" );
        if ( pborrar ) pborrar->setEnabled ( TRUE );

        QToolButton *peliminar = m_BlForm->findChild<QToolButton *> ( "mui_eliminar" );
        if ( peliminar ) peliminar->setEnabled ( TRUE );

    } // end if
    _depura ( "END BloqMenu::trataMenu", 0 );
}



