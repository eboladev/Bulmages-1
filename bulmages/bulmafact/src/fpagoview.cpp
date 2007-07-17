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

#include <QLineEdit>
#include <QMessageBox>
#include <QCloseEvent>

#include "fpagoview.h"
#include "company.h"
#include "funcaux.h"


/// Constructor de la clase inicializa la clase y llama a la clase de pintar para que pinte.
FPagoView::FPagoView(company *emp,QWidget *parent)
        : FichaBf(emp, parent) {
    _depura("FPagoView::FPagoView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    setModoEdicion();
    m_cursorFPagoView = NULL;
    m_item = NULL;
    empresaBase()->meteWindow(windowTitle(), this);
    pintar();
    _depura("END FPagoView::FPagoView", 0);
}


/// Carga el query de la base de datos y carga el qlistview.
void FPagoView::pintar() {
    mui_lista->clear();
    if (m_cursorFPagoView != NULL)
        delete m_cursorFPagoView;
    m_cursorFPagoView = empresaBase()->cargacursor("SELECT * FROM forma_pago ORDER BY idforma_pago");
    mui_lista->clear();
    while (!m_cursorFPagoView->eof()) {
        new QListWidgetItem(m_cursorFPagoView->valor("descforma_pago"), mui_lista);
        m_cursorFPagoView->siguienteregistro();
    } // end while
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
}


FPagoView::~FPagoView() {
    _depura("FPagoView::~FPagoView", 0);
    if (m_cursorFPagoView != NULL)
        delete m_cursorFPagoView;
    _depura("END FPagoView::~FPagoView", 0);

}


int FPagoView::sacaWindow() {
    empresaBase()->sacaWindow(this);
    return 0;
}


void FPagoView::on_mui_lista_currentItemChanged(QListWidgetItem *cur, QListWidgetItem *) {
    _depura("on_mui_lista_currentItemChanged", 0);
    int row = mui_lista->row(cur);
    trataModificado();
    m_descforma_pago->setText(m_cursorFPagoView->valor("descforma_pago", row));
    m_dias1tforma_pago->setText(m_cursorFPagoView->valor("dias1tforma_pago", row));
    m_descuentoforma_pago->setText(m_cursorFPagoView->valor("descuentoforma_pago", row));
    mdb_idforma_pago = m_cursorFPagoView->valor("idforma_pago", row);
    m_item = cur;

    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
}


void FPagoView::on_mui_guardar_clicked() {
    try {
        QString query = "UPDATE forma_pago SET descforma_pago = '" +
                        empresaBase()->sanearCadena(m_descforma_pago->text()) + "', dias1tforma_pago= " +
                        empresaBase()->sanearCadena(m_dias1tforma_pago->text()) + " , descuentoforma_pago = "+
                        empresaBase()->sanearCadena(m_descuentoforma_pago->text()) + " WHERE idforma_pago =" + mdb_idforma_pago;
        empresaBase()->ejecuta(query);
        if (m_cursorFPagoView != NULL) {
            delete m_cursorFPagoView;
        } // end if
        m_cursorFPagoView = empresaBase()->cargacursor("SELECT * FROM forma_pago ORDER BY idforma_pago");
        if (m_item) {
            m_item->setText(m_descforma_pago->text());
        } // end if
        dialogChanges_cargaInicial();
    } catch(...) {
        _depura ("error guardando la forma de pago", 1);
        return;
    } // end try
}


bool FPagoView::trataModificado() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar forma de pago"),
                                 tr("Desea guardar los cambios."),
                                 QMessageBox::Ok,
                                 QMessageBox::Cancel ) == QMessageBox::Ok)
            on_mui_guardar_clicked();
        return (TRUE);
    } // end if
    return(FALSE);
}


/// SLOT que responde a la pulsación del botón de nuevo tipo de iva
/// Inserta en la tabla de IVAs.
void FPagoView::on_mui_crear_clicked() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    trataModificado();
    QString query = "INSERT INTO forma_pago (descforma_pago, dias1tforma_pago, descuentoforma_pago) VALUES ('NUEVA FORMA DE PAGO', 0, 0)";
    empresaBase()->begin();
    int error = empresaBase()->ejecuta(query);
    if (error) {
        empresaBase()->rollback();
        return;
    } // end if
    cursor2 *cur = empresaBase()->cargacursor("SELECT max(idforma_pago) AS idFPagoView FROM forma_pago");
    empresaBase()->commit();
    mdb_idforma_pago = cur->valor("idFPagoView");
    delete cur;
    pintar();
}


/// SLOT que responde a la pulsacion del boton de borrar la familia que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
void FPagoView::on_mui_borrar_clicked() {
    trataModificado();
    empresaBase()->begin();
    QString query = "DELETE FROM forma_pago WHERE idforma_pago = " + mdb_idforma_pago;
    int error = empresaBase()->ejecuta(query);
    if (error) {
        empresaBase()->rollback();
        return;
    } // end if
    empresaBase()->commit();
    pintar();
}


void FPagoView::setModoConsulta() {
    m_modoConsulta = TRUE;
}


void FPagoView::setModoEdicion() {
    m_modoConsulta = FALSE;
}

