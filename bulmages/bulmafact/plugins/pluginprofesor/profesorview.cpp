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
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>

#include "profesorview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "busquedafecha.h"
#include "blfunctions.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
ProfesorView::ProfesorView ( BfCompany *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "ProfesorView::ProfesorView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );
        centrarEnPantalla ( this );

        setTitleName ( _( "Profesor" ) );
        setDbTableName ( "profesor" );
        setDbFieldId ( "idprofesor" );
        addDbField ( "idprofesor", BlDbField::DBint, BlDbField::DBPrimaryKey, _( "ID profesor" ) );
        addDbField ( "nombreprofesor", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Nombre del profesor" ) );


        /// Datos por defecto.
	mui_idprovincia->setMainCompany ( mainCompany() );
    mui_idprovincia->setQuery("SELECT * FROM provincia LEFT JOIN pais ON provincia.idpais = pais.idpais ORDER BY descpais, provincia");
    mui_idprovincia->setTableName ("provincia");
    mui_idprovincia->setCampoId ("idprovincia");
    mui_idprovincia->m_valores["provincia"] = "";
    mui_idprovincia->m_valores["descpais"] = "";
    mui_idprovincia->allowNull(TRUE);
        mui_idprovincia->setId ( "" );

        meteWindow ( windowTitle(), this, FALSE );
        pintar();
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        mensajeInfo ( _( "Error al crear el profesor" ), this );
    } // end try
    _depura ( "END ProfesorView::ProfesorView", 0 );
}


/** No precisa acciones adicionales en el destructor.
*/
ProfesorView::~ProfesorView()
{
    _depura ( "ProfesorView::~ProfesorView", 0 );
    _depura ( "END ProfesorView::~ProfesorView", 0 );
}


QString ProfesorView::nombrePlantilla(void) 
{
   return QString("recibo");
}

void ProfesorView::imprimir()
{
    _depura ( "ProfesorView::imprimir", 0 );
    /// Comprobamos que se disponen de los datos minimos para imprimir el recibo.
    QString SQLQuery = "";

    if ( DBvalue ( "idcliente" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        mensajeInfo ( _( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
        return;
    } 
    /// Disparamos los plugins
    int res = g_plugins->lanza ( "CoboView_on_mui_imprimir_clicked", this );
    if ( res != 0 ) {
        return;
    } // end if
    FichaBf::imprimir();

    _depura ( "END ProfesorView::imprimir", 0 );
}


int ProfesorView::guardarPost() {
	_depura(" ProfesorView::guardarPost", 0);

	_depura("END ProfesorView::guardarPost", 0);

}

