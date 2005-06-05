//
// C++ Implementation: Factura
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "factura.h"
#include "company.h"


Factura::Factura(company *comp) {
    companyact=comp;
    vaciaFactura();
}

Factura::~Factura() {}


void Factura::borraFactura() {
    if (mdb_idfactura != "") {
        listalineas->borrar();
        companyact->begin();
        companyact->ejecuta("DELETE FROM prfp WHERE idFactura="+mdb_idfactura);
        companyact->ejecuta("DELETE FROM Factura WHERE idFactura="+mdb_idfactura);
        companyact->commit();
        vaciaFactura();
        pintaFactura();
    }// end if
}// end borraFactura


void Factura::vaciaFactura() {
    m_idfactura = "0";
    m_idclient = "";
    mdb_idfactura = "";
    mdb_idcliente= "";
    mdb_idalmacen= "";
    mdb_numfactura= "";
    mdb_ffactura= "";
    mdb_contactfactura= "";
    mdb_telfactura= "";
    mdb_comentfactura= "";
    mdb_nomcliente= "";
    mdb_cifcliente= "";
    mdb_codigoalmacen= "";
    mdb_nomalmacen= "";
    mdb_idforma_pago="";
//    listalineas->vaciar();
}// end vaciaFactura

void Factura::pintaFactura() {
    fprintf(stderr,"pintaFactura\n");
    pintaIdClient(mdb_idcliente);
    pintaIdAlmacen(mdb_idalmacen);
    pintaNumFactura(mdb_numfactura);
    pintaFFactura(mdb_ffactura);
    pintaContractFactura(mdb_contactfactura);
    pintaTelFactura(mdb_telfactura);
    pintaComentFactura(mdb_comentfactura);
    pintaNomClient(mdb_nomcliente);
    pintaCifClient(mdb_cifcliente);
    pintaCodigoAlmacen(mdb_codigoalmacen);
    pintaNomAlmacen(mdb_nomalmacen);
    pintaFormaPago(mdb_idforma_pago);
    // Pinta el subformulario de detalle del Factura.
    listalineas->pintaListLinFactura();
    
    pintatotales(listalineas->calculabase(), listalineas->calculaiva());
}// end pintaFactura


// Esta funciï¿½ carga un Factura.
void Factura::cargaFactura(QString idbudget) {
    m_idfactura = idbudget;
    mdb_idfactura = idbudget;
    inicialize();
    QString query = "SELECT * FROM factura LEFT JOIN cliente ON cliente.idcliente = factura.idcliente LEFT JOIN almacen ON  factura.idalmacen = almacen.idalmacen WHERE idfactura="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        mdb_idcliente= cur->valor("idcliente");
        mdb_idalmacen= cur->valor("idalmacen");
        mdb_numfactura= cur->valor("numfactura");
        mdb_ffactura= cur->valor("ffactura");
        mdb_contactfactura= cur->valor("contactfactura");
        mdb_telfactura= cur->valor("telfactura");
        mdb_comentfactura= cur->valor("comentfactura");
        mdb_nomcliente= cur->valor("nomcliente");
        mdb_cifcliente= cur->valor("cifcliente");
        mdb_codigoalmacen= cur->valor("codigoalmacen");
        mdb_nomalmacen= cur->valor("nomalmacen");
        mdb_idusuari = cur->valor("idusuari");
	mdb_idforma_pago = cur->valor("idforma_pago");
    }// end if
    delete cur;
    fprintf(stderr,"Vamos a cargar las lineas\n");
    listalineas->cargaListLinFactura(idbudget);
    fprintf(stderr,"vamos a hacer el pintado de la FActura\n");
    pintaFactura();
}// end chargeBudget


void Factura::guardaFactura() {
    companyact->begin();
    if (mdb_numfactura == "") {
        QString SQLQueryn = "SELECT MAX(numFactura)+1 as num FROM Factura";
        cursor2 *cur= companyact->cargacursor(SQLQueryn);
        if (!cur->eof())
            mdb_numfactura = cur->valor("num");
        delete cur;
    }// end if
    if (mdb_idusuari="")
        mdb_idusuari="NULL";
    if (mdb_idfactura == "") {
        /// Se trata de una inserción
        QString SQLQuery = "INSERT INTO factura (numfactura, ffactura, contactfactura, telfactura, comentfactura, idusuari, idcliente, idalmacen, idforma_pago) VALUES ("+mdb_numfactura+",'"+mdb_ffactura+"','"+mdb_contactfactura+"','"+mdb_telfactura+"','"+mdb_comentfactura+"',"+mdb_idusuari+","+mdb_idcliente+","+mdb_idalmacen+","+mdb_idforma_pago+")";

        companyact->ejecuta(SQLQuery);
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idFactura) AS m FROM Factura");
        if (!cur->eof())
            mdb_idfactura = cur->valor("idFactura");
        delete cur;
        companyact->commit();
    } else {
        /// Se trata de una modificaciï¿½
        QString SQLQuery = "UPDATE Factura SET ";
        SQLQuery += " numFactura="+mdb_numfactura;
        SQLQuery += " ,fFactura='"+mdb_ffactura+"'";
        SQLQuery += " ,contactFactura='"+mdb_contactfactura+"'";
        SQLQuery += " ,telFactura='"+mdb_telfactura+"'";
        SQLQuery += " ,comentFactura='"+mdb_comentfactura+"'";
        SQLQuery += " ,idusuari="+mdb_idusuari;
        SQLQuery += " ,idcliente="+mdb_idcliente;
        SQLQuery += " ,idalmacen="+mdb_idalmacen;
        SQLQuery += " ,idforma_pago="+mdb_idforma_pago;	
        SQLQuery += " WHERE idFactura="+mdb_idfactura;
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        companyact->commit();
    }// end if
    listalineas->guardaListLinFactura();
}// end guardaFactura


void Factura::setCifClient(QString val) {
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
    pintaNomClient(mdb_nomcliente);
}// end setCifClient

void Factura::setCodigoAlmacen(QString val) {
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



