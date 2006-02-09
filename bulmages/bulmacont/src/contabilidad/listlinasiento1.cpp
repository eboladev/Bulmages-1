//
// C++ Implementation: ListLinAsiento1
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listlinasiento1.h"
#include "empresa.h"


ListLinAsiento1::ListLinAsiento1(empresa *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idasiento="";
}// end ListLinAsiento1

ListLinAsiento1::ListLinAsiento1() {
    _depura("Constructor de ListLinAsiento1\n",0);
    companyact=NULL;
    m_lista.setAutoDelete(TRUE);
    mdb_idasiento="";
}// end ListLinAsiento1

ListLinAsiento1::~ListLinAsiento1() {}


LinAsiento1 *ListLinAsiento1::linpos(int pos) {
    return (m_lista.at(pos));
}// end linFactura


// Carga l�eas de Factura
void ListLinAsiento1::cargaListLinAsiento1(QString idbudget) {
    _depura("ListLinAsiento1::cargaListLinAsiento1()\n",0);
    vaciar();
    mdb_idasiento = idbudget;
    QString SQLQuery = "SELECT * FROM borrador ";
    SQLQuery+= " LEFT JOIN (SELECT codigo, descripcion AS descripcioncuenta, idcuenta  FROM  cuenta) AS t1 ON t1.idcuenta=borrador.idcuenta ";
    SQLQuery += " LEFT JOIN (SELECT idcanal, nombre AS nombrecanal, descripcion AS descripcioncanal FROM canal) AS t2 ON borrador.idcanal = t2.idcanal ";
    SQLQuery += " LEFT JOIN (SELECT idc_coste, nombre AS nombrec_coste, descripcion AS descripcionc_coste FROM c_coste) AS t3 ON borrador.idc_coste = t3.idc_coste ";
    SQLQuery+= "WHERE idasiento="+idbudget+" ORDER BY orden";
    cursor2 * cur= companyact->cargacursor(SQLQuery);
    while (!cur->eof())   {
        /// Creamos un elemento del tipo LinAsiento1 y lo agregamos a la lista.
        LinAsiento1 *lin = new LinAsiento1(companyact,cur);
        m_lista.append(lin);
        cur->siguienteregistro();
    }// end while
    delete cur;
    fprintf(stderr,"Fin de ListLinAsiento1::cargaListLinAsiento1\n");
}// end chargeBudgetLines


int ListLinAsiento1::guardaListLinAsiento1() {
    LinAsiento1 *linea;
    uint i = 0;
    int error = 0;
    Fixed descuadre=totaldebe();
    descuadre = descuadre - totalhaber();
    if (descuadre != 0) {
        _depura("Asiento descuadrado, no puede ser cerrado",2);
        return -1;
    }// end if
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->setDBvalue("orden",QString::number(i));
        error = linea->guardaLinAsiento1();
        if (error)
            return error;
        i++;
    }// end for
    return error;
}// en guardaListLinAsiento1



void ListLinAsiento1::vaciar() {
    mdb_idasiento = "";
    m_lista.clear();
}// end guardaListLinAsiento1


void ListLinAsiento1::borrar() {
    if (mdb_idasiento != "")  {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM borrador WHERE idborrador="+mdb_idasiento);
        if (error) {
            companyact->rollback();
            return;
        }// end if
        companyact->commit();
    }// end if
}// end borrar


void ListLinAsiento1::borraLinAsiento1(int pos) {
    LinAsiento1 *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintaListLinAsiento1();
}// end borraLinAsiento1


Fixed ListLinAsiento1::totaldebe() {
    LinAsiento1 *linea;
    Fixed total=Fixed("0.0");
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        total = total + linea->calculadebe();
    }// end for
    return total;
};

Fixed ListLinAsiento1::totalhaber() {
    LinAsiento1 *linea;
    Fixed total=Fixed("0.0");
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        total = total + linea->calculahaber();
    }// end for
    return total;
};
