//
// C++ Implementation: ListLinPedidoCliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listlinpedidocliente.h"
#include "company.h"
#include "linpedidocliente.h"


ListLinPedidoCliente::ListLinPedidoCliente(company *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idpedidocliente="";
}// end ListLinPedidoCliente

ListLinPedidoCliente::ListLinPedidoCliente() {
    	   fprintf(stderr,"Constructor de ListLinPedidoCliente\n");
           companyact=NULL;
           m_lista.setAutoDelete(TRUE);
           mdb_idpedidocliente="";
}// end ListLinPedidoCliente

ListLinPedidoCliente::~ListLinPedidoCliente() {}


void ListLinPedidoCliente::nuevalinea(QString desclpedidocliente, QString cantlpedidocliente, QString pvplpedidocliente, QString prevlpedidocliente, QString ivalpedidocliente, QString descuentolpedidocliente, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo) {
        LinPedidoCliente *lin = new LinPedidoCliente(companyact,
                              "",
                              desclpedidocliente,
                              cantlpedidocliente,
                              pvplpedidocliente,
                              prevlpedidocliente,
                              ivalpedidocliente,
                              descuentolpedidocliente,
                              mdb_idpedidocliente,
                              idarticulo,
			      codigocompletoarticulo,
			      nomarticulo
			      );
	m_lista.append(lin);
}// end nuevalinea

LinPedidoCliente *ListLinPedidoCliente::linpos(int pos) {
	return (m_lista.at(pos));
}// end linpos


// Carga l�eas de Factura
void ListLinPedidoCliente::cargaListLinPedidoCliente(QString idbudget) {
    fprintf(stderr,"ListLinPedidoCliente::cargaListLinPedidoCliente(%s)\n", idbudget.ascii());
    mdb_idpedidocliente = idbudget;
    fprintf(stderr,"Hacemos la carga del cursor\n");
    cursor2 * cur= companyact->cargacursor("SELECT * FROM lpedidocliente, articulo WHERE idpedidocliente="+mdb_idpedidocliente+" AND articulo.idarticulo=lpedidocliente.idarticulo");
    int i=0;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo LinPedidoCliente y lo agregamos a la lista.
        LinPedidoCliente *lin = new LinPedidoCliente(companyact,
                              cur->valor("numlpedidocliente"),
                              cur->valor("desclpedidocliente"),
                              cur->valor("cantlpedidocliente"),
                              cur->valor("pvplpedidocliente"),
                              cur->valor("prevlpedidocliente"),
                              cur->valor("ivalpedidocliente"),
                              cur->valor("descuentolpedidocliente"),
                              cur->valor("idpedidocliente"),
                              cur->valor("idarticulo"),
			      cur->valor("codigocompletoarticulo"),
			      cur->valor("nomarticulo"));
        m_lista.append(lin);
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
    fprintf(stderr,"Fin de ListLinPedidoCliente::cargaListLinPedidoCliente\n");
}// end chargeBudgetLines


void ListLinPedidoCliente::guardaListLinPedidoCliente() {
    LinPedidoCliente *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->guardaLinPedidoCliente();
        i++;
    }// end for
}// en guardaListLinPedidoCliente


 float ListLinPedidoCliente::calculabase() {
     float base=0;
     LinPedidoCliente *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        base += linea->calculabase();
        i++;
    }// end for
    return base;
 }// end calculabase

 float ListLinPedidoCliente::calculaiva() {
     float iva=0;
     LinPedidoCliente *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        iva += linea->calculaiva();
        i++;
    }// end for
    return iva;
 }// end calculabase

 
void ListLinPedidoCliente::vaciar() {
    mdb_idpedidocliente = "";
    m_lista.clear();
}// end guardaListLinPedidoCliente


void ListLinPedidoCliente::borrar() {
    if (mdb_idpedidocliente != "")  {
        companyact->begin();
        companyact->ejecuta("DELETE FROM lpedidocliente WHERE idpedidocliente="+mdb_idpedidocliente);
        companyact->commit();
    }// end if
}// end borrar


void ListLinPedidoCliente::borraLinPedidoCliente(int pos) {
    LinPedidoCliente *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintaListLinPedidoCliente();
}// end borraLinPedidoCliente
