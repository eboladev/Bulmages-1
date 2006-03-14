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
#include "comparticulolist.h"
#include "company.h"
#include "comparticulo.h"


ListCompArticulo::ListCompArticulo(company *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idarticulo="";
}// end ListCompArticulo


ListCompArticulo::ListCompArticulo() {
    	   _depura("Constructor de ListCompArticulo\n",0);
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
void ListCompArticulo::cargar(QString idarticulo) {
    vaciar();
    _depura("ListCompArticulo::cargaListCompArticulo\n",0);
    mdb_idarticulo = idarticulo;
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
    _depura("END ListCompArticulo::cargar");
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


void ListCompArticulo::borrar(int pos) {
    CompArticulo *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintar();
}// end borraCompArticulo
