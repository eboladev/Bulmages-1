/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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
#include <QToolButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QFile>
#include <QTextStream>

#include "pluginticketbasico.h"
#include "funcaux.h"
#include "ticket.h"
#include "empresatpv.h"
#include "mticket.h"


QDockWidget *g_doc1;

MTicket *g_bud;



///
/**
\return
**/
int entryPoint ( BulmaTPV *tpv )
{
    _depura ( "entryPoint", 0 );

    /// Vamos a probar con un docwindow.
    g_doc1 = new QDockWidget ( "Ticket", tpv );
    g_doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );
//    g_doc1->setFeatures ( QDockWidget::DockWidgetMovable |  QDockWidget::DockWidgetFloatable);

    g_doc1->setGeometry ( 100, 100, 100, 500 );
    g_doc1->resize ( 330, 400 );



    /// Si existe el archivo de configuracion lo cargamos y configuramos el aspecto del widget.
// ============================
    QFile file ( confpr->valor ( CONF_DIR_USER ) + "pluginticketbasico.cfn" );
    QString line;
    if ( file.open ( QIODevice::ReadOnly ) ) {
        QTextStream stream ( &file );
        /// Establecemos la columna de ordenaci&oacute;n
        QString linea = stream.readLine();
        int width = linea.toInt();

        /// Establecemos el tipo de ordenaci&oacute;n
        linea = stream.readLine();
	int height = linea.toInt();

        g_doc1->resize ( width, height );
	g_doc1->setMinimumWidth(width);
	g_doc1->setMinimumHeight(height);

	file.close();
    } // end if

    /// Presentamos el widget
    tpv->addDockWidget ( Qt::RightDockWidgetArea, g_doc1 );
    g_doc1->show();


    _depura ( "END entryPoint", 0 );
    return 0;
}


///
/**
\return
**/
int exitPoint ( BulmaTPV *tpv )
{
    _depura ( "entryPoint", 0 );

    /// Vamos a probar con un docwindow.
    int width = g_doc1->width();
    int height = g_doc1->height();

/*
    QString aux = "";
    QFile file ( confpr->valor ( CONF_DIR_USER ) + "pluginticketbasico.cfn" );
    /// Guardado del orden y de configuraciones varias.
    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << width << "\n";
        stream << height << "\n";
        file.close();
    } // end if
*/

    _depura ( "END entryPoint", 0 );
    return 0;
}

int EmpresaTPV_createMainWindows_Post ( EmpresaTPV *etpv )
{
    g_bud =  new MTicket ( etpv, g_doc1 );
    g_doc1->setWidget ( ( QWidget * ) g_bud );

    return 0;
}

int Ticket_pintar ( Ticket * )
{
    g_bud->pintar();
    return 0;
}

