//
// C++ Implementation: ListDescuentoAlbaranProv
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listdescalbaranprov.h"
#include "company.h"
#include "descalbaranprov.h"


ListDescuentoAlbaranProv::ListDescuentoAlbaranProv(company *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idalbaranp="";
}// end ListDescuentoAlbaranProv

ListDescuentoAlbaranProv::ListDescuentoAlbaranProv() {
    	   fprintf(stderr,"Constructor de ListDescuentoAlbaranProv\n");
           companyact=NULL;
           m_lista.setAutoDelete(TRUE);
           mdb_idalbaranp="";
}// end ListDescuentoAlbaranProv

ListDescuentoAlbaranProv::~ListDescuentoAlbaranProv() {}


void ListDescuentoAlbaranProv::nuevalinea(QString concept, QString propor) {
        DescuentoAlbaranProv *lin = new DescuentoAlbaranProv(companyact,
                              "",
                              concept,
                              propor,
                              mdb_idalbaranp);
	m_lista.append(lin);
}// end nuevalinea

DescuentoAlbaranProv *ListDescuentoAlbaranProv::linpos(int pos) {
	return (m_lista.at(pos));
}// end linpos


// Carga l�eas de albaran
void ListDescuentoAlbaranProv::cargaDescuentos(QString idbudget) {
    vaciar();
    _depura("ListDescuentoAlbaranProv::chargeBudgetLines\n");
    mdb_idalbaranp = idbudget;
    _depura("Hacemos la carga del cursor\n");
    cursor2 * cur= companyact->cargacursor("SELECT * FROM dalbaranp WHERE idalbaranp="+idbudget);
    int i=0;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo DescuentoAlbaranProv y lo agregamos a la lista.
        DescuentoAlbaranProv *lin = new DescuentoAlbaranProv(companyact,
                              cur->valor("iddalbaranp"),
                              cur->valor("conceptdalbaranp"),
                              cur->valor("proporciondalbaranp"),
                              cur->valor("idalbaranp")
                   );
        m_lista.append(lin);
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
    _depura("Fin de ListDescuentoAlbaranProv::chargeBudgetLines\n");
}// end chargeBudgetLines


void ListDescuentoAlbaranProv::guardaListDescuentoAlbaranProv() {
    DescuentoAlbaranProv *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->guardaDescuentoAlbaranProv();
        i++;
    }// end for
}// en guardaListDescuentoAlbaranProv


 
void ListDescuentoAlbaranProv::vaciar() {
    mdb_idalbaranp = "";
    m_lista.clear();
}// end guardaListDescuentoAlbaranProv


void ListDescuentoAlbaranProv::borrar() {
    if (mdb_idalbaranp != "")  {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM dalbaranp WHERE idalbaranp="+mdb_idalbaranp);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
}// end borrar


void ListDescuentoAlbaranProv::borraDescuentoAlbaranProv(int pos) {
    DescuentoAlbaranProv *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintaListDescuentoAlbaranProv();
}// end borraDescuentoAlbaranProv
