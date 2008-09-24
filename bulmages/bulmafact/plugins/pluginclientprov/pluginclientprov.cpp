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
#include <QToolButton>

#include "pluginclientprov.h"
#include "fichabf.h"
#include "funcaux.h"



///
/**
\return
**/
int entryPoint ( Bulmafact * )
{
    _depura ( "Estoy dentro del plugin de Cliente Proveedor", 0 );
    return 0;
}


///
/**
\param l
\return
**/
int Ficha_guardar_Post ( Ficha *l )
{
    _depura ( "Ficha_guardar_Post", 0 );
	if (l->tableName() == "cliente") {

	FichaBf *fich = new FichaBf((Company *)l->empresaBase(), l);
        fich->setDBTableName ( "proveedor" );
        fich->setDBCampoId ( "idproveedor" );
        fich->addDBCampo ( "idproveedor", DBCampo::DBint, DBCampo::DBPrimaryKey,  "ID proveedor" );
        fich->addDBCampo ( "nomproveedor", DBCampo::DBvarchar, DBCampo::DBNotNull,  "Nombre del proveedor"  );
        fich->addDBCampo ( "nomaltproveedor", DBCampo::DBvarchar, DBCampo::DBNothing,  "Nombre alternativo del proveedor" );
        fich->addDBCampo ( "cifproveedor", DBCampo::DBvarchar, DBCampo::DBNotNull,  "C.I.F. del proveedor" );
        fich->addDBCampo ( "codicliproveedor", DBCampo::DBvarchar, DBCampo::DBNothing,  "Codigo cliente" );
        fich->addDBCampo ( "cbancproveedor", DBCampo::DBvarchar, DBCampo::DBNothing,  "Banco proveedor" );
        fich->addDBCampo ( "dirproveedor", DBCampo::DBvarchar, DBCampo::DBNothing,  "Direccion" );
        fich->addDBCampo ( "poblproveedor", DBCampo::DBvarchar, DBCampo::DBNothing,  "Poblacion" );
        fich->addDBCampo ( "idprovincia", DBCampo::DBint, DBCampo::DBNothing,  "Provincia" );
        fich->addDBCampo ( "cpproveedor", DBCampo::DBvarchar, DBCampo::DBNothing,  "Codigo postal" );
        fich->addDBCampo ( "telproveedor", DBCampo::DBvarchar, DBCampo::DBNothing,  "Numero de telefono" );
        fich->addDBCampo ( "faxproveedor", DBCampo::DBvarchar, DBCampo::DBNothing,  "Numero de fax" );
        fich->addDBCampo ( "emailproveedor", DBCampo::DBvarchar, DBCampo::DBNothing,  "Direccion electronica" );
        fich->addDBCampo ( "urlproveedor", DBCampo::DBvarchar, DBCampo::DBNothing,  "URL" );
        fich->addDBCampo ( "comentproveedor", DBCampo::DBvarchar, DBCampo::DBNothing,  "Comentarios" );
        fich->addDBCampo ( "codproveedor", DBCampo::DBvarchar, DBCampo::DBNothing,  "Codigo" );
        fich->addDBCampo ( "regimenfiscalproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Proveedor", "Regimen Fiscal" ) );
        fich->addDBCampo ( "idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate ( "Proveedor", "Forma_Pago" ) );
        fich->addDBCampo ( "recargoeqproveedor", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate ( "Proveedor", "Recargo de Equivalencia" ) );
        fich->addDBCampo ( "irpfproveedor", DBCampo::DBnumeric, DBCampo::DBNothing, QApplication::translate ( "Proveedor", "IRPF" ) );

	QString query = "SELECT * FROM proveedor WHERE cifproveedor = '"+l->DBvalue("cif\q
cliente")+"'";
	cursor2 *cur = l->empresaBase()->cargacursor(query);
	if (!cur->eof()) {
		fich->cargar(cur->valor("idproveedor"));
	} // end if
	delete cur;

	fich->setDBvalue("codicliprovedor", l->DBvalue("codcliente"));
	fich->setDBvalue("nomproveedor", l->DBvalue("nomcliente"));
	fich->setDBvalue("cifproveedor", l->DBvalue("cifcliente"));
	fich->setDBvalue("cpproveedor", l->DBvalue("cpcliente"));


	fich->guardar();
	delete fich;
	} // end if
    _depura ( "END Ficha_guardar_Post", 0 );
    return 0;
}

