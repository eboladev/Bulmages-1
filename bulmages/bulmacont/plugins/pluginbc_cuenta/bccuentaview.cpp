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

#include "bccuentaview.h"
#include "bccompany.h"


///
/**
\param emp
\param parent
\param fl
**/
BcCuentaView::BcCuentaView ( BcCompany  *emp, QWidget *parent, Qt::WFlags fl )
        : BcForm ( emp, parent, fl )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );

    setTitleName ( _ ( "Cuenta" ) );
    /// Establecemos cual es la tabla en la que basarse para el tema de los permisos
    setDbTableName ( "cuenta" );
    setDbFieldId ( "idcuenta" );

    addDbField ( "idcuenta", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID cuenta" ) );
    addDbField ( "codigo", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Codigo cuenta" ) );
    addDbField ( "descripcion", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Descripcion" ) );
    addDbField ( "padre", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Padre" ) );
    addDbField ( "imputacion", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Imputacion" ) );
    addDbField ( "bloqueada", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Bloqueada" ) );
    addDbField ( "nodebe", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "NoDebe" ) );
    addDbField ( "nohaber", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "NoHaber" ) );
    addDbField ( "regularizacion", BlDbField::DbBoolean , BlDbField::DbNothing, _ ( "Regularizacion" ) );
    addDbField ( "tipocuenta", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Tipo de Cuenta" ) );
    addDbField ( "debe", BlDbField::DbNumeric, BlDbField::DbNoSave, _ ( "Debe" ) );
    addDbField ( "haber", BlDbField::DbNumeric, BlDbField::DbNoSave, _ ( "Haber" ) );
    addDbField ( "idc_coste", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Centro de coste por defecto" ) );
    addDbField ( "nombreent_cuenta", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Entidad de la cuenta" ) );
    addDbField ( "cifent_cuenta", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "CIF de la cuenta" ) );
    addDbField ( "cpent_cuenta", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "CP de la cuenta" ) );
    addDbField ( "telent_cuenta", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Telefono de la cuenta" ) );
    addDbField ( "coment_cuenta", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentarios de la cuenta" ) );
    addDbField ( "bancoent_cuenta", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Banco de la cuenta" ) );
    addDbField ( "webent_cuenta", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Web de la cuenta" ) );
    addDbField ( "emailent_cuenta", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "E-Mail de la cuenta" ) );
    addDbField ( "dirent_cuenta", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Direccion de la cuenta" ) );
    addDbField ( "idprovincia", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Provincia" ) );
    addDbField ( "poblacion", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Poblacion" ) );

    mui_padre->setMainCompany ( mainCompany() );
    mui_idc_coste->setMainCompany ( mainCompany() );

    /// Establecemos los valores para el menu radio tipo_cuenta
    mui_tipocuenta_1->setFieldValue ( "1" );
    mui_tipocuenta_2->setFieldValue ( "2" );
    mui_tipocuenta_3->setFieldValue ( "3" );
    mui_tipocuenta_4->setFieldValue ( "4" );
    mui_tipocuenta_5->setFieldValue ( "5" );
    mui_tipocuenta_0->setFieldValue ( "0" );

    mui_idprovincia->setMainCompany ( mainCompany() );
    mui_idprovincia->setQuery ( "SELECT * FROM provincia LEFT JOIN pais ON provincia.idpais = pais.idpais ORDER BY descpais, provincia" );
    mui_idprovincia->setTableName ( "provincia" );
    mui_idprovincia->setFieldId ( "idprovincia" );
    mui_idprovincia->m_valores["provincia"] = "";
    mui_idprovincia->m_valores["descpais"] = "";
    mui_idprovincia->setAllowNull(TRUE);
    mui_idprovincia->setId("");

    /// Arreglamos la cuenta
    mui_padre->setMainCompany(mainCompany());
    mui_padre->setLabel ( _ ( "Cuenta Padre:" ) );
    mui_padre->setTableName ( "cuenta" );
    mui_padre->setFieldId("idcuenta");
    mui_padre->m_valores["descripcion"] = "";
    mui_padre->m_valores["codigo"] = "";

    dialogChanges_readValues();
    mainCompany() ->insertWindow ( windowTitle(), this );
    /// Llamamos a los scripts
    blScript(this);

    
}


///
/**
**/
BcCuentaView::~BcCuentaView()
{
    BL_FUNC_DEBUG
}


///
/**
\param codpadre
\return
**/
int BcCuentaView::nuevacuenta ( QString codpadre )
{
    BL_FUNC_DEBUG

    /// Suponiendo que las cuentas son num&eacute;ricas, al crear una nueva cuenta
    /// buscamos entre las que ser&aacute;n sus hermanas y le asignamos el n&uacute;mero
    /// siguiente que le corresponda.
    QString cpadreaux;
    QString query;
    query = "SELECT * FROM cuenta WHERE padre = id_cuenta('" + codpadre + "') ORDER BY codigo DESC";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
    if ( !cur->eof() ) {
        long int valor = cur->value( "codigo" ).toLong();
        valor ++;
        cpadreaux.setNum ( valor );
        mui_codigo->setText ( cpadreaux );

        /// Vamos a hacer la carga del tipocuenta.
        QString query2 = "SELECT * FROM cuenta WHERE codigo = '" + codpadre + "'";
	BlDbRecordSet *cur2 = mainCompany() ->loadQuery ( query2 );

        int tipocuenta = cur2->value( "tipocuenta" ).toInt();
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
        while ( codint.length() < ( ( BcCompany * ) mainCompany() ) ->numDigitosEmpresa() - 1 ) {
            codint = codint + "0";
        } // end while
        codint = codint + "0";
        mui_codigo->setText ( codint );
    }
    delete cur;
    /// Establecemos el valor del padre y del grupo.
//    mui_padre->setcodigocuenta ( codpadre );
    mui_padre->setText ( codpadre );
    
    return 0;
}



