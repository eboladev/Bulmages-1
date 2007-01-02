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

#include <QMap>
#include <QLineEdit>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

#include "familiasview.h"
#include "company.h"
#include "funcaux.h"

#define COL_NOMFAMILIA 0
#define COL_CODCOMPLETOFAMILIA 1
#define COL_DESCFAMILIA 2
#define COL_IDFAMILIA  3
#define COL_CODFAMILIA 4


FamiliasView::FamiliasView(company *comp, QWidget *parent, bool modoConsulta)
        : Ficha(parent) {
    _depura("FamiliasView::FamiliasView", 0);
    setupUi(this);
    companyact = comp;

    m_listFamilias->setColumnCount(3);
    QStringList headers;
    headers << tr("Nombre") << tr("Codigo") << tr("Descripcion") << tr("Id familia") << tr("Codigo completo");
    m_listFamilias->setHeaderLabels(headers);

    m_listFamilias->setColumnHidden(COL_IDFAMILIA, TRUE);
    m_listFamilias->setColumnHidden(COL_CODFAMILIA, TRUE);

    m_idfamilia = "";
    if (modoConsulta) {
        setModoConsulta();
        groupBox1->hide();
        mui_detalles->hide();
        mui_crear->hide();
        mui_guardar->hide();
        mui_borrar->hide();
        mui_cancelar->hide();
        mui_aceptar->hide();
        mui_imprimir->hide();
    } else {
        setModoEdicion();
        setAttribute(Qt::WA_DeleteOnClose);
        companyact->meteWindow(windowTitle(), this);
    } // end if
    pintar();
    _depura("END FamiliasView::FamiliasView", 0);
}


FamiliasView::~FamiliasView() {}


int FamiliasView::sacaWindow() {
    companyact->sacaWindow(this);
    return 0;
}


void FamiliasView::pintar() {
    _depura("FamiliasView::pintar", 0);
    QTreeWidgetItem * it;
    QMap <int, QTreeWidgetItem*> Lista1;
    int padre;
    int idfamilia = 0;
    cursor2 *cursoraux1, *cursoraux2;

    /// Vaciamos el arbol.
    while (m_listFamilias->topLevelItemCount() > 0) {
        it = m_listFamilias->takeTopLevelItem(0);
        delete it;
    } // end while

    cursoraux1 = companyact->cargacursor("SELECT * FROM familia WHERE padrefamilia ISNULL ORDER BY idfamilia");
    while (!cursoraux1->eof()) {
        padre = cursoraux1->valor("padrefamilia").toInt();
        idfamilia = cursoraux1->valor("idfamilia").toInt();
        it = new QTreeWidgetItem(m_listFamilias);
        Lista1[idfamilia] = it;
        it->setText(COL_NOMFAMILIA, cursoraux1->valor("nombrefamilia"));
        it->setText(COL_CODFAMILIA, cursoraux1->valor("codigofamilia"));
        it->setText(COL_DESCFAMILIA, cursoraux1->valor("descfamilia"));
        it->setText(COL_IDFAMILIA, cursoraux1->valor("idfamilia"));
        it->setText(COL_CODCOMPLETOFAMILIA, cursoraux1->valor("codigocompletofamilia"));
        m_listFamilias->expandItem(it);
        cursoraux1->siguienteregistro();
    } // end while
    delete cursoraux1;
    cursoraux2 = companyact->cargacursor("SELECT * FROM familia WHERE padrefamilia IS NOT NULL ORDER BY idfamilia");
    while (!cursoraux2->eof()) {
        padre = cursoraux2->valor("padrefamilia").toInt();
        idfamilia = cursoraux2->valor("idfamilia").toInt();
        it = new QTreeWidgetItem(Lista1[padre]);
        Lista1[idfamilia] = it;
        it->setText(COL_NOMFAMILIA, cursoraux2->valor("nombrefamilia"));
        it->setText(COL_CODFAMILIA, cursoraux2->valor("codigofamilia"));
        it->setText(COL_DESCFAMILIA, cursoraux2->valor("descfamilia"));
        it->setText(COL_IDFAMILIA, cursoraux2->valor("idfamilia"));
        it->setText(COL_CODCOMPLETOFAMILIA, cursoraux2->valor("codigocompletofamilia"));
        m_listFamilias->expandItem(it);
        cursoraux2->siguienteregistro();
    } // end while
    delete cursoraux2;
    m_idfamilia = "";
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
}


QString FamiliasView::codigoCompletoFamilia() {
    QTreeWidgetItem *it = m_listFamilias->currentItem();
    if (it)
        return it->text(COL_CODCOMPLETOFAMILIA);
    else
        return "";
}


QString FamiliasView::idFamilia() {
    QTreeWidgetItem *it = m_listFamilias->currentItem();
    if (it)
        return it->text(COL_IDFAMILIA);
    else
        return "";
}


QString FamiliasView::nombreFamilia() {
    QTreeWidgetItem *it = m_listFamilias->currentItem();
    if (it)
        return it->text(COL_NOMFAMILIA);
    else
        return "";
}


/// Se ha seleccionado un item en la lista.
/// Lo que hacemos es mostar el elemento.
/// Si el anterior ha sido modificado pedimos para actuar en consecuencia.
void FamiliasView::on_m_listFamilias_itemDoubleClicked(QTreeWidgetItem *it) {
    if (m_modoConsulta) {
        m_idfamilia = it->text(COL_IDFAMILIA);
        emit selected(m_idfamilia);
    } // end if
}


/// Se ha seleccionado un item en la lista.
/// Lo que hacemos es mostar el elemento.
/// Si el anterior ha sido modificado pedimos para actuar en consecuencia.
void FamiliasView::on_m_listFamilias_currentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem *previous) {
    QString idfamiliaold = "";
    if (previous)
        m_idfamilia = previous->text(COL_IDFAMILIA);
    if (m_idfamilia != "") {
        trataModificado();
        pintar(previous);
    } // end if
    m_idfamilia = current->text(COL_IDFAMILIA);
    mostrarplantilla();
}


void FamiliasView::mostrarplantilla() {
    _depura("mostramos la plantilla\n", 0);
    QString query;
    query= "SELECT * from familia WHERE idfamilia = " + m_idfamilia;
    cursor2 *cursorfamilia = companyact->cargacursor(query);
    if (!cursorfamilia->eof()) {
        m_nomFamilia->setText(cursorfamilia->valor("nombrefamilia"));
        m_descFamilia->setPlainText(cursorfamilia->valor("descfamilia"));
        m_codCompletoFamilia->setText(cursorfamilia->valor("codigocompletofamilia"));
        m_codFamilia->setText(cursorfamilia->valor("codigofamilia"));
    } // end if
    delete cursorfamilia;
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    _depura("Terminamos la ejecución de FamiliasView::mostrarplantilla\n", 0);
}


/// Antes de salir de la ventana debemos hacer la comprobacion de si se ha modificado algo
/// Esta funcion esta dedicada a Francina, Bienvenida al mundo :))
void FamiliasView::close() {
    trataModificado();
    QWidget::close();
}


bool FamiliasView::trataModificado() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar familia"),
                                 tr("Desea guardar los cambios?"),
                                 QMessageBox::Ok,
                                 QMessageBox::Cancel) == QMessageBox::Ok)
            on_mui_guardar_clicked();
        return (TRUE);
    } // end if
    return(FALSE);
}


/// SLOT que responde a la pulsacion del boton de guardar el tipo de IVA que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
void FamiliasView::on_mui_guardar_clicked() {
    _depura("FamiliasView::on_mui_guardar_clicked", 0);
    try {
        if (m_idfamilia == "") {
            mensajeInfo(tr("Debe seleccionar una familia"));
            return;
        } // end if
        QString query = "UPDATE familia SET nombrefamilia = '" +
                        companyact->sanearCadena(m_nomFamilia->text()) + "', descfamilia = '" +
                        companyact->sanearCadena(m_descFamilia->toPlainText()) + "' , codigofamilia = '" +
                        companyact->sanearCadena(m_codFamilia->text()) + "' WHERE idfamilia =" + m_idfamilia;
        int error = companyact->ejecuta(query);
        if (error)
            throw -1;
        dialogChanges_cargaInicial();
        _depura("END FamiliasView::on_mui_guardar_clicked", 0);
    } catch (...) {
        mensajeInfo("Error al guardar la familia");
        return;
    } // end try
}


void FamiliasView::pintar(QTreeWidgetItem *it) {
    QString idfamilia = it->text(COL_IDFAMILIA);
    if (it) {
        cursor2 *cursoraux1 = companyact->cargacursor("SELECT * FROM familia WHERE idfamilia = " + idfamilia);
        if (!cursoraux1->eof()) {
            it->setText(COL_NOMFAMILIA, cursoraux1->valor("nombrefamilia"));
            it->setText(COL_CODFAMILIA, cursoraux1->valor("codigofamilia"));
            it->setText(COL_DESCFAMILIA, cursoraux1->valor("descfamilia"));
            it->setText(COL_IDFAMILIA, cursoraux1->valor("idfamilia"));
            it->setText(COL_CODCOMPLETOFAMILIA, cursoraux1->valor("codigocompletofamilia"));
        } // end if
        delete cursoraux1;
    } // end if
}


/// SLOT que responde a la pulsacion del boton de nuevo tipo de IVA
/// Inserta en la tabla de IVAs.
void FamiliasView::on_mui_crear_clicked() {
    _depura("FamiliasView::on_mui_crear_clicked", 0);
    try {
        companyact->begin();
        /// Si se ha modificado el contenido advertimos y guardamos.
        trataModificado();
        QString padrefamilia;
        if (m_idfamilia != "")
            padrefamilia = m_idfamilia;
        else
            padrefamilia = "NULL";

        QString query = "INSERT INTO familia (nombrefamilia, descfamilia, padrefamilia, codigofamilia) VALUES ('NUEVA FAMILIA', 'Descripcion de la familia', " + padrefamilia + ", 'XXX')";

        int error = companyact->ejecuta(query);
        if (error) {
            throw -1;
        } // end if
        cursor2 *cur = companyact->cargacursor("SELECT max(idfamilia) AS idfamilia FROM familia");
        companyact->commit();
        m_idfamilia = cur->valor("idfamilia");
        delete cur;
        pintar();
        _depura("END FamiliasView::on_mui_crear_clicked", 0);
    } catch (...) {
        companyact->rollback();
        mensajeInfo("Error al crear la familia");
    } // end try
}


/// SLOT que responde a la pulsacion del botón de borrar la familia que se está editando.
/// Lo que hace es que se hace un update de todos los campos.
void FamiliasView::on_mui_borrar_clicked() {
    _depura("FamiliasView::on_mui_borrar_clicked", 0);
    try {
        if (m_idfamilia == "") {
            mensajeInfo(tr("Debe seleccionar una familia"));
            return;
        } // end if
        trataModificado();
        QString query = "DELETE FROM FAMILIA WHERE idfamilia = " + m_idfamilia;
        int error = companyact->ejecuta(query);
        if (error)
            throw -1;
        pintar();
        _depura("END FamiliasView::on_mui_borrar_clicked", 0);
    } catch (...) {
        mensajeInfo("Error al borrar la familia");
    } // end try
}


void FamiliasView::on_mui_imprimir_clicked() {
    /// Copiamos el archivo.
    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "familias.rml";
    archivo = "cp " + archivo + " /tmp/familias.rml";
    system (archivo.toAscii().constData());
    /// Copiamos el logo.
    archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";
    archivo = "cp " + archivo + " /tmp/logo.jpg";
    system(archivo.toAscii().constData());
    QFile file;
    file.setFileName("/tmp/familias.rml");
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt;
    /// Linea de totales del presupuesto.
    fitxersortidatxt = "<blockTable style=\"tabla\" colWidths=\"3cm, 15cm\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "        <td>" + tr("Codigo") + "</td>";
    fitxersortidatxt += "        <td>" + tr("Nombre") + "</td>";
    fitxersortidatxt += "</tr>";

    cursor2 *cur = companyact->cargacursor("SELECT * FROM familia ORDER BY codigocompletofamilia");
    while(!cur->eof()) {
        fitxersortidatxt += "<tr>";
        fitxersortidatxt += "        <td>" + cur->valor("codigocompletofamilia") + "</td>";
        fitxersortidatxt += "        <td>" + cur->valor("nombrefamilia") + "</td>";
        fitxersortidatxt += "</tr>";
        cur->siguienteregistro();
    } // end if
    delete cur;
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]", fitxersortidatxt);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    }
    system("bgtrml2pdf.py /tmp/familias.rml > /tmp/familias.pdf");
    system("kpdf /tmp/familias.pdf &");
}


void FamiliasView::on_mui_aceptar_clicked() {
    trataModificado();
    QTreeWidgetItem *it = m_listFamilias->currentItem();
    if (it)
        m_idfamilia = it->text(COL_IDFAMILIA);
    else
        m_idfamilia = "";
    close();
}

