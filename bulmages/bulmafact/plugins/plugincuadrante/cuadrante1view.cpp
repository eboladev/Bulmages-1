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

#include "cuadrante1view.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "plugins.h"
#include "blfunctions.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
/**
\param comp
\param parent
**/
Cuadrante1View::Cuadrante1View ( BfCompany *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "Cuadrante1View::Cuadrante1View", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        /// Lanzamos los plugins.
        if ( g_plugins->lanza ( "Cuadrante1View_Cuadrante1View", this ) ) return;

        setTitleName ( _( "Cuadrante" ) );
        setDBTableName ( "cuadrante" );
        setDBCampoId ( "idcuadrante" );
        addDBCampo ( "idcuadrante", BlDbField::DBint, BlDbField::DBPrimaryKey, _( "ID cuadrante" ) );
        addDBCampo ( "idalmacen", BlDbField::DBint, BlDbField::DBNoSave, _( "Almacen" ) );
        addDBCampo ( "fechacuadrante", BlDbField::DBdate, BlDbField::DBNoSave  , _( "Fecha" ) );
        addDBCampo ( "comentcuadrante", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Comentarios" ) );
        addDBCampo ( "aperturacuadrante", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Apertura manyana" ) );
        addDBCampo ( "cierrecuadrante", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Cierre manyana" ) );
        addDBCampo ( "apertura1cuadrante", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Apartura tarde" ) );
        addDBCampo ( "cierre1cuadrante", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Cierre tarde" ) );
        addDBCampo ( "fiestacuadrante", BlDbField::DBboolean, BlDbField::DBNothing, _( "Festivo" ) );

        mui_idalmacen->setMainCompany ( comp );

        mui_list->setMainCompany ( comp );
        mui_list->setDBTableName ( "horario" );
        mui_list->setDBCampoId ( "idhorario" );
        mui_list->addSubFormHeader ( "nomtrabajador",  BlDbField::DBvarchar, BlDbField::DBNoSave,     BlSubFormHeader::DBNone,                         _( "Nombre Trabajador" ) );
        mui_list->addSubFormHeader ( "horainhorario",  BlDbField::DBvarchar, BlDbField::DBNotNull,    BlSubFormHeader::DBNone,                         _( "Hora Inicio" ) );
        mui_list->addSubFormHeader ( "horafinhorario", BlDbField::DBvarchar, BlDbField::DBNotNull,    BlSubFormHeader::DBNone,                         _( "Hora Fin" ) );
        mui_list->addSubFormHeader ( "idcuadrante",    BlDbField::DBint,     BlDbField::DBNotNull,    BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite,  _( "ID Cuadrante" ) );
        mui_list->addSubFormHeader ( "idtrabajador",   BlDbField::DBint,     BlDbField::DBNotNull,    BlSubFormHeader::DBNone,  _( "ID trabajador" ) );
        mui_list->addSubFormHeader ( "idhorario",      BlDbField::DBint,     BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite,  _( "ID horario" ) );
        mui_list->setinsercion ( TRUE );
        mui_list->setDelete ( TRUE );
        mui_list->setSortingEnabled ( FALSE );

        dialogChanges_cargaInicial();
        meteWindow ( windowTitle(), this, FALSE );
    } catch ( ... ) {
        mensajeInfo ( _( "Error al crear el almacen" ) );
    } // end try
    _depura ( "END Cuadrante1View::Cuadrante1View", 0 );
}


/** No precisa acciones adicionales en el destructor.
*/
/**
**/
Cuadrante1View::~Cuadrante1View()
{
    _depura ( "Cuadrante1View::~Cuadrante1View", 0 );
    _depura ( "END Cuadrante1View::~Cuadrante1View", 0 );
}


///
/**
\param idcuadrante
\return
**/
int Cuadrante1View::cargarPost ( QString idcuadrante )
{
    mui_list->cargar ( "SELECT * FROM horario LEFT JOIN trabajador ON horario.idtrabajador = trabajador.idtrabajador WHERE idcuadrante=" + idcuadrante );
    return 0;
}


///
/**
\return
**/
int Cuadrante1View::guardarPost()
{
    mui_list->setColumnValue ( "idcuadrante", DBvalue ( "idcuadrante" ) );
    mui_list->guardar();
    emit ( save() );
    return 0;
}



