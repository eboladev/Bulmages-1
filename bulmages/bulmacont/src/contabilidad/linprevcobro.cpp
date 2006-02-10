//
// C++ Implementation: linprevcobro
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "linprevcobro.h"

#include "aplinteligentesview.h"
#include "asiento1view.h"


linprevcobro::linprevcobro(empresa *comp) {
    empresaactual = comp;
    conexionbase = comp->bdempresa();
    vacialinprevcobro();
}

linprevcobro::linprevcobro(empresa *comp, QString idprevcobro) {
    empresaactual = comp;
    conexionbase = comp->bdempresa();
    QString SQLQuery = "SELECT * FROM prevcobro "
	"LEFT JOIN  cuenta ON prevcobro.idcuenta = cuenta.idcuenta "
	"LEFT JOIN (SELECT codigo AS codigoctacliente, descripcion AS nomctacliente, idcuenta AS idctacliente FROM cuenta) AS T1 ON T1.idctacliente = prevcobro.idctacliente "
	"WHERE idprevcobro="+idprevcobro;
    cursor2 *cur = conexionbase->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_idprevcobro = cur->valor("idprevcobro");
        mdb_fprevistaprevcobro = cur->valor("fprevistaprevcobro");
        mdb_fcobroprevcobro = cur->valor("fcobroprevcobro");
        mdb_idfpago = cur->valor("idfpago");
        mdb_idcuenta = cur->valor("idcuenta");
        mdb_idasiento = cur->valor("idasiento");
        mdb_cantidadprevistaprevcobro = cur->valor("cantidadprevistaprevcobro");
        mdb_cantidadprevcobro = cur->valor("cantidadprevcobro");
        mdb_idregistroiva = cur->valor("idregistroiva");
        mdb_tipoprevcobro = cur->valor("tipoprevcobro");
        mdb_docprevcobro = cur->valor("docprevcobro");
        mdb_codigocuenta = cur->valor("codigo");
        mdb_nomcuenta = cur->valor("descripcion");

	mdb_idctacliente = cur->valor("idctacliente");
	mdb_codigoctacliente = cur->valor("codigoctacliente");
	mdb_nomctacliente = cur->valor("nomctacliente");
    } else {
        vacialinprevcobro();
    }// end if
}// end linprevcobro


linprevcobro::linprevcobro(empresa *comp, QString a, QString b, QString c, QString d, QString e, QString f, QString g, QString h, QString i, QString j, QString k, QString l, QString m, QString idctacliente, QString codigoctacliente, QString nomctacliente) {
    empresaactual = comp;
    conexionbase = comp->bdempresa();
    mdb_idprevcobro = a;
    mdb_fprevistaprevcobro = b;
    mdb_fcobroprevcobro = c;
    mdb_idfpago =d;
    mdb_idcuenta = e;
    mdb_idasiento = f;
    mdb_cantidadprevistaprevcobro = g;
    mdb_cantidadprevcobro = h;
    mdb_idregistroiva = i;
    mdb_tipoprevcobro = j;
    mdb_docprevcobro = k;
    mdb_codigocuenta = l;
    mdb_nomcuenta = m;

    mdb_idctacliente = idctacliente;
   mdb_codigoctacliente = codigoctacliente;
	mdb_nomctacliente = nomctacliente;
}// end linprevcobro


linprevcobro::~linprevcobro() {}


void linprevcobro::vacialinprevcobro() {
    mdb_idprevcobro = "";
    mdb_fprevistaprevcobro = "";
    mdb_fcobroprevcobro = "";
    mdb_idfpago ="";
    mdb_idcuenta = "";
    mdb_idasiento ="";
    mdb_cantidadprevistaprevcobro ="";
    mdb_cantidadprevcobro = "";
    mdb_idregistroiva = "";
    mdb_tipoprevcobro = "";
    mdb_docprevcobro = "";
    mdb_codigocuenta = "";
    mdb_nomcuenta = "";

	mdb_idctacliente = "";
	mdb_codigoctacliente = "";
	mdb_nomctacliente = "";
}// end vacialinprevcobro


void linprevcobro::borrar() {
    if (mdb_idprevcobro != "") {
        conexionbase->begin();
        conexionbase->ejecuta("DELETE FROM prevcobro WHERE idprevcobro="+mdb_idprevcobro);
        conexionbase->commit();
        vacialinprevcobro();
    }// end if
}// end delete

void linprevcobro::guardalinprevcobro() {
    if (mdb_idfpago == "")
        mdb_idfpago = "NULL";
    if (mdb_idasiento == "")
        mdb_idasiento = "NULL";
    if (mdb_idregistroiva == "")
        mdb_idregistroiva = "NULL";

    /// Segun est�la linea en la base de datos o no se hace una cosa u otra.
    if (mdb_idprevcobro == "") {
        QString SQLQuery = "INSERT INTO prevcobro (fprevistaprevcobro, fcobroprevcobro, idfpago, idcuenta, idasiento, cantidadprevistaprevcobro, cantidadprevcobro, idregistroiva, tipoprevcobro, docprevcobro, idctacliente) VALUES ('"+
                           conexionbase->sanearCadena(mdb_fprevistaprevcobro)+"',"+
                           "'"+conexionbase->sanearCadena(mdb_fcobroprevcobro)+"',"+
                           conexionbase->sanearCadena(mdb_idfpago)+","+
                           conexionbase->sanearCadena(mdb_idcuenta)+","+
                           conexionbase->sanearCadena(mdb_idasiento)+","+
                           conexionbase->sanearCadena(mdb_cantidadprevistaprevcobro)+", "+
                           conexionbase->sanearCadena(mdb_cantidadprevcobro)+", "+
                           conexionbase->sanearCadena(mdb_idregistroiva)+", "+
                           "'"+conexionbase->sanearCadena(mdb_tipoprevcobro)+"', "+
                           "'"+conexionbase->sanearCadena(mdb_docprevcobro)+"', "+
			   conexionbase->sanearCadena(mdb_idctacliente)+")";
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        cursor2 *cur = conexionbase->cargacursor("SELECT MAX(idprevcobro) AS m FROM prevcobro ");
        if(!cur->eof())
            mdb_idprevcobro = cur->valor("m");
        delete cur;
        conexionbase->commit();
    } else {
        QString SQLQuery = "UPDATE prevcobro SET ";
        SQLQuery += " fprevistaprevcobro = '"+conexionbase->sanearCadena(mdb_fprevistaprevcobro)+"' ";
        SQLQuery += " ,fcobroprevcobro = '"+conexionbase->sanearCadena(mdb_fcobroprevcobro)+"' ";
        SQLQuery += " ,idfpago = "+conexionbase->sanearCadena(mdb_idfpago)+" ";
        SQLQuery += " ,idcuenta = "+conexionbase->sanearCadena(mdb_idcuenta)+" ";
        SQLQuery += " ,idasiento = "+conexionbase->sanearCadena(mdb_idasiento)+" ";
        SQLQuery += " ,cantidadprevistaprevcobro = "+conexionbase->sanearCadena(mdb_cantidadprevistaprevcobro)+" ";
        SQLQuery += " ,cantidadprevcobro = "+conexionbase->sanearCadena(mdb_cantidadprevcobro)+" ";
        SQLQuery += " ,idregistroiva = "+conexionbase->sanearCadena(mdb_idregistroiva)+" ";
        SQLQuery += " ,tipoprevcobro = '"+conexionbase->sanearCadena(mdb_tipoprevcobro)+"' ";
        SQLQuery += " ,docprevcobro = '"+conexionbase->sanearCadena(mdb_docprevcobro)+"' ";
	SQLQuery += " ,idctacliente = "+conexionbase->sanearCadena(mdb_idctacliente)+" ";
        SQLQuery += " WHERE idprevcobro = "+conexionbase->sanearCadena(mdb_idprevcobro);
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
    }// end if
}// end guardalinprevcobro


void linprevcobro::setcodigocuenta(QString val) {
    fprintf(stderr,"setcodigocuenta(%s)\n", val.ascii());
    mdb_codigocuenta=extiendecodigo(val, empresaactual->numdigitosempresa());
    QString SQLQuery = "SELECT idcuenta, descripcion FROM cuenta WHERE codigo='"+mdb_codigocuenta+"'";
    cursor2 *cur=conexionbase->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomcuenta=cur->valor("descripcion");
        mdb_idcuenta = cur->valor("idcuenta");
    }// end if
    delete cur;
}// end setcodigocompletoarticulo


void linprevcobro::setidcuenta(QString val) {
    fprintf(stderr,"setidcuenta(%s)\n", val.ascii());
    mdb_idcuenta=val;
    QString SQLQuery = "SELECT codigo, descripcion FROM cuenta WHERE idcuenta="+mdb_idcuenta+"";
    cursor2 *cur=conexionbase->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomcuenta=cur->valor("descripcion");
        mdb_codigocuenta = cur->valor("codigo");
    }// end if
    delete cur;
    fprintf(stderr,"end setidcuenta\n");
}// end setidarticulo



/**
  * \brief SLOT que respoonde a la creación de un asiento de cobro o pago a partir de la gestion de cobros y pagos.
  * Descripción:
  * 1.- Calculamos los campos Total, Tipo de Asiento (compra/venta), Cuenta bancaria y cuenta de cliente
  * 2.- Determinamos si es un cobro o un pago.
  * 3.- Cargamos la plantilla de cobro o pago y le metemos los valores necesarios
  * 4.- Generamos el asiento a partir del asiento inteligente.
  */
int linprevcobro::creaPago() {

    /// Si la previsión no está guardada en la base de datos salimos para que no haya problemas.
    if (idprevcobro() == "")
        return 0;

    QString idainteligente;
    QString total=cantidadprevcobro();
    QString codcuenta = codigoctacliente();
    QString codbanco = codigocuenta(); /// Hay que hacer que en cobros y pagos aparezca el código de cuenta.
    QString tipo = tipoprevcobro();
    QString fecha = fcobroprevcobro();

if (tipo == "t") {
    /// Buscamos cual es el asiento inteligente que realiza los cobros
    QString query = "SELECT * FROM ainteligente, configuracion WHERE descripcion=valor AND configuracion.nombre='Cobro'";
    cursor2 *cur = conexionbase->cargacursor(query);
    if (!cur->eof()) {
        idainteligente = cur->valor("idainteligente");
    } else {
        return 0;
    }// end if
    delete cur;
} else {
    /// Buscamos cual es el asiento inteligente que realiza los pagos.
    QString query = "SELECT * FROM ainteligente, configuracion WHERE descripcion=valor AND configuracion.nombre='Pago'";
    cursor2 *cur = conexionbase->cargacursor(query);
    if (!cur->eof()) {
        idainteligente = cur->valor("idainteligente");
    } else {
        return 0;
    }// end if
    delete cur;
}// end if

	/// PAra saber si al final se ha hecho el asiento o no almacenamos el valor actual del asiento para ver después cual es el número de asiento y comprobar si ha cambiado o no.
    QString idasiento = empresaactual->intapuntsempresa()->idasiento();


    /// Se va a generar el asiento
    int numasiento = 0;
    aplinteligentesview *nueva=new aplinteligentesview(empresaactual, 0,"");
    nueva->inicializa(numasiento);
    nueva->muestraplantilla(idainteligente.toInt());
    nueva->setfechaasiento(fecha);
    nueva->setvalores("$fecha$",fecha);
    nueva->setvalores("$codbanco$",codbanco);
    nueva->setvalores("$codcuenta$",codcuenta);
    nueva->setvalores("$total$",total);
    nueva->setmodo(1);
    nueva->exec();
    delete nueva;

    QString idasiento1 = empresaactual->intapuntsempresa()->idasiento();
    if (idasiento1 == idasiento) {
        return 0;
    }// end if
    mdb_idasiento= idasiento1;
    guardalinprevcobro();
    return 1;
}// end s_creaPago





void linprevcobro::setcodigoctacliente(QString val) {
    fprintf(stderr,"setcodigocuenta(%s)\n", val.ascii());
    mdb_codigoctacliente=extiendecodigo(val, empresaactual->numdigitosempresa());
    QString SQLQuery = "SELECT idcuenta, descripcion FROM cuenta WHERE codigo='"+mdb_codigoctacliente+"'";
    cursor2 *cur=conexionbase->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomctacliente=cur->valor("descripcion");
        mdb_idctacliente = cur->valor("idcuenta");
    }// end if
    delete cur;
}// end setcodigocompletoarticulo


void linprevcobro::setidctacliente(QString val) {
    fprintf(stderr,"setidcuenta(%s)\n", val.ascii());
    mdb_idctacliente=val;
    QString SQLQuery = "SELECT codigo, descripcion FROM cuenta WHERE idcuenta="+mdb_idctacliente+"";
    cursor2 *cur=conexionbase->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_nomctacliente=cur->valor("descripcion");
        mdb_codigoctacliente = cur->valor("codigo");
    }// end if
    delete cur;
    fprintf(stderr,"end setidcuenta\n");
}// end setidarticulo
