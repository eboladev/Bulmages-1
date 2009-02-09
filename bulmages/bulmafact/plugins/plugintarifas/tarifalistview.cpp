/***************************************************************************
 *   Copyright (C) 2004 by J. M. Estopa Rey                                *
 *   pepma@telefonica.net                                                  *
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

#include "tarifalistview.h"
#include "tarifaview.h"
#include "pgimportfiles.h"
#include "company.h"
#include "funcaux.h"


///
/**
\param comp
\param parent
\param editomodo
**/
TarifaListView::TarifaListView ( Company *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo ), pgimportfiles ( comp )
{
    _depura ( "TarifaListView::INIT_TarifaListView()\n", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_list->setEmpresaBase ( empresaBase() );
    mui_list->cargar();
    mui_list->setColumnWidth(0, 250);
    empresaBase() ->meteWindow ( _( "Tarifas" ), this );
    setSubForm(mui_list);
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    //trataPermisos ( "tarifas" );

    _depura ( "TarifaListView::END_TarifaListView()\n", 0 );
}


///
/**
**/
TarifaListView::~TarifaListView()
{
    _depura ( "TarifaListView::INIT_destructor()\n", 0 );
    empresaBase() ->sacaWindow ( this );
    _depura ( "TarifaListView::END_destructor()\n", 0 );
}


///
/**
\param row
**/
void TarifaListView::editar (int row)
{
    _depura ( "TarifaListView::editar", 0 );
    TarifaView *tar = new TarifaView ( ( Company * ) empresaBase(), 0 );
    QObject::connect(tar, SIGNAL(guardartarifa()), this, SLOT(actualizar()));
    empresaBase() ->m_pWorkspace->addWindow ( tar );
    tar->cargar ( mui_list->DBvalue ( QString ( "idtarifa" ), row ) );
    tar->show();
    _depura ( "END TarifaListView::editar", 0 );
}


///
/**
**/
void TarifaListView::crear()
{
    _depura ( "TarifaListView::crear", 0 );
    TarifaView *tar = new TarifaView ( ( Company * ) empresaBase(), parentWidget() );
    QObject::connect(tar, SIGNAL(guardartarifa()), this, SLOT(actualizar()));
    empresaBase() ->m_pWorkspace->addWindow ( tar );
    tar->setWindowTitle(tr("Nueva tarifa"));
    tar->show();
    _depura ( "END  TarifaListView::crear", 0 );
}


///
/**
**/
void TarifaListView::borrar()
{
    _depura ( "TarifaListView::borrar\n", 0 );
    int a = mui_list->currentRow();
    if (a >= 0) {
	TarifaView *tar = new TarifaView ( ( Company * ) empresaBase(), 0 );
        QObject::connect(tar, SIGNAL(guardartarifa()), this, SLOT(actualizar()));
	tar->cargar ( mui_list->DBvalue ( QString ( "idtarifa" ), a ) );
	tar->on_mui_borrar_clicked();
	delete tar;
    } else {
	mensajeInfo(tr("No ha seleccionado ninguna tarifa"));
    } // end if
    _depura ( "END TarifaListView::borrar\n", 0 );
}


///
/**
**/
void TarifaListView::actualizar()
{
    _depura ( "TarifaListView::actualizar\n", 0 );
    mui_list->cargar();
    _depura ( "END TarifaListView::actualizar\n", 0 );
}

