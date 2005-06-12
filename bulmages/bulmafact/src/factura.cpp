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
        companyact->ejecuta("DELETE FROM Factura WHERE idFactura="+mdb_idfactura);
        companyact->commit();
        vaciaFactura();
        pintaFactura();
    }// end if
}// end borraFactura


void Factura::vaciaFactura() {
    mdb_idfactura = "";
    mdb_idcliente= "";
    mdb_idalmacen= "";
    mdb_numfactura= "";
    mdb_ffactura= "";
    mdb_reffactura = "";
    mdb_comentfactura= "";
    mdb_procesadafactura="FALSE";
    mdb_codigoserie_factura="";
    mdb_idforma_pago="";
}// end vaciaFactura

void Factura::pintaFactura() {
    fprintf(stderr,"pintaFactura\n");
    pintaidcliente(mdb_idcliente);
    pintaidalmacen(mdb_idalmacen);
    pintaNumFactura(mdb_numfactura);
    pintafechafactura(mdb_ffactura);
    pintareffactura(mdb_reffactura);
    pintaComentFactura(mdb_comentfactura);
    pintaprocesadafactura(mdb_procesadafactura);
    pintacodigoserie_factura(mdb_codigoserie_factura);
    pintaidforma_pago(mdb_idforma_pago);
    // Pinta el subformulario de detalle del Factura.
    listalineas->pintaListLinFactura();

    pintatotales(listalineas->calculabase(), listalineas->calculaiva());
}// end pintaFactura


// Esta funciï¿½ carga un Factura.
void Factura::cargaFactura(QString idbudget) {
    mdb_idfactura = idbudget;
    inicialize();
    QString query = "SELECT * FROM factura  WHERE idfactura="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        mdb_idcliente= cur->valor("idcliente");
        mdb_idalmacen= cur->valor("idalmacen");
        mdb_numfactura= cur->valor("numfactura");
        mdb_ffactura= cur->valor("ffactura");
	mdb_codigoserie_factura = cur->valor("codigoserie_factura");

        mdb_comentfactura= cur->valor("comentfactura");
        mdb_reffactura = cur->valor("reffactura");
        if (cur->valor("procesadafactura") == "t")
            mdb_procesadafactura = "TRUE";
        else
            mdb_procesadafactura = "FALSE";
        mdb_idusuari = cur->valor("idusuari");
        mdb_idforma_pago = cur->valor("idforma_pago");
    }// end if
    delete cur;
    listalineas->cargaListLinFactura(idbudget);
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
    if (mdb_idusuari=="")
        mdb_idusuari="NULL";
    if (mdb_idforma_pago == "")
        mdb_idforma_pago = "NULL";
    if (mdb_idfactura == "") {
        /// Se trata de una inserción
        QString SQLQuery = "INSERT INTO factura (codigoserie_factura, procesadafactura, reffactura, numfactura, ffactura, comentfactura, idusuari, idcliente, idalmacen, idforma_pago) VALUES ('"+mdb_codigoserie_factura+"',"+mdb_procesadafactura+",'"+mdb_reffactura+"',"+mdb_numfactura+",'"+mdb_ffactura+"','"+mdb_comentfactura+"',"+mdb_idusuari+","+mdb_idcliente+","+mdb_idalmacen+","+mdb_idforma_pago+")";

        companyact->ejecuta(SQLQuery);
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idFactura) AS m FROM Factura");
        if (!cur->eof())
            setidfactura (cur->valor("m"));
        delete cur;
        companyact->commit();
    } else {
        /// Se trata de una modificaciï¿½
        QString SQLQuery = "UPDATE Factura SET ";
        SQLQuery += " numFactura="+mdb_numfactura;
        SQLQuery += " ,fFactura='"+mdb_ffactura+"'";
        SQLQuery += " ,comentFactura='"+mdb_comentfactura+"'";
        SQLQuery += " ,idusuari="+mdb_idusuari;
        SQLQuery += " ,idcliente="+mdb_idcliente;
        SQLQuery += " ,idalmacen="+mdb_idalmacen;
        SQLQuery += " ,reffactura='"+mdb_reffactura+"'";
        SQLQuery += " ,idforma_pago="+mdb_idforma_pago;
        SQLQuery += " ,procesadafactura="+mdb_procesadafactura;
	SQLQuery += " ,codigoserie_factura='"+mdb_codigoserie_factura+"'";
        SQLQuery += " WHERE idFactura="+mdb_idfactura;
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        companyact->commit();
    }// end if
    listalineas->guardaListLinFactura();
    cargaFactura(mdb_idfactura);
}// end guardaFactura





