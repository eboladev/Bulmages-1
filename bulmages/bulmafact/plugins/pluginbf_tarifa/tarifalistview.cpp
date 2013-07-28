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
#include "blimportexport.h"
#include "bfcompany.h"
#include "blfunctions.h"


///
/**
\param comp
\param parent
\param editomodo
**/
TarifaListView::TarifaListView ( BfCompany *comp, QWidget *parent, Qt::WindowFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo ), BlImportExport ( comp )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_list->setMainCompany ( mainCompany() );
    mui_list->load();
    mui_list->setColumnWidth ( 0, 250 );
    mainCompany() ->insertWindow ( _ ( "Tarifas" ), this );
    setSubForm ( mui_list );
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    //trataPermisos ( "tarifas" );
    /// Llamamos a los scripts
    blScript(this);
    
}


///
/**
**/
TarifaListView::~TarifaListView()
{
    BL_FUNC_DEBUG
    mainCompany() ->removeWindow ( this );
    
}


///
/**
\param row
**/
void TarifaListView::editar ( int row )
{
    BL_FUNC_DEBUG
    TarifaView *tar = new TarifaView ( ( BfCompany * ) mainCompany(), 0 );
    QObject::connect ( tar, SIGNAL ( guardartarifa() ), this, SLOT ( actualizar() ) );
    mainCompany() ->m_pWorkspace->addSubWindow ( tar );
    tar->load ( mui_list->dbValue ( QString ( "idtarifa" ), row ) );
    tar->show();
    
}


///
/**
**/
void TarifaListView::crear()
{
    BL_FUNC_DEBUG
    TarifaView *tar = new TarifaView ( ( BfCompany * ) mainCompany(), parentWidget() );
    QObject::connect ( tar, SIGNAL ( guardartarifa() ), this, SLOT ( actualizar() ) );
    mainCompany() ->m_pWorkspace->addSubWindow ( tar );
    tar->setWindowTitle ( _ ( "Nueva tarifa" ) );
    tar->show();
    
}


///
/**
**/
void TarifaListView::remove()
{
    BL_FUNC_DEBUG
    int a = mui_list->currentRow();
    if ( a >= 0 ) {
        TarifaView *tar = new TarifaView ( ( BfCompany * ) mainCompany(), 0 );
        QObject::connect ( tar, SIGNAL ( guardartarifa() ), this, SLOT ( actualizar() ) );
        tar->load ( mui_list->dbValue ( QString ( "idtarifa" ), a ) );
        tar->on_mui_borrar_clicked();
        delete tar;
    } else {
        blMsgInfo ( _ ( "No ha seleccionado ninguna tarifa" ) );
    } // end if
    
}


///
/**
**/
void TarifaListView::actualizar()
{
    BL_FUNC_DEBUG
    mui_list->load();
    
}

