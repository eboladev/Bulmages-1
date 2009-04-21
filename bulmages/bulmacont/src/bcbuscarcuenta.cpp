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

#include  "bcbuscarcuenta.h"
#include  "bcplancontablelistview.h"
#include  "bccompany.h"
#include  "blplugins.h"
#include  "bccuentalistview.h"


///
/**
\param parent
**/
BcBuscarCuenta::BcBuscarCuenta ( QWidget *parent )
        : BlWidget ( parent )
{
    _depura ( "BcBuscarCuenta::BcBuscarCuenta", 0 );
    setupUi ( this );
    mdb_idcuenta = "";
    mdb_nomcuenta = "";
    mdb_codigocuenta = "";
    mdb_tipocuenta = "";
    m_semaforo = FALSE;
    _depura ( "END BcBuscarCuenta::BcBuscarCuenta", 0 );
}


///
/**
**/
BcBuscarCuenta::~BcBuscarCuenta()
{
    _depura ( "BcBuscarCuenta::~BcBuscarCuenta", 0 );
    _depura ( "END BcBuscarCuenta::~BcBuscarCuenta", 0 );
}


///
/**
\return
**/
QString BcBuscarCuenta::text()
{
    _depura ( "BcBuscarCuenta::text" );
    _depura ( "END BcBuscarCuenta::text" );
    return mdb_codigocuenta;
}


///
/**
\return
**/
QString BcBuscarCuenta::fieldValue()
{
    _depura ( "BcBuscarCuenta::fieldValue()" );
    _depura ( "END BcBuscarCuenta::fieldValue()" );
    return mdb_idcuenta;
}


///
/**
\param val
**/
void BcBuscarCuenta::setText ( QString val )
{
    _depura ( "BcBuscarCuenta::setText", 0, val );
    setcodigocuenta ( val );
    _depura ( "END BcBuscarCuenta::setText" );
}

///
/**
\param val
**/
void BcBuscarCuenta::setFieldValue ( QString val )
{
    _depura ( "BcBuscarCuenta::setFieldValue", 0, val );
    _depura ( "END BcBuscarCuenta::setFieldValue" );
    setidcuenta ( val );
}


///
/**
\return
**/
QString BcBuscarCuenta::codigocuenta()
{
    _depura ( "BcBuscarCuenta::codigocuenta" );
    _depura ( "END BcBuscarCuenta::codigocuenta" );
    return mui_codigocuenta->text();
}


///
/**
\return
**/
QString BcBuscarCuenta::idcuenta()
{
    _depura ( "BcBuscarCuenta::idcuenta" );
    _depura ( "END BcBuscarCuenta::idcuenta" );
    return mdb_idcuenta;
}


///
/**
\return
**/
QString BcBuscarCuenta::nomcuenta()
{
    _depura ( "BcBuscarCuenta::nomcuenta" );
    _depura ( "END BcBuscarCuenta::nomcuenta()" );
    return mdb_nomcuenta;
}


///
/**
\return
**/
QString BcBuscarCuenta::tipocuenta()
{
    _depura ( "BcBuscarCuenta::tipocuenta" );
    _depura ( "END BcBuscarCuenta::tipocuenta" );
    return mdb_tipocuenta;
}


///
/**
**/
void BcBuscarCuenta::hideNombre()
{
    _depura ( "BcBuscarCuenta::hideNombre" );
    _depura ( "END BcBuscarCuenta::hideNombre" );
    mui_nomcuenta->hide();
}


///
/**
**/
void BcBuscarCuenta::showNombre()
{
    _depura ( "BcBuscarCuenta::showNombre" );
    mui_nomcuenta->show();
    _depura ( "END BcBuscarCuenta::showNombre" );
}


///
/**
**/
void BcBuscarCuenta::on_mui_buscar_clicked()
{
    _depura ( "BcBuscarCuenta::on_mui_buscar_clicked" );
    s_searchCuenta();
    _depura ( "END BcBuscarCuenta::on_mui_buscar_clicked" );
}


///
/**
\param q
**/
void BcBuscarCuenta::on_mui_codigocuenta_textChanged ( const QString &q )
{
    _depura ( "BcBuscarCuenta::on_mui_codigocuenta_textChanged", 0, q );
    s_codigocuentatextChanged ( q );
    _depura ( "END BcBuscarCuenta::on_mui_codigocuenta_textChanged" );
}


///
/**
**/
void BcBuscarCuenta::selectAll()
{
    _depura ( "BcBuscarCuenta::selectAll" );
    mui_codigocuenta->selectAll();
    _depura ( "END BcBuscarCuenta::selectAll" );
}


///
/**
**/
void BcBuscarCuenta::setFocus()
{
    _depura ( "BcBuscarCuenta::setFocus" );
    mui_codigocuenta->setFocus();
    _depura ( "END BcBuscarCuenta::setFocus" );
}


///
/**
\param emp
**/
void BcBuscarCuenta::setMainCompany ( BcCompany *emp )
{
    _depura ( "BcBuscarCuenta::setMainCompany", 0 );
    BlWidget::setMainCompany ( emp );
    m_numdigitos = ( ( BcCompany * ) mainCompany() ) ->numdigitosempresa();
    _depura ( "END BcBuscarCuenta::setMainCompany", 0 );
}


///
/**
\param val
**/
void BcBuscarCuenta::setidcuenta ( QString val )
{
    _depura ( "BcBuscarCuenta::setidcuenta", 10 );
    mdb_idcuenta = val;
    if ( val == "" ) return;

    QString SQLQuery = "SELECT * FROM cuenta WHERE idcuenta = '" + mdb_idcuenta + "'";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery );
    if ( !cur->eof() ) {
        mdb_codigocuenta = cur->valor ( "codigo" );
        mdb_nomcuenta = cur->valor ( "descripcion" );
        mdb_tipocuenta = cur->valor ( "tipocuenta" );
    } else {
        mdb_idcuenta = "";
        mdb_nomcuenta = "";
        mdb_codigocuenta = "";
        mdb_tipocuenta = "";
    } // end if
    delete cur;
    mui_codigocuenta->setText ( mdb_codigocuenta );
    mui_nomcuenta->setText ( mdb_nomcuenta );
    _depura ( "END BcBuscarCuenta::setidcuenta", 0 );
}


///
/**
\param val
**/
void BcBuscarCuenta::setcodigocuenta ( QString val )
{
    _depura ( "BcBuscarCuenta::setcodigocuenta", 0 );
    mdb_codigocuenta = val;
    if ( val == "" ) return;
    QString SQLQuery = "SELECT * FROM cuenta WHERE codigo = '" + mdb_codigocuenta + "'";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery );
    if ( !cur->eof() ) {
        mdb_idcuenta = cur->valor ( "idcuenta" );
        mdb_nomcuenta = cur->valor ( "descripcion" );
        mdb_tipocuenta = cur->valor ( "tipocuenta" );
    } else {
        mdb_idcuenta = "";
        mdb_nomcuenta = "";
        mdb_tipocuenta = "";
    } // end if
    delete cur;
    mui_codigocuenta->setText ( mdb_codigocuenta );
    mui_nomcuenta->setText ( mdb_nomcuenta );
    _depura ( "END BcBuscarCuenta::setcodigocuenta", 0 );
}


/// B&uacute;squeda de cuentas.
/**
**/
void BcBuscarCuenta::s_searchCuenta()
{
    _depura ( "BcBuscarCuenta::s_searchCuenta", 10 );
    /// Generamos un di&aacute;logo.
    QDialog *diag = new QDialog ( 0 );
    diag->setModal ( true );

    /// Creamos una instancia del selector de cuentas.
    BcCuentaListView *listcuentas = new BcCuentaListView ( ( BcCompany * ) mainCompany(), diag, 0, BL_SELECT_MODE );

    /// Hacemos la conexi&oacute;n del cerrar de las cuentas con el cerrar di&aacute;logo.
    connect ( listcuentas, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );
    connect ( listcuentas, SIGNAL ( destroyed ( QObject ) ), diag, SLOT ( accept() ) );

    /// Sacamos el di&aacute;logo.
    diag->exec();

    if ( listcuentas->codigocuenta() != "" ) {
        mdb_idcuenta = listcuentas->idcuenta();
        mdb_codigocuenta = listcuentas->codigocuenta();
        mdb_nomcuenta = listcuentas->nombrecuenta();
        mui_codigocuenta->setText ( mdb_codigocuenta );
        mui_nomcuenta->setText ( mdb_nomcuenta );
    } // end if
    delete diag;

    _depura ( "END BcBuscarCuenta::s_searchCuenta", 0 );
}


/** Se encarga de presentar en el Widget los valores seleccionados.
*/
/**
**/
void BcBuscarCuenta::pinta()
{
    _depura ( "BcBuscarCuenta::pinta", 0 );
    m_semaforo = TRUE;
    mui_codigocuenta->setText ( mdb_codigocuenta );
    mui_nomcuenta->setText ( mdb_nomcuenta );
    m_semaforo = FALSE;
    emit ( valueChanged ( mdb_idcuenta ) );
    _depura ( "END BcBuscarCuenta::pinta", 0 );
}


/** Permite indicar al Widget cual es la cuenta seleccionada por defecto.
    Recarga cursor de serie_factura y cuando encuentra un registro cuyo codigo coincide con el pasado
    como parametro lo establece como el registro activo por el comboBox.
*/
/**
\param cod
\return
**/
void BcBuscarCuenta::s_codigocuentatextChanged ( const QString &val )
{
    _depura ( "BcBuscarCuenta::s_codigocuentatextChanged", 0, val );

    bool encontrado = FALSE;

    if ( m_semaforo ) {
        _depura ( "END BcBuscarCuenta::s_codigocuentatextChanged", 0, "Semaforo parado" );
        return;
    } // end if

    /// Comprobamos si se ha pulsado el * y entonces hacemos la busqueda
    if ( val == "*" ) {
        on_mui_buscar_clicked();
        _depura ( "END BcBuscarCuenta::s_codigocuentatextChanged", 0 );
        return;
    } // end if


    QString SQLQuery = "SELECT * FROM cuenta WHERE codigo = '" + extiendecodigo ( val, m_numdigitos ) + "'";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery );
    if ( !cur->eof() ) {
        mdb_idcuenta = cur->valor ( "idcuenta" );
        mdb_nomcuenta = cur->valor ( "descripcion" );
        mdb_codigocuenta = cur->valor ( "codigo" );
        mdb_tipocuenta = cur->valor ( "tipocuenta" );
        encontrado = TRUE;
    }
    delete cur;


    if ( ! encontrado ) {
        SQLQuery = "SELECT * FROM cuenta WHERE upper(descripcion) = upper('" + val + "')";
        cur = mainCompany() ->loadQuery ( SQLQuery );
        if ( cur->numregistros() == 1 ) {
            mdb_idcuenta = cur->valor ( "idcuenta" );
            mdb_nomcuenta = cur->valor ( "descripcion" );
            mdb_codigocuenta = cur->valor ( "codigo" );
            mdb_tipocuenta = cur->valor ( "tipocuenta" );
            encontrado = TRUE;
        } // end if
        delete cur;
    } // end if

    if ( ! encontrado ) {
        SQLQuery = "SELECT * FROM cuenta WHERE upper(descripcion) LIKE upper('%" + val + "%')";
        cur = mainCompany() ->loadQuery ( SQLQuery );
        if ( cur->numregistros() == 1 ) {
            mdb_idcuenta = cur->valor ( "idcuenta" );
            mdb_nomcuenta = cur->valor ( "descripcion" );
            mdb_codigocuenta = cur->valor ( "codigo" );
            mdb_tipocuenta = cur->valor ( "tipocuenta" );
            encontrado = TRUE;
        } // end if
        delete cur;
    } // end if



    if ( !encontrado ) {
        mui_nomcuenta->setText ( "" );
        mdb_idcuenta = "";
        mdb_nomcuenta = "";
        mdb_codigocuenta = "";
        mdb_tipocuenta = "";
        g_plugins->lanza ( "BcBuscarCuenta_textChanged_Post", this );
    } // end if

    if ( encontrado ) {
        mui_nomcuenta->setText ( mdb_nomcuenta );
        emit ( valueChanged ( mdb_idcuenta ) );

    } // end if

    _depura ( "END BcBuscarCuenta::s_codigocuentatextChanged", 0 );
}


/** SLOT que responde al fin de la edicion del campos de texto.
    Se supone que hemos introducido un cliente identificable por codigo, por nombre o por CIF
    Asi que se encarga de buscarlo y encontrarlo.
*/
/**
**/
void BcBuscarCuenta::on_mui_codigocuenta_editingFinished()
{
    _depura ( "BcBuscarCuenta::on_mui_codigocuenta_editingFinished", 0 );
    pinta();
    g_plugins->lanza ( "BcBuscarCuenta_on_mui_codigocuenta_editingFinished_Post", this );
    _depura ( "END BcBuscarCuenta::on_mui_codigocuenta_editingFinished", 0 );
}


/// ===================================================================
/// Busqueda Cuenta Delegate para usar con los subforms
/// ===================================================================
/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el SIGNAL activated() con m_activated() para tratarlo.
*/
/**
\param parent
**/
BcBuscarCuentaDelegate::BcBuscarCuentaDelegate ( QWidget *parent )
        : BlComboBox ( parent )
{
    _depura ( "BcBuscarCuentaDelegate::BcBuscarCuentaDelegate", 0 );
    m_cursorcombo = NULL;
    setEditable ( true );
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    connect ( this, SIGNAL ( editTextChanged ( const QString & ) ), this, SLOT ( s_editTextChanged ( const QString & ) ) );
    _depura ( "END BcBuscarCuentaDelegate::BcBuscarCuentaDelegate", 0 );
}


/// Libera la memoria reservada.
/**
**/
BcBuscarCuentaDelegate::~BcBuscarCuentaDelegate()
{
    _depura ( "BcBuscarCuentaDelegate::~BcBuscarCuentaDelegate", 0 );
    _depura ( "END BcBuscarCuentaDelegate::~BcBuscarCuentaDelegate", 0 );
}


/** Permite indicar al Widget cual es la serie de factura seleccionada por defecto.
    Recarga cursor de serie_factura y cuando encuentra un registro cuyo codigoserie_factura coincide con el pasado
    como parametro lo establece como el registro activo por el comboBox.
*/
/**
\param cod
\return
**/
void BcBuscarCuentaDelegate::s_editTextChanged ( const QString &cod )
{
    _depura ( "BcBuscarCuentaDelegate::s_editTextChanged", 0 );
    static bool semaforo = FALSE;
    QString codigo = cod;

    if ( codigo.size() < 3 ) {
        /// Si hay menos de 3 caracteres en el QComboBox no tiene que aparecer el autocompletar.
        _depura ( "END BcBuscarCuentaDelegate::s_editTextChanged", 0, "Menos de tres caracteres" );
        return;
    } // end if

    if ( codigo.size() > 3 ) {
        _depura ( "END BcBuscarCuentaDelegate::s_editTextChanged", 0, "Codigo mayor de 3 caracteres" );
        return;
    } // end if

    if ( semaforo ) {
        _depura ( "END BcBuscarCuentaDelegate::s_editTextChanged", 0, "Semaforo" );
        return;
    } else {
        semaforo = TRUE;
    } // end if

    codigo = codigo.left ( codigo.indexOf ( ".-" ) );
    m_cursorcombo = mainCompany() ->loadQuery ( "SELECT codigo, descripcion FROM cuenta WHERE codigo LIKE '" + codigo + "%' ORDER BY codigo LIMIT 25" );
    clear();

    ///TODO: La idea es que salga en el desplegable del combobox el listado de cuentas que
    /// coincidan con el texto escrito para poder elegirlo.
    while ( !m_cursorcombo->eof() ) {
        addItem ( m_cursorcombo->valor ( "codigo" ) + ".-" + m_cursorcombo->valor ( "descripcion" ) );
        m_cursorcombo->nextRecord();
    } // end while
    delete m_cursorcombo;

    setEditText ( cod );

    semaforo = FALSE;
    _depura ( "END BcBuscarCuentaDelegate::s_editTextChanged", 0 );
}




