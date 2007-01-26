/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
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

#include <QMessageBox>

#include "funcaux.h"
#include "propiedadesempresa.h"
#include "empresa.h"


propiedadesempresa::propiedadesempresa(empresa *emp, QWidget *parent)
        : Ficha(parent) {
    _depura("propiedadesempresa::propiedadesempresa", 0);
    this->setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    m_companyact = emp;
    inicializa();
    m_companyact->meteWindow(windowTitle(), this);
    _depura("END propiedadesempresa::propiedadesempresa", 0);
}


propiedadesempresa::~propiedadesempresa() {
    _depura("propiedadesempresa::~propiedadesempresa", 0);
    m_companyact->sacaWindow(this);
    _depura("END propiedadesempresa::~propiedadesempresa", 0);

}


int propiedadesempresa::inicializa() {
    _depura("propiedadesempresa::inicializa", 0);
    int num;
    int i;

    QString query = "SELECT * FROM configuracion WHERE nombre = 'CodCuenta'";
    cursor2 *curs = m_companyact->cargacursor(query);
    num = curs->numregistros();
    if (num > 0) {
        modcodigo->setText(curs->valor("valor"));
    } // end if
    delete curs;

    tpropiedades->setColumnCount(2);

    QStringList etiquetas;
    etiquetas << tr("Propiedad") << tr("Valor");
    tpropiedades->setHorizontalHeaderLabels(etiquetas);

    query = "SELECT * FROM configuracion";
    m_companyact->begin();
    curs = m_companyact->cargacursor(query, "queryconf");
    m_companyact->commit();
    num = curs->numregistros();
    tpropiedades->setRowCount(100);
    i = 0;

    /// Rellena la tabla con Items.
    while (!curs->eof()) {
        QTableWidgetItem *tablaItem0 = new QTableWidgetItem(curs->valor("nombre"));
        tpropiedades->setItem(i, 0, tablaItem0);
        QTableWidgetItem *tablaItem1 = new QTableWidgetItem(curs->valor("valor"));
        tpropiedades->setItem(i, 1, tablaItem1);
        curs->siguienteregistro();
        i++;
    } // end while
    delete curs;

    /// Este bloque de c&oacute;digo realiza la consulta para obtener los datos fiscales
    /// de la empresa.
    QString empresa, ano;

    /// Tiene que usar la empresa elegida, no bulmages!!!! TODO
    query = "SELECT nombre, valor FROM configuracion";

    m_companyact->begin();
    cursor2 *cur = m_companyact->cargacursor(query, "datos");
    m_companyact->commit();

    QString n, v;
    int nTuples = cur->numregistros();
    QLineEdit *p;
    for (int i = 0; i < nTuples; i++) {
        p = NULL;
        n = cur->valor("nombre");
        v = cur->valor("valor");
        cur->siguienteregistro();

        if (n == "NombreEmpresa")
            p = mui_nombreempresa;
        if (n == "CIF")
            p = mui_cif;
        if (n == "Telefono")
            p = mui_telefono;
        if (n == "TipoVia")
            p = mui_tipovia;
        if (n == "NombreVia")
            p = mui_nombrevia;
        if (n == "NumeroVia")
            p = mui_numerovia;
        if (n == "Escalera")
            p = mui_escalera;
        if (n == "Piso")
            p = mui_piso;
        if (n == "Puerta")
            p = mui_puerta;
        if (n == "CodPostal")
            p = mui_codigopostal;
        if (n == "Municipio")
            p = mui_poblacion;
        if (n == "Provincia")
            p = mui_provincia;
        if (n == "Pais")
            p = mui_pais;
        if (p) {
            p->setText(v);
        } // end if
    }

    delete cur;
    return 0;
    _depura("END propiedadesempresa::inicializa", 2);
}


int propiedadesempresa::guardar() {
    _depura("propiedadesempresa::on_mui_guardar_clicked()", 0);
    int i = 0;
    int pos = 0;
    /// Iniciamos transaccion.
    m_companyact->begin();
    QString query = "DELETE FROM configuracion";
    m_companyact->ejecuta(query);

    if (tpropiedades->rowCount() > 0) {
        for (i = 0; i < tpropiedades->rowCount(); i++) {
            /// Si vale 0 significa que no hay ningun QTableWidgetItem en esa posicion.
            if ((tpropiedades->item(i, 0) != 0) && (tpropiedades->item(i, 1) != 0) && (tpropiedades->item(i, 0)->text() != "")) {
                QString SQLQuery;
                SQLQuery.sprintf("INSERT INTO configuracion (idconfiguracion, nombre, valor) VALUES (%d,'%s','%s')", pos,
                                 m_companyact->sanearCadena(tpropiedades->item(i, 0)->text()).toAscii().constData(),
                                 m_companyact->sanearCadena(tpropiedades->item(i, 1)->text()).toAscii().constData());
                m_companyact->ejecuta(SQLQuery);
                pos++;
            } // end if
        } // end for
    } // end if

    /// Este bloque de c&oacute;digo guarda los datos fiscales en la tabla
    /// configuraci&oacute;n.
    update_value("NombreEmpresa", mui_nombreempresa->text());
    update_value("CIF", mui_cif->text());
    update_value("Telefono", mui_telefono->text());
    update_value("TipoVia", mui_tipovia->text());
    update_value("NombreVia", mui_nombrevia->text());
    update_value("NumeroVia", mui_numerovia->text());
    update_value("Escalera", mui_escalera->text());
    update_value("Piso", mui_piso->text());
    update_value("Puerta", mui_puerta->text());
    update_value("CodPostal", mui_codigopostal->text());
    update_value("Poblacion", mui_poblacion->text());
    update_value("Provincia", mui_provincia->text());
    update_value("Pais", mui_pais->text());
    update_value("Tipo", "BulmaCont");

    /// Procesamos la transaccion.
    m_companyact->commit();

    _depura("END propiedadesempresa::on_mui_guardar_clicked()", 10);
    return 0;
}


void propiedadesempresa::update_value(QString n, QString v) {
    _depura("propiedadesempresa::update_value", 0);
    QString query = "SELECT * FROM configuracion WHERE nombre = '" + n + "'";
    cursor2 *cur = m_companyact->cargacursor(query, "configuracion");
    if (cur->numregistros() == 0) {
        query.sprintf("INSERT INTO configuracion (idconfiguracion, nombre, valor) VALUES ((select coalesce(max(idconfiguracion), 0) + 1 AS idconfiguracion FROM configuracion), '%s', '%s')", n.toAscii().constData(), v.toAscii().constData());
    } else {
        query = "UPDATE configuracion SET valor = '" + v + "' WHERE nombre = '" + n + "'";
    } // end if
    delete cur;
    m_companyact->ejecuta(query);
    _depura("END propiedadesempresa::update_value", 0);
}


bool propiedadesempresa::close() {
    _depura("propiedadesempresa::close", 0);
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::question(this,
                                  tr("Guardar cambios"),
                                  tr("Desea guardar los cambios?"),
                                  tr("&Guardar"), tr("&No guardar"), 0, 0, 1) == 0) {
            on_mui_guardar_clicked();
        } // end if
    } // end if
    _depura("END propiedadesempresa::close", 0);
    return QWidget::close();
}

/// Este SLOT corresponde a la pulsaci&oacute;n del bot&oacute;n de modificar plan contable
/// de la pantalla de configuraci&oacute;n.
/** Coge todas las cuentas del plan contable y trata sus c&oacute;digos de forma que los
    extiende o reduce al tama&ntilde;o especificado en el par&aacute;metro modcodigo.
    De momento esta funci&oacute;n est&aacute; incompleta y hace varias consideraciones
    que deber&iacute;an ir solucion&aacute;ndose (no trata los d&iacute;gitos de cuenta
    y subcuenta) por lo que siempre considera que las cuentas son 4 d&iacute;gitos. */
void propiedadesempresa::on_mui_modificarplan_clicked() {
    _depura("propiedadesempresa::on_mui_modificarplan_clicked", 0);
    unsigned int nlong = modcodigo->text().length();
    QString codigo;
    QString query = "SELECT * FROM cuenta";
    cursor2 *cur = m_companyact->cargacursor(query);
    while (!cur->eof()) {
        codigo = cur->valor("codigo");
        codigo = ajustacodigo(codigo, nlong);
        m_companyact->begin();
        query = "UPDATE cuenta SET codigo = '" + codigo + "' WHERE idcuenta = " + cur->valor("idcuenta");
        m_companyact->ejecuta(query);
        m_companyact->commit();
        cur->siguienteregistro();
    } // end while
    delete cur;
    query = "UPDATE configuracion SET valor = '" + modcodigo->text() + "' WHERE nombre = 'CodCuenta'";
    m_companyact->begin();
    m_companyact->ejecuta(query);
    m_companyact->commit();
    if (QMessageBox::warning(this,
                             tr("Salir del programa"),
                             tr("Para que los cambios tengan efecto\ndebe salir del programa y volver a entrar.\nSalir ahora?"),
                             tr("&Salir"), tr("&No salir"), 0, 0, 1) == 0) {
        exit(1)
        ;
    } // end if
    dialogChanges_cargaInicial();
    _depura("END propiedadesempresa::on_mui_modificarplan_clicked", 0);
}

