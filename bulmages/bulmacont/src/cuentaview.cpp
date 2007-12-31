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
#include "empresa.h"


///
/**
\param emp
\param parent
\param fl
**/
CuentaView::CuentaView ( Empresa  *emp, QWidget *parent, Qt::WFlags fl )
        : FichaBc ( emp, parent, fl )
{
    _depura ( "CuentaView::CuentaView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );

    setTitleName ( tr ( "Cuenta" ) );
    /// Establecemos cual es la tabla en la que basarse para el tema de los permisos
    setDBTableName ( "cuenta" );
    setDBCampoId ( "idcuenta" );

    addDBCampo ( "idcuenta", DBCampo::DBint, DBCampo::DBPrimaryKey, tr ( "ID cuenta" ) );
    addDBCampo ( "codigo", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Codigo cuenta" ) );
    addDBCampo ( "descripcion", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Descripcion" ) );
    addDBCampo ( "padre", DBCampo::DBboolean  , DBCampo::DBNothing, tr ( "Padre" ) );
    addDBCampo ( "imputacion", DBCampo::DBboolean , DBCampo::DBNothing, tr ( "Imputacion" ) );
    addDBCampo ( "bloqueada", DBCampo::DBboolean , DBCampo::DBNothing, tr ( "Bloqueada" ) );
    addDBCampo ( "nodebe", DBCampo::DBboolean , DBCampo::DBNothing, tr ( "NoDebe" ) );
    addDBCampo ( "nohaber", DBCampo::DBboolean , DBCampo::DBNothing, tr ( "NoHaber" ) );
    addDBCampo ( "regularizacion", DBCampo::DBboolean , DBCampo::DBNothing, tr ( "Regularizacion" ) );
    addDBCampo ( "tipocuenta", DBCampo::DBint , DBCampo::DBNothing, tr ( "Tipo de Cuenta" ) );
    addDBCampo ( "debe", DBCampo::DBnumeric , DBCampo::DBNoSave, tr ( "Debe" ) );
    addDBCampo ( "haber", DBCampo::DBnumeric , DBCampo::DBNoSave, tr ( "Haber" ) );
    addDBCampo ( "idc_coste", DBCampo::DBint, DBCampo::DBNothing, tr ( "Centro de coste por defecto" ) );
    addDBCampo ( "nombreent_cuenta", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Entidad de la cuenta" ) );
    addDBCampo ( "cifent_cuenta", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "CIF de la cuenta" ) );
    addDBCampo ( "cpent_cuenta", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "CP de la cuenta" ) );
    addDBCampo ( "telent_cuenta", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Telefono de la cuenta" ) );
    addDBCampo ( "coment_cuenta", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Comentarios de la cuenta" ) );
    addDBCampo ( "bancoent_cuenta", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Banco de la cuenta" ) );
    addDBCampo ( "webent_cuenta", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Web de la cuenta" ) );
    addDBCampo ( "emailent_cuenta", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "E-Mail de la cuenta" ) );
    addDBCampo ( "dirent_cuenta", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Direccion de la cuenta" ) );

    mui_padre->setEmpresaBase ( empresaBase() );
    mui_idc_coste->setEmpresaBase ( empresaBase() );

    /// Establecemos los valores para el menu radio tipo_cuenta
    mui_tipocuenta_1->setValorCampo ( "1" );
    mui_tipocuenta_2->setValorCampo ( "2" );
    mui_tipocuenta_3->setValorCampo ( "3" );
    mui_tipocuenta_4->setValorCampo ( "4" );
    mui_tipocuenta_5->setValorCampo ( "5" );
    mui_tipocuenta_0->setValorCampo ( "0" );

    mui_idprovincia->setEmpresaBase ( empresaBase() );
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
    cursor2 *cur = empresaBase() ->cargacursor ( query );
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
        while ( codint.length() < ( ( Empresa * ) empresaBase() ) ->numdigitosempresa() - 1 ) {
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



