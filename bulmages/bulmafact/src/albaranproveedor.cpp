//
// C++ Implementation: albaranpproveedor
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "albaranproveedor.h"

#include <qfile.h>
//Added by qt3to4:
#include <QTextStream>


AlbaranProveedor::AlbaranProveedor(company *comp) {
    companyact=comp;
}


AlbaranProveedor::~AlbaranProveedor() {}

void AlbaranProveedor::borraAlbaranProveedor() {
    if (mdb_idalbaranp != "") {
        listalineas->borrar();
        listadescuentos->borrar();
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM albaranp WHERE idalbaranp="+mdb_idalbaranp);
        if (error) {
            companyact->rollback();
            return;
        }// end if
        companyact->commit();
        vaciaAlbaranProveedor();
        pintaAlbaranProveedor();
    }// end if
}// end borraAlbaranProveedor


void AlbaranProveedor::vaciaAlbaranProveedor() {
    mdb_idalbaranp="";
    mdb_numalbaranp="";
    mdb_fechaalbaranp="";
    mdb_loginusuario="";
    mdb_comentalbaranp="";
    mdb_idproveedor="";
    mdb_idforma_pago="";
    mdb_idalmacen="";
    mdb_numfactura="";
    mdb_descalbaranp="";
    mdb_refalbaranp="";
}// end vaciaAlbaranProveedor


void AlbaranProveedor::pintaAlbaranProveedor() {
    fprintf(stderr,"pintaAlbaranProveedor\n");
    pintaidalbaranp(mdb_idalbaranp);
    pintanumalbaranp(mdb_numalbaranp);
    pintafechaalbaranp(mdb_fechaalbaranp);
    pintaloginusuario(mdb_loginusuario);
    pintacomentalbaranp(mdb_comentalbaranp);
    pintaidproveedor(mdb_idproveedor);
    pintaidforma_pago(mdb_idforma_pago);
    pintaidalmacen(mdb_idalmacen);
    pintarefalbaranp(mdb_refalbaranp);
    pintadescalbaranp(mdb_descalbaranp);
    /// Pinta el subformulario de detalle del AlbaranProveedor.
    listalineas->pintaListLinAlbaranProveedor();
    listadescuentos->pintaListDescuentoAlbaranProv();
    /// Pintamos los totales
    pintatotales(listalineas->calculabase(), listalineas->calculaiva());
}// end pintaAlbaranProveedor



// Esta funci� carga un AlbaranProveedor.
int AlbaranProveedor::cargaAlbaranProveedor(QString idbudget) {
    fprintf(stderr,"AlbaranProveedor::cargaAlbaranProveedor(%s)\n",idbudget.ascii());
    mdb_idalbaranp = idbudget;
    QString query = "SELECT * FROM albaranp WHERE idalbaranp="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        mdb_idproveedor= cur->valor("idproveedor");
        mdb_idalmacen= cur->valor("idalmacen");
        mdb_numalbaranp= cur->valor("numalbaranp");
        mdb_fechaalbaranp= cur->valor("fechaalbaranp");
        mdb_comentalbaranp= cur->valor("comentalbaranp");
        mdb_loginusuario = "";
        mdb_idforma_pago = cur->valor("idforma_pago");
        mdb_descalbaranp = cur->valor("descalbaranp");
        mdb_refalbaranp = cur->valor("refalbaranp");
    }// end if
    delete cur;
    listalineas->cargaListLinAlbaranProveedor(idbudget);
    listadescuentos->cargaDescuentos(idbudget);
    pintaAlbaranProveedor();
    return 0;
}// end chargeBudget



void AlbaranProveedor::guardaAlbaranProveedor() {
    /// Todo el guardado es una transacci�
    companyact->begin();
    if (mdb_loginusuario=="")
        mdb_loginusuario="NULL";
    else
        mdb_loginusuario="'"+mdb_loginusuario+"'";
    if (mdb_numalbaranp=="")
        mdb_numalbaranp="NULL";
    if (mdb_idalbaranp == "") {
        /// Se trata de una inserci�
        QString SQLQuery = "INSERT INTO albaranp (numalbaranp, fechaalbaranp, comentalbaranp, idproveedor, idforma_pago, idalmacen, descalbaranp, refalbaranp) VALUES ("+mdb_numalbaranp+",'"+mdb_fechaalbaranp+"','"+mdb_comentalbaranp+"',"+mdb_idproveedor+","+mdb_idforma_pago+","+mdb_idalmacen+",'"+mdb_descalbaranp+"','"+mdb_refalbaranp+"')";
        int error = companyact->ejecuta(SQLQuery);
        if (error) {
            companyact->rollback();
            return;
        }// end if
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idalbaranp) AS m FROM albaranp");
        if (!cur->eof())
            setidalbaranp(cur->valor("m"));
        delete cur;
    } else {
        /// Se trata de una modificacion
        QString SQLQuery = "UPDATE albaranp SET ";
        SQLQuery += " numalbaranp="+mdb_numalbaranp;
        SQLQuery += " ,fechaalbaranp='"+mdb_fechaalbaranp+"'";
        SQLQuery += " ,comentalbaranp='"+mdb_comentalbaranp+"'";
        SQLQuery += " ,idproveedor="+mdb_idproveedor+"";
        SQLQuery += " ,idforma_pago="+mdb_idforma_pago;
        SQLQuery += " ,idalmacen="+mdb_idalmacen;
        SQLQuery += " ,descalbaranp='"+mdb_descalbaranp+"'";
        SQLQuery += " ,refalbaranp='"+mdb_refalbaranp+"'";
        SQLQuery += " WHERE idalbaranp="+mdb_idalbaranp;
        int error = companyact->ejecuta(SQLQuery);
        if (error) {
            companyact->rollback();
            return;
        }// end if
    }// end if
    companyact->commit();
    listalineas->guardaListLinAlbaranProveedor();
    listadescuentos->guardaListDescuentoAlbaranProv();
    cargaAlbaranProveedor(mdb_idalbaranp);
}// end guardaAlbaranProveedor


void AlbaranProveedor::imprimirAlbaranProveedor() {
    /// Copiamos el archivo
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"albaranpproveedor.rml";
    archivo = "cp "+archivo+" /tmp/albaranpproveedor.rml";
    system (archivo.ascii());

    /// Copiamos el logo
    archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";
    archivo = "cp "+archivo+" /tmp/logo.jpg";
    system (archivo.ascii());


    QFile file;
    file.setName( "/tmp/albaranpproveedor.rml" );
    file.open( QIODevice::ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;
    // L�ea de totales del presupuesto

    QString SQLQuery = "SELECT * FROM proveedor WHERE idproveedor="+mdb_idproveedor;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        buff.replace("[dirproveedor]",cur->valor("dirproveedor"));
        buff.replace("[poblproveedor]",cur->valor("poblproveedor"));
        buff.replace("[telproveedor]",cur->valor("telproveedor"));
        buff.replace("[nomproveedor]",cur->valor("nomproveedor"));
        buff.replace("[cifproveedor]",cur->valor("cifproveedor"));
    }// end if

    buff.replace("[numalbaranp]",mdb_numalbaranp);
    buff.replace("[fechaalbaranp]",mdb_fechaalbaranp);
    buff.replace("[comentalbaranp]",mdb_comentalbaranp);
    buff.replace("[descalbaranp]",mdb_descalbaranp);
    buff.replace("[refalbaranp]",mdb_refalbaranp);

    fitxersortidatxt = "<blockTable style=\"tabla\" colWidths=\"10cm, 2cm, 2cm, 3cm\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "	<td>Concepto</td>";
    fitxersortidatxt += "	<td>Cantidad</td>";
    fitxersortidatxt += "	<td>Precio Und.</td>";
    fitxersortidatxt += "	<td>Total</td>";
    fitxersortidatxt += "</tr>";

    QString l;
    LinAlbaranProveedor *linea;
    uint i = 0;
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        fitxersortidatxt += "<tr>";
        fitxersortidatxt += "	<td>"+linea->desclalbaranp()+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->cantlalbaranp().toFloat())+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->pvplalbaranp().toFloat())+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->cantlalbaranp().toFloat() * linea->pvplalbaranp().toFloat())+"</td>";
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
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }

    system("trml2pdf.py /tmp/albaranpproveedor.rml > /tmp/albaranpproveedor.pdf");
    system("kpdf /tmp/albaranpproveedor.pdf &");
} //end imprimirAlbaranProveedor




