/***************************************************************************
 *   Copyright (C) 2010 by Fco. Javier M. C.                               *
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

#include <QLineEdit>
#include <QWidget>

#include "pluginbf_sincrobulmacont.h"
#include "blfunctions.h"
#include "bfcompany.h"
#include "familiacuenta.h"



///
/**
\return
**/
int entryPoint ( BfBulmaFact *bf )
{
    _depura ( "Estoy dentro del plugin SincroBulmacont", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_sincrobulmacont", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    return 0;
}


FamiliaCuenta *famscuenta = NULL;


int FamiliasView_FamiliasView ( FamiliasView *famv )
{

    FamiliaCuenta *famscuenta = new FamiliaCuenta(famv);

    return 0;
}



int FamiliasView_Guardar_Pre ( FamiliasView *famv )
{
    BlDbRecordSet *rec_venta;
    BlDbRecordSet *tmp_venta;
    BlDbRecordSet *rec_compra;
    BlDbRecordSet *tmp_compra;
    QString query;
    
  
    try {
      
      famv->mainCompany()->run("SELECT conectabulmacont()");
      
      /// Hace la comprobacion de los datos introducidos y que son validos en la contabilidad.
      /// La cuenta que se especifique tiene que existir en la contabilidad. No se crean de forma
      /// automatica.
      QString cuentaventa = famv->mainCompany()->sanearCadena(famv->findChild<QLineEdit *>("mui_cuenta_venta")->text());

      if (cuentaventa.isEmpty()) {
	/// En el caso de dejar vacio el campo de cuenta preferente
	/// se mira si antes se hizo utilizo la cuenta preferente
	/// y restaura la cuenta original.
	query = "SELECT origenidcuentaventafamilia FROM familia WHERE idfamilia = " + famv->idFamilia();
	tmp_venta = famv->mainCompany()->loadQuery(query);
	if (!tmp_venta->valor("origenidcuentaventafamilia").isEmpty()) {
	    /// Restaura cuenta original
	    query = "UPDATE familia SET idcuentaventafamilia = " + tmp_venta->valor("origenidcuentaventafamilia") + " WHERE idfamilia = " + famv->idFamilia();
	    famv->mainCompany()->runQuery(query);
	    
	    /// Vacia campo origen.
	    query = "UPDATE familia SET origenidcuentaventafamilia = NULL WHERE idfamilia = " + famv->idFamilia();
	    famv->mainCompany()->runQuery(query);
	} // end if
	
      } else {
	QString query_venta = "SELECT idcuenta FROM bc_cuenta WHERE codigo = '" + cuentaventa + "' LIMIT 1";
	rec_venta = famv->mainCompany()->loadQuery(query_venta);
	
	if (rec_venta->numregistros() <= 0) {
	  throw -300;
	} // end if
	
	/// Almacena la informacion de la cuenta antes de sobreescribirla.
	/// Verifica primero que el campo este vacio.
	query = "SELECT origenidcuentaventafamilia FROM familia WHERE idfamilia = " + famv->idFamilia();
	tmp_venta = famv->mainCompany()->loadQuery(query);
	
	if (tmp_venta->valor("origenidcuentaventafamilia").isEmpty()) {
	  query = "SELECT idcuentaventafamilia FROM familia WHERE idfamilia = " + famv->idFamilia();
	  tmp_venta = famv->mainCompany()->loadQuery(query);
	  query = "UPDATE familia SET origenidcuentaventafamilia = " + tmp_venta->valor("idcuentaventafamilia") + " WHERE idfamilia = " + famv->idFamilia();
	  famv->mainCompany()->runQuery(query);
	} // end if

	query = "UPDATE familia SET idcuentaventafamilia = " + rec_venta->valor("idcuenta") + " WHERE idfamilia = " + famv->idFamilia();
	famv->mainCompany()->runQuery(query);
	
      }// end if

      famv->mainCompany()->run("SELECT conectabulmacont()");

      QString cuentacompra = famv->mainCompany()->sanearCadena(famv->findChild<QLineEdit *>("mui_cuenta_compra")->text());

      if (cuentacompra.isEmpty()) {
	/// En el caso de dejar vacio el campo de cuenta preferente
	/// se mira si antes se hizo utilizo la cuenta preferente
	/// y restaura la cuenta original.
	query = "SELECT origenidcuentacomprafamilia FROM familia WHERE idfamilia = " + famv->idFamilia();
	tmp_compra = famv->mainCompany()->loadQuery(query);
	if (!tmp_compra->valor("origenidcuentacomprafamilia").isEmpty()) {
	    /// Restaura cuenta original
	    query = "UPDATE familia SET idcuentacomprafamilia = " + tmp_compra->valor("origenidcuentacomprafamilia") + " WHERE idfamilia = " + famv->idFamilia();
	    famv->mainCompany()->runQuery(query);
	    
	    /// Vacia campo origen.
	    query = "UPDATE familia SET origenidcuentacomprafamilia = NULL WHERE idfamilia = " + famv->idFamilia();
	    famv->mainCompany()->runQuery(query);
	} // end if

      } else {

	QString query_compra = "SELECT idcuenta FROM bc_cuenta WHERE codigo = '" + cuentacompra + "' LIMIT 1";
	rec_compra = famv->mainCompany()->loadQuery(query_compra);

	if (rec_compra->numregistros() <= 0) {
	  throw -400;
	} // end if

	/// Almacena la informacion de la cuenta antes de sobreescribirla.
	/// Verifica primero que el campo este vacio.
	query = "SELECT origenidcuentacomprafamilia FROM familia WHERE idfamilia = " + famv->idFamilia();
	tmp_compra = famv->mainCompany()->loadQuery(query);
	if (tmp_compra->valor("origenidcuentacomprafamilia").isEmpty()) {
	  query = "SELECT idcuentacomprafamilia FROM familia WHERE idfamilia = " + famv->idFamilia();
	  BlDbRecordSet *tmp_compra = famv->mainCompany()->loadQuery(query);
	  query = "UPDATE familia SET origenidcuentacomprafamilia = " + tmp_compra->valor("idcuentacomprafamilia") + " WHERE idfamilia = " + famv->idFamilia();
	  famv->mainCompany()->runQuery(query);	
	} // end if

	query = "UPDATE familia SET idcuentacomprafamilia = " + rec_compra->valor("idcuenta") + " WHERE idfamilia = " + famv->idFamilia();
	famv->mainCompany()->runQuery(query);
	
     } // end if
    } catch (int e) {
	if (e == -300) {
	  mensajeError("Cuenta de venta no existe en la contabilidad.");
	} else if (e == -400) {
	  mensajeError("Cuenta de compra no existe en la contabilidad.");
	} // end if
	throw -1;
    } // end try

    return 0;
}


int FamiliasView_Guardar_Post ( FamiliasView *famv )
{
    /// Guarda los datos en la base de datos.
    QString query = "UPDATE familia SET prefcuentaventafamilia = '" + famv->mainCompany()->sanearCadena(famv->findChild<QLineEdit *>("mui_cuenta_venta")->text()) + "',  prefcuentacomprafamilia = '" + famv->mainCompany()->sanearCadena(famv->findChild<QLineEdit *>("mui_cuenta_compra")->text()) + "' WHERE idfamilia = '" + famv->idFamilia() + "'";
   
    famv->mainCompany()->runQuery(query);

    return 0;
}


