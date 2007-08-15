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
#include <QFileDialog>
#include <QPixmap>
#include <QLabel>
#include <QDialog>
#include <QCheckBox>

#include "bancoview.h"
#include "company.h"
#include "funcaux.h"


/// Constructor de la clase inicializa la clase y llama a la clase de pintar para que pinte.
BancoView::BancoView(Company *emp, QWidget *parent)
        : FichaBf(emp, parent) {
    _depura("BancoView::BancoView", 0);
    /// Indicamos que vamos a operar con la tabla banco para que los permisos se traten adecuadamente.
    setDBTableName("banco");
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    setModoEdicion();
    m_cursorbancos = NULL;
    m_item = NULL;
    pintar();
    meteWindow(windowTitle(), this);
    _depura("END BancoView::BancoView", 0);
}


/// Carga el query de la base de datos y carga el qlistview.
void BancoView::pintar() {
    mui_lista->clear();
    if (m_cursorbancos != NULL) {
        delete m_cursorbancos;
    } // end if
    m_cursorbancos = empresaBase()->cargacursor("SELECT * FROM banco ORDER BY nombanco");
    while (!m_cursorbancos->eof()) {
        new QListWidgetItem(m_cursorbancos->valor("nombanco") , mui_lista);
        m_cursorbancos->siguienteregistro();
    } // end while

    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
}


BancoView::~BancoView() {
    _depura("BancoView::~BancoView", 0);
    if (m_cursorbancos != NULL) {
        delete m_cursorbancos;
    } // end if
    _depura("END BancoView::~BancoView", 0);
}


void BancoView::on_mui_lista_currentItemChanged(QListWidgetItem *cur, QListWidgetItem *) {
    _depura( "on_mui_lista_currentItemChanged", 0);
    int row = mui_lista->row(cur);
    trataModificado();
    mui_nombanco->setText(m_cursorbancos->valor("nombanco", row));
    mdb_idbanco = m_cursorbancos->valor("idbanco", row);
    mui_dirbanco->setText(m_cursorbancos->valor("dirbanco", row));
    mui_poblbanco->setText(m_cursorbancos->valor("poblbanco", row));
    mui_cpbanco->setText(m_cursorbancos->valor("cpbanco", row));
    mui_telbanco->setText(m_cursorbancos->valor("telbanco", row));
    mui_faxbanco->setText(m_cursorbancos->valor("faxbanco", row));
    mui_emailbanco->setText(m_cursorbancos->valor("emailbanco", row));
    mui_contactobanco->setText(m_cursorbancos->valor("contactobanco", row));
    mui_codentidadbanco->setText(m_cursorbancos->valor("codentidadbanco", row));
    mui_codagenciabanco->setText(m_cursorbancos->valor("codagenciabanco", row));
    mui_numcuentabanco->setText(m_cursorbancos->valor("numcuentabanco", row));
    mui_dcbanco->setText(m_cursorbancos->valor("dcbanco", row));
    mui_comentbanco->setText(m_cursorbancos->valor("comentbanco", row));
    mui_webbanco->setText(m_cursorbancos->valor("webbanco", row));
    m_item = cur;
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    _depura( "END on_mui_lista_currentItemChanged", 0);
}


void BancoView::on_mui_guardar_clicked() {
    _depura("BancoView::on_mui_guardar_clicked", 0);

    QString query = "UPDATE banco SET ";
    query += "nombanco='" + empresaBase()->sanearCadena(mui_nombanco->text()) + "'";
    query += ", dirbanco='" + empresaBase()->sanearCadena(mui_dirbanco->text()) + "'";
    query += ", poblbanco='" + empresaBase()->sanearCadena(mui_poblbanco->text()) + "'";
    query += ", cpbanco='" + empresaBase()->sanearCadena(mui_cpbanco->text()) + "'";
    query += ", telbanco='" + empresaBase()->sanearCadena(mui_telbanco->text()) + "'";
    query += ", faxbanco='" + empresaBase()->sanearCadena(mui_faxbanco->text()) + "'";
    query += ", emailbanco='" + empresaBase()->sanearCadena(mui_emailbanco->text()) + "'";
    query += ", contactobanco='" + empresaBase()->sanearCadena(mui_contactobanco->text()) + "'";
    query += ", codentidadbanco='" + empresaBase()->sanearCadena(mui_codentidadbanco->text()) + "'";
    query += ", codagenciabanco='" + empresaBase()->sanearCadena(mui_codagenciabanco->text()) + "'";
    query += ", numcuentabanco='" + empresaBase()->sanearCadena(mui_numcuentabanco->text()) + "'";
    query += ", dcbanco='" + empresaBase()->sanearCadena(mui_dcbanco->text()) + "'";
    query += ", comentbanco='" + empresaBase()->sanearCadena(mui_comentbanco->toPlainText()) + "'";
    query += ",  webbanco='" + empresaBase()->sanearCadena(mui_webbanco->text()) + "'";
    query += " WHERE idbanco=" + empresaBase()->sanearCadena(mdb_idbanco);

    int error = empresaBase()->ejecuta(query);
    if (error) {
        empresaBase()->rollback();
        return;
    } // end if
    if (m_cursorbancos != NULL) {
        delete m_cursorbancos;
    } // end if

    m_cursorbancos = empresaBase()->cargacursor("SELECT * FROM banco ORDER BY nombanco");

    if (m_item) {
        m_item->setText( mui_nombanco->text());
    } // end if
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    _depura("END BancoView::on_mui_guardar_clicked", 0);
}


bool BancoView::trataModificado() {
    _depura( "BancoView::trataModificado", 0);
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar datos del trabajador"),
                                 tr("Desea guardar los cambios?"),
                                 tr("&Si"), tr("&No"), 0, 0, 1 ) == 0)
            on_mui_guardar_clicked();
        return (TRUE);
    } // end if
    _depura("END BancoView::trataModificado", 0);
    return (FALSE);
}


/// SLOT que responde a la pulsacion del boton de nuevo tipo de iva.
/// Inserta en la tabla de ivas
void BancoView::on_mui_nuevo_clicked() {
    _depura("BancoView::on_mui_nuevo_clicked", 0);
    /// Si se ha modificado el contenido advertimos y guardamos.
    trataModificado();
    QString query = "INSERT INTO banco (nombanco) VALUES ('NUEVO BANCO')";
    empresaBase()->begin();
    int error = empresaBase()->ejecuta(query);
    if (error) {
        empresaBase()->rollback();
        return;
    } // end if
    cursor2 *cur = empresaBase()->cargacursor("SELECT max(idbanco) AS idbanco FROM banco");
    empresaBase()->commit();
    mdb_idbanco = cur->valor("idbanco");
    delete cur;
    pintar();
    _depura("END BancoView::on_mui_nuevo_clicked", 0);
}


/// SLOT que responde a la pulsacion del boton de borrar la familia que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
void BancoView::on_mui_borrar_clicked() {
    _depura("BancoView::on_mui_borrar_clicked", 0);
    if (mdb_idbanco == "") return;
    trataModificado();
    empresaBase()->begin();
    QString query = "DELETE FROM banco WHERE idbanco = " + mdb_idbanco;
    int error = empresaBase()->ejecuta(query);
    if (error) {
        empresaBase()->rollback();
        return;
    } // end if
    empresaBase()->commit();
    pintar();
    _depura("END BancoView::on_mui_borrar_clicked", 0);
}

void BancoView::imprimir() {
        _depura("BancoView::imprimir", 0);
}


