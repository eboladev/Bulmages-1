//
// C++ Implementation: PedidoProveedor
//
// Description:
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pedidoproveedor.h"
#include "company.h"
#include "descpedidoproveedor.h"

#include <qfile.h>

#include "fixed.h"

typedef QMap<QString, Fixed> base;

PedidoProveedor::PedidoProveedor(company *comp) {
    companyact=comp;
    vaciaPedidoProveedor();
}

PedidoProveedor::~PedidoProveedor() {}


void PedidoProveedor::borraPedidoProveedor() {
    if (mdb_idpedidoproveedor != "") {
        listalineas->borrar();
        listadescuentos->borrar();
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM pedidoproveedor WHERE idpedidoproveedor="+mdb_idpedidoproveedor);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
        vaciaPedidoProveedor();
        pintaPedidoProveedor();
    }// end if
}// end borraPedidoProveedor


void PedidoProveedor::vaciaPedidoProveedor() {
    mdb_idproveedor = "";
    mdb_idalmacen = "";
    mdb_idpedidoproveedor = "";
    mdb_numpedidoproveedor = "";
    mdb_fechapedidoproveedor = "";
    mdb_descpedidoproveedor = "";

    mdb_idforma_pago = "";
    mdb_comentpedidoproveedor = "";
    mdb_refpedidoproveedor = "";
    mdb_procesadopedidoproveedor = "FALSE";
    mdb_contactpedidoproveedor = "";
    mdb_telpedidoproveedor = "";
    mdb_idtrabajador = "";
    //    listalineas->vaciar();
}// end vaciaPedidoProveedor

void PedidoProveedor::pintaPedidoProveedor() {
    fprintf(stderr,"PedidoProveedor::pintaPedidoProveedor\n");
    pintaidproveedor(mdb_idproveedor);
    pintaidalmacen(mdb_idalmacen);
    pintaidpedidoproveedor(mdb_idpedidoproveedor);
    pintanumpedidoproveedor(mdb_numpedidoproveedor);
    pintafechapedidoproveedor(mdb_fechapedidoproveedor);
    pintadescpedidoproveedor(mdb_descpedidoproveedor);

    pintaidforma_pago(mdb_idforma_pago);

    pintacomentpedidoproveedor(mdb_comentpedidoproveedor);

    pintarefpedidoproveedor(mdb_refpedidoproveedor);
    pintaprocesadopedidoproveedor(mdb_procesadopedidoproveedor);
    pintacontactpedidoproveedor(mdb_contactpedidoproveedor);
    pintatelpedidoproveedor(mdb_telpedidoproveedor);
    pintaidtrabajador(mdb_idtrabajador);

    /// Pinta el subformulario de detalle del PedidoProveedor.
    listalineas->pintaListLinPedidoProveedor();
    // Pinta el subformulario de descuentos del pedidoproveedor
    listadescuentos->pintaListDescuentoPedidoProveedor();
    calculaypintatotales();
    fprintf(stderr,"FIN PedidoProveedor::pintaPedidoProveedor()\n");
}// end pintaPedidoProveedor


// Esta funci� carga un PedidoProveedor.
void PedidoProveedor::cargaPedidoProveedor(QString idbudget) {
    fprintf(stderr,"cargaPedidoProveedor(%s)\n",idbudget.ascii());
    mdb_idpedidoproveedor = idbudget;
    QString query = "SELECT * FROM pedidoproveedor WHERE idpedidoproveedor="+mdb_idpedidoproveedor;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        mdb_idproveedor = cur->valor("idproveedor");
        mdb_idalmacen = cur->valor("idalmacen");
        mdb_idpedidoproveedor = cur->valor("idpedidoproveedor");
        mdb_numpedidoproveedor = cur->valor("numpedidoproveedor");
        mdb_fechapedidoproveedor = cur->valor("fechapedidoproveedor");
        mdb_descpedidoproveedor = cur->valor("descpedidoproveedor");
        mdb_idforma_pago = cur->valor("idforma_pago");
        mdb_refpedidoproveedor = cur->valor("refpedidoproveedor");
	mdb_contactpedidoproveedor = cur->valor("contactpedidoproveedor");
	mdb_telpedidoproveedor = cur->valor("telpedidoproveedor");
	mdb_idtrabajador = cur->valor("idtrabajador");
        if (cur->valor("procesadopedidoproveedor") == "t")
            mdb_procesadopedidoproveedor = "TRUE";
        else
            mdb_procesadopedidoproveedor = "FALSE";

        mdb_comentpedidoproveedor = cur->valor("comentpedidoproveedor");
    }// end if
    delete cur;
    fprintf(stderr,"Vamos a cargar las lineas\n");
    listalineas->cargaListLinPedidoProveedor(idbudget);
    listadescuentos->cargaDescuentos(idbudget);
    fprintf(stderr,"vamos a hacer el pintado de la LineaPedido\n");
    pintaPedidoProveedor();
}// end chargeBudget


void PedidoProveedor::guardaPedidoProveedor() {
    companyact->begin();
    if (mdb_idforma_pago == "")
        mdb_idforma_pago = "NULL";
    if (mdb_numpedidoproveedor == "")
        mdb_numpedidoproveedor = "NULL";
    if (mdb_idtrabajador == "")
        mdb_idtrabajador = "NULL";
    if (mdb_idpedidoproveedor == "") {
        /// Se trata de una inserci�
        QString SQLQuery = "INSERT INTO pedidoproveedor (contactpedidoproveedor, telpedidoproveedor, numpedidoproveedor, fechapedidoproveedor, idproveedor, idalmacen, idforma_pago, refpedidoproveedor, procesadopedidoproveedor, descpedidoproveedor, comentpedidoproveedor, idtrabajador) VALUES ('"+
	companyact->sanearCadena(mdb_contactpedidoproveedor)+"','"+
	companyact->sanearCadena(mdb_telpedidoproveedor)+"',"+
	companyact->sanearCadena(mdb_numpedidoproveedor)+",'"+
	companyact->sanearCadena(mdb_fechapedidoproveedor)+"',"+
	companyact->sanearCadena(mdb_idproveedor)+","+
	companyact->sanearCadena(mdb_idalmacen)+","+
	companyact->sanearCadena(mdb_idforma_pago)+",'"+
	companyact->sanearCadena(mdb_refpedidoproveedor)+"',"+
	companyact->sanearCadena(mdb_procesadopedidoproveedor)+",'"+
	companyact->sanearCadena(mdb_descpedidoproveedor)+"','"+
	companyact->sanearCadena(mdb_comentpedidoproveedor)+"', "+
	companyact->sanearCadena(mdb_idtrabajador)+")";
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idpedidoproveedor) AS m FROM pedidoproveedor");
        if (!cur->eof())
            setidpedidoproveedor(cur->valor("m"));
        delete cur;
        companyact->commit();
    } else {
        /// Se trata de una modificaci�
        QString SQLQuery = "UPDATE pedidoproveedor SET ";
        SQLQuery += " numpedidoproveedor="+companyact->sanearCadena(mdb_numpedidoproveedor)+"";
        SQLQuery += " ,fechapedidoproveedor='"+companyact->sanearCadena(mdb_fechapedidoproveedor)+"'";
        SQLQuery += " ,idproveedor="+companyact->sanearCadena(mdb_idproveedor);
        SQLQuery += " ,idalmacen="+companyact->sanearCadena(mdb_idalmacen);
        SQLQuery += " ,idforma_pago="+companyact->sanearCadena(mdb_idforma_pago);
        SQLQuery += " ,refpedidoproveedor='"+companyact->sanearCadena(mdb_refpedidoproveedor)+"'";
        SQLQuery += " ,procesadopedidoproveedor="+companyact->sanearCadena(mdb_procesadopedidoproveedor);
        SQLQuery += " ,descpedidoproveedor='"+companyact->sanearCadena(mdb_descpedidoproveedor)+"'";
        SQLQuery += " ,comentpedidoproveedor='"+companyact->sanearCadena(mdb_comentpedidoproveedor)+"'";
        SQLQuery += " ,contactpedidoproveedor='"+companyact->sanearCadena(mdb_contactpedidoproveedor)+"'"; 
	SQLQuery += " ,telpedidoproveedor='"+companyact->sanearCadena(mdb_telpedidoproveedor)+"'"; 
	SQLQuery += " ,idtrabajador ="+companyact->sanearCadena(mdb_idtrabajador);
	SQLQuery += " WHERE idpedidoproveedor="+companyact->sanearCadena(mdb_idpedidoproveedor);
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
    listalineas->guardaListLinPedidoProveedor();
    listadescuentos->guardaListDescuentoPedidoProveedor();
    cargaPedidoProveedor(mdb_idpedidoproveedor);
}// end guardaPedidoProveedor





void PedidoProveedor::imprimirPedidoProveedor() {
    /// Copiamos el archivo
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"pedidoproveedor.rml";
    archivo = "cp "+archivo+" /tmp/pedidoproveedor.rml";
    system (archivo.ascii());

    /// Copiamos el logo
    archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";
    archivo = "cp "+archivo+" /tmp/logo.jpg";
    system (archivo.ascii());

    QFile file;
    file.setName( "/tmp/pedidoproveedor.rml" );
    file.open( IO_ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;
    // L�ea de totales del pedidoproveedor

    QString SQLQuery = "SELECT * FROM proveedor WHERE idproveedor="+mdb_idproveedor;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        buff.replace("[dirproveedor]",cur->valor("dirproveedor"));
        buff.replace("[poblproveedor]",cur->valor("poblproveedor"));
        buff.replace("[telproveedor]",cur->valor("telproveedor"));
        buff.replace("[nomproveedor]",cur->valor("nomproveedor"));
        buff.replace("[cifproveedor]",cur->valor("cifproveedor"));
    }// end if

    buff.replace("[numpedidoproveedor]",mdb_numpedidoproveedor);
    buff.replace("[fechapedidoproveedor]",mdb_fechapedidoproveedor);
    buff.replace("[comentpedidoproveedor]",mdb_comentpedidoproveedor);
    buff.replace("[descpedidoproveedor]",mdb_descpedidoproveedor);
    buff.replace("[refpedidoproveedor]",mdb_refpedidoproveedor);

    fitxersortidatxt = "<blockTable style=\"tabla\" colWidths=\"10cm, 2cm, 2cm, 3cm\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "	<td>Concepto</td>";
    fitxersortidatxt += "	<td>Cantidad</td>";
    fitxersortidatxt += "	<td>Precio Und.</td>";
    fitxersortidatxt += "	<td>Total</td>";
    fitxersortidatxt += "</tr>";

    QString l;
    LinPedidoProveedor *linea;
    uint i = 0;
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        fitxersortidatxt += "<tr>";
        fitxersortidatxt += "	<td>"+linea->desclpedidoproveedor()+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->cantlpedidoproveedor().toFloat())+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->pvplpedidoproveedor().toFloat())+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->cantlpedidoproveedor().toFloat() * linea->pvplpedidoproveedor().toFloat())+"</td>";
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
    system("trml2pdf.py /tmp/pedidoproveedor.rml > /tmp/pedidoproveedor.pdf");
    system("kpdf /tmp/pedidoproveedor.pdf");
} //end imprimirPedidoProveedor







void PedidoProveedor::calculaypintatotales() {
    fprintf(stderr,"calculaypintatotales \n");
    base basesimp;
    LinPedidoProveedor *linea;
    /// Impresi� de los contenidos
    QString l;
    
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
    	Fixed cant(linea->cantlpedidoproveedor().ascii());
	Fixed pvpund(linea->pvplpedidoproveedor().ascii());
	Fixed desc1(linea->descuentolpedidoproveedor().ascii());
	Fixed cantpvp = cant * pvpund;
	Fixed base = cantpvp - cantpvp * desc1 / 100;
        basesimp[linea->ivalpedidoproveedor()] =  basesimp[linea->ivalpedidoproveedor()]+ base;
    }// end for
    

    Fixed basei("0.00");
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei = basei + it.data();
    }// end for
    /// Impresi� de los descuentos
    Fixed porcentt("0.00");
    DescuentoPedidoProveedor *linea1;
    if (listadescuentos->m_lista.first()) {
        for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
	    Fixed propor(linea1->proporciondpedidoproveedor().ascii());
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
    pintatotales(totiva, totbaseimp, totiva+totbaseimp, basei*porcentt/100);}// end calculaypintatotales
