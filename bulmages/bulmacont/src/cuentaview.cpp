/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
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

#include "cuentaview.h"
#include "bccompany.h"


///
/**
\param emp
\param parent
\param fl
**/
CuentaView::CuentaView ( BcCompany  *emp, QWidget *parent, Qt::WFlags fl )
        : FichaBc ( emp, parent, fl )
{
    _depura ( "CuentaView::CuentaView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );

    setTitleName ( _( "Cuenta" ) );
    /// Establecemos cual es la tabla en la que basarse para el tema de los permisos
    setDBTableName ( "cuenta" );
    setDBCampoId ( "idcuenta" );

    addDBCampo ( "idcuenta", BlDbField::DBint, BlDbField::DBPrimaryKey, _( "ID cuenta" ) );
    addDBCampo ( "codigo", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Codigo cuenta" ) );
    addDBCampo ( "descripcion", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Descripcion" ) );
    addDBCampo ( "padre", BlDbField::DBint  , BlDbField::DBNothing, _( "Padre" ) );
    addDBCampo ( "imputacion", BlDbField::DBboolean , BlDbField::DBNothing, _( "Imputacion" ) );
    addDBCampo ( "bloqueada", BlDbField::DBboolean , BlDbField::DBNothing, _( "Bloqueada" ) );
    addDBCampo ( "nodebe", BlDbField::DBboolean , BlDbField::DBNothing, _( "NoDebe" ) );
    addDBCampo ( "nohaber", BlDbField::DBboolean , BlDbField::DBNothing, _( "NoHaber" ) );
    addDBCampo ( "regularizacion", BlDbField::DBboolean , BlDbField::DBNothing, _( "Regularizacion" ) );
    addDBCampo ( "tipocuenta", BlDbField::DBint , BlDbField::DBNothing, _( "Tipo de Cuenta" ) );
    addDBCampo ( "debe", BlDbField::DBnumeric , BlDbField::DBNoSave, _( "Debe" ) );
    addDBCampo ( "haber", BlDbField::DBnumeric , BlDbField::DBNoSave, _( "Haber" ) );
    addDBCampo ( "idc_coste", BlDbField::DBint, BlDbField::DBNothing, _( "Centro de coste por defecto" ) );
    addDBCampo ( "nombreent_cuenta", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Entidad de la cuenta" ) );
    addDBCampo ( "cifent_cuenta", BlDbField::DBvarchar, BlDbField::DBNothing, _( "CIF de la cuenta" ) );
    addDBCampo ( "cpent_cuenta", BlDbField::DBvarchar, BlDbField::DBNothing, _( "CP de la cuenta" ) );
    addDBCampo ( "telent_cuenta", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Telefono de la cuenta" ) );
    addDBCampo ( "coment_cuenta", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Comentarios de la cuenta" ) );
    addDBCampo ( "bancoent_cuenta", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Banco de la cuenta" ) );
    addDBCampo ( "webent_cuenta", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Web de la cuenta" ) );
    addDBCampo ( "emailent_cuenta", BlDbField::DBvarchar, BlDbField::DBNothing, _( "E-Mail de la cuenta" ) );
    addDBCampo ( "dirent_cuenta", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Direccion de la cuenta" ) );

    mui_padre->setMainCompany ( empresaBase() );
    mui_idc_coste->setMainCompany ( empresaBase() );

    /// Establecemos los valores para el menu radio tipo_cuenta
    mui_tipocuenta_1->setValorCampo ( "1" );
    mui_tipocuenta_2->setValorCampo ( "2" );
    mui_tipocuenta_3->setValorCampo ( "3" );
    mui_tipocuenta_4->setValorCampo ( "4" );
    mui_tipocuenta_5->setValorCampo ( "5" );
    mui_tipocuenta_0->setValorCampo ( "0" );

    mui_idprovincia->setMainCompany ( empresaBase() );
    mui_idprovincia->setQuery("SELECT * FROM provincia LEFT JOIN pais ON provincia.idpais = pais.idpais ORDER BY descpais, provincia");
    mui_idprovincia->setTableName ("provincia");
    mui_idprovincia->setCampoId ("idprovincia");
    mui_idprovincia->m_valores["nomprovincia"] = "";
    mui_idprovincia->m_valores["descpais"] = "";


    dialogChanges_cargaInicial();
    empresaBase() ->meteWindow ( windowTitle(), this );
    _depura ( "END CuentaView::CuentaView", 0 );
}


///
/**
**/
CuentaView::~CuentaView()
{
    _depura ( "CuentaView::~CuentaView", 0 );
    _depura ( "END CuentaView::~CuentaView", 0 );
}


///
/**
\param codpadre
\return
**/
int CuentaView::nuevacuenta ( QString codpadre )
{
    _depura ( "CuentaView::nuevacuenta", 0 );

    /// Suponiendo que las cuentas son num&eacute;ricas, al crear una nueva cuenta
    /// buscamos entre las que ser&aacute;n sus hermanas y le asignamos el n&uacute;mero
    /// siguiente que le corresponda.
    QString cpadreaux;
    QString query;
    query = "SELECT * FROM cuenta WHERE padre = id_cuenta('" + codpadre + "') ORDER BY codigo DESC";
    BlDbRecordSet *cur = empresaBase() ->cargacursor ( query );
    if ( !cur->eof() ) {
        long int valor = cur->valor ( "codigo" ).toLong();
        valor ++;
        cpadreaux.setNum ( valor );
        mui_codigo->setText ( cpadreaux );
        /// Vamos a hacer la carga del tipocuenta.
        int tipocuenta = cur->valor ( "tipocuenta" ).toInt();
        switch ( tipocuenta ) {
        case 0:
            mui_tipocuenta_0->setChecked ( TRUE );
            break;
        case 1:
            mui_tipocuenta_1->setChecked ( TRUE );
            break;
        case 2:
            mui_tipocuenta_2->setChecked ( TRUE );
            break;
        case 3:
            mui_tipocuenta_3->setChecked ( TRUE );
            break;
        case 4:
            mui_tipocuenta_4->setChecked ( TRUE );
            break;
        case 5:
            mui_tipocuenta_5->setChecked ( TRUE );
            break;
        } // end switch
    } else {
        QString codint = codpadre;
        while ( codint.length() < ( ( BcCompany * ) empresaBase() ) ->numdigitosempresa() - 1 ) {
            codint = codint + "0";
        } // end while
        codint = codint + "0";
        mui_codigo->setText ( codint );
    }
    delete cur;
    /// Establecemos el valor del padre y del grupo.
    mui_padre->setcodigocuenta ( codpadre );
    _depura ( "END CuentaView::nuevacuenta", 0 );
    return 0;
}



