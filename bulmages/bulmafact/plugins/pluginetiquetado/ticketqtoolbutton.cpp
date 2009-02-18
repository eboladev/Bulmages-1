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
#include "ticketqtoolbutton.h"
#include "blfunctions.h"

#include <QString>
#include <QFileDialog>
#include <QMap>
#include <QList>
#include <QTextStream>
#include "blfixed.h"
#include "bfcompany.h"
#include "bldb.h"


///
/**
\param alb
\param parent
**/
TicketQToolButton::TicketQToolButton ( AlbaranProveedorView *alb, QWidget *parent ) : QToolButton ( parent )
{
    _depura ( "TicketQToolButton::TicketQToolButton", 0 );
    m_albaranProveedorView = alb;
    setBoton();
    _depura ( "END TicketQToolButton::TicketQToolButton", 0 );
}

///
TicketQToolButton::~TicketQToolButton()
{
    _depura ( "TicketQToolButton::~TicketQToolButton", 0 );
    _depura ( "END TicketQToolButton::~TicketQToolButton", 0 );
}


///
/**
**/
void TicketQToolButton::setBoton()
{
    _depura ( "TicketQToolButton::setBoton", 0 );
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( _("Imprimir Etiquetas") );
    setToolTip ( _("Imprimir Etiquetas") );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( confpr->valor ( CONF_PROGDATA ) + "icons/ticket.png" ) );
    setIconSize ( QSize ( 22, 22 ) );
    _depura ( "END TicketQToolButton::setBoton", 0 );
}


///
/**
**/
void TicketQToolButton::click()
{
    _depura ( "ImpQToolButton::click", 2 );
    QString txt = "";

    QString archivo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "etiquetas.rml";
    QString archivod = confpr->valor ( CONF_DIR_USER ) + "etiquetas.rml";
    QString archivologo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system ( archivo.toAscii().constData() );

    /// Copiamos el logo.
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#endif

    QFile file;
    file.setFileName ( archivod );
    file.open ( QIODevice::ReadOnly );
    QTextStream stream ( &file );
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt = "";


    /// Hacemos el texto de las etiquetas.
    m_companyact = m_albaranProveedorView->mainCompany();

    QString query = "SELECT * , ceil(cantlalbaranp) AS cantidad FROM lalbaranp NATURAL LEFT JOIN articulo WHERE idalbaranp = " + m_albaranProveedorView->DBvalue ( "idalbaranp" );
    BlDbRecordSet *cur = m_companyact->cargacursor ( query );
    while ( !cur->eof() ) {
        int i = 0;
        while ( i < cur->valor ( "cantidad" ).toInt() ) {
            fitxersortidatxt += "<blockTable><tr><td>";
            fitxersortidatxt += "<para><font face=\"Helvetica\" size=\"4\">" + cur->valor ( "nomarticulo" ) + "</font></para>\n";
            fitxersortidatxt += "<barCode code=\"code128\" height=\"0.60cm\">" + cur->valor ( "codigocompletoarticulo" ) + "</barCode>\n";
            fitxersortidatxt += "<para><font face=\"Helvetica\" size=\"4\">" + cur->valor ( "codigocompletoarticulo" ) + " - (" + m_albaranProveedorView->DBvalue ( "fechaalbaranp" ).left ( 10 ) + ")</font></para>\n";
//     if (cur->numcampo("lotelalbaranp") != -1)
//             fitxersortidatxt += "<para><font face=\"Helvetica\" size=\"4\"> Lote: " + cur->valor("lotelalbaranp") + "</font></para>\n";
            fitxersortidatxt += "</td></tr></blockTable>";
            fitxersortidatxt += "<spacer length=\"0.5cm\"/>\n";
            i++;
        } // end while
        cur->siguienteregistro();
    } // end while
    delete cur;


    buff.replace ( "[story]", fitxersortidatxt );

    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << buff;
        file.close();
    } // end if
    invocaPDF ( "etiquetas" );



    _depura ( "END ImpQToolButton::click", 0 );
}


