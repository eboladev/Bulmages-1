//
// C++ Implementation: FacturaProveedor
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "facturap.h"
#include "company.h"
#include <qfile.h>
//Added by qt3to4:
#include <QTextStream>

FacturaProveedor::FacturaProveedor(company *comp) {
    companyact=comp;
    vaciaFacturaProveedor();
}

FacturaProveedor::~FacturaProveedor() {}


void FacturaProveedor::borraFacturaProveedor() {
    if (mdb_idfacturap != "") {
        listalineas->borrar();
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM facturap WHERE idfacturap="+mdb_idfacturap);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
        vaciaFacturaProveedor();
        pintaFacturaProveedor();
    }// end if
}// end borraFacturaProveedor


void FacturaProveedor::vaciaFacturaProveedor() {
    mdb_idfacturap = "";
    mdb_idproveedor= "";
    mdb_idalmacen= "";
    mdb_numfacturap= "";
    mdb_ffacturap= "";
    mdb_reffacturap = "";
    mdb_comentfacturap= "";
    mdb_procesadafacturap="FALSE";
    mdb_codigoserie_facturap="";
    mdb_idforma_pago="";
    mdb_descfacturap="";
}// end vaciaFacturaProveedor

void FacturaProveedor::pintaFacturaProveedor() {
    fprintf(stderr,"pintaFacturaProveedor\n");
    pintaidproveedor(mdb_idproveedor);
    pintaidalmacen(mdb_idalmacen);
    pintanumfacturap(mdb_numfacturap);
    pintafechafacturap(mdb_ffacturap);
    pintareffacturap(mdb_reffacturap);
    pintacomentfacturap(mdb_comentfacturap);
    pintaprocesadafacturap(mdb_procesadafacturap);
    pintacodigoserie_facturap(mdb_codigoserie_facturap);
    pintaidforma_pago(mdb_idforma_pago);
    pintadescfacturap(mdb_descfacturap);
    // Pinta el subformulario de detalle del FacturaProveedor.
    listalineas->pintaListLinFacturaProveedor();
    listadescuentos->pintaListDescuentoFacturaProv();

    pintatotales(listalineas->calculabase(), listalineas->calculaiva());
}// end pintaFacturaProveedor


// Esta funci� carga un FacturaProveedor.
void FacturaProveedor::cargaFacturaProveedor(QString idbudget) {
    mdb_idfacturap = idbudget;
    inicialize();
    QString query = "SELECT * FROM facturap  WHERE idfacturap="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        mdb_idproveedor= cur->valor("idproveedor");
        mdb_idalmacen= cur->valor("idalmacen");
        mdb_numfacturap= cur->valor("numfacturap");
        mdb_ffacturap= cur->valor("ffacturap");
	mdb_codigoserie_facturap = cur->valor("codigoserie_facturap");

        mdb_comentfacturap= cur->valor("comentfacturap");
        mdb_reffacturap = cur->valor("reffacturap");
        if (cur->valor("procesadafacturap") == "t")
            mdb_procesadafacturap = "TRUE";
        else
            mdb_procesadafacturap = "FALSE";
        mdb_idusuari = cur->valor("idusuari");
        mdb_idforma_pago = cur->valor("idforma_pago");
	mdb_descfacturap  = cur->valor("descfacturap");
    }// end if
    delete cur;
    listalineas->cargaListLinFacturaProveedor(idbudget);
    listadescuentos->cargaDescuentos(idbudget);
    pintaFacturaProveedor();
}// end chargeBudget


void FacturaProveedor::guardaFacturaProveedor() {
    companyact->begin();
    if (mdb_idusuari=="")
        mdb_idusuari="NULL";
    if (mdb_idforma_pago == "")
        mdb_idforma_pago = "NULL";
    if (mdb_idfacturap == "") {
        /// Se trata de una inserci�
        QString SQLQuery = "INSERT INTO facturap (descfacturap, procesadafacturap, reffacturap, numfacturap, ffacturap, comentfacturap, idproveedor, idforma_pago) VALUES ('"+
	companyact->sanearCadena(mdb_descfacturap)+"',"+
	companyact->sanearCadena(mdb_procesadafacturap)+",'"+
	companyact->sanearCadena(mdb_reffacturap)+"','"+
	companyact->sanearCadena(mdb_numfacturap)+"','"+
	companyact->sanearCadena(mdb_ffacturap)+"','"+
	companyact->sanearCadena(mdb_comentfacturap)+"',"+
	companyact->sanearCadena(mdb_idproveedor)+","+
	companyact->sanearCadena(mdb_idforma_pago)+")";

        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idfacturap) AS m FROM facturap");
        if (!cur->eof())
            setidfacturap (cur->valor("m"));
        delete cur;
        companyact->commit();
    } else {
        /// Se trata de una modificaci�
        QString SQLQuery = "UPDATE facturap SET ";
        SQLQuery += " numfacturap="+companyact->sanearCadena(mdb_numfacturap);
        SQLQuery += " ,ffacturap='"+companyact->sanearCadena(mdb_ffacturap)+"'";
        SQLQuery += " ,comentfacturap='"+companyact->sanearCadena(mdb_comentfacturap)+"'";
        SQLQuery += " ,idproveedor="+companyact->sanearCadena(mdb_idproveedor);
        SQLQuery += " ,reffacturap='"+companyact->sanearCadena(mdb_reffacturap)+"'";
        SQLQuery += " ,idforma_pago="+companyact->sanearCadena(mdb_idforma_pago);
        SQLQuery += " ,procesadafacturap="+companyact->sanearCadena(mdb_procesadafacturap);
	SQLQuery += " ,descfacturap='"+companyact->sanearCadena(mdb_descfacturap)+"'";
        SQLQuery += " WHERE idfacturap="+companyact->sanearCadena(mdb_idfacturap);
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
    listalineas->guardaListLinFacturaProveedor();
    listadescuentos->guardaListDescuentoFacturaProv();
    cargaFacturaProveedor(mdb_idfacturap);
}// end guardaFacturaProveedor


void FacturaProveedor::imprimirFacturaProveedor() {
    /// Copiamos el archivo
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"facturap.rml";
    archivo = "cp "+archivo+" /tmp/facturap.rml";
    system (archivo.ascii());
    
    /// Copiamos el logo
    archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";
    archivo = "cp "+archivo+" /tmp/logo.jpg";
    system (archivo.ascii());
    
    QFile file;
    file.setName( "/tmp/facturap.rml" );
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
    buff.replace("[numfacturap]",mdb_numfacturap);
    buff.replace("[ffacturap]",mdb_ffacturap);
    buff.replace("[comentfacturap]",mdb_comentfacturap);
    buff.replace("[descfacturap]",mdb_descfacturap);
    buff.replace("[reffacturap]",mdb_reffacturap);
    buff.replace("[codigoserie_facturap]",mdb_codigoserie_facturap);
    fitxersortidatxt = "<blockTable style=\"tabla\" colWidths=\"10cm, 2cm, 2cm, 3cm\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "	<td>Concepto</td>";
    fitxersortidatxt += "	<td>Cantidad</td>";
    fitxersortidatxt += "	<td>Precio Und.</td>";
    fitxersortidatxt += "	<td>Total</td>";
    fitxersortidatxt += "</tr>";

    QString l;
    LinFacturaProveedor *linea;
    uint i = 0;
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        fitxersortidatxt += "<tr>";
        fitxersortidatxt += "	<td>"+linea->desclfacturap()+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->cantlfacturap().toFloat())+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->pvplfacturap().toFloat())+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->cantlfacturap().toFloat() * linea->pvplfacturap().toFloat())+"</td>";
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
    }// end if
    system("trml2pdf.py /tmp/facturap.rml > /tmp/facturap.pdf");
    system("kpdf /tmp/facturap.pdf");
} //end imprimirFacturaProveedor



