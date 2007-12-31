/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#include "asientolistsubform.h"


/// Subformulario.
/**
\param parent
**/
AsientoListSubform::AsientoListSubform ( QWidget *parent, const char * )
        : SubForm2Bc ( parent )
{
    _depura ( "AsientoListSubform::AsientoListSubform", 0 );
    setDBTableName ( "asiento" );
    setFileConfig ( "AsientoListSubform" );
    setDBCampoId ( "idasiento" );
    addSHeader ( "ordenasiento", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Orden" ) );
    addSHeader ( "fecha", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Fecha" ) );
    addSHeader ( "numap", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Apuntes" ) );
    addSHeader ( "numborr", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Borrador" ) );
    addSHeader ( "comentariosasiento", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Comentarios" ) );
    addSHeader ( "totaldebe", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Debe" ) );
    addSHeader ( "totalhaber", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Haber" ) );
    addSHeader ( "idasiento", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr ( "Id asiento" ) );
    addSHeader ( "clase", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Clase" ) );
    setinsercion ( FALSE );
    setDelete ( FALSE );
    setOrdenPorQuery ( TRUE );
    _depura ( "END AsientoListSubform::AsientoListSubform", 0 );
}


///
/**
**/
AsientoListSubform::~AsientoListSubform()
{
    _depura ( "AsientoListSubform::~AsientoListSubform", 0 );
    _depura ( "END AsientoListSubform::~AsientoListSubform", 0 );
}


///
/**
\return
**/
void AsientoListSubform::contextMenuEvent ( QContextMenuEvent * )
{
    _depura ( "AsientoListSubform::contextMenuEvent", 0 );
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
    popup->addSeparator();
    QAction *mostbalancedia = popup->addAction ( "Mostrar balance (dia)" );
    QAction *mostbalancemes = popup->addAction ( "Mostrar balance (mes)" );
    QAction *mostbalanceano = popup->addAction ( "Mostrar balance (ano)" );

    if ( m_delete ) {
        del = popup->addAction ( tr ( "Borrar registro" ) );
    } // end if
    popup->addSeparator();
    QAction *ajustc = popup->addAction ( tr ( "Ajustar columa" ) );
    QAction *ajustac = popup->addAction ( tr ( "Ajustar altura" ) );
    QAction *ajust = popup->addAction ( tr ( "Ajustar columnas" ) );
    QAction *ajusta = popup->addAction ( tr ( "Ajustar alturas" ) );
    popup->addSeparator();
    QAction *verconfig = popup->addAction ( tr ( "Ver configurador de subformulario" ) );

    QAction *opcion = popup->exec ( QCursor::pos() );

    if ( opcion == mostapunte ) {
        boton_asiento();
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
    if ( opcion == mostbalancedia ) {
        boton_balance1 ( 0 );
    } // end if
    if ( opcion == mostbalancemes ) {
        boton_balance1 ( 1 );
    } // end if
    if ( opcion == mostbalanceano ) {
        boton_balance1 ( 2 );
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
    delete popup;
    _depura ( "END AsientoListSubform::contextMenuEvent", 0 );
}

