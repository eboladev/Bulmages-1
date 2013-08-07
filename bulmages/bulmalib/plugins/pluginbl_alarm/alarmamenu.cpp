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
#include "alarmamenu.h"
#include "alarma.h"
#include "blfunctions.h"

///
/**
\param parent
**/
AlQToolButton::AlQToolButton ( QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
   /// Buscamos alguna otra instancia y si la hay nos quitamos de enmedio
    AlQToolButton *tool = parent->findChild<AlQToolButton *>("BotonAlarm");
    if (tool) {
      hide();
      return;
    } // end if
    setObjectName("BotonAlarm");

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
	setIcon ( QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/chrono.png" ) );
	setIconSize ( QSize ( 32, 32 ) );  	
	setPopupMode(QToolButton::InstantPopup);  
	
	hazMenu();

    } else {
        hide();
    } // end if
    
}


void AlQToolButton::hazMenu() {
  	/// Creamos el menu
	QMenu *menu = new QMenu(this);
	
	QAction *addaction = new QAction( _ ( "Agregar Alarma " ) , m_BlForm);
	addaction->setIcon(QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/chrono.png" ));
	menu->addAction (addaction);
	addaction->setObjectName ( "addalarma" );
	connect ( addaction, SIGNAL ( triggered ( bool ) ), this, SLOT ( trataMenu (  ) ) );
	menu->addSeparator();
	QString query = "SELECT * FROM alarma WHERE fichaalarma = '" + m_BlForm->tableName() + "' AND idfichaalarma= '" + m_BlForm->dbValue ( m_BlForm->fieldId() ) + "'";
	BlDbRecordSet *cur = m_BlForm->mainCompany()->loadQuery ( query );
	while ( !cur->eof() ) {
	    QMenu *n1menu = menu->addMenu ( cur->value( "subjectalarma" ) );
	    QAction *addaction1 = new QAction ( _ ( "Abrir"), m_BlForm );
	    n1menu->addAction (addaction1);
	    addaction1->setObjectName ( "abrir_alarma_" + cur->value( "idalarma" ) );
	    connect ( addaction1, SIGNAL ( triggered ( bool ) ), this, SLOT ( trataMenu (  ) ) );
		    
	    QAction *delaction = new QAction ( _ ( "Borrar"), m_BlForm );
	    n1menu->addAction (delaction);
	    delaction->setObjectName ( "borrar_alarma_" + cur->value( "idalarma" ) );
	    connect ( delaction, SIGNAL ( triggered ( bool ) ), this, SLOT ( trataMenu ( ) ) );
	    
	    cur->nextRecord();
	} // end while
	delete cur;
	
	setMenu(menu);
}

///
/**
**/
AlQToolButton::~AlQToolButton()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void AlQToolButton::pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
    QMenu *ajust = menu->addMenu (QIcon(":/Images/chrono.png"), _ ( "Avisos y alarmas" ) );

    
	QAction *addaction = new QAction( _ ( "Agregar Alarma " ) , m_BlForm);
	addaction->setIcon(QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/chrono.png" ));
	ajust->addAction (addaction);
	addaction->setObjectName ( "addalarma" );
	ajust->addSeparator();
	QString query = "SELECT * FROM alarma WHERE fichaalarma = '" + m_BlForm->tableName() + "' AND idfichaalarma= '" + m_BlForm->dbValue ( m_BlForm->fieldId() ) + "'";
	BlDbRecordSet *cur = m_BlForm->mainCompany()->loadQuery ( query );
	while ( !cur->eof() ) {
	    QMenu *n1menu = ajust->addMenu ( cur->value( "subjectalarma" ) );
	    QAction *addaction1 = new QAction ( _ ( "Abrir"), m_BlForm );
	    n1menu->addAction (addaction1);
	    addaction->setObjectName ( "abrir_alarma_" + cur->value( "idalarma" ) );
	    
	    QAction *delaction = new QAction ( _ ( "Borrar"), m_BlForm );
	    n1menu->addAction (delaction);
	    delaction->setObjectName ( "borrar_alarma_" + cur->value( "idalarma" ) );
	    
	    cur->nextRecord();
	} // end while
	delete cur;

    
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void AlQToolButton::trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG
    
     if (action == NULL) action = (QAction *) sender();
    

    if ( action->objectName() == "addalarma" ) {
      

        QDialog *diag = new QDialog;
        Alarma *camb = new Alarma ( m_BlForm->mainCompany(), diag );
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
        diag->setWindowTitle ( _("Agregar Alarma") );

	

        if ( diag->exec() ) {
            QString query = "INSERT INTO alarma (fichaalarma, idfichaalarma, subjectalarma, usuarioalarma, fechaalarma, horaalarma, textoalarma) VALUES ('" + m_BlForm->tableName() + "', '" + m_BlForm->dbValue ( m_BlForm->fieldId() ) + "' , '" + camb->mui_subjectalarma->text() + "', '"+g_confpr->value(CONF_LOGIN_USER)+"', '"+camb->mui_fechaalarma->text()+"', '"+camb->mui_horaalarma->text()+"', '"+camb->mui_textoalarma->toPlainText()+"') ";
            m_BlForm->mainCompany()->runQuery ( query );
        } // end if

        delete diag;
	hazMenu();
    } // end if

    if ( action->objectName().left ( 13 ) == "abrir_alarma_" ) {
        QString idalarma = action->objectName().right ( action->objectName().size() - 13 );
        QString query = "SELECT * FROM alarma WHERE idalarma = " + idalarma;
        BlDbRecordSet *cur = m_BlForm->mainCompany()->loadQuery ( query );
        if ( !cur->eof() ) {
	    QDialog *diag = new QDialog;
	    Alarma *camb = new Alarma ( m_BlForm->mainCompany(), diag );
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
	    diag->setWindowTitle ( _("Agregar Alarma") );

	    camb->mui_fechaalarma->setText(cur->value("fechaalarma"));
	    camb->mui_subjectalarma->setText(cur->value("subjectalarma"));
	    camb->mui_horaalarma->setTime(QTime::fromString(cur->value("horaalarma"), "hh:ss"));
	    camb->mui_textoalarma->setText(cur->value("textoalarma"));
	    if ( diag->exec() ) {
	    } // end if

	    delete diag;
        } // end if
        delete cur;
    } // end if


    if ( action->objectName().left ( 14 ) == "borrar_alarma_" ) {
        QString idalarma = action->objectName().right ( action->objectName().size() - 14 );
        QString query = "DELETE FROM alarma WHERE idalarma = " + idalarma;
        m_BlForm->mainCompany()->runQuery ( query );
	hazMenu();
    } // end if

    
    
}




