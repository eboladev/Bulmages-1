/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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
#include <QCheckBox>
#include <QFile>
#include <QCheckBox>
#include <QTextStream>

#include "almaceneslistview.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "blplugins.h"
#include "blconfiguration.h"
#include "almacenview.h"


///
/**
\param parent
\param flag
\return
**/
AlmacenesListView::AlmacenesListView ( QWidget *parent, Qt::WFlags flag )
        : BlFormList ( NULL, parent, flag )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    setAttribute ( Qt::WA_DeleteOnClose );
    /// Disparamos los plugins.
    int res = g_plugins->run ( "AlmacenesListView_AlmacenesListView", this );
    if ( res != 0 )
        return;
    m_idalmacen = "";
    setSubForm ( mui_list );

    mui_list->setDbTableName ( "almacen" );
    mui_list->setDbFieldId ( "idalmacen" );
    mui_list->addSubFormHeader ( "idalmacen", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID almacen" ) );
    mui_list->addSubFormHeader ( "codigoalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Codigo almacen" ) );
    mui_list->addSubFormHeader ( "nomalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre almacen" ) );
    mui_list->setInsert ( FALSE );
    mui_list->setDelete ( FALSE );
    mui_list->setSortingEnabled ( TRUE );

    hideBusqueda();
    /// Llamamos a los scripts
    blScript(this);
    
}


///
/**
\param comp
\param parent
\param flag
\return
**/
AlmacenesListView::AlmacenesListView ( BfCompany *comp, QWidget *parent, Qt::WFlags flag )
        : BlFormList ( comp, parent, flag )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    setAttribute ( Qt::WA_DeleteOnClose );
    /// Disparamos los plugins.
    int res = g_plugins->run ( "AlmacenesListView_AlmacenesListView", this );
    if ( res != 0 )
        return;
    mui_list->setMainCompany ( comp );
    setSubForm ( mui_list );

    mui_list->setDbTableName ( "almacen" );
    mui_list->setDbFieldId ( "idalmacen" );
    mui_list->addSubFormHeader ( "idalmacen", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID almacen" ) );
    mui_list->addSubFormHeader ( "codigoalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Codigo almacen" ) );
    mui_list->addSubFormHeader ( "nomalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre almacen" ) );
    mui_list->setInsert ( FALSE );
    mui_list->setDelete ( FALSE );
    mui_list->setSortingEnabled ( TRUE );

    presentar();
    m_idalmacen = "";
    mainCompany() ->insertWindow ( windowTitle(), this );
    hideBusqueda();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "almacen" );
    /// Llamamos a los scripts
    blScript(this);
    
}


///
/**
**/
AlmacenesListView::~AlmacenesListView()
{
    BL_FUNC_DEBUG
    
}


///
/**
\return
**/
QString AlmacenesListView::idalmacen()
{
    BL_FUNC_DEBUG
    
    return m_idalmacen;
}


///
/**
\param comp
**/
void AlmacenesListView::setMainCompany ( BfCompany *comp )
{
    BL_FUNC_DEBUG
    BlMainCompanyPointer::setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    
}


///
/**
**/
void AlmacenesListView::crear()
{
    BL_FUNC_DEBUG
    /*
        ((BfCompany *)mainCompany())->s_newAlmacen();
    */
    AlmacenView *alm = new AlmacenView ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->pWorkspace() ->addSubWindow ( alm );
    alm->show();
    
}


///
/**
**/
void AlmacenesListView::presentar()
{
    BL_FUNC_DEBUG

    /// Hacemos el listado y lo presentamos.
    mui_list->load ( "SELECT * FROM almacen" );

    
}


///
/**
\return
**/
const QString AlmacenesListView::generaFiltro()
{
    BL_FUNC_DEBUG
    /// Tratamiento de los filtros.
    QString filtro = "";

    /// Hacemos el filtrado like del campo m_filtro
    filtro += mui_list->likeFilterSQL(m_filtro->text());
    
    return ( filtro );
    
}


///
/**
\param row
**/
void AlmacenesListView::editar ( int row )
{
    BL_FUNC_DEBUG
    try {
        m_idalmacen = mui_list->dbValue ( QString ( "idalmacen" ), row );

        if ( editMode() ) {
            AlmacenView * alm = new AlmacenView ( ( BfCompany * ) mainCompany(), 0 );
            mainCompany() ->pWorkspace() ->addSubWindow ( alm );
            alm->show();
            alm->load ( m_idalmacen );
        } else {
            emit ( selected ( m_idalmacen ) );
        } // end if
        
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al editar el almacen" ) );
    } // end try
}


///
/**
**/
void AlmacenesListView::imprimir()
{
    BL_FUNC_DEBUG
    mui_list->printPDF ( _ ( "Almacenes" ) );
    
}


///
/**
\return
**/
void AlmacenesListView::remove()
{
    BL_FUNC_DEBUG
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        blMsgInfo ( _ ( "Debe seleccionar una linea" ) );
        return;
    } // end if
    try {
        m_idalmacen = mui_list->dbValue ( QString ( "idalmacen" ) );
        if ( editMode() ) {
            AlmacenView * alm = new AlmacenView ( ( BfCompany * ) mainCompany(), 0 );
            mainCompany() ->pWorkspace() ->addSubWindow ( alm );
//            alm->show();
            alm->load ( m_idalmacen );
            alm->on_mui_borrar_clicked();
            alm->close();
            /*
                        PresupuestoView *pv = ((BfCompany *)mainCompany())->nuevoPresupuestoView();
                        if (pv->load(m_idalmacen))
                            throw -1;
                        pv->on_mui_borrar_clicked();
                        pv->close();
            */
        } // end if
        presentar();
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al borrar el almacen" ) );
    } // end try
    
}


