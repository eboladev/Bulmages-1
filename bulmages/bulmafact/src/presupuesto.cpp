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

    pintatotales(listalineas->calculabase(), listalineas->calculaiva());
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
        if (cur->valor("procesadopresupuesto") == "t")
            mdb_procesadopresupuesto = "TRUE";
        else
            mdb_procesadopresupuesto = "FALSE";
    }// end if
    delete cur;
    fprintf(stderr,"Vamos a cargar las lineas\n");
    listalineas->chargeBudgetLines(idbudget);
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
        /// Se trata de una inserci�
        QString SQLQuery = "INSERT INTO presupuesto (numpresupuesto, fpresupuesto, contactpresupuesto, telpresupuesto, vencpresupuesto, comentpresupuesto, idusuari, idcliente, idalmacen, procesadopresupuesto, descpresupuesto, refpresupuesto, idforma_pago) VALUES ("+mdb_numpresupuesto+",'"+mdb_fpresupuesto+"','"+mdb_contactpresupuesto+"','"+mdb_telpresupuesto+"','"+mdb_vencpresupuesto+"','"+mdb_comentpresupuesto+"',"+mdb_idusuari+","+mdb_idcliente+","+mdb_idalmacen+","+mdb_procesadopresupuesto+",'"+mdb_descpresupuesto+"','"+mdb_refpresupuesto+"',"+mdb_idforma_pago+")";
        companyact->ejecuta(SQLQuery);
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idpresupuesto) AS m FROM presupuesto");
        if (!cur->eof())
            setidpresupuesto(cur->valor("m"));
        delete cur;
        companyact->commit();
    } else {
        /// Se trata de una modificaci�
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
    chargeBudget(mdb_idpresupuesto);
}// end guardapresupuesto


QString presupuesto::detalleArticulos() {
    QString texto="";


    cursor2 *cur=companyact->cargacursor("SELECT * FROM lpresupuesto LEFT JOIN articulo ON lpresupuesto.idarticulo = articulo.idarticulo WHERE presentablearticulo AND idpresupuesto="+mdb_idpresupuesto);
    int i=0;
    while(!cur->eof()) {
            i= !i;
    if (i) {
    texto += "<blockTable style=\"tabla1\" colWidths=\"5cm, 8cm\" rowHeights=\"5.5cm\">\n";
    } else {
     texto += "<blockTable style=\"tabla2\" colWidths=\"8cm, 5cm\" rowHeights=\"5.5cm\">\n";
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
    QString fitxersortidatxt;
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



    fitxersortidatxt = "<blockTable style=\"tabla\" colWidths=\"10cm, 2cm, 2cm, 3cm\" repeatRows=\"1\">\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "	<td>Concepto</td>\n";
    fitxersortidatxt += "	<td>Cantidad</td>\n";
    fitxersortidatxt += "	<td>Precio Und.</td>\n";
    fitxersortidatxt += "	<td>Total</td>\n";
    fitxersortidatxt += "</tr>\n";

    QString l;
    linpresupuesto *linea;
    uint i = 0;
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>"+linea->desclpresupuesto()+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->cantlpresupuesto().toFloat())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->pvplpresupuesto().toFloat())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->cantlpresupuesto().toFloat() * linea->pvplpresupuesto().toFloat())+"</td>\n";
        fitxersortidatxt += "</tr>";
        i++;
    }// end for


    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "	<td></td>\n";
    fitxersortidatxt += "	<td></td>\n";
    fitxersortidatxt += "	<td>Base</td>\n";
    fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",listalineas->calculabase())+"</td>\n";
    fitxersortidatxt += "</tr>\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "	<td></td>\n";
    fitxersortidatxt += "	<td></td>\n";
    fitxersortidatxt += "	<td>Iva</td>\n";
    fitxersortidatxt += "	<td>"+l.sprintf("%2.2f", listalineas->calculaiva())+"</td>\n";
    fitxersortidatxt += "</tr>\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "	<td></td>\n";
    fitxersortidatxt += "	<td></td>\n";
    fitxersortidatxt += "	<td>Total</td>\n";
    fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",listalineas->calculabase()+listalineas->calculaiva())+"</td>\n";
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

