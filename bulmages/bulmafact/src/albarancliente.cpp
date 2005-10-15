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
        int error = companyact->ejecuta("DELETE FROM albaran WHERE idalbaran="+mdb_idalbaran);
	if (error) {
		companyact->rollback();
		return;
	}// end if
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



// Esta funci� carga un AlbaranCliente.
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
    /// Todo el guardado es una transacci�
    companyact->begin();
    if (mdb_numalbaran=="") 
    	mdb_numalbaran="NULL";
    if (mdb_idtrabajador == "")
    	mdb_idtrabajador="NULL";
    if (mdb_procesadoalbaran == "")
	mdb_procesadoalbaran = "FALSE";
    if (mdb_idalbaran == "") {
        /// Se trata de una inserci�
        QString SQLQuery = "INSERT INTO albaran (numalbaran, fechaalbaran, comentalbaran, idcliente, idforma_pago, idalmacen, descalbaran, refalbaran, idtrabajador, contactalbaran, telalbaran, procesadoalbaran) VALUES ("+
	companyact->sanearCadena(mdb_numalbaran)+",'"+
	companyact->sanearCadena(mdb_fechaalbaran)+"','"+
	companyact->sanearCadena(mdb_comentalbaran)+"',"+
	companyact->sanearCadena(mdb_idcliente)+","+
	companyact->sanearCadena(mdb_idforma_pago)+","+
	companyact->sanearCadena(mdb_idalmacen)+",'"+
	companyact->sanearCadena(mdb_descalbaran)+"','"+
	companyact->sanearCadena(mdb_refalbaran)+"',"+
	companyact->sanearCadena(mdb_idtrabajador)+",'"+
	companyact->sanearCadena(mdb_contactalbaran)+"','"+
	companyact->sanearCadena(mdb_telalbaran)+"',"+
	companyact->sanearCadena(mdb_procesadoalbaran)+")";
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idalbaran) AS m FROM albaran");
        if (!cur->eof())
            setidalbaran(cur->valor("m"));
        delete cur;
    } else {
        /// Se trata de una modificacion
        QString SQLQuery = "UPDATE albaran SET ";
        SQLQuery += " numalbaran="+companyact->sanearCadena(mdb_numalbaran);
        SQLQuery += " ,fechaalbaran='"+companyact->sanearCadena(mdb_fechaalbaran)+"'";
        SQLQuery += " ,comentalbaran='"+companyact->sanearCadena(mdb_comentalbaran)+"'";
        SQLQuery += " ,idcliente="+companyact->sanearCadena(mdb_idcliente)+"";
        SQLQuery += " ,idforma_pago="+companyact->sanearCadena(mdb_idforma_pago);
        SQLQuery += " ,idalmacen="+companyact->sanearCadena(mdb_idalmacen);
	SQLQuery += " ,descalbaran='"+companyact->sanearCadena(mdb_descalbaran)+"'";
	SQLQuery += " ,refalbaran='"+companyact->sanearCadena(mdb_refalbaran)+"'";
	SQLQuery += " ,idtrabajador="+companyact->sanearCadena(mdb_idtrabajador);
	SQLQuery += " ,contactalbaran = '"+companyact->sanearCadena(mdb_contactalbaran)+"'";
	SQLQuery += " ,telalbaran = '"+companyact->sanearCadena(mdb_telalbaran)+"'";
	SQLQuery += " ,procesadoalbaran = "+companyact->sanearCadena(mdb_procesadoalbaran);
        SQLQuery += " WHERE idalbaran="+companyact->sanearCadena(mdb_idalbaran);
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
    }// end if
       companyact->commit();
       listalineas->guardaListLinAlbaranCliente();
       listadescuentos->guardaListDescuentoAlbaranCliente();
       cargaAlbaranCliente(mdb_idalbaran);
}// end guardaAlbaranCliente


void AlbaranCliente::imprimirAlbaranCliente() {
    base basesimp;
    /// Copiamos el archivo
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"albaran.rml";
    archivo = "cp "+archivo+" /tmp/albaran.rml";
    system (archivo.ascii());

    /// Copiamos el logo
    archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";
    archivo = "cp "+archivo+" /tmp/logo.jpg";
    system (archivo.ascii());


    QFile file;
    file.setName( "/tmp/albaran.rml" );
    file.open( IO_ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt="";
    // L�ea de totales del presupuesto

    QString SQLQuery = "SELECT * FROM cliente WHERE idcliente="+mdb_idcliente;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        buff.replace("[dircliente]",cur->valor("dircliente"));
        buff.replace("[poblcliente]",cur->valor("poblcliente"));
        buff.replace("[telcliente]",cur->valor("telcliente"));
        buff.replace("[nomcliente]",cur->valor("nomcliente"));
        buff.replace("[cifcliente]",cur->valor("cifcliente"));
        buff.replace("[idcliente]",cur->valor("idcliente"));
    }// end if
    delete cur;

    buff.replace("[numalbaran]",mdb_numalbaran);
    buff.replace("[falbaran]",mdb_fechaalbaran);
    buff.replace("[contactalbaran]",mdb_contactalbaran);
    buff.replace("[telalbaran]",mdb_telalbaran);
    buff.replace("[comentalbaran]",mdb_comentalbaran);
    buff.replace("[descalbaran]",mdb_descalbaran);
    buff.replace("[refalbaran]",mdb_refalbaran);


    LinAlbaranCliente *linea;
    /// Impresi� de la tabla de contenidos.
    fitxersortidatxt += "<blockTable style=\"tablacontenido\" colWidths=\"1.75cm, 8.75cm, 1.5cm, 1.5cm, 1.5cm, 2.25cm\" repeatRows=\"1\">\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "	<td>Cod.</td>\n";
    fitxersortidatxt += "	<td>Concepto</td>\n";
    fitxersortidatxt += "	<td>Cant.</td>\n";
    fitxersortidatxt += "	<td>Precio</td>\n";
    fitxersortidatxt += "	<td>Desc.</td>\n";
    fitxersortidatxt += "	<td>Total</td>\n";
    fitxersortidatxt += "</tr>\n";
    QString l;

    int i=0;// Contador que sirve para poner lineas de más en caso de que sea preciso.

    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        Fixed base = Fixed(linea->cantlalbaran().ascii()) * Fixed(linea->pvplalbaran().ascii());
        basesimp[linea->ivalalbaran()] = basesimp[linea->ivalalbaran()] + base - base * Fixed(linea->descontlalbaran().ascii()) /100;

        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>"+linea->codigocompletoarticulo()+"</td>\n";
        fitxersortidatxt += "	<td>"+linea->desclalbaran()+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->cantlalbaran().ascii())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->pvplalbaran().ascii())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->descontlalbaran().ascii())+" %</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",(base - base * Fixed (linea->descontlalbaran()) /100).toQString().ascii())+"</td>\n";
        fitxersortidatxt += "</tr>";
        i++;
    }// end for

    while (i++ < 15)
        fitxersortidatxt += "<tr></tr>";

    fitxersortidatxt += "</blockTable>\n";
    buff.replace("[story]",fitxersortidatxt);


    Fixed basei("0.00");
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei =basei + it.data();
    }// end for

    /// Impresi� de los descuentos
    fitxersortidatxt = "";
    Fixed porcentt("0.00");
    DescuentoAlbaranCliente *linea1;
    if (listadescuentos->m_lista.first()) {
        fitxersortidatxt += "<blockTable style=\"tabladescuento\" colWidths=\"12cm, 2cm, 3cm\" repeatRows=\"1\">\n";
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>Descuento</td>\n";
        fitxersortidatxt += "	<td>Porcentaje</td>\n";
        fitxersortidatxt += "	<td>Total</td>\n";
        fitxersortidatxt += "</tr>\n";
        for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
            porcentt = porcentt + Fixed(linea1->proporciondalbaran().ascii());
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "	<td>"+linea1->conceptdalbaran()+"</td>\n";
            fitxersortidatxt += "	<td>"+l.sprintf("%s",linea1->proporciondalbaran().ascii())+" %</td>\n";
            fitxersortidatxt += "	<td>"+l.sprintf("-%s",( Fixed(linea1->proporciondalbaran())*basei/100).toQString().ascii())+"</td>\n";
            fitxersortidatxt += "</tr>";
        }// end for
        fitxersortidatxt += "</blockTable>\n";
    }// end if
    buff.replace("[descuentos]",fitxersortidatxt);

    /// Impresión de los totales
    fitxersortidatxt= "";
    QString tr1 = "";	// Rellena el primer tr de titulares
    QString tr2 = "";	// Rellena el segundo tr de cantidades
    fitxersortidatxt += "<blockTable style=\"tablatotales\">\n";


    Fixed totbaseimp("0.00");
    Fixed parbaseimp("0.00");
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        if (porcentt > 0) {
            parbaseimp = it.data()-it.data()*porcentt/100;
        } else {
            parbaseimp = it.data();
        }// end if
        totbaseimp = totbaseimp + parbaseimp;
        tr1 += "	<td>Base "+it.key()+" %</td>\n";
        tr2 += "	<td>"+l.sprintf("%s",parbaseimp.toQString().ascii())+"</td>\n";
    }// end for

    Fixed totiva("0.0");
    Fixed pariva("0.0");
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        if (porcentt > 0) {
            pariva = (it.data()-it.data()*porcentt/100)* Fixed(it.key()) /100;
        } else {
            pariva = it.data()* Fixed(it.key()) /100;
        }// end if
        totiva = totiva + pariva;
        tr1 += "	<td>Iva "+it.key()+" %</td>\n";
        tr2 += "	<td>"+l.sprintf("%s", pariva.toQString().ascii())+"</td>\n";
    }// end for
    tr1 += "	<td>Total </td>\n";
    tr2 += "	<td>"+l.sprintf("%s",(totiva+totbaseimp).toQString().ascii())+"</td>\n";
    fitxersortidatxt += "<tr>"+tr1+"</tr><tr>"+tr2+"</tr></blockTable>\n";
    buff.replace("[totales]",fitxersortidatxt);

    if ( file.open( IO_WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }

    system("trml2pdf.py /tmp/albaran.rml > /tmp/albaran.pdf");
    system("kpdf /tmp/albaran.pdf &");
} //end imprimirAlbaranCliente





void AlbaranCliente::calculaypintatotales() {
    fprintf(stderr,"calculaypintatotales \n");
    base basesimp;
    LinAlbaranCliente *linea;
    /// Impresi� de los contenidos
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
    /// Impresi� de los descuentos
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


