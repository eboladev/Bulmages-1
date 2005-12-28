//
// C++ Implementation: ListDescuentoFacturaProv
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listdescfacturaprov.h"
#include "company.h"
#include "descfacturaprov.h"


ListDescuentoFacturaProv::ListDescuentoFacturaProv(company *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idfacturap="";
}// end ListDescuentoFacturaProv

ListDescuentoFacturaProv::ListDescuentoFacturaProv() {
    	   fprintf(stderr,"Constructor de ListDescuentoFacturaProv\n");
           companyact=NULL;
           m_lista.setAutoDelete(TRUE);
           mdb_idfacturap="";
}// end ListDescuentoFacturaProv

ListDescuentoFacturaProv::~ListDescuentoFacturaProv() {}


void ListDescuentoFacturaProv::nuevalinea(QString concept, QString propor) {
        DescuentoFacturaProv *lin = new DescuentoFacturaProv(companyact,
                              "",
                              concept,
                              propor,
                              mdb_idfacturap);
	m_lista.append(lin);
}// end nuevalinea

DescuentoFacturaProv *ListDescuentoFacturaProv::linpos(int pos) {
	return (m_lista.at(pos));
}// end linpos


// Carga l�eas de factura
void ListDescuentoFacturaProv::cargaDescuentos(QString idbudget) {
    vaciar();
    _depura("ListDescuentoFacturaProv::chargeBudgetLines\n");
    mdb_idfacturap = idbudget;
    _depura("Hacemos la carga del cursor\n");
    cursor2 * cur= companyact->cargacursor("SELECT * FROM dfacturap WHERE idfacturap="+idbudget);
    int i=0;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo DescuentoFactura y lo agregamos a la lista.
        DescuentoFacturaProv *lin = new DescuentoFacturaProv(companyact,
                              cur->valor("iddfacturap"),
                              cur->valor("conceptdfacturap"),
                              cur->valor("proporciondfacturap"),
                              cur->valor("idfacturap")
                   );
        m_lista.append(lin);
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
    _depura("Fin de ListDescuentoFacturaProv::chargeBudgetLines\n");
}// end chargeBudgetLines


void ListDescuentoFacturaProv::guardaListDescuentoFacturaProv() {
    DescuentoFacturaProv *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->guardaDescuentoFacturaProv();
        i++;
    }// end for
}// en guardaListDescuentoFacturaProv


 
void ListDescuentoFacturaProv::vaciar() {
    mdb_idfacturap = "";
    m_lista.clear();
}// end guardaListDescuentoFacturaProv


void ListDescuentoFacturaProv::borrar() {
    if (mdb_idfacturap != "")  {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM dfactura WHERE idfactura="+mdb_idfacturap);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
}// end borrar


void ListDescuentoFacturaProv::borraDescuentoFacturaProv(int pos) {
    DescuentoFacturaProv *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintaListDescuentoFacturaProv();
}// end borraDescuentoFactura
