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
#include "bldatesearch.h"
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
    blDebug ( Q_FUNC_INFO, 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    dialogChanges_readValues();

    /// Establecemos los parametros de busqueda del Cliente
    mui_idcliente->setMainCompany ( comp );
    mui_idcliente->setLabel ( _ ( "Cliente:" ) );
    mui_idcliente->setTableName ( "cliente" );
    mui_idcliente->m_valores["cifcliente"] = "";
    mui_idcliente->m_valores["nomcliente"] = "";
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param parent
**/
IncidenciaComercialView::IncidenciaComercialView ( QWidget *parent )
        :  IncidenciaComercial ( NULL, parent )
{
    blDebug ( Q_FUNC_INFO, 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    dialogChanges_readValues();

    /// Establecemos los parametros de busqueda del Cliente
    mui_idcliente->setLabel ( _ ( "Cliente:" ) );
    mui_idcliente->setTableName ( "cliente" );
    mui_idcliente->m_valores["cifcliente"] = "";
    mui_idcliente->m_valores["nomcliente"] = "";
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
IncidenciaComercialView::~IncidenciaComercialView()
{
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param e
**/
void IncidenciaComercialView::closeEvent ( QCloseEvent *e )
{
    blDebug ( Q_FUNC_INFO, 0 );
    if ( dialogChanges_isChanged() ) {
        int val = QMessageBox::warning ( this,
                                         _ ( "Guardar la incidencia" ),
                                         _ ( "Desea guardar los cambios?" ), _ ( "&Si" ), _ ( "&No" ), _ ( "&Cancelar" ), 0, 2 );
        if ( val == 0 )
            save();
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
    blDebug ( Q_FUNC_INFO, 0 );
    BlMainCompanyPointer::setMainCompany ( comp );
    mui_idcliente->setMainCompany ( comp );
    mui_idtrabajador->setMainCompany ( comp );
    mui_idtrabajador->setId ( "0" );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );

}


///
/**
\return
**/
int IncidenciaComercialView::save()
{
    blDebug ( Q_FUNC_INFO, 0 );
    if ( mui_fechaincidenciacomercial->text() == "" )
        return 0;
    setDbValue ( "fechaincidenciacomercial", mui_fechaincidenciacomercial->text() );
    setDbValue ( "idcliente", mui_idcliente->id() );
    setDbValue ( "comentincidenciacomercial", mui_comentincidenciacomercial->toPlainText() );
    setDbValue ( "idtrabajador", mui_idtrabajador->id() );
    setDbValue ( "estadoincidenciacomercial", mui_estadoincidenciacomercial->estado() );
    setDbValue ( "horaincidenciacomercial", mui_horaincidenciacomercial->text() );
    setDbValue ( "refincidenciacomercial", mui_refincidenciacomercial->text() );
    int err = IncidenciaComercial::save();
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return err;
}


///
/**
\param id
\return
**/
int IncidenciaComercialView::load ( QString id )
{
    blDebug ( Q_FUNC_INFO, 0 );
    int err = IncidenciaComercial::load ( id );
    setWindowTitle ( _ ( "Incidencia comercial" ) + " " + dbValue ( "idincidenciacomercial" ) );
    mui_fechaincidenciacomercial->setText ( dbValue ( "fechaincidenciacomercial" ) );
    mui_idcliente->setId ( dbValue ( "idcliente" ) );
    mui_comentincidenciacomercial->setPlainText ( dbValue ( "comentincidenciacomercial" ) );
    mui_idtrabajador->setId ( dbValue ( "idtrabajador" ) );
    mui_estadoincidenciacomercial->setestado ( dbValue ( "estadoincidenciacomercial" ) );
    mui_horaincidenciacomercial->setText ( dbValue ( "horaincidenciacomercial" ) );
    mui_refincidenciacomercial->setText ( dbValue ( "refincidenciacomercial" ) );
    dialogChanges_readValues();
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return err;
}

