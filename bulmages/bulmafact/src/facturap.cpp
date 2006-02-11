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
#include <QFile>
//Added by qt3to4:
#include <QTextStream>
#include <funcaux.h>
#include <plugins.h>
#include <fixed.h>

class Fixed;
typedef QMap<QString, Fixed> base;


FacturaProveedor::FacturaProveedor(company *comp) : DBRecord(comp) {
    companyact=comp;

    setDBTableName("facturap");
    setDBCampoId("idfacturap");
    addDBCampo("idfacturap", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Presupuesto");
    addDBCampo("idproveedor", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("numfacturap", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("ffacturap", DBCampo::DBdate, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("procesadafacturap", DBCampo::DBboolean, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("comentfacturap", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("reffacturap", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("descfacturap", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("idusuari", DBCampo::DBint, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, "Identificador Presupuesto");
}

FacturaProveedor::~FacturaProveedor() {}


void FacturaProveedor::borraFacturaProveedor() {
    if (DBvalue("idfacturap") != "") {
        listalineas->borrar();
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM facturap WHERE idfacturap="+DBvalue("idfacturap"));
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
    DBclear();
}// end vaciaFacturaProveedor

void FacturaProveedor::pintaFacturaProveedor() {
    fprintf(stderr,"pintaFacturaProveedor\n");
    pintaidproveedor(DBvalue("idproveedor"));
    pintanumfacturap(DBvalue("numfacturap"));
    pintafechafacturap(DBvalue("ffacturap"));
    pintareffacturap(DBvalue("reffacturap"));
    pintacomentfacturap(DBvalue("comentfacturap"));
    pintaprocesadafacturap(DBvalue("procesadafacturap"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintadescfacturap(DBvalue("descfacturap"));
    // Pinta el subformulario de detalle del FacturaProveedor.
    listalineas->pintaListLinFacturaProveedor();
    listadescuentos->pintaListDescuentoFacturaProv();
    pintatotales(listalineas->calculabase(), listalineas->calculaiva());
}// end pintaFacturaProveedor


// Esta funci� carga un FacturaProveedor.
int FacturaProveedor::cargaFacturaProveedor(QString idbudget) {
    inicialize();
    QString query = "SELECT * FROM facturap  WHERE idfacturap="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    }// end if
    delete cur;
    listalineas->cargaListLinFacturaProveedor(idbudget);
    listadescuentos->cargaDescuentos(idbudget);
    pintaFacturaProveedor();
    return 0;
}// end chargeBudget


void FacturaProveedor::guardaFacturaProveedor() {
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return;
    }// end if
    setidfacturap(id);
    companyact->commit();
    listalineas->guardaListLinFacturaProveedor();
    listadescuentos->guardaListDescuentoFacturaProv();
    cargaFacturaProveedor(DBvalue("idfacturap"));
}// end guardaFacturaProveedor




void FacturaProveedor::imprimirFacturaProveedor() {
	/// Hacemos el lanzamiento de plugins para este caso.
	int res = g_plugins->lanza("imprimirFacturaProveedor",this);
	if (res) return;

    base basesimp;

    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"facturap.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"facturap.rml";
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


    QString SQLQuery = "SELECT * FROM proveedor WHERE idproveedor="+DBvalue("idproveedor");
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        buff.replace("[dirproveedor]",cur->valor("dirproveedor"));
        buff.replace("[poblproveedor]",cur->valor("poblproveedor"));
        buff.replace("[telproveedor]",cur->valor("telproveedor"));
        buff.replace("[nomproveedor]",cur->valor("nomproveedor"));
        buff.replace("[cifproveedor]",cur->valor("cifproveedor"));
    }// end if
    delete cur;

    buff.replace("[numfacturap]",DBvalue("numfacturap"));
    buff.replace("[ffacturap]",DBvalue("ffacturap"));
    buff.replace("[comentfacturap]",DBvalue("comentfacturap"));
    buff.replace("[descfacturap]",DBvalue("descfacturap"));
    buff.replace("[reffacturap]",DBvalue("reffacturap"));

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

    LinFacturaProveedor *linea;
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        Fixed base = Fixed(linea->cantlfacturap().ascii()) * Fixed(linea->pvplfacturap().ascii());
        basesimp[linea->ivalfacturap()] = basesimp[linea->ivalfacturap()] + base - base * Fixed(linea->descuentolfacturap().ascii()) /100;

        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>"+linea->codigocompletoarticulo()+"</td>\n";
        fitxersortidatxt += "	<td>"+linea->desclfacturap()+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->cantlfacturap().ascii())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->pvplfacturap().ascii())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",linea->descuentolfacturap().ascii())+" %</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",(base - base * Fixed (linea->descuentolfacturap()) /100).toQString().ascii())+"</td>\n";
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
    DescuentoFacturaProv *linea1;
    if (listadescuentos->m_lista.first()) {
        fitxersortidatxt += "<blockTable style=\"tabladescuento\" colWidths=\"12cm, 2cm, 3cm\" repeatRows=\"1\">\n";
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>Descuento</td>\n";
        fitxersortidatxt += "	<td>Porcentaje</td>\n";
        fitxersortidatxt += "	<td>Total</td>\n";
        fitxersortidatxt += "</tr>\n";
        for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
            porcentt = porcentt + Fixed(linea1->proporciondfacturap().ascii());
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "	<td>"+linea1->conceptdfacturap()+"</td>\n";
            fitxersortidatxt += "	<td>"+l.sprintf("%s",linea1->proporciondfacturap().ascii())+" %</td>\n";
            fitxersortidatxt += "	<td>"+l.sprintf("-%s",( Fixed(linea1->proporciondfacturap())*basei/100).toQString().ascii())+"</td>\n";
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
    invocaPDF("facturap");
} //end imprimirFacturaProveedor

