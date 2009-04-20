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
        addDbField ( "idcuenta", BlDbField::DbDate, BlDbField::DbNoSave, _ ( "Fecha Actual" ) );
        addDbField ( "fechaactual", BlDbField::DbDate, BlDbField::DbNoSave, _ ( "Fecha Actual" ) );
        addDbField ( "fechaanterior", BlDbField::DbDate, BlDbField::DbNoSave, _ ( "Fecha Anterior" ) );

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

   QString fechaactual = dbValue("fechaactual");
   QString fechaanterior = dbValue("fechaanterior");


   if ( fechaactual.isEmpty() ) {
        mensajeError ( _ ( "ERROR: Debe introducir una fecha en el balance actual." ) );
        resultado = false;
    } else {
        if ( fechaanterior.isEmpty() ) {
            mensajeError ( _ ( "ERROR: Debe introducir una fecha en el balance anterior." ) );
            resultado = false;
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
