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


AlbaranCliente::AlbaranCliente(company *comp) {
	companyact=comp;
}


AlbaranCliente::~AlbaranCliente()
{
}

void AlbaranCliente::borraAlbaranCliente() {
    if (mdb_idalbaran != "") {
        listalineas->borrar();
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
    mdb_loginusuario="";
    mdb_comentalbaran="";
    mdb_idcliente="";
    mdb_idforma_pago="";
    mdb_idalmacen="";
    mdb_numfactura="";
    mdb_descalbaran="";
    mdb_refalbaran="";
}// end vaciaAlbaranCliente


void AlbaranCliente::pintaAlbaranCliente() {
    fprintf(stderr,"pintaAlbaranCliente\n");
    pintaIdAlbaran(mdb_idalbaran);
    pintaNumAlbaran(mdb_numalbaran);
    pintafechaalbaran(mdb_fechaalbaran);
    pintaloginusuario(mdb_loginusuario);
    pintaComentAlbaran(mdb_comentalbaran);
    pintaidcliente(mdb_idcliente);
    pintaidforma_pago(mdb_idforma_pago);
    pintaidalmacen(mdb_idalmacen);
    pintarefalbaran(mdb_refalbaran);
    pintadescalbaran(mdb_descalbaran);
    /// Pinta el subformulario de detalle del AlbaranCliente.
    listalineas->pintaListLinAlbaranCliente();
    /// Pintamos los totales
    pintatotales(listalineas->calculabase(), listalineas->calculaiva());
}// end pintaAlbaranCliente



// Esta funciï¿½ carga un AlbaranCliente.
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
        mdb_loginusuario = cur->valor("loginusuario");
	mdb_idforma_pago = cur->valor("idforma_pago");
	mdb_descalbaran = cur->valor("descalbaran");
	mdb_refalbaran = cur->valor("refalbaran");
    }// end if
    delete cur;
    listalineas->cargaListLinAlbaranCliente(idbudget);
    pintaAlbaranCliente();
}// end chargeBudget



void AlbaranCliente::guardaAlbaranCliente() {
    /// Todo el guardado es una transacción
    companyact->begin();
    if (mdb_loginusuario=="")
    	mdb_loginusuario="NULL";
    else
    	mdb_loginusuario="'"+mdb_loginusuario+"'";
    if (mdb_numalbaran=="") 
    	mdb_numalbaran="NULL";
    if (mdb_idalbaran == "") {
        /// Se trata de una inserción
        QString SQLQuery = "INSERT INTO albaran (numalbaran, fechaalbaran, loginusuario, comentalbaran, idcliente, idforma_pago, idalmacen, descalbaran, refalbaran) VALUES ("+mdb_numalbaran+",'"+mdb_fechaalbaran+"',"+mdb_loginusuario+",'"+mdb_comentalbaran+"',"+mdb_idcliente+","+mdb_idforma_pago+","+mdb_idalmacen+",'"+mdb_descalbaran+"','"+mdb_refalbaran+"')";
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
        SQLQuery += " ,loginusuario="+mdb_loginusuario+"";
        SQLQuery += " ,comentalbaran='"+mdb_comentalbaran+"'";
        SQLQuery += " ,idcliente="+mdb_idcliente+"";
        SQLQuery += " ,idforma_pago="+mdb_idforma_pago;
        SQLQuery += " ,idalmacen="+mdb_idalmacen;
	SQLQuery += " ,descalbaran='"+mdb_descalbaran+"'";
	SQLQuery += " ,refalbaran='"+mdb_refalbaran+"'";
        SQLQuery += " WHERE idalbaran="+mdb_idalbaran;
        companyact->ejecuta(SQLQuery);
    }// end if
       companyact->commit();
       listalineas->guardaListLinAlbaranCliente();
       cargaAlbaranCliente(mdb_idalbaran);
}// end guardaAlbaranCliente


void AlbaranCliente::imprimirAlbaranCliente() {
    /// Copiamos el archivo
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"albarancliente.rml";
    archivo = "cp "+archivo+" /tmp/albarancliente.rml";
    system (archivo.ascii());
    
    /// Copiamos el logo
    archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";
    archivo = "cp "+archivo+" /tmp/logo.jpg";
    system (archivo.ascii());
    
    
    QFile file;
    file.setName( "/tmp/albarancliente.rml" );
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

    buff.replace("[numalbaran]",mdb_numalbaran);
    buff.replace("[fechaalbaran]",mdb_fechaalbaran);
    buff.replace("[comentalbaran]",mdb_comentalbaran);
    buff.replace("[descalbaran]",mdb_descalbaran);
    buff.replace("[refalbaran]",mdb_refalbaran);

    fitxersortidatxt = "<blockTable style=\"tabla\" colWidths=\"10cm, 2cm, 2cm, 3cm\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "	<td>Concepto</td>";
    fitxersortidatxt += "	<td>Cantidad</td>";
    fitxersortidatxt += "	<td>Precio Und.</td>";
    fitxersortidatxt += "	<td>Total</td>";
    fitxersortidatxt += "</tr>";

    QString l;
    LinAlbaranCliente *linea;
    uint i = 0;
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        fitxersortidatxt += "<tr>";
        fitxersortidatxt += "	<td>"+linea->desclalbaran()+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->cantlalbaran().toFloat())+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->pvplalbaran().toFloat())+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->cantlalbaran().toFloat() * linea->pvplalbaran().toFloat())+"</td>";
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

    system("trml2pdf.py /tmp/albarancliente.rml > /tmp/albarancliente.pdf");
    system("kpdf /tmp/albarancliente.pdf");
} //end imprimirAlbaranCliente




