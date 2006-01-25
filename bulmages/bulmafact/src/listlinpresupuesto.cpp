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
#include "funcaux.h"

listlinpresupuesto::listlinpresupuesto(company *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idpresupuesto="";
}// end listlinpresupuesto

listlinpresupuesto::listlinpresupuesto() {
    	   fprintf(stderr,"Constructor de listlinpresupuesto\n");
           companyact=NULL;
           m_lista.setAutoDelete(TRUE);
           mdb_idpresupuesto="";
}// end listlinpresupuesto

listlinpresupuesto::~listlinpresupuesto() {}


void listlinpresupuesto::nuevalinea(QString desc, QString cantl, QString pvpl, QString descl, QString idart, QString codart, QString nomart, QString ivapres) {
        linpresupuesto *lin = new linpresupuesto(companyact,
                              "",
                              desc,
                              cantl,
                              pvpl,
                              descl,
                              mdb_idpresupuesto,
                              idart,
                              codart,
                              nomart,
			      ivapres);
	m_lista.append(lin);
}// end nuevalinea

linpresupuesto *listlinpresupuesto::linpos(int pos) {
	return (m_lista.at(pos));
}// end linpos


// Carga lineas de presupuesto
int listlinpresupuesto::chargeBudgetLines(QString idbudget) {
	int error=0;
    vaciar();
    _depura("listlinpresupuesto::chargeBudgetLines\n",0);
    mdb_idpresupuesto = idbudget;

    cursor2 * cur= companyact->cargacursor("SELECT * FROM lpresupuesto, articulo WHERE idpresupuesto="+idbudget+" AND articulo.idarticulo=lpresupuesto.idarticulo ORDER BY idlpresupuesto");
    int i=0;
	if (cur->error()) error=1;
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
                              cur->valor("nomarticulo"),
			      cur->valor("ivalpresupuesto")
                                                );
        m_lista.append(lin);
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;

    /// Tratamiento de excepciones
    if (error) {
        _depura("Error en la carga de la linea de presupuesto\n",0);
        return 1;
    }// end if

    _depura("Fin de listlinpresupuesto::chargeBudgetLines\n",0);
	return 0;
}// end chargeBudgetLines


void listlinpresupuesto::guardalistlinpresupuesto() {
    linpresupuesto *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->guardalinpresupuesto();
        i++;
    }// end for
}// en guardalistlinpresupuesto

 float listlinpresupuesto::calculabase() {
     float base=0;
     linpresupuesto *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        base += linea->calculabase();
        i++;
    }// end for
    return base;
 }// end calculabase

 float listlinpresupuesto::calculaiva() {
     float iva=0;
     linpresupuesto *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        iva += linea->calculaiva();
        i++;
    }// end for
    return iva;
 }// end calculabase

 
void listlinpresupuesto::vaciar() {
    mdb_idpresupuesto = "";
    m_lista.clear();
}// end guardalistlinpresupuesto


void listlinpresupuesto::borrar() {
    if (mdb_idpresupuesto != "")  {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM lpresupuesto WHERE idpresupuesto="+mdb_idpresupuesto);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
}// end borrar


void listlinpresupuesto::borralinpresupuesto(int pos) {
    linpresupuesto *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintalistlinpresupuesto();
}// end borralinpresupuesto
