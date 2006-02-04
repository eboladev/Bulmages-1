//
// C++ Implementation: PedidoCliente
//
// Description:
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pedidocliente.h"
#include "company.h"
#include "descpedidocliente.h"

#include <qfile.h>
//Added by qt3to4:
#include <QTextStream>

#include "fixed.h"
#include "funcaux.h"

typedef QMap<QString, Fixed> base;

PedidoCliente::PedidoCliente(company *comp) {
    companyact=comp;
    vaciaPedidoCliente();
}

PedidoCliente::~PedidoCliente() {}


void PedidoCliente::borraPedidoCliente() {
    if (mdb_idpedidocliente != "") {
        listalineas->borrar();
        listadescuentos->borrar();
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM pedidocliente WHERE idpedidocliente="+mdb_idpedidocliente);
        if (error) {
            companyact->rollback();
            return;
        }// end if
        companyact->commit();
        vaciaPedidoCliente();
        pintaPedidoCliente();
    }// end if
}// end borraPedidoCliente


void PedidoCliente::vaciaPedidoCliente() {
    mdb_idcliente = "";
    mdb_idalmacen = "";
    mdb_idpedidocliente = "";
    mdb_numpedidocliente = "";
    mdb_fechapedidocliente = "";
    mdb_descpedidocliente = "";

    mdb_idforma_pago = "";
    mdb_comentpedidocliente = "";
    mdb_refpedidocliente = "";
    mdb_procesadopedidocliente = "FALSE";
    mdb_contactpedidocliente = "";
    mdb_telpedidocliente = "";
    mdb_idtrabajador = "";
    //    listalineas->vaciar();
}// end vaciaPedidoCliente

void PedidoCliente::pintaPedidoCliente() {
    fprintf(stderr,"PedidoCliente::pintaPedidoCliente\n");
    pintaidcliente(mdb_idcliente);
    pintaidalmacen(mdb_idalmacen);
    pintaidpedidocliente(mdb_idpedidocliente);
    pintanumpedidocliente(mdb_numpedidocliente);
    pintafechapedidocliente(mdb_fechapedidocliente);
    pintadescpedidocliente(mdb_descpedidocliente);

    pintaidforma_pago(mdb_idforma_pago);

    pintacomentpedidocliente(mdb_comentpedidocliente);

    pintarefpedidocliente(mdb_refpedidocliente);
    pintaprocesadopedidocliente(mdb_procesadopedidocliente);
    pintacontactpedidocliente(mdb_contactpedidocliente);
    pintatelpedidocliente(mdb_telpedidocliente);
    pintaidtrabajador(mdb_idtrabajador);

    /// Pinta el subformulario de detalle del PedidoCliente.
    listalineas->pintaListLinPedidoCliente();
    // Pinta el subformulario de descuentos del pedidocliente
    listadescuentos->pintaListDescuentoPedidoCliente();
    calculaypintatotales();
    fprintf(stderr,"FIN PedidoCliente::pintaPedidoCliente()\n");
}// end pintaPedidoCliente


// Esta funci� carga un PedidoCliente.
int PedidoCliente::cargaPedidoCliente(QString idbudget) {
    fprintf(stderr,"cargaPedidoCliente(%s)\n",idbudget.ascii());
    mdb_idpedidocliente = idbudget;
    QString query = "SELECT * FROM pedidocliente WHERE idpedidocliente="+mdb_idpedidocliente;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        mdb_idcliente = cur->valor("idcliente");
        mdb_idalmacen = cur->valor("idalmacen");
        mdb_idpedidocliente = cur->valor("idpedidocliente");
        mdb_numpedidocliente = cur->valor("numpedidocliente");
        mdb_fechapedidocliente = cur->valor("fechapedidocliente");
        mdb_descpedidocliente = cur->valor("descpedidocliente");
        mdb_idforma_pago = cur->valor("idforma_pago");
        mdb_refpedidocliente = cur->valor("refpedidocliente");
        mdb_contactpedidocliente = cur->valor("contactpedidocliente");
        mdb_telpedidocliente = cur->valor("telpedidocliente");
        mdb_idtrabajador = cur->valor("idtrabajador");
        if (cur->valor("procesadopedidocliente") == "t")
            mdb_procesadopedidocliente = "TRUE";
        else
            mdb_procesadopedidocliente = "FALSE";

        mdb_comentpedidocliente = cur->valor("comentpedidocliente");
    }// end if
    delete cur;
    fprintf(stderr,"Vamos a cargar las lineas\n");
    listalineas->cargaListLinPedidoCliente(idbudget);
    listadescuentos->cargaDescuentos(idbudget);
    fprintf(stderr,"vamos a hacer el pintado de la LineaPedido\n");
    pintaPedidoCliente();
	return 0;
}// end chargeBudget


void PedidoCliente::guardaPedidoCliente() {
    companyact->begin();
	QString fecha;
    if (mdb_idforma_pago == "")
        mdb_idforma_pago = "NULL";
    if (mdb_numpedidocliente == "")
        mdb_numpedidocliente = "NULL";
    if (mdb_idtrabajador == "")
        mdb_idtrabajador = "NULL";
if (mdb_fechapedidocliente == "")
    fecha = "NULL";
else
    fecha = "'"+companyact->sanearCadena(mdb_fechapedidocliente)+"'";
    if (mdb_idpedidocliente == "") {
        /// Se trata de una insercion
        QString SQLQuery = "INSERT INTO pedidocliente (contactpedidocliente, telpedidocliente, numpedidocliente, fechapedidocliente, idcliente, idalmacen, idforma_pago, refpedidocliente, procesadopedidocliente, descpedidocliente, comentpedidocliente, idtrabajador) VALUES ('"+
                           companyact->sanearCadena(mdb_contactpedidocliente)+"','"+
                           companyact->sanearCadena(mdb_telpedidocliente)+"',"+
                           companyact->sanearCadena(mdb_numpedidocliente)+","+
                           fecha+","+
                           companyact->sanearCadena(mdb_idcliente)+","+
                           companyact->sanearCadena(mdb_idalmacen)+","+
                           companyact->sanearCadena(mdb_idforma_pago)+",'"+
                           companyact->sanearCadena(mdb_refpedidocliente)+"',"+
                           companyact->sanearCadena(mdb_procesadopedidocliente)+",'"+
                           companyact->sanearCadena(mdb_descpedidocliente)+"','"+
                           companyact->sanearCadena(mdb_comentpedidocliente)+"', "+
                           companyact->sanearCadena(mdb_idtrabajador)+")";
        int error = companyact->ejecuta(SQLQuery);
        if (error) {
            companyact->rollback();
            return;
        }// end if
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idpedidocliente) AS m FROM pedidocliente");
        if (!cur->eof())
            setidpedidocliente(cur->valor("m"));
        delete cur;
        companyact->commit();
    } else {
        /// Se trata de una modificaci�
        QString SQLQuery = "UPDATE pedidocliente SET ";
        SQLQuery += " numpedidocliente="+companyact->sanearCadena(mdb_numpedidocliente);
        SQLQuery += " ,fechapedidocliente="+fecha;
        SQLQuery += " ,idcliente="+companyact->sanearCadena(mdb_idcliente);
        SQLQuery += " ,idalmacen="+companyact->sanearCadena(mdb_idalmacen);
        SQLQuery += " ,idforma_pago="+companyact->sanearCadena(mdb_idforma_pago);
        SQLQuery += " ,refpedidocliente='"+companyact->sanearCadena(mdb_refpedidocliente)+"'";
        SQLQuery += " ,procesadopedidocliente="+companyact->sanearCadena(mdb_procesadopedidocliente);
        SQLQuery += " ,descpedidocliente='"+companyact->sanearCadena(mdb_descpedidocliente)+"'";
        SQLQuery += " ,comentpedidocliente='"+companyact->sanearCadena(mdb_comentpedidocliente)+"'";
        SQLQuery += " ,contactpedidocliente='"+companyact->sanearCadena(mdb_contactpedidocliente)+"'";
        SQLQuery += " ,telpedidocliente='"+companyact->sanearCadena(mdb_telpedidocliente)+"'";
        SQLQuery += " ,idtrabajador ="+companyact->sanearCadena(mdb_idtrabajador);
        SQLQuery += " WHERE idpedidocliente="+companyact->sanearCadena(mdb_idpedidocliente);
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
        if (error) {
            companyact->rollback();
            return;
        }// end if
        companyact->commit();
    }// end if
    listalineas->guardaListLinPedidoCliente();
    listadescuentos->guardaListDescuentoPedidoCliente();
    cargaPedidoCliente(mdb_idpedidocliente);
}// end guardaPedidoCliente


void PedidoCliente::imprimirPedidoCliente() {

    base basesimp;

    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"pedidocliente.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"pedidocliente.rml";
    QString archivologo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";

    /// Copiamos el archivo
#ifdef WINDOWS

    archivo = "copy "+archivo+" "+archivod;
#else

    archivo = "cp "+archivo+" "+archivod;
#endif

    system (archivo.ascii());

    /// Copiamos el logo

#ifdef WINDOWS

    archivologo = "copy "+archivologo+" "+confpr->valor(CONF_DIR_USER)+"logo.jpg";
#else

    archivologo = "cp "+archivologo+" "+confpr->valor(CONF_DIR_USER)+"logo.jpg";
#endif

    system (archivologo.ascii());


    QFile file;
    file.setName( archivod );
    file.open( QIODevice::ReadOnly );
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
        buff.replace("[cpcliente]",cur->valor("cpcliente"));
    }// end if
    delete cur;

    buff.replace("[numpedidocliente]",mdb_numpedidocliente);
    buff.replace("[fechapedidocliente]",mdb_fechapedidocliente);
    buff.replace("[contactpedidocliente]",mdb_contactpedidocliente);
    buff.replace("[telpedidocliente]",mdb_telpedidocliente);
    buff.replace("[comentpedidocliente]",mdb_comentpedidocliente);
    buff.replace("[descpedidocliente]",mdb_descpedidocliente);
    buff.replace("[refpedidocliente]",mdb_refpedidocliente);


    LinPedidoCliente *linea;
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
        Fixed base = Fixed(linea->cantlpedidocliente().ascii()) * Fixed(linea->pvplpedidocliente().ascii());
        basesimp[linea->ivalpedidocliente()] = basesimp[linea->ivalpedidocliente()] + base - base * Fixed(linea->descuentolpedidocliente().ascii()) /100;

        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>"+linea->codigocompletoarticulo()+"</td>\n";
        fitxersortidatxt += "	<td>"+linea->desclpedidocliente()+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->cantlpedidocliente().ascii())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->pvplpedidocliente().ascii())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->descuentolpedidocliente().ascii())+" %</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",(base - base * Fixed (linea->descuentolpedidocliente()) /100).toQString().ascii())+"</td>\n";
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
    DescuentoPedidoCliente *linea1;
    if (listadescuentos->m_lista.first()) {
        fitxersortidatxt += "<blockTable style=\"tabladescuento\" colWidths=\"12cm, 2cm, 3cm\" repeatRows=\"1\">\n";
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>Descuento</td>\n";
        fitxersortidatxt += "	<td>Porcentaje</td>\n";
        fitxersortidatxt += "	<td>Total</td>\n";
        fitxersortidatxt += "</tr>\n";
        for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
            porcentt = porcentt + Fixed(linea1->proporciondpedidocliente().ascii());
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "	<td>"+linea1->conceptdpedidocliente()+"</td>\n";
            fitxersortidatxt += "	<td>"+l.sprintf("%s",linea1->proporciondpedidocliente().ascii())+" %</td>\n";
            fitxersortidatxt += "	<td>"+l.sprintf("-%s",( Fixed(linea1->proporciondpedidocliente())*basei/100).toQString().ascii())+"</td>\n";
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

    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }


    invocaPDF("pedidocliente");

} //end imprimirPresupuesto


void PedidoCliente::calculaypintatotales() {
    fprintf(stderr,"calculaypintatotales \n");
    base basesimp;
    LinPedidoCliente *linea;
    /// Impresi� de los contenidos
    QString l;

    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        Fixed cant(linea->cantlpedidocliente().ascii());
        Fixed pvpund(linea->pvplpedidocliente().ascii());
        Fixed desc1(linea->descuentolpedidocliente().ascii());
        Fixed cantpvp = cant * pvpund;
        Fixed base = cantpvp - cantpvp * desc1 / 100;
        basesimp[linea->ivalpedidocliente()] =  basesimp[linea->ivalpedidocliente()]+ base;
    }// end for


    Fixed basei("0.00");
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei = basei + it.data();
    }// end for
    /// Impresi� de los descuentos
    Fixed porcentt("0.00");
    DescuentoPedidoCliente *linea1;
    if (listadescuentos->m_lista.first()) {
        for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
            Fixed propor(linea1->proporciondpedidocliente().ascii());
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
