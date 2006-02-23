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
#include "configuracion.h"
#include <QFile>
#include <QTextStream>
#include "fixed.h"
#include "funcaux.h"
#include "plugins.h"


typedef QMap<QString, Fixed> base;

presupuesto::presupuesto(company *comp) : DBRecord(comp) {
    companyact=comp;
    setDBTableName("presupuesto");
    setDBCampoId("idpresupuesto");
    addDBCampo("idpresupuesto", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Presupuesto");
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("numpresupuesto", DBCampo::DBint, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("fpresupuesto", DBCampo::DBdate, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("vencpresupuesto", DBCampo::DBdate, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("contactpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("telpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("comentpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("idusuari", DBCampo::DBint, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("procesadopresupuesto", DBCampo::DBboolean, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("descpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("refpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNothing, "Identificador Presupuesto");
}// end presupuesto

presupuesto::~presupuesto() {}


void presupuesto::borraPresupuesto() {
    if (DBvalue("idpresupuesto") != "") {
        listalineas->borrar();
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM presupuesto WHERE idpresupuesto="+DBvalue("idpresupuesto"));
        if (error) {
            companyact->rollback();
            return;
        }// end if
        companyact->commit();
        vaciaPresupuesto();
        pintaPresupuesto();
    }// end if
}// end borraPresupuesto


void presupuesto::vaciaPresupuesto() {
    DBclear();
}// end vaciaPresupuesto

void presupuesto::pintaPresupuesto() {
    pintaidcliente(DBvalue("idcliente"));
    pintaIdAlmacen(DBvalue("idalmacen"));
    pintaNumPresupuesto(DBvalue("numpresupuesto"));
    pintaFPresupuesto(DBvalue("fpresupuesto"));
    pintaVencPresupuesto(DBvalue("vencpresupuesto"));
    pintaContractPresupuesto(DBvalue("contactpresupuesto"));
    pintaTelPresupuesto(DBvalue("telpresupuesto"));
    pintaComentPresupuesto(DBvalue("comentpresupuesto"));
    pintaprocesadopresupuesto(DBvalue("procesadopresupuesto"));
    pintadescpresupuesto(DBvalue("descpresupuesto"));
    pintarefpresupuesto(DBvalue("refpresupuesto"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintaidalmacen(DBvalue("idalmacen"));
    pintaidtrabajador(DBvalue("idtrabajador"));
    // Pinta el subformulario de detalle del presupuesto.
    listalineas->pintalistlinpresupuesto();
    // Pinta el subformulario de descuentos del presupuesto
    listadescuentos->pintaListDescuentoPresupuesto();

    calculaypintatotales();
}// end pintaPresupuesto

// Esta funci� carga un presupuesto.
int presupuesto::chargeBudget(QString idbudget) {
    int error = 0;
    QString query = "SELECT * FROM presupuesto WHERE idpresupuesto="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (cur->error())
        error =1;
    if (!cur->eof()) {
        DBload(cur);
    }// end if
    delete cur;

    error += listalineas->chargeBudgetLines(idbudget);
    error += listadescuentos->cargaDescuentos(idbudget);

    /// Tratamiento de excepciones
    if (error) {
        _depura("Error en la carga del presupuesto\n",0);
        return 1;
    }// end if

    pintaPresupuesto();
    return 0;
}// end chargeBudget


void presupuesto::guardapresupuesto() {
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return;
    }// end if
    setidpresupuesto(id);
    companyact->commit();
    listalineas->guardalistlinpresupuesto();
    listadescuentos->guardaListDescuentoPresupuesto();
    chargeBudget(id);
}// end guardapresupuesto


QString presupuesto::detalleArticulos() {
    QString texto="";

    cursor2 *cur=companyact->cargacursor("SELECT * FROM lpresupuesto LEFT JOIN articulo ON lpresupuesto.idarticulo = articulo.idarticulo WHERE presentablearticulo AND idpresupuesto="+DBvalue("idpresupuesto"));
    int i=0;
    while(!cur->eof()) {
        i= !i;
        if (i) {
            texto += "<blockTable style=\"tabladetalle1\" colWidths=\"5cm, 8cm\" rowHeights=\"5.5cm\">\n";
        } else {
            texto += "<blockTable style=\"tabladetalle2\" colWidths=\"8cm, 5cm\" rowHeights=\"5.5cm\">\n";
        }// end if
        texto += "<tr>\n";

        if (i) {
            texto += "<td><h1>"+cur->valor("nomarticulo")+"</h1>";
            texto += "<para><pre>"+cur->valor("obserarticulo")+"</pre></para></td>\n";
        }// end if

        QString file = confpr->valor(CONF_DIR_IMG_ARTICLES)+cur->valor("codigocompletoarticulo")+".jpg";
        QFile f( file );
        if (f.exists() ) {
            texto += "	<td><illustration x=\"0\" y=\"0\" height=\"5cm\">\n"
                     "<image file=\""+confpr->valor(CONF_DIR_IMG_ARTICLES)+cur->valor("codigocompletoarticulo")+".jpg\" x=\"0\" y=\"0\" height=\"5cm\"/>\n"
                     "</illustration></td>\n";
        } else {
            texto += "<td></td>\n";
        }

        if (!i) {
            texto += "<td><h1>"+cur->valor("nomarticulo")+"</h1>";
            texto += "<para><pre>"+cur->valor("obserarticulo")+"</pre></para></td>\n";
        }// end if
        texto += "</tr>\n";
        texto += "</blockTable>";
        cur->siguienteregistro();
    }// end while
    delete cur;

    return texto;
}// end detalleArticulos


void presupuesto::imprimirPresupuesto() {

    /// Disparamos los plugins con presupuesto_imprimirPresupuesto
    int res = g_plugins->lanza("presupuesto_imprimirPresupuesto", this);
    if (res != 0)
        return;



    base basesimp;

    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"presupuesto.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"presupuesto.rml";
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

    buff.replace("[numpresupuesto]",DBvalue("numpresupuesto"));
    buff.replace("[fpresupuesto]",DBvalue("fpresupuesto"));
    buff.replace("[vencpresupuesto]",DBvalue("vencpresupuesto"));
    buff.replace("[contactpresupuesto]",DBvalue("contactpresupuesto"));
    buff.replace("[telpresupuesto]",DBvalue("telpresupuesto"));
    buff.replace("[comentpresupuesto]",DBvalue("comentpresupuesto"));
    buff.replace("[descpresupuesto]",DBvalue("descpresupuesto"));
    buff.replace("[refpresupuesto]",DBvalue("refpresupuesto"));


    linpresupuesto *linea;
    /// Impresion de la tabla de contenidos.
    fitxersortidatxt += "<blockTable style=\"tablacontenido\" colWidths=\"1.75cm, 8.75cm, 1.2cm, 1.5cm, 1.8cm, 2.25cm\" repeatRows=\"1\">\n";
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
        Fixed base = Fixed(linea->cantlpresupuesto().ascii()) * Fixed(linea->pvplpresupuesto().ascii());
        basesimp[linea->ivalpresupuesto()] = basesimp[linea->ivalpresupuesto()] + base - base * Fixed(linea->descuentolpresupuesto().ascii()) /100;

        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>"+XMLProtect(linea->codigocompletoarticulo())+"</td>\n";
        fitxersortidatxt += "	<td>"+XMLProtect(linea->desclpresupuesto())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",XMLProtect(linea->cantlpresupuesto()).ascii())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",XMLProtect(linea->pvplpresupuesto()).ascii())+"</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",XMLProtect(linea->descuentolpresupuesto()).ascii())+" %</td>\n";
        fitxersortidatxt += "	<td>"+l.sprintf("%s",(base - base * Fixed (linea->descuentolpresupuesto()) /100).toQString().ascii())+"</td>\n";
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
    DescuentoPresupuesto *linea1;
    if (listadescuentos->m_lista.first()) {
        fitxersortidatxt += "<blockTable style=\"tabladescuento\" colWidths=\"12cm, 2cm, 3cm\" repeatRows=\"1\">\n";
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "	<td>Descuento</td>\n";
        fitxersortidatxt += "	<td>Porcentaje</td>\n";
        fitxersortidatxt += "	<td>Total</td>\n";
        fitxersortidatxt += "</tr>\n";
        for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
            porcentt = porcentt + Fixed(linea1->proporciondpresupuesto().ascii());
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "	<td>"+XMLProtect(linea1->conceptdpresupuesto())+"</td>\n";
            fitxersortidatxt += "	<td>"+l.sprintf("%s",linea1->proporciondpresupuesto().ascii())+" %</td>\n";
            fitxersortidatxt += "	<td>"+l.sprintf("-%s",( Fixed(linea1->proporciondpresupuesto())*basei/100).toQString().ascii())+"</td>\n";
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
        tr1 += "	<td>Base "+XMLProtect(it.key())+" %</td>\n";
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
        tr1 += "	<td>Iva "+XMLProtect(it.key())+" %</td>\n";
        tr2 += "	<td>"+l.sprintf("%s", pariva.toQString().ascii())+"</td>\n";
    }// end for
    tr1 += "	<td>Total </td>\n";
    tr2 += "	<td>"+l.sprintf("%s",(totiva+totbaseimp).toQString().ascii())+"</td>\n";
    fitxersortidatxt += "<tr>"+tr1+"</tr><tr>"+tr2+"</tr></blockTable>\n";
    buff.replace("[totales]",fitxersortidatxt);

    /// En la versi� para windows hay problemas con las imagenes, por eso de momento lo dejamos as�
#ifndef WINDOWS

    buff.replace("[detallearticulos]",detalleArticulos());
#endif

    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }
    QString cadsys;
    invocaPDF("presupuesto");
} //end imprimirPresupuesto


void presupuesto::calculaypintatotales() {
    _depura("calculaypintatotales \n",0);
	
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto
    int res = g_plugins->lanza("presupuesto_calculaypintatotales", this);
    if (res != 0)
        return;
		
    base basesimp;
    linpresupuesto *linea;
    /// Impresi� de los contenidos
    QString l;

    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        Fixed cant(linea->cantlpresupuesto().ascii());
        Fixed pvpund(linea->pvplpresupuesto().ascii());
        Fixed desc1(linea->descuentolpresupuesto().ascii());
        Fixed cantpvp = cant * pvpund;
        Fixed base = cantpvp - cantpvp * desc1 / 100;
        basesimp[linea->ivalpresupuesto()] =  basesimp[linea->ivalpresupuesto()]+ base;
    }// end for


    Fixed basei("0.00");
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei = basei + it.data();
    }// end for
    /// Impresi� de los descuentos
    Fixed porcentt("0.00");
    DescuentoPresupuesto *linea1;
    if (listadescuentos->m_lista.first()) {
        for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
            Fixed propor(linea1->proporciondpresupuesto().ascii());
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
