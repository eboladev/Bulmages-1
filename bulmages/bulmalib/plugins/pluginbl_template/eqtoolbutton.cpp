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

#include "blfixed.h"
#include "blmaincompany.h"
#include "bldb.h"
#include "eqtoolbutton.h"


///
/**
\param parent
**/
EQToolButton::EQToolButton ( QWidget *parent ) : QWidget ( parent )
{
    _depura ( "EQToolButton::EQToolButton", 0 );
    connect ( parent, SIGNAL ( pintaMenu ( QMenu * ) ), this, SLOT ( pintaMenu ( QMenu * ) ) );
    connect ( parent, SIGNAL ( trataMenu ( QAction * ) ), this, SLOT ( trataMenu ( QAction * ) ) );
    m_BlForm = ( BlForm * ) parent;
    _depura ( "END EQToolButton::EQToolButton", 0 );
}


///
/**
**/
EQToolButton::~EQToolButton()
{
    _depura ( "EQToolButton::~EQToolButton", 0 );
    _depura ( "END EQToolButton::~EQToolButton", 0 );
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void EQToolButton::pintaMenu ( QMenu *menu )
{
    _depura ( "EQToolButton::pintaMenu", 0 );
    QMenu *ajust = menu->addMenu ( _ ( "Informes personales" ) );

    /// Buscamos ficheros que tengan el nombre de la tabla
    QDir dir ( g_confpr->valor ( CONF_DIR_OPENREPORTS ) );
    dir.setFilter ( QDir::Files | QDir::NoSymLinks );
    dir.setSorting ( QDir::Size | QDir::Reversed );
    /// Hacemos un filtrado de busqueda
    QStringList filters;
    filters << "*" + m_BlForm->tableName() + "*.rml";
    dir.setNameFilters ( filters );


    QFileInfoList list = dir.entryInfoList();
    for ( int i = 0; i < list.size(); ++i ) {
        QFileInfo fileInfo = list.at ( i );


        QFile file;
        file.setFileName ( g_confpr->valor ( CONF_DIR_OPENREPORTS ) + fileInfo.fileName() );
        file.open ( QIODevice::ReadOnly );
        QTextStream stream ( &file );
        QString buff = stream.readAll();
        file.close();

        /// Buscamos Query's por tratar
        QString titulo = fileInfo.fileName();
        QRegExp rx1 ( "title\\s*=\\s*\"(.*)\"" );
        rx1.setMinimal ( TRUE );
        if ( rx1.indexIn ( buff, 0 )  != -1 ) {
            titulo = rx1.cap ( 1 );
        } // end while


        QAction *accion = ajust->addAction ( titulo );
        accion->setObjectName ( fileInfo.fileName() );
    }
    _depura ( "END EQToolButton::pintaMenu", 0 );
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void EQToolButton::trataMenu ( QAction *action )
{
    _depura ( "EQToolButton::trataMenu", 0 );

    /// Buscamos ficheros que tengan el nombre de la tabla
    QDir dir ( g_confpr->valor ( CONF_DIR_OPENREPORTS ) );
    dir.setFilter ( QDir::Files | QDir::NoSymLinks );
    dir.setSorting ( QDir::Size | QDir::Reversed );
    /// Hacemos un filtrado de busqueda
    QStringList filters;
    filters << "*" + m_BlForm->tableName() + "*.rml";
    dir.setNameFilters ( filters );


    QFileInfoList list = dir.entryInfoList();
    for ( int i = 0; i < list.size(); ++i ) {
        QFileInfo fileInfo = list.at ( i );
        if ( action->objectName() == fileInfo.fileName() ) {
            if ( m_BlForm->generaRML ( fileInfo.fileName() ) ) {
                invocaPDF ( fileInfo.fileName().left ( fileInfo.fileName().size() - 4 ) );
            } // end if
        } // end if
    }
    _depura ( "END EQToolButton::trataMenu", 0 );
}



