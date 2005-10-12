//
// C++ Implementation: ListDescuentoPedidoCliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listdescpedidocliente.h"
#include "company.h"
#include "descpedidocliente.h"


ListDescuentoPedidoCliente::ListDescuentoPedidoCliente(company *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idpedidocliente="";
}// end ListDescuentoPedidoCliente

ListDescuentoPedidoCliente::ListDescuentoPedidoCliente() {
    	   fprintf(stderr,"Constructor de ListDescuentoPedidoCliente\n");
           companyact=NULL;
           m_lista.setAutoDelete(TRUE);
           mdb_idpedidocliente="";
}// end ListDescuentoPedidoCliente

ListDescuentoPedidoCliente::~ListDescuentoPedidoCliente() {}


void ListDescuentoPedidoCliente::nuevalinea(QString concept, QString propor) {
        DescuentoPedidoCliente *lin = new DescuentoPedidoCliente(companyact,
                              "",
                              concept,
                              propor,
                              mdb_idpedidocliente);
	m_lista.append(lin);
}// end nuevalinea

DescuentoPedidoCliente *ListDescuentoPedidoCliente::linpos(int pos) {
	return (m_lista.at(pos));
}// end linpos


// Carga l�eas de pedidocliente
void ListDescuentoPedidoCliente::cargaDescuentos(QString idbudget) {
    vaciar();
    fprintf(stderr,"ListDescuentoPedidoCliente::chargeBudgetLines\n");
    mdb_idpedidocliente = idbudget;
    fprintf(stderr,"Hacemos la carga del cursor\n");
    cursor2 * cur= companyact->cargacursor("SELECT * FROM dpedidocliente WHERE idpedidocliente="+idbudget,"unquery");
    int i=0;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo DescuentoPedidoCliente y lo agregamos a la lista.
        DescuentoPedidoCliente *lin = new DescuentoPedidoCliente(companyact,
                              cur->valor("iddpedidocliente"),
                              cur->valor("conceptdpedidocliente"),
                              cur->valor("proporciondpedidocliente"),
                              cur->valor("idpedidocliente")
                   );
        m_lista.append(lin);
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
    fprintf(stderr,"Fin de ListDescuentoPedidoCliente::chargeBudgetLines\n");
}// end chargeBudgetLines


void ListDescuentoPedidoCliente::guardaListDescuentoPedidoCliente() {
    DescuentoPedidoCliente *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->guardaDescuentoPedidoCliente();
        i++;
    }// end for
}// en guardaListDescuentoPedidoCliente


 
void ListDescuentoPedidoCliente::vaciar() {
    mdb_idpedidocliente = "";
    m_lista.clear();
}// end guardaListDescuentoPedidoCliente


void ListDescuentoPedidoCliente::borrar() {
    if (mdb_idpedidocliente != "")  {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM dpedidocliente WHERE idpedidocliente="+mdb_idpedidocliente);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
}// end borrar


void ListDescuentoPedidoCliente::borraDescuentoPedidoCliente(int pos) {
    DescuentoPedidoCliente *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintaListDescuentoPedidoCliente();
}// end borraDescuentoPedidoCliente
