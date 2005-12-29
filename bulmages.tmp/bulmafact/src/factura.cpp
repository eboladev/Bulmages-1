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
//Added by qt3to4:
#include <QTextStream>
#include "funcaux.h"

class Fixed;

typedef QMap<QString, Fixed> base;


Factura::Factura(company *comp) {
    companyact=comp;
    vaciaFactura();
}

Factura::~Factura() {}


void Factura::borraFactura() {
    if (mdb_idfactura != "") {
        listalineas->borrar();
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM factura WHERE idfactura="+mdb_idfactura);
        if (error) {
            companyact->rollback();
            return;
        }// end if
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


// Esta funci� carga un Factura.
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
        /// Se trata de una inserci�
        QString SQLQuery = "INSERT INTO factura (descfactura, codigoserie_factura, procesadafactura, reffactura, numfactura, ffactura, comentfactura, idusuari, idcliente, idalmacen, idforma_pago) VALUES ('"+
                           companyact->sanearCadena(mdb_descfactura)+"','"+
                           companyact->sanearCadena(mdb_codigoserie_factura)+"',"+
                           companyact->sanearCadena(mdb_procesadafactura)+",'"+
                           companyact->sanearCadena(mdb_reffactura)+"',"+
                           companyact->sanearCadena(mdb_numfactura)+",'"+
                           companyact->sanearCadena(mdb_ffactura)+"','"+
                           companyact->sanearCadena(mdb_comentfactura)+"',"+
                           companyact->sanearCadena(mdb_idusuari)+","+
                           companyact->sanearCadena(mdb_idcliente)+","+
                           companyact->sanearCadena(mdb_idalmacen)+","+
                           companyact->sanearCadena(mdb_idforma_pago)+")";

        int error = companyact->ejecuta(SQLQuery);
        if (error) {
            companyact->rollback();
            return;
        }// end if
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idFactura) AS m FROM Factura");
        if (!cur->eof())
            setidfactura (cur->valor("m"));
        delete cur;
        companyact->commit();
    } else {
        /// Se trata de una modificaci�
        QString SQLQuery = "UPDATE Factura SET ";
        SQLQuery += " numFactura="+companyact->sanearCadena(mdb_numfactura);
        SQLQuery += " ,fFactura='"+companyact->sanearCadena(mdb_ffactura)+"'";
        SQLQuery += " ,comentFactura='"+companyact->sanearCadena(mdb_comentfactura)+"'";
        SQLQuery += " ,idusuari="+companyact->sanearCadena(mdb_idusuari);
        SQLQuery += " ,idcliente="+companyact->sanearCadena(mdb_idcliente);
        SQLQuery += " ,idalmacen="+companyact->sanearCadena(mdb_idalmacen);
        SQLQuery += " ,reffactura='"+companyact->sanearCadena(mdb_reffactura)+"'";
        SQLQuery += " ,idforma_pago="+companyact->sanearCadena(mdb_idforma_pago);
        SQLQuery += " ,procesadafactura="+companyact->sanearCadena(mdb_procesadafactura);
        SQLQuery += " ,codigoserie_factura='"+companyact->sanearCadena(mdb_codigoserie_factura)+"'";
        SQLQuery += " ,descfactura='"+companyact->sanearCadena(mdb_descfactura)+"'";
        SQLQuery += " WHERE idFactura="+companyact->sanearCadena(mdb_idfactura);
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
        if (error) {
            companyact->rollback();
            return;
        }// end if
        companyact->commit();
    }// end if
    listalineas->guardaListLinFactura();
    listadescuentos->guardaListDescuentoFactura();
    cargaFactura(mdb_idfactura);
}// end guardaFactura



void Factura::imprimirFactura() {

    base basesimp;

    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"facturacliente.rml";
    QString archivod = confpr->valor(CONF_DIR_TMP)+"facturacliente.rml";
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

    archivologo = "copy "+archivologo+" "+confpr->valor(CONF_DIR_TMP)+"logo.jpg";
#else

    archivologo = "cp "+archivologo+" "+confpr->valor(CONF_DIR_TMP)+"logo.jpg";
#endif

    system (archivologo.ascii());


    QFile file;
    file.setName(archivod);
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
    }// end if
    delete cur;

    buff.replace("[numfactura]",mdb_numfactura);
    buff.replace("[ffactura]",mdb_ffactura);
    buff.replace("[comentfactura]",mdb_comentfactura);
    buff.replace("[descfactura]",mdb_descfactura);
    buff.replace("[reffactura]",mdb_reffactura);
    buff.replace("[codigoserie_factura]", mdb_codigoserie_factura);

    LinFactura *linea;
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
        Fixed base = Fixed(linea->cantlfactura().ascii()) * Fixed(linea->pvplfactura().ascii());
        basesimp[linea->ivalfactura()] = basesimp[linea->ivalfactura()] + base - base * Fixed(linea->descuentolfactura().ascii()) /100;

        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>"+linea->codigocompletoarticulo()+"</td>\n";
        fitxersortidatxt += "	<td>"+linea->desclfactura()+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->cantlfactura().ascii())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->pvplfactura().ascii())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->descuentolfactura().ascii())+" %</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",(base - base * Fixed (linea->descuentolfactura()) /100).toQString().ascii())+"</td>\n";
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
    DescuentoFactura *linea1;
    if (listadescuentos->m_lista.first()) {
        fitxersortidatxt += "<blockTable style=\"tabladescuento\" colWidths=\"12cm, 2cm, 3cm\" repeatRows=\"1\">\n";
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>Descuento</td>\n";
        fitxersortidatxt += "	<td>Porcentaje</td>\n";
        fitxersortidatxt += "	<td>Total</td>\n";
        fitxersortidatxt += "</tr>\n";
        for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
            porcentt = porcentt + Fixed(linea1->proporciondfactura().ascii());
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "	<td>"+linea1->conceptdfactura()+"</td>\n";
            fitxersortidatxt += "	<td>"+l.sprintf("%s",linea1->proporciondfactura().ascii())+" %</td>\n";
            fitxersortidatxt += "	<td>"+l.sprintf("-%s",( Fixed(linea1->proporciondfactura())*basei/100).toQString().ascii())+"</td>\n";
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



    invocaPDF("facturacliente");
} //end imprimirPresupuesto





void Factura::calculaypintatotales() {
    base basesimp;
    LinFactura *linea;
    /// Impresi� de los contenidos
    QString l;

    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        Fixed cant(linea->cantlfactura().ascii());
        Fixed pvpund(linea->pvplfactura().ascii());
        Fixed desc1(linea->descuentolfactura().ascii());
        Fixed cantpvp = cant * pvpund;
        Fixed base = cantpvp - cantpvp * desc1 / 100;
        basesimp[linea->ivalfactura()] =  basesimp[linea->ivalfactura()]+ base;
    }// end for


    Fixed basei("0.00");
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei = basei + it.data();
    }// end for


    /// Impresi� de los descuentos
    /// Impresi� de los descuentos
    Fixed porcentt("0.00");
    DescuentoFactura *linea1;
    if (listadescuentos->m_lista.first()) {
        for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
            Fixed propor(linea1->proporciondfactura().ascii());
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

