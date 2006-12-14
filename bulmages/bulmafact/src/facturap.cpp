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
#include <funcaux.h>
#include <plugins.h>
#include <fixed.h>

#include "facturap.h"
#include "company.h"


class Fixed;
/// Una factura puede tener multiples bases. Por eso definimos el tipo base como un QMap.
typedef QMap<QString, Fixed> base;

/** Inicializa todos los componentes.
    Prepara el DBRecord para trabajar con la tabla facturap.
*/
FacturaProveedor::FacturaProveedor(company *comp) : DBRecord(comp) {
    _depura("FacturaProveedor::FacturaProveedor", 0);
    companyact = comp;
    setDBTableName("facturap");
    setDBCampoId("idfacturap");
    addDBCampo("idfacturap", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("FacturaProveedor", "Id facturap"));
    addDBCampo("idproveedor", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("FacturaProveedor", "Id proveedor"));
    addDBCampo("numfacturap", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("FacturaProveedor", "Numero"));
    addDBCampo("ffacturap", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("FacturaProveedor", "Fecha"));
    addDBCampo("procesadafacturap", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("FacturaProveedor", "Procesada facturap"));
    addDBCampo("comentfacturap", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("FacturaProveedor", "Comentario facturap"));
    addDBCampo("reffacturap", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("FacturaProveedor", "Referencia facturap"));
    addDBCampo("descfacturap", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("FacturaProveedor", "Descripcion facturap"));
    addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("FacturaProveedor", "Id trabajador"));
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("FacturaProveedor", "Id forma de pago"));
    _depura("END FacturaProveedor::FacturaProveedor", 0);
}


/** No precisa de acciones especiales en el destructor de la clase.
*/
FacturaProveedor::~FacturaProveedor() {
	_depura("FacturaProveedor::~FacturaProveedor", 0);
	_depura("END FacturaProveedor::~FacturaProveedor", 0);
}


/** Hace el borrado de la factura.
    Primero borra las lineas de factura.
    Luego borra los descuentos de factura.
    Y por ultimo delega en DBRecord el borrado del registro.
    Si algo falla devuelve -1.
    Si todo va bien devuelve 0.
*/
int FacturaProveedor::borrar() {
    _depura("FacturaProveedor::borrar", 0);
    int error = 0;
    if (DBvalue("idfacturap") != "") {
        companyact->begin();
        error = listalineas->borrar();
        error += listadescuentos->borrar();
        error += DBRecord::borrar();
        if (error) {
            companyact->rollback();
            return -1;
        } // end if
        companyact->commit();
    } // end if
    _depura("END FacturaProveedor::borrar", 0);
    return 0;
}

/** Pinta una FacturaProveedor
	Para ello utiliza los metodos pintaXXX que estan implementados en la clase de visualizacion. (FacturaProveedorView).
*/
void FacturaProveedor::pintar() {
    _depura("FacturaProveedor::pintar", 0);
    pintaidproveedor(DBvalue("idproveedor"));
    pintanumfacturap(DBvalue("numfacturap"));
    pintafechafacturap(DBvalue("ffacturap"));
    pintareffacturap(DBvalue("reffacturap"));
    pintacomentfacturap(DBvalue("comentfacturap"));
    pintaprocesadafacturap(DBvalue("procesadafacturap"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintadescfacturap(DBvalue("descfacturap"));
//     pintatotales(listalineas->calculabase(), listalineas->calculaiva());
    calculaypintatotales();
    _depura("END FacturaProveedor::pintar", 0);
}


/** Carga un FacturaProveedor.
	Carga el DBRecord con DBload y luego llama a la carga de las lineas y los descuentos.
	Al finalizar hace un pintado para que la informacion se vea actualizada.
	Si algo falla genera una excepcion -1.
	Si todo va bien devuelve 0.
*/
int FacturaProveedor::cargar(QString idfacturap) {
    _depura("FacturaProveedor::cargar", 0);
    try {
        inicialize();
        QString query = "SELECT * FROM facturap WHERE idfacturap=" + idfacturap;
        cursor2 * cur = companyact->cargacursor(query);
	if (cur->eof()) {
		delete cur;
		throw -1;
	} // end if
        if (DBload(cur)) {
		delete cur;
                throw -1;
	} // end if
        delete cur;
        listalineas->cargar(idfacturap);
        listadescuentos->cargar(idfacturap);
        pintar();
    } catch (...) {
	mensajeInfo("FacturaProveedor::cargar producida una excepcion");
        throw -1;
    } // end try
    _depura("END FacturaProveedor::cargar", 0);
    return 0;
}

/** Hace el guardado de una Factura de Proveedor.
	guarda el registro a traves de DBRecord y luego actualiza el identificador de facturap en
	las lineas y en los descuentos para guardarlos.
	Si algo falla genera una excepcion -1.
	Si todo va bien devuelve 0.
*/
int FacturaProveedor::guardar() {
    _depura("FacturaProveedor::guardar()", 0);
    companyact->begin();
    try {
        QString id;
        DBsave(id);
        setidfacturap(id);
        listalineas->guardar();
        listadescuentos->guardar();
        companyact->commit();

	/// Hacemos una carga para recuperar referencias y demás datos
	cargar(id);

        _depura("END FacturaProveedor::guardar()", 0);
        return 0;
    } catch (...) {
        mensajeInfo("FacturaProveedor::guardar() se produjo un error al guardar");
        companyact->rollback();
        throw -1;
    } // end try
}


/** Imprime una factura de proveedor
*/
/// \DEPRECATED: No se prevee que se tengan que imprimir facturas de proveedor.
void FacturaProveedor::imprimirFacturaProveedor() {
    /// Hacemos el lanzamiento de plugins para este caso.
    int res = g_plugins->lanza("imprimirFacturaProveedor", this);
    if (res)
        return;

    base basesimp;

    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "facturap.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "facturap.rml";
    QString archivologo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system(archivo.toAscii().constData());

    /// Copiamos el logo.
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
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
    QString fitxersortidatxt="";

    /// Linea de totales del presupuesto
    QString SQLQuery = "SELECT * FROM proveedor WHERE idproveedor = " + DBvalue("idproveedor");
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        buff.replace("[dirproveedor]", cur->valor("dirproveedor"));
        buff.replace("[poblproveedor]", cur->valor("poblproveedor"));
        buff.replace("[telproveedor]", cur->valor("telproveedor"));
        buff.replace("[nomproveedor]", cur->valor("nomproveedor"));
        buff.replace("[cifproveedor]", cur->valor("cifproveedor"));
    } // end if
    delete cur;

    buff.replace("[numfacturap]", DBvalue("numfacturap"));
    buff.replace("[ffacturap]", DBvalue("ffacturap"));
    buff.replace("[comentfacturap]", DBvalue("comentfacturap"));
    buff.replace("[descfacturap]", DBvalue("descfacturap"));
    buff.replace("[reffacturap]", DBvalue("reffacturap"));

    /// Impresion de la tabla de contenidos.
    fitxersortidatxt += "<blockTable style=\"tablacontenido\" colWidths=\"1.75cm, 8.75cm, 1.5cm, 1.5cm, 1.5cm, 2.25cm\" repeatRows=\"1\">\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "        <td>" + QApplication::translate("FacturaProveedor", "Codigo") + "</td>\n";
    fitxersortidatxt += "        <td>" + QApplication::translate("FacturaProveedor", "Concepto") + "</td>\n";
    fitxersortidatxt += "        <td>" + QApplication::translate("FacturaProveedor", "Cant.") + "</td>\n";
    fitxersortidatxt += "        <td>" + QApplication::translate("FacturaProveedor", "Precio") + "</td>\n";
    fitxersortidatxt += "        <td>" + QApplication::translate("FacturaProveedor", "Desc.") + "</td>\n";
    fitxersortidatxt += "        <td>" + QApplication::translate("FacturaProveedor", "Total") + "</td>\n";
    fitxersortidatxt += "</tr>\n";
    QString l;

    /// Contador que sirve para poner lineas de mas en caso de que sea preciso.
    int i = 0;

    SDBRecord *linea;
    for (int i = 0; i < listalineas->rowCount(); ++i) {
        linea = listalineas->lineaat(i);
        Fixed base = Fixed(linea->DBvalue("cantlfacturap").toAscii().constData()) * Fixed(linea->DBvalue("pvplfacturap").toAscii().constData());
        basesimp[linea->DBvalue("ivalfacturap")] = basesimp[linea->DBvalue("ivalfacturap")] + base - base * Fixed(linea->DBvalue("descuentolfacturap").toAscii().constData()) / 100;

        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "        <td>" + linea->DBvalue("codigocompletoarticulo") + "</td>\n";
        fitxersortidatxt += "        <td>" + linea->DBvalue("desclfacturap") + "</td>\n";
        fitxersortidatxt += "        <td>" + linea->DBvalue("cantlfacturap") + "</td>\n";
        fitxersortidatxt += "        <td>" + linea->DBvalue("pvplfacturap") + "</td>\n";
        fitxersortidatxt += "        <td>" + linea->DBvalue("descuentolfacturap") + " %</td>\n";
        fitxersortidatxt += "        <td>" + (base - base * Fixed (linea->DBvalue("descuentolfacturap")) / 100).toQString() + "</td>\n";
        fitxersortidatxt += "</tr>";
        i++;
    } // end for

    while (i++ < 15)
        fitxersortidatxt += "<tr></tr>";

    fitxersortidatxt += "</blockTable>\n";
    buff.replace("[story]", fitxersortidatxt);

    Fixed basei("0.00");
    base::Iterator it;
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        basei = basei + it.value();
    } // end for

    /// Impresion de los descuentos.
    fitxersortidatxt = "";
    Fixed porcentt("0.00");
    SDBRecord *linea1;
    if (listadescuentos->rowCount()) {
        fitxersortidatxt += "<blockTable style=\"tabladescuento\" colWidths=\"12cm, 2cm, 3cm\" repeatRows=\"1\">\n";
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "        <td>" + QApplication::translate("FacturaProveedor", "Descuento") + "</td>\n";
        fitxersortidatxt += "        <td>" + QApplication::translate("FacturaProveedor", "Porcentaje") + "</td>\n";
        fitxersortidatxt += "        <td>" + QApplication::translate("FacturaProveedor", "Total") + "</td>\n";
        fitxersortidatxt += "</tr>\n";
        for (int i = 0; i < listadescuentos->rowCount(); ++i) {
            linea1 = listadescuentos->lineaat(i);
            porcentt = porcentt + Fixed(linea1->DBvalue("proporciondfacturap").toAscii().constData());
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "        <td>" + linea1->DBvalue("conceptdfacturap") + "</td>\n";
            fitxersortidatxt += "        <td>" + linea1->DBvalue("proporciondfacturap") + " %</td>\n";
            fitxersortidatxt += "        <td>" + l.sprintf("-%s", ( Fixed(linea1->DBvalue("proporciondfacturap")) * basei / 100).toQString().toAscii().constData()) + "</td>\n";
            fitxersortidatxt += "</tr>";
        } // end for
        fitxersortidatxt += "</blockTable>\n";
    } // end if
    buff.replace("[descuentos]", fitxersortidatxt);

    /// Impresion de los totales.
    fitxersortidatxt= "";
    QString tr1 = "";	/// Rellena el primer tr de titulares.
    QString tr2 = "";	/// Rellena el segundo tr de cantidades.
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
        tr1 += "        <td>" + QApplication::translate("FacturaProveedor", "Base ") + it.key() + " %</td>\n";
        tr2 += "        <td>" + l.sprintf("%s",parbaseimp.toQString().toAscii().constData()) + "</td>\n";
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
        tr1 += "        <td>" + QApplication::translate("FacturaProveedor", "IVA ") + it.key() + " %</td>\n";
        tr2 += "        <td>" + l.sprintf("%s", pariva.toQString().toAscii().constData()) + "</td>\n";
    } // end for
    tr1 += "        <td>" + QApplication::translate("FacturaProveedor", "Total ") + "</td>\n";
    tr2 += "        <td>" + l.sprintf("%s", (totiva + totbaseimp).toQString().toAscii().constData()) + "</td>\n";
    fitxersortidatxt += "<tr>" + tr1 + "</tr><tr>" + tr2 + "</tr></blockTable>\n";
    buff.replace("[totales]", fitxersortidatxt);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file) ;
        stream << buff;
        file.close();
    } // end if
    invocaPDF("facturap");
}


/** Devuelve la lista de lineas de factura de proveedor. (Puntero al subformulario).
*/
ListLinFacturaProveedorView* FacturaProveedor::getlistalineas() {
	_depura("FacturaProveedor::getlistalineas", 0);
	return listalineas;
}

/** Devuelve la lista de lineas de descuento. (Puntero al subformulario).
*/
ListDescuentoFacturaProvView* FacturaProveedor::getlistadescuentos() {
	_depura("FacturaProveedor::getlistadescuentos", 0);
	return listadescuentos;
}

/// Establece cual es la lista subformulario del presupuesto. Normalmente para
/// apuntar listlinpresupuestoview.
void FacturaProveedor::setListLinFacturaProveedor(ListLinFacturaProveedorView *a) {
	listalineas = a;
	listalineas->setcompany(companyact);
}
void FacturaProveedor::setListDescuentoFacturaProv(ListDescuentoFacturaProvView *a) {
	listadescuentos = a;
	listadescuentos->setcompany(companyact);
}
void FacturaProveedor::setidproveedor(QString val) {
	setDBvalue("idproveedor", val);
}
void FacturaProveedor::setreffacturap(QString val) {
	setDBvalue("reffacturap", val);
}
void FacturaProveedor::setnumfacturap(QString val) {
	setDBvalue("numfacturap", val);
}
void FacturaProveedor::setfechafacturap(QString val) {
	setDBvalue("ffacturap", val);
}
void FacturaProveedor::setdescfacturap(QString val) {
	setDBvalue("descfacturap", val);
}
void FacturaProveedor::setcomentfacturap(QString val) {
	setDBvalue("comentfacturap", val);
}
void FacturaProveedor::setidfacturap(QString val) {
	_depura("FacturaProveedor::setidfacturap", 0);
	setDBvalue("idfacturap", val);
	listalineas->setColumnValue("idfacturap", val);
	listadescuentos->setColumnValue("idfacturap", val);
	_depura("END FacturaProveedor::setidfacturap", 0);
}
void FacturaProveedor::setidforma_pago(QString val) {
	setDBvalue("idforma_pago", val);
}
void FacturaProveedor::setprocesadafacturap(QString val) {
	setDBvalue("procesadafacturap", val);
}

/** Calcula el total de factura, los impuestos y descuentos y se encarga de que se muestre adecuadamente.
*/
void FacturaProveedor::calculaypintatotales() {
    _depura("FacturaProveedor::calculaypintatotales", 0);

    /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
    int res = g_plugins->lanza("FacturaProveedor::_calculaypintatotales", this);
    if (res != 0)
        return;

    base basesimp;
    SDBRecord *linea;
    /// Impresion de los contenidos.
    QString l;
    for (int i = 0; i < listalineas->rowCount(); ++i) {
        linea = listalineas->lineaat(i);
        Fixed cant(linea->DBvalue("cantlfacturap").toAscii().constData());
        Fixed pvpund(linea->DBvalue("pvplfacturap").toAscii().constData());
        Fixed desc1(linea->DBvalue("descuentolfacturap").toAscii().constData());
        Fixed cantpvp = cant * pvpund;
        Fixed base = cantpvp - cantpvp * desc1 / 100;
        Fixed val("0.00");
        val = basesimp[linea->DBvalue("ivalfacturap")];
        val = val + base;
        QString lin = linea->DBvalue("ivalfacturap");
        basesimp[lin] = val;
    } // end for
    Fixed basei("0.00");
    base::Iterator it;
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        basei = basei + it.value();
    } // end for

    /// Impresion de los descuentos.
    Fixed porcentt("0.00");
    SDBRecord *linea1;
    if (listadescuentos->rowCount()) {
        for (int i = 0; i < listadescuentos->rowCount(); ++i) {
            linea1 = listadescuentos->lineaat(i);
            Fixed propor(linea1->DBvalue("proporciondfacturap").toAscii().constData());
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
    pintatotales(totiva, totbaseimp, totiva + totbaseimp, basei * porcentt / 100);
    _depura("END FacturaProveedor::calculaypintatotales", 0);
}
