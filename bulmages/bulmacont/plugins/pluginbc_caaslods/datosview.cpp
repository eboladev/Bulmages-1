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


///
/**
\param parent
**/
DatosView::DatosView ( BlMainCompany *emp, QWidget *parent , Qt::WFlags f , edmode modo ) : BlForm ( emp, parent, f, modo )
{
    _depura ( "DatosView::DatosView", 0 );

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

    _depura ( "END DatosView::DatosView", 0 );
}


///
/**
**/
DatosView::~DatosView()
{
    _depura ( "END DatosView::~DatosView", 0 );
    _depura ( "DatosView::~DatosView", 0 );
}



void DatosView::on_mui_aceptar_clicked() {
   bool resultado = true; 
   recogeValores();

   QString fechaInicialActual = dbValue("fechainicialactual");
   QString fechaFinalActual = dbValue("fechafinalactual");
   QString fechaInicialAnterior = dbValue("fechainicialanteiror");
   QString fechaFinalAnterior = dbValue("fechafinalanterior");

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
         QString archivod = m_informe;
         QString archivo =  m_informe.left ( m_informe.size() - 4 );
         invocaPYS ( archivo );
      }
      else{
         mensajeError ( _("ERROR: Ha habido un error generando el informe"));
      }
   }
}
