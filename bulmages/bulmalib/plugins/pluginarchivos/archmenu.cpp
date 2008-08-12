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

#include "fixed.h"
#include "empresabase.h"
#include "dbrecord.h"
#include "archmenu.h"
#include "funcaux.h"

///
/**
\param parent
**/
ArchMenu::ArchMenu ( QWidget *parent ) : QWidget ( parent )
{
    _depura ( "ArchMenu::ArchMenu", 0 );
    connect ( parent, SIGNAL ( pintaMenu ( QMenu * ) ), this, SLOT ( pintaMenu ( QMenu * ) ) );
    connect ( parent, SIGNAL ( trataMenu ( QAction * ) ), this, SLOT ( trataMenu ( QAction * ) ) );
    m_ficha = ( Ficha * ) parent;
    _depura ( "END ArchMenu::ArchMenu", 0 );
}


///
/**
**/
ArchMenu::~ArchMenu()
{
    _depura ( "ArchMenu::~ArchMenu", 0 );
    _depura ( "END ArchMenu::~ArchMenu", 0 );
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void ArchMenu::pintaMenu ( QMenu *menu )
{
    _depura ( "ArchMenu::pintaMenu", 0 );
    QMenu *nmenu = menu->addMenu(tr("Archivo Documental"));
    QAction *addaction = nmenu->addAction(tr("Agregar Archivo "));
    addaction->setObjectName("addaction");
    nmenu->addSeparator();
    _depura ( "END ArchMenu::pintaMenu", 0 );
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void ArchMenu::trataMenu ( QAction *action )
{
    _depura ( "ArchMenu::trataMenu", 0 );
    if ( action->objectName() == "desbloquearficha" ) {
        QString query = "DELETE FROM bloqueo WHERE fichabloqueo = '" + m_ficha->campoId() + "' AND identificadorfichabloqueo= '" + m_ficha->DBvalue ( m_ficha->campoId() ) + "'";
        m_ficha->empresaBase()->ejecuta ( query );

        /// Miramos si existe un boton de guardar, borrar y uno de aceptar y los desactivamos
        QToolButton *pguardar = m_ficha->findChild<QToolButton *> ( "mui_guardar" );
        if ( pguardar ) pguardar->setEnabled ( TRUE );

        QPushButton *paceptar = m_ficha->findChild<QPushButton *> ( "mui_aceptar" );
        if ( paceptar ) paceptar->setEnabled ( TRUE );

        QToolButton *pborrar = m_ficha->findChild<QToolButton *> ( "mui_borrar" );
        if ( pborrar ) pborrar->setEnabled ( TRUE );

        QToolButton *peliminar = m_ficha->findChild<QToolButton *> ( "mui_eliminar" );
        if ( peliminar ) peliminar->setEnabled ( TRUE );

    } // end if
    _depura ( "END ArchMenu::trataMenu", 0 );
}



