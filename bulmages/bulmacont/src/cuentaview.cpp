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

#include "cuentaview.h"
#include "empresa.h"


CuentaView::CuentaView(empresa *emp, QWidget *parent, Qt::WFlags fl)
        : FichaBc(emp, parent, fl) {
    _depura("CuentaView::CuentaView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    idcuenta = 0;
    numdigitos = emp->numdigitosempresa();
    inicializa();
    dialogChanges_cargaInicial();
    empresaBase()->meteWindow(windowTitle(), this);
    _depura("END CuentaView::CuentaView", 0);
}


/// Saca el formulario, crea una cuenta y devuelve su identificador.
void CuentaView::cuentanueva(QString cod) {
    _depura("CuentaView::cuentanueva", 0);
    codigo->setText(cod);
    /// Vamos a buscar el posible padre de una cuenta y lo pondremos como toca.
    QString cpadre = empresaBase()->searchParent(cod);
    codigopadre->setText(cpadre);
    /// Tambi&eacute;n buscaremos el grupo y lo pondremos como toca.
    QString query = "SELECT * FROM cuenta WHERE codigo = '" + cpadre + "'";
    cursor2 *cur = empresaBase()->cargacursor(query);

    /// Y tambi&eacute;n buscaremos los tipos y los usaremos.
    if (!cur->eof()) {
        descripcion->setText(cur->valor("descripcion"));
        debe->setText("0");
        haber->setText("0");
        if (cur->valor("bloqueada") != "f") {
            bloqueada->setChecked(true);
        } else {
            bloqueada->setChecked(false);
        } // end if
        if (cur->valor("imputacion") != "f") {
            imputacion->setChecked(true);
        } else {
            imputacion->setChecked(false);
        } // end if
        if (cur->valor("nodebe") != "f") {
            nodebe->setChecked(true);
        } else {
            nodebe->setChecked(false);
        } // end if
        if (cur->valor("nohaber") != "f") {
            nohaber->setChecked(true);
        } else {
            nohaber->setChecked(false);
        } // end if
        if (cur->valor("regularizacion") != "f") {
            regularizacion->setChecked(true);
        } else {
            regularizacion->setChecked(false);
        } // end if
        /// Vamos a hacer la carga del tipocuenta.
        int tipocuenta = atoi(cur->valor("tipocuenta").toAscii());
        switch (tipocuenta) {
        case 0:
            cuentasintipo->setChecked(TRUE);
            break;
        case 1:
            cuentaactivo->setChecked(TRUE);
            break;
        case 2:
            cuentapasivo->setChecked(TRUE);
            break;
        case 3:
            cuentaneto->setChecked(TRUE);
            break;
        case 4:
            cuentaingreso->setChecked(TRUE);
            break;
        case 5:
            cuentagasto->setChecked(TRUE);
            break;
        } // end switch
        /// Vamos a hacer la carga del grupo.
        int idgrupo = atoi(cur->valor(6).toAscii());
        int i = 0;
        while (idgrupos[i] != idgrupo && i < 100) {
            i++;
        } // end while
        combogrupos->setCurrentIndex(i);
        delete cur;
    } // end if
    _depura("END CuentaView::cuentanueva", 0);
}


CuentaView::~CuentaView() {
    _depura("CuentaView::~CuentaView", 0);
    _depura("CuentaView::~CuentaView", 0);
}


void CuentaView::on_mui_borrar_clicked() {
    borrarCuenta();
}


void CuentaView::on_mui_guardar_clicked() {
    guardarCuenta();
}


void CuentaView::on_mui_aceptar_clicked() {
    guardarCuenta();
    close();
}


/// Esta funci&oacute;n se activa cada vez que se pulsa una tecla sobre la cuenta.
void CuentaView::cambiapadre(const QString &cadena) {
    _depura("CuentaView::cambiapadre", 0);
    QString cad;
    cursor2 *cursoraux;
    cad = codigopadre->text();
    unsigned int i = cad.indexOf(".", 0);
    if (i+1 < (unsigned int) cad.length()) {
        if (i != 0) {
            cad.replace(i, 1, "%");
        } // end if
        empresaBase()->begin();
        cursoraux = empresaBase()->cargacuenta(0, cad);
        empresaBase()->commit();
        if (!cursoraux->eof()) {
            codigopadre->setText(cursoraux->valor(1));
        } // end if
        delete cursoraux;
    } // end if
    /// Para quitar el warning.
    cadena.isNull();
    _depura("END CuentaView::cambiapadre", 0);
}


/// Este es el SLOT que se activa al pulsar el bot&oacute;n ok del formulario.
/** Lo que hace es recoger los datos del formulario y hacer una inserci&oacute;n
    o una modificaci&oacute;n de la tabla de cuentas. */
void CuentaView::closeEvent(QCloseEvent *e) {
    _depura("CuentaView::closeEvent", 0);
    if (dialogChanges_hayCambios()) {
        int val = QMessageBox::warning(this,
                                       tr("Guardar cuenta"),
                                       tr("Desea guardar los cambios?"),
                                       tr("&Si"), tr("&No"), tr("&Cancelar"), 0, 2);
        if (val == 0)
            try {
                guardarCuenta();
            } catch (...) {
                e->ignore();
            } // end try
        if (val == 2)
            e->ignore();
    } // end if
    _depura("END CuentaView::closeEvent", 0);
}


int CuentaView::inicializa() {
    _depura("CuentaView::inicializa", 0);
    cursor2 *cursorgrupos;
    for (int i = 0; i < 100; i++) {
        idgrupos[i] = -1;
    } // end for
    empresaBase()->begin();
    cursorgrupos = empresaBase()->cargagrupos();
    empresaBase()->commit();
    while (!cursorgrupos->eof()) {
        combogrupos->addItem(cursorgrupos->valor("descripcion"));
        idgrupos[combogrupos->count() - 1] = cursorgrupos->valor("idgrupo").toInt();
        cursorgrupos->siguienteregistro();
    } // end while
    delete cursorgrupos;
    return 0;
    _depura("END CuentaView::inicializa", 0);
}


int CuentaView::cargacuenta(int idcuenta1) {
    _depura("CuentaView::cargacuenta", 0);
    QString cadena;
    int cpadre;
    cursor2 *cursorcuenta, *cursorpadre;
    idcuenta = idcuenta1;
    empresaBase()->begin();
    cursorcuenta = empresaBase()->cargacuenta(idcuenta1);
    empresaBase()->commit();
    codigo->setText(cursorcuenta->valor("codigo"));

    /// Cambiamos el t&iacute;tulo de la ventana con el c&oacute;digo.
    setWindowTitle(tr("Cuenta") + " " + codigo->text());
    empresaBase()->meteWindow(windowTitle(), this);

    descripcion->setText(cursorcuenta->valor("descripcion"));
    debe->setText(cursorcuenta->valor("debe"));
    haber->setText(cursorcuenta->valor("haber"));
    if (cursorcuenta->valor("bloqueada") != "f") {
        bloqueada->setChecked(true);
    } else {
        bloqueada->setChecked(false);
    } // end if
    if (cursorcuenta->valor("imputacion") != "f") {
        imputacion->setChecked(true);
    } else {
        imputacion->setChecked(false);
    } // end if
    if (cursorcuenta->valor("nodebe") != "f") {
        nodebe->setChecked(true);
    } else {
        nodebe->setChecked(false);
    } // end if
    if (cursorcuenta->valor("nohaber") != "f") {
        nohaber->setChecked(true);
    } else {
        nohaber->setChecked(false);
    } // end if
    if (cursorcuenta->valor("regularizacion") != "f") {
        regularizacion->setChecked(true);
    } else {
        regularizacion->setChecked(false);
    } // end if
    /// Vamos a hacer la carga del tipocuenta.
    int tipocuenta = cursorcuenta->valor("tipocuenta").toInt();
    switch (tipocuenta) {
    case 0:
        cuentasintipo->setChecked(TRUE);
        break;
    case 1:
        cuentaactivo->setChecked(TRUE);
        break;
    case 2:
        cuentapasivo->setChecked(TRUE);
        break;
    case 3:
        cuentaneto->setChecked(TRUE);
        break;
    case 4:
        cuentaingreso->setChecked(TRUE);
        break;
    case 5:
        cuentagasto->setChecked(TRUE);
        break;
    } /// end switch
    /// Vamos a hacer la carga del grupo.
    int idgrupo = atoi(cursorcuenta->valor(6).toAscii());
    int i = 0;
    while (idgrupos[i] != idgrupo && i < 100)
        i++;
    combogrupos->setCurrentIndex(i);
    /// Vamos a coger el c&oacute;digo del padre de la cuenta para que tambi&eacute;n
    /// aparezca en el formulario.
    cpadre = atoi(cursorcuenta->valor(4).toAscii());
    if (cpadre != 0) {
        empresaBase()->begin();
        cursorpadre = empresaBase()->cargacuenta(cpadre);
        empresaBase()->commit();
        cadena = cursorpadre->valor(1);
        codigopadre->setText(cadena);
        codigopadre->setReadOnly(TRUE);
        delete cursorpadre;
    } /// end if
    /// Vamos a hacer las inserciones pertinentes para los campos de la entidad.
    nombreent->setText(cursorcuenta->valor("nombreent_cuenta"));
    cif->setText(cursorcuenta->valor("cifent_cuenta"));
    direccion->setText(cursorcuenta->valor("dirent_cuenta"));
    cp->setText(cursorcuenta->valor("cpent_cuenta"));
    telf->setText(cursorcuenta->valor("telent_cuenta"));
    coments->setPlainText(cursorcuenta->valor("coment_cuenta"));
    banco->setText(cursorcuenta->valor("bancoent_cuenta"));
    email->setText(cursorcuenta->valor("emailent_cuenta"));
    web->setText(cursorcuenta->valor("webent_cuenta"));
    delete cursorcuenta;

    /// Estamos probando la nueva forma de almacenar cambios.
    dialogChanges_cargaInicial();

    _depura("END CuentaView::cargacuenta", 0);
    return 0;
}


int CuentaView::nuevacuenta(QString codpadre, int idgrupo) {
    _depura("CuentaView::nuevacuenta", 0);
    /// Suponiendo que las cuentas son num&eacute;ricas, al crear una nueva cuenta
    /// buscamos entre las que ser&aacute;n sus hermanas y le asignamos el n&uacute;mero
    /// siguiente que le corresponda.
    QString cpadreaux;
    QString query;
    query.sprintf("SELECT * FROM cuenta WHERE padre = id_cuenta('%s') ORDER BY codigo DESC", codpadre.toAscii().constData());
    cursor2 *cur = empresaBase()->cargacursor(query);
    if (!cur->eof()) {
        long int valor = cur->valor("codigo").toLong();
        valor ++;
        cpadreaux.setNum(valor);
        codigo->setText(cpadreaux);
        /// Vamos a hacer la carga del tipocuenta.
        int tipocuenta = cur->valor("tipocuenta").toInt();
        switch (tipocuenta) {
        case 0:
            cuentasintipo->setChecked(TRUE);
            break;
        case 1:
            cuentaactivo->setChecked(TRUE);
            break;
        case 2:
            cuentapasivo->setChecked(TRUE);
            break;
        case 3:
            cuentaneto->setChecked(TRUE);
            break;
        case 4:
            cuentaingreso->setChecked(TRUE);
            break;
        case 5:
            cuentagasto->setChecked(TRUE);
            break;
        } // end switch
    } else {
        QString codint = codpadre;
        while ((unsigned int) codint.length() < numdigitos - 1) {
            codint = codint + "0";
        } // end while
        codint = codint+"0";
        codigo->setText(codint);
    }
    delete cur;
    /// Establecemos el valor del padre y del grupo.
    codigopadre->setText(codpadre);
    int i = 0;
    while (idgrupos[i] != idgrupo && i < 100) {
        i++;
    } // end while
    if (idgrupos[i] == idgrupo) {
        combogrupos->setCurrentIndex(i);
    } // end if
    _depura("END CuentaView::nuevacuenta", 0);
    return 0;
}

/// Esta funci&oacute;n se dispara cuando se ha pulsado
/// return sobre el bot&oacute;n del c&oacute;digo.
void CuentaView::codigo_ret() {
    _depura("CuentaView::codigo_ret", 0);
    QString cod = codigo->text();
    cod = extiendecodigo(cod, numdigitos);
    codigo->setText(cod);
    _depura("END CuentaView::codigo_ret", 0);
}


void CuentaView::guardarCuenta() {
    _depura("CuentaView::guardarCuenta", 0);
    QString codigocuenta;
    int idpadre = 0;
    cursor2 *cursoraux;
    /// Comprobamos que el campo codigopadre tiene un numero de cuenta y
    /// calculamos el id correspondiente consultando en la base de datos.
    codigocuenta = codigopadre->text();
    if (codigocuenta != "") {
        empresaBase()->begin();
        cursoraux = empresaBase()->cargacuenta(0, codigocuenta);
        empresaBase()->commit();
        idpadre = atoi(cursoraux->valor("idcuenta").toAscii());
        delete cursoraux;
    } // end if
    /// Recogemos el valor de tipocuenta.
    int tipocuenta = 0;
    if (cuentaactivo->isChecked()) {
        tipocuenta = 1;
    } else if (cuentapasivo->isChecked()) {
        tipocuenta = 2;
    } else if (cuentaneto->isChecked()) {
        tipocuenta = 3;
    } else if (cuentaingreso->isChecked()) {
        tipocuenta = 4;
    } else if (cuentagasto->isChecked()) {
        tipocuenta = 5;
    } else {
        tipocuenta = 0;
    } // end if
    /// Si se trata de una modificaci&oacute;n modificamos y si se trata de
    /// una inserci&oacute;n insertamos :)
    if (idcuenta != 0) {
        empresaBase()->begin();
        empresaBase()->modificacuenta(idcuenta,
                                     empresaBase()->sanearCadena(descripcion->text()),
                                     empresaBase()->sanearCadena(codigo->text()),
                                     imputacion->isChecked(),
                                     bloqueada->isChecked(),
                                     idgrupos[combogrupos->currentIndex()],
                                     TRUE,
                                     empresaBase()->sanearCadena(nombreent->text()),
                                     empresaBase()->sanearCadena(cif->text()),
                                     empresaBase()->sanearCadena(direccion->text()),
                                     empresaBase()->sanearCadena(cp->text()),
                                     empresaBase()->sanearCadena(telf->text()),
                                     empresaBase()->sanearCadena(coments->toPlainText()),
                                     empresaBase()->sanearCadena(banco->text()),
                                     empresaBase()->sanearCadena(email->text()),
                                     empresaBase()->sanearCadena(web->text()),
                                     tipocuenta,
                                     nodebe->isChecked(),
                                     nohaber->isChecked());
        empresaBase()->commit();
    } else {
        empresaBase()->begin();
        ((postgresiface2 *) empresaBase())->nuevacuenta(empresaBase()->sanearCadena(descripcion->text()),
                                  empresaBase()->sanearCadena(codigo->text()),
                                  idpadre,
                                  idgrupos[combogrupos->currentIndex()],
                                  empresaBase()->sanearCadena(nombreent->text()),
                                  empresaBase()->sanearCadena(cif->text()),
                                  empresaBase()->sanearCadena(direccion->text()),
                                  empresaBase()->sanearCadena(cp->text()),
                                  empresaBase()->sanearCadena(telf->text()),
                                  empresaBase()->sanearCadena(coments->toPlainText()),
                                  empresaBase()->sanearCadena(banco->text()),
                                  empresaBase()->sanearCadena(email->text()),
                                  empresaBase()->sanearCadena(web->text()),
                                  tipocuenta,
                                  nodebe->isChecked(),
                                  nohaber->isChecked());
        QString query = "SELECT max(idcuenta) AS id from cuenta";
        cursoraux = empresaBase()->cargacursor(query, "maxidcuenta");
        idcuenta = atoi(cursoraux->valor("id").toAscii());
        empresaBase()->commit();
        delete cursoraux;
    } // end if
    /// Estamos probando la nueva forma de almacenar cambios.
    dialogChanges_cargaInicial();
    _depura("END CuentaView::guardarCuenta", 0);
}


void CuentaView::borrarCuenta() {
    _depura("CuentaView::borrarCuenta", 0);
    switch (QMessageBox::warning(this,
                                 tr("Borrar cuenta"),
                                 tr("Se va a borrar la cuenta,\nEsto puede ocasionar perdida de datos\nTal vez deberia pensarselo mejor antes\nporque igual su trabajo se pierde."),
                                 QMessageBox::Ok, QMessageBox::Cancel)) {
    case QMessageBox::Ok: /// Retry clicked or Enter pressed.
        empresaBase()->ejecuta("DELETE FROM cuenta WHERE idcuenta = " + QString::number(idcuenta));
        close();
        break;
    case QMessageBox::Cancel: /// Abort clicked or Escape pressed.
        break;
    } // end switch
    _depura("CuentaView::borrarCuenta", 0);
}

