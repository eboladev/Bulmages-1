/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <fstream>

#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QFile>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>
#include <QCloseEvent>

#include "rutacomercialview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"

#define coma "'"


///
/**
\param comp
\param parent
**/
RutaComercialView::RutaComercialView ( BfCompany *comp, QWidget *parent )
        :  RutaComercial ( comp, parent )
{
    _depura ( "RutaComercialView::RutaComercialView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );

	/// Establecemos los parametros de busqueda del Cliente
	mui_idcliente->setMainCompany(comp);
    mui_idcliente->setLabel ( _( "Cliente:" ) );
	mui_idcliente->setTableName( "cliente" );
	mui_idcliente->m_valores["cifcliente"] = "";
	mui_idcliente->m_valores["nomcliente"] = "";

    dialogChanges_cargaInicial();

    _depura ( "END RutaComercialView::RutaComercialView", 0 );
}


///
/**
\param parent
**/
RutaComercialView::RutaComercialView ( QWidget *parent )
        : RutaComercial ( NULL, parent )
{
    _depura ( "RutaComercialView::RutaComercialView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );

	/// Establecemos los parametros de busqueda del Cliente
    mui_idcliente->setLabel ( _( "Cliente:" ) );
	mui_idcliente->setTableName( "cliente" );
	mui_idcliente->m_valores["cifcliente"] = "";
	mui_idcliente->m_valores["nomcliente"] = "";

    dialogChanges_cargaInicial();
    _depura ( "END RutaComercialView::RutaComercialView", 0 );
}


///
/**
**/
RutaComercialView::~RutaComercialView()
{
    _depura ( "END RutaComercialView::RutaComercialView", 0 );
}


///
/**
\param comp
**/
void RutaComercialView::setMainCompany ( BfCompany *comp )
{
    _depura ( "RutaComercialView::setMainCompany", 0 );
    BlMainCompanyPointer::setMainCompany ( comp );
    mui_idcliente->setMainCompany ( comp );
    _depura ( "END RutaComercialView::setMainCompany", 0 );
}


///
/**
\return
**/
int RutaComercialView::guardar()
{
    _depura ( "RutaComercialView::guardar", 0 );
    if ( mui_fecharutacomercial->text() == "" )
        return 0;
    setDbValue ( "fecharutacomercial", mui_fecharutacomercial->text() );
    setDbValue ( "idcliente", mui_idcliente->id() );
    setDbValue ( "comentariosrutacomercial", mui_comentrutacomercial->toPlainText() );
    setDbValue ( "horarutacomercial", mui_horarutacomercial->text() );
    setDbValue ( "refrutacomercial", mui_refrutacomercial->text() );
    int err = RutaComercial::guardar();
    _depura ( "END RutaComercialView::guardar", 0 );
    return err;
}


///
/**
\param id
\return
**/
int RutaComercialView::cargar ( QString id )
{
    _depura ( "RutaComercialView::cargar", 0 );
    int err = RutaComercial::cargar ( id );
    setWindowTitle ( _( "Ruta comercial" ) + " " + dbValue ( "idrutacomercial" ) );
    mui_fecharutacomercial->setText ( dbValue ( "fecharutacomercial" ) );
    mui_idcliente->setId ( dbValue ( "idcliente" ) );
    mui_comentrutacomercial->setPlainText ( dbValue ( "comentariosrutacomercial" ) );
    mui_horarutacomercial->setText ( dbValue ( "horarutacomercial" ) );
    mui_refrutacomercial->setText ( dbValue ( "refrutacomercial" ) );
    dialogChanges_cargaInicial();
    _depura ( "END RutaComercialView::cargar", 0 );
    return err;
}




