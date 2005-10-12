//
// C++ Implementation: ListLinPedidoProveedor
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listlinpedidoproveedor.h"
#include "company.h"
#include "linpedidoproveedor.h"


ListLinPedidoProveedor::ListLinPedidoProveedor(company *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idpedidoproveedor="";
}// end ListLinPedidoProveedor

ListLinPedidoProveedor::ListLinPedidoProveedor() {
    	   fprintf(stderr,"Constructor de ListLinPedidoProveedor\n");
           companyact=NULL;
           m_lista.setAutoDelete(TRUE);
           mdb_idpedidoproveedor="";
}// end ListLinPedidoProveedor

ListLinPedidoProveedor::~ListLinPedidoProveedor() {}


void ListLinPedidoProveedor::nuevalinea(QString desclpedidoproveedor, QString cantlpedidoproveedor, QString pvplpedidoproveedor, QString prevlpedidoproveedor, QString ivalpedidoproveedor, QString descuentolpedidoproveedor, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo) {
        LinPedidoProveedor *lin = new LinPedidoProveedor(companyact,
                              "",
                              desclpedidoproveedor,
                              cantlpedidoproveedor,
                              pvplpedidoproveedor,
                              prevlpedidoproveedor,
                              ivalpedidoproveedor,
                              descuentolpedidoproveedor,
                              mdb_idpedidoproveedor,
                              idarticulo,
			      codigocompletoarticulo,
			      nomarticulo
			      );
	m_lista.append(lin);
}// end nuevalinea

LinPedidoProveedor *ListLinPedidoProveedor::linpos(int pos) {
	return (m_lista.at(pos));
}// end linpos


// Carga l�eas de Factura
void ListLinPedidoProveedor::cargaListLinPedidoProveedor(QString idbudget) {
    vaciar();
    fprintf(stderr,"ListLinPedidoProveedor::cargaListLinPedidoProveedor(%s)\n", idbudget.ascii());
    mdb_idpedidoproveedor = idbudget;
    fprintf(stderr,"Hacemos la carga del cursor\n");
    cursor2 * cur= companyact->cargacursor("SELECT * FROM lpedidoproveedor, articulo WHERE idpedidoproveedor="+mdb_idpedidoproveedor+" AND articulo.idarticulo=lpedidoproveedor.idarticulo");
    int i=0;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo LinPedidoProveedor y lo agregamos a la lista.
        LinPedidoProveedor *lin = new LinPedidoProveedor(companyact,
                              cur->valor("numlpedidoproveedor"),
                              cur->valor("desclpedidoproveedor"),
                              cur->valor("cantlpedidoproveedor"),
                              cur->valor("pvplpedidoproveedor"),
                              cur->valor("prevlpedidoproveedor"),
                              cur->valor("ivalpedidoproveedor"),
                              cur->valor("descuentolpedidoproveedor"),
                              cur->valor("idpedidoproveedor"),
                              cur->valor("idarticulo"),
			      cur->valor("codigocompletoarticulo"),
			      cur->valor("nomarticulo"));
        m_lista.append(lin);
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
    fprintf(stderr,"Fin de ListLinPedidoProveedor::cargaListLinPedidoProveedor\n");
}// end chargeBudgetLines


void ListLinPedidoProveedor::guardaListLinPedidoProveedor() {
    LinPedidoProveedor *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->guardaLinPedidoProveedor();
        i++;
    }// end for
}// en guardaListLinPedidoProveedor


 float ListLinPedidoProveedor::calculabase() {
     float base=0;
     LinPedidoProveedor *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        base += linea->calculabase();
        i++;
    }// end for
    return base;
 }// end calculabase

 float ListLinPedidoProveedor::calculaiva() {
     float iva=0;
     LinPedidoProveedor *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        iva += linea->calculaiva();
        i++;
    }// end for
    return iva;
 }// end calculabase

 
void ListLinPedidoProveedor::vaciar() {
    mdb_idpedidoproveedor = "";
    m_lista.clear();
}// end guardaListLinPedidoProveedor


void ListLinPedidoProveedor::borrar() {
    if (mdb_idpedidoproveedor != "")  {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM lpedidoproveedor WHERE idpedidoproveedor="+mdb_idpedidoproveedor);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
}// end borrar


void ListLinPedidoProveedor::borraLinPedidoProveedor(int pos) {
    LinPedidoProveedor *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintaListLinPedidoProveedor();
}// end borraLinPedidoProveedor
