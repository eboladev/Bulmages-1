/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org -- BulmaFact Facturacion GPL                    *
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
#include <QComboBox>

#include "subform2bf.h"
#include "funcaux.h"
#include "articulolist.h"
#include "busquedaarticulo.h"
#include "busquedatipoiva.h"
#include "qtexteditdelegate.h"
#include "busquedatrabajador.h"
#include "plugins.h"


SubForm2Bf::SubForm2Bf(QWidget *parent) : SubForm3(parent) {
    _depura("SubForm2Bf::SubForm2Bf", 0);
    setDelete(TRUE);
    m_delegate = new QSubForm2BfDelegate(this);
    mui_list->setItemDelegate(m_delegate);
    mdb_idcliente = "";
    _depura("END SubForm2Bf::SubForm2Bf", 0);
}


SubForm2Bf::~SubForm2Bf() {
    _depura("SubForm2Bf::~SubForm2Bf", 0);
    delete m_delegate;
    _depura("END SubForm2Bf::~SubForm2Bf", 0);
}


void SubForm2Bf::cargar(QString query) {
    _depura("SubForm2Bf::cargar", 0);
    SubForm3::cargar(query);
    _depura("END SubForm2Bf::cargar", 0);
}


void SubForm2Bf::on_mui_list_pressedAsterisk(int row, int col) {
    _depura("SubForm2Bf::pressedAsterisk", 0);


    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row, col);

    if (camp->nomcampo() != "codigocompletoarticulo") {
        return;
    } // end if
    
    m_procesacambios = FALSE;

    ArticuloList *artlist = new ArticuloList((company *) empresaBase(), NULL, 0, ArticuloList::SelectMode);
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    this->setEnabled(false);
    artlist->show();
    while (!artlist->isHidden())
        theApp->processEvents();
    this->setEnabled(true);
    QString idArticle = artlist->idarticulo();
    delete artlist;
    cursor2 *cur = empresaBase()->cargacursor("SELECT * FROM articulo WHERE idarticulo = " + idArticle);
    if (!cur->eof()) {
        rec->setDBvalue("idarticulo", idArticle);
        rec->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
        rec->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
    } // end if
    delete cur;
    m_procesacambios = TRUE;
    /// Invocamos la finalizacion de edicion para que todos los campos se actualicen.
    on_mui_list_cellChanged(row, col);
    _depura("END SubForm2Bf::pressedAsterisk", 0);
}


void SubForm2Bf::on_mui_list_pressedSlash(int row, int col) {
    _depura("SubForm2Bf::pressedSlash", 0);
    SDBCampo *camp = (SDBCampo *) item(row, col);
    QString text = editaTexto(camp->text());
    camp->set(text);
    _depura("END SubForm2Bf::pressedSlash", 0);
}


void SubForm2Bf::on_mui_list_pressedMinus(int row, int col) {
    _depura("SubForm2Bf::pressedMinus", 0);

    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row, col);
    cursor2 *cur = empresaBase()->cargacursor("SELECT * FROM articulo WHERE idarticulo = " + rec->DBvalue("idarticulo"));
    if (!cur->eof()) {
        rec->setDBvalue(camp->nomcampo(), cur->valor("obserarticulo"));
    } // end if
    delete cur;
    /// Invocamos la finalizacion de edicion para que todos los campos se actualicen.
    on_mui_list_cellChanged(row, col);
    _depura("END SubForm2Bf::pressedMinus", 0);
}


//void SubForm2Bf::on_mui_list_editFinished(int row, int col, int key) {
void SubForm2Bf::on_mui_list_cellChanged(int row, int col) {
    _depura("SubForm2Bf::on_mui_list_cellChanged", 0, QString::number(row) + " " + QString::number(col));

    if (!m_procesacambios) {
        _depura("SubForm2Bf::on_mui_list_cellChanged", 0, QString::number(row) + " " + QString::number(col)+" m_procesacambios es FALSE");
        return;
    }
    m_procesacambios = FALSE;

    cursor2 *cur2 = NULL;
    cursor2 *cur = NULL;
    cursor2 *cur1 = NULL;

    /// Disparamos los plugins.
    int res = g_plugins->lanza("SubForm2Bf_on_mui_list_editFinished", this);
    if (res != 0) {
        m_procesacambios = TRUE;
        return;
    }

    SDBRecord *rec = lineaat(row);
    if (rec == NULL) {
        _depura ("SubForm2Bf::on_mui_list_cellChanged", 0, QString::number(row) + " " + QString::number(col)+ "la linea no existe");
        m_procesacambios = TRUE;
        return;
    }

    SDBCampo *camp = (SDBCampo *) item(row, col);
    camp->refresh();

    /// Si el campo no ha sido cambiado se sale.
    if ( ! camp->cambiado() ) {
        _depura("SubForm2Bf::on_mui_list_cellChanged", 0, QString::number(row) + " " + QString::number(col)+ camp->valorcampo() + "Sin cambios");
        m_procesacambios = TRUE;
        SubForm3::on_mui_list_cellChanged(row, col);
        return;
    } // end if


    if (camp->nomcampo() == "desctipo_iva") {
        cur = empresaBase()->cargacursor("SELECT * FROM tipo_iva WHERE desctipo_iva = '" + camp->text() + "'");
        if (!cur->eof()) {
            rec->setDBvalue("idtipo_iva", cur->valor("idtipo_iva"));
        } // end if
    } // end if


    if (camp->nomcampo() == "nomtrabajador") {
        cur = empresaBase()->cargacursor("SELECT * FROM trabajador WHERE apellidostrabajador ||', '||nomtrabajador = '" + camp->text() + "'");
        if (!cur->eof()) {
            rec->setDBvalue("idtrabajador", cur->valor("idtrabajador"));
        } // end if
    } // end if

    if (camp->nomcampo() == "codigocompletoarticulo") {
        cur = empresaBase()->cargacursor("SELECT * FROM articulo WHERE codigocompletoarticulo = '" + camp->text() + "'");
        if (!cur->eof()) {
            rec->setDBvalue("idarticulo", cur->valor("idarticulo"));
            rec->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
            rec->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
            if (m_tablename == "lpresupuesto"
                    || m_tablename == "lpedidoproveedor"
                    || m_tablename == "lpedidocliente"
                    || m_tablename == "lalbaranp"
                    || m_tablename == "lfacturap"
                    || m_tablename == "lalbaran"
                    || m_tablename == "lfactura") {
                rec->setDBvalue("desc"+m_tablename, cur->valor("nomarticulo"));
                rec->setDBvalue("cant"+m_tablename, "1.00");
                rec->setDBvalue("descuento"+m_tablename, "0.00");
                rec->setDBvalue("pvp"+m_tablename, cur->valor("pvparticulo"));
            } // end if
        } else {
            mensajeAviso(tr("Articulo inexistente"));
            delete cur;
            m_procesacambios = TRUE;
            return;
        } // end if


        cur1 = empresaBase()->cargacursor("SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->valor("idtipo_iva") + " ORDER BY fechatasa_iva LIMIT 1");
        if (!cur->eof() ) {
            if (m_tablename == "lpresupuesto"
                    || m_tablename == "lpedidocliente"
                    || m_tablename == "lpedidoproveedor"
                    || m_tablename == "lalbaranp"
                    || m_tablename == "lfacturap"
                    || m_tablename == "lalbaran"
                    || m_tablename == "lfactura") {
                rec->setDBvalue("iva"+m_tablename, cur1->valor("porcentasa_iva"));

                /// Calculamos el recargo equivalente.
                if (mdb_idcliente != "") {
                    cur2 = empresaBase()->cargacursor("SELECT recargoeqcliente FROM cliente WHERE idcliente="+mdb_idcliente);
                    if (!cur2->eof()) {
                        if (cur2->valor("recargoeqcliente") == "t") {
                            rec->setDBvalue("reqeq"+m_tablename, cur1->valor("porcentretasa_iva"));
                        } // end if
                    } // end if
                    delete cur2;
                } else {
                    rec->setDBvalue("reqeq"+m_tablename, "0");
                } // end if

                if (mdb_idproveedor != "") {
                    cur2 = empresaBase()->cargacursor("SELECT recargoeqproveedor FROM proveedor WHERE idproveedor="+mdb_idproveedor);
                    if (!cur2->eof()) {
                        if (cur2->valor("recargoeqproveedor") == "t") {
                            rec->setDBvalue("reqeq"+m_tablename, cur1->valor("porcentretasa_iva"));
                        } // end if
                    } // end if
                    delete cur2;
                } else {
                    rec->setDBvalue("reqeq"+m_tablename, "0");
                } // end if

            } // end if
        } // end if
        if (cur1 != NULL)
            delete cur1;
        if (cur != NULL)
            delete cur;
    } // end if

    m_procesacambios = TRUE;
    SubForm3::on_mui_list_cellChanged(row, col);

    _depura("END SubForm2Bf::on_mui_list_editFinished", 0);
}

/*
void SubForm2Bf::contextMenuEvent(QContextMenuEvent *) {
    _depura("SubForm2Bf::contextMenuEvent", 0);
    QAction *del = NULL;
    int row = currentRow();
    if (row < 0)
        return;

    int col = currentColumn();
    if (row < 0)
        return;

    QMenu *popup = new QMenu(this);
    if (m_delete)
        del = popup->addAction(tr("Borrar registro"));
    popup->addSeparator();
    QAction *ajustc = popup->addAction(tr("Ajustar columna"));
    QAction *ajustac = popup->addAction(tr("Ajustar altura"));

    QAction *ajust = popup->addAction(tr("Ajustar columnas"));
    QAction *ajusta = popup->addAction(tr("Ajustar alturas"));

    popup->addSeparator();
    QAction *verconfig = popup->addAction(tr("Ver/Ocultar configurador de subformulario"));

    QAction *opcion = popup->exec(QCursor::pos());
    if (opcion == del && m_delete)
        borrar(row);
    if (opcion == ajust)
        resizeColumnsToContents();
    if (opcion == ajusta)
        resizeRowsToContents();
    if (opcion == ajustc)
        resizeColumnToContents(col);
    if (opcion == ajustac)
        resizeRowToContents(row);
    if (opcion == verconfig)
        toogleConfig();

    delete popup;
    _depura("END SubForm2Bf::contextMenuEvent", 0);
}
*/

void SubForm2Bf::setIdCliente(QString id) {
    _depura("SubForm2Bf::setIdCliente", 0, id);

    /// Reseteamos los valores.
    for (int i = 0; i < rowCount() - 1; i++) {
        SDBRecord *rec = lineaat(i);
        rec->setDBvalue("iva" + m_tablename, "0");
        rec->setDBvalue("reqeq" + m_tablename, "0");
    } // end for

    mdb_idcliente = id;


    /// Si el idcliente no existe salimos.
    if (id == "") {
        _depura("END SubForm2Bf::setIdCliente", 0, "idcliente invalido");
        return;
    } // end if

    cursor2 *curcliente = empresaBase()->cargacursor("SELECT recargoeqcliente, regimenfiscalcliente FROM cliente WHERE idcliente = " + mdb_idcliente);

    if (!curcliente->eof()) {

        /// Cuando se cambia el cliente se deben recalcular las lineas por si hay Recargo Equivalente
        for (int i = 0; i < rowCount() - 1; i++) {
            SDBRecord *rec = lineaat(i);
            cursor2 *cur = empresaBase()->cargacursor("SELECT * FROM articulo WHERE idarticulo = " + rec->DBvalue("idarticulo") );
            cursor2 *cur1 = empresaBase()->cargacursor("SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->valor("idtipo_iva") + " ORDER BY fechatasa_iva LIMIT 1");
            if (!cur->eof()) {

                if (curcliente->valor("regimenfiscalcliente") == "Normal") {
                    rec->setDBvalue("iva" + m_tablename, cur1->valor("porcentasa_iva"));
                } // end if

                if (curcliente->valor("recargoeqcliente") == "t") {
                    rec->setDBvalue("reqeq" + m_tablename, cur1->valor("porcentretasa_iva"));
                } // end if

            } // end if
            delete cur1;
            delete cur;
        } // end for
    } // end if
    delete curcliente;
    _depura("END SubForm2Bf::setIdCliente", 0);
}


void SubForm2Bf::setIdProveedor(QString id) {
    _depura("SubForm2Bf::setIdProveedor", 0, id);
    mdb_idproveedor = id;

    if (mdb_idproveedor == "")
        return;

    /// Reseteamos los valores
    for (int i = 0; i < rowCount() - 1; i++) {
        SDBRecord *rec = lineaat(i);
        rec->setDBvalue("iva"+m_tablename, "0");
        rec->setDBvalue("reqeq"+m_tablename, "0");
    } // end for

    cursor2 *curproveedor = empresaBase()->cargacursor("SELECT recargoeqproveedor, regimenfiscalproveedor FROM proveedor WHERE idproveedor="+mdb_idproveedor);
    if (! curproveedor->eof()) {

        /// Cuando se cambia el cliente se deben recalcular las lineas por si hay Recargo Equivalente
        for (int i = 0; i < rowCount() - 1; i++) {
            SDBRecord *rec = lineaat(i);
            cursor2 *cur = empresaBase()->cargacursor("SELECT * FROM articulo WHERE idarticulo = " + rec->DBvalue("idarticulo") );
            cursor2 *cur1 = empresaBase()->cargacursor("SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->valor("idtipo_iva") + " ORDER BY fechatasa_iva LIMIT 1");
            if (!cur->eof() ) {
                if (curproveedor->valor("regimenfiscalproveedor") == "Normal") {
                    rec->setDBvalue("iva"+m_tablename, cur1->valor("porcentasa_iva"));
                } // end if
                if (curproveedor->valor("recargoeqproveedor") == "t") {
                    rec->setDBvalue("reqeq"+m_tablename, cur1->valor("porcentretasa_iva"));
                } // end if

            } // end if
            delete cur1;
            delete cur;
        } // end for
    } // end if
    delete curproveedor;
    _depura("END SubForm2Bf::setIdProveedor", 0);
}


void SubForm2Bf::setEmpresaBase(EmpresaBase *c) {
    _depura("SubForm2Bf::setcompany", 0);
    SubForm3::setEmpresaBase(c);
    m_delegate->setEmpresaBase(c);
    _depura("END SubForm2Bf::setcompany", 0);
}


void SubForm2Bf::setDelete(bool f) {
    _depura("SubForm2Bf::setDelete", 0);
    m_delete = f;
    _depura("END SubForm2Bf::setDelete", 0);
}


/// ===============================================================
///  Tratamientos del Item Delegate
/// ===============================================================
QSubForm2BfDelegate::QSubForm2BfDelegate(QObject *parent = 0) : QItemDelegate(parent), PEmpresaBase() {
    _depura("QSubForm2BfDelegate::QSubForm2BfDelegate", 0);
    m_subform = (SubForm2Bf *) parent;
//    m_subform->m_procesacambios = FALSE;
    installEventFilter(this);
    _depura("END QSubForm2BfDelegate::QSubForm2BfDelegate", 0);
}


QSubForm2BfDelegate::~QSubForm2BfDelegate() {
    _depura("QSubForm2BfDelegate::~QSubForm2BfDelegate", 0);
    _depura("END QSubForm2BfDelegate::~QSubForm2BfDelegate", 0);
}


QWidget *QSubForm2BfDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    _depura("QSubForm2BfDelegate::createEditor", 0);
    SHeader *linea;
    linea = m_subform->cabecera()->at(index.column());
    _depura("QSubForm2BfDelegate::createEditor", 0, "CurrentColumn: " + QString::number(index.column()));
    _depura("QSubForm2BfDelegate::createEditor", 0, "CurrentRow" + QString::number(index.row()));

    if (linea->nomcampo() == "desc" + m_subform->tableName()) {
        QTextEditDelegate *editor = new QTextEditDelegate(parent);
        editor->setObjectName("QTextEditDelegate");
        return editor;
        /*
            } else if (linea->nomcampo() == "cant" + m_subform->tableName()
                       || linea->nomcampo() == "pvp" + m_subform->tableName()
                       || linea->nomcampo() == "descuento" + m_subform->tableName()
                       || linea->nomcampo() == "iva" + m_subform->tableName()) {

                QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
                editor->setMinimum(-1000000);
                editor->setMaximum(1000000);
                return editor;
        */

    } else if (linea->nomcampo() == "codigocompletoarticulo") {
        BusquedaArticuloDelegate *editor = new BusquedaArticuloDelegate(parent);
        editor->setEmpresaBase((company *)m_subform->empresaBase());
        return editor;
    } else if (linea->nomcampo() == "desctipo_iva") {
        BusquedaTipoIVADelegate *editor = new BusquedaTipoIVADelegate(parent);
        editor->setEmpresaBase((company *)m_subform->empresaBase());
        return editor;
    } else if (linea->nomcampo() == "nomtrabajador") {
        BusquedaTrabajadorDelegate *editor = new BusquedaTrabajadorDelegate(parent);
        editor->setEmpresaBase((company *)m_subform->empresaBase());
        return editor;
    } else  {
        return QItemDelegate::createEditor(parent, option, index);
    } // end if
    _depura("END QSubForm2BfDelegate::createEditor", 0);
}


void QSubForm2BfDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    _depura("QSubForm2BfDelegate::setModelData", 0);
    _depura("QSubForm2BfDelegate::setModelData", 0, "CurrentColumn: " + QString::number(index.column()));
    _depura("QSubForm2BfDelegate::setModelData", 0, "CurrentRow: " + QString::number(index.row()));

    /// Si la fila o columna pasadas son invalidas salimos.
    if (index.column() < 0 || index.row() < 0)
        return;

    SHeader *linea;
    linea = m_subform->cabecera()->at(index.column());
    if (linea->nomcampo() == "desc" + m_subform->tableName()) {
        QTextEditDelegate *textedit = qobject_cast<QTextEditDelegate *>(editor);
        model->setData(index, textedit->toPlainText());
        return;
        /// Quitamos el SpinBox para cantidades porque funciona realmente mal */
        /*
            } else if (linea->nomcampo() == "cant" + m_subform->tableName()
                       || linea->nomcampo() == "pvp" + m_subform->tableName()
                       || linea->nomcampo() == "descuento" + m_subform->tableName()
                       || linea->nomcampo() == "iva" + m_subform->tableName()   ) {

                QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
            // Esto funciona como el culo. con cantidad 1090,89 se va
                //spinBox->interpretText();
                QString value = spinBox->text();
                // value = value.replace(",", ".");
                model->setData(index, value );

        */

    } else if (linea->nomcampo() == "codigocompletoarticulo") {
        BusquedaArticuloDelegate *comboBox = static_cast<BusquedaArticuloDelegate*>(editor);
        QString value = comboBox->currentText();
        value = value.left(value.indexOf(".-"));
        model->setData(index, value);
    } else if (linea->nomcampo() == "desctipo_iva") {
        BusquedaTipoIVADelegate *comboBox = static_cast<BusquedaTipoIVADelegate*>(editor);
        QString value = comboBox->currentText();
        model->setData(index, value);
    } else if (linea->nomcampo() == "nomtrabajador") {
        BusquedaTrabajadorDelegate *comboBox = static_cast<BusquedaTrabajadorDelegate*>(editor);
        QString value = comboBox->currentText();
        model->setData(index, value);
    } else {
        QItemDelegate::setModelData(editor, model, index);
    } // end if
    _depura("END QSubForm2BfDelegate::setModelData", 0);
}


void QSubForm2BfDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    _depura("QSubForm2BfDelegate::setEditorData", 0);
    _depura("QSubForm2BfDelegate::setEditorData", 0, "CurrentColumn: " + QString::number(index.column()));
    _depura("QSubForm2BfDelegate::setEditorData", 0, "CurrentRow: " + QString::number(index.row()));
    SHeader *linea;
    linea = m_subform->cabecera()->at(index.column());
    if (linea->nomcampo() == "desc" + m_subform->tableName()) {
        QString data = index.model()->data(index, Qt::DisplayRole).toString();
        QTextEditDelegate *textedit = qobject_cast<QTextEditDelegate*>(editor);
        textedit->setText(data);
        /// Quitamos el SpinBox para cantidades pq funciona realmente mal
        /*
            } else if (linea->nomcampo() == "cant" + m_subform->tableName()
                       || linea->nomcampo() == "pvp" + m_subform->tableName()
                       || linea->nomcampo() == "descuento" + m_subform->tableName()
                       || linea->nomcampo() == "iva" + m_subform->tableName()   ) {

                QString value = index.model()->data(index, Qt::DisplayRole).toString();
                QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
                spinBox->setValue(value.toDouble());
        */

    } else if (linea->nomcampo() == "codigocompletoarticulo") {
        QString value = index.model()->data(index, Qt::DisplayRole).toString();
        BusquedaArticuloDelegate *comboBox = static_cast<BusquedaArticuloDelegate*>(editor);
        comboBox->addItem(value);
    } else if (linea->nomcampo() == "desctipo_iva") {
        QString value = index.model()->data(index, Qt::DisplayRole).toString();
        BusquedaTipoIVADelegate *comboBox = static_cast<BusquedaTipoIVADelegate*>(editor);
        comboBox->set(value);
    } else if (linea->nomcampo() == "nomtrabajador") {
        QString value = index.model()->data(index, Qt::DisplayRole).toString();
        BusquedaTrabajadorDelegate *comboBox = static_cast<BusquedaTrabajadorDelegate*>(editor);
        comboBox->set(value);
    } else {
        QItemDelegate::setEditorData(editor, index);
    } // end if
    _depura("END QSubForm2BfDelegate::setEditorData", 0);
}


bool QSubForm2BfDelegate::eventFilter(QObject *obj, QEvent *event) {
    _depura("QSubForm2BfDelegate::eventFilter", 0, obj->objectName() + " --> " + QString::number(event->type()));
    if (obj->isWidgetType()) {
        _depura("QSubForm2BfDelegate:: de tipo toolTip", 0, ((QWidget *)obj)->toolTip());
        _depura("QSubForm2BfDelegate:: de tipo windowRole", 0, ((QWidget *)obj)->windowRole());
        _depura("QSubForm2BfDelegate:: de tipo accesibleDescription", 0, ((QWidget *)obj)->accessibleDescription());
        _depura("QSubForm2BfDelegate:: de tipo accesibleName", 0, ((QWidget *)obj)->accessibleName());
    } // end if

    /// Si es un release de tecla se hace la funcionalidad especificada.
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
        _depura("QSubForm2BfDelegate::key = :", 0, QString::number(key));
        Qt::KeyboardModifiers mod = keyEvent->modifiers();
        /// ------------------ EL CAMBIO ------------------------------
        switch (key) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            if (obj->objectName() == "QTextEditDelegate") {
                obj->event(event);
                return TRUE;
            } // end if
        } // end switch

    } // end if
    _depura("END QSubForm2BfDelegate::eventFilter", 0);
    return QItemDelegate::eventFilter(obj, event);
}


