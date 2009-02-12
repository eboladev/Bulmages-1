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

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginclienteprov", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    return 0;
}


///
/**
\param l
\return
**/
int BlForm_guardar_Post ( BlForm *l )
{
    _depura ( "BlForm_guardar_Post", 0 );
	bool guardar = FALSE;
	if (l->tableName() == "cliente") {

	FichaBf *fich = new FichaBf((Company *)l->empresaBase(), l);
        fich->setDBTableName ( "proveedor" );
        fich->setDBCampoId ( "idproveedor" );
        fich->addDBCampo ( "idproveedor", BlDbField::DBint, BlDbField::DBPrimaryKey, _("ID proveedor") );
        fich->addDBCampo ( "nomproveedor", BlDbField::DBvarchar, BlDbField::DBNotNull, _("Nombre del proveedor")  );
        fich->addDBCampo ( "nomaltproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _("Nombre alternativo del proveedor") );
        fich->addDBCampo ( "cifproveedor", BlDbField::DBvarchar, BlDbField::DBNotNull, _("C.I.F. del proveedor") );
        fich->addDBCampo ( "codicliproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _("Codigo cliente") );
        fich->addDBCampo ( "cbancproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _("Banco proveedor") );
        fich->addDBCampo ( "dirproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _("Direccion") );
        fich->addDBCampo ( "poblproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _("Poblacion") );
        fich->addDBCampo ( "idprovincia", BlDbField::DBint, BlDbField::DBNothing, _("Provincia") );
        fich->addDBCampo ( "cpproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _("Codigo postal") );
        fich->addDBCampo ( "telproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _("Numero de telefono") );
        fich->addDBCampo ( "faxproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _("Numero de fax") );
        fich->addDBCampo ( "emailproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _("Direccion electronica") );
        fich->addDBCampo ( "urlproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _("URL") );
        fich->addDBCampo ( "comentproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _("Comentarios") );
        fich->addDBCampo ( "codproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _("Codigo") );
        fich->addDBCampo ( "regimenfiscalproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Regimen fiscal" ) );
        fich->addDBCampo ( "idforma_pago", BlDbField::DBint, BlDbField::DBNothing, _( "Id forma de pago" ) );
        fich->addDBCampo ( "recargoeqproveedor", BlDbField::DBboolean, BlDbField::DBNothing, _( "Recargo de equivalencia" ) );
        fich->addDBCampo ( "irpfproveedor", BlDbField::DBnumeric, BlDbField::DBNothing, _( "IRPF" ) );

	QString query = "SELECT * FROM proveedor WHERE cifproveedor = '" + l->DBvalue("cifcliente")+"'";
	cursor2 *cur = l->empresaBase()->cargacursor(query);
	if (!cur->eof()) {
		fich->cargar(cur->valor("idproveedor"));
		guardar = TRUE;
	} else {
		if (QMessageBox::question(l,"Crear proveedor", "Desea crear un proveedor con los datos del cliente", QMessageBox::Yes | QMessageBox::No)== QMessageBox::Yes) {
			guardar = TRUE;
		} // end if
	} // end if
	delete cur;

	fich->setDBvalue("codicliprovedor", l->DBvalue("codcliente"));
	fich->setDBvalue("nomproveedor", l->DBvalue("nomcliente"));
	fich->setDBvalue("cifproveedor", l->DBvalue("cifcliente"));
	fich->setDBvalue("cpproveedor", l->DBvalue("cpcliente"));
	fich->setDBvalue("dirproveedor", l->DBvalue("dircliente"));
	fich->setDBvalue("poblproveedor", l->DBvalue("poblcliente"));
	fich->setDBvalue("idprovincia", l->DBvalue("idprovincia"));
	fich->setDBvalue("cpproveedor", l->DBvalue("cpcliente"));
	fich->setDBvalue("telproveedor", l->DBvalue("telcliente"));

	if (guardar)
		fich->guardar();
	delete fich;
	} // end if
    _depura ( "END BlForm_guardar_Post", 0 );
    return 0;
}

