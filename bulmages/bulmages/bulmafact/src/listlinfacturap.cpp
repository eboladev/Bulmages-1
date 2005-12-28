//
// C++ Implementation: ListLinFacturaProveedor
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listlinfacturap.h"
#include "company.h"
#include "linfacturap.h"


ListLinFacturaProveedor::ListLinFacturaProveedor(company *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idfacturap="";
}// end ListLinFacturaProveedor

ListLinFacturaProveedor::ListLinFacturaProveedor() {
    	   fprintf(stderr,"Constructor de ListLinFacturaProveedor\n");
           companyact=NULL;
           m_lista.setAutoDelete(TRUE);
           mdb_idfacturap="";
}// end ListLinFacturaProveedor

ListLinFacturaProveedor::~ListLinFacturaProveedor() {}


void ListLinFacturaProveedor::nuevalinea(QString desclfacturap, QString cantlfacturap, QString pvplfacturap, QString descuentolfacturap, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString ivalfacturap) {
        LinFacturaProveedor *lin = new LinFacturaProveedor(companyact,
                              "",
                              desclfacturap,
                              cantlfacturap,
                              pvplfacturap,
                              descuentolfacturap,
                              mdb_idfacturap,
                              idarticulo,
                              codigocompletoarticulo,
                              nomarticulo,
			      ivalfacturap
			      );
	m_lista.append(lin);
}// end nuevalinea

LinFacturaProveedor *ListLinFacturaProveedor::linpos(int pos) {
	return (m_lista.at(pos));
}// end linpos


// Carga l�eas de FacturaProveedor
void ListLinFacturaProveedor::cargaListLinFacturaProveedor(QString idbudget) {
    fprintf(stderr,"ListLinFacturaProveedor::cargaListLinFacturaProveedor(%s)\n", idbudget.ascii());
    vaciar();
    mdb_idfacturap = idbudget;
    fprintf(stderr,"Hacemos la carga del cursor\n");
    cursor2 * cur= companyact->cargacursor("SELECT * FROM lfacturap, articulo WHERE idfacturap="+idbudget+" AND articulo.idarticulo=lfacturap.idarticulo");
    int i=0;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo LinFacturaProveedor y lo agregamos a la lista.
        LinFacturaProveedor *lin = new LinFacturaProveedor(companyact,
                              cur->valor("idlfacturap"),
                              cur->valor("desclfacturap"),
                              cur->valor("cantlfacturap"),
                              cur->valor("pvplfacturap"),
                              cur->valor("descuentolfacturap"),
                              cur->valor("idfacturap"),
                              cur->valor("idarticulo"),
                              cur->valor("codigocompletoarticulo"),
                              cur->valor("nomarticulo"),
			      cur->valor("ivalfacturap"));
        m_lista.append(lin);
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
    fprintf(stderr,"Fin de ListLinFacturaProveedor::cargaListLinFacturaProveedor\n");
}// end chargeBudgetLines


void ListLinFacturaProveedor::guardaListLinFacturaProveedor() {
    LinFacturaProveedor *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->guardaLinFacturaProveedor();
        i++;
    }// end for
}// en guardaListLinFacturaProveedor


 float ListLinFacturaProveedor::calculabase() {
     float base=0;
     LinFacturaProveedor *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        base += linea->calculabase();
        i++;
    }// end for
    return base;
 }// end calculabase

 float ListLinFacturaProveedor::calculaiva() {
     float iva=0;
     LinFacturaProveedor *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        iva += linea->calculaiva();
        i++;
    }// end for
    return iva;
 }// end calculabase

 
void ListLinFacturaProveedor::vaciar() {
    mdb_idfacturap = "";
    m_lista.clear();
}// end guardaListLinFacturaProveedor


void ListLinFacturaProveedor::borrar() {
    if (mdb_idfacturap != "")  {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM lfacturap WHERE idfacturap="+mdb_idfacturap);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
}// end borrar


void ListLinFacturaProveedor::borraLinFacturaProveedor(int pos) {
    LinFacturaProveedor *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintaListLinFacturaProveedor();
}// end borraLinFacturaProveedor
