//
// C++ Implementation: PedidoCliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pedidocliente.h"
#include "company.h"
#include "descpedidocliente.h"

#include <qfile.h>

typedef QMap<QString, float> base;

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
        companyact->ejecuta("DELETE FROM pedidocliente WHERE idpedidocliente="+mdb_idpedidocliente);
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

    /// Pinta el subformulario de detalle del PedidoCliente.
    listalineas->pintaListLinPedidoCliente();
    // Pinta el subformulario de descuentos del pedidocliente
    listadescuentos->pintaListDescuentoPedidoCliente();
    calculaypintatotales();
    fprintf(stderr,"FIN PedidoCliente::pintaPedidoCliente()\n");
}// end pintaPedidoCliente


// Esta funciï¿½ carga un PedidoCliente.
void PedidoCliente::cargaPedidoCliente(QString idbudget) {
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
        //        mdb_nomalmacen = cur->valor("nomalmacen");
        //        mdb_codigoalmacen = cur->valor("codigoalmacen");
        mdb_idforma_pago = cur->valor("idforma_pago");
        mdb_refpedidocliente = cur->valor("refpedidocliente");
        //	mdb_idpedidocliente = cur->valor("idpedidocliente");
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
}// end chargeBudget


void PedidoCliente::guardaPedidoCliente() {
    companyact->begin();
    if (mdb_idforma_pago == "")
        mdb_idforma_pago = "NULL";
    if (mdb_numpedidocliente == "")
        mdb_numpedidocliente = "NULL";
    if (mdb_idpedidocliente == "") {
        /// Se trata de una inserción
        QString SQLQuery = "INSERT INTO pedidocliente (numpedidocliente, fechapedidocliente, idcliente, idalmacen, idforma_pago, refpedidocliente, procesadopedidocliente, descpedidocliente, comentpedidocliente) VALUES ("+mdb_numpedidocliente+",'"+mdb_fechapedidocliente+"',"+mdb_idcliente+","+mdb_idalmacen+","+mdb_idforma_pago+",'"+mdb_refpedidocliente+"',"+mdb_procesadopedidocliente+",'"+mdb_descpedidocliente+"','"+mdb_comentpedidocliente+"')";
        companyact->ejecuta(SQLQuery);
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idpedidocliente) AS m FROM pedidocliente");
        if (!cur->eof())
            setidpedidocliente(cur->valor("m"));
        delete cur;
        companyact->commit();
    } else {
        /// Se trata de una modificación
        QString SQLQuery = "UPDATE pedidocliente SET ";
        SQLQuery += " numpedidocliente="+mdb_numpedidocliente+"";
        SQLQuery += " ,fechapedidocliente='"+mdb_fechapedidocliente+"'";
        SQLQuery += " ,idcliente="+mdb_idcliente;
        SQLQuery += " ,idalmacen="+mdb_idalmacen;
        SQLQuery += " ,idforma_pago="+mdb_idforma_pago;
        SQLQuery += " ,refpedidocliente='"+mdb_refpedidocliente+"'";
        SQLQuery += " ,procesadopedidocliente="+mdb_procesadopedidocliente;
        SQLQuery += " ,descpedidocliente='"+mdb_descpedidocliente+"'";
        SQLQuery += " ,comentpedidocliente='"+mdb_comentpedidocliente+"'";
        SQLQuery += " WHERE idpedidocliente="+mdb_idpedidocliente;
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        companyact->commit();
    }// end if
    listalineas->guardaListLinPedidoCliente();
    listadescuentos->guardaListDescuentoPedidoCliente();
    cargaPedidoCliente(mdb_idpedidocliente);
}// end guardaPedidoCliente





void PedidoCliente::imprimirPedidoCliente() {
    /// Copiamos el archivo
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"pedidocliente.rml";
    archivo = "cp "+archivo+" /tmp/pedidocliente.rml";
    system (archivo.ascii());

    /// Copiamos el logo
    archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";
    archivo = "cp "+archivo+" /tmp/logo.jpg";
    system (archivo.ascii());

    QFile file;
    file.setName( "/tmp/pedidocliente.rml" );
    file.open( IO_ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;
    // Lï¿½ea de totales del pedidocliente

    QString SQLQuery = "SELECT * FROM cliente WHERE idcliente="+mdb_idcliente;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        buff.replace("[dircliente]",cur->valor("dircliente"));
        buff.replace("[poblcliente]",cur->valor("poblcliente"));
        buff.replace("[telcliente]",cur->valor("telcliente"));
        buff.replace("[nomcliente]",cur->valor("nomcliente"));
        buff.replace("[cifcliente]",cur->valor("cifcliente"));
    }// end if

    buff.replace("[numpedidocliente]",mdb_numpedidocliente);
    buff.replace("[fechapedidocliente]",mdb_fechapedidocliente);
    buff.replace("[comentpedidocliente]",mdb_comentpedidocliente);
    //    buff.replace("[codigoalmacen]",mdb_codigoalmacen);
    //    buff.replace("[nomalmacen]",mdb_nomalmacen);
    buff.replace("[descpedidocliente]",mdb_descpedidocliente);
    buff.replace("[refpedidocliente]",mdb_refpedidocliente);



    fitxersortidatxt = "<blockTable style=\"tabla\" colWidths=\"10cm, 2cm, 2cm, 3cm\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "	<td>Concepto</td>";
    fitxersortidatxt += "	<td>Cantidad</td>";
    fitxersortidatxt += "	<td>Precio Und.</td>";
    fitxersortidatxt += "	<td>Total</td>";
    fitxersortidatxt += "</tr>";

    QString l;
    LinPedidoCliente *linea;
    uint i = 0;
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        fitxersortidatxt += "<tr>";
        fitxersortidatxt += "	<td>"+linea->desclpedidocliente()+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->cantlpedidocliente().toFloat())+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->pvplpedidocliente().toFloat())+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->cantlpedidocliente().toFloat() * linea->pvplpedidocliente().toFloat())+"</td>";
        fitxersortidatxt += "</tr>";
        i++;
    }// end for


    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "	<td></td>";
    fitxersortidatxt += "	<td></td>";
    fitxersortidatxt += "	<td>Base</td>";
    fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",listalineas->calculabase())+"</td>";
    fitxersortidatxt += "</tr>";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "	<td></td>";
    fitxersortidatxt += "	<td></td>";
    fitxersortidatxt += "	<td>Iva</td>";
    fitxersortidatxt += "	<td>"+l.sprintf("%2.2f", listalineas->calculaiva())+"</td>";
    fitxersortidatxt += "</tr>";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "	<td></td>";
    fitxersortidatxt += "	<td></td>";
    fitxersortidatxt += "	<td>Total</td>";
    fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",listalineas->calculabase()+listalineas->calculaiva())+"</td>";
    fitxersortidatxt += "</tr>";
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]",fitxersortidatxt);

    if ( file.open( IO_WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }

    system("trml2pdf.py /tmp/pedidocliente.rml > /tmp/pedidocliente.pdf");
    system("kpdf /tmp/pedidocliente.pdf");
} //end imprimirPedidoCliente







void PedidoCliente::calculaypintatotales() {
    base basesimp;
    LinPedidoCliente *linea;
    /// Impresión de los contenidos
    QString l;

    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        float base = linea->cantlpedidocliente().toFloat() * linea->pvplpedidocliente().toFloat();
        basesimp[linea->ivalpedidocliente()] +=  base - base * linea->descuentolpedidocliente().toFloat()/100;
    }// end for
    float basei=0;
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei+=it.data();
    }// end for


    /// Impresión de los descuentos
    float porcentt=0;
    DescuentoPedidoCliente *linea1;
    if (listadescuentos->m_lista.first()) {
        for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
            porcentt += linea1->proporciondpedidocliente().toFloat();
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
