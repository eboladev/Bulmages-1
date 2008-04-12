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

#include "busquedaarticulo.h"
#include "articulolist.h"
#include "company.h"
#include "funcaux.h"


/** Inicializa todos los componentes a null para que no haya posibles
    errores al introducir el puntero a company. */
/**
\param parent
**/
BusquedaArticulo::BusquedaArticulo ( QWidget *parent )
        : BLWidget ( parent )
{
    _depura ( "BusquedaArticulo::BusquedaArticulo", 0 );
    setupUi ( this );
    mdb_idarticulo = "";
    mdb_nomarticulo = "";
    mdb_codigocompletoarticulo = "";
    m_semaforo = FALSE;
    _depura ( "END BusquedaArticulo::BusquedaArticulo", 0 );
}


/** El destructor no requiere de acciones especiales.
*/
/**
**/
BusquedaArticulo::~BusquedaArticulo()
{
    _depura ( "BusquedaArticulo::~BusquedaArticulo", 0 );
    _depura ( "END BusquedaArticulo::~BusquedaArticulo", 0 );
}


/** Con este metodo indicamos al Widget que articulo presentar como cargado.
    Lo busca en la base de datos y muestra el codigocompleto y el nombre.
*/
/**
\param val
**/
void BusquedaArticulo::setidarticulo ( QString val )
{
    _depura ( "BusquedaArticulo::setidarticulo", 0 );
    mdb_idarticulo = val;
    QString SQLQuery = "SELECT * FROM articulo WHERE idarticulo='" + mdb_idarticulo + "'";
    cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );

    if ( !cur->eof() ) {
        mdb_codigocompletoarticulo = cur->valor ( "codigocompletoarticulo" );
        mdb_nomarticulo = cur->valor ( "nomarticulo" );
    } else {
        mdb_idarticulo = "";
        mdb_nomarticulo = "";
        mdb_codigocompletoarticulo = "";
    } // end if

    delete cur;
    m_codigocompletoarticulo->setText ( mdb_codigocompletoarticulo );
    m_nomarticulo->setText ( mdb_nomarticulo );
    pinta();
    _depura ( "END BusquedaArticulo::setidarticulo", 0 );
}


/** Se encarga de presentar en el Widget los valores seleccionados.
*/
/**
**/
void BusquedaArticulo::pinta()
{
    _depura ( "BusquedaCliente::pinta", 0 );
    m_semaforo = TRUE;
//    m_cifcliente->setText ( mdb_cifcliente );
//    m_nomcliente->setText ( mdb_codcliente + " - " + mdb_nomcliente );
    m_codigocompletoarticulo->setText ( mdb_codigocompletoarticulo );
    m_nomarticulo->setText ( mdb_nomarticulo );
    m_semaforo = FALSE;
    emit ( valueChanged ( mdb_idarticulo ) );
    _depura ( "END BusquedaCliente::pinta", 0 );
}


///
/**
\param val
**/
void BusquedaArticulo::setValorCampo ( QString val )
{
    _depura ( "BusquedaArticulo::setValorCampo", 0 );
    setidarticulo ( val );
    _depura ( "END BusquedaArticulo::setValorCampo", 0 );
}



/** Con este metodo indicamos cual es el articulo que debe presentar el Widget
    como seleccionado, indicando cual es su codigocompleto ya que es un indice
    muy utilizado con los articulos.
*/
/**
\param val
**/
void BusquedaArticulo::setcodigocompletoarticulo ( QString val )
{
    _depura ( "BusquedaArticulo::setcodigocompletoarticulo", 0 );
    mdb_codigocompletoarticulo = val;
    QString SQLQuery = "SELECT * FROM articulo WHERE codigocompletoarticulo='" + mdb_codigocompletoarticulo + "'";
    cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );
    if ( !cur->eof() ) {
        mdb_idarticulo = cur->valor ( "idarticulo" );
        mdb_nomarticulo = cur->valor ( "nomarticulo" );
    } else {
        mdb_idarticulo = "";
        mdb_nomarticulo = "";
    } // end if
    delete cur;
    pinta();
    _depura ( "END BusquedaArticulo::setcodigocompletoarticulo", 0 );
}


/** SLOT que responde a la pulsacion del boton buscar.
    Crea un listado de articulos en modo seleccion y lo presenta quedando a la
    espera de que se seleccione un articulo o se cierre dicho listado.
    Una vez seleccionado un articulo lo considera como el articulo seleccionado por
    el Widget.
*/
/**
**/
void BusquedaArticulo::on_mui_buscar_clicked()
{
    _depura ( "BusquedaArticulo::on_mui_buscar_clicked", 0 );
    QDialog *diag = new QDialog ( 0 );
    diag->setModal ( true );
    ArticuloList *articulos = new ArticuloList ( ( Company * ) empresaBase(), diag, 0, ArticuloList::SelectMode );
    connect ( articulos, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

    /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
    /// para que sea redimensionable y aparezca el titulo de la ventana.
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget ( articulos );
    layout->setMargin ( 0 );
    layout->setSpacing ( 0 );
    diag->setLayout ( layout );
    diag->setWindowTitle ( articulos->windowTitle() );

    diag->exec();
    if ( articulos->codigocompletoarticulo() != "" ) {
/*
        m_codigocompletoarticulo->setText ( articulos->codigocompletoarticulo() );
        mdb_codigocompletoarticulo = articulos->codigocompletoarticulo();
        m_nomarticulo->setText ( articulos->nomarticulo() );
        mdb_nomarticulo = articulos->nomarticulo();
        mdb_idarticulo = articulos->idarticulo();
*/
	setcodigocompletoarticulo(articulos->codigocompletoarticulo());
    } // end if
    delete diag;
}


/** SLOT que responde a terminar de escribir en el cuadro de texto del Widget.
    Busca en la tabla de articulos un elemento con el codigocompletoarticulo coincidente
    y si lo encuentra lo muestra y lo considera como el elemento seleccionado por este Widget.
*/
/**
\param val
**/
void BusquedaArticulo::on_m_codigocompletoarticulo_textChanged ( const QString &val )
{
    _depura ( "BusquedaArticulo::on_m_codigocompletoarticulo_textChanged", 0 );
    if ( m_semaforo ) {
        _depura ( "END BusquedaArticulo::on_m_codigocompletoarticulo_textChanged", 0, "Semaforo parado" );
        return;
    } // end if

    bool encontrado = FALSE;
    QString SQLQuery = "SELECT * FROM articulo WHERE codigocompletoarticulo = '" + val + "'";
    cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );
    if ( !cur->eof() ) {
        mdb_idarticulo = cur->valor ( "idarticulo" );
        mdb_nomarticulo = cur->valor ( "nomarticulo" );
	mdb_codigocompletoarticulo = cur->valor("codigocompletoarticulo");
        encontrado = TRUE;
    }
    delete cur;

    bool buscarmas = TRUE;
    if ( ! encontrado ) {
        QString SQLQuery = "SELECT * FROM articulo WHERE upper(codigocompletoarticulo) LIKE upper('%" + val + "%')";
        cur = empresaBase() ->cargacursor ( SQLQuery );
        if ( cur->numregistros() == 1 ) {
            mdb_idarticulo = cur->valor ( "idarticulo" );
            mdb_nomarticulo = cur->valor ( "nomarticulo" );
      	    mdb_codigocompletoarticulo = cur->valor("codigocompletoarticulo");
            encontrado = TRUE;
        } // end if

	if (cur->numregistros() > 1) {
		buscarmas = FALSE;
	} // end if
        delete cur;
    } // end if


    if ( ! encontrado && buscarmas) {
        QString SQLQuery = "SELECT * FROM articulo WHERE upper(nomarticulo) LIKE upper('%" + val + "%')";
        cur = empresaBase() ->cargacursor ( SQLQuery );
        if ( cur->numregistros() == 1 ) {
            mdb_idarticulo = cur->valor ( "idarticulo" );
            mdb_nomarticulo = cur->valor ( "nomarticulo" );
      	    mdb_codigocompletoarticulo = cur->valor("codigocompletoarticulo");
            encontrado = TRUE;
        } // end if

        delete cur;
    } // end if

    if ( !encontrado ) {
        m_nomarticulo->setText ( "" );
        mdb_idarticulo = "";
        mdb_nomarticulo = "";
        mdb_codigocompletoarticulo = "";
    } // end if

    if ( encontrado ) {
	m_codigocompletoarticulo->setText ( mdb_codigocompletoarticulo );
	m_nomarticulo->setText ( mdb_nomarticulo );
	emit ( valueChanged ( mdb_idarticulo ) );
    } // end if



/// Hasta aqui
/*
    mdb_codigocompletoarticulo = val;
    QString SQLQuery = "SELECT * FROM articulo WHERE codigocompletoarticulo='" + mdb_codigocompletoarticulo + "'";
    cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );
    if ( !cur->eof() ) {
        mdb_idarticulo = cur->valor ( "idarticulo" );
        mdb_nomarticulo = cur->valor ( "nomarticulo" );
    } else {
        mdb_idarticulo = "";
        mdb_nomarticulo = "";
    } // end if
    delete cur;
    m_codigocompletoarticulo->setText ( mdb_codigocompletoarticulo );
    m_nomarticulo->setText ( mdb_nomarticulo );
    emit ( valueChanged ( mdb_idarticulo ) );
*/
    _depura ( "END BusquedaArticulo::on_m_codigocompletoarticulo_textChanged", 0 );
}


///
/**
\return
**/
QString BusquedaArticulo::codigocompletoarticulo()
{
    _depura ( "BusquedaArticulo::codigocompletoarticulo", 0 );
    _depura ( "END BusquedaArticulo::codigocompletoarticulo", 0 );
    return m_codigocompletoarticulo->text();
}


///
/**
\return
**/
QString BusquedaArticulo::idarticulo()
{
    _depura ( "BusquedaArticulo::idarticulo", 0 );
    _depura ( "END BusquedaArticulo::idarticulo", 0 );
    return mdb_idarticulo;
}


///
/**
\return
**/
QString BusquedaArticulo::valorCampo()
{
    _depura ( "BusquedaArticulo::valorCampo", 0 );
    _depura ( "END BusquedaArticulo::valorCampo", 0 );
    return mdb_idarticulo;
}


///
/**
\return
**/
QString BusquedaArticulo::nomarticulo()
{
    _depura ( "BusquedaArticulo::nomarticulo", 0 );
    _depura ( "END BusquedaArticulo::nomarticulo", 0 );
    return m_nomarticulo->text();
}


/// ===================================================================
/// Busqueda Articulo Delegate para usar con los subforms
/// ===================================================================
/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el SIGNAL activated() con m_activated() para tratarlo.
*/
/**
\param parent
**/
BusquedaArticuloDelegate::BusquedaArticuloDelegate ( QWidget *parent )
        : QComboBox2 ( parent )
{
    _depura ( "BusquedaArticuloDelegate::BusquedaArticuloDelegate", 0 );
    m_cursorcombo = NULL;
    setEditable ( true );
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    connect ( this, SIGNAL ( editTextChanged ( const QString & ) ), this, SLOT ( s_editTextChanged ( const QString & ) ) );
    _depura ( "END BusquedaArticuloDelegate::BusquedaArticuloDelegate", 0 );
}


/** Libera la memoria reservada. */
/**
**/
BusquedaArticuloDelegate::~BusquedaArticuloDelegate()
{
    _depura ( "BusquedaArticuloDelegate::~BusquedaArticuloDelegate", 0 );
    _depura ( "END BusquedaArticuloDelegate::~BusquedaArticuloDelegate", 0 );
}


/** Permite indicar al Widget cual es la serie de factura seleccionada por defecto.
    Recarga cursor de serie_factura y cuando encuentra un registro cuyo codigoserie_factura coincide con el pasado
    como parametro lo establece como el registro activo por el comboBox.
*/
/**
\param cod
\return
**/
void BusquedaArticuloDelegate::s_editTextChanged ( const QString &cod )
{
    _depura ( "BusquedaArticuloDelegate::s_editTextChanged", 0 );
    static bool semaforo = FALSE;
    QString codigo = cod;

    if ( codigo.size() < 3 ) {
        return;
    } // end if

    if ( semaforo ) {
        return;
    } else  {
        semaforo = TRUE;
    } // end if

    codigo = codigo.left ( codigo.indexOf ( ".-" ) );

    m_cursorcombo = empresaBase() ->cargacursor ( "SELECT codigocompletoarticulo, nomarticulo FROM articulo WHERE codigocompletoarticulo LIKE '" + codigo + "%' ORDER BY codigocompletoarticulo LIMIT 25" );
    clear();
    while ( !m_cursorcombo->eof() ) {
        addItem ( m_cursorcombo->valor ( "codigocompletoarticulo" ) + ".-" + m_cursorcombo->valor ( "nomarticulo" ) );
        m_cursorcombo->siguienteregistro();
    } // end while
    delete m_cursorcombo;
    setEditText ( cod );
    //    showPopup();
    semaforo = FALSE;
    _depura ( "END BusquedaArticuloDelegate::s_editTextChanged", 0 );
}

