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
EQToolButtonMail::EQToolButtonMail ( QWidget *parent ) : QWidget ( parent )
{
    _depura ( "EQToolButtonMail::EQToolButtonMail", 0 );
    connect ( parent, SIGNAL ( pintaMenu ( QMenu * ) ), this, SLOT ( pintaMenu ( QMenu * ) ) );
    connect ( parent, SIGNAL ( trataMenu ( QAction * ) ), this, SLOT ( trataMenu ( QAction * ) ) );
    m_BlForm = ( BlForm * ) parent;
    _depura ( "END EQToolButtonMail::EQToolButtonMail", 0 );
}


///
/**
**/
EQToolButtonMail::~EQToolButtonMail()
{
    _depura ( "EQToolButtonMail::~EQToolButtonMail", 0 );
    _depura ( "END EQToolButtonMail::~EQToolButtonMail", 0 );
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void EQToolButtonMail::pintaMenu ( QMenu *menu )
{
    _depura ( "EQToolButtonMail::pintaMenu", 0 );
    QMenu *ajust = menu->addMenu ( _( "Inf. personales por e-mail" ) );

    /// Buscamos ficheros que tengan el nombre de la tabla
    QDir dir ( confpr->valor ( CONF_DIR_OPENREPORTS ) );
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
        file.setFileName ( confpr->valor ( CONF_DIR_OPENREPORTS ) + fileInfo.fileName() );
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


        QAction * action = ajust->addAction ( titulo );
        action->setObjectName ( "em_"+fileInfo.fileName() );
    }
    _depura ( "END EQToolButtonMail::pintaMenu", 0 );
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void EQToolButtonMail::trataMenu ( QAction *action )
{
    _depura ( "EQToolButtonMail::trataMenu", 0 );

    /// Buscamos ficheros que tengan el nombre de la tabla
    QDir dir ( confpr->valor ( CONF_DIR_OPENREPORTS ) );
    dir.setFilter ( QDir::Files | QDir::NoSymLinks );
    dir.setSorting ( QDir::Size | QDir::Reversed );
    /// Hacemos un filtrado de busqueda
    QStringList filters;
    filters << "*" + m_BlForm->tableName() + "*.rml";
    dir.setNameFilters ( filters );


    QFileInfoList list = dir.entryInfoList();
    for ( int i = 0; i < list.size(); ++i ) {
        QFileInfo fileInfo = list.at ( i );

        if ( action->objectName() == "em_"+fileInfo.fileName() ) {

            if(m_BlForm->generaRML ( fileInfo.fileName() )) {

		QString email = "";
		QString id = m_BlForm->DBvalue ( "id"+m_BlForm->tableName() );
		QString num = m_BlForm->DBvalue ( "num"+m_BlForm->tableName() );
		QString ref = m_BlForm->DBvalue ( "ref"+m_BlForm->tableName() );
		QString idcliente = m_BlForm->DBvalue ( "idcliente" );
		if (!idcliente.isEmpty()) {
			QString query = "SELECT mailcliente from cliente WHERE idcliente=" + idcliente;
			BlDbRecordSet *curs = ((BlForm *)parent())->empresaBase()->cargacursor ( query );
			if (!curs->eof()) {
				email = curs->valor ( "mailcliente" );
			} // end if
			delete curs;
		} // end if

		QString doc = fileInfo.fileName().left ( fileInfo.fileName().size() - 4 );
            	generaPDF ( doc );
	
		QString cad = "mv " + confpr->valor ( CONF_DIR_USER ) + doc + ".pdf " + confpr->valor ( CONF_DIR_USER ) +   doc  + num + ".pdf";
		system ( cad.toAscii().data() );
	
		cad = "kmail -s \" "+ doc + num + "\" --body \" Adjunto remito " + doc + " numero " + num + ". Con referencia " + ref + "\n Atentamente\n\" --attach " + confpr->valor ( CONF_DIR_USER ) + doc + num + ".pdf " + email;

		system ( cad.toAscii().data() );


	    } // end if
        } // end if
    }
    _depura ( "END EQToolButtonMail::trataMenu", 0 );
}



