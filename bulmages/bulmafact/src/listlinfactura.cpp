//
// C++ Implementation: ListLinFactura
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listlinfactura.h"
#include "company.h"
#include "linfactura.h"


ListLinFactura::ListLinFactura(company *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idfactura="";
}// end ListLinFactura

ListLinFactura::ListLinFactura() {
    	   fprintf(stderr,"Constructor de ListLinFactura\n");
           companyact=NULL;
           m_lista.setAutoDelete(TRUE);
           mdb_idfactura="";
}// end ListLinFactura

ListLinFactura::~ListLinFactura() {}


void ListLinFactura::nuevalinea(QString desclfactura, QString cantlfactura, QString pvplfactura, QString descuentolfactura, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString ivalfactura) {
        LinFactura *lin = new LinFactura(companyact,
                              "",
                              desclfactura,
                              cantlfactura,
                              pvplfactura,
                              descuentolfactura,
                              mdb_idfactura,
                              idarticulo,
                              codigocompletoarticulo,
                              nomarticulo,
			      ivalfactura
			      );
	m_lista.append(lin);
}// end nuevalinea

LinFactura *ListLinFactura::linpos(int pos) {
	return (m_lista.at(pos));
}// end linpos


// Carga l�eas de Factura
void ListLinFactura::cargaListLinFactura(QString idbudget) {
    fprintf(stderr,"ListLinFactura::cargaListLinFactura(%s)\n", idbudget.ascii());
    vaciar();
    mdb_idfactura = idbudget;
    fprintf(stderr,"Hacemos la carga del cursor\n");
    cursor2 * cur= companyact->cargacursor("SELECT * FROM lfactura, articulo WHERE idfactura="+idbudget+" AND articulo.idarticulo=lfactura.idarticulo ORDER BY idlfactura");
    int i=0;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo LinFactura y lo agregamos a la lista.
        LinFactura *lin = new LinFactura(companyact,
                              cur->valor("idlfactura"),
                              cur->valor("desclfactura"),
                              cur->valor("cantlfactura"),
                              cur->valor("pvplfactura"),
                              cur->valor("descuentolfactura"),
                              cur->valor("idfactura"),
                              cur->valor("idarticulo"),
                              cur->valor("codigocompletoarticulo"),
                              cur->valor("nomarticulo"),
			      cur->valor("ivalfactura"));
        m_lista.append(lin);
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
    fprintf(stderr,"Fin de ListLinFactura::cargaListLinFactura\n");
}// end chargeBudgetLines


void ListLinFactura::guardaListLinFactura() {
    LinFactura *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->guardaLinFactura();
        i++;
    }// end for
}// en guardaListLinFactura


 float ListLinFactura::calculabase() {
     float base=0;
     LinFactura *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        base += linea->calculabase();
        i++;
    }// end for
    return base;
 }// end calculabase

 float ListLinFactura::calculaiva() {
     float iva=0;
     LinFactura *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        iva += linea->calculaiva();
        i++;
    }// end for
    return iva;
 }// end calculabase

 
void ListLinFactura::vaciar() {
    mdb_idfactura = "";
    m_lista.clear();
}// end guardaListLinFactura


void ListLinFactura::borrar() {
    if (mdb_idfactura != "")  {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM lfactura WHERE idfactura="+mdb_idfactura);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
}// end borrar


void ListLinFactura::borraLinFactura(int pos) {
    LinFactura *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintaListLinFactura();
}// end borraLinFactura
