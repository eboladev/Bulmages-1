/***************************************************************************
 *   Copyright (C) 2004 by J. M. Torres Rigo                               *
 *   joanmi@bulma.net                                                      *
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
#include <QMessageBox>
#include <QCheckBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>

#include "providerslist.h"
#include "blfunctions.h"
#include "bfcompany.h"
#include "provedit.h"
#include "pgimportfiles.h"


///
/**
\param comp
\param parent
\param flag
\param editmode
\return
**/
ProveedorList::ProveedorList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmode )
        : BlFormList ( comp, parent, flag, editmode ), pgimportfiles ( comp )
{
    _depura ( "ProveedorList::ProveedorList", 0 );
    setupUi ( this );
    mui_list->setMainCompany ( comp );

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ProveedorList_ProveedorList", this );
    if ( res != 0 )
        return;

    setSubForm ( mui_list );
    hideBusqueda();
    m_idprovider = "";
    m_cifprovider = "";
    m_nomprovider = "";
    if ( modoEdicion() ) {
        mainCompany() ->meteWindow ( windowTitle(), this );
    } else {
        setWindowTitle ( _( "Selector de proveedores" ) );
        mui_editar->setHidden ( TRUE );
        mui_crear->setHidden ( TRUE );
        mui_borrar->setHidden ( TRUE );
        mui_exportar->setHidden ( TRUE );
        mui_importar->setHidden ( TRUE );
        mui_imprimir->setHidden ( TRUE );
    } // end if
    presentar();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "proveedor" );
    _depura ( "END ProveedorList::ProveedorList", 0 );
}


///
/**
**/
ProveedorList::~ProveedorList()
{
    _depura ( "ProveedorList::~ProveedorList", 0 );
    _depura ( "END ProveedorList::~ProveedorList", 0 );

}


///
/**
\return
**/
QString ProveedorList::idprovider()
{
    _depura ( "ProveedorList::idprovider", 0 );
    _depura ( "END ProveedorList::idprovider", 0 );
    return m_idprovider;
}

///
/**
\return
**/
QString ProveedorList::cifprovider()
{
    _depura ( "ProveedorList::cifprovider", 0 );
    _depura ( "END ProveedorList::cifprovider", 0 );
    return m_cifprovider;
}


///
/**
\return
**/
QString ProveedorList::nomprovider()
{
    _depura ( "ProveedorList::nomprovider", 0 );
    _depura ( "END ProveedorList::nomprovider", 0 );
    return m_nomprovider;
}


///
/**
**/
void ProveedorList::presentar()
{
    _depura ( "ProveedorList::presentar", 0 );
    mui_list->cargar ( "SELECT * FROM proveedor WHERE lower(nomproveedor) LIKE lower('%" + m_filtro->text() + "%')" );
    _depura ( "END ProveedorList::presentar", 0 );
}


/// \TODO: Esta creacion debe pasar por la clase company.
/**
**/
void ProveedorList::crear()
{
    _depura ( "ProveedorList::crear", 0 );
    ProveedorView *prov = new ProveedorView( ( BfCompany * ) mainCompany() );
    mainCompany() ->m_pWorkspace->addWindow ( prov );
    prov->show();
    _depura ( "END ProveedorList::crear", 0 );
}


///
/**
\param row
\return
**/
void ProveedorList::editar ( int row )
{
    _depura ( "ProveedorList::editar", 0 );
    m_idprovider = mui_list->dbValue ( QString ( "idproveedor" ), row );
    m_cifprovider = mui_list->dbValue ( QString ( "cifproveedor" ), row );
    m_nomprovider = mui_list->dbValue ( QString ( "nomproveedor" ), row );
    if ( modoEdicion() ) {
        ProveedorView * prov = new ProveedorView( ( BfCompany * ) mainCompany() );
        if ( prov->cargar ( mui_list->dbValue ( QString ( "idproveedor" ), row ) ) ) {
            delete prov;
            return;
        } // end if
        mainCompany() ->m_pWorkspace->addWindow ( prov );
        prov->show();
    } else {
        emit ( selected ( m_idprovider ) );
    } // end if
    _depura ( "END ProveedorList::editar", 0 );
}




/// SLOT que responde a la pulsacion de borrar un determinado proveedor
/// Dicha funcion avisa de la perdida de datos y si se decide continuar
/// Se procede a borrar el proveedor.
/**
**/
void ProveedorList::borrar()
{
    _depura ( "ProveedorList::borrar", 0 );
    try {
        QString idprov = mui_list->dbValue ( QString ( "idproveedor" ) );
        ProveedorView *prov = new ProveedorView ( ( BfCompany * ) mainCompany() );
        prov->cargar ( idprov );
        prov->on_mui_borrar_clicked();
        delete prov;
        presentar();
    } catch ( ... ) {
        mensajeInfo ( _( "Error al borrar el proveedor" ) );
    } // end try
    _depura ( "END ProveedorList::borrar", 0 );
}


/// SLOT que se ejecuta al pulsar sobre el boton de imprimir en la ventana de proveedores
/**
**/
void ProveedorList::imprimir()
{
    _depura ( "ProveedorList::on_mui_imprimir_clicked", 0 );
    mui_list->imprimirPDF ( _( "Listado de Proveedores" ) );
    _depura ( "END ProveedorList::on_mui_imprimir_clicked", 0 );
}


///
/**
**/
void ProveedorList::on_mui_exportar_clicked()
{
    QFile filexml ( QFileDialog::getSaveFileName ( this,
                    _( "Seleccione el archivo" ),
                    confpr->valor ( CONF_DIR_USER ),
                    _( "Proveedores (*.xml)" ) ) );

    if ( filexml.open ( QIODevice::WriteOnly ) ) {
        bulmafact2XML ( filexml, IMPORT_PROVEEDORES );
        filexml.close();
    } else {
        _depura ( "ERROR AL ABRIR EL ARCHIVO\n", 2 );
    } // end if
}

///
/**
**/
void ProveedorList::on_mui_importar_clicked()
{
    QFile filexml ( QFileDialog::getOpenFileName ( this,
                    _( "Elija el archivo" ),
                    confpr->valor ( CONF_DIR_USER ),
                    _( "Proveedores (*.xml)" ) ) );

    if ( filexml.open ( QIODevice::ReadOnly ) ) {
        XML2BulmaFact ( filexml, IMPORT_PROVEEDORES );
        filexml.close();
        presentar();
    } else {
        _depura ( "ERROR AL ABRIR EL ARCHIVO\n", 2 );
    } // end if
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
///
/**
**/
void ProveedorListSubform::cargar()
{
    _depura ( "ProveedorListSubform::cargar", 0 );
    QString SQLQuery = "SELECT * FROM proveedor";
    BlSubForm::cargar ( SQLQuery );
    _depura ( "END ProveedorListSubform::cargar", 0 );
}


///
/**
\param a
**/
void ProveedorListSubform::cargar ( QString a )
{
    _depura ( "ProveedorListSubform::cargar", 0 );
    BlSubForm::cargar ( a );
    _depura ( "END ProveedorListSubform::cargar", 0 );
}


///
/**
\param parent
**/
ProveedorListSubform::ProveedorListSubform ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ProveedorListSubform::ProveedorListSubform", 0 );
    setDbTableName ( "proveedor" );
    setDbFieldId ( "idproveedor" );
    addSubFormHeader ( "idproveedor", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _( "ID proveedor" ) );
    addSubFormHeader ( "codproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Codigo" ) );
    addSubFormHeader ( "cifproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "C.I.F." ) );
    addSubFormHeader ( "nomproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Nombre" ) );
    addSubFormHeader ( "nomaltproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Nombre alternativo" ) );
    addSubFormHeader ( "codicliproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Codigo" ) );
    addSubFormHeader ( "cbancproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Numero de cuenta corriente" ) );
    addSubFormHeader ( "comentproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Comentarios" ) );
    addSubFormHeader ( "dirproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Direccion" ) );
    addSubFormHeader ( "poblproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Poblacion" ) );
    addSubFormHeader ( "cpproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Codigo postal" ) );
    addSubFormHeader ( "telproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Numero de telefono" ) );
    addSubFormHeader ( "faxproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Numero de fax" ) );
    addSubFormHeader ( "emailproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Direccion de correo electronico" ) );
    addSubFormHeader ( "urlproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Direccion de URL" ) );
    addSubFormHeader ( "clavewebproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Clave de acceso a la web del proveedor" ) );
    setinsercion ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    _depura ( "END ProveedorListSubform::ProveedorListSubform", 0 );
}

