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
#include "company.h"
#include "blfunctions.h"
#include "plugins.h"


///
/**
\param comp
\param parent
\param flag
\return
**/
ZList::ZList ( Company *comp, QWidget *parent, Qt::WFlags flag )
        : BlFormList ( comp, parent, flag )
{
    _depura ( "ZList::ZList(2)", 0 );
    setupUi ( this );
    setAttribute ( Qt::WA_DeleteOnClose );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ZList_ZList", this );
    if ( res != 0 )
        return;
    mui_list->setMainCompany ( comp );
    setSubForm ( mui_list );

    mui_list->setDBTableName ( "z" );
    mui_list->setDBCampoId ( "idz" );
    mui_list->addSHeader ( "idz", BlDbField::DBint, BlDbField::DBNotNull | BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "ID Z" ) );
    mui_list->addSHeader ( "fechaz", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Fecha" ) );
    mui_list->addSHeader ( "horaz", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Hora" ) );
    mui_list->addSHeader ( "nomalmacen", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Almacen" ) );
    mui_list->addSHeader ( "totalz", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Total" ) );
    mui_list->addSHeader ( "numtickets", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Tickets" ) );
    mui_list->addSHeader ( "idalmacen", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, _( "Id. almacen" ) );

    mui_list->setinsercion ( FALSE );
    mui_list->setDelete ( FALSE );
    mui_list->setSortingEnabled ( TRUE );

    presentar();
    m_idz = "";
    empresaBase() ->meteWindow ( windowTitle(), this );
    hideBusqueda();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "almacen" );
    _depura ( "END ZList::ZList(2)", 0 );
}


///
/**
**/
ZList::~ZList()
{
    _depura ( "ZList::~ZList", 0 );
    _depura ( "END ZList::~ZList", 0 );
}


///
/**
\return
**/
QString ZList::idalmacen()
{
    _depura ( "ZList::idalmacenk", 0 );
    _depura ( "END ZList::idalmacenk", 0 );
    return m_idz;
}


///
/**
\param comp
**/
void ZList::setMainCompany ( Company *comp )
{
    _depura ( "ZList::setMainCompany", 0 );
    BlMainCompanyPointer::setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    _depura ( "END ZList::setMainCompany", 0 );
}


///
/**
**/
void ZList::crear()
{
    _depura ( "ZList::crear", 0 );
    /*
        ZView *alm = new ZView((Company *)empresaBase(), 0);
        empresaBase()->pWorkspace()->addWindow(alm);
        alm->show();
    */
    _depura ( "END ZList::crear", 0 );
}


///
/**
**/
void ZList::presentar()
{
    _depura ( "ZList::presentar", 0 );

    /// Hacemos el listado y lo presentamos.
    mui_list->cargar ( "SELECT * FROM z NATURAL LEFT JOIN almacen" );

    _depura ( "END ZList::presentar", 0 );
}


///
/**
\return
**/
QString ZList::generaFiltro()
{
    _depura ( "ZList::generaFiltro", 0 );
    /// Tratamiento de los filtros.
    QString filtro = "";

    return ( filtro );
    _depura ( "END ZList::generaFiltro", 0 );
}


///
/**
\param row
**/
void ZList::editar ( int row )
{
    _depura ( "ZList::editar", 0 );

    try {
        m_idz = mui_list->DBvalue ( QString ( "idz" ), row );

        if ( modoEdicion() ) {
            ZView * alm = new ZView ( ( Company * ) empresaBase(), 0 );
            empresaBase() ->pWorkspace() ->addWindow ( alm );
            alm->show();
            alm->cargar ( m_idz );
        } else {
            emit ( selected ( m_idz ) );
        } // end if
        _depura ( "END ZList::editar", 0 );
    } catch ( ... ) {
        mensajeInfo ( _( "Error al editar el almacen" ) );
    } // end try

}


///
/**
**/
void ZList::imprimir()
{
    _depura ( "ZList::imprimir", 0 );
    mui_list->imprimirPDF ( _( "Almacenes" ) );
    _depura ( "END ZList::imprimir", 0 );
}


///
/**
\return
**/
void ZList::borrar()
{
    _depura ( "ZList::borrar", 0 );
    /*
        int a = mui_list->currentRow();
        if (a < 0) {
            mensajeInfo(tr("Debe seleccionar una linea"));
            return;
        } // end if
        try {
            m_idz = mui_list->DBvalue(QString("idalmacen"));
            if (modoEdicion()) {
                ZView *alm = new ZView((Company *)empresaBase(), 0);
                empresaBase()->pWorkspace()->addWindow(alm);
    //            alm->show();
                alm->cargar(m_idz);
         alm->on_mui_borrar_clicked();
         alm->close();
     
            } // end if
            presentar();
        } catch (...) {
            mensajeInfo(tr("Error al borrar el almacen"));
        } // end try
    */
    _depura ( "END ZList::borrar", 0 );
}



