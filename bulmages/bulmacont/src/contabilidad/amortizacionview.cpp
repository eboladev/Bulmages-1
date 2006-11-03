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

#include <QPushButton>
#include <QRadioButton>
#include <QMenu>

#include "amortizacionview.h"
#include "empresa.h"
#include "funcaux.h"
#include "aplinteligentesview.h"
#include "busquedacuenta.h"
#include "asiento1view.h"

#define COL_FECHA                  0
#define COL_CUOTA                  1
#define COL_IDASIENTO              2
#define COL_EJERCICIO              3
#define COL_IDLINAMORTIZACION      4
#define COL_ORDENASIENTO           5


amortizacionview::amortizacionview(empresa *emp, QWidget *parent)
        : QWidget(parent), dialogChanges(this) {
    _depura("amortizacionview::amortizacionview", 0);
    this->setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    m_companyact = emp;
    idamortizacion = "";
    ctaactivo->setempresa(emp);
    ctaamortizacion->setempresa(emp);
    /// Buscamos cual es el asiento inteligente que realiza la amortizaci&oacute;n.
    QString query = "SELECT * FROM ainteligente, configuracion WHERE descripcion = valor AND configuracion.nombre = 'Amortizacion'";
    m_companyact->begin();
    cursor2 *cur = m_companyact->cargacursor(query, "hola");
    m_companyact->commit();
    if (!cur->eof()) {
        idainteligente = cur->valor("idainteligente");
    } // end if
    delete cur;
    idamortizacion = "";
    idctaactivo = "";
    idctaamortizacion = "";
    /// Ocultamos los campos que no interesa ver.
    table1->hideColumn(COL_IDASIENTO);
    table1->hideColumn(COL_EJERCICIO);
    table1->hideColumn(COL_IDLINAMORTIZACION);

    m_companyact->meteWindow(windowTitle(), this);
    _depura("END amortizacionview::amortizacionview", 0);
}


void amortizacionview::s_newAmortizacion() {}


void amortizacionview::s_deleteAmortizacion() {
    _depura("amortizacionview::s_deleteAmortizacion", 0);
    trataModificado();
    QString codigo = idamortizacion;
    if (codigo != "") {
        QString query = "DELETE FROM linamortizacion WHERE idamortizacion = " + codigo;
        m_companyact->begin();
        m_companyact->ejecuta(query);
        m_companyact->commit();
        query = "DELETE FROM amortizacion WHERE idamortizacion = " + codigo;
        m_companyact->begin();
        m_companyact->ejecuta(query);
        m_companyact->commit();
        close();
    } // end if
    _depura("END amortizacionview::s_deleteAmortizacion", 0);
}


void amortizacionview::s_saveAmortizacion() {
    _depura("amortizacionview::s_saveAmortizacion", 0);
    QString query;
    QString namortizacion = nomamortizacion->text();
    double valorcompradbl = valorcompra->text().toDouble();
    int numcuotasint = numcuotas->text().toInt();
    QString fechacomprastr = fechacompra->text();
    QString fecha1cuotastr = fecha1cuota->text();
    QString ctaactivostr = ctaactivo->codigocuenta();
    QString ctaamortizacionstr = ctaamortizacion->text();
    QString agrupacionstr = agrupacion->text();
    fprintf(stderr, "Vamos a hacer un accept\n");
    if (idamortizacion == "") {
        fprintf(stderr, "Se trata de una insercion");
        query.sprintf("INSERT INTO amortizacion (nomamortizacion, valorcompra, numcuotas, fechacompra,fecha1cuota, idcuentaactivo, idcuentaamortizacion, agrupacion) VALUES ('%s', %f, %d, '%s','%s', id_cuenta('%s'), id_cuenta('%s'),'%s')", namortizacion.toAscii().constData(), valorcompradbl, numcuotasint, fechacomprastr.toAscii().constData(), fecha1cuotastr.toAscii().constData(), ctaactivostr.toAscii().constData(), ctaamortizacionstr.toAscii().constData(), agrupacionstr.toAscii().constData());
        m_companyact->begin();
        if (m_companyact->ejecuta(query)) {
            /// El mensaje de error lo deber&iacute;a dar la funci&oacute;n ejecuta,
            /// por tanto esto va a ser obsoleto.
            QMessageBox::warning(this,
                                 tr("Error..."),
                                 tr("Ocurrio un error con la Base de Datos"),
                                 tr("&Aceptar"));
            return;
        } //  end if
        m_companyact->commit();
        query.sprintf("SELECT max(idamortizacion) AS idamortizacion FROM amortizacion");
        m_companyact->begin();
        cursor2 *cur = m_companyact->cargacursor(query, "unquery");
        m_companyact->commit();
        if (!cur->eof()) {
            idamortizacion = cur->valor("idamortizacion");
        } // end if
        /// Iteramos para cada l&iacute;nea en el subformulario.
        for (int i = 0; i < table1->numRows(); i++) {
            /// Insertamos en la base de datos cada l&iacute;nea de amortizaci&oacute;n.
            query.sprintf("INSERT INTO linamortizacion (idamortizacion, fechaprevista, cantidad) VALUES (%s, '%s', %s)", idamortizacion.toAscii().constData(), table1->text(i,COL_FECHA).toAscii().constData(), table1->text(i, COL_CUOTA).toAscii().constData());
            m_companyact->begin();
            m_companyact->ejecuta(query);
            m_companyact->commit();
        } // end for
    } else {
        fprintf(stderr, "Se trata de una modificacion\n");
        query.sprintf("UPDATE amortizacion SET nomamortizacion = '%s', valorcompra = %f, numcuotas = %d, fechacompra = '%s', idcuentaactivo = %s, idcuentaamortizacion = %s, fecha1cuota = '%s', agrupacion = '%s' WHERE idamortizacion = %s", namortizacion.toAscii().constData(), valorcompradbl, numcuotasint,fechacomprastr.toAscii().constData(), idctaactivo.toAscii().constData(), idctaamortizacion.toAscii().constData(), fecha1cuotastr.toAscii().constData(), agrupacionstr.toAscii().constData(), idamortizacion.toAscii().constData());
        m_companyact->begin();
        m_companyact->ejecuta(query);
        m_companyact->commit();
        /// Iteramos para cada linea en el subformulario.
        for (int i = 0; i < table1->numRows(); i++) {
            if (table1->text(i,COL_IDLINAMORTIZACION) != "") {
                /// Modificamos las entradas en la l&iacute;nea de amortizaci&oacute;n.
                query.sprintf("UPDATE linamortizacion SET fechaprevista = '%s', cantidad = %s WHERE idlinamortizacion = %s", table1->text(i, COL_FECHA).toAscii().constData(), table1->text(i, COL_CUOTA).toAscii().constData(), table1->text(i, COL_IDLINAMORTIZACION).toAscii().constData());
                m_companyact->begin();
                m_companyact->ejecuta(query);
                m_companyact->commit();
            } else {
                /// AQUI VAMOS A PONER LA INSERCION DE NUEVAS ENTRADAS DE AMORTIZACION.
                query = "INSERT INTO linamortizacion (idamortizacion, fechaprevista, cantidad) VALUES (" + idamortizacion + ",'" + table1->text(i, COL_FECHA) + "'," + table1->text(i, COL_CUOTA) + ")";
                m_companyact->begin();
                m_companyact->ejecuta(query);
                m_companyact->commit();
            } // end if
        } // end for
    } // end if
    dialogChanges_cargaInicial();
    _depura("END amortizacionview::s_saveAmortizacion", 0);
}


void amortizacionview::accept() {}


bool amortizacionview::close() {
    _depura("amortizacionview::close", 0);
    trataModificado();
    _depura("END amortizacionview::close", 0);
    return QWidget::close();
}

amortizacionview::~amortizacionview() {}


/// Esta funci&oacute;n inicializa la clase, y adem&aacute;s hace la presentaci&oacuet;n
/// de una determinada amortizaci&oacute;n.
void amortizacionview::inicializa(QString idamortiza) {
    _depura("amortizacionview::inicializa", 0);
    idamortizacion = idamortiza;
    fprintf(stderr, "Inicializamos el formulario %s\n", idamortizacion.toAscii().constData());
    QString query = "SELECT * FROM amortizacion LEFT JOIN (SELECT idcuenta AS idcta, codigo AS codctaactivo FROM cuenta) AS t1 ON t1.idcta = amortizacion.idcuentaactivo LEFT JOIN (SELECT idcuenta AS idcta1, codigo AS codctaamortizacion FROM cuenta) AS t2 ON t2.idcta1 = amortizacion.idcuentaamortizacion WHERE idamortizacion = " + idamortizacion;

    m_companyact->begin();
    cursor2 *curs = m_companyact->cargacursor(query, "unquery");
    m_companyact->commit();

    /// Si existe el registro que se pasa como par&aacute;metro.
    if (!curs->eof()) {
        /// Ponemos en pantalla todos los campos del registro de amortizaci&oacute;n que
        /// se ha cargado de la base de datos.
        nomamortizacion->setText(curs->valor("nomamortizacion"));
        valorcompra->setText(curs->valor("valorcompra"));
        numcuotas->setText(curs->valor("numcuotas"));
        QString cadena;
        cadena.sprintf("%10.10s", curs->valor("fechacompra").toAscii().constData());
        fechacompra->setText(cadena);
        cadena.sprintf("%10.10s", curs->valor("fecha1cuota").toAscii().constData());
        fecha1cuota->setText(cadena);
        ctaactivo->setText(curs->valor("codctaactivo"));
        idctaactivo = curs->valor("idcta");
        ctaamortizacion->setText(curs->valor("codctaamortizacion"));
        idctaamortizacion= curs->valor("idcta1");
        agrupacion->setText(curs->valor("agrupacion"));
    } // end if
    delete curs;

    /// Buscamos todos los plazos de amortizaci&oacute;n y los vamos poniendo.
    query = "SELECT *, fechaprevista <= now() AS ant FROM linamortizacion LEFT JOIN asiento ON linamortizacion.idasiento = asiento.idasiento WHERE idamortizacion = " + idamortizacion + " ORDER BY fechaprevista";
    m_companyact->begin();
    curs = m_companyact->cargacursor(query, "otroquery");
    m_companyact->commit();
    table1->setNumRows(curs->numregistros());
    for (int i = 0; i < curs->numregistros(); i++) {
        if (curs->valor("idasiento") == "" && curs->valor("ant") == "t") {
            table1->setItem(i, COL_FECHA, new QTableItem1(table1, Q3TableItem::OnTyping, "", 10));
            table1->setText(i, COL_FECHA, curs->valor("fechaprevista"));
            table1->setItem(i, COL_CUOTA, new QTableItem1(table1, Q3TableItem::OnTyping, "", 10));
            table1->setText(i, COL_CUOTA, curs->valor("cantidad"));
        } else {
            table1->setText(i, COL_FECHA, curs->valor("fechaprevista"));
            table1->setText(i, COL_CUOTA, curs->valor("cantidad"));
        } // end if
        table1->setText(i, COL_IDLINAMORTIZACION, curs->valor("idlinamortizacion"));
        table1->setText(i, COL_IDASIENTO, curs->valor("idasiento"));
        table1->setText(i, COL_ORDENASIENTO, curs->valor("ordenasiento"));
        curs->siguienteregistro();
    } // end for
    delete curs;

    /// Calculamos lo que ya llevamos amortizado y lo presentamos en la pantalla.
    query = "SELECT sum(cantidad) AS amortizado FROM linamortizacion WHERE idasiento IS NOT NULL AND idamortizacion = " + idamortizacion;
    m_companyact->begin();
    curs = m_companyact->cargacursor(query, "amortizado");
    m_companyact->commit();
    if (!curs->eof()) {
        amortizado->setText(curs->valor("amortizado"));
    } // end if
    delete curs;

    /// Calculamos lo que nos falta por amortizar y lo presentamos en la pantalla.
    query = "SELECT sum(cantidad) AS pdte FROM linamortizacion WHERE idasiento IS NULL AND idamortizacion = " + idamortizacion;
    m_companyact->begin();
    curs = m_companyact->cargacursor(query, "pdte");
    m_companyact->commit();
    if (!curs->eof()) {
        pendiente->setText(curs->valor("pdte"));
    } // end if
    delete curs;

    /// Deshabilitamos el bot&oacute;n de calcular, porque la amortizaci&oacute;n ya
    /// est&aacute; hecha.
    botoncalcular->setDisabled(TRUE);
    dialogChanges_cargaInicial();
    m_companyact->meteWindow(windowTitle(), this);
    _depura("END amortizacionview::inicializa", 0);
}


void amortizacionview::calculaamortizacion() {
    _depura("amortizacionview::calculaamortizacion", 0);
    /// Para hacer el c&iacute;rculo de los plazos de cada amortizaci&oacute;n
    /// Hay que obtener diversos datos.
    QDate f1cuota = normalizafecha(fecha1cuota->text());
    int ncuotas = numcuotas->text().toInt();
    QString periodicidadtxt = periodicidad->currentText();
    double valcuota;
    QString valcuotastr;
    table1->setNumRows(ncuotas);
    /// Si usamos el m&eacute;todo lineal:
    if (metodolineal->isChecked()) {
        valcuota = valorcompra->text().toDouble() / ncuotas;
        valcuotastr.sprintf("%10.2f", valcuota);
        for (int i = 0; i < ncuotas; i++) {
            fprintf(stderr, "calculo de una cuota %s\n", f1cuota.toString("dd/MM/yyyy").toAscii().constData());
            table1->setText(i, 0, f1cuota.toString("dd/MM/yyyy"));
            table1->setText(i, 1, valcuotastr);
            /// Dependiendo de la periodicidad actualizamos la fecha.
            if (periodicidadtxt == tr("Anual")) {
                f1cuota = f1cuota.addYears(1);
            } else if (periodicidadtxt == tr("Mensual")) {
                f1cuota = f1cuota.addMonths(1);
            } else if (periodicidadtxt == tr("Semestral")) {
                f1cuota = f1cuota.addMonths(6);
            } else if (periodicidadtxt == tr("Trimestral")) {
                f1cuota = f1cuota.addMonths(3);
            } // end if
        } // end for
    } else if (metodoincremental->isChecked()) {
        /// El m&eacute;todo de amortizaci&oacute;n es el incremental.
        double total = 0;
        for (int j = 1; j <= ncuotas; j++) {
            total = total + j;
        } // end for
        for (int i = 1; i <= ncuotas; i++) {
            valcuota = valorcompra->text().toDouble() * i/total;
            valcuotastr.sprintf("%10.2f", valcuota);
            fprintf(stderr, "calculo de una cuota %s\n", f1cuota.toString("dd/MM/yyyy").toAscii().constData());
            table1->setText(i - 1, 0, f1cuota.toString("dd/MM/yyyy"));
            table1->setText(i - 1, 1, valcuotastr);
            /// Dependiendo de la periodicidad actualizamos la fecha.
            if (periodicidadtxt == tr("Anual")) {
                f1cuota = f1cuota.addYears(1);
            } else if (periodicidadtxt == tr("Mensual")) {
                f1cuota = f1cuota.addMonths(1);
            } else if (periodicidadtxt == tr("Semestral")) {
                f1cuota = f1cuota.addMonths(6);
            } else if (periodicidadtxt == tr("Trimestral")) {
                f1cuota = f1cuota.addMonths(3);
            } // end if
        } // end for
    } else if (metododecremental->isChecked()) {
        /// El m&eacute;todo de amortizaci&oacute;n es el incremental.
        double total = 0;
        for (int j = 1; j <= ncuotas; j++) {
            total = total + j;
        } // end for
        for (int i = 0; i < ncuotas; i++) {
            valcuota = valorcompra->text().toDouble() * (ncuotas - i) / total;
            valcuotastr.sprintf("%10.2f", valcuota);
            fprintf(stderr, "calculo de una cuota %s\n", f1cuota.toString("dd/MM/yyyy").toAscii().constData());
            table1->setText(i, 0, f1cuota.toString("dd/MM/yyyy"));
            table1->setText(i, 1, valcuotastr);
            /// Dependiendo de la periodicidad actualizamos la fecha.
            if (periodicidadtxt == tr("Anual")) {
                f1cuota = f1cuota.addYears(1);
            } else if (periodicidadtxt == tr("Mensual")) {
                f1cuota = f1cuota.addMonths(1);
            } else if (periodicidadtxt == tr("Semestral")) {
                f1cuota = f1cuota.addMonths(6);
            } else if (periodicidadtxt == tr("Trimestral")) {
                f1cuota = f1cuota.addMonths(3);
            } // end if
        } // end for
    } else if (metodoporcentual->isChecked()) {
        /// El m&eacute;todo de amortizaci&oacute;n es el incremental.
        double porcent = (double) 1 / (double) ncuotas;
        fprintf(stderr, "El coeficiente es: %10.2f\n", porcent);
        double total = 0;
        for (int i = 0; i < ncuotas; i++) {
            if (i < (ncuotas - 1)) {
                valcuota = (valorcompra->text().toDouble()-total) * porcent;
                total += valcuota;
                fprintf(stderr, "cuota: %10.2f -- total: %10.2f\n", valcuota, total);
            } else {
                valcuota = valorcompra->text().toDouble() - total;
            } // end if
            valcuotastr.sprintf("%10.2f", valcuota);
            fprintf(stderr, "calculo de una cuota %s\n", f1cuota.toString("dd/MM/yyyy").toAscii().constData());
            table1->setText(i, 0, f1cuota.toString("dd/MM/yyyy"));
            table1->setText(i, 1, valcuotastr);
            /// Dependiendo de la periodicidad actualizamos la fecha.
            if (periodicidadtxt == tr("Anual")) {
                f1cuota = f1cuota.addYears(1);
            } else if (periodicidadtxt == tr("Mensual")) {
                f1cuota = f1cuota.addMonths(1);
            } else if (periodicidadtxt == tr("Semestral")) {
                f1cuota = f1cuota.addMonths(6);
            } else if (periodicidadtxt == tr("Trimestral")) {
                f1cuota = f1cuota.addMonths(3);
            } // end if
        } // end for
    } // end if
    _depura("amortizacionview::calculaamortizacion", 0);
}


/// SLOT que captura el men&uacute; contextual sobre la tabla de amortizaci&oacute;n.
/** Datos para el asiento de amortizaci&oacute;n.
    El asiento inteligente debe llamarse como el par&aacute;metro Amortizaci&oacute;n de confpr.
    El asiento inteligente debe tener las variables que se llamen:
    $cuenta$ Cuenta de Amortizaci&oacute;n.
    $cuentabien$ Cuenta del Bien.
    $cuota$ Cuota a pagar. */
void amortizacionview::contextMenuRequested(int row, int col, const QPoint &poin) {
    _depura("amortizacionview::contextMenuRequested", 0);

    QMenu *menupopup = new QMenu(this);
    QAction *opt1 = menupopup->addAction(tr("Generar asiento"));
    menupopup->addSeparator();
    QAction *opt2 = menupopup->addAction(tr("Ver asiento"));
    menupopup->addSeparator();
    QAction *opt3 = menupopup->addAction(tr("Desvincular asiento"));
    QAction *opt4 = menupopup->addAction(tr("Borrar asiento"));
    menupopup->addSeparator();
    QAction *opt5 = menupopup->addAction(tr("Insertar cuota"));
    QAction *opt6 = menupopup->addAction(tr("Borrar cuota"));

    if (table1->text(row, COL_IDASIENTO) == "") {
        opt1->setEnabled(TRUE);
        opt2->setEnabled(TRUE);
        opt3->setEnabled(TRUE);
        opt4->setEnabled(TRUE);
        opt5->setEnabled(TRUE);
        opt6->setEnabled(TRUE);
    } else {
        opt1->setEnabled(FALSE);
        opt2->setEnabled(FALSE);
        opt3->setEnabled(FALSE);
        opt4->setEnabled(FALSE);
        opt5->setEnabled(FALSE);
        opt6->setEnabled(FALSE);
    } // end if

    QAction *opcion = menupopup->exec(poin);

    delete menupopup;
    /// Inserci&oacute;n de una nueva cuota.
    if (opcion == opt5) {
        table1->insertRows(row + 1, 1);
    } // end if
    /// Eliminar una cuota.
    if (opcion == opt6) {
        QString idlinamortizacion = table1->text(row, COL_IDLINAMORTIZACION);
        QString query = "DELETE FROM linamortizacion WHERE idlinamortizacion = " + idlinamortizacion;
        if(idlinamortizacion != "") {
            m_companyact->begin();
            m_companyact->ejecuta(query);
            m_companyact->commit();
        } // end if
        table1->removeRow(row);
    } // end if
    if (opcion == opt2 || opcion == opt4) {
        /// Si se va a mostrar el asiento, o se va a borrar.
        QString idasiento = table1->text(row, COL_IDASIENTO);
        m_companyact->intapuntsempresa()->muestraasiento(idasiento.toInt());
    } // end if
    if (opcion == opt3 || opcion == opt4) {
        /// Si se va a desvincular el asiento o se va a borrar.
        QString idasiento = table1->text(row, COL_IDASIENTO);
        QString idlinamortizacion = table1->text(row, COL_IDLINAMORTIZACION);
        QString query = "UPDATE linamortizacion SET idasiento = NULL WHERE idlinamortizacion = " + idlinamortizacion;
        m_companyact->begin();
        m_companyact->ejecuta(query);
        m_companyact->commit();
        table1->setText(row,COL_IDASIENTO, "");
        table1->setText(row,COL_ORDENASIENTO, "");
    } // end if
    if (opcion == opt4) {
        /// Si se va a borrar el asiento.
        m_companyact->intapuntsempresa()->borraAsiento1();
    } // end if
    if (opcion == opt1) {
        /// Se va a generar el asiento.
        QString fecha = table1->text(row, COL_FECHA);
        fprintf(stderr, "Fecha: %s\n", fecha.toAscii().constData());
        QString cant = table1->text(row, COL_CUOTA);
        fprintf(stderr, "Cuota: %s\n", cant.toAscii().constData());
        /// El asiento debe ser uno nuevo.
        int numasiento = 0;
        aplinteligentesview *nueva = new aplinteligentesview(m_companyact, 0);
        QString cuenta = ctaactivo->text();
        QString cuentaamort = ctaamortizacion->text();
        nueva->inicializa(numasiento);
        nueva->muestraplantilla("amortizacion.xml");
        nueva->setvalores("$cuenta$", cuentaamort);
        nueva->setvalores("$cuentabien$", cuenta);
        nueva->setvalores("$fechaasiento$", table1->text(row, COL_FECHA));
        nueva->setvalores("$cuota$", table1->text(row, COL_CUOTA));
        /// Ponemos la fecha del asiento para evitar escribir.
        nueva->setfechaasiento(table1->text(row, COL_FECHA));
        /// Ponemos los asientos plantilla en modo exclusivo, para poder recuperar
        /// el control en cuanto se haya hecho la inserci&oacute;n del asiento.
        nueva->setmodo(1);
        m_companyact->pWorkspace()->addWindow(nueva);
        nueva->show();
        int numasiento1 = m_companyact->intapuntsempresa()->idasiento().toInt();
        QString ordenasiento;
        QString SQLQuery = "SELECT * FROM asiento where idasiento = " + QString::number(numasiento1);
        m_companyact->begin();
        cursor2 *cur = m_companyact->cargacursor(SQLQuery);
        m_companyact->commit();
        if (!cur->eof()) {
            ordenasiento = cur->valor("ordenasiento");
        } // end if
        table1->setText(row, COL_IDASIENTO,QString::number(numasiento1));
        table1->setText(row, COL_ORDENASIENTO, ordenasiento);
        delete cur;
        /// Debemos guardar la modificaci&oacute;n en la l&iacute;nea de amortizaci&oacute;n.
        QString idlinamortizacion = table1->text(row, COL_IDLINAMORTIZACION);
        SQLQuery = "UPDATE linamortizacion set idasiento = " + QString::number(numasiento1) + " WHERE idlinamortizacion = " + idlinamortizacion;
        m_companyact->begin();
        m_companyact->ejecuta(SQLQuery);
        m_companyact->commit();
    } // end if
    col = 0;
    _depura("END amortizacionview::contextMenuRequested", 0);
}


void amortizacionview::trataModificado() {
    _depura("amortizacionview::trataModificado", 0);
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar amortizacion"),
                                 tr("Desea guardar los cambios?"),
                                 QMessageBox::Ok ,
                                 QMessageBox::Cancel ) == QMessageBox::Ok)
            s_saveAmortizacion();
    } // end if
    _depura("END amortizacionview::trataModificado", 0);
}

