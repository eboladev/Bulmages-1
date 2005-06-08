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

#include <qfile.h>
#include <qtextstream.h>

presupuesto::presupuesto(company *comp) {
    companyact=comp;
    vaciaPresupuesto();
}

presupuesto::~presupuesto() {
}


void presupuesto::borraPresupuesto() {
    if (mdb_idpresupuesto != "") {
        listalineas->borrar();
        companyact->begin();
        companyact->ejecuta("DELETE FROM prfp WHERE idpresupuesto="+mdb_idpresupuesto);
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
    mdb_nomcliente= "";
    mdb_cifcliente= "";
    mdb_codigoalmacen= "";
    mdb_nomalmacen= "";
    mdb_procesadopresupuesto = "FALSE";
    mdb_descpresupuesto = "";
    mdb_refpresupuesto = "";
    mdb_idforma_pago = "";
    //    listalineas->vaciar();
}// end vaciaPresupuesto

void presupuesto::pintaPresupuesto() {
    pintaIdClient(mdb_idcliente);
    pintaIdAlmacen(mdb_idalmacen);
    pintaNumPresupuesto(mdb_numpresupuesto);
    pintaFPresupuesto(mdb_fpresupuesto);
    pintaVencPresupuesto(mdb_vencpresupuesto);
    pintaContractPresupuesto(mdb_contactpresupuesto);
    pintaTelPresupuesto(mdb_telpresupuesto);
    pintaComentPresupuesto(mdb_comentpresupuesto);
    pintaNomClient(mdb_nomcliente);
    pintaCifClient(mdb_cifcliente);
    pintaCodigoAlmacen(mdb_codigoalmacen);
    pintaNomAlmacen(mdb_nomalmacen);
    pintaprocesadopresupuesto(mdb_procesadopresupuesto);
    pintadescpresupuesto(mdb_descpresupuesto);
    pintarefpresupuesto(mdb_refpresupuesto);
    pintaidforma_pago(mdb_idforma_pago);
    // Pinta el subformulario de detalle del presupuesto.
    listalineas->pintalistlinpresupuesto();

    pintatotales(listalineas->calculabase(), listalineas->calculaiva());
}// end pintaPresupuesto


// Esta funci� carga un presupuesto.
void presupuesto::chargeBudget(QString idbudget) {
    mdb_idpresupuesto = idbudget;
    QString query = "SELECT * FROM presupuesto LEFT JOIN cliente ON cliente.idcliente = presupuesto.idcliente LEFT JOIN almacen ON  presupuesto.idalmacen = almacen.idalmacen WHERE idpresupuesto="+idbudget;
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
        mdb_nomcliente= cur->valor("nomcliente");
        mdb_cifcliente= cur->valor("cifcliente");
        mdb_codigoalmacen= cur->valor("codigoalmacen");
        mdb_nomalmacen= cur->valor("nomalmacen");
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

    //    m_initialValues = calculateValues();
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
}// end guardapresupuesto


void presupuesto::setCifClient(QString val) {
    mdb_cifcliente=val;
    QString SQLQuery = "SELECT * FROM cliente WHERE cifcliente='"+mdb_cifcliente+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idcliente = cur->valor("idcliente");
        mdb_nomcliente = cur->valor("nomcliente");
    } else {
        mdb_idcliente="";
        mdb_nomcliente="";
    }// end if
    delete cur;
    pintaNomClient(mdb_nomcliente);
}// end setCifClient

void presupuesto::setCodigoAlmacen(QString val) {
    mdb_codigoalmacen=val;
    QString SQLQuery = "SELECT * FROM almacen WHERE codigoalmacen='"+mdb_codigoalmacen+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idalmacen = cur->valor("idalmacen");
        mdb_nomalmacen = cur->valor("nomalmacen");
    } else {
        mdb_idalmacen="";
        mdb_nomalmacen="";
    }// end if
    delete cur;
    pintaNomAlmacen(mdb_nomalmacen);
}// end setCodigoAlmacen


void presupuesto::imprimirPresupuesto() {
    system ("cp /home/tborras/Desktop/prueba.rml /tmp/presupuesto.rml");
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

    buff.replace("[numpresupuesto]",mdb_numpresupuesto);
    buff.replace("[fpresupuesto]",mdb_fpresupuesto);
    buff.replace("[vencpresupuesto]",mdb_vencpresupuesto);
    buff.replace("[contactpresupuesto]",mdb_contactpresupuesto);
    buff.replace("[telpresupuesto]",mdb_telpresupuesto);
    buff.replace("[comentpresupuesto]",mdb_comentpresupuesto);
    buff.replace("[codigoalmacen]",mdb_codigoalmacen);
    buff.replace("[nomalmacen]",mdb_nomalmacen);
    buff.replace("[descpresupuesto]",mdb_descpresupuesto);
    buff.replace("[refpresupuesto]",mdb_refpresupuesto);



    fitxersortidatxt = "<blockTable style=\"tabla\" colWidths=\"10cm, 2cm, 2cm, 3cm\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "	<td>Concepto</td>";
    fitxersortidatxt += "	<td>Cantidad</td>";
    fitxersortidatxt += "	<td>Precio Und.</td>";
    fitxersortidatxt += "	<td>Total</td>";
    fitxersortidatxt += "</tr>";

    QString l;
    linpresupuesto *linea;
    uint i = 0;
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        fitxersortidatxt += "<tr>";
        fitxersortidatxt += "	<td>"+linea->desclpresupuesto()+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->cantlpresupuesto().toFloat())+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->pvplpresupuesto().toFloat())+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->cantlpresupuesto().toFloat() * linea->pvplpresupuesto().toFloat())+"</td>";
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

    system("trml2pdf.py /tmp/presupuesto.rml > /tmp/pressupost.pdf");
    system("kpdf /tmp/pressupost.pdf");
} //end imprimirPresupuesto

