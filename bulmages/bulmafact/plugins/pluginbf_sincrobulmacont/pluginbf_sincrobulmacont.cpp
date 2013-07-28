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

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

#include "pluginbf_sincrobulmacont.h"
#include "blfunctions.h"
#include "bfcompany.h"
#include "familiacuenta.h"
#include "fpagocuenta.h"
#include "bancocuenta.h"
#include "proveedorcuenta.h"
#include "clientecuenta.h"



///
/**
\return
**/
int entryPoint ( BfBulmaFact *bf )
{
    BL_FUNC_DEBUG

    /// El plugin necesita un parche en la base de datos para funcionar.
    bf->company()->dbPatchVersionCheck("PluginBf_SincroBulmaCont", "0.9.1-003");
    
    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_sincrobulmacont", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

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
	if (!tmp_venta->value("origenidcuentaventafamilia").isEmpty()) {
	    /// Restaura cuenta original
	    query = "UPDATE familia SET idcuentaventafamilia = " + tmp_venta->value("origenidcuentaventafamilia") + " WHERE idfamilia = " + famv->idFamilia();
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
	
	if (tmp_venta->value("origenidcuentaventafamilia").isEmpty()) {
	  query = "SELECT idcuentaventafamilia FROM familia WHERE idfamilia = " + famv->idFamilia();
	  tmp_venta = famv->mainCompany()->loadQuery(query);
	  query = "UPDATE familia SET origenidcuentaventafamilia = " + tmp_venta->value("idcuentaventafamilia") + " WHERE idfamilia = " + famv->idFamilia();
	  famv->mainCompany()->runQuery(query);
	} // end if

	query = "UPDATE familia SET idcuentaventafamilia = " + rec_venta->value("idcuenta") + " WHERE idfamilia = " + famv->idFamilia();
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
	if (!tmp_compra->value("origenidcuentacomprafamilia").isEmpty()) {
	    /// Restaura cuenta original
	    query = "UPDATE familia SET idcuentacomprafamilia = " + tmp_compra->value("origenidcuentacomprafamilia") + " WHERE idfamilia = " + famv->idFamilia();
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
	if (tmp_compra->value("origenidcuentacomprafamilia").isEmpty()) {
	  query = "SELECT idcuentacomprafamilia FROM familia WHERE idfamilia = " + famv->idFamilia();
	  BlDbRecordSet *tmp_compra = famv->mainCompany()->loadQuery(query);
	  query = "UPDATE familia SET origenidcuentacomprafamilia = " + tmp_compra->value("idcuentacomprafamilia") + " WHERE idfamilia = " + famv->idFamilia();
	  famv->mainCompany()->runQuery(query);	
	} // end if

	query = "UPDATE familia SET idcuentacomprafamilia = " + rec_compra->value("idcuenta") + " WHERE idfamilia = " + famv->idFamilia();
	famv->mainCompany()->runQuery(query);
	
     } // end if
    } catch (int e) {
	if (e == -300) {
	  blMsgError("Cuenta de venta no existe en la contabilidad.");
	} else if (e == -400) {
	  blMsgError("Cuenta de compra no existe en la contabilidad.");
	} // end if
	throw -1;
    } // end try

    return 0;
}


int FamiliasView_Guardar_Post ( FamiliasView *famv )
{
    return 0;
}


/* FORMAS DE PAGO */
/********************************************/

FPagoCuenta *fpagocuenta = NULL;


int FPagoView_FPagoView ( FPagoView *fpago )
{

    FPagoCuenta *fpagocuenta = new FPagoCuenta(fpago);

    return 0;
}


int FPagoView_Guardar_Pre ( FPagoView *fpagov )
{
    BlDbRecordSet *rec_forma_pago;
    BlDbRecordSet *tmp_forma_pago;
    QString query;
  
    try {
      
      fpagov->mainCompany()->run("SELECT conectabulmacont()");
      
      /// Hace la comprobacion de los datos introducidos y que son validos en la contabilidad.
      /// La cuenta que se especifique tiene que existir en la contabilidad. No se crean de forma
      /// automatica.
      QString cuentaforma_pago = fpagov->mainCompany()->sanearCadena(fpagov->findChild<QLineEdit *>("mui_cuenta_forma_pago")->text());

      if (cuentaforma_pago.isEmpty()) {
	/// En el caso de dejar vacio el campo de cuenta preferente
	/// se mira si antes se hizo utilizo la cuenta preferente
	/// y restaura la cuenta original.
	query = "SELECT origenidcuentaforma_pago FROM forma_pago WHERE idforma_pago = " + fpagov->idFormaPago();
	tmp_forma_pago = fpagov->mainCompany()->loadQuery(query);
	if (!tmp_forma_pago->value("origenidcuentaforma_pago").isEmpty()) {
	    /// Restaura cuenta original
	    query = "UPDATE forma_pago SET idcuentaforma_pago = " + tmp_forma_pago->value("origenidcuentaforma_pago") + " WHERE idforma_pago = " + fpagov->idFormaPago();
	    fpagov->mainCompany()->runQuery(query);
	    
	    /// Vacia campo origen.
	    query = "UPDATE forma_pago SET origenidcuentaforma_pago = NULL WHERE idforma_pago = " + fpagov->idFormaPago();
	    fpagov->mainCompany()->runQuery(query);
	} // end if
	
      } else {
	QString query_forma_pago = "SELECT idcuenta FROM bc_cuenta WHERE codigo = '" + cuentaforma_pago + "' LIMIT 1";
	rec_forma_pago = fpagov->mainCompany()->loadQuery(query_forma_pago);
	
	if (rec_forma_pago->numregistros() <= 0) {
	  throw -300;
	} // end if
	
	/// Almacena la informacion de la cuenta antes de sobreescribirla.
	/// Verifica primero que el campo este vacio.
	query = "SELECT origenidcuentaforma_pago FROM forma_pago WHERE idforma_pago = " + fpagov->idFormaPago();
	tmp_forma_pago = fpagov->mainCompany()->loadQuery(query);
	
	if (tmp_forma_pago->value("origenidcuentaforma_pago").isEmpty()) {
	  query = "SELECT idcuentaforma_pago FROM forma_pago WHERE idforma_pago = " + fpagov->idFormaPago();
	  tmp_forma_pago = fpagov->mainCompany()->loadQuery(query);
	  query = "UPDATE forma_pago SET origenidcuentaforma_pago = " + tmp_forma_pago->value("idcuentaforma_pago") + " WHERE idforma_pago = " + fpagov->idFormaPago();
	  fpagov->mainCompany()->runQuery(query);
	} // end if

	query = "UPDATE forma_pago SET idcuentaforma_pago = " + rec_forma_pago->value("idcuenta") + " WHERE idforma_pago = " + fpagov->idFormaPago();
	fpagov->mainCompany()->runQuery(query);
	
      }// end if

    } catch (int e) {
	if (e == -300) {
	  blMsgError("La cuenta no existe en la contabilidad.");
	} // end if
	throw -1;
    } // end try

    return 0;
}


int FPagoView_Guardar_Post ( FPagoView *fpagov )
{
    return 0;
}






/* BANCOS */
/********************************************/

BancoCuenta *bancocuenta = NULL;

int BancoView_BancoView ( BancoView *banco )
{

    BancoCuenta *bancocuenta = new BancoCuenta(banco);

    return 0;
}


int BancoView_Guardar_Pre ( BancoView *bancov )
{
    BlDbRecordSet *rec_banco;
    BlDbRecordSet *tmp_banco;
    QString query;
    
    try {

      bancov->mainCompany()->run("SELECT conectabulmacont()");
      
      /// Hace la comprobacion de los datos introducidos y que son validos en la contabilidad.
      /// La cuenta que se especifique tiene que existir en la contabilidad. No se crean de forma
      /// automatica.
      QString cuentabanco = bancov->mainCompany()->sanearCadena(bancov->findChild<QLineEdit *>("mui_cuenta_banco")->text());
    
      if (cuentabanco.isEmpty()) {
	/// En el caso de dejar vacio el campo de cuenta preferente
	/// se mira si antes se hizo utilizo la cuenta preferente
	/// y restaura la cuenta original.
	query = "SELECT origenidcuentabanco FROM banco WHERE idbanco = " + bancov->idBanco();
	tmp_banco = bancov->mainCompany()->loadQuery(query);

	if (!tmp_banco->value("origenidcuentabanco").isEmpty()) {
	    /// Restaura cuenta original
	    query = "UPDATE banco SET idcuentabanco = " + tmp_banco->value("origenidcuentabanco") + " WHERE idbanco = " + bancov->idBanco();
	    bancov->mainCompany()->runQuery(query);
	    
	    /// Vacia campo origen.
	    query = "UPDATE banco SET origenidcuentabanco = NULL WHERE idbanco = " + bancov->idBanco();
	    bancov->mainCompany()->runQuery(query);
	} // end if
	
      } else {
	QString query_banco = "SELECT idcuenta FROM bc_cuenta WHERE codigo = '" + cuentabanco + "' LIMIT 1";
	rec_banco = bancov->mainCompany()->loadQuery(query_banco);
	
	if (rec_banco->numregistros() <= 0) {
	  throw -300;
	} // end if
	
	/// Almacena la informacion de la cuenta antes de sobreescribirla.
	/// Verifica primero que el campo este vacio.
	query = "SELECT origenidcuentabanco FROM banco WHERE idbanco = " + bancov->idBanco();
	tmp_banco = bancov->mainCompany()->loadQuery(query);
	
	if (tmp_banco->value("origenidcuentabanco").isEmpty()) {
	  //query = "SELECT idcuentabanco FROM banco WHERE idbanco = " + bancov->idBanco();
	  query = "SELECT idcuentabanco FROM banco WHERE idbanco = " + bancov->idBanco();
	  tmp_banco = bancov->mainCompany()->loadQuery(query);
	  query = "UPDATE banco SET origenidcuentabanco = " + tmp_banco->value("idcuentabanco") + " WHERE idbanco = " + bancov->idBanco();
	  bancov->mainCompany()->runQuery(query);
	} // end if

	query = "UPDATE banco SET idcuentabanco = " + rec_banco->value("idcuenta") + " WHERE idbanco = " + bancov->idBanco();
	bancov->mainCompany()->runQuery(query);
	
      }// end if


    } catch (int e) {
	if (e == -300) {
	  blMsgError(_("La cuenta no existe en la contabilidad."));
	} // end if
	throw -1;
    } // end try

    return 0;
}


int BancoView_Guardar_Post ( BancoView *bancov )
{
    return 0;
}



/* PROVEEDOR */
/********************************************/
int ProveedorView_ProveedorView_Post ( ProveedorView *proveedor )
{
  
    ProveedorCuenta *proveedorcuenta = new ProveedorCuenta(proveedor);

    proveedor->addDbField ( "idcuentaproveedor", BlDbField::DbInt, BlDbField::DbNothing, _ ( "idcuentaproveedor" ) );
    
    return 0;
}


int ProveedorView_cargarPost_Post ( ProveedorView *proveedor )
{
    BlDbRecordSet *rec;

    proveedor->mainCompany()->begin();
  
    // 1) coge idcuentabanco de banco.
    // 2) se conecta a contabilidad.
    // 3) busca el codigo cuenta usando el idcuenta de banco.
    // 4) rellena qlineedit con codigo cuenta.
    
    QString query = "SELECT idcuentaproveedor FROM proveedor WHERE idproveedor = '" + proveedor->dbValue(proveedor->fieldId()) + "' LIMIT 1";
    
    
    rec = proveedor->mainCompany()->loadQuery(query);
    
    // Si no hay datos en idcuenta no se hace nada.
    if ( rec != NULL ) {

	proveedor->mainCompany()->run("SELECT conectabulmacont()");
	
	QString query = "SELECT codigo FROM bc_cuenta WHERE idcuenta = '" + rec->value("idcuentaproveedor") + "' LIMIT 1";
	rec = proveedor->mainCompany()->loadQuery(query);
	
	proveedor->findChild<QLineEdit *>("mui_cuenta_proveedor")->setText( rec->value("codigo") );
	
    } // end if

  
    proveedor->mainCompany()->commit();

    return 0;
}


int ProveedorView_Guardar_Pre ( ProveedorView *proveedor )
{
    BlDbRecordSet *rec;

    /// En la creacion del proveedor no existe ID.
    if ( !proveedor->dbValue ( proveedor->fieldId() ).isEmpty() ) {
    
      QString query = "SELECT idcuentaproveedor FROM proveedor WHERE idproveedor = '" + proveedor->dbValue(proveedor->fieldId()) + "' LIMIT 1";

      rec = proveedor->mainCompany()->loadQuery(query);

      if ( rec != NULL ) {
	  proveedor->setDbValue("idcuentaproveedor", proveedor->dbValue("idcuentaproveedor"));
      } // end if
      
      delete rec;

    } else {
      proveedor->setDbValue("idcuentaproveedor", "0");
    } // end if
  
    return 0;
}


int ProveedorView_Guardar_Post ( ProveedorView *proveedor )
{
    BlDbRecordSet *rec_proveedor;
    BlDbRecordSet *tmp_proveedor;
    BlDbRecordSet *rsa;
    QString query;

    try {

      proveedor->mainCompany()->run("SELECT conectabulmacont()");
      
      /// Hace la comprobacion de los datos introducidos y que son validos en la contabilidad.
      /// La cuenta que se especifique tiene que existir en la contabilidad. No se crean de forma
      /// automatica.
      QString cuentaproveedor = proveedor->mainCompany()->sanearCadena(proveedor->findChild<QLineEdit *>("mui_cuenta_proveedor")->text());
      
      if ( proveedor->dbValue(proveedor->fieldId()).isEmpty() ) return -1;

      
      if (cuentaproveedor.isEmpty()) {
	/// En el caso de dejar vacio el campo de cuenta preferente
	/// se mira si antes se hizo utilizo la cuenta preferente
	/// y restaura la cuenta original.
	query = "SELECT origenidcuentaproveedor FROM proveedor WHERE idproveedor = " + proveedor->dbValue(proveedor->fieldId());
	tmp_proveedor = proveedor->mainCompany()->loadQuery(query);

	if (!tmp_proveedor->value("origenidcuentaproveedor").isEmpty()) {
	    /// Restaura cuenta original
	    query = "UPDATE proveedor SET idcuentaproveedor = " + tmp_proveedor->value("origenidcuentaproveedor") + " WHERE idproveedor = " + proveedor->dbValue(proveedor->fieldId());
	    proveedor->mainCompany()->runQuery(query);
	    
	    /// Vacia campo origen.
	    query = "UPDATE proveedor SET origenidcuentaproveedor = NULL WHERE idproveedor = " + proveedor->dbValue(proveedor->fieldId());
	    proveedor->mainCompany()->runQuery(query);
	} // end if
	
      } else {
	/// Verifica que el idcuenta guardado y el codigo introducido coincidan
	/// si no coinciden entonces el codigo ha cambiado y se tiene que buscar
	/// a que idcuenta corresponde y hacer la actualizacion.
	
	query = "SELECT codigo FROM bc_cuenta WHERE idcuenta = '" + proveedor->dbValue("idcuentaproveedor") + "'";
	rsa = proveedor->mainCompany()->loadQuery(query);
	if (rsa != NULL) {
	  
	  if (rsa->value("codigo") != cuentaproveedor) {
	
	      QString query_proveedor = "SELECT idcuenta FROM bc_cuenta WHERE codigo = '" + cuentaproveedor + "' LIMIT 1";
	      rec_proveedor = proveedor->mainCompany()->loadQuery(query_proveedor);
	      
	      if (rec_proveedor->numregistros() <= 0) {
		throw -300;
	      } // end if
	      
	      /// Almacena la informacion de la cuenta antes de sobreescribirla.
	      /// Verifica primero que el campo este vacio.
	      query = "SELECT origenidcuentaproveedor FROM proveedor WHERE idproveedor = " + proveedor->dbValue(proveedor->fieldId());
	      tmp_proveedor = proveedor->mainCompany()->loadQuery(query);
	      
	      if (tmp_proveedor->value("origenidcuentaproveedor").isEmpty()) {
		query = "SELECT idcuentaproveedor FROM proveedor WHERE idproveedor = " + proveedor->dbValue(proveedor->fieldId());
		tmp_proveedor = proveedor->mainCompany()->loadQuery(query);
		query = "UPDATE proveedor SET origenidcuentaproveedor = " + tmp_proveedor->value("idcuentaproveedor") + " WHERE idproveedor = " + proveedor->dbValue(proveedor->fieldId());
		proveedor->mainCompany()->runQuery(query);
	      } // end if

	      query = "UPDATE proveedor SET idcuentaproveedor = " + rec_proveedor->value("idcuenta") + " WHERE idproveedor = " + proveedor->dbValue(proveedor->fieldId());
	      proveedor->mainCompany()->runQuery(query);
	      
	    } // end if

	  } // end if
	  
	} // end if

/*
      /// Traspasa datos para poder usarlos en Guardar_Post
      proveedor->findChild<QLineEdit *>("tmp_cuenta_proveedor")->setText( proveedor->mainCompany()->sanearCadena(proveedor->findChild<QLineEdit *>("mui_cuenta_proveedor")->text()) );
*/
    } catch (int e) {
	if (e == -300) {
	  blMsgError(_("La cuenta no existe en la contabilidad."));
	} // end if
	throw -1;
    } // end try

    return 0;
}



/* CLIENTE */
/********************************************/
int ClienteView_ClienteView_Post ( ClienteView *cliente )
{

  
    ClienteCuenta *clientecuenta = new ClienteCuenta(cliente);

    cliente->addDbField ( "idcuentacliente", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Id cuenta cliente" ) );
    
    return 0;
}


int ClienteView_cargarPost_Post ( ClienteView *cliente )
{
    BlDbRecordSet *rec;

    cliente->mainCompany()->begin();
  
    // 1) coge idcuentabanco de banco.
    // 2) se conecta a contabilidad.
    // 3) busca el codigo cuenta usando el idcuenta de banco.
    // 4) rellena qlineedit con codigo cuenta.
    
    QString query = "SELECT idcuentacliente FROM cliente WHERE idcliente = '" + cliente->dbValue(cliente->fieldId()) + "' LIMIT 1";
    
    
    rec = cliente->mainCompany()->loadQuery(query);
    
    // Si no hay datos en idcuenta no se hace nada.
    if ( rec != NULL ) {

	cliente->mainCompany()->run("SELECT conectabulmacont()");
	
	QString query = "SELECT codigo FROM bc_cuenta WHERE idcuenta = '" + rec->value("idcuentacliente") + "' LIMIT 1";
	rec = cliente->mainCompany()->loadQuery(query);
	
	cliente->findChild<QLineEdit *>("mui_cuenta_cliente")->setText( rec->value("codigo") );
	
    } // end if

  
    cliente->mainCompany()->commit();

    return 0;
}


int ClienteView_Guardar_Pre ( ClienteView *cliente )
{
    BlDbRecordSet *rec;

    /// En la creacion del cliente no existe ID.
    if ( !cliente->dbValue ( cliente->fieldId() ).isEmpty() ) {
    
      QString query = "SELECT idcuentacliente FROM cliente WHERE idcliente = '" + cliente->dbValue(cliente->fieldId()) + "' LIMIT 1";

      rec = cliente->mainCompany()->loadQuery(query);

      if ( rec != NULL ) {
	  cliente->setDbValue("idcuentacliente", cliente->dbValue("idcuentacliente"));
      } // end if
      
      delete rec;

    } else {
      cliente->setDbValue("idcuentacliente", "0");
    } // end if

    return 0;
}


int ClienteView_Guardar_Post ( ClienteView *cliente )
{
    BlDbRecordSet *rec_cliente;
    BlDbRecordSet *tmp_cliente;
    BlDbRecordSet *rsa;
    QString query;

    try {

      cliente->mainCompany()->run("SELECT conectabulmacont()");
      
      /// Hace la comprobacion de los datos introducidos y que son validos en la contabilidad.
      /// La cuenta que se especifique tiene que existir en la contabilidad. No se crean de forma
      /// automatica.
      QString cuentacliente = cliente->mainCompany()->sanearCadena(cliente->findChild<QLineEdit *>("mui_cuenta_cliente")->text());

      if ( cliente->dbValue(cliente->fieldId()).isEmpty() ) return -1;

      
      if (cuentacliente.isEmpty()) {

	/// En el caso de dejar vacio el campo de cuenta preferente
	/// se mira si antes se hizo utilizo la cuenta preferente
	/// y restaura la cuenta original.
	query = "SELECT origenidcuentacliente FROM cliente WHERE idcliente = " + cliente->dbValue(cliente->fieldId());
	tmp_cliente = cliente->mainCompany()->loadQuery(query);

	if (!tmp_cliente->value("origenidcuentacliente").isEmpty()) {
	    /// Restaura cuenta original
	    query = "UPDATE cliente SET idcuentacliente = " + tmp_cliente->value("origenidcuentacliente") + " WHERE idcliente = " + cliente->dbValue(cliente->fieldId());
	    cliente->mainCompany()->runQuery(query);
	    
	    /// Vacia campo origen.
	    query = "UPDATE cliente SET origenidcuentacliente = NULL WHERE idcliente = " + cliente->dbValue(cliente->fieldId());
	    cliente->mainCompany()->runQuery(query);
	} // end if
	
      } else {
	/// Verifica que el idcuenta guardado y el codigo introducido coincidan
	/// si no coinciden entonces el codigo ha cambiado y se tiene que buscar
	/// a que idcuenta corresponde y hacer la actualizacion.
	
	query = "SELECT codigo FROM bc_cuenta WHERE idcuenta = '" + cliente->dbValue("idcuentacliente") + "'";
	rsa = cliente->mainCompany()->loadQuery(query);
	if (rsa != NULL) {
	  
	  if (rsa->value("codigo") != cuentacliente) {
		  
	      QString query_cliente = "SELECT idcuenta FROM bc_cuenta WHERE codigo = '" + cuentacliente + "' LIMIT 1";
	      rec_cliente = cliente->mainCompany()->loadQuery(query_cliente);
	      
	      if (rec_cliente->numregistros() <= 0) {
		throw -300;
	      } // end if

	      /// Almacena la informacion de la cuenta antes de sobreescribirla.
	      /// Verifica primero que el campo este vacio.
	      query = "SELECT origenidcuentacliente FROM cliente WHERE idcliente = " + cliente->dbValue(cliente->fieldId());
	      tmp_cliente = cliente->mainCompany()->loadQuery(query);
	      
	      if (tmp_cliente->value("origenidcuentacliente").isEmpty()) {
		query = "SELECT idcuentacliente FROM cliente WHERE idcliente = " + cliente->dbValue(cliente->fieldId());
		tmp_cliente = cliente->mainCompany()->loadQuery(query);
		query = "UPDATE cliente SET origenidcuentacliente = " + tmp_cliente->value("idcuentacliente") + " WHERE idcliente = " + cliente->dbValue(cliente->fieldId());
		cliente->mainCompany()->runQuery(query);
	      } // end if

	      query = "UPDATE cliente SET idcuentacliente = " + rec_cliente->value("idcuenta") + " WHERE idcliente = " + cliente->dbValue(cliente->fieldId());
	      cliente->mainCompany()->runQuery(query);
	      
	    } // end if
	    
	  } // end if
	  
	} // end if
	
    } catch (int e) {
	if (e == -300) {
	  blMsgError(_("La cuenta no existe en la contabilidad."));
	} // end if
	throw -1;
    } // end try

    return 0;
}


