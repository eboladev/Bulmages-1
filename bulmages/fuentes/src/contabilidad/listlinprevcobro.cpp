//
// C++ Implementation: listlinprevcobro
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listlinprevcobro.h"
#include "empresa.h"
#include "linprevcobro.h"


listlinprevcobro::listlinprevcobro(empresa *comp) {
    empresaactual = comp;
    conexionbase = comp->bdempresa();
    m_lista.setAutoDelete(TRUE);
    mdb_idregistroiva="";
}// end listlinprevcobro

listlinprevcobro::listlinprevcobro() {
    fprintf(stderr,"Constructor de listlinprevcobro\n");
    empresaactual=NULL;
    conexionbase = NULL;
    m_lista.setAutoDelete(TRUE);
    mdb_idregistroiva="";
}// end listlinprevcobro

listlinprevcobro::~listlinprevcobro() {}


void listlinprevcobro::nuevalinea(QString desc, QString cantl, QString pvpl, QString descl,QString idart, QString codart, QString nomart, QString ivapres, QString k, QString l, QString m) {
    linprevcobro *lin = new linprevcobro(empresaactual,
                                         "",
                                         desc,
                                         cantl,
                                         pvpl,
                                         descl,
                                         idart,
                                         codart,
                                         nomart,
                                         mdb_idregistroiva,
                                         ivapres,
                                         k,
                                         l,
                                         m);
    m_lista.append(lin);
}// end nuevalinea

linprevcobro *listlinprevcobro::linpos(int pos) {
    return (m_lista.at(pos));
}// end linpos


// Carga l�eas de una factura.
void listlinprevcobro::chargeBudgetLines(QString idregistroiva) {
    vaciar();
    fprintf(stderr,"listlinprevcobro::chargeBudgetLines\n");
    mdb_idregistroiva = idregistroiva;
    fprintf(stderr,"Hacemos la carga del cursor\n");
    cursor2 * cur= conexionbase->cargacursor("SELECT * FROM prevcobro, cuenta WHERE idregistroiva="+idregistroiva+" AND cuenta.idcuenta=prevcobro.idcuenta");
    int i=0;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo linprevcobro y lo agregamos a la lista.
        linprevcobro *lin = new linprevcobro(empresaactual,
                                             cur->valor("idprevcobro"),
                                             cur->valor("fprevistaprevcobro"),
                                             cur->valor("fcobroprevcobro"),
                                             cur->valor("idfpago"),
                                             cur->valor("idcuenta"),
                                             cur->valor("idasiento"),
                                             cur->valor("cantidadprevistaprevcobro"),
                                             cur->valor("cantidadprevcobro"),
                                             cur->valor("idregistroiva"),
                                             cur->valor("tipoprevcobro"),
                                             cur->valor("docprevcobro"),
                                             cur->valor("codigo"),
                                             cur->valor("descripcion")
                                            );
        m_lista.append(lin);
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
    fprintf(stderr,"Fin de listlinprevcobro::chargeBudgetLines\n");
}// end chargeBudgetLines


void listlinprevcobro::guardalistlinprevcobro() {
	fprintf(stderr,"guardalistlinprevcobro()\n");
    linprevcobro *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->guardalinprevcobro();
        i++;
    }// end for
}// en guardalistlinprevcobro


void listlinprevcobro::vaciar() {
    mdb_idregistroiva = "";
    m_lista.clear();
}// end guardalistlinprevcobro


void listlinprevcobro::borrar() {
    if (mdb_idregistroiva != "")  {
        conexionbase->begin();
        conexionbase->ejecuta("DELETE FROM prevcobro WHERE idregistroiva="+mdb_idregistroiva);
        conexionbase->commit();
    }// end if
}// end borrar


void listlinprevcobro::borralinprevcobro(int pos) {
    linprevcobro *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintalistlinprevcobro();
}// end borralinprevcobro
