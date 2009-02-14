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
#include "company.h"
#include "blconfiguration.h"
#include "busquedafecha.h"
#include "blfunctions.h"

#define coma "'"


///
/**
\param comp
\param parent
**/
RutaComercialIncView::RutaComercialIncView ( Company *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "RutaComercialIncView::RutaComercialIncView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_rutacomercial->setEmpresaBase ( comp );
    mui_incidenciacomercial->setEmpresaBase ( comp );
    meteWindow ( windowTitle(), this );
    _depura ( "END RutaComercialIncView::RutaComercialIncView", 0 );
}


///
/**
**/
RutaComercialIncView::~RutaComercialIncView()
{
    _depura ( "RutaComercialIncView::~RutaComercialIncView", 0 );
    empresaBase() ->sacaWindow ( this );
    _depura ( "END RutaComercialIncView::~RutaComercialIncView", 0 );
}


///
/**
**/
void RutaComercialIncView::on_mui_borrar_clicked()
{
    _depura ( "RutaComercialIncView::on_mui_borrar_clicked\n", 0 );
    if ( QMessageBox::warning ( this,
                                _( "BulmaFact - Rutas comerciales" ),
                                _( "Desea borrar esta ruta?" ),
                                _( "&Si" ), _( "&No" ) ) == 0 ) {
        mui_rutacomercial->borrar();
        mui_incidenciacomercial->borrar();
    } // end if
    _depura ( "END RutaComercialIncView::on_mui_borrar_clicked\n", 0 );
}


///
/**
**/
void RutaComercialIncView::closeEvent ( QCloseEvent * )
{
    _depura ( "closeEvent", 0 );
}


///
/**
**/
void RutaComercialIncView::pintar()
{
    _depura ( "RutaComercialIncView::pintar", 0 );
    _depura ( "END RutaComercialIncView::pintar", 0 );
};


///
/**
**/
void RutaComercialIncView::on_mui_guardar_clicked()
{
    _depura ( "RutaComercialIncView::on_mui_guardar_clicked", 0 );
    mui_rutacomercial->guardar();
    mui_incidenciacomercial->guardar();
    _depura ( "END RutaComercialIncView::on_mui_guardar_clicked", 0 );
}


///
/**
\param id
\param id1
\return
**/
int RutaComercialIncView::cargar ( QString id, QString id1 )
{
    _depura ( "RutaComercialIncView::cargar", 0 );
    int err = mui_rutacomercial->cargar ( id );
    err = mui_incidenciacomercial->cargar ( id1 );
    setWindowTitle ( _( "Accion comercial" ) + " " + mui_rutacomercial->DBvalue ( "idrutacomercial" ) + " + " + mui_incidenciacomercial->DBvalue ( "idincidenciacomercial" ) );
    meteWindow ( windowTitle(), this );
    _depura ( "END RutaComercialIncView::cargar", 0 );
    return err;
}


///
/**
**/
void RutaComercialIncView::on_mui_aceptar_clicked()
{
    _depura ( "RutaComercialIncView::on_mui_aceptar_clicked", 0 );
    on_mui_guardar_clicked();
    close();
    _depura ( "END RutaComercialIncView::on_mui_aceptar_clicked", 0 );
}

