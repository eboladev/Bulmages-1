//
// C++ Implementation: Factura
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "factura.h"
#include "company.h"
#include <qfile.h>

typedef QMap<QString, float> base;


Factura::Factura(company *comp) {
    companyact=comp;
    vaciaFactura();
}

Factura::~Factura() {}


void Factura::borraFactura() {
    if (mdb_idfactura != "") {
        listalineas->borrar();
        companyact->begin();
        companyact->ejecuta("DELETE FROM factura WHERE idfactura="+mdb_idfactura);
        companyact->commit();
        vaciaFactura();
        pintaFactura();
    }// end if
}// end borraFactura


void Factura::vaciaFactura() {
    mdb_idfactura = "";
    mdb_idcliente= "";
    mdb_idalmacen= "";
    mdb_numfactura= "";
    mdb_ffactura= "";
    mdb_reffactura = "";
    mdb_comentfactura= "";
    mdb_procesadafactura="FALSE";
    mdb_codigoserie_factura="";
    mdb_idforma_pago="";
    mdb_descfactura="";
}// end vaciaFactura

void Factura::pintaFactura() {
    fprintf(stderr,"pintaFactura\n");
    pintaidcliente(mdb_idcliente);
    pintaidalmacen(mdb_idalmacen);
    pintaNumFactura(mdb_numfactura);
    pintafechafactura(mdb_ffactura);
    pintareffactura(mdb_reffactura);
    pintaComentFactura(mdb_comentfactura);
    pintaprocesadafactura(mdb_procesadafactura);
    pintacodigoserie_factura(mdb_codigoserie_factura);
    pintaidforma_pago(mdb_idforma_pago);
    pintadescfactura(mdb_descfactura);
    // Pinta el subformulario de detalle del Factura.
    listalineas->pintaListLinFactura();
    listadescuentos->pintaListDescuentoFactura();

    calculaypintatotales();
}// end pintaFactura


// Esta funciï¿½ carga un Factura.
void Factura::cargaFactura(QString idbudget) {
    mdb_idfactura = idbudget;
    inicialize();
    QString query = "SELECT * FROM factura  WHERE idfactura="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        mdb_idcliente= cur->valor("idcliente");
        mdb_idalmacen= cur->valor("idalmacen");
        mdb_numfactura= cur->valor("numfactura");
        mdb_ffactura= cur->valor("ffactura");
	mdb_codigoserie_factura = cur->valor("codigoserie_factura");

        mdb_comentfactura= cur->valor("comentfactura");
        mdb_reffactura = cur->valor("reffactura");
        if (cur->valor("procesadafactura") == "t")
            mdb_procesadafactura = "TRUE";
        else
            mdb_procesadafactura = "FALSE";
        mdb_idusuari = cur->valor("idusuari");
        mdb_idforma_pago = cur->valor("idforma_pago");
	mdb_descfactura  = cur->valor("descfactura");
    }// end if
    delete cur;
    listalineas->cargaListLinFactura(idbudget);
    listadescuentos->cargaDescuentos(idbudget);
    pintaFactura();
}// end chargeBudget


void Factura::guardaFactura() {
    companyact->begin();
    if (mdb_numfactura == "") {
        QString SQLQueryn = "SELECT MAX(numFactura)+1 as num FROM Factura";
        cursor2 *cur= companyact->cargacursor(SQLQueryn);
        if (!cur->eof())
            mdb_numfactura = cur->valor("num");
        delete cur;
    }// end if
    if (mdb_idusuari=="")
        mdb_idusuari="NULL";
    if (mdb_idforma_pago == "")
        mdb_idforma_pago = "NULL";
    if (mdb_idfactura == "") {
        /// Se trata de una inserción
        QString SQLQuery = "INSERT INTO factura (descfactura, codigoserie_factura, procesadafactura, reffactura, numfactura, ffactura, comentfactura, idusuari, idcliente, idalmacen, idforma_pago) VALUES ('"+mdb_descfactura+"','"+mdb_codigoserie_factura+"',"+mdb_procesadafactura+",'"+mdb_reffactura+"',"+mdb_numfactura+",'"+mdb_ffactura+"','"+mdb_comentfactura+"',"+mdb_idusuari+","+mdb_idcliente+","+mdb_idalmacen+","+mdb_idforma_pago+")";

        companyact->ejecuta(SQLQuery);
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idFactura) AS m FROM Factura");
        if (!cur->eof())
            setidfactura (cur->valor("m"));
        delete cur;
        companyact->commit();
    } else {
        /// Se trata de una modificaciï¿½
        QString SQLQuery = "UPDATE Factura SET ";
        SQLQuery += " numFactura="+mdb_numfactura;
        SQLQuery += " ,fFactura='"+mdb_ffactura+"'";
        SQLQuery += " ,comentFactura='"+mdb_comentfactura+"'";
        SQLQuery += " ,idusuari="+mdb_idusuari;
        SQLQuery += " ,idcliente="+mdb_idcliente;
        SQLQuery += " ,idalmacen="+mdb_idalmacen;
        SQLQuery += " ,reffactura='"+mdb_reffactura+"'";
        SQLQuery += " ,idforma_pago="+mdb_idforma_pago;
        SQLQuery += " ,procesadafactura="+mdb_procesadafactura;
	SQLQuery += " ,codigoserie_factura='"+mdb_codigoserie_factura+"'";
	SQLQuery += " ,descfactura='"+mdb_descfactura+"'";
        SQLQuery += " WHERE idFactura="+mdb_idfactura;
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        companyact->commit();
    }// end if
    listalineas->guardaListLinFactura();
    listadescuentos->guardaListDescuentoFactura();
    cargaFactura(mdb_idfactura);
}// end guardaFactura


void Factura::imprimirFactura() {
    /// Copiamos el archivo
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"factura.rml";
    archivo = "cp "+archivo+" /tmp/factura.rml";
    system (archivo.ascii());
    
    /// Copiamos el logo
    archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";
    archivo = "cp "+archivo+" /tmp/logo.jpg";
    system (archivo.ascii());
    
    QFile file;
    file.setName( "/tmp/factura.rml" );
    file.open( IO_ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;
    // Lï¿½ea de totales del presupuesto

    QString SQLQuery = "SELECT * FROM cliente WHERE idcliente="+mdb_idcliente;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        buff.replace("[dircliente]",cur->valor("dircliente"));
        buff.replace("[poblcliente]",cur->valor("poblcliente"));
        buff.replace("[telcliente]",cur->valor("telcliente"));
        buff.replace("[nomcliente]",cur->valor("nomcliente"));
        buff.replace("[cifcliente]",cur->valor("cifcliente"));
    }// end if
    buff.replace("[numfactura]",mdb_numfactura);
    buff.replace("[ffactura]",mdb_ffactura);
    buff.replace("[comentfactura]",mdb_comentfactura);
    buff.replace("[descfactura]",mdb_descfactura);
    buff.replace("[reffactura]",mdb_reffactura);
    buff.replace("[codigoserie_factura]",mdb_codigoserie_factura);
    fitxersortidatxt = "<blockTable style=\"tabla\" colWidths=\"10cm, 2cm, 2cm, 3cm\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "	<td>Concepto</td>";
    fitxersortidatxt += "	<td>Cantidad</td>";
    fitxersortidatxt += "	<td>Precio Und.</td>";
    fitxersortidatxt += "	<td>Total</td>";
    fitxersortidatxt += "</tr>";

    QString l;
    LinFactura *linea;
    uint i = 0;
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        fitxersortidatxt += "<tr>";
        fitxersortidatxt += "	<td>"+linea->desclfactura()+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->cantlfactura().toFloat())+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->pvplfactura().toFloat())+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->cantlfactura().toFloat() * linea->pvplfactura().toFloat())+"</td>";
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
    }// end if
    system("trml2pdf.py /tmp/factura.rml > /tmp/factura.pdf");
    system("kpdf /tmp/factura.pdf");
} //end imprimirFactura



void Factura::calculaypintatotales() {
    base basesimp;
    LinFactura *linea;
    /// Impresión de los contenidos
    QString l;

    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        float base = linea->cantlfactura().toFloat() * linea->pvplfactura().toFloat();
        basesimp[linea->ivalfactura()] +=  base - base * linea->descuentolfactura().toFloat()/100;
    }// end for
    float basei=0;
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei+=it.data();
    }// end for


    /// Impresión de los descuentos
    float porcentt=0;
    DescuentoFactura *linea1;
    if (listadescuentos->m_lista.first()) {
        for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
            porcentt += linea1->proporciondfactura().toFloat();
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

