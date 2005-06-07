//
// C++ Implementation: albarancliente
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "albarancliente.h"

AlbaranCliente::AlbaranCliente(company *comp) {
	companyact=comp;
}


AlbaranCliente::~AlbaranCliente()
{
}

void AlbaranCliente::borraAlbaranCliente() {
    if (mdb_idfactura != "") {
        listalineas->borrar();
        companyact->begin();
        companyact->ejecuta("DELETE FROM albaran WHERE idalbaran="+mdb_idalbaran);
        companyact->commit();
        vaciaAlbaranCliente();
        pintaAlbaranCliente();
    }// end if
}// end borraAlbaranCliente


void AlbaranCliente::vaciaAlbaranCliente() {
    mdb_idalbaran="";
    mdb_numalbaran="";
    mdb_fechaalbaran="";
    mdb_idusuario="";
    mdb_comentalbaran="";
    mdb_idcliente="";
    mdb_idforma_pago="";
    mdb_idfactura="";
    mdb_idalmacen="";
    
    mdb_codigoalmacen="";
    mdb_nomalmacen="";
    mdb_cifcliente="";
    mdb_nomcliente="";
    mdb_numfactura="";
}// end vaciaAlbaranCliente


void AlbaranCliente::pintaAlbaranCliente() {
    fprintf(stderr,"pintaAlbaranCliente\n");
    pintaIdAlbaran(mdb_idalbaran);
    pintaNumAlbaran(mdb_numalbaran);
    pintaFechaAlbaran(mdb_fechaalbaran);
    pintaIdUsuario(mdb_idusuario);
    pintaComentAlbaran(mdb_comentalbaran);
    pintaIdCliente(mdb_idcliente);
    pintaIdForma_Pago(mdb_idforma_pago);
    pintaIdAlbaran(mdb_idfactura);
    pintaIdAlmacen(mdb_idalmacen);
    pintaCodigoAlmacen(mdb_codigoalmacen);
    pintaNomAlmacen(mdb_nomalmacen);
    pintaCifCliente(mdb_cifcliente);
    pintaNomCliente(mdb_nomcliente);
    pintaNumAlbaran(mdb_numfactura);
        
    /// Pinta el subformulario de detalle del AlbaranCliente.
    listalineas->pintaListLinAlbaranCliente();
    /// Pintamos los totales
    pintatotales(listalineas->calculabase(), listalineas->calculaiva());
}// end pintaAlbaranCliente



// Esta funciï¿½ carga un AlbaranCliente.
void AlbaranCliente::cargaAlbaranCliente(QString idbudget) {
    fprintf(stderr,"AlbaranCliente::cargaAlbaranCliente(%s)\n",idbudget.ascii());
    mdb_idalbaran = idbudget;
//    inicialize();
    QString query = "SELECT * FROM albaran LEFT JOIN cliente ON cliente.idcliente = albaran.idcliente LEFT JOIN almacen ON  albaran.idalmacen = almacen.idalmacen WHERE idalbaran="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        mdb_idcliente= cur->valor("idcliente");
        mdb_idalmacen= cur->valor("idalmacen");
        mdb_numalbaran= cur->valor("numalbaran");
        mdb_fechaalbaran= cur->valor("fechaalbaran");
        mdb_comentalbaran= cur->valor("comentalbaran");
        mdb_nomcliente= cur->valor("nomcliente");
        mdb_cifcliente= cur->valor("cifcliente");
        mdb_codigoalmacen= cur->valor("codigoalmacen");
        mdb_nomalmacen= cur->valor("nomalmacen");
        mdb_idusuario = cur->valor("idusuario");
	mdb_idforma_pago = cur->valor("idforma_pago");
    }// end if
    delete cur;
    fprintf(stderr,"Vamos a cargar las lineas\n");
    listalineas->cargaListLinAlbaranCliente(idbudget);
    fprintf(stderr,"vamos a hacer el pintado del Albaran\n");
    pintaAlbaranCliente();
}// end chargeBudget



void AlbaranCliente::guardaAlbaranCliente() {
    /// Todo el guardado es una transacción
    companyact->begin();
    if (mdb_idalbaran == "") {
        QString SQLQueryn = "SELECT MAX(numalbaran)+1 as num FROM albaran";
        cursor2 *cur= companyact->cargacursor(SQLQueryn);
        if (!cur->eof())
            mdb_numfactura = cur->valor("num");
        delete cur;
    }// end if
    if (mdb_idusuario="")
        mdb_idusuario="NULL";
    if (mdb_idfactura="")
    	mdb_idfactura="NULL";
    if (mdb_idalbaran == "") {
        /// Se trata de una inserción
        QString SQLQuery = "INSERT INTO albaran (numalbaran, fechaalbaran, idusuario, comentalbaran, idcliente, idforma_pago, idfactura, idalmacen) VALUES ("+mdb_numalbaran+",'"+mdb_fechaalbaran+"',"+mdb_idusuario+",'"+mdb_comentalbaran+"',"+mdb_idcliente+","+mdb_idforma_pago+","+mdb_idfactura+","+mdb_idalmacen+")";
        companyact->ejecuta(SQLQuery);
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idalbaran) AS m FROM albaran");
        if (!cur->eof())
            mdb_idalbaran = cur->valor("m");
        delete cur;
    } else {
        /// Se trata de una modificacion
        QString SQLQuery = "UPDATE albaran SET ";
        SQLQuery += " numalbaran="+mdb_numalbaran;
        SQLQuery += " ,fechaalbaran='"+mdb_fechaalbaran+"'";
        SQLQuery += " ,idusuario="+mdb_idusuario+"";
        SQLQuery += " ,comentalbaran='"+mdb_comentalbaran+"'";
        SQLQuery += " ,idcliente="+mdb_idcliente+"";
        SQLQuery += " ,idforma_pago="+mdb_idforma_pago;
        SQLQuery += " ,idfactura="+mdb_idfactura;
        SQLQuery += " ,idalmacen="+mdb_idalmacen;
        SQLQuery += " WHERE idalbaran="+mdb_idalbaran;
        companyact->ejecuta(SQLQuery);
    }// end if
       companyact->commit();
       listalineas->guardaListLinAlbaranCliente();
}// end guardaAlbaranCliente


void AlbaranCliente::setCifCliente(QString val) {
    mdb_cifcliente=val;
    QString SQLQuery = "SELECT * FROM cliente WHERE cifcliente='"+mdb_cifcliente+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idcliente = cur->valor("idcliente");
        mdb_nomcliente = cur->valor("nomcliente");
    } else {
        mdb_idcliente="";
        mdb_nomcliente="";
    }// end if
    delete cur;
    pintaNomCliente(mdb_nomcliente);
}// end setCifClient

void AlbaranCliente::setCodigoAlmacen(QString val) {
    mdb_codigoalmacen=val;
    QString SQLQuery = "SELECT * FROM almacen WHERE codigoalmacen='"+mdb_codigoalmacen+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idalmacen = cur->valor("idalmacen");
        mdb_nomalmacen = cur->valor("nomalmacen");
    } else {
        mdb_idalmacen="";
        mdb_nomalmacen="";
    }// end if
    delete cur;
    pintaNomAlmacen(mdb_nomalmacen);
}// end setCodigoAlmacen

