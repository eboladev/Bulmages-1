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

#include "blfixed.h"
#include "blmaincompany.h"
#include "bldb.h"
#include "eqtoolbutton.h"
#include "local_blI18n.h"

///
/**
\param parent
**/
EQToolButtonMail::EQToolButtonMail ( QWidget *parent ) : QWidget ( parent )
{
    BL_FUNC_DEBUG
    connect ( parent, SIGNAL ( pintaMenu ( QMenu * ) ), this, SLOT ( pintaMenu ( QMenu * ) ) );
    connect ( parent, SIGNAL ( trataMenu ( QAction * ) ), this, SLOT ( trataMenu ( QAction * ) ) );
    m_BlForm = ( BlForm * ) parent;
    
}


///
/**
**/
EQToolButtonMail::~EQToolButtonMail()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void EQToolButtonMail::pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
    QMenu *ajust = menu->addMenu ( _ ( "Inf. personales por e-mail" ) );

    /// Buscamos ficheros que tengan el nombre de la tabla
    QDir dir ( g_confpr->value( CONF_DIR_OPENREPORTS ) );
    dir.setFilter ( QDir::Files | QDir::NoSymLinks );
    dir.setSorting ( QDir::Size | QDir::Reversed );
    /// Hacemos un filtrado de busqueda
    QStringList filters;
    filters << "*impers_" + m_BlForm->tableName() + "*.rml";
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
        QRegExp rx1 ( "title\\s*=\\s*\"(.*)\"" );
        rx1.setMinimal ( true );
        if ( rx1.indexIn ( buff, 0 )  != -1 ) {
            titulo = rx1.cap ( 1 );
        } // end while


        QAction * action = ajust->addAction ( titulo );
        action->setObjectName ( "em_" + fileInfo.fileName() );
    }
    
}


///
/**
\param menu El menu sobre el que pintar la opcion
**/
void EQToolButtonMail::trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG

    /// Buscamos ficheros que tengan el nombre de la tabla
    QDir dir ( g_confpr->value( CONF_DIR_OPENREPORTS ) );
    dir.setFilter ( QDir::Files | QDir::NoSymLinks );
    dir.setSorting ( QDir::Size | QDir::Reversed );
    /// Hacemos un filtrado de busqueda
    QStringList filters;
    filters << "*" + m_BlForm->tableName() + "*.rml";
    dir.setNameFilters ( filters );


    QFileInfoList list = dir.entryInfoList();
    for ( int i = 0; i < list.size(); ++i ) {
        QFileInfo fileInfo = list.at ( i );

        if ( action->objectName() == "em_" + fileInfo.fileName() ) {

            if ( m_BlForm->generateRML ( fileInfo.fileName() ) ) {

                QString email = "";
                QString id = m_BlForm->dbValue ( "id" + m_BlForm->tableName() );
                QString num = m_BlForm->dbValue ( "num" + m_BlForm->tableName() );
                QString ref = m_BlForm->dbValue ( "ref" + m_BlForm->tableName() );
                QString idcliente = m_BlForm->dbValue ( "idcliente" );
                if ( !idcliente.isEmpty() ) {
                    QString query = "SELECT mailcliente from cliente WHERE idcliente=" + idcliente;
                    BlDbRecordSet *curs = ( ( BlForm * ) parent() )->mainCompany()->loadQuery ( query );
                    if ( !curs->eof() ) {
                        email = curs->value( "mailcliente" );
                    } // end if
                    delete curs;
                } // end if

                QString idproveedor = m_BlForm->dbValue ( "idproveedor" );
                if ( !idproveedor.isEmpty() ) {
                    QString query = "SELECT emailproveedor from proveedor WHERE idproveedor=" + idproveedor;
                    BlDbRecordSet *curs = ( ( BlForm * ) parent() )->mainCompany()->loadQuery ( query );
                    if ( !curs->eof() ) {
                        email = curs->value( "emailproveedor" );
                    } // end if
                    delete curs;
                } // end if

                QString doc = fileInfo.fileName().left ( fileInfo.fileName().size() - 4 );
                blCreatePDF ( doc );

                QString oldName = g_confpr->value( CONF_DIR_USER ) + doc + ".pdf";
                QString newName = g_confpr->value( CONF_DIR_USER ) +   doc  + num + ".pdf";
                blMoveFile(oldName,newName);
#ifdef Q_OS_WIN32
		/// En windows las rutas relativas no funcionan bien con algunos sistemas de e-mail
		/// Por eso pasamos una posible ruta relativa a absoluta.
		newName = QDir(g_confpr->value( CONF_DIR_USER )).absolutePath() + "/"+ doc + num + ".pdf";
#endif
		QString subject = doc + num;
		QString body = "Adjunto remito " + doc + " numero " + num + ". Con referencia " + ref + "\n Atentamente\n";
		QString bcc= "";

		blSendEmail( email, bcc, subject , body, newName );


            } // end if
        } // end if
    }
    
}



