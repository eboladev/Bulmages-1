/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include <QtWidgets/QToolButton>

#include "pluginbf_clienteproveedor.h"
#include "bfform.h"
#include "blfunctions.h"


///
/**
\return
**/
int entryPoint ( BfBulmaFact * )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_clienteproveedor", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    return 0;
}


///
/**
\param l
\return
**/
int BlForm_guardar_Post ( BlForm *l )
{
    BL_FUNC_DEBUG
    bool guardar = false;
    if ( l->tableName() == "cliente" ) {

        BfForm *fich = new BfForm ( ( BfCompany * ) l->mainCompany(), l );
        fich->setDbTableName ( "proveedor" );
        fich->setDbFieldId ( "idproveedor" );
        fich->addDbField ( "idproveedor", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID proveedor" ) );
        fich->addDbField ( "nomproveedor", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Nombre del proveedor" )  );
        fich->addDbField ( "nomaltproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre alternativo del proveedor" ) );
        fich->addDbField ( "cifproveedor", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "C.I.F. del proveedor" ) );
        fich->addDbField ( "codicliproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Codigo cliente" ) );
        fich->addDbField ( "cbancproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Banco proveedor" ) );
        fich->addDbField ( "dirproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Direccion" ) );
        fich->addDbField ( "poblproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Poblacion" ) );
        fich->addDbField ( "idprovincia", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Provincia" ) );
        fich->addDbField ( "cpproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Codigo postal" ) );
        fich->addDbField ( "telproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de telefono" ) );
        fich->addDbField ( "faxproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de fax" ) );
        fich->addDbField ( "emailproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Direccion electronica" ) );
        fich->addDbField ( "urlproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "URL" ) );
        fich->addDbField ( "comentproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentarios" ) );
        fich->addDbField ( "codproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Codigo" ) );
        fich->addDbField ( "regimenfiscalproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Regimen fiscal" ) );
        fich->addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Id forma de pago" ) );
        fich->addDbField ( "recargoeqproveedor", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Recargo de equivalencia" ) );
        fich->addDbField ( "irpfproveedor", BlDbField::DbNumeric, BlDbField::DbNothing, _ ( "IRPF" ) );

        QString query = "SELECT * FROM proveedor WHERE cifproveedor = '" + l->dbValue ( "cifcliente" ) + "'";
        BlDbRecordSet *cur = l->mainCompany()->loadQuery ( query );
        if ( !cur->eof() ) {
            fich->load ( cur->value( "idproveedor" ) );
            guardar = true;
        } else {
            if ( QMessageBox::question ( l, "Crear proveedor", "Desea crear un proveedor con los datos del cliente", QMessageBox::Yes | QMessageBox::No ) == QMessageBox::Yes ) {
                guardar = true;
            } // end if
        } // end if
        delete cur;

        fich->setDbValue ( "codicliprovedor", l->dbValue ( "codcliente" ) );
        fich->setDbValue ( "nomproveedor", l->dbValue ( "nomcliente" ) );
        fich->setDbValue ( "cifproveedor", l->dbValue ( "cifcliente" ) );
        fich->setDbValue ( "cpproveedor", l->dbValue ( "cpcliente" ) );
        fich->setDbValue ( "dirproveedor", l->dbValue ( "dircliente" ) );
        fich->setDbValue ( "poblproveedor", l->dbValue ( "poblcliente" ) );
        fich->setDbValue ( "idprovincia", l->dbValue ( "idprovincia" ) );
        fich->setDbValue ( "cpproveedor", l->dbValue ( "cpcliente" ) );
        fich->setDbValue ( "telproveedor", l->dbValue ( "telcliente" ) );

        if ( guardar )
            fich->save();
        delete fich;
    } // end if

    return 0;
}

