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

#include "balancesubform.h"


///
/**
\param parent
**/
BalanceSubForm::BalanceSubForm ( QWidget *parent, const char * ) : BcSubForm ( parent )
{
    _depura ( "BalanceSubForm::BalanceSubForm", 0 );
    setDBTableName ( "borrador" );
    setFileConfig ( "balancesubform" );
    setDBCampoId ( "idborrador" );
    addSubFormHeader ( "idcuenta", BlDbField::DBint, BlDbField::DBNotNull | BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "idcuenta" ) );
    addSubFormHeader ( "tipocuenta", BlDbField::DBint, BlDbField::DBNotNull | BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "tipocuenta" ) );
    addSubFormHeader ( "codigo", BlDbField::DBint, BlDbField::DBNotNull | BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "codigo" ) );
    addSubFormHeader ( "descripcion", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "descripcion" ) );
    addSubFormHeader ( "asaldo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "asaldo" ) );
    addSubFormHeader ( "tdebe", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "tdebe" ) );
    addSubFormHeader ( "thaber", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "thaber" ) );
    addSubFormHeader ( "tsaldo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "tsaldo" ) );
    addSubFormHeader ( "ejdebe", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "ejdebe" ) );
    addSubFormHeader ( "ejhaber", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "ejhaber" ) );
    addSubFormHeader ( "ejsaldo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "ejsaldo" ) );
    setinsercion ( FALSE );
    _depura ( "END BalanceSubForm::BalanceSubForm", 0 );
}


///
/**
\return
**/
void BalanceSubForm::contextMenuEvent ( QContextMenuEvent * )
{
    _depura ( "BcSubForm::contextMenuEvent", 0 );
    QAction *del = NULL;
    int row = currentRow();
    if ( row < 0 ) {
        return;
    } // end if
    int col = currentColumn();
    if ( row < 0 ) {
        return;
    } // end if
    QMenu *popup = new QMenu ( this );
    QAction *mostapunte = popup->addAction ( "Mostrar asiento" );
    popup->addSeparator();
    QAction *mostextractodia = popup->addAction ( "Mostrar extracto (dia)" );
    QAction *mostextractomes = popup->addAction ( "Mostrar extracto (mes)" );
    QAction *mostextractoano = popup->addAction ( "Mostrar extracto (ano)" );


    if ( m_delete )
        del = popup->addAction ( _( "Borrar registro" ) );
    popup->addSeparator();
    QAction *ajustc = popup->addAction ( _( "Ajustar columa" ) );
    QAction *ajustac = popup->addAction ( _( "Ajustar altura" ) );
    QAction *ajust = popup->addAction ( _( "Ajustar columnas" ) );
    QAction *ajusta = popup->addAction ( _( "Ajustar alturas" ) );
    popup->addSeparator();
    QAction *verconfig = popup->addAction ( _( "Ver configurador de subformulario" ) );
    QAction *opcion = popup->exec ( QCursor::pos() );

    if ( opcion == mostapunte ) {
        boton_asiento();
    } // end if
    if ( opcion == del ) {
        borrar ( row );
    } // end if
    if ( opcion == ajust ) {
        resizeColumnsToContents();
    } // end if
    if ( opcion == ajusta ) {
        resizeRowsToContents();
    } // end if
    if ( opcion == ajustc ) {
        resizeColumnToContents ( col );
    } // end if
    if ( opcion == ajustac ) {
        resizeRowToContents ( row );
    } // end if
    if ( opcion == verconfig ) {
        showConfig();
    } // end if
    if ( opcion == mostextractodia ) {
        boton_extracto1 ( 0 );
    } // end if
    if ( opcion == mostextractomes ) {
        boton_extracto1 ( 1 );
    } // end if
    if ( opcion == mostextractoano ) {
        boton_extracto1 ( 2 );
    } // end if
    delete popup;
    _depura ( "END BcSubForm::contextMenuEvent", 0 );
}

