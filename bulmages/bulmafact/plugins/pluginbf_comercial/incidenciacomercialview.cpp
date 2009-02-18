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

#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QFile>
#include <QTextStream>
#include <QLayout>
#include <QCloseEvent>

#include "incidenciacomercialview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "busquedafecha.h"
#include "blfunctions.h"

#define coma "'"


///
/**
\param comp
\param parent
**/
IncidenciaComercialView::IncidenciaComercialView ( BfCompany *comp, QWidget *parent )
        : IncidenciaComercial ( comp, parent )
{
    _depura ( "IncidenciaComercialView::IncidenciaComercialView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    dialogChanges_cargaInicial();

		/// Establecemos los parametros de busqueda del Cliente
	mui_idcliente->setMainCompany(comp);
    mui_idcliente->setLabel ( _( "Cliente:" ) );
	mui_idcliente->setTableName( "cliente" );
	mui_idcliente->m_valores["cifcliente"] = "";
	mui_idcliente->m_valores["nomcliente"] = "";
    _depura ( "END IncidenciaComercialView::IncidenciaComercialView", 0 );
}


///
/**
\param parent
**/
IncidenciaComercialView::IncidenciaComercialView ( QWidget *parent )
        :  IncidenciaComercial ( NULL, parent )
{
    _depura ( "IncidenciaComercialView::IncidenciaComercialView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    dialogChanges_cargaInicial();

		/// Establecemos los parametros de busqueda del Cliente
    mui_idcliente->setLabel ( _( "Cliente:" ) );
	mui_idcliente->setTableName( "cliente" );
	mui_idcliente->m_valores["cifcliente"] = "";
	mui_idcliente->m_valores["nomcliente"] = "";
    _depura ( "END IncidenciaComercialView::IncidenciaComercialView", 0 );
}


///
/**
**/
IncidenciaComercialView::~IncidenciaComercialView()
{
    _depura ( "END IncidenciaComercialView::IncidenciaComercialView", 0 );
}


///
/**
\param e
**/
void IncidenciaComercialView::closeEvent ( QCloseEvent *e )
{
    _depura ( "closeEvent", 0 );
    if ( dialogChanges_hayCambios() ) {
        int val = QMessageBox::warning ( this,
                                         _( "Guardar la incidencia" ),
                                         _( "Desea guardar los cambios?" ), _( "&Si" ), _( "&No" ), _( "&Cancelar" ), 0, 2 );
        if ( val == 0 )
            guardar();
        if ( val == 2 )
            e->ignore();
    } // end if
}


///
/**
\param comp
**/
void IncidenciaComercialView::setMainCompany ( BfCompany *comp )
{
    _depura ( "IncidenciaComercialView::setcompany", 0 );
    BlMainCompanyPointer::setMainCompany ( comp );
    mui_idcliente->setMainCompany ( comp );
    mui_idtrabajador->setMainCompany ( comp );
    mui_idtrabajador->setId ( "" );
    _depura ( "END IncidenciaComercialView::setcompany", 0 );

}


///
/**
\return
**/
int IncidenciaComercialView::guardar()
{
    _depura ( "IncidenciaComercialView::guardar", 0 );
    if ( mui_fechaincidenciacomercial->text() == "" )
        return 0;
    setDbValue ( "fechaincidenciacomercial", mui_fechaincidenciacomercial->text() );
    setDbValue ( "idcliente", mui_idcliente->id() );
    setDbValue ( "comentincidenciacomercial", mui_comentincidenciacomercial->toPlainText() );
    setDbValue ( "idtrabajador", mui_idtrabajador->id() );
    setDbValue ( "estadoincidenciacomercial", mui_estadoincidenciacomercial->estado() );
    setDbValue ( "horaincidenciacomercial", mui_horaincidenciacomercial->text() );
    setDbValue ( "refincidenciacomercial", mui_refincidenciacomercial->text() );
    int err = IncidenciaComercial::guardar();
    _depura ( "END IncidenciaComercialView::guardar", 0 );
    return err;
}


///
/**
\param id
\return
**/
int IncidenciaComercialView::cargar ( QString id )
{
    _depura ( "IncidenciaComercialView::cargar", 0 );
    int err = IncidenciaComercial::cargar ( id );
    setWindowTitle ( _( "Incidencia comercial" ) + " " + DBvalue ( "idincidenciacomercial" ) );
    mui_fechaincidenciacomercial->setText ( DBvalue ( "fechaincidenciacomercial" ) );
    mui_idcliente->setId ( DBvalue ( "idcliente" ) );
    mui_comentincidenciacomercial->setPlainText ( DBvalue ( "comentincidenciacomercial" ) );
    mui_idtrabajador->setId ( DBvalue ( "idtrabajador" ) );
    mui_estadoincidenciacomercial->setestado ( DBvalue ( "estadoincidenciacomercial" ) );
    mui_horaincidenciacomercial->setText ( DBvalue ( "horaincidenciacomercial" ) );
    mui_refincidenciacomercial->setText ( DBvalue ( "refincidenciacomercial" ) );
    dialogChanges_cargaInicial();
    _depura ( "END IncidenciaComercialView::cargar", 0 );
    return err;
}

