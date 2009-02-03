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

#include <QMessageBox>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>

#include <fstream>

#include "alumnoview.h"
#include "company.h"
#include "configuracion.h"
#include "busquedafecha.h"

#include "funcaux.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
AlumnoView::AlumnoView ( Company *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "AlumnoView::AlumnoView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );
        centrarEnPantalla ( this );

        setTitleName ( _( "Alumno" ) );
        setDBTableName ( "alumno" );
        setDBCampoId ( "idalumno" );
        addDBCampo ( "idalumno", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate ( "Alumno", "ID alumno" ) );
        addDBCampo ( "nombrealumno", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Alumno", "Nombre del alumno" ) );

        meteWindow ( windowTitle(), this, FALSE );
        pintar();
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        mensajeInfo ( _( "Error al crear el alumno" ), this );
    } // end try
    _depura ( "END AlumnoView::AlumnoView", 0 );
}


/** No precisa acciones adicionales en el destructor.
*/
AlumnoView::~AlumnoView()
{
    _depura ( "AlumnoView::~AlumnoView", 0 );
    _depura ( "END AlumnoView::~AlumnoView", 0 );
}


QString AlumnoView::nombrePlantilla(void) 
{
   return QString("recibo");
}

void AlumnoView::imprimir()
{
    _depura ( "AlumnoView::imprimir", 0 );
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

    _depura ( "END AlumnoView::imprimir", 0 );
}


int AlumnoView::guardarPost() {
	_depura(" AlumnoView::guardarPost", 0);

	_depura("END AlumnoView::guardarPost", 0);

}

