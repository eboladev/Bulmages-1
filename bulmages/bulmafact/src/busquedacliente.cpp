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

#include "busquedacliente.h"
#include "clientslist.h"
#include "company.h"
#include "funcaux.h"


/** Inicializa la clase poniendo a NULL todos los valores para que no haya confusion
    en el hecho de que la clase aun no ha sido completamente inicializada.
*/
/**
\param parent
**/
BusquedaCliente::BusquedaCliente ( QWidget *parent )
        : BLWidget ( parent )
{
    _depura ( "BusquedaCliente::BusquedaCliente", 0 );
    setupUi ( this );
    /// Establecemos los Buddies:
    mui_labelCliente->setText ( tr ( "Clien&te:" ) );
    mui_labelCliente->setBuddy ( mui_buscar );

    m_nomcliente->setText ( "" );
    mdb_idcliente = "";
    mdb_nomcliente = "";
    mdb_cifcliente = "";
    mdb_codcliente = "";
    m_semaforo = FALSE;
    _depura ( "END BusquedaCliente::BusquedaCliente", 0 );
}


/** No requiere de acciones especiales en el destructor de clase.
*/
/**
**/
BusquedaCliente::~BusquedaCliente()
{
    _depura ( "BusquedaCliente::~BusquedaCliente", 0 );
    _depura ( "END BusquedaCliente::~BusquedaCliente", 0 );
}


/** Se encarga de presentar en el Widget los valores seleccionados.
*/
/**
**/
void BusquedaCliente::pinta()
{
    _depura ( "BusquedaCliente::pinta", 0 );
    m_semaforo = TRUE;
    m_cifcliente->setText ( mdb_cifcliente );

    if (mdb_codcliente.isEmpty() && mdb_nomcliente.isEmpty()) {
	m_nomcliente->setText ( "" );
    } else {
	m_nomcliente->setText ( mdb_codcliente + " - " + mdb_nomcliente );
    } // end if
    m_semaforo = FALSE;
    emit ( valueChanged ( mdb_idcliente ) );
    _depura ( "END BusquedaCliente::pinta", 0 );
}


/** Mediante este metodo indicamos al Widget que cargue un cliente determinado.
    Busca en la base de datos un cliente que coincida con el identificador pasado
    y si lo encuentra lo carga y lo presente.
    Dicho elemento se considera el elemento seleccionado.
    Al finalizar llama al metodo pinta() para que se refelejen visualmente los cambios.
*/
/**
\param val
**/
void BusquedaCliente::setidcliente ( QString val )
{
    _depura ( "BusquedaCliente::setidcliente", 0, val );
    mdb_idcliente = val;

    if ( val == "" ) {
        m_cifcliente->setText ( "" );
        m_nomcliente->setText ( "" );
        mdb_idcliente = "";
        mdb_nomcliente = "";
        mdb_cifcliente = "";
        mdb_codcliente = "";
    } else {
        QString SQLQuery = "SELECT * FROM cliente WHERE idcliente = '" + mdb_idcliente + "'";
        cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );
        if ( !cur->eof() ) {
            mdb_nomcliente = cur->valor ( "nomcliente" );
            mdb_cifcliente = cur->valor ( "cifcliente" );
            mdb_codcliente = cur->valor ( "codcliente" );
        } // end if
        delete cur;
    } // end if
    pinta();
    _depura ( "END BusquedaCliente::setidcliente", 0 );
}


///
/**
\param val
**/
void BusquedaCliente::setValorCampo ( QString val )
{
    _depura ( "BusquedaCliente::setValorCampo", 0 );
    setidcliente ( val );
    _depura ( "END BusquedaCliente::setValorCampo", 0 );
}



/** Mediante este metodo le decimos al Widget cual es el CIF del cliente seleccionado.
    Busca en la base de datos cual el cliente cuyo CIF coincida con el que hemos seleccionado
    y si lo encuentra lo pinta y lo considera el elemento seleccionado.
    Al final llama al metodo pintar() para garantizar la correcta visualizacion del elemento en pantalla.
*/
/**
\param val
**/
void BusquedaCliente::setcifcliente ( QString val )
{
    _depura ( "BusquedaCliente::setcifcliente", 0, val );
    mdb_cifcliente = val;
    QString SQLQuery = "SELECT * FROM cliente WHERE cifcliente = '" + mdb_cifcliente + "'";
    cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );

    if ( !cur->eof() ) {
        mdb_idcliente = cur->valor ( "idcliente" );
        mdb_nomcliente = cur->valor ( "nomcliente" );
        mdb_codcliente = cur->valor ( "codcliente" );
    } else {
        mdb_idcliente = "";
        mdb_nomcliente = "";
        mdb_codcliente = "";
    } // end if
    delete cur;
    pinta();
    _depura ( "END BusquedaCliente::setcifcliente", 0, val );
}


/** Resetea el valor del 'id_cliente' y vacia la informacion del formulario.
    Es util sobre todo el los filtros por cliente para anular el filtro.
*/
void BusquedaCliente::on_mui_borrar_idcliente_clicked()
{
    _depura ( "BusquedaCliente::on_mui_borrar_idcliente_clicked", 0 );
    setidcliente ( "" );
    _depura ( "END BusquedaCliente::on_mui_borrar_idcliente_clicked", 0 );
}


/** SLOT que responde a la accion de buscar un cliente.
    Abre el listado de clientes en modo Seleccionar y espera a que cerremos
    dicha ventana o que seleccionemos un elemento.
    Una vez determinado el elemento lo considera como el elemento seleccionado
    y lo muestra.
*/
/**
**/
void BusquedaCliente::on_mui_buscar_clicked()
{
    _depura ( "BusquedaCliente::on_mui_buscar_clicked", 0 );
    QDialog *diag = new QDialog ( 0 );
    diag->setModal ( true );

    ClientsList *clients = new ClientsList ( ( Company * ) empresaBase(), diag, 0, ClientsList::SelectMode );
    connect ( clients, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

    /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
    /// para que sea redimensionable y aparezca el titulo de la ventana.
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget ( clients );
    layout->setMargin ( 0 );
    layout->setSpacing ( 0 );
    diag->setLayout ( layout );
    diag->setWindowTitle ( clients->windowTitle() );

    diag->exec();
    if ( clients->idclient() != "" ) {
        setidcliente ( clients->idclient() );
    } // end if
    delete diag;

    _depura ( "END BusquedaCliente::on_mui_buscar_clicked", 0 );
}


/** SLOT que responde al fin de la edicion del campos de texto.
    Se supone que hemos introducido un cliente identificable por codigo, por nombre o por CIF
    Asi que se encarga de buscarlo y encontrarlo.
*/
/**
**/
void BusquedaCliente::on_m_cifcliente_editingFinished()
{
    _depura ( "BusquedaCliente::on_m_cifcliente_editingFinished", 0 );
    pinta();
    g_plugins->lanza("BusquedaCliente_on_m_cifcliente_editingFinished_Post",this);
    _depura ( "END BusquedaCliente::on_m_cifcliente_editingFinished", 0 );
}

/** SLOT que responde a la modificacion del campo de texto del Widget.
    A medida que vamos escribiendo en el campo de textos el sistema va buscando
    posibles clientes que cumplan las caracteristicas. Si encuentra solo uno lo
    considera como el cliente seleccionado.
*/
/**
\param val
\return
**/
void BusquedaCliente::on_m_cifcliente_textChanged ( const QString &val )
{
    _depura ( "BusquedaCliente::on_m_cifcliente_textChanged", 0 );
    if ( m_semaforo ) {
        _depura ( "END BusquedaCliente::on_m_cifcliente_textChanged", 0, "Semaforo parado" );
        return;
    } // end if

    bool encontrado = FALSE;
    QString SQLQuery = "SELECT * FROM cliente WHERE cifcliente = '" + val + "'";
    cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );
    if ( !cur->eof() ) {
        mdb_idcliente = cur->valor ( "idcliente" );
        mdb_nomcliente = cur->valor ( "nomcliente" );
        mdb_cifcliente = cur->valor ( "cifcliente" );
        mdb_codcliente = cur->valor ( "codcliente" );
        encontrado = TRUE;
    }
    delete cur;

    if ( ! encontrado ) {
        QString SQLQuery = "SELECT * FROM cliente WHERE codcliente = '" + val + "'";
        cur = empresaBase() ->cargacursor ( SQLQuery );
        if ( !cur->eof() ) {
            mdb_idcliente = cur->valor ( "idcliente" );
            mdb_nomcliente = cur->valor ( "nomcliente" );
            mdb_cifcliente = cur->valor ( "cifcliente" );
            mdb_codcliente = cur->valor ( "codcliente" );
            encontrado = TRUE;
        } // end if
        delete cur;
    } // end if


    if ( ! encontrado ) {
        QString SQLQuery = "SELECT * FROM cliente WHERE upper(nomcliente) LIKE upper('%" + val + "%')";
        cur = empresaBase() ->cargacursor ( SQLQuery );
        if ( cur->numregistros() == 1 ) {
            mdb_idcliente = cur->valor ( "idcliente" );
            mdb_nomcliente = cur->valor ( "nomcliente" );
            mdb_cifcliente = cur->valor ( "cifcliente" );
            mdb_codcliente = cur->valor ( "codcliente" );
            encontrado = TRUE;
        } // end if
        delete cur;
    } // end if


    if ( !encontrado ) {
        m_nomcliente->setText ( "" );
        mdb_idcliente = "";
        mdb_cifcliente = "";
        mdb_codcliente = "";
        mdb_nomcliente = "";
    } // end if

    if ( encontrado ) {
        m_nomcliente->setText ( mdb_codcliente + " - " + mdb_nomcliente );
    } // end if
    _depura ( "END BusquedaCliente::on_m_cifcliente_textChanged", 0 );

}


///
/**
\return
**/
QString BusquedaCliente::cifcliente()
{
    _depura ( "BusquedaCliente::cifcliente", 0 );
    _depura ( "END BusquedaCliente::cifcliente", 0 );
    return m_cifcliente->text();
}

///
/**
**/
QString BusquedaCliente::idcliente()
{
    _depura ( "BusquedaCliente::idcliente", 0 );
    _depura ( "END BusquedaCliente::idcliente", 0 );
    return mdb_idcliente;
}


///
/**
\return
**/
QString BusquedaCliente::valorCampo()
{
    _depura ( "BusquedaCliente::valorCampo", 0 );
    _depura ( "END BusquedaCliente::valorCampo", 0 );
    return mdb_idcliente;
}


///
/**
\return
**/
QString BusquedaCliente::nomcliente()
{
    _depura ( "BusquedaCliente::nomcliente", 0 );
    _depura ( "END BusquedaCliente::nomcliente", 0 );
    return mdb_nomcliente;
}
