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
#include "company.h"
#include "blconfiguration.h"
#include "plugins.h"


///
/**
\param comp
\param parent
**/
IncidenciaComercial::IncidenciaComercial ( Company *comp, QWidget *parent ) : FichaBf ( comp, parent )
{
    _depura ( "IncidenciaComercial::IncidenciaComercial", 0 );
    setTitleName ( _( "Incidencia comercial" ) );
    setDBTableName ( "incidenciacomercial" );
    setDBCampoId ( "idincidenciacomercial" );
    addDBCampo ( "idincidenciacomercial", BlDbField::DBint, BlDbField::DBPrimaryKey, _( "Identificador" ) );
    addDBCampo ( "fechaincidenciacomercial", BlDbField::DBdate, BlDbField::DBNotNull, _( "Fecha" ) );
    addDBCampo ( "idcliente", BlDbField::DBint, BlDbField::DBNotNull, _( "Familia" ) );
    addDBCampo ( "idtrabajador", BlDbField::DBint, BlDbField::DBNotNull, _( "Trabajador" ) );
    addDBCampo ( "comentincidenciacomercial", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Comentarios" ) );
    addDBCampo ( "estadoincidenciacomercial", BlDbField::DBint, BlDbField::DBNotNull, _( "Estado" ) );
    addDBCampo ( "horaincidenciacomercial", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Hora" ) );
    addDBCampo ( "refincidenciacomercial", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Referencia" ) );
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
    cursor2 *cur = empresaBase() ->cargacursor ( query );
    if ( !cur->eof() )  {
        DBload ( cur );
    }
    delete cur;
    setWindowTitle ( _( "Ruta comercial" ) + " " + DBvalue ( "idincidenciacomercial" ) );
    dialogChanges_cargaInicial();
    _depura ( "END IncidenciaComercial::cargar", 0 );
    return 0;
}
