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

#include <QFile>
//Added by qt3to4:
#include <QTextStream>

#include "fixed.h"
#include "funcaux.h"

typedef QMap<QString, Fixed> base;

PedidoProveedor::PedidoProveedor(company *comp) : DBRecord(comp) {
    companyact=comp;

    setDBTableName("pedidoproveedor");
    setDBCampoId("idpedidoproveedor");
    addDBCampo("idpedidoproveedor", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Presupuesto");
    addDBCampo("idproveedor", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("numpedidoproveedor", DBCampo::DBint, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("fechapedidoproveedor", DBCampo::DBdate, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("comentpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("procesadopedidoproveedor", DBCampo::DBboolean, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("descpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("refpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("contactpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("telpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
}

PedidoProveedor::~PedidoProveedor() {}


void PedidoProveedor::borraPedidoProveedor() {
    if (DBvalue("idpedidoproveedor") != "") {
        listalineas->borrar();
        listadescuentos->borrar();
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM pedidoproveedor WHERE idpedidoproveedor="+DBvalue("idpedidoproveedor"));
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
    DBclear();
}// end vaciaPedidoProveedor

void PedidoProveedor::pintaPedidoProveedor() {
    _depura("PedidoProveedor::pintaPedidoProveedor\n",0);
    pintaidproveedor(DBvalue("idproveedor"));
    pintaidalmacen(DBvalue("idalmacen"));
    pintaidpedidoproveedor(DBvalue("idpedidoproveedor"));
    pintanumpedidoproveedor(DBvalue("numpedidoproveedor"));
    pintafechapedidoproveedor(DBvalue("fechapedidoproveedor"));
    pintadescpedidoproveedor(DBvalue("descpedidoproveedor"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintacomentpedidoproveedor(DBvalue("comentpedidoproveedor"));
    pintarefpedidoproveedor(DBvalue("refpedidoproveedor"));
    pintaprocesadopedidoproveedor(DBvalue("procesadopedidoproveedor"));
    pintacontactpedidoproveedor(DBvalue("contactpedidoproveedor"));
    pintatelpedidoproveedor(DBvalue("telpedidoproveedor"));
    pintaidtrabajador(DBvalue("idtrabajador"));
    /// Pinta el subformulario de detalle del PedidoProveedor.
    listalineas->pintaListLinPedidoProveedor();
    // Pinta el subformulario de descuentos del pedidoproveedor
    listadescuentos->pintaListDescuentoPedidoProveedor();
    calculaypintatotales();
    _depura("FIN PedidoProveedor::pintaPedidoProveedor()\n",0);
}// end pintaPedidoProveedor


// Esta funcion carga un PedidoProveedor.
int PedidoProveedor::cargaPedidoProveedor(QString idbudget) {
    _depura("cargaPedidoProveedor()\n",0);
    QString query = "SELECT * FROM pedidoproveedor WHERE idpedidoproveedor="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    }// end if
    delete cur;
    listalineas->cargaListLinPedidoProveedor(idbudget);
    listadescuentos->cargaDescuentos(idbudget);
    pintaPedidoProveedor();
    return 0;
}// end chargeBudget


void PedidoProveedor::guardaPedidoProveedor() {
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return;
    }// end if
    setidpedidoproveedor(id);
    companyact->commit();
    listalineas->guardaListLinPedidoProveedor();
    listadescuentos->guardaListDescuentoPedidoProveedor();
    cargaPedidoProveedor(id);
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
    file.open( QIODevice::ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;
    // L�ea de totales del pedidoproveedor

    QString SQLQuery = "SELECT * FROM proveedor WHERE idproveedor="+DBvalue("idproveedor");
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        buff.replace("[dirproveedor]",cur->valor("dirproveedor"));
        buff.replace("[poblproveedor]",cur->valor("poblproveedor"));
        buff.replace("[telproveedor]",cur->valor("telproveedor"));
        buff.replace("[nomproveedor]",cur->valor("nomproveedor"));
        buff.replace("[cifproveedor]",cur->valor("cifproveedor"));
    }// end if

    buff.replace("[numpedidoproveedor]",DBvalue("numpedidoproveedor"));
    buff.replace("[fechapedidoproveedor]",DBvalue("fechapedidoproveedor"));
    buff.replace("[comentpedidoproveedor]",DBvalue("comentpedidoproveedor"));
    buff.replace("[descpedidoproveedor]",DBvalue("descpedidoproveedor"));
    buff.replace("[refpedidoproveedor]",DBvalue("refpedidoproveedor"));

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

    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }
    system("trml2pdf.py /tmp/pedidoproveedor.rml > /tmp/pedidoproveedor.pdf");
    system("kpdf /tmp/pedidoproveedor.pdf");
} //end imprimirPedidoProveedor







void PedidoProveedor::calculaypintatotales() {
    _depura("calculaypintatotales \n",0);
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
    pintatotales(totiva, totbaseimp, totiva+totbaseimp, basei*porcentt/100);
}// end calculaypintatotales
