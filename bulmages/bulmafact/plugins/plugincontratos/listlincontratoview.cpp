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

#include <QMessageBox>
#include <QKeyEvent>
#include <QEvent>

#include "listlincontratoview.h"
#include "blfunctions.h"
#include "blfixed.h"
#include "plugins.h"


///
/**
\param parent
**/
ListLinContratoView::ListLinContratoView ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListLinContratoView::ListLinContratoView", 0 );
    setDBTableName ( "lcontrato" );
    setDBCampoId ( "idlcontrato" );
    addSHeader ( "idarticulo", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView, _( "Id articulo" ) );
    addSHeader ( "codigocompletoarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Codigo completo articulo" ) );
    addSHeader ( "nomarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Nombre articulo" ) );
    addSHeader ( "idlcontrato", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView, _( "Idl contrato" ) );
    addSHeader ( "desclcontrato", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Descripcionl contrato" ) );
    addSHeader ( "cantlcontrato", BlDbField::DBnumeric, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Cantidadl contrato" ) );
    addSHeader ( "pvplcontrato", BlDbField::DBnumeric, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "PVPl contrato" ) );
    addSHeader ( "idcontrato", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView, _( "Id contrato" ) );
    addSHeader ( "ordenlcontrato", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView, _( "Orden" ) );
    setinsercion ( TRUE );
    setOrdenEnabled ( TRUE );
    _depura ( "END ListLinContratoView::ListLinContratoView", 0 );
}


///
/**
\param idcontrato
**/
void ListLinContratoView::cargar ( QString idcontrato )
{
    _depura ( "ListLinContratoView::cargar", 0 );
    mdb_idcontrato = idcontrato;
    BlSubForm::cargar ( "SELECT * FROM lcontrato LEFT JOIN articulo ON lcontrato.idarticulo = articulo.idarticulo WHERE idcontrato=" + mdb_idcontrato + " ORDER BY ordenlcontrato" );
    _depura ( "END ListLinContratoView::cargar", 0 );
}



