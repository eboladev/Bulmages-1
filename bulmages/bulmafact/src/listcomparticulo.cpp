//
// C++ Implementation: ListCompArticulo
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listcomparticulo.h"
#include "company.h"
#include "comparticulo.h"


ListCompArticulo::ListCompArticulo(company *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idarticulo="";
}// end ListCompArticulo

ListCompArticulo::ListCompArticulo() {
    	   fprintf(stderr,"Constructor de ListCompArticulo\n");
           companyact=NULL;
           m_lista.setAutoDelete(TRUE);
           mdb_idarticulo="";
}// end ListCompArticulo

ListCompArticulo::~ListCompArticulo() {}


void ListCompArticulo::nuevalinea(QString idcomponente, QString cantcomparticulo, QString codigocompleto, QString nombre) {
        CompArticulo *lin = new CompArticulo(companyact,
                              mdb_idarticulo,
                              idcomponente,
                              cantcomparticulo,
                              codigocompleto,
                              nombre);
	m_lista.append(lin);
}// end nuevalinea

CompArticulo *ListCompArticulo::linpos(int pos) {
	return (m_lista.at(pos));
}// end linpos


// Carga l�eas de presupuesto
void ListCompArticulo::cargaListCompArticulo(QString idarticulo) {
    vaciar();
    fprintf(stderr,"ListCompArticulo::cargaListCompArticulo\n");
    mdb_idarticulo = idarticulo;
    fprintf(stderr,"Hacemos la carga del cursor\n");
    cursor2 * cur= companyact->cargacursor("SELECT * FROM comparticulo, articulo WHERE comparticulo.idarticulo="+mdb_idarticulo+" AND articulo.idarticulo=comparticulo.idcomponente");
    int i=0;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo CompArticulo y lo agregamos a la lista.
        CompArticulo *lin = new CompArticulo(companyact,
                              cur->valor("idarticulo"),
                              cur->valor("idcomponente"),
                              cur->valor("cantcomparticulo"),
                              cur->valor("codigocompletoarticulo"),
                              cur->valor("nomarticulo")
                                                );
        m_lista.append(lin);
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
    fprintf(stderr,"Fin de ListCompArticulo::cargaListCompArticulo\n");
}// end chargeBudgetLines


void ListCompArticulo::guardaListCompArticulo() {
    CompArticulo *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->guardaCompArticulo();
        i++;
    }// end for
}// en guardaListCompArticulo

 
void ListCompArticulo::vaciar() {
    mdb_idarticulo = "";
    m_lista.clear();
}// end guardaListCompArticulo


void ListCompArticulo::borrar() {
    if (mdb_idarticulo != "")  {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM comparticulo WHERE idarticulo="+mdb_idarticulo);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
}// end borrar


void ListCompArticulo::borraCompArticulo(int pos) {
    CompArticulo *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintaListCompArticulo();
}// end borraCompArticulo
