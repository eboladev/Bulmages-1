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

#include <stdio.h>
#include <stdlib.h>

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QFileDialog>
#include <QMap>
#include <QList>
#include <QWidget>

#include "catalogoqtoolbutton.h"
#include "blfunctions.h"
#include "blfixed.h"
#include "bfcompany.h"
#include "bldb.h"
#include "blprogressbar.h"


///
/**
\param art
\param parent
**/
CatalogoQToolButton::CatalogoQToolButton ( ArticuloList *art , QWidget *parent ) : QToolButton ( parent ), BlMainCompanyPointer()
{
    BL_FUNC_DEBUG
    m_articuloList = art;
    setBoton();
    
}


///
/**
**/
CatalogoQToolButton::~CatalogoQToolButton()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void CatalogoQToolButton::setBoton()
{
    BL_FUNC_DEBUG
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( _("Imprimir catalogo") );
    setToolTip ( _("Imprimir catalogo") );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/catalogo.png" ) );
    setIconSize ( QSize ( 22, 22 ) );
    
}


///
/**
**/
void CatalogoQToolButton::click()
{
    BL_FUNC_DEBUG

    setMainCompany ( m_articuloList->mainCompany() );
    
    /// Copiamos el archivo.
    QString archivo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "articulos.rml";
    QString archivod = g_confpr->value( CONF_DIR_USER ) + "articulos.rml";
    blCopyFile(archivo,archivod);

    /// Copiamos el logo.
    QString archivologo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "logo.jpg";
    QString logousuario = g_confpr->value( CONF_DIR_USER ) + "logo.jpg";
    blCopyFile(archivologo,logousuario);
    
    QFile file;
    file.setFileName ( archivod );
    file.open ( QIODevice::ReadOnly );
    QTextStream stream ( &file );
    QString buff = stream.readAll();
    file.close();
    QString texto;
    /// Linea de totales del presupuesto.
    buff.replace ( "[detallearticulos]", detalleArticulos() );
    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << buff;
        file.close();
    } // end if
    blCreateAndLoadPDF ( "articulos" );

    
}



/** Metodo auxiliar para la creacion del catalogo de articulos
*/
/**
\return
**/
QString CatalogoQToolButton::detalleArticulos()
{
    BL_FUNC_DEBUG
    QString texto = "";

    BlProgressBar barra;
    barra.show();
    barra.setText ( _ ( "Generando catalogo" ) );


    BlDbRecordSet *cur = mainCompany() ->loadQuery ( m_articuloList->formaQuery() );
    barra.setRange ( 0, cur->numregistros() );
    barra.setValue ( 0 );
    while ( !cur->eof() ) {
        texto += "<blockTable style=\"tabla1\">\n";
        texto += "<tr>\n";
        texto += "<td><para><H1>" + blXMLEncode ( cur->value( "nomarticulo" ) ) + "</H1></para>";
        texto += "<para>" + blXMLEncode ( cur->value( "obserarticulo" ) ) + "</para></td></tr><tr>\n";
        QString file = g_confpr->value( CONF_DIR_IMG_ARTICLES ) + blXMLEncode ( cur->value( "codigocompletoarticulo" ) ) + ".jpg";
        QFile f ( file );
        if ( f.exists() ) {
            texto += "<td><!-- illustration x=\"0\" y=\"0\" height=\"5cm\" -->\n"
                     "<image file=\"" + g_confpr->value( CONF_DIR_IMG_ARTICLES ) +
                     blXMLEncode ( cur->value( "codigocompletoarticulo" ) ) +
                     ".jpg\" x=\"0\" y=\"0\" height=\"5cm\"/>\n"
                     "<!-- /illustration --></td>\n";
        } else {
            texto += "<td></td>\n";
        }
        texto += "</tr>\n";
        texto += "</blockTable>";
        cur->nextRecord();
        barra.setValue ( barra.value() + 1 );
    } // end while
    delete cur;
    
    return texto;
}

