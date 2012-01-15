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
    BL_FUNC_DEBUG
    setupUi ( this );
    setAttribute ( Qt::WA_DeleteOnClose );
    /// Disparamos los plugins.
    int res = g_plugins->run ( "ZList_ZList", this );
    if ( res != 0 )
        return;

    mui_list->setMainCompany ( comp );
    setSubForm ( mui_list );

    mui_list->setDbTableName ( "z" );
    mui_list->setDbFieldId ( "idz" );
    mui_list->addSubFormHeader ( "idz", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Z" ) );
    mui_list->addSubFormHeader ( "fechaz", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha" ) );
    mui_list->addSubFormHeader ( "horaz", BlDbField::DbTime, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Hora" ) );
    mui_list->addSubFormHeader ( "nomalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Almacen" ) );
    mui_list->addSubFormHeader ( "totalz", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Total" ) );
    mui_list->addSubFormHeader ( "numtickets", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Tickets" ) );
    mui_list->addSubFormHeader ( "idalmacen", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Id. almacen" ) );

    mui_list->setInsert ( FALSE );
    mui_list->setDelete ( FALSE );
    mui_list->setSortingEnabled ( TRUE );
    
    /// Cargamos los filtros guardados.
    cargaFiltrosXML();
    
    presentar();
    m_idz = "";
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
ZList::~ZList()
{
    BL_FUNC_DEBUG
    
}


///
/**
\return
**/
QString ZList::idalmacen()
{
    BL_FUNC_DEBUG
    
    return m_idz;
}


///
/**
\param comp
**/
void ZList::setMainCompany ( BfCompany *comp )
{
    BL_FUNC_DEBUG
    BlMainCompanyPointer::setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    
}


///
/**
**/
void ZList::crear()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void ZList::presentar()
{
    BL_FUNC_DEBUG

    /// Hacemos el listado y lo presentamos.
    mui_list->load ( "SELECT idz, fechaz, TO_CHAR(horaz, 'HH24:MI:SS') AS horaz, totalz, idalmacen, numtickets, codigoalmacen, nomalmacen, diralmacen, poblalmacen, cpalmacen, telalmacen, faxalmacen, emailalmacen, tipoalmacen, inactivoalmacen FROM z NATURAL JOIN almacen" );

    
}


///
/**
\return
**/
const QString ZList::generaFiltro()
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
void ZList::editar ( int row )
{
    BL_FUNC_DEBUG

    try {
        m_idz = mui_list->dbValue ( QString ( "idz" ), row );

        if ( editMode() ) {
            ZView * alm = new ZView ( ( BfCompany * ) mainCompany(), 0 );
            mainCompany() ->pWorkspace() ->addSubWindow ( alm );
            alm->show();
            alm->load ( m_idz );
        } else {
            emit ( selected ( m_idz ) );
        } // end if
        
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al editar el almacen" ) );
    } // end try

}


///
/**
**/
void ZList::imprimir()
{
    BL_FUNC_DEBUG
    mui_list->printPDF ( _ ( "Cuadres de caja" ) );
    
}


///
/**
\return
**/
void ZList::remove()
{
    BL_FUNC_DEBUG
    /*
        int a = mui_list->currentRow();
        if (a < 0) {
            blMsgInfo(tr("Debe seleccionar una linea"));
            return;
        } // end if
        try {
            m_idz = mui_list->dbValue(QString("idalmacen"));
            if (editMode()) {
                ZView *alm = new ZView((BfCompany *)mainCompany(), 0);
                mainCompany()->pWorkspace()->addSubWindow(alm);
    //            alm->show();
                alm->load(m_idz);
         alm->on_mui_borrar_clicked();
         alm->close();

            } // end if
            presentar();
        } catch (...) {
            blMsgInfo(tr("Error al borrar el almacen"));
        } // end try
    */
    
}



