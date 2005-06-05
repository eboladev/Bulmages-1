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
}// end vaciaFactura


void AlbaranCliente::pintaAlbaranCliente() {
    fprintf(stderr,"pintaAlbaranCliente\n");
    pintaIdAlbaran(mdb_idalbaran);
    pintaNumAlbaran(mdb_numalbaran);
    pintaFechaAlbaran(mdb_fechaalbaran);
    pintaIdUsuario(mdb_idusuario);
    pintaComentAlbaran(mdb_comentalbaran);
    pintaIdCliente(mdb_idcliente);
    pintaIdForma_Pago(mdb_idforma_pago);
    pintaIdFactura(mdb_idfactura);
    pintaIdAlmacen(mdb_idalmacen);
    pintaCodigoAlmacen(mdb_codigoalmacen);
    pintaNomAlmacen(mdb_nomalmacen);
    pintaCifCliente(mdb_cifcliente);
    pintaNomCliente(mdb_nomcliente);
    pintaNumFactura(mdb_numfactura);
        
    /// Pinta el subformulario de detalle del Factura.
    listalineas->pintaListLinAlbaranCliente();
    /// Pintamos los totales
    pintatotales(listalineas->calculabase(), listalineas->calculaiva());
}// end pintaFactura



// Esta funci� carga un Factura.
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

