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
#include <QMap>

#include "fixed.h"
#include "funcaux.h"
#include "albarancliente.h"

/// Definimos el tipo base que almacena los diferentes importes para las distintas bases imponibles.
typedef QMap<QString, Fixed> base;

/** Constructor de la clase que inicializa la clase, prepara la clase DBRecord
    para que trabaje en la base de datos con la tabla albaran y procese adecuadamente
    todos los campos de dicha tabla.    
*/    
AlbaranCliente::AlbaranCliente(company *comp) : DBRecord (comp)  {
    _depura("AlbaranCliente::AlbaranCliente(company *)", 0);
    m_companyact = comp;
    setDBTableName("albaran");
    setDBCampoId("idalbaran");
    addDBCampo("idalbaran", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("AlbaranCliente", "Id albaran"));
    addDBCampo("idcliente", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("AlbaranCliente", "Id cliente"));
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("AlbaranCliente", "Id almacen"));
    addDBCampo("numalbaran", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Numero de albaran"));
    addDBCampo("fechaalbaran", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Fecha albaran"));
    addDBCampo("contactalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Contacto albaran"));
    addDBCampo("telalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Telefono"));
    addDBCampo("comentalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Comentario"));
    addDBCampo("comentprivalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Comentario priv albaran"));
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Id forma de pago"));
    addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Id trabajador"));
    addDBCampo("procesadoalbaran", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Procesado albaran"));
    addDBCampo("descalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Descripcion albaran"));
    addDBCampo("refalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Referencia albaran"));
    listalineas = NULL;
    listadescuentos = NULL;
    _depura("END AlbaranCliente::AlbaranCliente(company *)", 0);
}


/// Destructor de la clase.
AlbaranCliente::~AlbaranCliente() {
    _depura("AlbaranCliente::~AlbaranCliente", 0);
    _depura("END AlbaranCliente::~AlbaranCliente", 0);
}

/** Metodo de borrar un albaran. Se encarga de mandar a la base de datos
    la instruccion necesaria para el borrado de un albaran y controlar
    los posibles errores que se produzcan.
    Tambi&eacute;n borra todas las lineas y las lineas de descuento que se refieren
    a el albaran que se pretende borrar.
*/
/// \todo: Este metodo deberia poderse delegar en DBRecord, o por lo menos la parte del borrado del registro.
/// \todo: Hace falta meter el metodo dentro del sistema de excepciones try catch.
int AlbaranCliente::borrar() {
    _depura("AlbaranCliente::borrar", 0);
    if (DBvalue("idalbaran") != "")  {
        m_companyact->begin();
        listalineas->borrar();
        listadescuentos->borrar();

        int error = m_companyact->ejecuta("DELETE FROM albaran WHERE idalbaran = " + DBvalue("idalbaran"));

        if (error)  {
            m_companyact->rollback();
            return -1;
        }
        m_companyact->commit();
    }
    _depura("END AlbaranCliente::borrar", 0);
    return 0;
}

/// Hace un vaciado de los elementos que se corresponden con la base de datos.
/// Es &uacute;til para sistemas de recarga de la pantalla.
void AlbaranCliente::vaciaAlbaranCliente() {
    _depura("AlbaranCliente::vaciaAlbaranCliente", 0);
    DBclear();
    _depura("END AlbaranCliente::vaciaAlbaranCliente", 0);
}

/** Se encarga de sincronizar los elementos de la base de datos con los de la pantalla
    Invocando este m&eacute;todo conseguimos que la pantalla muestre lo que hay en la base
    de datos. No invoca el pintado de las lineas de albar&aacute;n ni descuentos de albar&aacute;n.
    Sin embarco si que provoca el calculo y pintado de los totales.
*/
void AlbaranCliente::pintar() {
    _depura("AlbaranCliente::pintar", 0);
    pintaIdAlbaran(DBvalue("idalbaran"));
    pintaNumAlbaran(DBvalue("numalbaran"));
    pintafechaalbaran(DBvalue("fechaalbaran"));
    pintaComentAlbaran(DBvalue("comentalbaran"));
    pintaComentPrivAlbaran(DBvalue("comentprivalbaran"));
    pintaidcliente(DBvalue("idcliente"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintaidalmacen(DBvalue("idalmacen"));
    pintarefalbaran(DBvalue("refalbaran"));
    pintadescalbaran(DBvalue("descalbaran"));
    pintaidtrabajador(DBvalue("idtrabajador"));
    pintacontactalbaran(DBvalue("contactalbaran"));
    pintatelalbaran(DBvalue("telalbaran"));
    pintaprocesadoalbaran(DBvalue("procesadoalbaran"));
    /// Pinta el subformulario de detalle del AlbaranCliente.
    /// Pintamos los totales.
    calculaypintatotales();
    _depura("END AlbaranCliente::pintar", 0);
}


/** Este m&eacute;todo carga un AlbaranCliente. Tambi&eacute;n carga las lineas
    de albar&aacute;n y los descuentos de albar&aacute;n.
    Tras la carga tambi&eacute;n invoca un repintado del albaran para que se vea
    correctamente la pantalla.
*/
/// \TODO: Falta introducir este m&eacute;todo dentro de sentencias try catch.
int AlbaranCliente::cargar(QString idalbaran)  {
    _depura("AlbaranCliente::cargar", 0);
    QString query = "SELECT * FROM albaran WHERE idalbaran = " + idalbaran;
    cursor2 *cur = m_companyact->cargacursor(query);
    if (!cur->eof())
        DBload(cur);
    delete cur;
    listalineas->cargar(idalbaran);
    listadescuentos->cargar(idalbaran);
    pintar();
    _depura("Fin AlbaranCliente::cargar", 0);
    return 0;
}

/** Este m&eacute;todo se encarga de hacer el guardado del albar&aacute; en la
    base de datos. Una vez guardado se guardan las lineas de albar&aacute;n y los
    descuentos de albar&aacute;n. Tras hacer el guardado se hace una carga del albaran
    para recuperar los datos que haya podido escribir la base de datos automaticamente.
    Dichos datos son la referencia y el n&uacute;mero de albar&aacute;n.
    
    Si todo funciona bien este m&eacute;todo devuelve 0. Si se produce algun error
    se genera una excepcion -1.
*/    
int AlbaranCliente::guardar() {
    _depura("AlbaranCliente::guardar", 0);
    /// Todo el guardado es una transaccion.
    m_companyact->begin();
    try {
        QString id;
        DBsave(id);
        setidalbaran(id);
        listalineas->guardar();
        listadescuentos->guardar();
        m_companyact->commit();

        /// Hacemos una carga para recuperar datos como la referencia y el numero de albaran
        cargar(id);

        _depura("END AlbaranCliente::guardar", 0);
        return 0;
    } catch (...) {
        _depura("AlbaranCliente::guardar error al guardar albaran cliente", 0);
        m_companyact->rollback();
        throw -1;
    } // end try
}


/** Este m&eacute;todo se encarga de la impresi&oacute;n de un albar&aacute;n de 
    cliente. Para imprimir cogemos la plantilla albaran.rml del directorio de plantillas
    hacemos en ella todas las sustituciones por los valores reales correspondientes y
    guardamos el rml resultante en ~/.bulmages/albaran.rml una vez hecho esto invocamos
    a bgtrml2pdf.py para que transforme el rml en PDF.
*/
void AlbaranCliente::imprimirAlbaranCliente()  {
    _depura("AlbaranCliente::imprimirAlbaranCliente", 0);
    base basesimp;
    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "albaran.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "albaran.rml";
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
    QString fitxersortidatxt = "";
    /// L&iacute;nea de totales del albar&aacute;.
    QString SQLQuery = "SELECT * FROM cliente WHERE idcliente = " + DBvalue("idcliente");
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        buff.replace("[dircliente]", cur->valor("dircliente"));
        buff.replace("[poblcliente]",cur->valor("poblcliente"));
        buff.replace("[telcliente]", cur->valor("telcliente"));
        buff.replace("[nomcliente]", cur->valor("nomcliente"));
        buff.replace("[cifcliente]", cur->valor("cifcliente"));
        buff.replace("[idcliente]", cur->valor("idcliente"));
        buff.replace("[cpcliente]", cur->valor("cpcliente"));
    }
    delete cur;
    buff.replace("[numalbaran]", DBvalue("numalbaran"));
    buff.replace("[falbaran]", DBvalue("fechaalbaran"));
    buff.replace("[contactalbaran]", DBvalue("contactalbaran"));
    buff.replace("[telalbaran]", DBvalue("telalbaran"));
    buff.replace("[comentalbaran]", DBvalue("comentalbaran"));
    buff.replace("[descalbaran]", DBvalue("descalbaran"));
    buff.replace("[refalbaran]", DBvalue("refalbaran"));

    /// Impresi&oacute;n de la tabla de contenidos.
    QString l;

    SDBRecord *linea;
    for (int i = 0; i < listalineas->rowCount() - 1; ++i) {
        linea = listalineas->lineaat(i);
        Fixed base = Fixed(linea->DBvalue("cantlalbaran").toAscii().constData()) *
                     Fixed(linea->DBvalue("pvplalbaran").toAscii().constData());
        basesimp[linea->DBvalue("ivalalbaran")] = basesimp[linea->DBvalue("ivalalbaran")] + base -
                base * Fixed(linea->DBvalue("descontlalbaran").toAscii().constData()) / 100;

        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "        <td>" + XMLProtect(linea->DBvalue("codigocompletoarticulo")) + "</td>\n";
        fitxersortidatxt += "        <td>" + XMLProtect(linea->DBvalue("desclalbaran")) + "</td>\n";
        fitxersortidatxt += "        <td>" + l.sprintf("%s",linea->DBvalue("cantlalbaran").toAscii().constData()) + "</td>\n";
        fitxersortidatxt += "        <td>" + l.sprintf("%s",linea->DBvalue("pvplalbaran").toAscii().constData()) + "</td>\n";
        fitxersortidatxt += "        <td>" + l.sprintf("%s",linea->DBvalue("descontlalbaran").toAscii().constData()) + " %</td>\n";
        fitxersortidatxt += "        <td>" + l.sprintf("%s",(base - base *
                            Fixed(linea->DBvalue("descontlalbaran")) / 100).toQString().toAscii().constData())
                            + "</td>\n";
        fitxersortidatxt += "</tr>";
    }

    buff.replace("[story]", fitxersortidatxt);
    Fixed basei("0.00");
    base::Iterator it;

    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        basei = basei + it.value();
    }

    /// Impresi&oacute;n de los descuentos.
    fitxersortidatxt = "";
    Fixed porcentt("0.00");
    SDBRecord *linea1;
    if (listadescuentos->rowCount() - 1) {
        fitxersortidatxt += "<blockTable style=\"tabladescuento\" colWidths=\"12cm," \
                            " 2cm, 3cm\" repeatRows=\"1\">\n";
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "        <td>" + QApplication::translate("AlbaranCliente", "Descuento") + "</td>\n";
        fitxersortidatxt += "        <td>" + QApplication::translate("AlbaranCliente", "Porcentaje") + "</td>\n";
        fitxersortidatxt += "        <td>" + QApplication::translate("AlbaranCliente", "Total") + "</td>\n";
        fitxersortidatxt += "</tr>\n";

        for (int i = 0; i < listadescuentos->rowCount() - 1; ++i) {
            linea1 = listadescuentos->lineaat(i);
            porcentt = porcentt + Fixed(linea1->DBvalue("proporciondalbaran").toAscii().constData());
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "        <td>" + linea1->DBvalue("conceptdalbaran") + "</td>\n";
            fitxersortidatxt += "        <td>" + l.sprintf("%s", linea1->DBvalue("proporciondalbaran").toAscii().constData()) + " %</td>\n";
            fitxersortidatxt += "        <td>" + l.sprintf("-%s",
                                (Fixed(linea1->DBvalue("proporciondalbaran")) * basei /
                                 100).toQString().toAscii().constData()) + "</td>\n";
            fitxersortidatxt += "</tr>";
        } // end for

        fitxersortidatxt += "</blockTable>\n";
    }

    buff.replace("[descuentos]", fitxersortidatxt);
    /// Impresi&oacute;n de los totales.
    fitxersortidatxt = "";
    /// Rellena el primer 'tr' de titulares.
    QString tr1 = "";
    /// Rellena el segundo 'tr' de cantidades.
    QString tr2 = "";
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
        tr1 += "<td>" + QApplication::translate("AlbaranCliente", "Base ") + it.key() + " %</td>\n";
        tr2 += "<td>" + l.sprintf("%s", parbaseimp.toQString().toAscii().constData()) + "</td>\n";
    }

    Fixed totiva("0.0");
    Fixed pariva("0.0");

    for (it = basesimp.begin(); it != basesimp.end(); ++it) {

        if (porcentt > 0) {
            pariva = (it.value() - it.value() * porcentt / 100) * Fixed(it.key()) / 100;
        } else {
            pariva = it.value() * Fixed(it.key()) / 100;
        } // end if

        totiva = totiva + pariva;
        tr1 += "<td>" + QApplication::translate("AlbaranCliente", "I.V.A. ") + it.key() + " %</td>\n";
        tr2 += "<td>" + l.sprintf("%s", pariva.toQString().toAscii().constData()) + "</td>\n";
    } // end for

    tr1 += "<td>" + QApplication::translate("AlbaranCliente", "Total ") + "</td>\n";
    tr2 += "<td>" + l.sprintf("%s", (totiva + totbaseimp).toQString().toAscii().constData()) + "</td>\n";
    fitxersortidatxt += "<tr>" + tr1 + "</tr><tr>" + tr2 + "</tr></blockTable>\n";
    buff.replace("[totales]",fitxersortidatxt);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    } // end if

    invocaPDF("albaran");
    _depura("END AlbaranCliente::imprimirAlbaranCliente", 0);
}


/** Hace el c&aacute;lculo de los totales del albar&aacute;n a partir de las
    l&iacute;neas de albar&aacute;n y descuentos de albar&aacute;n.
    Una vez hecho esto hace una llamada a pintatotales para que los muestre
*/
void AlbaranCliente::calculaypintatotales()  {
    _depura("AlbaranCliente::calculaypintatotales.", 0);
    base basesimp;
    SDBRecord *linea;
    /// Impresi&oacute;n de los contenidos.
    QString l;

    for (int i = 0; i < listalineas->rowCount(); ++i) {
        linea = listalineas->lineaat(i);
        Fixed cant(linea->DBvalue("cantlalbaran").toAscii().constData());
        Fixed pvpund(linea->DBvalue("pvplalbaran").toAscii().constData());
        Fixed desc1(linea->DBvalue("descontlalbaran").toAscii().constData());
        Fixed cantpvp = cant * pvpund;
        Fixed base = cantpvp - cantpvp * desc1 / 100;
        Fixed val("0.00");
        val = basesimp[linea->DBvalue("ivalalbaran")];
        val = val + base;
        QString lin = linea->DBvalue("ivalalbaran");
        basesimp[lin] = val;
    } // end for

    Fixed basei("0.00");
    base::Iterator it;

    for (it = basesimp.begin(); it != basesimp.end(); ++it)
        basei = basei + it.value();

    /// Impresi&oacute;n de los descuentos.
    Fixed porcentt("0.00");
    SDBRecord *linea1;

    for(int i = 0; i < listadescuentos->rowCount(); ++i) {
        linea1 = listadescuentos->lineaat(i);
        Fixed propor(linea1->DBvalue("proporciondalbaran").toAscii().constData());
        porcentt = porcentt + propor;
    } // end for

    Fixed totbaseimp("0.00");
    Fixed parbaseimp("0.00");

    for (it = basesimp.begin(); it != basesimp.end(); ++it) {

        if (porcentt > Fixed("0.00")) {
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
    _depura("END AlbaranCliente::calculaypintatotales", 0);
}

