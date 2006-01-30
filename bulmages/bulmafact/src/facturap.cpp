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
#include <funcaux.h>
#include <plugins.h>
#include <fixed.h>

class Fixed;
typedef QMap<QString, Fixed> base;


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
    mdb_numfacturap= "";
    mdb_ffacturap= "";
    mdb_reffacturap = "";
    mdb_comentfacturap= "";
    mdb_procesadafacturap="FALSE";
    mdb_idforma_pago="";
    mdb_descfacturap="";
}// end vaciaFacturaProveedor

void FacturaProveedor::pintaFacturaProveedor() {
    fprintf(stderr,"pintaFacturaProveedor\n");
    pintaidproveedor(mdb_idproveedor);
    pintanumfacturap(mdb_numfacturap);
    pintafechafacturap(mdb_ffacturap);
    pintareffacturap(mdb_reffacturap);
    pintacomentfacturap(mdb_comentfacturap);
    pintaprocesadafacturap(mdb_procesadafacturap);
    pintaidforma_pago(mdb_idforma_pago);
    pintadescfacturap(mdb_descfacturap);
    // Pinta el subformulario de detalle del FacturaProveedor.
    listalineas->pintaListLinFacturaProveedor();
    listadescuentos->pintaListDescuentoFacturaProv();

    pintatotales(listalineas->calculabase(), listalineas->calculaiva());
}// end pintaFacturaProveedor


// Esta funci� carga un FacturaProveedor.
int FacturaProveedor::cargaFacturaProveedor(QString idbudget) {
    mdb_idfacturap = idbudget;
    inicialize();
    QString query = "SELECT * FROM facturap  WHERE idfacturap="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        mdb_idproveedor= cur->valor("idproveedor");
        mdb_numfacturap= cur->valor("numfacturap");
        mdb_ffacturap= cur->valor("ffacturap");

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
    return 0;
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
        SQLQuery += " numfacturap='"+companyact->sanearCadena(mdb_numfacturap)+"'";
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


    QString SQLQuery = "SELECT * FROM proveedor WHERE idproveedor="+mdb_idproveedor;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        buff.replace("[dirproveedor]",cur->valor("dirproveedor"));
        buff.replace("[poblproveedor]",cur->valor("poblproveedor"));
        buff.replace("[telproveedor]",cur->valor("telproveedor"));
        buff.replace("[nomproveedor]",cur->valor("nomproveedor"));
        buff.replace("[cifproveedor]",cur->valor("cifproveedor"));
    }// end if
    delete cur;

    buff.replace("[numfacturap]",mdb_numfacturap);
    buff.replace("[ffacturap]",mdb_ffacturap);
    buff.replace("[comentfacturap]",mdb_comentfacturap);
    buff.replace("[descfacturap]",mdb_descfacturap);
    buff.replace("[reffacturap]",mdb_reffacturap);

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

