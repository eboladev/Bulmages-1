/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QFile>
#include <QTextStream>

#include "factura.h"
#include "company.h"
#include "funcaux.h"
#include "plugins.h"


class Fixed;

/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base como un QMap.
typedef QMap<QString, Fixed> base;


/** PReparamos la clase DBRecord para funcionar con la tabla factura.
*/
Factura::Factura(company *comp) : DBRecord(comp) {
    _depura("Factura::Factura", 0);
    companyact = comp;
    setDBTableName("factura");
    setDBCampoId("idfactura");
    addDBCampo("idfactura", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("Factura", "Id factura"));
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("Factura", "Id cliente"));
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("Factura", "Id almacen"));
    addDBCampo("numfactura", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Factura", "Numero factura"));
    addDBCampo("ffactura", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("Factura", "Ffactura"));
    addDBCampo("procesadafactura", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("Factura", "Procesada factura"));
    addDBCampo("codigoserie_factura", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Factura", "Codigo serie factura"));
    addDBCampo("comentfactura", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Factura", "Comentario factura"));
    addDBCampo("reffactura", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Factura", "Referencia factura"));
    addDBCampo("descfactura", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Factura", "Descripcion factura"));
    addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Factura", "Id trabajador"));
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Factura", "Id formad de pago"));
    _depura("END Factura::Factura", 0);
}


/** No requiere de acciones especiales en el destructor.
*/
Factura::~Factura() {
    _depura("Factura::~Factura", 0);
    _depura("END Factura::~Factura", 0);
}



/** Se encarga del borrado de una factura.
    Para ello primero borra las lineas y descuentos de esta. y luego
    Borra la ficha de la factura.
    Sit odo va bien devuelve 0, sino devuelve -1.
*/
/// \TODO: Deberia contener un bloque try{} catch{}
int Factura::borrar() {
    _depura("Factura::borrar", 0);
    if (DBvalue("idfactura") != "") {
        companyact->begin();
        int error = listalineas->borrar();
        if (error) {
            companyact->rollback();
            return -1;
        } // end if
        error = listadescuentos->borrar();
        if (error) {
            companyact->rollback();
            return -1;
        } // end if
        error = DBRecord::borrar();
        if (error) {
            companyact->rollback();
            return -1;
        } // end if
        companyact->commit();
    } // end if
    _depura("END Factura::borrar", 0);
    return 0;
}


/** Vacia la memoria que pudiera tener DBRecord
*/
void Factura::vaciaFactura() {
    _depura("Factura::vaciaFactura", 0);
    DBclear();
    _depura("END Factura::vaciaFactura", 0);
}


/** Pinta una factura.
    Para ello utiliza los metodos sobrecargables pintaXXX de esta forma se podria generar una segunda interficie reutilizando esta clase.
*/
/// \TODO: Deberia llamarse pintar.
void Factura::pintaFactura() {
    _depura("pintaFactura", 0);
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
    /// Pinta el subformulario de detalle del factura.
    calculaypintatotales();
    _depura("END pintaFactura", 0);
}


/// Esta funcion carga un factura.
/** Carga una factura. 
    Crea el query de carga y delega el llenodo de registros a DBRecord.
    Una vez cargado DBRecord hace una llamada a la carga de lineas y descuentos.
    Tras el cargado se hace un pintado para que se vea bien todo.
    Si todo va bien devuelve 0.
*/
/// \TODO: Deberia utilizar un bloque try{} catch{}
int Factura::cargar(QString idbudget) {
    _depura("Factura::cargar", 0);
    inicialize();
    QString query = "SELECT * FROM factura  WHERE idfactura=" + idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    } // end if
    delete cur;
    listalineas->cargar(idbudget);
    listadescuentos->cargar(idbudget);
    pintaFactura();
    _depura("END Factura::cargar", 0);
    return 0;
}


/** Se encarga del guardado de una factura.
    Primero busca el numero de factura que hace falta.
    Luego guarda el registro.
    Despues guarda las lineas y descuentos.
    Al finalizar el guardado hace un cargar para repintar todos los elementos.
    Si todo va bien devuelve 0
    Si se produce un error genera una excepcion -1
*/
int Factura::guardar() {
    _depura("Factura::guardar", 0);
    QString fecha;
    try {
        /// Calculamos el proximo numero de factura para poder insertarlo en caso de que este sea nulo.
        if (DBvalue("numfactura") == "") {
            QString SQLQueryn = "SELECT MAX(numFactura)+1 as num FROM Factura WHERE codigoserie_factura = '".DBvalue("codigoserie_factura")."'";
            cursor2 *cur= companyact->cargacursor(SQLQueryn);
            if (!cur->eof())
                setDBvalue("numfactura", cur->valor("num"));
            pintaNumFactura(DBvalue("numfactura"));
            delete cur;
        } // end if
        QString id;
        companyact->begin();
        DBsave(id);
        setidfactura(id);
        listalineas->guardar();
        listadescuentos->guardar();
        companyact->commit();

	/// Hacemos una carga para recuperar datos como la referencia
	cargar(id);

        _depura("END Factura::guardar", 0);
        return 0;
    } catch (...) {
        _depura("Factura::guardar() se produjo un error guardando la factura", 0);
        companyact->rollback();
        throw  -1;
    } // end try
}


/** Impresion de una factura.
    Coge la plantilla facturacliente.rml y la guarda en ~/.bulmages/facturacliente.rml
    Luego hace en este archivo toda la sustitucion de cadenas segun la ficha de factura.
    Por ultimo llama a bgtrml2pdf.py para generear un pdf y lo muestra con el visor predeterminado.
*/
void  Factura::imprimirFactura() {
    _depura("Factura::imprimirFactura", 0);
    /// Hacemos el lanzamiento de plugins para este caso.
    int res = g_plugins->lanza("Factura_imprimirFactura", this);
    if (res)
        return;

    base basesimp;
    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "facturacliente.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "facturacliente.rml";
    QString archivologo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";
    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system (archivo.toAscii().constData());
    /// Copiamos el logo.
#ifdef WINDOWS

    archivologo = "copy "  +archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#endif

    system(archivologo.toAscii().constData());
    QFile file;
    file.setFileName(archivod);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt = "";

    /// Linea de totales del presupuesto.
    QString SQLQuery = "SELECT * FROM cliente WHERE idcliente=" + DBvalue("idcliente");
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        buff.replace("[dircliente]", cur->valor("dircliente"));
        buff.replace("[poblcliente]", cur->valor("poblcliente"));
        buff.replace("[telcliente]", cur->valor("telcliente"));
        buff.replace("[nomcliente]", cur->valor("nomcliente"));
        buff.replace("[cifcliente]", cur->valor("cifcliente"));
        buff.replace("[idcliente]", cur->valor("idcliente"));
        buff.replace("[cpcliente]", cur->valor("cpcliente"));
    } // end if
    delete cur;

    buff.replace("[numfactura]", DBvalue("numfactura"));
    buff.replace("[ffactura]", DBvalue("ffactura"));
    buff.replace("[comentfactura]", DBvalue("comentfactura"));
    buff.replace("[descfactura]", DBvalue("descfactura"));
    buff.replace("[reffactura]", DBvalue("reffactura"));
    buff.replace("[codigoserie_factura]", DBvalue("codigoserie_factura"));

    /// Impresion de la tabla de contenidos.
    QString l;
    SDBRecord *linea;
    for(int i=0; i < listalineas->rowCount()-1; ++i) {
        linea = listalineas->lineaat(i);
        Fixed base = Fixed(linea->DBvalue("cantlfactura").toAscii().constData()) * Fixed(linea->DBvalue("pvplfactura").toAscii().constData());
        basesimp[linea->DBvalue("ivalfactura")] = basesimp[linea->DBvalue("ivalfactura")] + base - base * Fixed(linea->DBvalue("descuentolfactura").toAscii().constData()) / 100;
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "        <td>" + linea->DBvalue("codigocompletoarticulo") + "</td>\n";
        fitxersortidatxt += "        <td>" + linea->DBvalue("desclfactura")+"</td>\n";
        fitxersortidatxt += "        <td>" + l.sprintf("%s", linea->DBvalue("cantlfactura").toAscii().constData()) + "</td>\n";
        fitxersortidatxt += "        <td>" + l.sprintf("%s", linea->DBvalue("pvplfactura").toAscii().constData()) + "</td>\n";
        fitxersortidatxt += "        <td>" + l.sprintf("%s", linea->DBvalue("descuentolfactura").toAscii().constData()) + " %</td>\n";
        fitxersortidatxt += "        <td>" + l.sprintf("%s", (base - base * Fixed (linea->DBvalue("descuentolfactura")) / 100).toQString().toAscii().constData()) + "</td>\n";
        fitxersortidatxt += "</tr>";
    } // end for

    buff.replace("[story]", fitxersortidatxt);

    Fixed basei("0.00");
    base::Iterator it;
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        basei =basei + it.value();
    } // end for

    /// Impresion de los descuentos.
    fitxersortidatxt = "";
    Fixed porcentt("0.00");
    SDBRecord *linea1;
    if (listadescuentos->rowCount()-1) {
        fitxersortidatxt += "<blockTable style=\"tabladescuento\" colWidths=\"12cm, 2cm, 3cm\" repeatRows=\"1\">\n";
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "        <td>" + QApplication::translate("Factura", "Descuento") + "</td>\n";
        fitxersortidatxt += "        <td>" + QApplication::translate("Factura", "Porcentaje") + "</td>\n";
        fitxersortidatxt += "        <td>" + QApplication::translate("Factura", "Total") + "</td>\n";
        fitxersortidatxt += "</tr>\n";
        for (int i=0; i < listadescuentos->rowCount()-1; ++i) {
            linea1 = listadescuentos->lineaat(i);
            porcentt = porcentt + Fixed(linea1->DBvalue("proporciondfactura").toAscii().constData());
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "        <td>" + linea1->DBvalue("conceptdfactura") + "</td>\n";
            fitxersortidatxt += "        <td>" + l.sprintf("%s", linea1->DBvalue("proporciondfactura").toAscii().constData()) + " %</td>\n";
            fitxersortidatxt += "        <td>" + l.sprintf("-%s", (Fixed(linea1->DBvalue("proporciondfactura")) * basei / 100).toQString().toAscii().constData()) + "</td>\n";
            fitxersortidatxt += "</tr>";
        } // end for
        fitxersortidatxt += "</blockTable>\n";
    } // end if
    buff.replace("[descuentos]", fitxersortidatxt);

    /// Impresion de los totales.
    fitxersortidatxt = "";
    QString tr1 = ""; /// Rellena el primer tr de titulares.
    QString tr2 = ""; /// Rellena el segundo tr de cantidades.
    fitxersortidatxt += "<blockTable style=\"tablatotales\">\n";

    Fixed totbaseimp("0.00");
    Fixed parbaseimp("0.00");
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        if (porcentt > 0) {
            parbaseimp = it.value() - it.value() * porcentt / 100;
        } else {
            parbaseimp = it.value();
        } // end if
        totbaseimp = totbaseimp + parbaseimp;
        tr1 += "        <td>" + QApplication::translate("Factura", "Base") +" "+ it.key() + "%</td>\n";
        tr2 += "        <td>" + l.sprintf(" %s ", parbaseimp.toQString().toAscii().constData()) + "</td>\n";
    } // end for

    Fixed totiva("0.0");
    Fixed pariva("0.0");
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        if (porcentt > 0) {
            pariva = (it.value() - it.value() * porcentt / 100) * Fixed(it.key()) / 100;
        } else {
            pariva = it.value() * Fixed(it.key()) / 100;
        } // end if
        totiva = totiva + pariva;
        tr1 += "        <td>" + QApplication::translate("Factura", "Iva ") +" "+ it.key() + " %</td>\n";
        tr2 += "        <td>" + l.sprintf(" %s ", pariva.toQString().toAscii().constData()) + "</td>\n";
    } // end for
    tr1 += "        <td>" + QApplication::translate("Factura", "Total ") +"</td>\n";
    tr2 += "        <td>" + l.sprintf(" %s ", (totiva + totbaseimp).toQString().toAscii().constData()) + "</td>\n";
    fitxersortidatxt += "<tr>" + tr1 + "</tr><tr>" + tr2 + "</tr></blockTable>\n";
    buff.replace("[totales]", fitxersortidatxt);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    }
    invocaPDF("facturacliente");
    _depura("Factura::imprimirFactura", 0);
}



/** Calcula los totales de factura, descuentos e impuestos y invoca al metodo de pintaTotales para
    que se pinten dichos valores en la pantalla.
*/
void Factura::calculaypintatotales() {
    _depura("Factura::calculaypintatotales", 0);
    base basesimp;
    SDBRecord *linea;
    /// Impresion de los contenidos.
    QString l;

    for (int i = 0; i < listalineas->rowCount(); ++i) {
        linea = listalineas->lineaat(i);
        Fixed cant(linea->DBvalue("cantlfactura").toAscii().constData());
        Fixed pvpund(linea->DBvalue("pvplfactura").toAscii().constData());
        Fixed desc1(linea->DBvalue("descuentolfactura").toAscii().constData());
        Fixed cantpvp = cant * pvpund;
        Fixed base = cantpvp - cantpvp * desc1 / 100;
        basesimp[linea->DBvalue("ivalfactura")] = basesimp[linea->DBvalue("ivalfactura")] + base;
    } // end for

    Fixed basei("0.00");
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it) {
        basei = basei + it.value();
    } // end for

    /// Impresion de los descuentos.
    Fixed porcentt("0.00");
    SDBRecord *linea1;
    if (listadescuentos->rowCount()) {
        for (int i = 0; i < listadescuentos->rowCount(); ++i) {
            linea1 = listadescuentos->lineaat(i);
            Fixed propor(linea1->DBvalue("proporciondfactura").toAscii().constData());
            porcentt = porcentt + propor;
        } // end for
    } // end if

    Fixed totbaseimp("0.00");
    Fixed parbaseimp("0.00");
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        if (porcentt > Fixed("0.00") ) {
            parbaseimp = it.value() - it.value() * porcentt / 100;
        } else {
            parbaseimp = it.value();
        } // end if
        totbaseimp = totbaseimp + parbaseimp;
    } // end for

    Fixed totiva("0.00");
    Fixed pariva("0.00");
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        Fixed piva(it.key().toAscii().constData());
        if (porcentt > Fixed("0.00")) {
            pariva = (it.value() - it.value() * porcentt / 100) * piva / 100;
        } else {
            pariva = it.value() * piva / 100;
        } // end if
        totiva = totiva + pariva;
    } // end for
    pintatotales(totiva, totbaseimp, totiva+totbaseimp, basei * porcentt / 100);
    _depura("Factura::calculaypintatotales", 0);
}

