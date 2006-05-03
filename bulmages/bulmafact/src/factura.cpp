//
// C++ Implementation: Factura
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QFile>
#include <QTextStream>

#include "factura.h"
#include "company.h"
#include "funcaux.h"
#include "plugins.h"


class Fixed;
typedef QMap<QString, Fixed> base;


Factura::Factura(company *comp) : DBRecord(comp) {
    companyact=comp;
    setDBTableName("factura");
    setDBCampoId("idfactura");
    addDBCampo("idfactura", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Presupuesto");
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("numfactura", DBCampo::DBint, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("ffactura", DBCampo::DBdate, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("procesadafactura", DBCampo::DBboolean, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("codigoserie_factura", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("comentfactura", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("reffactura", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("descfactura", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNothing, "Trabajador");
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, "Identificador Presupuesto");
}

Factura::~Factura() {}


int Factura::borrar() {
    if (DBvalue("idfactura") != "") {
        companyact->begin();
        int error = listalineas->borrar();
        if (error) {
            companyact->rollback();
            return -1;
        }// end if
	error = listadescuentos->borrar();
        if (error) {
            companyact->rollback();
            return -1;
        }// end if
	error = DBRecord::borrar();
        if (error) {
            companyact->rollback();
            return -1;
        }// end if
        companyact->commit();
    }// end if
    return 0;
}// end borraFactura


void Factura::vaciaFactura() {
    DBclear();
}// end vaciaFactura

void Factura::pintaFactura() {
    _depura("pintaFactura\n",0);
    pintaidcliente(DBvalue("idcliente"));
    pintaidalmacen(DBvalue("idalmacen"));
    pintaNumFactura(DBvalue("numfactura"));
    pintafechafactura(DBvalue("ffactura"));
    pintareffactura(DBvalue("reffactura"));
    pintaComentFactura(DBvalue("comentfactura"));
    pintaprocesadafactura(DBvalue("procesadafactura"));
    pintacodigoserie_factura(DBvalue("codigoserie_factura"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintadescfactura(DBvalue("descfactura"));
    /// Pinta el subformulario de detalle del Factura.
//    listalineas->pintaListLinFactura();
//    listadescuentos->pintaListDescuentoFactura();
    calculaypintatotales();
}// end pintaFactura


// Esta funci� carga un Factura.
int Factura::cargar(QString idbudget) {
    inicialize();
    QString query = "SELECT * FROM factura  WHERE idfactura="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    }// end if
    delete cur;
    listalineas->cargar(idbudget);
    listadescuentos->cargar(idbudget);
    pintaFactura();
    return 0;
}// end chargeBudget


int Factura::guardar() {
    QString fecha;
    if (DBvalue("numfactura") == "") {
        QString SQLQueryn = "SELECT MAX(numFactura)+1 as num FROM Factura";
        cursor2 *cur= companyact->cargacursor(SQLQueryn);
        if (!cur->eof())
            setDBvalue("numfactura", cur->valor("num"));
    	    pintaNumFactura( DBvalue("numfactura"));
        delete cur;
    }// end if
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return -1;
    }// end if
    setidfactura(id);
    error = listalineas->guardar();
    if(error) {
	companyact->rollback();
	return -1;
    }// end if
    error = listadescuentos->guardar();
    if (error) {
	companyact->rollback();
	return -1;
    }// end if


    companyact->commit();
	return 0;
}// end guardaFactura



void Factura::imprimirFactura() {
    /// Hacemos el lanzamiento de plugins para este caso.
    int res = g_plugins->lanza("Factura_imprimirFactura",this);
    if (res)
        return;

    base basesimp;

    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"facturacliente.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"facturacliente.rml";
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
    file.setName(archivod);
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

    buff.replace("[numfactura]",DBvalue("numfactura"));
    buff.replace("[ffactura]",DBvalue("ffactura"));
    buff.replace("[comentfactura]",DBvalue("comentfactura"));
    buff.replace("[descfactura]",DBvalue("descfactura"));
    buff.replace("[reffactura]",DBvalue("reffactura"));
    buff.replace("[codigoserie_factura]", DBvalue("codigoserie_factura"));


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

    SDBRecord *linea;
    int i=0;// Contador que sirve para poner lineas de más en caso de que sea preciso.

    for( int i=0; i < listalineas->rowCount(); ++i) {
	linea = listalineas->lineaat(i);
        Fixed base = Fixed(linea->DBvalue("cantlfactura").ascii()) * Fixed(linea->DBvalue("pvplfactura").ascii());
        basesimp[linea->DBvalue("ivalfactura")] = basesimp[linea->DBvalue("ivalfactura")] + base - base * Fixed(linea->DBvalue("descuentolfactura").ascii()) /100;

        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>"+linea->DBvalue("codigocompletoarticulo")+"</td>\n";
        fitxersortidatxt += "	<td>"+linea->DBvalue("desclfactura")+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->DBvalue("cantlfactura").ascii())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->DBvalue("pvplfactura").ascii())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->DBvalue("descuentolfactura").ascii())+" %</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",(base - base * Fixed (linea->DBvalue("descuentolfactura")) /100).toQString().ascii())+"</td>\n";
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
	for ( int i=0; i < listadescuentos->rowCount(); ++i) {
            porcentt = porcentt + Fixed(linea1->DBvalue("proporciondfactura").ascii());
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "	<td>"+linea1->DBvalue("conceptdfactura")+"</td>\n";
            fitxersortidatxt += "	<td>"+l.sprintf("%s",linea1->DBvalue("proporciondfactura").ascii())+" %</td>\n";
            fitxersortidatxt += "	<td>"+l.sprintf("-%s",( Fixed(linea1->DBvalue("proporciondfactura"))*basei/100).toQString().ascii())+"</td>\n";
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
    invocaPDF("facturacliente");
} //end imprimirPresupuesto





void Factura::calculaypintatotales() {
    base basesimp;
    SDBRecord *linea;
    /// Impresi� de los contenidos
    QString l;

    for ( int i = 0; i < listalineas->rowCount(); ++i) {
	linea = listalineas->lineaat(i);
        Fixed cant(linea->DBvalue("cantlfactura").ascii());
        Fixed pvpund(linea->DBvalue("pvplfactura").ascii());
        Fixed desc1(linea->DBvalue("descuentolfactura").ascii());
        Fixed cantpvp = cant * pvpund;
        Fixed base = cantpvp - cantpvp * desc1 / 100;
        basesimp[linea->DBvalue("ivalfactura")] =  basesimp[linea->DBvalue("ivalfactura")]+ base;
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
	for ( int i = 0; i < listadescuentos->rowCount(); ++i) {
	linea1 = listadescuentos->lineaat(i);
            Fixed propor(linea1->DBvalue("proporciondfactura").ascii());
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

