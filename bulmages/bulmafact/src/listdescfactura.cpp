//
// C++ Implementation: ListDescuentoFactura
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listdescfactura.h"
#include "company.h"
#include "descfactura.h"


ListDescuentoFactura::ListDescuentoFactura(company *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idfactura="";
}// end ListDescuentoFactura

ListDescuentoFactura::ListDescuentoFactura() {
    	   fprintf(stderr,"Constructor de ListDescuentoFactura\n");
           companyact=NULL;
           m_lista.setAutoDelete(TRUE);
           mdb_idfactura="";
}// end ListDescuentoFactura

ListDescuentoFactura::~ListDescuentoFactura() {}


void ListDescuentoFactura::nuevalinea(QString concept, QString propor) {
        DescuentoFactura *lin = new DescuentoFactura(companyact,
                              "",
                              concept,
                              propor,
                              mdb_idfactura);
	m_lista.append(lin);
}// end nuevalinea

DescuentoFactura *ListDescuentoFactura::linpos(int pos) {
	return (m_lista.at(pos));
}// end linpos


// Carga l�eas de factura
void ListDescuentoFactura::cargaDescuentos(QString idbudget) {
    vaciar();
    fprintf(stderr,"ListDescuentoFactura::chargeBudgetLines\n");
    mdb_idfactura = idbudget;
    fprintf(stderr,"Hacemos la carga del cursor\n");
    cursor2 * cur= companyact->cargacursor("SELECT * FROM dfactura WHERE idfactura="+idbudget,"unquery");
    int i=0;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo DescuentoFactura y lo agregamos a la lista.
        DescuentoFactura *lin = new DescuentoFactura(companyact,
                              cur->valor("iddfactura"),
                              cur->valor("conceptdfactura"),
                              cur->valor("proporciondfactura"),
                              cur->valor("idfactura")
                   );
        m_lista.append(lin);
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
    fprintf(stderr,"Fin de ListDescuentoFactura::chargeBudgetLines\n");
}// end chargeBudgetLines


void ListDescuentoFactura::guardaListDescuentoFactura() {
    DescuentoFactura *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->guardaDescuentoFactura();
        i++;
    }// end for
}// en guardaListDescuentoFactura


 
void ListDescuentoFactura::vaciar() {
    mdb_idfactura = "";
    m_lista.clear();
}// end guardaListDescuentoFactura


void ListDescuentoFactura::borrar() {
    if (mdb_idfactura != "")  {
        companyact->begin();
        companyact->ejecuta("DELETE FROM dfactura WHERE idfactura="+mdb_idfactura);
        companyact->commit();
    }// end if
}// end borrar


void ListDescuentoFactura::borraDescuentoFactura(int pos) {
    DescuentoFactura *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintaListDescuentoFactura();
}// end borraDescuentoFactura
