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
        : QWidget(parent), dialogChanges(this) {
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
    int num;
    int i;

    QString query = "SELECT * FROM configuracion WHERE nombre = 'CodCuenta'";
    cursor2 *curs = m_companyact->cargacursor(query);
    num = curs->numregistros();
    if (num > 0) {
        modcodigo->setText(curs->valor("valor"));
    } // end if
    delete curs;

    tpropiedades->setNumCols(2);

    tpropiedades->horizontalHeader()->setLabel(0, tr( "Propiedad"));
    tpropiedades->horizontalHeader()->setLabel(1, tr( "Valor"));

    query = "SELECT * FROM configuracion";
    m_companyact->begin();
    curs = m_companyact->cargacursor(query, "queryconf");
    m_companyact->commit();
    num = curs->numregistros();
    tpropiedades->setNumRows(100);
    i = 0;
    while (!curs->eof()) {
        tpropiedades->setText(i, 0, curs->valor("nombre"));
        tpropiedades->setText(i, 1, curs->valor("valor"));
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
            p = m_nomEmpresa;
        if (n == "CIF")
            p = lineCIF;
        if (n == "TipoVia")
            p = lineTipoVia;
        if (n == "NombreVia")
            p = lineNombreVia;
        if (n == "NumeroVia")
            p = lineNumeroVia;
        if (n == "Escalera")
            p = lineEscalera;
        if (n == "Piso")
            p = linePiso;
        if (n == "Puerta")
            p = linePuerta;
        if (n == "CodPostal")
            p = lineCodPostal;
        if (n == "Municipio")
            p = lineMunicipio;
        if (n == "Provincia")
            p = lineProvincia;
        if (n == "Pais")
            p = linePais;
        if (p) {
            p->setText(v);
        } // end if
    }
    delete cur;
    dialogChanges_cargaInicial();
    return(0);
}


void propiedadesempresa::s_saveConfig() {
    _depura("propiedadesempresa::s_saveConfig", 0);
    QString query = "DELETE FROM configuracion";
    m_companyact->ejecuta(query);
    int i = 0;
    while (tpropiedades->text(i, 0) != "") {
        QString SQLQuery;
        SQLQuery.sprintf("INSERT INTO configuracion (idconfiguracion, nombre, valor) VALUES (%d,'%s','%s')", i,
                         m_companyact->sanearCadena(tpropiedades->text(i, 0)).toAscii().constData(),
                         m_companyact->sanearCadena(tpropiedades->text(i, 1)).toAscii().constData());
        m_companyact->ejecuta(SQLQuery);
        i++;
    } // end while

    /// Este bloque de c&oacute;digo guarda los datos fiscales en la tabla configuraci&oacute;n.
    update_value("NombreEmpresa", m_nomEmpresa->text());
    update_value("CIF", lineCIF->text());
    update_value("TipoVia", lineTipoVia->text());
    update_value("NombreVia", lineNombreVia->text());
    update_value("NumeroVia", lineNumeroVia->text());
    update_value("Escalera", lineEscalera->text());
    update_value("Piso", linePiso->text());
    update_value("Puerta", linePuerta->text());
    update_value("CodPostal", lineCodPostal->text());
    update_value("Municipio", lineMunicipio->text());
    update_value("Provincia", lineProvincia->text());
    update_value("Pais", linePais->text());
    dialogChanges_cargaInicial();
    _depura("END propiedadesempresa::s_saveConfig", 0);
}


void propiedadesempresa::update_value(QString n, QString v) {
    _depura("propiedadesempresa::update_value", 0);
    QString query = "SELECT * FROM configuracion WHERE nombre = '" + n + "'";
    cursor2 *cur = m_companyact->cargacursor(query, "configuracion");
    if (cur->numregistros() == 0) {
        query.sprintf("INSERT INTO configuracion (idconfiguracion, nombre, valor) VALUES ((select max(idconfiguracion) + 1 FROM configuracion), '%s', '%s')", n.toAscii().constData(), v.toAscii().constData());
    } else {
        query = "UPDATE configuracion SET valor = '" + v + "' WHERE nombre = '" + n + "'";
    } // end if
    delete cur;
    m_companyact->ejecuta(query);
}


bool propiedadesempresa::close() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::question(this,
                                  tr("Guardar cambios"),
                                  tr("Desea guardar los cambios?"),
                                  tr("&Guardar"), tr("&No guardar"), 0, 0, 1) == 0) {
            s_saveConfig();
        } // end if
    } // end if
    return QWidget::close();
}

/// Este SLOT corresponde a la pulsaci&oacute;n del bot&oacute;n de modificar plan contable
/// de la pantalla de configuraci&oacute;n.
/** Coge todas las cuentas del plan contable y trata sus c&oacute;digos de forma que los
    extiende o reduce al tama&ntilde;o especificado en el par&aacute;metro modcodigo.
    De momento esta funci&oacute;n est&aacute; incompleta y hace varias consideraciones
    que deber&iacute;an ir solucion&aacute;ndose (no trata los d&iacute;gitos de cuenta
    y subcuenta) por lo que siempre considera que las cuentas son 4 d&iacute;gitos. */
void propiedadesempresa::extiendeCuentas() {
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
                             tr("Salir"),
                             tr("Para que los cambios tengan efecto, \
                                debe salir del programa y volver a entrar. \n \
                                Salir ahora?"),
                             tr("Salir"), tr("No salir"), 0, 0, 1) == 0) {
        exit(1)
        ;
    } // end if
    dialogChanges_cargaInicial();
}

