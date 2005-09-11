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

void listlinprevcobro::inicializaVariables() {
    empresaactual=NULL;
    conexionbase = NULL;
    mdb_idregistroiva = "";
    mfilt_idregistroiva = "";
    mfilt_tipoprevcobro = "";
    mfilt_codigocuentaprevcobro = "";
    mfilt_finprevcobro = "";
    mfilt_ffiprevcobro = "";
    m_lista.setAutoDelete(TRUE);
}


listlinprevcobro::listlinprevcobro(empresa *comp) {
    inicializaVariables();
    empresaactual = comp;
    conexionbase = comp->bdempresa();
}// end listlinprevcobro

listlinprevcobro::listlinprevcobro() {
    inicializaVariables();
}// end listlinprevcobro

listlinprevcobro::~listlinprevcobro() {}


void listlinprevcobro::nuevalinea(QString desc, QString cantl, QString pvpl, QString descl,QString idart, QString codart, QString nomart, QString ivapres, QString k, QString l, QString m, QString idctacliente, QString codigoctacliente, QString nomctacliente) {
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
                                         m,
					idctacliente,
					codigoctacliente,
					nomctacliente);
    m_lista.append(lin);
}// end nuevalinea

linprevcobro *listlinprevcobro::linpos(int pos) {
    return (m_lista.at(pos));
}// end linpos


// Carga l�eas de una factura.
void listlinprevcobro::chargeBudgetLines() {
    QString cadwhere = "";
    vaciar();
    fprintf(stderr,"listlinprevcobro::chargeBudgetLines\n");
    fprintf(stderr,"Hacemos la carga del cursor\n");
    if (mfilt_idregistroiva != "")
        cadwhere = " AND idregistroiva = "+mfilt_idregistroiva;
    if (mfilt_finprevcobro != "")
	cadwhere += " AND fcobroprevcobro >= '"+mfilt_finprevcobro+"'";
   if ( mfilt_codigocuentaprevcobro != "")
	cadwhere += " AND idcuenta = id_cuenta('"+mfilt_codigocuentaprevcobro+"')";
   if ( mfilt_tipoprevcobro != "")
	cadwhere += " AND tipoprevcobro = '"+mfilt_tipoprevcobro+"'";
   if (mfilt_procesado == "PROCESADO")
	cadwhere += " AND idasiento IS NOT NULL ";
   if (mfilt_procesado == "NO PROCESADO")
	cadwhere += " AND idasiento IS NULL ";

    cursor2 * cur= conexionbase->cargacursor("SELECT * FROM prevcobro "
	" LEFT JOIN cuenta ON cuenta.idcuenta=prevcobro.idcuenta "
	" LEFT JOIN (SELECT idcuenta AS idctacliente, codigo AS codigoctacliente, descripcion AS nomctacliente FROM cuenta) AS T1 ON t1.idctacliente = prevcobro.idctacliente "
	" WHERE 1=1 "+ cadwhere+ " ORDER BY fcobroprevcobro ");

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
                                             cur->valor("descripcion"),
						cur->valor("idctacliente"),
						cur->valor("codigoctacliente"),
						cur->valor("nomctacliente")
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
    //    mdb_idregistroiva = "";
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
