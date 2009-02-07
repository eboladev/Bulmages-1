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

#include "busquedafamilia.h"
#include "familiasview.h"
#include "company.h"


/** Se encarga de poner todos los componentes a NULL para que no exista confusion
    en si los componentes han sido inicializados o no.
*/
/**
\param parent
**/
BusquedaFamilia::BusquedaFamilia ( QWidget *parent )
        : BlWidget ( parent )
{
    _depura ( "BusquedaFamilia::BusquedaFamilia", 0 );
    setupUi ( this );
    mdb_idfamilia = "";
    mdb_nombrefamilia = "";
    mdb_codigocompletofamilia = "";
    _depura ( "END BusquedaFamilia::BusquedaFamilia", 0 );
}


/** No requiere de acciones especiales en la destruccion del Widget
*/
/**
**/
BusquedaFamilia::~BusquedaFamilia()
{
    _depura ( "BusquedaFamilia::~BusquedaFamilia", 0 );
    _depura ( "END BusquedaFamilia::~BusquedaFamilia", 0 );
}


///
/**
\param val
**/
void BusquedaFamilia::setidfamilia ( QString val )
{
    _depura ( "BusquedaFamilia::setidfamilia", 0 );
    mdb_idfamilia = val;
    QString SQLQuery = "SELECT * FROM familia WHERE idfamilia='" + mdb_idfamilia + "'";
    cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );

    if ( !cur->eof() ) {
        mdb_codigocompletofamilia = cur->valor ( "codigocompletofamilia" );
        mdb_nombrefamilia = cur->valor ( "nombrefamilia" );
    } else {
        mdb_idfamilia = "";
        mdb_nombrefamilia = "";
        mdb_codigocompletofamilia = "";
    } // end if

    delete cur;
    m_codigocompletofamilia->setText ( mdb_codigocompletofamilia );
    m_nombrefamilia->setText ( mdb_nombrefamilia );
    _depura ( "END BusquedaFamilia::setidfamilia", 0 );
}


///
/**
\param val
**/
void BusquedaFamilia::setValorCampo ( QString val )
{
    _depura ( "BusquedaFamilia::setValorCampo", 0 );
    setidfamilia ( val );
    _depura ( "END BusquedaFamilia::setValorCampo", 0 );
}


///
/**
\param val
**/
void BusquedaFamilia::setcodigocompletofamilia ( QString val )
{
    _depura ( "BusquedaFamilia::setcodigocompletofamilia", 0 );
    mdb_codigocompletofamilia = val;
    QString SQLQuery = "SELECT * FROM familia WHERE codigocompletofamilia='" + mdb_codigocompletofamilia + "'";
    cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );

    if ( !cur->eof() ) {
        mdb_idfamilia = cur->valor ( "idfamilia" );
        mdb_nombrefamilia = cur->valor ( "nombrefamilia" );
    } else {
        mdb_idfamilia = "";
        mdb_nombrefamilia = "";
    } // end if

    delete cur;
    m_codigocompletofamilia->setText ( mdb_codigocompletofamilia );
    m_nombrefamilia->setText ( mdb_nombrefamilia );
    _depura ( "END BusquedaFamilia::setcodigocompletofamilia", 0 );
}


/// Busqueda de familias.
/**
**/
void BusquedaFamilia::on_mui_buscar_clicked()
{
    _depura ( "BusquedaFamilia::on_mui_buscar_clicked", 0 );
    QDialog *diag = new QDialog ( 0 );
    diag->setModal ( true );

    FamiliasView *fam = ( ( Company * ) empresaBase() ) ->newfamiliasview ( diag, TRUE );
    connect ( fam, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

    /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
    /// para que sea redimensionable y aparezca el titulo de la ventana.
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget ( fam );
    layout->setMargin ( 0 );
    layout->setSpacing ( 0 );
    diag->setLayout ( layout );
    diag->setWindowTitle ( fam->windowTitle() );

    diag->exec();
    if ( fam->codigoCompletoFamilia() != "" ) {
        m_codigocompletofamilia->setText ( fam->codigoCompletoFamilia() );
        mdb_codigocompletofamilia = fam->codigoCompletoFamilia();
        m_nombrefamilia->setText ( fam->nombreFamilia() );
        mdb_nombrefamilia = fam->nombreFamilia();
        mdb_idfamilia = fam->idFamilia();
    } // end if
    delete diag;
    emit ( valueChanged ( mdb_idfamilia ) );
    _depura ( "END BusquedaFamilia::on_mui_buscar_clicked", 0 );
}

///
/**
\param val
**/
void BusquedaFamilia::on_m_codigocompletofamilia_textChanged ( const QString &val )
{
    _depura ( "BusquedaFamilia::on_m_codigocompletofamilia_textChanged", 0 );
    mdb_codigocompletofamilia = val;
    QString SQLQuery = "SELECT * FROM familia WHERE codigocompletofamilia='" + mdb_codigocompletofamilia + "'";
    cursor2 *cur = empresaBase() ->cargacursor ( SQLQuery );

    if ( !cur->eof() ) {
        mdb_idfamilia = cur->valor ( "idfamilia" );
        mdb_nombrefamilia = cur->valor ( "nombrefamilia" );
    } else {
        mdb_idfamilia = "";
        mdb_nombrefamilia = "";
    } // end if

    delete cur;
    m_codigocompletofamilia->setText ( mdb_codigocompletofamilia );
    m_nombrefamilia->setText ( mdb_nombrefamilia );
    emit ( valueChanged ( mdb_idfamilia ) );
    _depura ( "END BusquedaFamilia::on_m_codigocompletofamilia_textChanged", 0 );
}


///
/**
\return
**/
QString BusquedaFamilia::codigocompletofamilia()
{
    _depura ( "BusquedaFamilia::codigocompletofamilia", 0 );
    _depura ( "END BusquedaFamilia::codigocompletofamilia", 0 );
    return m_codigocompletofamilia->text();
}


///
/**
\return
**/
QString BusquedaFamilia::idfamilia()
{
    _depura ( "BusquedaFamilia::idfamilia", 0 );
    _depura ( "END BusquedaFamilia::idfamilia", 0 );
    return mdb_idfamilia;
}


///
/**
\return
**/
QString BusquedaFamilia::valorCampo()
{
    _depura ( "BusquedaFamilia::valorCampo", 0 );
    _depura ( "END BusquedaFamilia::valorCampo", 0 );
    return mdb_idfamilia;
}


///
/**
\returns
**/
QString BusquedaFamilia::nombrefamilia()
{
    _depura ( "BusquedaFamilia::nombrefamilia", 0 );
    _depura ( "END BusquedaFamilia::nombrefamilia", 0 );
    return nombrefamilia();
}

