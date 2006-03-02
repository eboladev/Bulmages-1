//
// C++ Implementation: ListLinPrevCobro
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

void ListLinPrevCobro::inicializaVariables() {
    m_companyact=NULL;
    mdb_idregistroiva = "";
    mfilt_idregistroiva = "";
    mfilt_tipoprevcobro = "";
    mfilt_codigocuentaprevcobro = "";
    mfilt_finprevcobro = "";
    mfilt_ffiprevcobro = "";
    m_lista.setAutoDelete(TRUE);
}


ListLinPrevCobro::ListLinPrevCobro(empresa *comp) {
    inicializaVariables();
    m_companyact = comp;
}// end ListLinPrevCobro

ListLinPrevCobro::ListLinPrevCobro() {
    inicializaVariables();
}// end ListLinPrevCobro

ListLinPrevCobro::~ListLinPrevCobro() {}


void ListLinPrevCobro::nuevalinea(QString desc, QString cantl, QString pvpl, QString descl,QString idart, QString codart, QString nomart, QString ivapres, QString k, QString l, QString m, QString idctacliente, QString codigoctacliente, QString nomctacliente) {
    linprevcobro *lin = new linprevcobro(m_companyact,
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

linprevcobro *ListLinPrevCobro::linpos(int pos) {
    return (m_lista.at(pos));
}// end linpos


// Carga l�eas de una factura.
int ListLinPrevCobro::chargeBudgetLines() {
    QString cadwhere = "";
    vaciar();
    fprintf(stderr,"ListLinPrevCobro::chargeBudgetLines\n");
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

    cursor2 * cur= m_companyact->cargacursor("SELECT * FROM prevcobro "
                   " LEFT JOIN cuenta ON cuenta.idcuenta=prevcobro.idcuenta "
                   " LEFT JOIN (SELECT idcuenta AS idctacliente, codigo AS codigoctacliente, descripcion AS nomctacliente FROM cuenta) AS T1 ON t1.idctacliente = prevcobro.idctacliente "
                   " WHERE 1=1 "+ cadwhere+ " ORDER BY fcobroprevcobro ");

    int i=0;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo linprevcobro y lo agregamos a la lista.
        linprevcobro *lin = new linprevcobro(m_companyact,
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
    _depura("END de ListLinPrevCobro::chargeBudgetLines\n");
    return 0;
}// end chargeBudgetLines


void ListLinPrevCobro::guardaListLinPrevCobro() {
    _depura("guardaListLinPrevCobro()");
    linprevcobro *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->guardalinprevcobro();
        i++;
    }// end for
}// en guardaListLinPrevCobro


void ListLinPrevCobro::vaciar() {
    //    mdb_idregistroiva = "";
    m_lista.clear();
}// end guardaListLinPrevCobro


void ListLinPrevCobro::borrar() {
    if (mdb_idregistroiva != "")  {
        m_companyact->begin();
        m_companyact->ejecuta("DELETE FROM prevcobro WHERE idregistroiva="+mdb_idregistroiva);
        m_companyact->commit();
    }// end if
}// end borrar


void ListLinPrevCobro::borralinprevcobro(int pos) {
    linprevcobro *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintaListLinPrevCobro();
}// end borralinprevcobro


Fixed ListLinPrevCobro::totalCobro() {
    linprevcobro *linea;
    Fixed tcobro("0");
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        if (linea->tipoprevcobro() == "t")
            tcobro = tcobro + Fixed(linea->cantidadprevcobro());
    }// end for
    return tcobro;
}// end totalCobro

Fixed ListLinPrevCobro::totalPago() {
    linprevcobro *linea;
    Fixed tpago("0");
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        if (linea->tipoprevcobro() == "f")
            tpago = tpago + Fixed(linea->cantidadprevcobro());
    }// end for
    return tpago;
}// end totalPago

