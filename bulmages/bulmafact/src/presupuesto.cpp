//
// C++ Implementation: presupuesto
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "presupuesto.h"
#include "company.h"
#include "configuracion.h"
#include <qfile.h>
#include <qtextstream.h>

typedef QMap<QString, float> base;

presupuesto::presupuesto(company *comp) {
    companyact=comp;
    vaciaPresupuesto();
}

presupuesto::~presupuesto() {}


void presupuesto::borraPresupuesto() {
    if (mdb_idpresupuesto != "") {
        listalineas->borrar();
        companyact->begin();
        companyact->ejecuta("DELETE FROM presupuesto WHERE idpresupuesto="+mdb_idpresupuesto);
        companyact->commit();
        vaciaPresupuesto();
        pintaPresupuesto();
    }// end if
}// end borraPresupuesto


void presupuesto::vaciaPresupuesto() {
    mdb_idpresupuesto = "";
    mdb_idcliente= "";
    mdb_idalmacen= "";
    mdb_numpresupuesto= "";
    mdb_fpresupuesto= "";
    mdb_vencpresupuesto= "";
    mdb_contactpresupuesto= "";
    mdb_telpresupuesto= "";
    mdb_comentpresupuesto= "";
    mdb_procesadopresupuesto = "FALSE";
    mdb_descpresupuesto = "";
    mdb_refpresupuesto = "";
    mdb_idforma_pago = "";
    //    listalineas->vaciar();
}// end vaciaPresupuesto

void presupuesto::pintaPresupuesto() {
    pintaidcliente(mdb_idcliente);
    pintaIdAlmacen(mdb_idalmacen);
    pintaNumPresupuesto(mdb_numpresupuesto);
    pintaFPresupuesto(mdb_fpresupuesto);
    pintaVencPresupuesto(mdb_vencpresupuesto);
    pintaContractPresupuesto(mdb_contactpresupuesto);
    pintaTelPresupuesto(mdb_telpresupuesto);
    pintaComentPresupuesto(mdb_comentpresupuesto);
    pintaprocesadopresupuesto(mdb_procesadopresupuesto);
    pintadescpresupuesto(mdb_descpresupuesto);
    pintarefpresupuesto(mdb_refpresupuesto);
    pintaidforma_pago(mdb_idforma_pago);
    pintaidalmacen(mdb_idalmacen);
    // Pinta el subformulario de detalle del presupuesto.
    listalineas->pintalistlinpresupuesto();
    // Pinta el subformulario de descuentos del presupuesto
    listadescuentos->pintaListDescuentoPresupuesto();

    calculaypintatotales();
}// end pintaPresupuesto


// Esta funciï¿½ carga un presupuesto.
void presupuesto::chargeBudget(QString idbudget) {
    mdb_idpresupuesto = idbudget;
    QString query = "SELECT * FROM presupuesto LEFT JOIN almacen ON  presupuesto.idalmacen = almacen.idalmacen WHERE idpresupuesto="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        mdb_idcliente= cur->valor("idcliente");
        mdb_idalmacen= cur->valor("idalmacen");
        mdb_numpresupuesto= cur->valor("numpresupuesto");
        mdb_fpresupuesto= cur->valor("fpresupuesto");
        mdb_vencpresupuesto= cur->valor("vencpresupuesto");
        mdb_contactpresupuesto= cur->valor("contactpresupuesto");
        mdb_telpresupuesto= cur->valor("telpresupuesto");
        mdb_comentpresupuesto= cur->valor("comentpresupuesto");
        mdb_idusuari = cur->valor("idusuari");
        mdb_descpresupuesto = cur->valor("descpresupuesto");
        mdb_refpresupuesto = cur->valor("refpresupuesto");
        mdb_idforma_pago = cur->valor("idforma_pago");
        if (cur->valor("procesadopresupuesto") == "t")
            mdb_procesadopresupuesto = "TRUE";
        else
            mdb_procesadopresupuesto = "FALSE";
    }// end if
    delete cur;
    fprintf(stderr,"Vamos a cargar las lineas\n");
    listalineas->chargeBudgetLines(idbudget);
    listadescuentos->cargaDescuentos(idbudget);
    pintaPresupuesto();
}// end chargeBudget


void presupuesto::guardapresupuesto() {
    companyact->begin();
    if (mdb_numpresupuesto == "") {
        QString SQLQueryn = "SELECT MAX(numpresupuesto)+1 as num FROM presupuesto";
        cursor2 *cur= companyact->cargacursor(SQLQueryn);
        if (!cur->eof())
            mdb_numpresupuesto = cur->valor("num");
        delete cur;
    }// end if
    if (mdb_idusuari="")
        mdb_idusuari="NULL";
    if (mdb_idforma_pago == "")
        mdb_idforma_pago = "NULL";
    if (mdb_idpresupuesto == "") {
        /// Se trata de una inserciï¿½
        QString SQLQuery = "INSERT INTO presupuesto (numpresupuesto, fpresupuesto, contactpresupuesto, telpresupuesto, vencpresupuesto, comentpresupuesto, idusuari, idcliente, idalmacen, procesadopresupuesto, descpresupuesto, refpresupuesto, idforma_pago) VALUES ("+mdb_numpresupuesto+",'"+mdb_fpresupuesto+"','"+mdb_contactpresupuesto+"','"+mdb_telpresupuesto+"','"+mdb_vencpresupuesto+"','"+mdb_comentpresupuesto+"',"+mdb_idusuari+","+mdb_idcliente+","+mdb_idalmacen+","+mdb_procesadopresupuesto+",'"+mdb_descpresupuesto+"','"+mdb_refpresupuesto+"',"+mdb_idforma_pago+")";
        companyact->ejecuta(SQLQuery);
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idpresupuesto) AS m FROM presupuesto");
        if (!cur->eof())
            setidpresupuesto(cur->valor("m"));
        delete cur;
        companyact->commit();
    } else {
        /// Se trata de una modificaciï¿½
        QString SQLQuery = "UPDATE presupuesto SET ";
        SQLQuery += " numpresupuesto="+mdb_numpresupuesto;
        SQLQuery += " ,fpresupuesto='"+mdb_fpresupuesto+"'";
        SQLQuery += " ,contactpresupuesto='"+mdb_contactpresupuesto+"'";
        SQLQuery += " ,telpresupuesto='"+mdb_telpresupuesto+"'";
        SQLQuery += " ,vencpresupuesto='"+mdb_vencpresupuesto+"'";
        SQLQuery += " ,comentpresupuesto='"+mdb_comentpresupuesto+"'";
        SQLQuery += " ,idusuari="+mdb_idusuari;
        SQLQuery += " ,idcliente="+mdb_idcliente;
        SQLQuery += " ,idalmacen="+mdb_idalmacen;
        SQLQuery += " ,procesadopresupuesto="+mdb_procesadopresupuesto;
        SQLQuery += " ,descpresupuesto='"+mdb_descpresupuesto+"'";
        SQLQuery += " ,refpresupuesto= '"+mdb_refpresupuesto+"'";
        SQLQuery += " ,idforma_pago="+mdb_idforma_pago;
        SQLQuery += " WHERE idpresupuesto="+mdb_idpresupuesto;
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        companyact->commit();
    }// end if
    listalineas->guardalistlinpresupuesto();
    listadescuentos->guardaListDescuentoPresupuesto();
    chargeBudget(mdb_idpresupuesto);
}// end guardapresupuesto


QString presupuesto::detalleArticulos() {
    QString texto="";


    cursor2 *cur=companyact->cargacursor("SELECT * FROM lpresupuesto LEFT JOIN articulo ON lpresupuesto.idarticulo = articulo.idarticulo WHERE presentablearticulo AND idpresupuesto="+mdb_idpresupuesto);
    int i=0;
    while(!cur->eof()) {
        i= !i;
        if (i) {
            texto += "<blockTable style=\"tabladetalle1\" colWidths=\"5cm, 8cm\" rowHeights=\"5.5cm\">\n";
        } else {
            texto += "<blockTable style=\"tabladetalle2\" colWidths=\"8cm, 5cm\" rowHeights=\"5.5cm\">\n";
        }// end if
        texto += "<tr>\n";

        if (i) {
            texto += "<td><h1>"+cur->valor("nomarticulo")+"</h1>";
            texto += "<para><pre>"+cur->valor("obserarticulo")+"</pre></para></td>\n";
        }// end if
        texto += "	<td><illustration x=\"0\" y=\"0\" height=\"5cm\">\n"
                 "<image file=\""+confpr->valor(CONF_DIR_IMG_ARTICLES)+cur->valor("codigocompletoarticulo")+".jpg\" x=\"0\" y=\"0\" height=\"5cm\"/>\n"
                 "</illustration></td>\n";

        if (!i) {
            texto += "<td><h1>"+cur->valor("nomarticulo")+"</h1>";
            texto += "<pre>"+cur->valor("obserarticulo")+"</pre></td>\n";
        }// end if
        texto += "</tr>\n";
        texto += "</blockTable>";
        cur->siguienteregistro();
    }// end while
    delete cur;

    return texto;
}// end detalleArticulos





void presupuesto::imprimirPresupuesto() {
    base basesimp;

    /// Copiamos el archivo
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"presupuesto.rml";
    archivo = "cp "+archivo+" /tmp/presupuesto.rml";
    system (archivo.ascii());

    /// Copiamos el logo
    archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";
    archivo = "cp "+archivo+" /tmp/logo.jpg";
    system (archivo.ascii());


    QFile file;
    file.setName( "/tmp/presupuesto.rml" );
    file.open( IO_ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt="";
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
    delete cur;

    buff.replace("[numpresupuesto]",mdb_numpresupuesto);
    buff.replace("[fpresupuesto]",mdb_fpresupuesto);
    buff.replace("[vencpresupuesto]",mdb_vencpresupuesto);
    buff.replace("[contactpresupuesto]",mdb_contactpresupuesto);
    buff.replace("[telpresupuesto]",mdb_telpresupuesto);
    buff.replace("[comentpresupuesto]",mdb_comentpresupuesto);
    buff.replace("[descpresupuesto]",mdb_descpresupuesto);
    buff.replace("[refpresupuesto]",mdb_refpresupuesto);


    linpresupuesto *linea;
    /// Impresión de los contenidos
    fitxersortidatxt += "<blockTable style=\"tablacontenido\" colWidths=\"7cm, 2cm, 2cm, 3cm, 3cm\" repeatRows=\"1\">\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "	<td>Concepto</td>\n";
    fitxersortidatxt += "	<td>Cantidad</td>\n";
    fitxersortidatxt += "	<td>Precio Und.</td>\n";
    fitxersortidatxt += "	<td>Descuento</td>\n"; 
        fitxersortidatxt += "	<td>Total</td>\n";
    fitxersortidatxt += "</tr>\n";
    QString l;

    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
    	float base = linea->cantlpresupuesto().toFloat() * linea->pvplpresupuesto().toFloat();
        basesimp[linea->ivalpresupuesto()] +=  base - base * linea->descuentolpresupuesto().toFloat()/100;
	
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>"+linea->desclpresupuesto()+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->cantlpresupuesto().toFloat())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->pvplpresupuesto().toFloat())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->descuentolpresupuesto().toFloat())+" %</td>\n";	
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",base - base * linea->descuentolpresupuesto().toFloat()/100)+"</td>\n";
        fitxersortidatxt += "</tr>";
    }// end for
    fitxersortidatxt += "</blockTable>\n";
    float basei=0;
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei+=it.data();
    }// end for



        /// Impresión de los descuentos
    float porcentt=0;
    DescuentoPresupuesto *linea1;
    if (listadescuentos->m_lista.first()) {
        fitxersortidatxt += "<blockTable style=\"tabladescuento\" colWidths=\"10cm, 2cm, 2cm, 3cm\" repeatRows=\"1\">\n";
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>Descuento</td>\n";
        fitxersortidatxt += "	<td></td>\n";
        fitxersortidatxt += "	<td>Porcentaje</td>\n";
        fitxersortidatxt += "	<td>Total</td>\n";
        fitxersortidatxt += "</tr>\n";
        for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
            porcentt += linea1->proporciondpresupuesto().toFloat();
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "	<td>"+linea1->conceptdpresupuesto()+"</td>\n";
            fitxersortidatxt += "	<td></td>\n";
            fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea1->proporciondpresupuesto().toFloat())+" %</td>\n";
            fitxersortidatxt += "	<td>"+l.sprintf("-%2.2f",linea1->proporciondpresupuesto().toFloat()*basei/100)+"</td>\n";
            fitxersortidatxt += "</tr>";
        }// end for
    fitxersortidatxt += "</blockTable>\n";	
    }// end if

    fitxersortidatxt += "<blockTable style=\"tablatotales\" colWidths=\"10cm, 2cm, 2cm, 3cm\" repeatRows=\"1\">\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "	<td>Totales</td>\n";
    fitxersortidatxt += "	<td></td>\n";
    fitxersortidatxt += "	<td></td>\n";
    fitxersortidatxt += "	<td></td>\n";
    fitxersortidatxt += "</tr>\n";


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
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td></td>\n";
        fitxersortidatxt += "	<td></td>\n";
        fitxersortidatxt += "	<td>Base "+it.key()+" %</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",parbaseimp)+"</td>\n";
        fitxersortidatxt += "</tr>\n";
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

        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td></td>\n";
        fitxersortidatxt += "	<td></td>\n";
        fitxersortidatxt += "	<td>Iva "+it.key()+" %</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f", pariva)+"</td>\n";
        fitxersortidatxt += "</tr>\n";
    }// end for

    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "	<td></td>\n";
    fitxersortidatxt += "	<td></td>\n";
    fitxersortidatxt += "	<td>Total </td>\n";
    fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",totiva+totbaseimp)+"</td>\n";
    fitxersortidatxt += "</tr>\n";
    fitxersortidatxt += "</blockTable>\n";

    buff.replace("[story]",fitxersortidatxt);
    buff.replace("[detallearticulos]",detalleArticulos());
    if ( file.open( IO_WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }

    system("trml2pdf.py /tmp/presupuesto.rml > /tmp/pressupost.pdf");
    system("kpdf /tmp/pressupost.pdf &");
} //end imprimirPresupuesto




void presupuesto::calculaypintatotales() {
    base basesimp;
    linpresupuesto *linea;
    /// Impresión de los contenidos
    QString l;

    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
    	float base = linea->cantlpresupuesto().toFloat() * linea->pvplpresupuesto().toFloat();
        basesimp[linea->ivalpresupuesto()] +=  base - base * linea->descuentolpresupuesto().toFloat()/100;
    }// end for
    float basei=0;
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei+=it.data();
    }// end for
    
    
    /// Impresión de los descuentos
    float porcentt=0;
    DescuentoPresupuesto *linea1;
    if (listadescuentos->m_lista.first()) {
        for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
            porcentt += linea1->proporciondpresupuesto().toFloat();
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
