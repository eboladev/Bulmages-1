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
#include <QtWidgets/QHBoxLayout>

#include "blfixed.h"
#include "blmaincompany.h"
#include "bldb.h"
#include "eqtoolbutton.h"
#include "local_blI18n.h"

///
/**
\param parent
**/
EQToolButton::EQToolButton ( QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
    
    /// Buscamos alguna otra instancia y si la hay nos quitamos de enmedio
    EQToolButton *tool = parent->findChild<EQToolButton *>("EQToolButtonN");
    if (tool) {
      hide();
      return;
    } // end if
    setObjectName("EQToolButtonN");
    
    
    connect ( parent, SIGNAL ( pintaMenu ( QMenu * ) ), this, SLOT ( pintaMenu ( QMenu * ) ) );
    connect ( parent, SIGNAL ( trataMenu ( QAction * ) ), this, SLOT ( trataMenu ( QAction * ) ) );
    m_BlForm = ( BlForm * ) parent;
    

    
    
    
    QFrame *plugbotones = m_BlForm->findChild<QFrame *>("mui_plugbotones");
    if (plugbotones) {
	QHBoxLayout *m_hboxLayout1 = plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );
	if ( !m_hboxLayout1 ) {
	    m_hboxLayout1 = new QHBoxLayout ( plugbotones );
	    m_hboxLayout1->setSpacing ( 5 );
	    m_hboxLayout1->setMargin ( 0 );
	    m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
	} // end if
	m_hboxLayout1->addWidget ( this );

	setMinimumSize ( QSize ( 32, 32 ) );
        setMaximumSize ( QSize ( 32, 32 ) );
	setIcon ( QIcon ( ":/Images/template2ods.png" ) );
	setIconSize ( QSize ( 32, 32 ) );  	
	setPopupMode(QToolButton::InstantPopup);  
	
	/// Creamos el menu
	QMenu *menu = new QMenu(this);
	
	/// Buscamos ficheros que tengan el nombre de la tabla
	QDir dir ( g_confpr->value( CONF_DIR_OPENREPORTS ) );
	dir.setFilter ( QDir::Files | QDir::NoSymLinks );
	dir.setSorting ( QDir::Size | QDir::Reversed );
	/// Hacemos un filtrado de busqueda
	QStringList filters;
	filters << "*impers_" + m_BlForm->tableName() + "*.pys";
	dir.setNameFilters ( filters );


	QFileInfoList list = dir.entryInfoList();
	// Si no hay elementos que mostrar entonces ocultamos el boton ya que no lleva a ninguna parte.
	if (list.size() == 0) {
	    hide();
	    return;
	} // end if
	for ( int i = 0; i < list.size(); ++i ) {
	    QFileInfo fileInfo = list.at ( i );


	    QFile file;
	    file.setFileName ( g_confpr->value( CONF_DIR_OPENREPORTS ) + fileInfo.fileName() );
	    file.open ( QIODevice::ReadOnly );
	    QTextStream stream ( &file );
	    QString buff = stream.readAll();
	    file.close();

	    /// Buscamos Query's por tratar
	    QString titulo = fileInfo.fileName();
	    QRegExp rx1 ( " title\\s*=\\s*\"(.*)\"" );
	    rx1.setMinimal ( true );
	    if ( rx1.indexIn ( buff, 0 )  != -1 ) {
		titulo = rx1.cap ( 1 );
	    } // end while


            /// Buscamos Query's por tratar
            QString icon = ":/Images/template2ods.png";
            QRegExp rx2 ( " icon\\s*=\\s*\"(.*)\"" );
            rx2.setMinimal ( true );
            if ( rx2.indexIn ( buff, 0 )  != -1 ) {
                icon = rx2.cap ( 1 );
            } // end if


	    QAction *accion = menu->addAction ( titulo );
	    accion->setObjectName ( fileInfo.fileName() );
	    accion->setIcon(QIcon(icon));
	    connect ( accion, SIGNAL ( triggered ( bool ) ), this, SLOT ( trataMenu ( ) ) );
	}
	setMenu(menu);
    } else {
	hide();
    } // end if
    
    
}


///
/**
**/
EQToolButton::~EQToolButton()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void EQToolButton::pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
    QMenu *ajust = menu->addMenu ( QIcon(":/Images/template2ods.png"), _ ( "Informes personales ODS" ) );
	
    /// Buscamos ficheros que tengan el nombre de la tabla
    QDir dir ( g_confpr->value( CONF_DIR_OPENREPORTS ) );
    dir.setFilter ( QDir::Files | QDir::NoSymLinks );
    dir.setSorting ( QDir::Size | QDir::Reversed );
    /// Hacemos un filtrado de busqueda
    QStringList filters;
    filters << "*impers_" + m_BlForm->tableName() + "*.pys";
    dir.setNameFilters ( filters );


    QFileInfoList list = dir.entryInfoList();
    for ( int i = 0; i < list.size(); ++i ) {
        QFileInfo fileInfo = list.at ( i );


        QFile file;
        file.setFileName ( g_confpr->value( CONF_DIR_OPENREPORTS ) + fileInfo.fileName() );
        file.open ( QIODevice::ReadOnly );
        QTextStream stream ( &file );
        QString buff = stream.readAll();
        file.close();

        /// Buscamos Query's por tratar
        QString titulo = fileInfo.fileName();
        QRegExp rx1 ( " title\\s*=\\s*\"(.*)\"" );
        rx1.setMinimal ( true );
        if ( rx1.indexIn ( buff, 0 )  != -1 ) {
            titulo = rx1.cap ( 1 );
        } // end while

        /// Buscamos Query's por tratar
        QString icon = ":/Images/template2ods.png";
        QRegExp rx2 ( "icon\\s*=\\s*\"(.*)\"" );
        rx2.setMinimal ( true );
        if ( rx2.indexIn ( buff, 0 )  != -1 ) {
            icon = rx2.cap ( 1 );
        } // end while

        QAction *accion = ajust->addAction ( titulo );
        accion->setObjectName ( fileInfo.fileName() );
	accion->setIcon(QIcon(icon));
    }
    
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void EQToolButton::trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG
    
    if (action == NULL) action = (QAction *) sender();
    
    if (action ) {
	/// Buscamos ficheros que tengan el nombre de la tabla
	QDir dir ( g_confpr->value( CONF_DIR_OPENREPORTS ) );
	dir.setFilter ( QDir::Files | QDir::NoSymLinks );
	dir.setSorting ( QDir::Size | QDir::Reversed );
	/// Hacemos un filtrado de busqueda
	QStringList filters;
	filters << "*impers_" + m_BlForm->tableName() + "*.pys";
	dir.setNameFilters ( filters );


	QFileInfoList list = dir.entryInfoList();
	for ( int i = 0; i < list.size(); ++i ) {
	    QFileInfo fileInfo = list.at ( i );
	    if ( action->objectName() == fileInfo.fileName() ) {
		if ( m_BlForm->generateRML ( fileInfo.fileName() ) ) {
		    blCreateAndLoadODS ( fileInfo.fileName().left ( fileInfo.fileName().size() - 4 ) );
		} // end if
	    } // end if
	}
    } // end if
    
}



