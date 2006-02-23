//
// C++ Implementation: PedidoCliente
//
// Description:
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pedidocliente.h"
#include "company.h"
#include "descpedidocliente.h"

#include <QFile>
//Added by qt3to4:
#include <QTextStream>

#include "fixed.h"
#include "funcaux.h"

typedef QMap<QString, Fixed> base;

PedidoCliente::PedidoCliente(company *comp) : DBRecord(comp) {
    companyact=comp;

    setDBTableName("pedidocliente");
    setDBCampoId("idpedidocliente");
    addDBCampo("idpedidocliente", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Presupuesto");
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("numpedidocliente", DBCampo::DBint, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("fechapedidocliente", DBCampo::DBdate, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("descpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("contactpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("telpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("comentpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("procesadopedidocliente", DBCampo::DBboolean, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("refpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
}

PedidoCliente::~PedidoCliente() {}


void PedidoCliente::borraPedidoCliente() {
    if (DBvalue("idpedidocliente") != "") {
        listalineas->borrar();
        listadescuentos->borrar();
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM pedidocliente WHERE idpedidocliente="+DBvalue("idpedidocliente"));
        if (error) {
            companyact->rollback();
            return;
        }// end if
        companyact->commit();
        vaciaPedidoCliente();
        pintaPedidoCliente();
    }// end if
}// end borraPedidoCliente


void PedidoCliente::vaciaPedidoCliente() {
    DBclear();
}// end vaciaPedidoCliente

void PedidoCliente::pintaPedidoCliente() {
    _depura("PedidoCliente::pintaPedidoCliente\n",0);
    pintaidcliente(DBvalue("idcliente"));
    pintaidalmacen(DBvalue("idalmacen"));
    pintaidpedidocliente(DBvalue("idpedidocliente"));
    pintanumpedidocliente(DBvalue("numpedidocliente"));
    pintafechapedidocliente(DBvalue("fechapedidocliente"));
    pintadescpedidocliente(DBvalue("descpedidocliente"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintacomentpedidocliente(DBvalue("comentpedidocliente"));
    pintarefpedidocliente(DBvalue("refpedidocliente"));
    pintaprocesadopedidocliente(DBvalue("procesadopedidocliente"));
    pintacontactpedidocliente(DBvalue("contactpedidocliente"));
    pintatelpedidocliente(DBvalue("telpedidocliente"));
    pintaidtrabajador(DBvalue("idtrabajador"));

    /// Pinta el subformulario de detalle del PedidoCliente.
    listalineas->pintaListLinPedidoCliente();
    /// Pinta el subformulario de descuentos del pedidocliente
    listadescuentos->pintaListDescuentoPedidoCliente();
    calculaypintatotales();
    _depura("FIN PedidoCliente::pintaPedidoCliente()\n",0);
}// end pintaPedidoCliente


// Esta funci� carga un PedidoCliente.
int PedidoCliente::cargaPedidoCliente(QString idbudget) {
    _depura("cargaPedidoCliente()\n",0);
    QString query = "SELECT * FROM pedidocliente WHERE idpedidocliente="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    }// end if
    delete cur;
    listalineas->cargaListLinPedidoCliente(idbudget);
    listadescuentos->cargaDescuentos(idbudget);
    pintaPedidoCliente();
	return 0;
}// end chargeBudget


void PedidoCliente::guardaPedidoCliente() {
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return;
    }// end if
    setidpedidocliente(id);
    companyact->commit();
    listalineas->guardaListLinPedidoCliente();
    listadescuentos->guardaListDescuentoPedidoCliente();
    cargaPedidoCliente(id);
}// end guardaPedidoCliente



void PedidoCliente::imprimirPedidoCliente() {

    base basesimp;

    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"pedidocliente.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"pedidocliente.rml";
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

    archivologo = "copy "+archivologo+" "+confpr->valor(CONF_DIR_USER)+"logo.jpg";
#else

    archivologo = "cp "+archivologo+" "+confpr->valor(CONF_DIR_USER)+"logo.jpg";
#endif

    system (archivologo.ascii());


    QFile file;
    file.setName( archivod );
    file.open( QIODevice::ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt="";
    // L�ea de totales del presupuesto

    QString SQLQuery = "SELECT * FROM cliente WHERE idcliente="+DBvalue("idcliente");
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        buff.replace("[dircliente]",cur->valor("dircliente"));
        buff.replace("[poblcliente]",cur->valor("poblcliente"));
        buff.replace("[telcliente]",cur->valor("telcliente"));
        buff.replace("[nomcliente]",cur->valor("nomcliente"));
        buff.replace("[cifcliente]",cur->valor("cifcliente"));
        buff.replace("[idcliente]",cur->valor("idcliente"));
        buff.replace("[cpcliente]",cur->valor("cpcliente"));
    }// end if
    delete cur;

    buff.replace("[numpedidocliente]",DBvalue("numpedidocliente"));
    buff.replace("[fechapedidocliente]",DBvalue("fechapedidocliente"));
    buff.replace("[contactpedidocliente]",DBvalue("contactpedidocliente"));
    buff.replace("[telpedidocliente]",DBvalue("telpedidocliente"));
    buff.replace("[comentpedidocliente]",DBvalue("comentpedidocliente"));
    buff.replace("[descpedidocliente]",DBvalue("descpedidocliente"));
    buff.replace("[refpedidocliente]",DBvalue("refpedidocliente"));


    LinPedidoCliente *linea;
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
        Fixed base = Fixed(linea->cantlpedidocliente().ascii()) * Fixed(linea->pvplpedidocliente().ascii());
        basesimp[linea->ivalpedidocliente()] = basesimp[linea->ivalpedidocliente()] + base - base * Fixed(linea->descuentolpedidocliente().ascii()) /100;

        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>"+linea->codigocompletoarticulo()+"</td>\n";
        fitxersortidatxt += "	<td>"+linea->desclpedidocliente()+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->cantlpedidocliente().ascii())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->pvplpedidocliente().ascii())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->descuentolpedidocliente().ascii())+" %</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",(base - base * Fixed (linea->descuentolpedidocliente()) /100).toQString().ascii())+"</td>\n";
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
    DescuentoPedidoCliente *linea1;
    if (listadescuentos->m_lista.first()) {
        fitxersortidatxt += "<blockTable style=\"tabladescuento\" colWidths=\"12cm, 2cm, 3cm\" repeatRows=\"1\">\n";
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>Descuento</td>\n";
        fitxersortidatxt += "	<td>Porcentaje</td>\n";
        fitxersortidatxt += "	<td>Total</td>\n";
        fitxersortidatxt += "</tr>\n";
        for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
            porcentt = porcentt + Fixed(linea1->proporciondpedidocliente().ascii());
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "	<td>"+linea1->conceptdpedidocliente()+"</td>\n";
            fitxersortidatxt += "	<td>"+l.sprintf("%s",linea1->proporciondpedidocliente().ascii())+" %</td>\n";
            fitxersortidatxt += "	<td>"+l.sprintf("-%s",( Fixed(linea1->proporciondpedidocliente())*basei/100).toQString().ascii())+"</td>\n";
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


    invocaPDF("pedidocliente");

} //end imprimirPresupuesto


void PedidoCliente::calculaypintatotales() {
    fprintf(stderr,"calculaypintatotales \n");
    base basesimp;
    LinPedidoCliente *linea;
    /// Impresi� de los contenidos
    QString l;

    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        Fixed cant(linea->cantlpedidocliente().ascii());
        Fixed pvpund(linea->pvplpedidocliente().ascii());
        Fixed desc1(linea->descuentolpedidocliente().ascii());
        Fixed cantpvp = cant * pvpund;
        Fixed base = cantpvp - cantpvp * desc1 / 100;
        basesimp[linea->ivalpedidocliente()] =  basesimp[linea->ivalpedidocliente()]+ base;
    }// end for


    Fixed basei("0.00");
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei = basei + it.data();
    }// end for
    /// Impresi� de los descuentos
    Fixed porcentt("0.00");
    DescuentoPedidoCliente *linea1;
    if (listadescuentos->m_lista.first()) {
        for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
            Fixed propor(linea1->proporciondpedidocliente().ascii());
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
