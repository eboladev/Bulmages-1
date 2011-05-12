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

#include <QDesktopServices>
#include <QUrl>

#include "blfixed.h"
#include "blmaincompany.h"
#include "bldb.h"
#include "archmenu.h"
#include "archivo.h"


///
/**
\param parent
**/
ArchMenu::ArchMenu ( QWidget *parent ) : QWidget ( parent )
{
    blDebug ( "ArchMenu::ArchMenu", 0 );
    connect ( parent, SIGNAL ( pintaMenu ( QMenu * ) ), this, SLOT ( pintaMenu ( QMenu * ) ) );
    connect ( parent, SIGNAL ( trataMenu ( QAction * ) ), this, SLOT ( trataMenu ( QAction * ) ) );
    m_BlForm = ( BlForm * ) parent;
    blDebug ( "END ArchMenu::ArchMenu", 0 );
}


///
/**
**/
ArchMenu::~ArchMenu()
{
    blDebug ( "ArchMenu::~ArchMenu", 0 );
    blDebug ( "END ArchMenu::~ArchMenu", 0 );
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void ArchMenu::pintaMenu ( QMenu *menu )
{
    blDebug ( "ArchMenu::pintaMenu", 0 );
    QMenu *nmenu = menu->addMenu ( tr ( "Archivo Documental" ) );
    QAction *addaction = nmenu->addAction ( tr ( "Agregar Archivo " ) );
    addaction->setObjectName ( "addarchivo" );
    nmenu->addSeparator();
    QString query = "SELECT * FROM archivo WHERE fichaarchivo = '" + m_BlForm->fieldId() + "' AND identificadorfichaarchivo= '" + m_BlForm->dbValue ( m_BlForm->fieldId() ) + "'";
    BlDbRecordSet *cur = m_BlForm->mainCompany()->loadQuery ( query );
    while ( !cur->eof() ) {
        QMenu *n1menu = nmenu->addMenu ( cur->valor ( "rutaarchivo" ) );
        QAction *addaction = n1menu->addAction ( tr ( "Abrir") );
        addaction->setObjectName ( "abrir_archivo_" + cur->valor ( "idarchivo" ) );
	
        QAction *delaction = n1menu->addAction ( tr ( "Borrar") );
        delaction->setObjectName ( "borrar_archivo_" + cur->valor ( "idarchivo" ) );
	
        cur->nextRecord();
    } // end while
    delete cur;
    blDebug ( "END ArchMenu::pintaMenu", 0 );
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void ArchMenu::trataMenu ( QAction *action )
{
    blDebug ( "ArchMenu::trataMenu", 0 );
    if ( action->objectName() == "addarchivo" ) {
        QDialog *diag = new QDialog;
        Archivo *camb = new Archivo ( m_BlForm->mainCompany(), diag );
        diag->setModal ( true );

        QPushButton *button = camb->findChild<QPushButton *> ( "mui_aceptar" );
        connect ( button, SIGNAL ( released ( ) ), diag, SLOT ( accept() ) );
        QPushButton *button1 = camb->findChild<QPushButton *> ( "mui_cancelar" );
        connect ( button1, SIGNAL ( released ( ) ), diag, SLOT ( reject() ) );

        /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
        /// para que sea redimensionable y aparezca el titulo de la ventana.
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget ( camb );
        layout->setMargin ( 0 );
        layout->setSpacing ( 0 );
        diag->setLayout ( layout );
        diag->setWindowTitle ( "Agregar Archivo Documental" );

        if ( diag->exec() ) {
            QString query = "INSERT INTO archivo (fichaarchivo, identificadorfichaarchivo, rutaarchivo) VALUES ('" + m_BlForm->fieldId() + "', '" + m_BlForm->dbValue ( m_BlForm->fieldId() ) + "' , '" + camb->mui_archivo->text() + "') ";
            m_BlForm->mainCompany()->runQuery ( query );
        } // end if

        delete diag;
    } // end if

    if ( action->objectName().left ( 14 ) == "abrir_archivo_" ) {
        QString idarchivo = action->objectName().right ( action->objectName().size() - 14 );
        QString query = "SELECT * FROM archivo WHERE idarchivo = " + idarchivo;
        BlDbRecordSet *cur = m_BlForm->mainCompany()->loadQuery ( query );
        if ( !cur->eof() ) {
             QDesktopServices::openUrl(QUrl( cur->valor ( "rutaarchivo" ), QUrl::TolerantMode));
            //QString comando = "konqueror " + cur->valor ( "rutaarchivo" ) + " &";
           // system ( comando.toAscii() );
        } // end if
        delete cur;
    } // end if


    if ( action->objectName().left ( 15 ) == "borrar_archivo_" ) {
        QString idarchivo = action->objectName().right ( action->objectName().size() - 15 );
        QString query = "DELETE FROM archivo WHERE idarchivo = " + idarchivo;
        m_BlForm->mainCompany()->runQuery ( query );
    } // end if

    blDebug ( "END ArchMenu::trataMenu", 0 );
    return;
}



