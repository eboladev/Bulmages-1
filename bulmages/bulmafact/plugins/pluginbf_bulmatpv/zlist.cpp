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

#include <QLineEdit>
#include <QTextStream>
#include <QFileDialog>
#include <QCheckBox>
#include <QMessageBox>

#include "zlist.h"
#include "zview.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "blplugins.h"


///
/**
\param comp
\param parent
\param flag
\return
**/
ZList::ZList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag )
        : BlFormList ( comp, parent, flag )
{
    blDebug ( "ZList::ZList(2)", 0 );
    setupUi ( this );
    setAttribute ( Qt::WA_DeleteOnClose );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ZList_ZList", this );
    if ( res != 0 )
        return;

    mui_list->setMainCompany ( comp );
    setSubForm ( mui_list );

    mui_list->setDbTableName ( "z" );
    mui_list->setDbFieldId ( "idz" );
    mui_list->addSubFormHeader ( "idz", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Z" ) );
    mui_list->addSubFormHeader ( "fechaz", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha" ) );
    mui_list->addSubFormHeader ( "horaz", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Hora" ) );
    mui_list->addSubFormHeader ( "nomalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Almacen" ) );
    mui_list->addSubFormHeader ( "totalz", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Total" ) );
    mui_list->addSubFormHeader ( "numtickets", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Tickets" ) );
    mui_list->addSubFormHeader ( "idalmacen", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Id. almacen" ) );

    mui_list->setInsert ( FALSE );
    mui_list->setDelete ( FALSE );
    mui_list->setSortingEnabled ( TRUE );

    presentar();
    m_idz = "";
    mainCompany() ->meteWindow ( windowTitle(), this );
    hideBusqueda();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "almacen" );
    blDebug ( "END ZList::ZList(2)", 0 );
}


///
/**
**/
ZList::~ZList()
{
    blDebug ( "ZList::~ZList", 0 );
    blDebug ( "END ZList::~ZList", 0 );
}


///
/**
\return
**/
QString ZList::idalmacen()
{
    blDebug ( "ZList::idalmacenk", 0 );
    blDebug ( "END ZList::idalmacenk", 0 );
    return m_idz;
}


///
/**
\param comp
**/
void ZList::setMainCompany ( BfCompany *comp )
{
    blDebug ( "ZList::setMainCompany", 0 );
    BlMainCompanyPointer::setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    blDebug ( "END ZList::setMainCompany", 0 );
}


///
/**
**/
void ZList::crear()
{
    blDebug ( "ZList::crear", 0 );
    /*
        ZView *alm = new ZView((BfCompany *)mainCompany(), 0);
        mainCompany()->pWorkspace()->addWindow(alm);
        alm->show();
    */
    blDebug ( "END ZList::crear", 0 );
}


///
/**
**/
void ZList::presentar()
{
    blDebug ( "ZList::presentar", 0 );

    /// Hacemos el listado y lo presentamos.
    mui_list->cargar ( "SELECT * FROM z NATURAL LEFT JOIN almacen" );

    blDebug ( "END ZList::presentar", 0 );
}


///
/**
\return
**/
QString ZList::generaFiltro()
{
    blDebug ( "ZList::generaFiltro", 0 );
    /// Tratamiento de los filtros.
    QString filtro = "";

    return ( filtro );
    blDebug ( "END ZList::generaFiltro", 0 );
}


///
/**
\param row
**/
void ZList::editar ( int row )
{
    blDebug ( "ZList::editar", 0 );

    try {
        m_idz = mui_list->dbValue ( QString ( "idz" ), row );

        if ( modoEdicion() ) {
            ZView * alm = new ZView ( ( BfCompany * ) mainCompany(), 0 );
            mainCompany() ->pWorkspace() ->addWindow ( alm );
            alm->show();
            alm->cargar ( m_idz );
        } else {
            emit ( selected ( m_idz ) );
        } // end if
        blDebug ( "END ZList::editar", 0 );
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al editar el almacen" ) );
    } // end try

}


///
/**
**/
void ZList::imprimir()
{
    blDebug ( "ZList::imprimir", 0 );
    mui_list->imprimirPDF ( _ ( "Almacenes" ) );
    blDebug ( "END ZList::imprimir", 0 );
}


///
/**
\return
**/
void ZList::borrar()
{
    blDebug ( "ZList::borrar", 0 );
    /*
        int a = mui_list->currentRow();
        if (a < 0) {
            blMsgInfo(tr("Debe seleccionar una linea"));
            return;
        } // end if
        try {
            m_idz = mui_list->dbValue(QString("idalmacen"));
            if (modoEdicion()) {
                ZView *alm = new ZView((BfCompany *)mainCompany(), 0);
                mainCompany()->pWorkspace()->addWindow(alm);
    //            alm->show();
                alm->cargar(m_idz);
         alm->on_mui_borrar_clicked();
         alm->close();

            } // end if
            presentar();
        } catch (...) {
            blMsgInfo(tr("Error al borrar el almacen"));
        } // end try
    */
    blDebug ( "END ZList::borrar", 0 );
}



