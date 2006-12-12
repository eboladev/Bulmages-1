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


AmortizacionSubForm::AmortizacionSubForm(QWidget *parent) : SubForm2Bc(parent) {}


AmortizacionSubForm::~AmortizacionSubForm() {}
;


/// Constructor de la clase
AmortizacionView::AmortizacionView(empresa *emp, QWidget *parent)
        : Ficha(parent), DBRecord(emp) {
    _depura("AmortizacionView::AmortizacionView", 0);
    this->setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    m_companyact = emp;
    m_idamortizacion = "";
    m_idctaactivo = "";
    m_idctaamortizacion = "";

    /// Nueva inicializacion de amortizaciones
    mui_listcuotas->setcompany(emp);
    ctaactivo->setempresa(emp);
    ctaamortizacion->setempresa(emp);

    /// Buscamos cual es el asiento inteligente que realiza la amortizaci&oacute;n.
    QString query = "SELECT * FROM ainteligente, configuracion WHERE descripcion = valor AND configuracion.nombre = 'Amortizacion'";
    cursor2 *cur = m_companyact->cargacursor(query);
    if (!cur->eof()) {
        m_idainteligente = cur->valor("m_idainteligente");
    } // end if
    delete cur;

    /// Inicializamos la clase para la base de datos.
    setDBTableName("amortizacion");
    setDBCampoId("idamortizacion");
    addDBCampo("idamortizacion", DBCampo::DBint, DBCampo::DBPrimaryKey  , tr( "idamortizacion"));
    addDBCampo("idcuentaactivo", DBCampo::DBint, DBCampo::DBNothing, tr( "Identificador"));
    addDBCampo("idcuentaamortizacion", DBCampo::DBint, DBCampo::DBNotNull, tr("idcuentaamortizacion"));
    addDBCampo("descamortizacion", DBCampo::DBvarchar, DBCampo::DBNothing, tr( "descamortizacion"));
    addDBCampo("nomamortizacion", DBCampo::DBvarchar, DBCampo::DBNothing, tr( "nomamortizacion"));
    addDBCampo("fechacompra", DBCampo::DBdate, DBCampo::DBNothing, tr( "fechacompra"));
    addDBCampo("fecha1cuota", DBCampo::DBdate, DBCampo::DBNothing, tr( "fecha1cuota"));
    addDBCampo("valorcompra", DBCampo::DBnumeric, DBCampo::DBNothing, tr( "valorcompra"));
    addDBCampo("periodicidad", DBCampo::DBint, DBCampo::DBNothing, tr( "periodicidad"));
    addDBCampo("numcuotas", DBCampo::DBint, DBCampo::DBNothing, tr( "numcuotas"));
    addDBCampo("metodo", DBCampo::DBvarchar, DBCampo::DBNoSave, tr( "metodo"));
    addDBCampo("nifproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr( "nifproveedor"));
    addDBCampo("nomproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr( "nomproveedor"));
    addDBCampo("dirproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr( "dirproveedor"));
    addDBCampo("telproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr( "telproveedor"));
    addDBCampo("agrupacion", DBCampo::DBvarchar, DBCampo::DBNothing, tr( "agrupacion"));

    /// Inicializamos el listado.
    mui_listcuotas->setDBTableName("linamortizacion");
    mui_listcuotas->setDBCampoId("idlinamortizacion");
    mui_listcuotas->addSHeader("idlinamortizacion", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoWrite , tr("idlinamortizacion"));
    mui_listcuotas->addSHeader("idamortizacion", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoWrite , tr("idamortizacion"));
    mui_listcuotas->addSHeader("idasiento", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNoWrite , tr("idasiento"));
    mui_listcuotas->addSHeader("ejercicio", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNone, tr("ejercicio"));
    mui_listcuotas->addSHeader("fechaprevista", DBCampo::DBdate, DBCampo::DBNothing, SHeader::DBNone , tr("fechaprevista"));
    mui_listcuotas->addSHeader("cantidad", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("cantidad"));
    mui_listcuotas->setinsercion(FALSE);

    /// Fin de nuevas amortizaciones
    m_companyact->meteWindow(windowTitle(), this);
    _depura("END AmortizacionView::AmortizacionView", 0);
}


int AmortizacionView::borrar() {
    _depura("AmortizacionView::borrar", 0);
    if (m_idamortizacion != "") {
        QString query = "DELETE FROM linamortizacion WHERE idamortizacion = " + m_idamortizacion;
        m_companyact->ejecuta(query);
        query = "DELETE FROM amortizacion WHERE idamortizacion = " + m_idamortizacion;
        m_companyact->ejecuta(query);
        close();
    } // end if
    _depura("END AmortizacionView::borrar", 0);
    return 0;
}


int AmortizacionView::guardar() {
    _depura("AmortizacionView::guardar", 0);
    try {
        /// Guardamos los datos del formulario
        setDBvalue("nomamortizacion", nomamortizacion->text() );
        setDBvalue("idcuentaactivo",  ctaactivo->idcuenta() );
        setDBvalue("valorcompra", valorcompra->text() );
        setDBvalue("numcuotas", numcuotas->text());
        setDBvalue("fechacompra", fechacompra->text());
        setDBvalue("fecha1cuota", fecha1cuota->text());
        setDBvalue("idcuentaamortizacion", ctaamortizacion->idcuenta());
        setDBvalue("agrupacion", agrupacion->text());

        QString id = "";
        DBRecord::DBsave(id);

        /// Guardamos las lineas de amortizacion.
        mui_listcuotas->setColumnValue("idamortizacion", id);
        mui_listcuotas->guardar();
        dialogChanges_cargaInicial();
        _depura("END AmortizacionView::guardar", 0);
        return 0;
    } catch (...) {
        mensajeInfo("Error en el guardado");
        return -1;
    } // end try
}


void AmortizacionView::on_mui_guardar_clicked() {
    guardar();
}


AmortizacionView::~AmortizacionView() {
    _depura("AmortizacionView::~AmortizacionView", 0);
    m_companyact->sacaWindow( this);
    _depura("END AmortizacionView::~AmortizacionView", 0);
}


/// Esta funci&oacute;n inicializa la clase, y adem&aacute;s hace la presentaci&oacuet;n
/// de una determinada amortizaci&oacute;n.
int AmortizacionView::cargar(QString idamortizacion) {
    _depura("AmortizacionView::cargar", 0, idamortizacion);
    try {
        m_idamortizacion = idamortizacion;
        DBRecord::cargar(m_idamortizacion);

        /// se ha cargado de la base de datos.
        nomamortizacion->setText(DBvalue("nomamortizacion"));
        valorcompra->setText(DBvalue("valorcompra"));
        numcuotas->setText(DBvalue("numcuotas"));
        fechacompra->setText(DBvalue( "fechacompra"));
        fecha1cuota->setText(DBvalue("fecha1cuota"));
        ctaactivo->setidcuenta(DBvalue("idcuentaactivo"));
        m_idctaactivo = DBvalue("idcuentaactivo");
        ctaamortizacion->setidcuenta(DBvalue("idcuentaamortizacion"));
        m_idctaamortizacion= DBvalue("idcuentaamortizacion");
        agrupacion->setText(DBvalue("agrupacion"));

        QString query = "SELECT *, fechaprevista <= now() AS ant FROM linamortizacion LEFT JOIN asiento ON linamortizacion.idasiento = asiento.idasiento WHERE idamortizacion = " + m_idamortizacion + " ORDER BY fechaprevista";
        cursor2 *curs = m_companyact->cargacursor(query);
        mui_listcuotas->cargar(curs);
        delete curs;

        /// Calculamos lo que ya llevamos amortizado y lo presentamos en la pantalla.
        query = "SELECT sum(cantidad) AS amortizado FROM linamortizacion WHERE idasiento IS NOT NULL AND idamortizacion = " + m_idamortizacion;
        curs = m_companyact->cargacursor(query, "amortizado");
        if (!curs->eof()) {
            amortizado->setText(curs->valor("amortizado"));
        } // end if
        delete curs;

        /// Calculamos lo que nos falta por amortizar y lo presentamos en la pantalla.
        query = "SELECT sum(cantidad) AS pdte FROM linamortizacion WHERE idasiento IS NULL AND idamortizacion = " + m_idamortizacion;
        curs = m_companyact->cargacursor(query, "pdte");
        if (!curs->eof()) {
            pendiente->setText(curs->valor("pdte"));
        } // end if
        delete curs;

        /// Deshabilitamos el bot&oacute;n de calcular, porque la amortizaci&oacute;n ya
        /// est&aacute; hecha.
        mui_btcalcular->setDisabled(TRUE);
        dialogChanges_cargaInicial();
        m_companyact->meteWindow(windowTitle(), this);
        _depura("END AmortizacionView::cargar", 0);
        return 0;
    } catch (...) {
        mensajeInfo(tr("Error en la carga de la amortizacion"));
        return -1;
    } // end try
}


void AmortizacionView::on_mui_btcalcular_clicked() {
    _depura("AmortizacionView::calculaamortizacion", 0);
    /// Para hacer el c&iacute;rculo de los plazos de cada amortizaci&oacute;n
    /// Hay que obtener diversos datos.
    QDate f1cuota = normalizafecha(fecha1cuota->text());
    int ncuotas = numcuotas->text().toInt();
    QString periodicidadtxt = periodicidad->currentText();
    double valcuota;
    QString valcuotastr;

    mui_listcuotas->inicializar();
    mui_listcuotas->pintar();
    /// Si usamos el m&eacute;todo lineal:
    if (metodolineal->isChecked()) {
        valcuota = valorcompra->text().toDouble() / ncuotas;
        valcuotastr.sprintf("%10.2f", valcuota);
        for (int i = 0; i < ncuotas; i++) {
            mui_listcuotas->setinsercion(TRUE);
            mui_listcuotas->nuevoRegistro();
            mui_listcuotas->setinsercion(FALSE);
            SDBRecord *rec = mui_listcuotas->lista()->last();
            rec->setDBvalue("fechaprevista", f1cuota.toString("dd/MM/yyyy"));
            rec->setDBvalue("cantidad", valcuotastr);

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
            mui_listcuotas->setinsercion(TRUE);
            mui_listcuotas->nuevoRegistro();
            mui_listcuotas->setinsercion(FALSE);
            SDBRecord *rec = mui_listcuotas->lista()->last();
            rec->setDBvalue("fechaprevista", f1cuota.toString("dd/MM/yyyy"));
            rec->setDBvalue("cantidad", valcuotastr);
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
            mui_listcuotas->setinsercion(TRUE);
            mui_listcuotas->nuevoRegistro();
            mui_listcuotas->setinsercion(FALSE);
            SDBRecord *rec = mui_listcuotas->lista()->last();
            rec->setDBvalue("fechaprevista", f1cuota.toString("dd/MM/yyyy"));
            rec->setDBvalue("cantidad", valcuotastr);

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
                valcuota = (valorcompra->text().toDouble() - total) * porcent;
                total += valcuota;
                fprintf(stderr, "cuota: %10.2f -- total: %10.2f\n", valcuota, total);
            } else {
                valcuota = valorcompra->text().toDouble() - total;
            } // end if
            valcuotastr.sprintf("%10.2f", valcuota);
            mui_listcuotas->setinsercion(TRUE);
            mui_listcuotas->nuevoRegistro();
            mui_listcuotas->setinsercion(FALSE);
            SDBRecord *rec = mui_listcuotas->lista()->last();
            rec->setDBvalue("fechaprevista", f1cuota.toString("dd/MM/yyyy"));
            rec->setDBvalue("cantidad", valcuotastr);
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
    _depura("AmortizacionView::calculaamortizacion", 0);
}


/// SLOT que captura el men&uacute; contextual sobre la tabla de amortizaci&oacute;n.
/** Datos para el asiento de amortizaci&oacute;n.
    El asiento inteligente debe llamarse como el par&aacute;metro Amortizaci&oacute;n de confpr.
    El asiento inteligente debe tener las variables que se llamen:
    $cuenta$ Cuenta de Amortizaci&oacute;n.
    $cuentabien$ Cuenta del Bien.
    $cuota$ Cuota a pagar. */
void AmortizacionView::contextMenuRequested(int row, int col, const QPoint &poin) {
    _depura("AmortizacionView::contextMenuRequested", 0);
    /*
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
     
        if (mui_listcuotas->text(row, COL_IDASIENTO) == "") {
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
            mui_listcuotas->insertRows(row + 1, 1);
        } // end if
        /// Eliminar una cuota.
        if (opcion == opt6) {
            QString idlinamortizacion = mui_listcuotas->text(row, COL_IDLINAMORTIZACION);
            QString query = "DELETE FROM linamortizacion WHERE idlinamortizacion = " + idlinamortizacion;
            if(idlinamortizacion != "") {
                m_companyact->begin();
                m_companyact->ejecuta(query);
                m_companyact->commit();
            } // end if
            mui_listcuotas->removeRow(row);
        } // end if
        if (opcion == opt2 || opcion == opt4) {
            /// Si se va a mostrar el asiento, o se va a borrar.
            QString idasiento = mui_listcuotas->text(row, COL_IDASIENTO);
            m_companyact->intapuntsempresa()->muestraasiento(idasiento.toInt());
        } // end if
        if (opcion == opt3 || opcion == opt4) {
            /// Si se va a desvincular el asiento o se va a borrar.
            QString idasiento = mui_listcuotas->text(row, COL_IDASIENTO);
            QString idlinamortizacion = mui_listcuotas->text(row, COL_IDLINAMORTIZACION);
            QString query = "UPDATE linamortizacion SET idasiento = NULL WHERE idlinamortizacion = " + idlinamortizacion;
            m_companyact->begin();
            m_companyact->ejecuta(query);
            m_companyact->commit();
            mui_listcuotas->setText(row,COL_IDASIENTO, "");
            mui_listcuotas->setText(row,COL_ORDENASIENTO, "");
        } // end if
        if (opcion == opt4) {
            /// Si se va a borrar el asiento.
            m_companyact->intapuntsempresa()->borraAsiento1();
        } // end if
        if (opcion == opt1) {
            /// Se va a generar el asiento.
            QString fecha = mui_listcuotas->text(row, COL_FECHA);
            fprintf(stderr, "Fecha: %s\n", fecha.toAscii().constData());
            QString cant = mui_listcuotas->text(row, COL_CUOTA);
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
            nueva->setvalores("$fechaasiento$", mui_listcuotas->text(row, COL_FECHA));
            nueva->setvalores("$cuota$", mui_listcuotas->text(row, COL_CUOTA));
            /// Ponemos la fecha del asiento para evitar escribir.
            nueva->setfechaasiento(mui_listcuotas->text(row, COL_FECHA));
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
            mui_listcuotas->setText(row, COL_IDASIENTO,QString::number(numasiento1));
            mui_listcuotas->setText(row, COL_ORDENASIENTO, ordenasiento);
            delete cur;
            /// Debemos guardar la modificaci&oacute;n en la l&iacute;nea de amortizaci&oacute;n.
            QString idlinamortizacion = mui_listcuotas->text(row, COL_IDLINAMORTIZACION);
            SQLQuery = "UPDATE linamortizacion set idasiento = " + QString::number(numasiento1) + " WHERE idlinamortizacion = " + idlinamortizacion;
            m_companyact->begin();
            m_companyact->ejecuta(SQLQuery);
            m_companyact->commit();
        } // end if
        col = 0;
    */
    _depura("END AmortizacionView::contextMenuRequested", 0);
}


/*
void AmortizacionView::trataModificado() {
    _depura("AmortizacionView::trataModificado", 0);
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar amortizacion"),
                                 tr("Desea guardar los cambios?"),
                                 QMessageBox::Ok ,
                                 QMessageBox::Cancel ) == QMessageBox::Ok)
            on_mui_guardar_clicked();
    } // end if
    _depura("END AmortizacionView::trataModificado", 0);
}
*/
