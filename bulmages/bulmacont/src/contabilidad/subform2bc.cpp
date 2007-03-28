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

#include <QMessageBox>
#include <QMenu>
#include <QKeyEvent>
#include <QEvent>
#include <QLocale>

#include "subform3.h"
#include "subform2bc.h"
#include "funcaux.h"
#include "empresa.h"
#include "asiento1view.h"
#include "extractoview1.h"
#include "balance1view.h"
#include "qtexteditdelegate.h"

/// Incluimos las imagenes que catalogan los tipos de cuentas.
#include "images/cactivo.xpm"
#include "images/cpasivo.xpm"
#include "images/cneto.xpm"
#include "images/cingresos.xpm"
#include "images/cgastos.xpm"


SubForm2Bc::SubForm2Bc(QWidget *parent) : SubForm3(parent) {
    _depura("SubForm2Bc::SubForm2Bc", 0);
    m_delegate = new QSubForm2BcDelegate(this);
    mui_list->setItemDelegate(m_delegate);
    _depura("END SubForm2Bc::SubForm2Bc", 0);
}


SubForm2Bc::~SubForm2Bc() {
    _depura("SubForm2Bc::~SubForm2Bc", 0);
    delete m_delegate;
    _depura("END SubForm2Bc::~SubForm2Bc", 0);
}


void SubForm2Bc::on_mui_list_pressedAsterisk(int row, int col) {
    _depura ("SubForm2Bc::on_mui_list_pressedAsterisk", 0);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row, col);
    /// Si no es un campo de tipo codigo salimos.
    if (camp->nomcampo() != "codigo" && camp->nomcampo() != "codigoctacliente")
        return;

    QDialog *diag = new QDialog(0);
    diag->setModal(true);
    listcuentasview1 *listcuentas = new listcuentasview1((empresa *)m_companyact, diag, 0, listcuentasview1::SelectMode);
    connect(listcuentas, SIGNAL(selected(QString)), diag, SLOT(accept()));
    diag->exec();
    if (listcuentas->codcuenta() != "") {
        cursor2 *cur = companyact()->cargacursor("SELECT * FROM cuenta WHERE codigo = '" + listcuentas->codcuenta() + "'");
        if (!cur->eof() ) {
            if (camp->nomcampo() == "codigo") {
                rec->setDBvalue("idcuenta", cur->valor("idcuenta"));
                rec->setDBvalue("codigo", cur->valor("codigo"));
                rec->setDBvalue("tipocuenta", cur->valor("tipocuenta"));
                rec->setDBvalue("descripcion", cur->valor("descripcion"));
            } // end if
            if (camp->nomcampo() == "codigoctacliente") {
                rec->setDBvalue("idctacliente", cur->valor("idcuenta"));
                rec->setDBvalue("codigoctacliente", cur->valor("codigo"));
                rec->setDBvalue("tipoctacliente", cur->valor("tipocuenta"));
                rec->setDBvalue("nomctacliente", cur->valor("descripcion"));
            } // end if
        } // end if
        delete cur;
    } // end if
    delete diag;
    _depura ("END SubForm2Bc::on_mui_list_pressedAsterisk", 0);
}


void SubForm2Bc::on_mui_list_pressedSlash(int row, int col) {
    _depura("SubForm2Bc::on_mui_list_pressedSlash", 0);
    SDBCampo *camp = (SDBCampo *) item(row, col);
    if (camp->nomcampo() == "fecha")
        return;
    QString text = editaTexto(camp->text());
    camp->set(text);
    _depura("END SubForm2Bc::on_mui_list_pressedSlash", 0);
}


void SubForm2Bc::on_mui_list_editFinished(int row, int col, int key) {
    _depura("SubForm2Bc::editFinished", 0);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row, col);
    camp->refresh();
    if (camp->nomcampo() == "codigo") {
        QString codigoext = extiendecodigo(camp->text(), ((empresa *) m_companyact)->numdigitosempresa());
        cursor2 *cur = companyact()->cargacursor("SELECT * FROM cuenta WHERE codigo = '" + codigoext + "'");
        if (!cur->eof() ) {
            rec->setDBvalue("idcuenta", cur->valor("idcuenta"));
            rec->setDBvalue("codigo", cur->valor("codigo"));
            rec->setDBvalue("tipocuenta", cur->valor("tipocuenta"));
            rec->setDBvalue("descripcioncuenta", cur->valor("descripcion"));
        } // end if
        delete cur;
    } // end if
    if (camp->nomcampo() == "fecha") {
        QString nfecha = normalizafecha( camp->text()).toString("dd/MM/yyyy");
        rec->setDBvalue( "fecha", nfecha);
    } // end if
    SubForm3::on_mui_list_editFinished(row, col, key);
    _depura("END SubForm2Bc::editFinished", 0);
}


/// Muestra la ventana de asiento.
void SubForm2Bc::boton_asiento() {
    _depura("SubForm2Bc::boton_asiento", 0);
    empresa *companyact = (empresa *) m_companyact;
    QString numasiento = DBvalue("idasiento");
    if (numasiento != "") {
        companyact->intapuntsempresa()->muestraasiento(numasiento.toInt());
        companyact->muestraapuntes1();
    } // end if
    _depura("END SubForm2Bc::boton_asiento", 0);
}


/// Si el parametro pasado es un:
/// 0 -> del d&iacute;a actual
/// 1 -> del mes actual
/// 2 -> del a&ntilde;o actual
void SubForm2Bc::boton_extracto1(int tipo) {
    _depura("SubForm2Bc::boton_extracto1", 0);
    empresa *companyact = (empresa *) m_companyact;
    QDate fecha1, fecha2, fechaact;
    QString fecha = DBvalue("fecha").left(10);
    QString codigo = DBvalue("codigo");
    if (fecha != "" && codigo != "") {
        fechaact = normalizafecha(fecha);
        switch (tipo) {
        case 0:
            fecha1.setYMD(fechaact.year(), fechaact.month(), fechaact.day());
            fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.day());
            break;
        case 1:
            fecha1.setYMD(fechaact.year(), fechaact.month(), 1);
            fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.daysInMonth());
            break;
        case 2:
            fecha1.setYMD(fechaact.year(), 1, 1);
            fecha2.setYMD(fechaact.year(), 12, 31);
            break;
        } // end switch
        companyact->extractoempresa()->inicializa1(codigo, codigo, fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
        companyact->extractoempresa()->accept();
        companyact->libromayor();
    } // end if
    _depura("END SubForm2Bc::boton_extracto1", 0);
}


/// Si el parametro pasado es un:
/// 0 -> del d&iacute;a actual
/// 1 -> del mes actual
/// 2 -> del a&ntilde;o actual
void SubForm2Bc::boton_diario1(int tipo) {
    _depura("SubForm2Bc::boton_diario1", 0);
    empresa *companyact = (empresa *) m_companyact;
    QDate fecha1, fecha2, fechaact, fechaact1;
    fechaact = normalizafecha(DBvalue("fecha").left(10));
    fechaact1 = normalizafecha(DBvalue("fecha").left(10));
    if (DBvalue("fecha").left(10) != "") {
        switch (tipo) {
        case 0:
            fecha1.setYMD(fechaact.year(), fechaact.month(), fechaact.day());
            fecha2.setYMD(fechaact1.year(), fechaact1.month(), fechaact1.day());
            break;
        case 1:
            fecha1.setYMD(fechaact.year(), fechaact.month(), 1);
            fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.daysInMonth());
            break;
        case 2:
            fecha1.setYMD(fechaact.year(), 1, 1);
            fecha2.setYMD(fechaact.year(), 12, 31);
            break;
        } // end switch
        companyact->diarioempresa()->inicializa1((char *) fecha1.toString("dd/MM/yyyy").toAscii().constData(), (char *) fecha2.toString("dd/MM/yyyy").toAscii().constData(), 0);
    } // end if
    companyact->diarioempresa()->accept();
    companyact->librodiario();
    _depura("SubForm2Bc::boton_diario1", 0);
}


/// Si el parametro pasado es un:
/// 0 -> del per&iacute;odo actual.
/// 1 -> del mes actual mirado a partir de la fecha de inicio.
/// 2 -> del a&ntilde;o actual mirado a partir de la fecha de inicio.
void SubForm2Bc::boton_balance1(int tipo) {
    _depura("SubForm2Bc::boton_balance1", 0);
    empresa *companyact = (empresa *) m_companyact;
    QString fecha = DBvalue("fecha").left(10);
    QString codigo = DBvalue("codigo");
    QDate fecha1, fecha2, fechaact, fechaact1;
    if (fecha != "" && codigo != "") {
        fechaact = normalizafecha(fecha);
        switch (tipo) {
        case 0:
            fecha1.setYMD(fechaact.year(), fechaact.month(), fechaact.day());
            fecha2.setYMD(fechaact1.year(), fechaact.month(), fechaact.day());
            break;
        case 1:
            fecha1.setYMD(fechaact.year(), fechaact.month(), 1);
            fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.daysInMonth());
            break;
        case 2:
            fecha1.setYMD(fechaact.year(), 1, 1);
            fecha2.setYMD(fechaact.year(), 12, 31);
            break;
        } // end switch
        companyact->balanceempresa()->inicializa1(codigo, codigo, fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), "0");
        companyact->balanceempresa()->accept();
        companyact->librobalance();
    } // end if
    _depura("END SubForm2Bc::boton_balance1", 0);
}


/// Si el parametro pasado es un:
/// 0 -> del per&iacute;odo actual.
/// 1 -> del mes actual mirado a partir de la fecha de inicio.
/// 2 -> del a&ntilde;o actual mirado a partir de la fecha de inicio.
void SubForm2Bc::boton_balancetree(int tipo) {
    _depura("SubForm2Bc::boton_balance2",0);
    empresa *companyact = (empresa *) m_companyact;
    QString fecha = DBvalue("fecha").left(10);
    QString codigo = DBvalue("codigo");
    QDate fecha1, fecha2, fechaact, fechaact1;
    if (fecha != "" && codigo != "") {
        fechaact = normalizafecha(fecha);
        switch (tipo) {
        case 0:
            fecha1.setYMD(fechaact.year(), fechaact.month(), fechaact.day());
            fecha2.setYMD(fechaact1.year(), fechaact.month(), fechaact.day());
            break;
        case 1:
            fecha1.setYMD(fechaact.year(), fechaact.month(), 1);
            fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.daysInMonth());
            break;
        case 2:
            fecha1.setYMD(fechaact.year(), 1, 1);
            fecha2.setYMD(fechaact.year(), 12, 31);
            break;
        } // end switch
        companyact->balance1empresa()->inicializa1(codigo, codigo, fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
        companyact->balance1empresa()->on_mui_actualizar_clicked();
        companyact->librobalancetree();
    } // end if
    _depura("END SubForm2Bc::boton_balance2", 0);
}


void SubForm2Bc::creaMenu(QMenu *menu) {
    _depura("SubForm2Bc::pintaMenu", 0);
    QAction *ac = menu->addAction(tr("Submenu de contabilidad"));
    menu->addSeparator();
    _depura("END SubForm2Bc::pintaMenu", 0);
}


void SubForm2Bc::procesaMenu(QAction *ac) {
    _depura("SubForm2Bc::procesaMenu", 0);
    _depura("END SubForm2Bc::procesaMenu", 0);
}


/// ===============================================================
///  Tratamientos del Item Delegate
/// ===============================================================
QSubForm2BcDelegate::QSubForm2BcDelegate(QObject *parent = 0) : QItemDelegate(parent) {
    _depura("QSubForm2BcDelegate::QSubForm2BcDelegate", 0);
    m_subform = (SubForm2Bc *) parent;
    installEventFilter(this);
    _depura("END QSubForm2BcDelegate::QSubForm2BcDelegate", 0);
}


QSubForm2BcDelegate::~QSubForm2BcDelegate() {
    _depura("QSubForm2BcDelegate::~QSubForm2BcDelegate", 0);
    _depura("END QSubForm2BcDelegate::~QSubForm2BcDelegate", 0);
}


QWidget *QSubForm2BcDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    _depura("QSubForm2BcDelegate::createEditor", 0);
    SHeader *linea;
    linea = m_subform->cabecera()->at(index.column());
    _depura("QSubForm2BcDelegate::createEditor", 0, "CurrentColumn: " + QString::number(index.column()));
    _depura("QSubForm2BcDelegate::createEditor", 0, "CurrentRow: " + QString::number(index.row()));

    if (linea->nomcampo() == "desc" + m_subform->tableName()) {
        QTextEditDelegate *editor = new QTextEditDelegate(parent);
        editor->setObjectName("QTextEditDelegate");
        return editor;
    } else if (linea->nomcampo() == "debe"
               || linea->nomcampo() == "haber") {
        QDoubleSpinBox2 *editor = new QDoubleSpinBox2(parent);
        editor->setMinimum(-1000000);
        editor->setMaximum(1000000);
        return editor;
    } else if (linea->nomcampo() == "codigo") {
        BusquedaCuentaDelegate *editor = new BusquedaCuentaDelegate(parent);
        editor->setcompany((empresa *)m_subform->companyact());
        return editor;
    } else {
        return QItemDelegate::createEditor(parent, option, index);
    } // end if
}


void QSubForm2BcDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    _depura("QSubForm2BcDelegate::setModelData", 0);
    _depura("QSubForm2BcDelegate::setModelData", 0, "CurrentColumn: " + QString::number(index.column()));
    _depura("QSubForm2BcDelegate::setModelData", 0, "CurrentRow: " + QString::number(index.row()));

    /// Si la fila o columna pasadas son invalidas salimos.
    if ( index.column() < 0 || index.row() < 0)
        return;

    SHeader *linea;
    linea = m_subform->cabecera()->at(index.column());
    if (linea->nomcampo() == "desc" + m_subform->tableName()) {
        QTextEditDelegate *textedit = qobject_cast<QTextEditDelegate *>(editor);
        model->setData(index, textedit->toPlainText());
        return;

    } else if (linea->nomcampo() == "debe"
               || linea->nomcampo() == "haber" + m_subform->tableName() ) {
        QDoubleSpinBox2 *spinBox = static_cast<QDoubleSpinBox2*>(editor);
        spinBox->interpretText();
        QString value = spinBox->text();
        //value = value.replace(",", ".");
        model->setData(index, value);
    } else if (linea->nomcampo() == "codigo") {
        BusquedaCuentaDelegate *comboBox = static_cast<BusquedaCuentaDelegate*>(editor);
        QString value = comboBox->currentText();
        value = value.left(value.indexOf(".-"));
        QString codigoext = extiendecodigo(value, ((empresa *) m_companyact)->numdigitosempresa());
        model->setData(index, codigoext);
    } else {
        QItemDelegate::setModelData(editor, model, index);
    } // end if
    _depura("END QSubForm2BcDelegate::setModelData", 0);
}


void QSubForm2BcDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    _depura("QSubForm2BcDelegate::setEditorData", 0);
    _depura("QSubForm2BcDelegate::setEditorData", 0, "CurrentColumn: " + QString::number(index.column()));
    _depura("QSubForm2BcDelegate::setEditorData", 0, "CurrentRow: " + QString::number(index.row()));
    SHeader *linea;
    linea = m_subform->cabecera()->at(index.column());
    if (linea->nomcampo() == "desc" + m_subform->tableName()) {
        QString data = index.model()->data(index, Qt::DisplayRole).toString();
        QTextEditDelegate *textedit = qobject_cast<QTextEditDelegate*>(editor);
        textedit->setText(data);
        textedit->selectAll();
    } else if (linea->nomcampo() == "debe"
               || linea->nomcampo() == "haber") {
        QString value = index.model()->data(index, Qt::DisplayRole).toString();
        QDoubleSpinBox2 *spinBox = static_cast<QDoubleSpinBox2*>(editor);
        spinBox->setValue(value.toDouble());
        //_depura("ggg: " + QString::number(value.toDouble()), 2);
        spinBox->selectAll();
    } else if (linea->nomcampo() == "codigo") {
        QString value = index.model()->data(index, Qt::DisplayRole).toString();
        BusquedaCuentaDelegate *comboBox = static_cast<BusquedaCuentaDelegate*>(editor);
        comboBox->addItem(value);
        comboBox->lineEdit()->selectAll();
    } else {
        QItemDelegate::setEditorData(editor, index);
    } // end if
    _depura("END QSubForm2BcDelegate::setEditorData", 0);
}


bool QSubForm2BcDelegate::eventFilter(QObject *obj, QEvent *event) {
    _depura("QSubForm2BcDelegate::eventFilter", 0, obj->objectName() + " --> " + QString::number(event->type()));
    if (obj->isWidgetType()) {
        _depura("QSubForm2BcDelegate:: de tipo toolTip", 0, ((QWidget *)obj)->toolTip());
        _depura("QSubForm2BcDelegate:: de tipo windowRole", 0, ((QWidget *)obj)->windowRole());
    } // end if

    /// Si es un release de tecla se hace la funcionalidad especificada.

    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
        _depura("QSubForm2BcDelegate::key = : ", 0, QString::number(key));
        Qt::KeyboardModifiers mod = keyEvent->modifiers();
        /// ------------------ EL CAMBIO ------------------------------
        switch (key) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            if (event->type() == QEvent::KeyPress)
                return TRUE;
            if (obj->objectName() == "QTextEditDelegate") {
                obj->event(event);
                _depura("QSubForm2BcDelegate::eventFilter", 2, "Se ha pulsado el enter");
                return TRUE;
            } // end if
        } // end switch
    } // end if
    _depura("END QSubForm2BcDelegate::eventFilter()", 0);
    return QItemDelegate::eventFilter(obj, event);
}

