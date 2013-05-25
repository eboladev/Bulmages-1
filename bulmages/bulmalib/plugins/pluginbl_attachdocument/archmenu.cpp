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


#include "blfixed.h"
#include "blmaincompany.h"
#include "bldb.h"
#include "archmenu.h"
#include "archivo.h"
#include "blfunctions.h"

///
/**
\param parent
**/
ADQToolButton::ADQToolButton ( QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
   /// Buscamos alguna otra instancia y si la hay nos quitamos de enmedio
    ADQToolButton *tool = parent->findChild<ADQToolButton *>("BotonArchDoc");
    if (tool) {
      hide();
      return;
    } // end if
    setObjectName("BotonArchDoc");

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
	setIcon ( QIcon ( ":/Images/attach_document.png" ) );
	setIconSize ( QSize ( 32, 32 ) );  	
	setPopupMode(QToolButton::InstantPopup);  
	
	hazMenu();

    } else {
        hide();
    } // end if
    
}


void ADQToolButton::hazMenu() {
  	/// Creamos el menu
	QMenu *menu = new QMenu(this);
	
	QAction *addaction = new QAction( _ ( "Agregar Archivo " ) , m_BlForm);
	addaction->setIcon(QIcon( ":/Images/attach_document.png" ));
	menu->addAction (addaction);
	addaction->setObjectName ( "addarchivo" );
	connect ( addaction, SIGNAL ( triggered ( bool ) ), this, SLOT ( trataMenu (  ) ) );
	menu->addSeparator();
	QString query = "SELECT * FROM archivo WHERE fichaarchivo = '" + m_BlForm->fieldId() + "' AND identificadorfichaarchivo= '" + m_BlForm->dbValue ( m_BlForm->fieldId() ) + "'";
	BlDbRecordSet *cur = m_BlForm->mainCompany()->loadQuery ( query );
	while ( !cur->eof() ) {
	    QMenu *n1menu = menu->addMenu ( cur->value( "rutaarchivo" ) );
	    QAction *addaction1 = new QAction ( _ ( "Abrir"), m_BlForm );
	    n1menu->addAction (addaction1);
	    addaction1->setObjectName ( "abrir_archivo_" + cur->value( "idarchivo" ) );
	    connect ( addaction1, SIGNAL ( triggered ( bool ) ), this, SLOT ( trataMenu (  ) ) );
		    
	    QAction *delaction = new QAction ( _ ( "Borrar"), m_BlForm );
	    n1menu->addAction (delaction);
	    delaction->setObjectName ( "borrar_archivo_" + cur->value( "idarchivo" ) );
	    connect ( delaction, SIGNAL ( triggered ( bool ) ), this, SLOT ( trataMenu ( ) ) );
	    
	    cur->nextRecord();
	} // end while
	delete cur;
	
	setMenu(menu);
}

///
/**
**/
ADQToolButton::~ADQToolButton()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void ADQToolButton::pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
    QMenu *ajust = menu->addMenu (QIcon(":/Images/attach_document.png"), _ ( "Archivo Documental" ) );

    
	QAction *addaction = new QAction( _ ( "Agregar Archivo " ) , m_BlForm);
	addaction->setIcon(QIcon( ":/Images/attach_document.png" ));
	ajust->addAction (addaction);
	addaction->setObjectName ( "addarchivo" );
	ajust->addSeparator();
	QString query = "SELECT * FROM archivo WHERE fichaarchivo = '" + m_BlForm->fieldId() + "' AND identificadorfichaarchivo= '" + m_BlForm->dbValue ( m_BlForm->fieldId() ) + "'";
	BlDbRecordSet *cur = m_BlForm->mainCompany()->loadQuery ( query );
	while ( !cur->eof() ) {
	    QMenu *n1menu = ajust->addMenu ( cur->value( "rutaarchivo" ) );
	    QAction *addaction1 = new QAction ( _ ( "Abrir"), m_BlForm );
	    n1menu->addAction (addaction1);
	    addaction->setObjectName ( "abrir_archivo_" + cur->value( "idarchivo" ) );
	    
	    QAction *delaction = new QAction ( _ ( "Borrar"), m_BlForm );
	    n1menu->addAction (delaction);
	    delaction->setObjectName ( "borrar_archivo_" + cur->value( "idarchivo" ) );
	    
	    cur->nextRecord();
	} // end while
	delete cur;

    
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void ADQToolButton::trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG
    
     if (action == NULL) action = (QAction *) sender();
    

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
        diag->setWindowTitle ( _("Agregar Archivo Documental") );

	

        if ( diag->exec() ) {
            QString query = "INSERT INTO archivo (fichaarchivo, identificadorfichaarchivo, rutaarchivo) VALUES ('" + m_BlForm->fieldId() + "', '" + m_BlForm->dbValue ( m_BlForm->fieldId() ) + "' , '" + camb->mui_archivo->text() + "') ";
            m_BlForm->mainCompany()->runQuery ( query );
        } // end if

        delete diag;
	hazMenu();
    } // end if

    if ( action->objectName().left ( 14 ) == "abrir_archivo_" ) {
        QString idarchivo = action->objectName().right ( action->objectName().size() - 14 );
        QString query = "SELECT * FROM archivo WHERE idarchivo = " + idarchivo;
        BlDbRecordSet *cur = m_BlForm->mainCompany()->loadQuery ( query );
        if ( !cur->eof() ) {
            blWebBrowser(cur->value( "rutaarchivo" ));
        } // end if
        delete cur;
    } // end if


    if ( action->objectName().left ( 15 ) == "borrar_archivo_" ) {
        QString idarchivo = action->objectName().right ( action->objectName().size() - 15 );
        QString query = "DELETE FROM archivo WHERE idarchivo = " + idarchivo;
        m_BlForm->mainCompany()->runQuery ( query );
	hazMenu();
    } // end if

    
    
}




