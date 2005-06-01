//
// C++ Implementation: listlinpresupuesto
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listlinpresupuesto.h"
#include "company.h"
#include "linpresupuesto.h"


listlinpresupuesto::listlinpresupuesto(company *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idpresupuesto="";
}// end listlinpresupuesto

listlinpresupuesto::~listlinpresupuesto() {}


void listlinpresupuesto::nuevalinea(QString desc, QString cantl, QString pvpl, QString descl, QString idart, QString codart, QString nomart) {
        linpresupuesto *lin = new linpresupuesto(companyact,
                              "",
                              desc,
                              cantl,
                              pvpl,
                              descl,
                              mdb_idpresupuesto,
                              idart,
                              codart,
                              nomart);
	m_lista.append(lin);
}// end nuevalinea

linpresupuesto *listlinpresupuesto::linpos(int pos) {
	return (m_lista.at(pos));
}// end linpos


// Carga l�eas de presupuesto
void listlinpresupuesto::chargeBudgetLines(QString idbudget) {
    mdb_idpresupuesto = idbudget;
    companyact->begin();
    cursor2 * cur= companyact->cargacursor("SELECT * FROM lpresupuesto, articulo WHERE idpresupuesto="+idbudget+" AND articulo.idarticulo=lpresupuesto.idarticulo","unquery");
    companyact->commit();
    int i=0;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo linpresupuesto y lo agregamos a la lista.
        linpresupuesto *lin = new linpresupuesto(companyact,
                              cur->valor("idlpresupuesto"),
                              cur->valor("desclpresupuesto"),
                              cur->valor("cantlpresupuesto"),
                              cur->valor("pvplpresupuesto"),
                              cur->valor("descuentolpresupuesto"),
                              cur->valor("idpresupuesto"),
                              cur->valor("idarticulo"),
                              cur->valor("codigocompletoarticulo"),
                              cur->valor("nomarticulo")
                                                );
        m_lista.append(lin);
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
}// end chargeBudgetLines


void listlinpresupuesto::guardalistlinpresupuesto() {
    linpresupuesto *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->guardalinpresupuesto();
        i++;
    }// end for
}// en guardalistlinpresupuesto


void listlinpresupuesto::vaciar() {
    mdb_idpresupuesto = "";
    m_lista.clear();
}// end guardalistlinpresupuesto


void listlinpresupuesto::borrar() {
    if (mdb_idpresupuesto != "")  {
        companyact->begin();
        companyact->ejecuta("DELETE FROM lpresupuesto WHERE idpresupuesto="+mdb_idpresupuesto);
        companyact->commit();
    }// end if
}// end borrar
