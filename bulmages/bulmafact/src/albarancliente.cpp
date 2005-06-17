//
// C++ Implementation: albarancliente
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "albarancliente.h"

#include <qfile.h>

typedef QMap<QString, float> base;


AlbaranCliente::AlbaranCliente(company *comp) {
	companyact=comp;
}


AlbaranCliente::~AlbaranCliente()
{
}

void AlbaranCliente::borraAlbaranCliente() {
    if (mdb_idalbaran != "") {
        listalineas->borrar();
	listadescuentos->borrar();
        companyact->begin();
        companyact->ejecuta("DELETE FROM albaran WHERE idalbaran="+mdb_idalbaran);
        companyact->commit();
        vaciaAlbaranCliente();
        pintaAlbaranCliente();
    }// end if
}// end borraAlbaranCliente


void AlbaranCliente::vaciaAlbaranCliente() {
    mdb_idalbaran="";
    mdb_numalbaran="";
    mdb_fechaalbaran="";
    mdb_loginusuario="";
    mdb_comentalbaran="";
    mdb_idcliente="";
    mdb_idforma_pago="";
    mdb_idalmacen="";
    mdb_numfactura="";
    mdb_descalbaran="";
    mdb_refalbaran="";
}// end vaciaAlbaranCliente


void AlbaranCliente::pintaAlbaranCliente() {
    fprintf(stderr,"pintaAlbaranCliente\n");
    pintaIdAlbaran(mdb_idalbaran);
    pintaNumAlbaran(mdb_numalbaran);
    pintafechaalbaran(mdb_fechaalbaran);
    pintaloginusuario(mdb_loginusuario);
    pintaComentAlbaran(mdb_comentalbaran);
    pintaidcliente(mdb_idcliente);
    pintaidforma_pago(mdb_idforma_pago);
    pintaidalmacen(mdb_idalmacen);
    pintarefalbaran(mdb_refalbaran);
    pintadescalbaran(mdb_descalbaran);
    /// Pinta el subformulario de detalle del AlbaranCliente.
    listalineas->pintaListLinAlbaranCliente();
    listadescuentos->pintaListDescuentoAlbaranCliente();
    /// Pintamos los totales
    calculaypintatotales();
}// end pintaAlbaranCliente



// Esta funciï¿½ carga un AlbaranCliente.
void AlbaranCliente::cargaAlbaranCliente(QString idbudget) {
    fprintf(stderr,"AlbaranCliente::cargaAlbaranCliente(%s)\n",idbudget.ascii());
    mdb_idalbaran = idbudget;
    QString query = "SELECT * FROM albaran WHERE idalbaran="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        mdb_idcliente= cur->valor("idcliente");
        mdb_idalmacen= cur->valor("idalmacen");
        mdb_numalbaran= cur->valor("numalbaran");
        mdb_fechaalbaran= cur->valor("fechaalbaran");
        mdb_comentalbaran= cur->valor("comentalbaran");
        mdb_loginusuario = cur->valor("loginusuario");
	mdb_idforma_pago = cur->valor("idforma_pago");
	mdb_descalbaran = cur->valor("descalbaran");
	mdb_refalbaran = cur->valor("refalbaran");
    }// end if
    delete cur;
    listalineas->cargaListLinAlbaranCliente(idbudget);
    listadescuentos->cargaDescuentos(idbudget);
    pintaAlbaranCliente();
}// end chargeBudget



void AlbaranCliente::guardaAlbaranCliente() {
    /// Todo el guardado es una transacción
    companyact->begin();
    if (mdb_loginusuario=="")
    	mdb_loginusuario="NULL";
    else
    	mdb_loginusuario="'"+mdb_loginusuario+"'";
    if (mdb_numalbaran=="") 
    	mdb_numalbaran="NULL";
    if (mdb_idalbaran == "") {
        /// Se trata de una inserción
        QString SQLQuery = "INSERT INTO albaran (numalbaran, fechaalbaran, loginusuario, comentalbaran, idcliente, idforma_pago, idalmacen, descalbaran, refalbaran) VALUES ("+mdb_numalbaran+",'"+mdb_fechaalbaran+"',"+mdb_loginusuario+",'"+mdb_comentalbaran+"',"+mdb_idcliente+","+mdb_idforma_pago+","+mdb_idalmacen+",'"+mdb_descalbaran+"','"+mdb_refalbaran+"')";
        companyact->ejecuta(SQLQuery);
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idalbaran) AS m FROM albaran");
        if (!cur->eof())
            setidalbaran(cur->valor("m"));
        delete cur;
    } else {
        /// Se trata de una modificacion
        QString SQLQuery = "UPDATE albaran SET ";
        SQLQuery += " numalbaran="+mdb_numalbaran;
        SQLQuery += " ,fechaalbaran='"+mdb_fechaalbaran+"'";
        SQLQuery += " ,loginusuario="+mdb_loginusuario+"";
        SQLQuery += " ,comentalbaran='"+mdb_comentalbaran+"'";
        SQLQuery += " ,idcliente="+mdb_idcliente+"";
        SQLQuery += " ,idforma_pago="+mdb_idforma_pago;
        SQLQuery += " ,idalmacen="+mdb_idalmacen;
	SQLQuery += " ,descalbaran='"+mdb_descalbaran+"'";
	SQLQuery += " ,refalbaran='"+mdb_refalbaran+"'";
        SQLQuery += " WHERE idalbaran="+mdb_idalbaran;
        companyact->ejecuta(SQLQuery);
    }// end if
       companyact->commit();
       listalineas->guardaListLinAlbaranCliente();
       listadescuentos->guardaListDescuentoAlbaranCliente();
       cargaAlbaranCliente(mdb_idalbaran);
}// end guardaAlbaranCliente


void AlbaranCliente::imprimirAlbaranCliente() {
} //end imprimirAlbaranCliente



void AlbaranCliente::calculaypintatotales() {
    base basesimp;
    LinAlbaranCliente *linea;
    /// Impresión de los contenidos
    QString l;

    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
    	float base = linea->cantlalbaran().toFloat() * linea->pvplalbaran().toFloat();
        basesimp[linea->ivalalbaran()] +=  base - base * linea->descontlalbaran().toFloat()/100;
    }// end for
    float basei=0;
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei+=it.data();
    }// end for
    
    
    /// Impresión de los descuentos
    float porcentt=0;
    DescuentoAlbaranCliente *linea1;
    if (listadescuentos->m_lista.first()) {
        for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
            porcentt += linea1->proporciondalbaran().toFloat();
        }// end for	
    }// end if


    float totbaseimp=0;
    float parbaseimp=0;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        if (porcentt > 0.01) {
            parbaseimp = it.data()-it.data()*porcentt/100;
            totbaseimp += parbaseimp;
        } else {
            parbaseimp = it.data();
            totbaseimp += parbaseimp;
        }// end if
    }// end for

    float totiva=0;
    float pariva=0;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        if (porcentt > 0.01) {
            pariva = (it.data()-it.data()*porcentt/100)*it.key().toFloat()/100;
            totiva += pariva;
        } else {
            pariva = it.data()*it.key().toFloat()/100;
            totiva += pariva;
        }// end if
    }// end for
    pintatotales(totiva, totbaseimp, totiva+totbaseimp, basei*porcentt/100);
}// end calculaypintatotales


