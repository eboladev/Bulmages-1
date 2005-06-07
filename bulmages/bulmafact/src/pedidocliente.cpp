//
// C++ Implementation: PedidoCliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pedidocliente.h"
#include "company.h"


PedidoCliente::PedidoCliente(company *comp) {
    companyact=comp;
    vaciaPedidoCliente();
}

PedidoCliente::~PedidoCliente() {}


void PedidoCliente::borraPedidoCliente() {
    if (mdb_idpedidocliente != "") {
        listalineas->borrar();
        companyact->begin();
        companyact->ejecuta("DELETE FROM pedidocliente WHERE idpedidocliente="+mdb_idpedidocliente);
        companyact->commit();
        vaciaPedidoCliente();
        pintaPedidoCliente();
    }// end if
}// end borraPedidoCliente


void PedidoCliente::vaciaPedidoCliente() {
    mdb_idcliente = "";
    mdb_idalmacen = "";
    mdb_idpedidocliente = "";
    mdb_numpedidocliente = "";
    mdb_fechapedidocliente = "";
    mdb_descpedidocliente = "";
    mdb_nomcliente = "";
    mdb_nomalmacen = "";
    mdb_codigoalmacen = "";
    mdb_cifcliente = "";
    mdb_idforma_pago = "";
    mdb_comentpedidocliente = "";
    //    listalineas->vaciar();
}// end vaciaPedidoCliente

void PedidoCliente::pintaPedidoCliente() {
    fprintf(stderr,"PedidoCliente::pintaPedidoCliente\n");    
    pintaidcliente(mdb_idcliente);
    pintaidalmacen(mdb_idalmacen);
    pintaidpedidocliente(mdb_idpedidocliente);
    pintanumpedidocliente(mdb_numpedidocliente);
    pintafechapedidocliente(mdb_fechapedidocliente);
    pintadescpedidocliente(mdb_descpedidocliente);
    pintanomcliente(mdb_nomcliente);
    pintanomalmacen(mdb_nomalmacen);
    pintaidforma_pago(mdb_idforma_pago);
    pintacodigoalmacen(mdb_codigoalmacen);      
    pintacifcliente(mdb_cifcliente);  
    pintacomentpedidocliente(mdb_comentpedidocliente);    
    // Pinta el subformulario de detalle del PedidoCliente.
    listalineas->pintaListLinPedidoCliente();
    pintatotales(listalineas->calculabase(), listalineas->calculaiva());   
  fprintf(stderr,"FIN PedidoCliente::pintaPedidoCliente()\n");
}// end pintaPedidoCliente


// Esta funciï¿½ carga un PedidoCliente.
void PedidoCliente::cargaPedidoCliente(QString idbudget) {
    fprintf(stderr,"cargaPedidoCliente(%s)\n",idbudget.ascii());
    mdb_idpedidocliente = idbudget;
    QString query = "SELECT * FROM pedidocliente LEFT JOIN cliente ON cliente.idcliente = pedidocliente.idcliente LEFT JOIN almacen ON  pedidocliente.idalmacen = almacen.idalmacen WHERE idpedidocliente="+mdb_idpedidocliente;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        mdb_idcliente = cur->valor("idcliente");
        mdb_idalmacen = cur->valor("idalmacen");
        mdb_idpedidocliente = cur->valor("idpedidocliente");
        mdb_numpedidocliente = cur->valor("numpedidocliente");
        mdb_fechapedidocliente = cur->valor("fechapedidocliente");
        mdb_descpedidocliente = cur->valor("descpedidocliente");
        mdb_nomcliente = cur->valor("nomcliente");
        mdb_nomalmacen = cur->valor("nomalmacen");
        mdb_codigoalmacen = cur->valor("codigoalmacen");
        mdb_cifcliente = cur->valor("cifcliente");
        mdb_idforma_pago = cur->valor("idforma_pago");
	mdb_comentpedidocliente = cur->valor("comentpedidocliente");
    }// end if
    delete cur;
    fprintf(stderr,"Vamos a cargar las lineas\n");
    listalineas->cargaListLinPedidoCliente(idbudget);
    fprintf(stderr,"vamos a hacer el pintado de la LineaPedido\n");
    pintaPedidoCliente();
}// end chargeBudget


void PedidoCliente::guardaPedidoCliente() {
    companyact->begin();
    if (mdb_numpedidocliente == "") {
        QString SQLQueryn = "SELECT MAX(numpedidocliente)+1 as num FROM pedidocliente";
        cursor2 *cur= companyact->cargacursor(SQLQueryn);
        if (!cur->eof())
            mdb_numpedidocliente = cur->valor("num");
        delete cur;
    }// end if
    if (mdb_idpedidocliente == "") {
        /// Se trata de una inserción
        QString SQLQuery = "INSERT INTO pedidocliente (numpedidocliente, fechapedidocliente, idcliente, idalmacen, idforma_pago) VALUES ("+mdb_numpedidocliente+",'"+mdb_fechapedidocliente+"',"+mdb_idcliente+","+mdb_idalmacen+","+mdb_idforma_pago+")";

        companyact->ejecuta(SQLQuery);
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idpedidocliente) AS m FROM pedidocliente");
        if (!cur->eof())
            mdb_idpedidocliente = cur->valor("idpedidocliente");
        delete cur;
        companyact->commit();
    } else {
        /// Se trata de una modificación
        QString SQLQuery = "UPDATE pedidocliente SET ";
        SQLQuery += " numpedidocliente='"+mdb_numpedidocliente+"'";
        SQLQuery += " ,fechapedidocliente='"+mdb_fechapedidocliente+"'";
        SQLQuery += " ,idcliente="+mdb_idcliente;
        SQLQuery += " ,idalmacen="+mdb_idalmacen;
        SQLQuery += " ,idforma_pago="+mdb_idforma_pago;
        SQLQuery += " WHERE idpedidocliente="+mdb_idpedidocliente;
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        companyact->commit();
    }// end if
    listalineas->guardaListLinPedidoCliente();
}// end guardaPedidoCliente


void PedidoCliente::setcifcliente(QString val) {
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
    pintanomcliente(mdb_nomcliente);
}// end setCifClient

void PedidoCliente::setcodigoalmacen(QString val) {
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
    pintanomalmacen(mdb_nomalmacen);
}// end setCodigoAlmacen



