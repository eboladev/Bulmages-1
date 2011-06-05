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

#include "rutacomercialincview.h"
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
RutaComercialIncView::RutaComercialIncView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    blDebug ( Q_FUNC_INFO, 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_rutacomercial->setMainCompany ( comp );
    mui_incidenciacomercial->setMainCompany ( comp );
    insertWindow ( windowTitle(), this );
    blScript(this);
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
RutaComercialIncView::~RutaComercialIncView()
{
    blDebug ( Q_FUNC_INFO, 0 );
    mainCompany() ->removeWindow ( this );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
void RutaComercialIncView::on_mui_borrar_clicked()
{
    blDebug ( Q_FUNC_INFO, 0 );
    if ( QMessageBox::warning ( this,
                                _ ( "BulmaFact - Rutas comerciales" ),
                                _ ( "Desea borrar esta ruta?" ),
                                _ ( "&Si" ), _ ( "&No" ) ) == 0 ) {
        mui_rutacomercial->remove();
        mui_incidenciacomercial->remove();
    } // end if
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
void RutaComercialIncView::closeEvent ( QCloseEvent * )
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
void RutaComercialIncView::pintar()
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
};


///
/**
**/
void RutaComercialIncView::on_mui_guardar_clicked()
{
    blDebug ( Q_FUNC_INFO, 0 );
    mui_rutacomercial->save();
    mui_incidenciacomercial->save();
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param id
\param id1
\return
**/
int RutaComercialIncView::load ( QString id, QString id1 )
{
    blDebug ( Q_FUNC_INFO, 0 );
    int err = mui_rutacomercial->load ( id );
    err = mui_incidenciacomercial->load ( id1 );
    setWindowTitle ( _ ( "Accion comercial" ) + " " + mui_rutacomercial->dbValue ( "idrutacomercial" ) + " + " + mui_incidenciacomercial->dbValue ( "idincidenciacomercial" ) );
    insertWindow ( windowTitle(), this );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return err;
}


///
/**
**/
void RutaComercialIncView::on_mui_aceptar_clicked()
{
    blDebug ( Q_FUNC_INFO, 0 );
    on_mui_guardar_clicked();
    close();
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}

