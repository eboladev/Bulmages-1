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
#include "fixed.h"


typedef QMap<QString, Fixed> base;


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
    mdb_comentalbaran="";
    mdb_idcliente="";
    mdb_idforma_pago="";
    mdb_idalmacen="";
    mdb_numfactura="";
    mdb_descalbaran="";
    mdb_refalbaran="";
    mdb_idtrabajador = "";
    mdb_contactalbaran = "";
    mdb_telalbaran = "";
    mdb_procesadoalbaran = "FALSE";
}// end vaciaAlbaranCliente


void AlbaranCliente::pintaAlbaranCliente() {
    fprintf(stderr,"pintaAlbaranCliente\n");
    pintaIdAlbaran(mdb_idalbaran);
    pintaNumAlbaran(mdb_numalbaran);
    pintafechaalbaran(mdb_fechaalbaran);
    pintaComentAlbaran(mdb_comentalbaran);
    pintaidcliente(mdb_idcliente);
    pintaidforma_pago(mdb_idforma_pago);
    pintaidalmacen(mdb_idalmacen);
    pintarefalbaran(mdb_refalbaran);
    pintadescalbaran(mdb_descalbaran);
    pintaidtrabajador(mdb_idtrabajador);
    pintacontactalbaran(mdb_contactalbaran);
    pintatelalbaran(mdb_telalbaran);
    pintaprocesadoalbaran(mdb_procesadoalbaran);
    /// Pinta el subformulario de detalle del AlbaranCliente.
    listalineas->pintaListLinAlbaranCliente();
    listadescuentos->pintaListDescuentoAlbaranCliente();
    /// Pintamos los totales
    calculaypintatotales();
}// end pintaAlbaranCliente



// Esta función carga un AlbaranCliente.
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
	mdb_idforma_pago = cur->valor("idforma_pago");
	mdb_descalbaran = cur->valor("descalbaran");
	mdb_refalbaran = cur->valor("refalbaran");
	mdb_idtrabajador = cur->valor("idtrabajador");
	mdb_contactalbaran = cur->valor("contactalbaran");
	mdb_telalbaran = cur->valor("telalbaran");
	if (cur->valor("procesadoalbaran") == "t") 
		mdb_procesadoalbaran = "TRUE";
	else
		mdb_procesadoalbaran = "FALSE";
    }// end if
    delete cur;
    listalineas->cargaListLinAlbaranCliente(idbudget);
    listadescuentos->cargaDescuentos(idbudget);
    pintaAlbaranCliente();
}// end chargeBudget



void AlbaranCliente::guardaAlbaranCliente() {
    /// Todo el guardado es una transacción
    companyact->begin();
    if (mdb_numalbaran=="") 
    	mdb_numalbaran="NULL";
    if (mdb_idtrabajador == "")
    	mdb_idtrabajador="NULL";
    if (mdb_idalbaran == "") {
        /// Se trata de una inserción
        QString SQLQuery = "INSERT INTO albaran (numalbaran, fechaalbaran, comentalbaran, idcliente, idforma_pago, idalmacen, descalbaran, refalbaran, idtrabajador, contactalbaran, telalbaran, procesadoalbaran) VALUES ("+mdb_numalbaran+",'"+mdb_fechaalbaran+"','"+mdb_comentalbaran+"',"+mdb_idcliente+","+mdb_idforma_pago+","+mdb_idalmacen+",'"+mdb_descalbaran+"','"+mdb_refalbaran+"',"+mdb_idtrabajador+",'"+mdb_contactalbaran+"','"+mdb_telalbaran+"',"+mdb_procesadoalbaran+")";
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
        SQLQuery += " ,comentalbaran='"+mdb_comentalbaran+"'";
        SQLQuery += " ,idcliente="+mdb_idcliente+"";
        SQLQuery += " ,idforma_pago="+mdb_idforma_pago;
        SQLQuery += " ,idalmacen="+mdb_idalmacen;
	SQLQuery += " ,descalbaran='"+mdb_descalbaran+"'";
	SQLQuery += " ,refalbaran='"+mdb_refalbaran+"'";
	SQLQuery += " ,idtrabajador="+mdb_idtrabajador;
	SQLQuery += " ,contactalbaran = '"+mdb_contactalbaran+"'";
	SQLQuery += " ,telalbaran = '"+mdb_telalbaran+"'";
	SQLQuery += " ,procesadoalbaran = "+mdb_procesadoalbaran;
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
    fprintf(stderr,"calculaypintatotales \n");
    base basesimp;
    LinAlbaranCliente *linea;
    /// Impresión de los contenidos
    QString l;
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
    	Fixed cant(linea->cantlalbaran().ascii());
	Fixed pvpund(linea->pvplalbaran().ascii());
	Fixed desc1(linea->descontlalbaran().ascii());
	Fixed cantpvp = cant * pvpund;
	Fixed base = cantpvp - cantpvp * desc1 / 100;
        basesimp[linea->ivalalbaran()] =  basesimp[linea->ivalalbaran()]+ base;
    }// end for
    

    Fixed basei("0.00");
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei = basei + it.data();
    }// end for
    /// Impresión de los descuentos
    Fixed porcentt("0.00");
    DescuentoAlbaranCliente *linea1;
    if (listadescuentos->m_lista.first()) {
        for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
	    Fixed propor(linea1->proporciondalbaran().ascii());
            porcentt = porcentt + propor;
        }// end for	
    }// end if


    Fixed totbaseimp("0.00");
    Fixed parbaseimp("0.00");
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        if (porcentt > Fixed("0.00") ) {
            parbaseimp = it.data()-it.data()*porcentt/100;
        } else {
            parbaseimp = it.data();
        }// end if
	totbaseimp = totbaseimp + parbaseimp;
    }// end for

    Fixed totiva("0.00");
    Fixed pariva("0.00");
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
	    Fixed piva(it.key().ascii());
        if (porcentt > Fixed("0.00")) {
            pariva = (it.data()-it.data()*porcentt/100)* piva/100;
        } else {
            pariva = it.data()* piva/100;
        }// end if
	totiva = totiva + pariva;
    }// end for
    pintatotales(totiva, totbaseimp, totiva+totbaseimp, basei*porcentt/100);

}// end calculaypintatotales


