/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
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

#include "datosview.h"
#include <iostream>
#include <iomanip>


///
/**
\param parent
**/
DatosView::DatosView ( BlMainCompany *emp, QWidget *parent , Qt::WFlags f , edmode modo ) : BlForm ( emp, parent, f, modo )
{
    blDebug ( "DatosView::DatosView", 0 );
    resultado = false; 

    setupUi ( this );
    setTitleName ( _ ( "Cuenta" ) );
    setDbTableName ( "cuenta" );
    setDbFieldId ( "idcuenta" );
    addDbField ( "idcuenta", BlDbField::DbDate, BlDbField::DbNoSave, _ ( "Id cuenta" ) );
    addDbField ( "fechainicialactual", BlDbField::DbDate, BlDbField::DbNoSave, _ ( "Fecha Inicial Actual" ) );
    addDbField ( "fechafinalactual", BlDbField::DbDate, BlDbField::DbNoSave, _ ( "Fecha Final Actual" ) );
    addDbField ( "fechainicialanterior", BlDbField::DbDate, BlDbField::DbNoSave, _ ( "Fecha Inicial Anterior" ) );
    addDbField ( "fechafinalanterior", BlDbField::DbDate, BlDbField::DbNoSave, _ ( "Fecha Final Anterior" ) );

// mui_fechainicialactual
// mui_fechafinalactual
// mui_fechainicialanterior
// mui_fechafinalanterior

    blDebug ( "END DatosView::DatosView", 0 );
}


///
/**
**/
DatosView::~DatosView()
{
    blDebug ( "END DatosView::~DatosView", 0 );
    blDebug ( "DatosView::~DatosView", 0 );
}



void DatosView::on_mui_aceptar_clicked() {
  
   recogeValores();
   resultado = true;

   QString fechaInicialActual = dbValue("fechainicialactual");
   QString fechaFinalActual = dbValue("fechafinalactual");
   QString fechaInicialAnterior = dbValue("fechainicialanterior");
   QString fechaFinalAnterior = dbValue("fechafinalanterior");
   
   std::cout << "fechaInicialActual = " << fechaInicialActual.toAscii().constData() << std::endl;
   std::cout << "fechaFinalActual = " << fechaFinalActual.toAscii().constData() << std::endl;
   std::cout << "fechaInicialAnterior = " << fechaInicialAnterior.toAscii().constData() << std::endl;
   std::cout << "fechaFinalAnterior = " << fechaFinalAnterior.toAscii().constData() << std::endl;

   if ( fechaInicialActual.isEmpty() ) {
        mensajeError ( _ ( "ERROR: Debe introducir una fecha inicial del balance actual." ) );
        resultado = false;
    } else {
        if ( fechaFinalActual.isEmpty() ) {
            mensajeError ( _ ( "ERROR: Debe introducir una fecha final del balance actual." ) );
            resultado = false;
        }
         else {
            if ( fechaInicialAnterior.isEmpty() ) {
               mensajeError ( _ ( "ERROR: Debe introducir una fecha inicial del balance anterior." ) );
               resultado = false;
            } else {
            if ( fechaFinalAnterior.isEmpty() ) {
               mensajeError ( _ ( "ERROR: Debe introducir una fecha final del balance anterior." ) );
               resultado = false;
            }
         }
    }
   } // end if

   // Se pulso el boton 'cancelar'
   if ( resultado ) {
      if ( generaRML ( m_informe ) )
      {
         BlProgressBar barra;
         barra.setRange ( 0, 21 );
         barra.setValue ( 0 );
         barra.show();
         barra.setText ( _ ( "Calculando Balance y Resultados" ) );

         QString archivod = m_informe;
         QString archivo =  m_informe.left ( m_informe.size() - 4 );
         invocaPYS ( archivo );
      }
      else{
         mensajeError ( _("ERROR: Ha habido un error generando el informe"));
      }
   }
   
}

void DatosView::on_mui_cancelar_clicked()
{
   resultado = true;
}
