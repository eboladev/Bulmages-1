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

#include "albaranproveedor.h"


/** Constructor de la clase. Prepara el DBRecord para trabajar con la tabla albaranp
*/
AlbaranProveedor::AlbaranProveedor(company *comp)
        : DBRecord(comp) {
    _depura("AlbaranProveedor::AlbaranProveedor", 0);	
    companyact = comp;
    setDBTableName("albaranp");
    setDBCampoId("idalbaranp");
    addDBCampo("idalbaranp", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("AlbaranProveedor", "Id albaran proveedor"));
    addDBCampo("numalbaranp", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("AlbaranProveedor", "Numero albaran proveedor"));
    addDBCampo("fechaalbaranp", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("AlbaranProveedor", "Fecha albaran proveedor"));
    addDBCampo("comentalbaranp", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranProveedor", "Comentario albaran proveedor"));
    addDBCampo("idproveedor", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("AlbaranProveedor", "Id proveedor"));
    addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("AlbaranProveedor", "Id forma de pago"));
    addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("AlbaranProveedor", "Id almacen"));
    addDBCampo("refalbaranp", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranProveedor", "Referencia albaran proveedor"));
    addDBCampo("descalbaranp", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("AlbaranProveedor", "Descripcion albaran proveedor"));
    _depura("END AlbaranProveedor::AlbaranProveedor", 0);	
}


/** Destructor de la clase, no necesita funcionalidades adicionales.
*/
AlbaranProveedor::~AlbaranProveedor() {
    _depura("AlbaranProveedor::~AlbaranProveedor", 0);
    _depura("END AlbaranProveedor::~AlbaranProveedor", 0);
}


/** Se encarga del borrado en la base de datos de una albaran de proveedor.
    Si se produce algun error devuelve una excepcion.
    Primero llama al borrado de las lineas y luego a los descuentos.
    Por ultimo llama al borrado del registro correspondiente en albaranp.
*/
int AlbaranProveedor::borrar() {
    _depura("AlbaranProveedor::borrar", 0);
    try {
        if (DBvalue("idalbaranp") != "")  {
            companyact->begin();
            listalineas->borrar();
            listadescuentos->borrar();
            companyact->ejecuta("DELETE FROM albaranp WHERE idalbaranp = " + DBvalue("idalbaranp"));
            companyact->commit();
        } // end if
        _depura("END AlbaranProveedor::borrar", 0);
        return 0;
    } catch (...) {
        mensajeInfo( "Error al borrar el Albaran de Proveedor");
        companyact->rollback();
        return -1;
    } // end catch
}


/** Vacia los datos que puedan haber quedado en la clase DBRecord.
*/
void AlbaranProveedor::vaciaAlbaranProveedor()  {
    _depura("AlbaranProveedor::vaciaAlbaranProveedor", 0);
    DBclear();
    _depura("END AlbaranProveedor::vaciaAlbaranProveedor", 0);
}


/** Se encarga de hacerel pintado de la ficha del albaran.
    Para ello coge todos los campos e invoca a los metodos pinta... que deben estar
    implementados en la clase encargada de la visualizacion.
    Tambien pinta los totales.
*/
void AlbaranProveedor::pintar()  {
    _depura("AlbaranProveedor::pintar", 0);
    pintaidalbaranp(DBvalue("idalbaranp"));
    pintanumalbaranp(DBvalue("numalbaranp"));
    pintafechaalbaranp(DBvalue("fechaalbaranp"));
    pintacomentalbaranp(DBvalue("comentalbaranp"));
    pintaidproveedor(DBvalue("idproveedor"));
    pintaidforma_pago(DBvalue("idforma_pago"));
    pintaidalmacen(DBvalue("idalmacen"));
    pintarefalbaranp(DBvalue("refalbaranp"));
    pintadescalbaranp(DBvalue("descalbaranp"));
    /// Pintamos los totales.
    pintatotales(listalineas->calculabase(), listalineas->calculaiva());
    _depura("END AlbaranProveedor::pintar", 0);
}


/// Esta funcion carga un AlbaranProveedor.
/// Hace el query adecuado, carga el registro a traves de DBRecord.
/// Hace la carga de las lineas y de los descuentos.
/// Invoca al pintado.
int AlbaranProveedor::cargar(QString idbudget) {
    _depura("AlbaranProveedor::cargar", 0);
    QString query = "SELECT * FROM albaranp WHERE idalbaranp =" + idbudget;
    cursor2 * cur = companyact->cargacursor(query);

    if (!cur->eof())
        DBload(cur);

    delete cur;
    listalineas->cargar(idbudget);
    listadescuentos->cargar(idbudget);
    pintar();
    _depura("END AlbaranProveedor::cargar", 0);
    return 0;
}


/** Guarda el albaran de proveedor en la base de datos.
    Para ello hace el guardado de DBRecord y luego guarda las lineas y los descuentos.
    Una vez hecho el guardado se hace una carga para recuperar posibles datos que haya
    introducido la base de datos como la referencia.
    
    Si algo falla devuelve una excepcion -1.
*/
int AlbaranProveedor::guardar() {
    _depura("AlbaranProveedor::guardar", 0);
    QString id;
    try {
        companyact->begin();
        DBsave(id);
        setidalbaranp(id);
        listalineas->guardar();
        listadescuentos->guardar();
        companyact->commit();

	/// Hacemos una carga para recuperar la referencia
	cargar(id);

        _depura("END AlbaranProveedor::guardar", 0);
        return 0;
    } catch(...) {
        _depura("AlbaranProveedor::guardar error al guardar", 2);
        companyact->rollback();
        throw -1;
    } // end catch
}


/** La impresion del albaran de proveedor no esta activada en estos momentos
    ya que no se prevee que un albaran de un proveedor vaya a ser imprimido ya que
    es un documento que entregan a la empresa y no uno que la empresa emite.
*/
void AlbaranProveedor::imprimirAlbaranProveedor() {
    /// Copiamos el archivo.
    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "albaranpproveedor.rml";
    archivo = "cp " + archivo + " /tmp/albaranpproveedor.rml";
    system(archivo.toAscii().constData());
    /// Copiamos el logo.
    archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";
    archivo = "cp " + archivo + " /tmp/logo.jpg";
    system(archivo.toAscii().constData());

    QFile file;
    file.setFileName("/tmp/albaranpproveedor.rml");
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt;
    /// Linea de totales del presupuesto.
    QString SQLQuery = "SELECT * FROM proveedor WHERE idproveedor = " + DBvalue("idproveedor");
    cursor2 *cur = companyact->cargacursor(SQLQuery);

    if (!cur->eof()) {
        buff.replace("[dirproveedor]", cur->valor("dirproveedor"));
        buff.replace("[poblproveedor]", cur->valor("poblproveedor"));
        buff.replace("[telproveedor]", cur->valor("telproveedor"));
        buff.replace("[nomproveedor]", cur->valor("nomproveedor"));
        buff.replace("[cifproveedor]", cur->valor("cifproveedor"));
    } // end if

    buff.replace("[numalbaranp]", DBvalue("numalbaranp"));
    buff.replace("[fechaalbaranp]", DBvalue("fechaalbaranp"));
    buff.replace("[comentalbaranp]", DBvalue("comentalbaranp"));
    buff.replace("[descalbaranp]", DBvalue("descalbaranp"));
    buff.replace("[refalbaranp]", DBvalue("refalbaranp"));

    fitxersortidatxt = "<blockTable style=\"tabla\" colWidths=\"10cm, 2cm, 2cm, " \
                       "3cm\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "        <td>" + QApplication::translate("AlbaranProveedor", "Concepto") + "</td>";
    fitxersortidatxt += "        <td>" + QApplication::translate("AlbaranProveedor", "Cantidad") + "</td>";
    fitxersortidatxt += "        <td>" + QApplication::translate("AlbaranProveedor", "Precio/u.") + "</td>";
    fitxersortidatxt += "        <td>" + QApplication::translate("AlbaranProveedor", "Total") + "</td>";
    fitxersortidatxt += "</tr>";

    QString l;
    SDBRecord *linea;

    for (int i = 0; i < listalineas->rowCount(); ++i) {
        linea = listalineas->lineaat(i);
        fitxersortidatxt += "<tr>";
        fitxersortidatxt += "        <td>" + linea->DBvalue("desclalbaranp") + "</td>";
        fitxersortidatxt += "        <td>" + linea->DBvalue("cantlalbaranp") +"</td>";
        fitxersortidatxt += "        <td>" + linea->DBvalue("pvplalbaranp") +"</td>";
        fitxersortidatxt += "        <td>" + (Fixed(linea->DBvalue("pvplalbaranp")) * Fixed(linea->DBvalue("pvplalbaranp"))).toQString() + "</td>";
        fitxersortidatxt += "</tr>";
        i++;
    }

    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "        <td></td>";
    fitxersortidatxt += "        <td></td>";
    fitxersortidatxt += "        <td>" + QApplication::translate("AlbaranProveedor", "Base imponible") + "</td>";
    fitxersortidatxt += "        <td>" + listalineas->calculabase().toQString() + "</td>";
    fitxersortidatxt += "</tr>";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "        <td></td>";
    fitxersortidatxt += "        <td></td>";
    fitxersortidatxt += "        <td>" + QApplication::translate("AlbaranProveedor", "I.V.A.") + "</td>";
    fitxersortidatxt += "        <td>" + listalineas->calculaiva().toQString() + "</td>";
    fitxersortidatxt += "</tr>";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "        <td></td>";
    fitxersortidatxt += "        <td></td>";
    fitxersortidatxt += "        <td>" + QApplication::translate("AlbaranProveedor", "Total") + "</td>";
    fitxersortidatxt += "        <td>" + (listalineas->calculabase() + listalineas->calculaiva()).toQString() + "</td>";
    fitxersortidatxt += "</tr>";
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]", fitxersortidatxt);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    } // end if

    system("bgtrml2pdf.py /tmp/albaranpproveedor.rml > /tmp/albaranpproveedor.pdf");
    system("kpdf /tmp/albaranpproveedor.pdf &");
}

