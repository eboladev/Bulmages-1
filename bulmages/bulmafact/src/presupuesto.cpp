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
#include "fixed.h"


typedef QMap<QString, Fixed> base;

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
    mdb_idtrabajador = "";
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
    pintaidtrabajador(mdb_idtrabajador);
    // Pinta el subformulario de detalle del presupuesto.
    listalineas->pintalistlinpresupuesto();
    // Pinta el subformulario de descuentos del presupuesto
    listadescuentos->pintaListDescuentoPresupuesto();

    calculaypintatotales();
}// end pintaPresupuesto

// Esta funci� carga un presupuesto.
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
	mdb_idtrabajador = cur->valor("idtrabajador");
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
    if (mdb_numpresupuesto == "")
        mdb_numpresupuesto = "NULL";
    if (mdb_idusuari="")
        mdb_idusuari="NULL";
    if (mdb_idforma_pago == "")
        mdb_idforma_pago = "NULL";
    if (mdb_idtrabajador== "")
    	mdb_idtrabajador = "NULL";
    if (mdb_idpresupuesto == "") {
        /// Se trata de una inserci�
        QString SQLQuery = "INSERT INTO presupuesto (numpresupuesto, fpresupuesto, contactpresupuesto, telpresupuesto, vencpresupuesto, comentpresupuesto, idusuari, idcliente, idalmacen, procesadopresupuesto, descpresupuesto, refpresupuesto, idforma_pago, idtrabajador) VALUES ("+
	companyact->sanearCadena(mdb_numpresupuesto)+",'"+
	companyact->sanearCadena(mdb_fpresupuesto)+"','"+
	companyact->sanearCadena(mdb_contactpresupuesto)+"','"+
	companyact->sanearCadena(mdb_telpresupuesto)+"','"+
	companyact->sanearCadena(mdb_vencpresupuesto)+"','"+
	companyact->sanearCadena(mdb_comentpresupuesto)+"',"+
	companyact->sanearCadena(mdb_idusuari)+","+
	companyact->sanearCadena(mdb_idcliente)+","+
	companyact->sanearCadena(mdb_idalmacen)+","+
	companyact->sanearCadena(mdb_procesadopresupuesto)+",'"+
	companyact->sanearCadena(mdb_descpresupuesto)+"','"+
	companyact->sanearCadena(mdb_refpresupuesto)+"',"+
	companyact->sanearCadena(mdb_idforma_pago)+","+
	companyact->sanearCadena(mdb_idtrabajador)+")";
        companyact->ejecuta(SQLQuery);
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idpresupuesto) AS m FROM presupuesto");
        if (!cur->eof())
            setidpresupuesto(cur->valor("m"));
        delete cur;
        companyact->commit();
    } else {
        /// Se trata de una modificaci�
        QString SQLQuery = "UPDATE presupuesto SET ";
        SQLQuery += " numpresupuesto="+companyact->sanearCadena(mdb_numpresupuesto);
        SQLQuery += " ,fpresupuesto='"+companyact->sanearCadena(mdb_fpresupuesto)+"'";
        SQLQuery += " ,contactpresupuesto='"+companyact->sanearCadena(mdb_contactpresupuesto)+"'";
        SQLQuery += " ,telpresupuesto='"+companyact->sanearCadena(mdb_telpresupuesto)+"'";
        SQLQuery += " ,vencpresupuesto='"+companyact->sanearCadena(mdb_vencpresupuesto)+"'";
        SQLQuery += " ,comentpresupuesto='"+companyact->sanearCadena(mdb_comentpresupuesto)+"'";
        SQLQuery += " ,idusuari="+companyact->sanearCadena(mdb_idusuari);
        SQLQuery += " ,idcliente="+companyact->sanearCadena(mdb_idcliente);
        SQLQuery += " ,idalmacen="+companyact->sanearCadena(mdb_idalmacen);
        SQLQuery += " ,procesadopresupuesto="+companyact->sanearCadena(mdb_procesadopresupuesto);
        SQLQuery += " ,descpresupuesto='"+companyact->sanearCadena(mdb_descpresupuesto)+"'";
        SQLQuery += " ,refpresupuesto= '"+companyact->sanearCadena(mdb_refpresupuesto)+"'";
        SQLQuery += " ,idforma_pago="+companyact->sanearCadena(mdb_idforma_pago);
	SQLQuery += " ,idtrabajador="+companyact->sanearCadena(mdb_idtrabajador);
        SQLQuery += " WHERE idpresupuesto="+companyact->sanearCadena(mdb_idpresupuesto);
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

	QString file = confpr->valor(CONF_DIR_IMG_ARTICLES)+cur->valor("codigocompletoarticulo")+".jpg";
        QFile f( file );
	if (f.exists() ) {
        texto += "	<td><illustration x=\"0\" y=\"0\" height=\"5cm\">\n"
                 "<image file=\""+confpr->valor(CONF_DIR_IMG_ARTICLES)+cur->valor("codigocompletoarticulo")+".jpg\" x=\"0\" y=\"0\" height=\"5cm\"/>\n"
                 "</illustration></td>\n";
	} else {
		texto += "<td></td>\n";
	}

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
    // L�ea de totales del presupuesto

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
    /// Impresi� de los contenidos
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
    	Fixed base = Fixed(linea->cantlpresupuesto().ascii()) * Fixed(linea->pvplpresupuesto().ascii());
        basesimp[linea->ivalpresupuesto()] = basesimp[linea->ivalpresupuesto()] + base - base * Fixed(linea->descuentolpresupuesto().ascii()) /100;
	
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>"+linea->desclpresupuesto()+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->cantlpresupuesto().ascii())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->pvplpresupuesto().ascii())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->descuentolpresupuesto().ascii())+" %</td>\n";	
        fitxersortidatxt += "	<td>"+l.sprintf("%s",(base - base * Fixed (linea->descuentolpresupuesto()) /100).toQString().ascii())+"</td>\n";
        fitxersortidatxt += "</tr>";
    }// end for
    fitxersortidatxt += "</blockTable>\n";
    Fixed basei("0.00");
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei =basei + it.data();
    }// end for



        /// Impresi� de los descuentos
    Fixed porcentt("0.00");
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
            porcentt = porcentt + Fixed(linea1->proporciondpresupuesto().ascii());
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "	<td>"+linea1->conceptdpresupuesto()+"</td>\n";
            fitxersortidatxt += "	<td></td>\n";
            fitxersortidatxt += "	<td>"+l.sprintf("%s",linea1->proporciondpresupuesto().ascii())+" %</td>\n";
            fitxersortidatxt += "	<td>"+l.sprintf("-%s",( Fixed(linea1->proporciondpresupuesto())*basei/100).toQString().ascii())+"</td>\n";
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


    Fixed totbaseimp("0.00");
    Fixed parbaseimp("0.00");
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        if (porcentt > 0) {
            parbaseimp = it.data()-it.data()*porcentt/100;
        } else {
            parbaseimp = it.data();
        }// end if
	 totbaseimp = totbaseimp + parbaseimp;
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td></td>\n";
        fitxersortidatxt += "	<td></td>\n";
        fitxersortidatxt += "	<td>Base "+it.key()+" %</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",parbaseimp.toQString().ascii())+"</td>\n";
        fitxersortidatxt += "</tr>\n";
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

        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td></td>\n";
        fitxersortidatxt += "	<td></td>\n";
        fitxersortidatxt += "	<td>Iva "+it.key()+" %</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s", pariva.toQString().ascii())+"</td>\n";
        fitxersortidatxt += "</tr>\n";
    }// end for

    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "	<td></td>\n";
    fitxersortidatxt += "	<td></td>\n";
    fitxersortidatxt += "	<td>Total </td>\n";
    fitxersortidatxt += "	<td>"+l.sprintf("%s",(totiva+totbaseimp).toQString().ascii())+"</td>\n";
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
    fprintf(stderr,"calculaypintatotales \n");
    base basesimp;
    linpresupuesto *linea;
    /// Impresi� de los contenidos
    QString l;
    
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
    	Fixed cant(linea->cantlpresupuesto().ascii());
	Fixed pvpund(linea->pvplpresupuesto().ascii());
	Fixed desc1(linea->descuentolpresupuesto().ascii());
	Fixed cantpvp = cant * pvpund;
	Fixed base = cantpvp - cantpvp * desc1 / 100;
        basesimp[linea->ivalpresupuesto()] =  basesimp[linea->ivalpresupuesto()]+ base;
    }// end for
    

    Fixed basei("0.00");
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei = basei + it.data();
    }// end for
    /// Impresi� de los descuentos
    Fixed porcentt("0.00");
    DescuentoPresupuesto *linea1;
    if (listadescuentos->m_lista.first()) {
        for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
	    Fixed propor(linea1->proporciondpresupuesto().ascii());
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
