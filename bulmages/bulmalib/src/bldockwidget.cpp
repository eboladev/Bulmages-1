/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#include <QObject>
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <map>

#include "bldockwidget.h"
#include "blconfiguration.h"
#include "blmainwindow.h"



///
/**
\param w
**/
void BlDockWidget::setWorkspace ( BlWorkspace *w )
{
    _depura ( "BlDockWidget::setWorkspace", 0 );
    m_pWorkspace = w;
    _depura ( "END BlDockWidget::setWorkspace", 0 );
}


///
/**
\param a
**/
BlDockWidget::BlDockWidget ( const QString & title, QWidget * parent, const QString &name , Qt::WindowFlags flags ) : QDockWidget ( title, parent, flags )
{
    _depura ( "BlDockWidget::BlDockWidget", 0 );
    setFocusPolicy ( Qt::StrongFocus );
    m_name = name;
    setObjectName ( name );
    connect ( this, SIGNAL ( dockLocationChanged ( Qt::DockWidgetArea ) ), this, SLOT ( mi_dockLocationChanged ( Qt::DockWidgetArea ) ) );
    _depura ( "END BlDockWidget::BlDockWidget", 0 );
}


///
/**
**/
BlDockWidget::~BlDockWidget()
{
    _depura ( "BlDockWidget::~BlDockWidget", 0 );
    guardaconf();
    _depura ( "END BlDockWidget::~BlDockWidget", 0 );
}



///
/**
\param visible
**/
void BlDockWidget::cambiaVisible ( bool visible )
{
    _depura ( "BlDockWidget::cambiaVisible", 0 );
    if ( visible == TRUE ) {
        this->show();
    } else {
        this->hide();
    } // end if
    _depura ( "END BlDockWidget::cambiaVisible", 0 );
}


///
/**
**/
void BlDockWidget::closeEvent ( QCloseEvent * )
{
    _depura ( "BlDockWidget::closeEvent", 0 );
    emit ( cambiaEstadoVisible ( FALSE ) );
    _depura ( "END BlDockWidget::closeEvent", 0 );
}

void BlDockWidget::mi_dockLocationChanged ( Qt::DockWidgetArea area )
{
    m_area = area;
}

void BlDockWidget::cargaconf()
{
    /// Si existe el archivo de configuracion lo cargamos y configuramos el aspecto del widget.
// ============================
    QFile file ( confpr->valor ( CONF_DIR_USER ) + m_name + ".cfn" );
    QString line;
    if ( file.open ( QIODevice::ReadOnly ) ) {
        QTextStream stream ( &file );

        // Buscamos el area
        int area = stream.readLine().toInt();
// g_theApp->((QMainWindow *)mainWidget())->
        g_main->addDockWidget ( ( Qt::DockWidgetArea ) area, this );

        /// Establecemos la columna de ordenaci&oacute;n
        QString linea = stream.readLine();
        int width = linea.toInt();

        /// Establecemos el tipo de ordenaci&oacute;n
        linea = stream.readLine();
        int height = linea.toInt();

        resize ( width, height );
//        setMinimumWidth ( width );
//        setMinimumHeight ( height );

        file.close();
    } // end if
}


void BlDockWidget::guardaconf()
{
    _depura ( "BlDockWidget::guardaconf", 0 );
    /// Vamos a probar con un docwindow.
    int lwidth = width();
    int lheight = height();

    QString aux = "";
    QFile file ( confpr->valor ( CONF_DIR_USER ) + m_name + ".cfn" );
    /// Guardado del orden y de configuraciones varias.
    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << m_area << "\n";
        stream << lwidth << "\n";
        stream << lheight << "\n";
        file.close();
    } // end if
    _depura ( "END BlDockWidget::guardaconf", 0 );
}



