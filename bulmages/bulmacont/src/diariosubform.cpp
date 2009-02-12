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
DiarioSubForm::DiarioSubForm ( QWidget *parent, const char * ) : SubForm2Bc ( parent )
{
    _depura ( "DiarioSubForm::DiarioSubForm", 0 );
    setDBTableName ( "borrador" );
    setFileConfig ( "DiarioSubForm" );
    setDBCampoId ( "idborrador" );
    addSHeader ( "idasiento", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "idasiento" ) );
//     addSHeader ( "ordenasientoconfecha", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Asiento" ) );
    addSHeader ( "ordenasiento", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Asiento" ) );
    addSHeader ( "fecha", BlDbField::DBdate, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Fecha" ) );
    addSHeader ( "codigo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Cuenta" ) );
    addSHeader ( "descripcion", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Descripcion" ) );
    addSHeader ( "conceptocontable", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Concepto contable" ) );
    addSHeader ( "debe", BlDbField::DBnumeric, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Debe" ) );
    addSHeader ( "haber", BlDbField::DBnumeric, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Haber" ) );
    addSHeader ( "comentario", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite | BlSubFormHeader::DBNoWrite, _( "Comentario" ) );
    addSHeader ( "idborrador", BlDbField::DBint, BlDbField::DBNotNull | BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "idborrador" ) );
    addSHeader ( "codigoborrador", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "codigoborrador" ) );
    addSHeader ( "tipocuenta", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "tipocuenta" ) );
    addSHeader ( "descripcioncuenta", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Descripcion de Cuenta" ) );
    addSHeader ( "contrapartida", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Contrapartida" ) );
    addSHeader ( "codcontrapartida", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Contrapartida" ) );
    addSHeader ( "nombrecanal", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Canal" ) );
    addSHeader ( "marcaconciliacion", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "MarcaConciliacion" ) );
    addSHeader ( "nombrec_coste", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Centro Coste" ) );
    addSHeader ( "factura", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Factura" ) );
    addSHeader ( "idapunte", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "idapunte" ) );
    addSHeader ( "idtipoiva", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "idtipoiva" ) );
    addSHeader ( "idregistroiva", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoView, _( "idregistroiva" ) );
    addSHeader ( "idcanal", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoView , _( "idcanal" ) );
    addSHeader ( "idc_coste", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite | BlSubFormHeader::DBNoView, _( "idc_coste" ) );
    addSHeader ( "iddiario", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "iddiario" ) );
    addSHeader ( "idcuenta", BlDbField::DBint, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "idcuenta" ) );
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


