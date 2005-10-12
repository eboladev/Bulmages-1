//
// C++ Implementation: ListLinAlbaranProveedor
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listlinalbaranproveedor.h"
#include "company.h"
#include "linalbaranproveedor.h"


ListLinAlbaranProveedor::ListLinAlbaranProveedor(company *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idalbaranp="";
}// end ListLinAlbaranProveedor

ListLinAlbaranProveedor::ListLinAlbaranProveedor() {
    	   fprintf(stderr,"Constructor de ListLinAlbaranProveedor\n");
           companyact=NULL;
           m_lista.setAutoDelete(TRUE);
           mdb_idalbaranp="";
}// end ListLinAlbaranProveedor

ListLinAlbaranProveedor::~ListLinAlbaranProveedor() {}


void ListLinAlbaranProveedor::nuevalinea(QString desclalbaranp, QString cantlalbaranp, QString pvplalbaranp, QString descontlalbaranp, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString ivalalbaranp) {
        LinAlbaranProveedor *lin = new LinAlbaranProveedor(companyact,
                              "",
                              desclalbaranp,
                              cantlalbaranp,
                              pvplalbaranp,
                              descontlalbaranp,
                              idarticulo,
                              codigocompletoarticulo,
                              nomarticulo,
                              mdb_idalbaranp,
			      ivalalbaranp
			      );
	m_lista.append(lin);
}// end nuevalinea

LinAlbaranProveedor *ListLinAlbaranProveedor::linpos(int pos) {
	return (m_lista.at(pos));
}// end linpos


// Carga l�eas de Factura
void ListLinAlbaranProveedor::cargaListLinAlbaranProveedor(QString idbudget) {
    vaciar();
    fprintf(stderr,"ListLinAlbaranProveedor::cargaListLinAlbaranProveedor(%s)\n", idbudget.ascii());
    mdb_idalbaranp = idbudget;
    fprintf(stderr,"Hacemos la carga del cursor\n");
    cursor2 * cur= companyact->cargacursor("SELECT * FROM lalbaranp, articulo WHERE idalbaranp="+mdb_idalbaranp+" AND articulo.idarticulo=lalbaranp.idarticulo");
    int i=0;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo LinAlbaran y lo agregamos a la lista.
        LinAlbaranProveedor *lin = new LinAlbaranProveedor(companyact,
                              cur->valor("numlalbaranp"),
                              cur->valor("desclalbaranp"),
                              cur->valor("cantlalbaranp"),
                              cur->valor("pvplalbaranp"),
                              cur->valor("descontlalbaranp"),
                              cur->valor("idarticulo"),
                              cur->valor("codigocompletoarticulo"),
                              cur->valor("nomarticulo"),
			      cur->valor("idalbaranp"),
			      cur->valor("ivalalbaranp"));
        m_lista.append(lin);
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
    fprintf(stderr,"Fin de ListLinAlbaranProveedor::cargaListLinAlbaranProveedor\n");
}// end chargeBudgetLines


void ListLinAlbaranProveedor::guardaListLinAlbaranProveedor() {
    LinAlbaranProveedor *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->guardaLinAlbaranProveedor();
        i++;
    }// end for
}// en guardaListLinAlbaranProveedor


 float ListLinAlbaranProveedor::calculabase() {
     float base=0;
     LinAlbaranProveedor *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        base += linea->calculabase();
        i++;
    }// end for
    return base;
 }// end calculabase

 float ListLinAlbaranProveedor::calculaiva() {
     float iva=0;
     LinAlbaranProveedor *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        iva += linea->calculaiva();
        i++;
    }// end for
    return iva;
 }// end calculabase

 
void ListLinAlbaranProveedor::vaciar() {
    mdb_idalbaranp = "";
    m_lista.clear();
}// end guardaListLinAlbaranProveedor


void ListLinAlbaranProveedor::borrar() {
    if (mdb_idalbaranp != "")  {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM lalbaranp WHERE idalbaranp="+mdb_idalbaranp);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
}// end borrar


void ListLinAlbaranProveedor::borraLinAlbaranProveedor(int pos) {
    LinAlbaranProveedor *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintaListLinAlbaranProveedor();
}// end borraLinAlbaran
