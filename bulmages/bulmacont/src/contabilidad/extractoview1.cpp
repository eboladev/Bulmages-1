/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include <QLineEdit>
#include <QWidget>
#include <QToolButton>
#include <QLayout>
#include <QTextStream>
#include <QCheckBox>
#include <QFileDialog>

#include "extractoview1.h"
#include "extractoprintview.h"
#include "libromayorprint.h"
#include "calendario.h"
#include "empresa.h"
#include "asiento1view.h"
#include "balanceview.h"
#include "diarioview.h"
#include "listcuentasview1.h"
#include "selectccosteview.h"
#include "selectcanalview.h"
#include "busquedafecha.h"
#include "busquedacuenta.h"
#include "extractosubform.h"


/// Este archivo contiene la implementaci&oacute;n de la clase extractoview1 que saca el
/// extracto por pantalla de una o varias cuentas determinadas. Esta clase es una de las
/// tres principales junto a \ref introapuntes1 y \ref diarioview1.
extractoview1::extractoview1(empresa *emp, QWidget *parent, int)
        : Ficha(parent) {
    _depura("extractoview1::extractoview1", 0);
    setupUi(this);
    mui_list->setcompany(emp);
    m_companyact = emp;

    /// Iniciamos los componentes
    m_codigoinicial->setempresa(emp);
    m_codigofinal->setempresa(emp);
    mui_codigocontrapartida->setempresa(emp);
    m_codigoinicial->hideNombre();
    m_codigofinal->hideNombre();

    /// Iniciamos los componentes de la fecha para que al principio aparezcan
    /// como el a&ntilde;o inicial.
    QString cadena;
    cadena.sprintf("%2.2d/%2.2d/%4.4d", 1, 1, QDate::currentDate().year());
    m_fechainicial1->setText(cadena);
    cadena.sprintf("%2.2d/%2.2d/%4.4d", 31, 12, QDate::currentDate().year());
    m_fechafinal1->setText(cadena);
    m_cursorcta = NULL;
    m_companyact->meteWindow(windowTitle(), this);
    _depura("END extractoview1::extractoview1", 0);
}

/// Destructor de la clase.
extractoview1::~extractoview1() {
    _depura("extractoview1::~extractoview1\n", 0);
    delete m_cursorcta;
    m_companyact->sacaWindow(this);
    _depura("END extractoview1::~extractoview1\n", 0);
}


/// Esta funci&oacute;n carga el cursor de cuentas que forman el todo por el todo.
/** Tambi&eacute;n ser&aacute; la encargada de recoger la informaci&oacute;n de
    filtraci&oacute;n para que todo sea correcto. */
void extractoview1::accept() {
    _depura("extractoview1::accept", 0);
    QString codinicial = m_codigoinicial->codigocuenta();
    QString codfinal = m_codigofinal->codigocuenta();
    QString query;
    /// Si los datos de c&oacute;digo inicial y final est&aacute;n vacios los ponemos
    /// nosotros.
    if (codinicial == "") {
        codinicial = "0";
    } // end if
    if (codfinal == "") {
        codfinal = "9999999";
    } // end if
    query = "SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM apunte) AND codigo >= '" + codinicial + "' AND codigo <= '" + codfinal + "' ORDER BY codigo";

    if (m_cursorcta != NULL) {
        delete m_cursorcta;
    } // end if
    m_cursorcta = m_companyact->cargacursor(query,"cursorcuenta");
    presentar();
    _depura("END extractoview1::accept",0);
}


/// Esta es la funci&oacute;n que avanza un registro entre las cuentas.
void extractoview1::boton_siguiente() {
    _depura("extractoview1::boton_siguiente", 0);
    if (m_cursorcta != NULL) {
        if (!m_cursorcta->esultimoregistro()) {
            m_cursorcta->siguienteregistro();
            presentar();
        } // end if
    } // end if
    _depura("END extractoview1::boton_siguiente", 0);
}


/// Esta es la funci&oacute;n que retrocede un registro entre las cuentas.
void extractoview1::boton_anterior() {
    _depura("extractoview1::boton_anterior", 0);
    if (m_cursorcta != NULL) {
        if (!m_cursorcta->esprimerregistro()) {
            m_cursorcta->registroanterior();
            presentar();
        } // end if
    } // end if
    _depura("END extractoview1::boton_anterior", 0);
}

/// Retrocede al principio de las cuentas.
void extractoview1::boton_inicio() {
    _depura("extractoview1::boton_inicio", 0);
    if (m_cursorcta != NULL) {
        m_cursorcta->primerregistro();
        presentar();
    } // end if
    _depura("END extractoview1::boton_inicio", 0);
}

/// Avanza al final de las cuentas.
void extractoview1::boton_fin() {
    _depura("extractoview1::boton_fin", 0);
    if (m_cursorcta != NULL) {
        m_cursorcta->ultimoregistro();
        presentar();
    } // end if
    _depura("END extractoview1::boton_fin", 0);
}


/// Imprime el extracto
void extractoview1::boton_imprimir() {
    _depura("extractoview1::boton_imprimir", 0);
    ExtractoPrintView *print = new ExtractoPrintView(m_companyact, 0);
    print->exec();
    _depura("END extractoview1::boton_imprimir", 0);
}


void extractoview1::boton_guardar() {
    _depura("extractoview1::boton_guardar", 0);
    QString fn = QFileDialog::getSaveFileName(this,
                 tr("Guardar libro diario"),
                 confpr->valor(CONF_DIR_USER),
                 tr("Diarios (*.txt)"));

    if (!fn.isEmpty()) {
        libromayorprint libromayor;
        QString finicial = m_fechainicial1->text().toAscii().constData();
        QString ffinal = m_fechafinal1->text().toAscii().constData();
        libromayor.inicializa(m_companyact);
        libromayor.inicializa1(m_codigoinicial->text(), m_codigofinal->text(), finicial, ffinal);
        libromayor.inicializa2((char *)fn.toAscii().constData());
        libromayor.accept();
    } // end if
    _depura("END extractoview1::boton_guardar", 0);
}


/// Limpia los totales
void extractoview1::vaciar() {
    _depura("extractoview1::vaciar", 0);
    inicialdebe->setText("0");
    inicialhaber->setText("0");
    inicialsaldo->setText("0");
    totaldebe->setText("0");
    totalhaber->setText("0");
    totalsaldo->setText("0");
    _depura("END extractoview1::vaciar", 0);
}


void extractoview1::ajustes() {}


void extractoview1::on_mui_guardar_clicked() {
    _depura("extractoview1::on_mui_guardar_clicked", 0);
    mui_list->guardar();
    _depura("END extractoview1::on_mui_guardar_clicked", 0);

}


/// Esta funci&oacute;n se encarga de montar la consulta que va a hacer la consulta en
/// la base de datos.
void extractoview1::presentar() {
    _depura("extractoview1::presentar", 0);
    float debe, haber, saldo;
    float debeinicial = 0, haberinicial = 0, saldoinicial = 0;
    float debefinal, haberfinal, saldofinal;
    QString idcuenta;
    QString finicial = m_fechainicial1->text();
    QString ffinal = m_fechafinal1->text();
    QString contra = mui_codigocontrapartida->text();

    /// Preparamos el string para que aparezca una u otra cosa seg&uacute;n el punteo.
    QString tipopunteo;
    tipopunteo = "";
    if (mui_punteotodos->isChecked()) {
        tipopunteo = "";
    } else if (mui_punteopunteado->isChecked()) {
        tipopunteo = " AND punteo = TRUE ";
    } else {
        tipopunteo = " AND punteo = FALSE ";
    } // end if

    if (contra != "") {
        tipopunteo += " AND apunte.contrapartida = id_cuenta('" + contra + "') ";
    } // end if

    QString cad;
    QString cadaux;
    cursor2 *cursorapt;

    if (m_cursorcta->eof() || m_cursorcta->bof())
        return;

    idcuenta = m_cursorcta->valor("idcuenta");
    /// Escribimos el nombre de la cuenta y el c&oacute;digo de la misma.
    codigocuenta->setText(m_cursorcta->valor("codigo"));
    nombrecuenta->setText(m_cursorcta->valor("descripcion"));
    /// Hacemos la consulta de los apuntes a listar en la base de datos.
    QString query = "";
    /// Al igual que en el caso anterior los centros de coste han cambiado y a&uacute;n
    /// no se pueden implementar.
    selectcanalview *scanal = m_companyact->getselcanales();
    selectccosteview *scoste = m_companyact->getselccostes();
    QString ccostes = scoste->cadcoste();
    if (ccostes != "") {
        ccostes.sprintf(" AND idc_coste IN (%s) ", ccostes.toAscii().constData());
    } // end if
    QString ccanales = scanal->cadcanal();
    if (ccanales != "") {
        ccanales.sprintf(" AND idcanal IN (%s) ", ccanales.toAscii().constData());
    } // end if
    QString tabla;
    QString cont;
    if (mui_asAbiertos->isChecked()) {
        tabla = "borrador";
        cont = " FALSE AS punteo, * ";
    } else {
        tabla = "apunte";
        cont = " * ";
    } // end if

    query = "SELECT * FROM ((SELECT "+cont+" FROM " + tabla + " WHERE  idcuenta = " + idcuenta + " AND fecha >= '" + finicial + "' AND fecha <= '" + ffinal + "' " + ccostes + " " + ccanales + " " + tipopunteo + ") AS t2 LEFT JOIN cuenta ON t2.idcuenta = cuenta.idcuenta) AS t1 LEFT JOIN asiento ON asiento.idasiento = t1.idasiento ";
    query += " LEFT JOIN (SELECT idc_coste, nombre AS nombrec_coste FROM c_coste) AS t5 ON t5.idc_coste = t1.idc_coste ";
    query += " LEFT JOIN (SELECT idcanal, nombre AS nombrecanal FROM canal) AS t6 ON t6.idcanal = t1.idcanal ";
    query += " ORDER BY t1.fecha, ordenasiento, t1.orden";

    cursorapt=m_companyact->cargacursor(query);
    mui_list->cargar(cursorapt);

    cursorapt->primerregistro();
    if (!cursorapt->eof()) {
        /// Cargamos los saldos iniciales.
        cursor2 *cursoraux;
        query = "SELECT sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte WHERE idcuenta =" + idcuenta + " AND fecha < '" + finicial + "'";
        cursoraux = m_companyact->cargacursor(query);
        if (!cursoraux->eof()) {
            debeinicial = atof(cursoraux->valor("tdebe").toAscii());
            haberinicial = atof(cursoraux->valor("thaber").toAscii());
            saldoinicial = debeinicial - haberinicial;
        } // end if
        delete cursoraux;

        cadaux.sprintf("%5.2f", debeinicial);
        inicialdebe->setText(cadaux);
        cadaux.sprintf("%5.2f", haberinicial);
        inicialhaber->setText(cadaux);
        cadaux.sprintf("%5.2f", saldoinicial);
        inicialsaldo->setText(cadaux);
        saldo = saldoinicial;
        debefinal = debeinicial;
        haberfinal = haberinicial;

        while (!cursorapt->eof()) {
            debe = atof(cursorapt->valor("debe").toAscii());
            haber = atof(cursorapt->valor("haber").toAscii());
            saldo += debe - haber;
            debefinal += debe;
            haberfinal += haber;
            cursorapt->siguienteregistro();
        } // end while

        saldofinal = debefinal - haberfinal;
        cadaux.sprintf("%5.2f", debefinal);
        totaldebe->setText(cadaux);
        cadaux.sprintf("%5.2f", haberfinal);
        totalhaber->setText(cadaux);
        cadaux.sprintf("%5.2f", saldofinal);
        totalsaldo->setText(cadaux);
    } // end if
    delete cursorapt;
    ajustes();
    _depura("END extractoview1::presentar", 0);
}


void extractoview1::inicializa1(QString codinicial, QString codfinal, QString fecha1, QString fecha2, int) {
    _depura("extractoview1::inicializa1", 0);
    m_codigoinicial->setText(codinicial);
    m_codigofinal->setText(codfinal);
    m_fechainicial1->setText(normalizafecha(fecha1).toString("dd/MM/yyyy"));
    m_fechafinal1->setText(normalizafecha(fecha2).toString("dd/MM/yyyy"));
    _depura("END extractoview1::inicializa1", 0);
}


/// Realiza la casacion de los apuntes.
void extractoview1::on_mui_casacion_clicked() {
    _depura("extractoview1::on_mui_casacion_clicked", 0);
    QString query;
    query.sprintf("SELECT * FROM apunte WHERE punteo = FALSE AND haber <> 0 AND idcuenta = %s ORDER BY fecha", m_cursorcta->valor("idcuenta").toAscii().constData());
    m_companyact->begin();
    cursor2 *curshaber = m_companyact->cargacursor(query, "curshaber");
    m_companyact->commit();
    while (!curshaber->eof()) {
        query.sprintf("SELECT * FROM apunte WHERE punteo = FALSE AND debe = %s AND idcuenta = %s ORDER BY fecha", curshaber->valor("haber").toAscii().constData(), m_cursorcta->valor("idcuenta").toAscii().constData());
        m_companyact->begin();
        cursor2 *cursdebe = m_companyact->cargacursor(query.toAscii(), "cursdebe");
        m_companyact->commit();
        if (!cursdebe->eof()) {
            query.sprintf("UPDATE apunte set punteo = TRUE WHERE idapunte = %s", curshaber->valor("idapunte").toAscii().constData());
            m_companyact->begin();
            m_companyact->ejecuta(query);
            query.sprintf("UPDATE apunte SET punteo = TRUE WHERE idapunte = %s", cursdebe->valor("idapunte").toAscii().constData());
            m_companyact->ejecuta(query);
            m_companyact->commit();
        } // end if
        delete cursdebe;
        curshaber->siguienteregistro();
    } // end while
    delete curshaber;
    presentar();
    _depura("END extractoview1::on_mui_casacion_clicked", 0);
}


/// Guarda el punteo en disco para poder recuperarlo despues
void extractoview1::on_mui_guardarpunteo_clicked() {
    _depura("extractoview1::on_mui_guardarpunteo_clicked", 0);
    QString fn = QFileDialog::getSaveFileName(this,
                 tr("Guardar punteo"),
                 confpr->valor(CONF_DIR_USER),
                 tr("Punteos (*.pto)"));

    if (!fn.isEmpty()) {
        FILE *mifile;
        mifile = fopen((char *) fn.toAscii().constData(), "wt");
        if (mifile != NULL) {
            QString query;
            query = "SELECT * FROM apunte WHERE punteo = TRUE";
            cursor2 *cursp = m_companyact->cargacursor(query, "punteos");
            while (!cursp->eof()) {
                fprintf(mifile,"%s\n", cursp->valor("idapunte").toAscii().constData());
                cursp->siguienteregistro();
            } // end while
            delete cursp;
            fclose(mifile);
        } // end if
    } // end if
    _depura("END extractoview1::on_mui_guardarpunteo_clicked", 0);
}


/// Esta funci&oacute;n borra todo el punteo de un extracto y lo pone a cero.
/** Esta funci&oacute;n se activa justo cuando se pulsa sobre el bot&oacute;n de
    resetear el punteo.
    Por supuesto cuando se pulsa dicho bot&oacute;n se borra el punteo. */
void extractoview1::on_mui_borrapunteo_clicked() {
    _depura("extractoview1::on_mui_borrapunteo_clicked", 0);
    int valor = QMessageBox::warning(0,
                                     tr("Borrar punteo"),
                                     tr("Se dispone a borrar el punteo. Este cambio \
                                        es irrecuperable si no ha guardado su el punte. \
                                        Desea continuar?"),
                                     QMessageBox::Yes, QMessageBox::No);
    if (valor == QMessageBox::Yes) {
        m_companyact->begin()
        ;
        m_companyact->ejecuta("UPDATE apunte SET punteo = FALSE");
        m_companyact->commit();
        presentar();
    } // end if
    _depura("END extractoview1::on_mui_borrapunteo_clicked", 0);
}


/// Carga los punteos desde un fichero de texto que se solicita mediante un QFileDialog.
/** El fichero de punteos requiere que no se hayan modificado los identificadores de
    borrador.
    Para ello es preciso que no se hayan abierto y cerrado los asientos correspondientes
    ya que en dicho caso la carga del punteo no funciona correctamente. */
void extractoview1::on_mui_cargarpunteos_clicked() {
    _depura("extractoview1::on_mui_cargarpunteos_clicked", 0);
    QString fn = QFileDialog::getOpenFileName(this,
                 tr("Cargar punteo"),
                 confpr->valor(CONF_DIR_USER),
                 tr("Punteo (*.pto)"));

    if (!fn.isEmpty()) {
	QFile file(fn);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return;
	} // end if
	QTextStream filestr(&file);
        QString a;
        m_companyact->ejecuta("UPDATE apunte SET punteo = FALSE");
        while (!filestr.atEnd()) {
            filestr >> a;
            QString query;
            query = "UPDATE apunte SET punteo = TRUE WHERE idapunte = "+ a;
            m_companyact->ejecuta(query);
        } // end while
        file.close();
    } // end if
    presentar();
    _depura("END extractoview1::on_mui_cargarpunteos_clicked", 0);
}


/// Slot que responde a la Impresion del extracto
void extractoview1::on_mui_imprimir_clicked() {
    _depura("extractoview1::on_mui_imprimir_clicked", 0);
    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "extracto.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "extracto.rml";
    QString archivologo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";
    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system(archivo.toAscii().constData());
    /// Copiamos el logo
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
    QString fitxersortidatxt;
    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += mui_list->imprimir();
    fitxersortidatxt += "</blockTable>";
    buff.replace("[story]",fitxersortidatxt);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    } // end if
    /// Crea el pdf y lo muestra.
    invocaPDF("extracto");
    _depura("END extractoview1::on_mui_imprimir_clicked", 0);
}

