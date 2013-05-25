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

#include <QtWidgets/QWidget>
#include <QtCore/QObject>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QToolButton>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtWidgets/QLayout>
#include <QtWidgets/QMessageBox>
#include <QtGui/QCloseEvent>

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
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    setDbTableName ( "rutacomercial" );
    setupUi ( this );
    
    mui_incidenciacomercial->setMainCompany ( comp );
    insertWindow ( windowTitle(), this );
    blScript(this);
    
}


///
/**
**/
RutaComercialIncView::~RutaComercialIncView()
{
    BL_FUNC_DEBUG
    mainCompany() ->removeWindow ( this );
    
}


///
/**
**/
void RutaComercialIncView::on_mui_borrar_clicked()
{
    BL_FUNC_DEBUG
    if ( QMessageBox::warning ( this,
                                _ ( "BulmaFact - Rutas comerciales" ),
                                _ ( "Desea borrar esta ruta?" ),
                                _ ( "&Si" ), _ ( "&No" ) ) == 0 ) {
        mui_rutacomercial->remove();
        mui_incidenciacomercial->remove();
    } // end if
    
}


///
/**
**/
void RutaComercialIncView::closeEvent ( QCloseEvent * )
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void RutaComercialIncView::pintar()
{
    BL_FUNC_DEBUG
    
};


///
/**
**/
void RutaComercialIncView::on_mui_guardar_clicked()
{
    BL_FUNC_DEBUG
    try {
      mui_rutacomercial->save();
      mui_incidenciacomercial->save();
    } catch(...) {
      blMsgInfo("Error inesperado al guardar");
      
      throw -1;
    } // end try
    
}


///
/**
\param id
\param id1
\return
**/
int RutaComercialIncView::load ( QString id, QString id1 )
{
    BL_FUNC_DEBUG
    int err = mui_rutacomercial->load ( id );
    if (id1 != "") {
      err = mui_incidenciacomercial->load ( id1 );
    } // end if
    setWindowTitle ( _ ( "Accion comercial" ) + " " + mui_rutacomercial->dbValue ( "idrutacomercial" ) + " + " + mui_incidenciacomercial->dbValue ( "idincidenciacomercial" ) );
    insertWindow ( windowTitle(), this );
    
    return err;
}


///
/**
**/
void RutaComercialIncView::on_mui_aceptar_clicked()
{
    BL_FUNC_DEBUG
    try {
      on_mui_guardar_clicked();
      close();
    } catch (...) {
      return;
    } // end try
    
}

