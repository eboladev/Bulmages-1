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
#include "ticketqtoolbutton.h"
#include "blfunctions.h"

#include <QtCore/QString>
#include <QtWidgets/QFileDialog>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QTextStream>
#include "blfixed.h"
#include "bfcompany.h"
#include "bldb.h"
#include "blfile.h"


///
/**
\param alb
\param parent
**/
TicketQToolButton::TicketQToolButton ( AlbaranProveedorView *alb, QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
    m_albaranProveedorView = alb;
    setBoton();
    
}

///
TicketQToolButton::~TicketQToolButton()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void TicketQToolButton::setBoton()
{
    BL_FUNC_DEBUG
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( _ ( "Imprimir Etiquetas" ) );
    setToolTip ( _ ( "Imprimir Etiquetas" ) );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/ticket.png" ) );
    setIconSize ( QSize ( 22, 22 ) );
    
}


///
/**
**/
void TicketQToolButton::click()
{
    BL_FUNC_DEBUG
    QString txt = "";

    /// Copiamos el archivo.
    QString archivo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "etiquetas.rml";
    QString archivod = g_confpr->value( CONF_DIR_USER ) + "etiquetas.rml";
    blCopyFile(archivo,archivod);

    BlFile file;
    file.setFileName ( archivod );
    file.open ( QIODevice::ReadOnly );
    QTextStream stream ( &file );
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt = "";


    /// Hacemos el texto de las etiquetas.
    m_companyact = m_albaranProveedorView->mainCompany();

    QString query = "SELECT * , ceil(cantlalbaranp) AS cantidad FROM lalbaranp NATURAL LEFT JOIN articulo WHERE idalbaranp = " + m_albaranProveedorView->dbValue ( "idalbaranp" );
    BlDbRecordSet *cur = m_companyact->loadQuery ( query );
    while ( !cur->eof() ) {
        int i = 0;
        while ( i < cur->value( "cantidad" ).toInt() ) {
            fitxersortidatxt += "<blockTable><tr><td>";
            fitxersortidatxt += "<para><font face=\"Helvetica\" size=\"4\">" + cur->value( "nomarticulo" ) + "</font></para>\n";
            fitxersortidatxt += "<barCode code=\"code128\" height=\"0.60cm\">" + cur->value( "codigocompletoarticulo" ) + "</barCode>\n";
            fitxersortidatxt += "<para><font face=\"Helvetica\" size=\"4\">" + cur->value( "codigocompletoarticulo" ) + " - (" + m_albaranProveedorView->dbValue ( "fechaalbaranp" ).left ( 10 ) + ")</font></para>\n";
//     if (cur->numcampo("lotelalbaranp") != -1)
//             fitxersortidatxt += "<para><font face=\"Helvetica\" size=\"4\"> Lote: " + cur->value("lotelalbaranp") + "</font></para>\n";
            fitxersortidatxt += "</td></tr></blockTable>";
            fitxersortidatxt += "<spacer length=\"0.5cm\"/>\n";
            i++;
        } // end while
        cur->nextRecord();
    } // end while
    delete cur;


    buff.replace ( "[story]", fitxersortidatxt );

    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << buff;
        file.close();
    } // end if
    blCreateAndLoadPDF ( "etiquetas" );



    
}


