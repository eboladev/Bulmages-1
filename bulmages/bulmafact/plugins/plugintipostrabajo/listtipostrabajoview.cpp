/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C. (Porting to QT4)              *
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

#include <QMessageBox>
#include <QWidget>
#include <QComboBox>
#include <QToolButton>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>

#include <fstream>

#include "company.h"
#include "configuracion.h"
#include "listtipostrabajoview.h"

#include "funcaux.h"


///
/**
\param comp
\param parent
**/
ListTiposTrabajoView::ListTiposTrabajoView ( Company *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "ListTiposTrabajoView::ListTiposTrabajoView", 1 );
    setDBTableName ( _( "almacen" ) );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_listado->setEmpresaBase ( comp );

    mui_listado->setDBTableName ( "tipotrabajo" );
    mui_listado->setDBCampoId ( "idtipotrabajo" );
    mui_listado->addSHeader ( "idtipotrabajo", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, _( "Id tipo trabajo" ) );
    mui_listado->addSHeader ( "nomtipotrabajo", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, _( "Nombre tipo trabajo" ) );
    mui_listado->addSHeader ( "colortipotrabajo", DBCampo::DBvarchar, DBCampo::DBNothing , SHeader::DBNone, _( "Color tipo trabajo" ) );
    mui_listado->setinsercion ( TRUE );

    mui_listado->cargar ( "SELECT * FROM tipotrabajo" );
    meteWindow ( windowTitle(), this );
    _depura ( "END ListTiposTrabajoView::ListTiposTrabajoView", 1 );
}


///
/**
**/
void ListTiposTrabajoView::on_mui_aceptar_clicked()
{
    try {
        mui_listado->guardar();
        close();
    } catch ( ... ) {
        mensajeInfo ( "Error al guardar los tipos de trabajo" );
    } // end try
}


///
/**
**/
ListTiposTrabajoView::~ListTiposTrabajoView()
{
    _depura ( "ListTiposTrabajoView::~ListTiposTrabajoView", 0 );
    _depura ( "END ListTiposTrabajoView::~ListTiposTrabajoView", 0 );
}


