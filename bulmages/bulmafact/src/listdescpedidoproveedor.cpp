//
// C++ Implementation: ListDescuentoPedidoProveedor
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listdescpedidoproveedor.h"
#include "company.h"
#include "descpedidoproveedor.h"


ListDescuentoPedidoProveedor::ListDescuentoPedidoProveedor(company *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idpedidoproveedor="";
}// end ListDescuentoPedidoProveedor

ListDescuentoPedidoProveedor::ListDescuentoPedidoProveedor() {
    	   fprintf(stderr,"Constructor de ListDescuentoPedidoProveedor\n");
           companyact=NULL;
           m_lista.setAutoDelete(TRUE);
           mdb_idpedidoproveedor="";
}// end ListDescuentoPedidoProveedor

ListDescuentoPedidoProveedor::~ListDescuentoPedidoProveedor() {}


void ListDescuentoPedidoProveedor::nuevalinea(QString concept, QString propor) {
        DescuentoPedidoProveedor *lin = new DescuentoPedidoProveedor(companyact,
                              "",
                              concept,
                              propor,
                              mdb_idpedidoproveedor);
	m_lista.append(lin);
}// end nuevalinea

DescuentoPedidoProveedor *ListDescuentoPedidoProveedor::linpos(int pos) {
	return (m_lista.at(pos));
}// end linpos


// Carga l�eas de pedidoproveedor
void ListDescuentoPedidoProveedor::cargaDescuentos(QString idbudget) {
    vaciar();
    fprintf(stderr,"ListDescuentoPedidoProveedor::chargeBudgetLines\n");
    mdb_idpedidoproveedor = idbudget;
    fprintf(stderr,"Hacemos la carga del cursor\n");
    cursor2 * cur= companyact->cargacursor("SELECT * FROM dpedidoproveedor WHERE idpedidoproveedor="+idbudget,"unquery");
    int i=0;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo DescuentoPedidoProveedor y lo agregamos a la lista.
        DescuentoPedidoProveedor *lin = new DescuentoPedidoProveedor(companyact,
                              cur->valor("iddpedidoproveedor"),
                              cur->valor("conceptdpedidoproveedor"),
                              cur->valor("proporciondpedidoproveedor"),
                              cur->valor("idpedidoproveedor")
                   );
        m_lista.append(lin);
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
    fprintf(stderr,"Fin de ListDescuentoPedidoProveedor::chargeBudgetLines\n");
}// end chargeBudgetLines


void ListDescuentoPedidoProveedor::guardaListDescuentoPedidoProveedor() {
    DescuentoPedidoProveedor *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->guardaDescuentoPedidoProveedor();
        i++;
    }// end for
}// en guardaListDescuentoPedidoProveedor


 
void ListDescuentoPedidoProveedor::vaciar() {
    mdb_idpedidoproveedor = "";
    m_lista.clear();
}// end guardaListDescuentoPedidoProveedor


void ListDescuentoPedidoProveedor::borrar() {
    if (mdb_idpedidoproveedor != "")  {
        companyact->begin();
        companyact->ejecuta("DELETE FROM dpedidoproveedor WHERE idpedidoproveedor="+mdb_idpedidoproveedor);
        companyact->commit();
    }// end if
}// end borrar


void ListDescuentoPedidoProveedor::borraDescuentoPedidoProveedor(int pos) {
    DescuentoPedidoProveedor *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintaListDescuentoPedidoProveedor();
}// end borraDescuentoPedidoProveedor
