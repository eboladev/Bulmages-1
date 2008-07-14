/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include <QDialog>

#include "busquedaproveedor.h"
#include "providerslist.h"
#include "company.h"
#include "funcaux.h"


/** Inicializa los componentes del Widget a NULL para que no haya confusiones en saber
    si han sido creados o no.
*/
/**
\param parent
**/
BusquedaProveedor::BusquedaProveedor ( QWidget *parent )
        : BLWidget ( parent )
{
    _depura ( "BusquedaProveedor::BusquedaProveedor", 0 );
    setupUi ( this );
    mdb_idproveedor = "";
    mdb_nomproveedor = "";
    mdb_cifproveedor = "";
    mdb_codproveedor = "";
    m_semaforo = FALSE;
    _depura ( "END BusquedaProveedor::BusquedaProveedor", 0 );
}


/** Presenta en pantalla el proveedor seleccionado.
    Usa la variable de clase m_semaforo para que no haya dead-lock entre este metodo y
    el slot on_mui_textChanged
*/
/**
**/
void BusquedaProveedor::pinta()
{
    _depura ( "BusquedaProveedor::pinta", 0 );
    m_semaforo = TRUE;
    m_cifproveedor->setText ( mdb_cifproveedor );

    if ( mdb_codproveedor.isEmpty() && mdb_nomproveedor.isEmpty() ) {
        m_nomproveedor->setText ( "" );
    } else {
        m_nomproveedor->setText ( mdb_codproveedor + " - " + mdb_nomproveedor );
    } // end if
    m_semaforo = FALSE;
    emit ( valueChanged ( mdb_idproveedor ) );
    _depura ( "END BusquedaProveedor::pinta", 0 );
}


/** No hay actividades necesarias en el destructor.
*/
/**
**/
BusquedaProveedor::~BusquedaProveedor()
{
    _depura ( "BusquedaProveedor::~BusquedaProveedor", 0 );
    _depura ( "END BusquedaProveedor::~BusquedaProveedor", 0 );
}


/** Mediante este metodo indicamos al Widget que proveedor debe ser puesto como el Seleccionado.
    Busca en la tabla proveedor de la Base de Datos un registro coincidente.
    Cuando lo tiene establece las variables que lo contendran y llama al metodo pintar().
*/
/**
\param val
**/
void BusquedaProveedor::setidproveedor ( QString val )
{
    _depura ( "BusquedaProveedor::setidproveedor", 0 );
    mdb_idproveedor = val;

    if ( val == "" ) {
        m_cifproveedor->setText ( "" );
        m_nomproveedor->setText ( "" );
        mdb_idproveedor = "";
        mdb_nomproveedor = "";
        mdb_cifproveedor = "";
        mdb_codproveedor = "";
    } else {
        QString SQLQuery = "SELECT * FROM proveedor WHERE idproveedor = '" + mdb_idproveedor + "'";
        cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );
        if ( !cur->eof() ) {
            mdb_cifproveedor = cur->valor ( "cifproveedor" );
            mdb_nomproveedor = cur->valor ( "nomproveedor" );
            mdb_codproveedor = cur->valor ( "codproveedor" );
        } // end if
        delete cur;
    } // end if
    pinta();
    _depura ( "END BusquedaProveedor::setidproveedor", 0 );
}

///
/**
\param val
**/
void BusquedaProveedor::setValorCampo ( QString val )
{
    _depura ( "BusquedaProveedor::setValorCampo", 0 );
    setidproveedor ( val );
    _depura ( "END BusquedaProveedor::setValorCampo", 0 );
}

/** Mediante este metodo indicamos al Widget que proveedor debe ser puesto como el Seleccionado.
    Busca en la tabla proveedor de la Base de Datos un registro con CIF coincidente.
    Cuando lo tiene establece las variables que lo contendran y llama al metodo pintar().
*/
/**
\param val
**/
void BusquedaProveedor::setcifproveedor ( QString val )
{
    _depura ( "BusquedaProveedor::setcifproveedor", 0 );
    mdb_cifproveedor = val;
    QString SQLQuery = "SELECT * FROM proveedor WHERE cifproveedor = '" + mdb_cifproveedor + "'";
    cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );

    if ( !cur->eof() ) {
        mdb_idproveedor = cur->valor ( "idproveedor" );
        mdb_nomproveedor = cur->valor ( "nomproveedor" );
        mdb_codproveedor = cur->valor ( "codproveedor" );
    } else {
        mdb_idproveedor = "";
        mdb_nomproveedor = "";
        mdb_codproveedor = "";
    } // end if

    delete cur;
    pinta();
    _depura ( "END BusquedaProveedor::setcifproveedor", 0 );
}


/** SLOT que responde a la pulsacion del boton buscar en el Widget.
    Crea una instancia del listado de proveedores en el modo seleccion.
    Muestra dicha instancia como un popup y se queda a la espera de que se le devuelva el control.
    Actualiza su estado y su proveedor seleccionado deacuerdo a lo establecido en la ventana de listado.
*/
/**
**/
void BusquedaProveedor::on_mui_buscar_clicked()
{
    _depura ( "BusquedaProveedor::on_mui_buscar_clicked", 0 );

    /// Esto es convertir un QWidget en un sistema modal de di&aacute;logo.
    QDialog *diag = new QDialog ( 0 );
    diag->setModal ( true );
    diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
    centrarEnPantalla ( diag );

    ProveedorList *providers = new ProveedorList ( ( Company * ) empresaBase(), diag, 0, ProveedorList::SelectMode );
    connect ( providers, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

    /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
    /// para que sea redimensionable y aparezca el titulo de la ventana.
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget ( providers );
    layout->setMargin ( 0 );
    layout->setSpacing ( 0 );
    diag->setLayout ( layout );
    diag->setWindowTitle ( providers->windowTitle() );

    diag->exec();

    if ( providers->cifprovider() != "" ) {
        setcifproveedor ( providers->cifprovider() );
        emit ( valueChanged ( mdb_idproveedor ) );
    } // end if

    delete providers;
    delete diag;
    _depura ( "END BusquedaProveedor::on_mui_buscar_clicked", 0 );
}

/** SLOT que responde al fin de la edicion del campo de texto para el CIF.
    Llama al metodo pintar()
*/
/**
**/
void BusquedaProveedor::on_m_cifproveedor_editingFinished()
{
    _depura ( "BusquedaProveedor::on_m_cifproveedor_editingFinished", 0 );
    pinta();
    emit ( valueChanged ( mdb_idproveedor ) );
    _depura ( "END BusquedaProveedor::on_m_cifproveedor_editingFinished", 0 );
}


/** Resetea el valor del 'id_proveedor' y vacia la informacion del formulario.
    Es util sobre todo el los filtros por proveedor para anular el filtro.
*/
void BusquedaProveedor::on_mui_borrar_idproveedor_clicked()
{
    _depura ( "BusquedaProveedor::on_mui_borrar_idproveedor_clicked", 0 );
    setidproveedor ( "" );
    _depura ( "END BusquedaProveedor::on_mui_borrar_idproveedor_clicked", 0 );
}


/** SLOT que responde a la produccion de cambios en el campo de texto del widget.
    Busca en la tabla proveedor un registro que se parezca en cif y si lo encuentra.
    Lo considera como el elemento seleccionado.
*/
/**
\param val
\return
**/
void BusquedaProveedor::on_m_cifproveedor_textChanged ( const QString &val )
{
    _depura ( "BusquedaProveedor::on_m_cifproveedor_textChanged", 0 );
    if ( m_semaforo ) {
        _depura ( "BusquedaProveedor::on_m_cifproveedor_textChanged", 0, "Semaforo en rojo" );
        return;
    } // end if

    bool encontrado = FALSE;
    QString SQLQuery = "SELECT * FROM proveedor WHERE cifproveedor = '" + val + "'";
    cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );
    if ( !cur->eof() ) {
        mdb_idproveedor = cur->valor ( "idproveedor" );
        mdb_nomproveedor = cur->valor ( "nomproveedor" );
        mdb_cifproveedor = cur->valor ( "cifproveedor" );
        mdb_codproveedor = cur->valor ( "codproveedor" );
        encontrado = TRUE;
    }
    delete cur;

    if ( !encontrado ) {
        QString SQLQuery = "SELECT * FROM proveedor WHERE codproveedor = '" + val + "'";
        cur = empresaBase() ->cargacursor ( SQLQuery );
        if ( !cur->eof() ) {
            mdb_idproveedor = cur->valor ( "idproveedor" );
            mdb_nomproveedor = cur->valor ( "nomproveedor" );
            mdb_cifproveedor = cur->valor ( "cifproveedor" );
            mdb_codproveedor = cur->valor ( "codproveedor" );
            encontrado = TRUE;
        } // end if
        delete cur;
    } // end if

    if ( !encontrado ) {
        QString SQLQuery = "SELECT * FROM proveedor WHERE upper(nomproveedor) LIKE upper('%" + val + "%')";
        cur = empresaBase() ->cargacursor ( SQLQuery );
        if ( cur->numregistros() == 1 ) {
            mdb_idproveedor = cur->valor ( "idproveedor" );
            mdb_nomproveedor = cur->valor ( "nomproveedor" );
            mdb_cifproveedor = cur->valor ( "cifproveedor" );
            mdb_codproveedor = cur->valor ( "codproveedor" );
            encontrado = TRUE;
        } // end if
        delete cur;
    } // end if


    if ( !encontrado ) {
        m_nomproveedor->setText ( "" );
    } // end if

    if ( encontrado ) {
        m_nomproveedor->setText ( mdb_codproveedor + ".- " + mdb_nomproveedor );
    } // end if
    _depura ( "END BusquedaProveedor::on_m_cifproveedor_textChanged", 0 );
}


/** Devuelve el cif del proveedor seleccionado, si no hay ningun proveedor seleccionado devuelve un string vacio
**/
/**
\return
**/
QString BusquedaProveedor::cifproveedor()
{
    _depura ( "BusquedaProveedor::cifproveedor", 0 );
    _depura ( "END BusquedaProveedor::cifproveedor", 0 );
    return m_cifproveedor->text();
}


/** Devuelve el id del proveedor seleccionado, si ni hay ningun proveedor seleccionado devuelve un string vacio
**/
/**
\return
**/
QString BusquedaProveedor::idproveedor()
{
    _depura ( "BusquedaProveedor::idproveedor", 0 );
    _depura ( "END BusquedaProveedor::idproveedor", 0 );
    return mdb_idproveedor;
}


/** Devuelve el id del proveedor seleccionado, si ni hay ningun proveedor seleccionado devuelve un string vacio
**/
/**
\return
**/
QString BusquedaProveedor::valorCampo()
{
    _depura ( "BusquedaProveedor::idproveedor", 0 );
    _depura ( "END BusquedaProveedor::idproveedor", 0 );
    return mdb_idproveedor;
}


/** Devuelve el nombre del proveedor seleccionado, si no hay ningun proveedor seleccionado devuelve un string vacio **/
/**
\return
**/
QString BusquedaProveedor::nomproveedor()
{
    _depura ( "BusquedaProveedor::nomproveedor", 0 );
    _depura ( "END BusquedaProveedor::nomproveedor", 0 );
    return nomproveedor();
}

