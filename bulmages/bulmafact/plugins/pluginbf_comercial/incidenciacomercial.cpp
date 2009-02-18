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

#include <QFile>
#include <QTextStream>

#include "incidenciacomercial.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "plugins.h"


///
/**
\param comp
\param parent
**/
IncidenciaComercial::IncidenciaComercial ( BfCompany *comp, QWidget *parent ) : FichaBf ( comp, parent )
{
    _depura ( "IncidenciaComercial::IncidenciaComercial", 0 );
    setTitleName ( _( "Incidencia comercial" ) );
    setDbTableName ( "incidenciacomercial" );
    setDbFieldId ( "idincidenciacomercial" );
    addDbField ( "idincidenciacomercial", BlDbField::DBint, BlDbField::DBPrimaryKey, _( "Identificador" ) );
    addDbField ( "fechaincidenciacomercial", BlDbField::DBdate, BlDbField::DBNotNull, _( "Fecha" ) );
    addDbField ( "idcliente", BlDbField::DBint, BlDbField::DBNotNull, _( "Familia" ) );
    addDbField ( "idtrabajador", BlDbField::DBint, BlDbField::DBNotNull, _( "Trabajador" ) );
    addDbField ( "comentincidenciacomercial", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Comentarios" ) );
    addDbField ( "estadoincidenciacomercial", BlDbField::DBint, BlDbField::DBNotNull, _( "Estado" ) );
    addDbField ( "horaincidenciacomercial", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Hora" ) );
    addDbField ( "refincidenciacomercial", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Referencia" ) );
    _depura ( "END IncidenciaComercial::IncidenciaComercial", 0 );


}


///
/**
**/
IncidenciaComercial::~IncidenciaComercial()
{
    _depura ( "IncidenciaComercial::~IncidenciaComercial", 0 );
    _depura ( "END IncidenciaComercial::~IncidenciaComercial", 0 );
}


///
/**
\return
**/
void IncidenciaComercial::pintar()
{
    _depura ( "IncidenciaComercial::pintar", 0 );
    /// Disparamos los plugins con IncidenciaComercial_pintar
    int res = g_plugins->lanza ( "IncidenciaComercial_pintar", this );
    if ( res != 0 )
        return;
    _depura ( "END IncidenciaComercial::pintar", 0 );
}


///
/**
\param id
\return
**/
int IncidenciaComercial::cargar ( QString id )
{
    _depura ( "IncidenciaComercial::cargar", 0 );
    QString query = "SELECT * FROM incidenciacomercial WHERE idincidenciacomercial = " + id;
    BlDbRecordSet *cur = mainCompany() ->cargacursor ( query );
    if ( !cur->eof() )  {
        DBload ( cur );
    }
    delete cur;
    setWindowTitle ( _( "Ruta comercial" ) + " " + DBvalue ( "idincidenciacomercial" ) );
    dialogChanges_cargaInicial();
    _depura ( "END IncidenciaComercial::cargar", 0 );
    return 0;
}
