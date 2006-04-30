//
// C++ Implementation: PedidoCliente
//
// Description:
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <QFile>
#include <QTextStream>

#include "pedidocliente.h"
#include "company.h"
#include "fixed.h"
#include "funcaux.h"

typedef QMap<QString, Fixed> base;

PedidoCliente::PedidoCliente(company *comp) : DBRecord(comp) {
    companyact=comp;
    setDBTableName("pedidocliente");
    setDBCampoId("idpedidocliente");
    addDBCampo("idpedidocliente", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador");
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, "Cliente");
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, "Almacen");
    addDBCampo("numpedidocliente", DBCampo::DBint, DBCampo::DBNothing, "Número Pedido Cliente");
    addDBCampo("fechapedidocliente", DBCampo::DBdate, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("descpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Fecha");
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, "Forma Pago");
    addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNothing, "Trabajador");
    addDBCampo("contactpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Contacto");
    addDBCampo("telpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Teléfono");
    addDBCampo("comentpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Comentarios");
    addDBCampo("procesadopedidocliente", DBCampo::DBboolean, DBCampo::DBNothing, "Procesado");
    addDBCampo("refpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Referencia");
}

PedidoCliente::~PedidoCliente() {}


int PedidoCliente::borrar() {
    if (DBvalue("idpedidocliente") != "") {
        companyact->begin();
        listalineas->borrar();
        listadescuentos->borrar();
        int error = companyact->ejecuta("DELETE FROM pedidocliente WHERE idpedidocliente="+DBvalue("idpedidocliente"));
        if (error) {
            companyact->rollback();
            return -1;
        }// end if
        companyact->commit();
    }// end if
    return 0;
}// end borraPedidoCliente


void PedidoCliente::vaciaPedidoCliente() {
    DBclear();
}// end vaciaPedidoCliente

void PedidoCliente::pintar() {
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

    calculaypintatotales();
    _depura("FIN PedidoCliente::pintaPedidoCliente()\n",0);
}// end pintaPedidoCliente


// Esta funci� carga un PedidoCliente.
int PedidoCliente::cargar(QString idbudget) {
    _depura("cargaPedidoCliente()\n",0);
    QString query = "SELECT * FROM pedidocliente WHERE idpedidocliente="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    }// end if
    delete cur;
    listalineas->cargar(idbudget);
    listadescuentos->cargar(idbudget);
    pintar();
    return 0;
}// end chargeBudget


int PedidoCliente::guardar() {
    _depura("PedidoCliente::guardar",0);
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return -1;
    }// end if
    setidpedidocliente(id);
    error = listalineas->guardar();
    if (error ) {
        companyact->rollback();
        return -1;
    }// end if
    error = listadescuentos->guardar();
    if (error ) {
        companyact->rollback();
        return -1;
    }// end if
    companyact->commit();
    _depura("END PedidoCliente::guardar",0);
    return 0;
}// end guardaPedidoCliente



void PedidoCliente::imprimirPedidoCliente() {
    _depura("PedidoCliente::imprimirPedidoCliente",0);

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

    SDBRecord *linea;
    for ( int i = 0; i < listalineas->rowCount(); ++i) {
	linea = listalineas->lineaat(i);
        Fixed base = Fixed(linea->DBvalue("cantlpedidocliente").ascii()) * Fixed(linea->DBvalue("pvplpedidocliente").ascii());
        basesimp[linea->DBvalue("ivalpedidocliente")] = basesimp[linea->DBvalue("ivalpedidocliente")] + base - base * Fixed(linea->DBvalue("descuentolpedidocliente").ascii()) /100;
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>"+linea->DBvalue("codigocompletoarticulo")+"</td>\n";
        fitxersortidatxt += "	<td>"+linea->DBvalue("desclpedidocliente")+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->DBvalue("cantlpedidocliente").ascii())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->DBvalue("pvplpedidocliente").ascii())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->DBvalue("descuentolpedidocliente").ascii())+" %</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",(base - base * Fixed (linea->DBvalue("descuentolpedidocliente")) /100).toQString().ascii())+"</td>\n";
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
    SDBRecord *linea1;
    if (listadescuentos->rowCount()) {
        fitxersortidatxt += "<blockTable style=\"tabladescuento\" colWidths=\"12cm, 2cm, 3cm\" repeatRows=\"1\">\n";
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>Descuento</td>\n";
        fitxersortidatxt += "	<td>Porcentaje</td>\n";
        fitxersortidatxt += "	<td>Total</td>\n";
        fitxersortidatxt += "</tr>\n";
        for (int i = 0; i < listadescuentos->rowCount(); ++i) {
            linea1 = listadescuentos->lineaat(i);
            porcentt = porcentt + Fixed(linea1->DBvalue("proporciondpedidocliente").ascii());
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "	<td>"+linea1->DBvalue("conceptdpedidocliente")+"</td>\n";
            fitxersortidatxt += "	<td>"+l.sprintf("%s",linea1->DBvalue("proporciondpedidocliente").ascii())+" %</td>\n";
            fitxersortidatxt += "	<td>"+l.sprintf("-%s",( Fixed(linea1->DBvalue("proporciondpedidocliente"))*basei/100).toQString().ascii())+"</td>\n";
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
    _depura("PedidoCliente::calculaypintatotales \n",0);
    base basesimp;

    /// Impresi� de los contenidos
    QString l;
    SDBRecord *linea;
    for ( int i = 0; i < listalineas->rowCount(); ++i) {
	linea = listalineas->lineaat(i);
        Fixed cant(linea->DBvalue("cantlpedidocliente").ascii());
        Fixed pvpund(linea->DBvalue("pvplpedidocliente").ascii());
        Fixed desc1(linea->DBvalue("descuentolpedidocliente").ascii());
        Fixed cantpvp = cant * pvpund;
        Fixed base = cantpvp - cantpvp * desc1 / 100;
        basesimp[linea->DBvalue("ivalpedidocliente")] =  basesimp[linea->DBvalue("ivalpedidocliente")]+ base;
    }// end for


    Fixed basei("0.00");
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei = basei + it.data();
    }// end for
    /// Impresi� de los descuentos
    Fixed porcentt("0.00");
    SDBRecord *linea1;
    if (listadescuentos->rowCount()) {
	for ( int i = 0; i < listadescuentos->rowCount() ; ++i) {
		linea1 = listadescuentos->lineaat(i);
            Fixed propor(linea1->DBvalue("proporciondpedidocliente").ascii());
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
