/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "incidenciacomercial.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "blplugins.h"


///
/**
\param comp
\param parent
**/
IncidenciaComercial::IncidenciaComercial ( BfCompany *comp, QWidget *parent ) : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setTitleName ( _ ( "Incidencia comercial" ) );
    setDbTableName ( "incidenciacomercial" );
    setDbFieldId ( "idincidenciacomercial" );
    addDbField ( "idincidenciacomercial", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Identificador" ) );
    addDbField ( "fechaincidenciacomercial", BlDbField::DbDate, BlDbField::DbNotNull, _ ( "Fecha" ) );
    addDbField ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Familia" ) );
    addDbField ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Trabajador" ) );
    addDbField ( "comentincidenciacomercial", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentarios" ) );
    addDbField ( "estadoincidenciacomercial", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Estado" ) );
    addDbField ( "horaincidenciacomercial", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Hora" ) );
    addDbField ( "refincidenciacomercial", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Referencia" ) );
    blScript(this);
    
}


///
/**
**/
IncidenciaComercial::~IncidenciaComercial()
{
    BL_FUNC_DEBUG
    
}


///
/**
\return
**/
void IncidenciaComercial::pintar()
{
    BL_FUNC_DEBUG
    /// Disparamos los plugins con IncidenciaComercial_pintar
    int res = g_plugins->run ( "IncidenciaComercial_pintar", this );
    if ( res != 0 )
        return;
    
}


///
/**
\param id
\return
**/
int IncidenciaComercial::load ( QString id )
{
    BL_FUNC_DEBUG
    QString query = "SELECT * FROM incidenciacomercial WHERE idincidenciacomercial = " + id;
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
    if ( !cur->eof() )  {
        DBload ( cur );
    }
    delete cur;
    setWindowTitle ( _ ( "Ruta comercial" ) + " " + dbValue ( "idincidenciacomercial" ) );
    dialogChanges_readValues();
    
    return 0;
}
