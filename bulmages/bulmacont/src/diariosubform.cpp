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
#include "diariosubform.h"


///
/**
\param parent
**/
DiarioSubForm::DiarioSubForm ( QWidget *parent, const char * ) : BcSubForm ( parent )
{
    _depura ( "DiarioSubForm::DiarioSubForm", 0 );
    setDBTableName ( "borrador" );
    setFileConfig ( "DiarioSubForm" );
    setDBCampoId ( "idborrador" );
    addSubFormHeader ( "idasiento", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "idasiento" ) );
//     addSubFormHeader ( "ordenasientoconfecha", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Asiento" ) );
    addSubFormHeader ( "ordenasiento", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Asiento" ) );
    addSubFormHeader ( "fecha", BlDbField::DBdate, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Fecha" ) );
    addSubFormHeader ( "codigo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Cuenta" ) );
    addSubFormHeader ( "descripcion", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Descripcion" ) );
    addSubFormHeader ( "conceptocontable", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Concepto contable" ) );
    addSubFormHeader ( "debe", BlDbField::DBnumeric, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Debe" ) );
    addSubFormHeader ( "haber", BlDbField::DBnumeric, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Haber" ) );
    addSubFormHeader ( "comentario", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite | BlSubFormHeader::DBNoWrite, _( "Comentario" ) );
    addSubFormHeader ( "idborrador", BlDbField::DBint, BlDbField::DBNotNull | BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "idborrador" ) );
    addSubFormHeader ( "codigoborrador", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "codigoborrador" ) );
    addSubFormHeader ( "tipocuenta", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "tipocuenta" ) );
    addSubFormHeader ( "descripcioncuenta", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Descripcion de Cuenta" ) );
    addSubFormHeader ( "contrapartida", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Contrapartida" ) );
    addSubFormHeader ( "codcontrapartida", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Contrapartida" ) );
    addSubFormHeader ( "nombrecanal", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Canal" ) );
    addSubFormHeader ( "marcaconciliacion", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "MarcaConciliacion" ) );
    addSubFormHeader ( "nombrec_coste", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Centro Coste" ) );
    addSubFormHeader ( "factura", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Factura" ) );
    addSubFormHeader ( "idapunte", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "idapunte" ) );
    addSubFormHeader ( "idtipoiva", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "idtipoiva" ) );
    addSubFormHeader ( "idregistroiva", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoView, _( "idregistroiva" ) );
    addSubFormHeader ( "idcanal", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoView , _( "idcanal" ) );
    addSubFormHeader ( "idc_coste", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite | BlSubFormHeader::DBNoView, _( "idc_coste" ) );
    addSubFormHeader ( "iddiario", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "iddiario" ) );
    addSubFormHeader ( "idcuenta", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "idcuenta" ) );
    /// Van juntas. Desactivas un tipo de pijama y se activa otro personalizado al hacer 'rowspan'.
    setListadoPijama ( FALSE );
    setColumnToRowSpan ( "ordenasientoconfecha" );
    setColorFondo1 ( confpr->valor ( CONF_BG_DIARIO ) );
    setColorFondo2 ( confpr->valor ( CONF_BG_ALT_DIARIO ) );
    setinsercion ( FALSE );
    setOrdenEnabled ( FALSE );
    setOrdenPorQuery ( TRUE );
    setDelete ( FALSE );
    /// Preparamos el sistema de menus contextuales
    connect ( this, SIGNAL ( pintaMenu ( QMenu * ) ), this, SLOT ( s_pintaMenu ( QMenu * ) ) );
    connect ( this, SIGNAL ( trataMenu ( QAction * ) ), this, SLOT ( s_trataMenu ( QAction * ) ) );
    _depura ( "END DiarioSubForm::DiarioSubForm", 0 );
}


///
/**
**/
DiarioSubForm::~DiarioSubForm()
{
    _depura ( "DiarioSubForm::~DiarioSubForm", 0 );
    _depura ( "END DiarioSubForm::~DiarioSubForm", 0 );
}


/// Slot que trata la solicitud de pintar el menu contextual sobre el subformulario.
/**
\param menu
**/
void DiarioSubForm::s_pintaMenu ( QMenu *menu )
{
    _depura ( "ListLinAsiento1View::s_pintaMenu", 0 );
    menu->addSeparator();
    menu->addAction ( _( "Mostrar asiento" ) );
    menu->addSeparator();
    menu->addAction ( _( "Mostrar extracto (dia)" ) );
    menu->addAction ( _( "Mostrar extracto (mes)" ) );
    menu->addAction ( _( "Mostrar extracto (ano)" ) );
    _depura ( "END ListLinAsiento1View::s_pintaMenu", 0 );
}

/// Slot que trata la activacion de un elemento en el menu contextual.
/**
\param action
/return
**/
void DiarioSubForm::s_trataMenu ( QAction *action )
{
    _depura ( "ListLinAsiento1View::s_trataMenu", 0 );
    if ( !action ) return;
    if ( action->text() == _( "Mostrar asiento" ) )
        boton_asiento();
    if ( action->text() == _( "Mostrar extracto (dia)" ) )
        boton_extracto1 ( 0 );
    if ( action->text() == _( "Mostrar extracto (mes)" ) )
        boton_extracto1 ( 1 );
    if ( action->text() == _( "Mostrar extracto (ano)" ) )
        boton_extracto1 ( 2 );

    _depura ( "END ListLinAsiento1View::s_trataMenu", 0 );
}


