/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include <QMenu>

#include "extractosubform.h"


///
/**
**/
ExtractoSubForm::~ExtractoSubForm()
{
    _depura ( "ExtractoSubForm::~ExtractoSubForm", 0 );
    _depura ( "END ExtractoSubForm::~ExtractoSubForm", 0 );
}


///
/**
\param parent
**/
ExtractoSubForm::ExtractoSubForm ( QWidget *parent, const char * ) : SubForm2Bc ( parent )
{
    _depura ( "ExtractoSubForm::ExtractoSubForm", 0 );
    setDBTableName ( "apunte" );
    setFileConfig ( "ExtractoSubForm" );
    setDBCampoId ( "idapunte" );
    addSHeader ( "punteo", DBCampo::DBboolean, DBCampo::DBNotNull, BlSubFormHeader::DBNone, _( "Punteo" ) );
    addSHeader ( "fecha", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite | BlSubFormHeader::DBBlockView, _( "Fecha" ) );
    addSHeader ( "codigo", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Cuenta" ) );
    addSHeader ( "ordenasiento", DBCampo::DBint, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite | BlSubFormHeader::DBBlockView, _( "Asiento" ) );
    addSHeader ( "descripcioncuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Descripcion" ) );
    addSHeader ( "descripcionapunte", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Descripcion Apunte" ) );
    addSHeader ( "descripcionasiento", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Descripcion Asiento" ) );
    addSHeader ( "conceptocontable", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Concepto" ) );
    addSHeader ( "debe", DBCampo::DBnumeric, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite | BlSubFormHeader::DBBlockView, _( "Debe" ) );
    addSHeader ( "haber", DBCampo::DBnumeric, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite | BlSubFormHeader::DBBlockView, _( "Haber" ) );
    addSHeader ( "saldo", DBCampo::DBnumeric, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Saldo" ) );

    addSHeader ( "idapunte", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "idapunte" ) );
    addSHeader ( "codigoborrador", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "codigoborrador" ) );
    addSHeader ( "idasiento", DBCampo::DBint, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "idasiento" ) );
    addSHeader ( "iddiario", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "iddiario" ) );
    addSHeader ( "idcuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "idcuenta" ) );
    addSHeader ( "tipocuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "tipocuenta" ) );
    addSHeader ( "contrapartida", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Id contrapartida" ) );
    addSHeader ( "codcontrapartida", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "contrapartida" ) );
    addSHeader ( "comentario", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Comentario" ) );
    addSHeader ( "idcanal", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "idcanal" ) );
    addSHeader ( "nombrecanal", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Canal" ) );
    addSHeader ( "marcaconciliacion", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "marcaconciliacion" ) );
    addSHeader ( "idc_coste", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "idc_coste" ) );
    addSHeader ( "nombrec_coste", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Centro Coste" ) );
    addSHeader ( "idtipoiva", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "idtipoiva" ) );
    addSHeader ( "orden", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Orden" ) );
    setinsercion ( FALSE );
    setOrdenEnabled ( FALSE );
    setOrdenPorQuery ( FALSE );
    setDelete ( FALSE );
    connect ( this, SIGNAL ( pintaMenu ( QMenu * ) ), this, SLOT ( s_pintaMenu ( QMenu * ) ) );
    connect ( this, SIGNAL ( trataMenu ( QAction * ) ), this, SLOT ( s_trataMenu ( QAction * ) ) );

    _depura ( "END ExtractoSubForm::ExtractoSubForm", 0 );
}



/// Slot que trata la solicitud de pintar el menu contextual sobre el subformulario.
/**
\param menu
**/
void ExtractoSubForm::s_pintaMenu ( QMenu *menu )
{
    _depura ( "ListLinAsiento1View::s_pintaMenu", 0 );
    menu->addSeparator();
    menu->addAction ( _( "Mostrar asiento" ) );
    menu->addSeparator();
    menu->addAction ( _( "Mostrar diario (dia)" ) );
    menu->addAction ( _( "Mostrar diario (mes)" ) );
    menu->addAction ( _( "Mostrar diario (ano)" ) );
    _depura ( "ListLinAsiento1View::s_pintaMenu", 0 );
}

/// Slot que trata la activacion de un elemento en el menu contextual.
/**
\param action
/return
**/
void ExtractoSubForm::s_trataMenu ( QAction *action )
{
    _depura ( "ListLinAsiento1View::s_trataMenu", 0 );
    if ( !action ) return;
    if ( action->text() == _( "Mostrar asiento" ) )
        boton_asiento();
    if ( action->text() == _( "Mostrar diario (dia)" ) )
        boton_diario1 ( 0 );
    if ( action->text() == _( "Mostrar diario (mes)" ) )
        boton_diario1 ( 1 );
    if ( action->text() == _( "Mostrar diario (ano)" ) )
        boton_diario1 ( 2 );
    _depura ( "END ListLinAsiento1View::s_trataMenu", 0 );
}

