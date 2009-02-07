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
#include <QAbstractItemView>

#include "subform2bt.h"
#include "funcaux.h"
#include "qtexteditdelegate.h"
#include "plugins.h"


///
/**
\param parent
**/
SubForm2Bt::SubForm2Bt ( QWidget *parent ) : SubForm3 ( parent )
{
    _depura ( "SubForm2Bt::SubForm2Bt", 0 );
    setDelete ( TRUE );
    m_delegate = new QSubForm2BtDelegate ( this );
    mui_list->setItemDelegate ( m_delegate );
    mdb_idcliente = "";
    _depura ( "END SubForm2Bt::SubForm2Bt", 0 );
}


///
/**
**/
SubForm2Bt::~SubForm2Bt()
{
    _depura ( "SubForm2Bt::~SubForm2Bt", 0 );
    delete m_delegate;
    _depura ( "END SubForm2Bt::~SubForm2Bt", 0 );
}


///
/**
\param query
**/
void SubForm2Bt::cargar ( QString query )
{
    _depura ( "SubForm2Bt::cargar", 0 );
    SubForm3::cargar ( query );
    _depura ( "END SubForm2Bt::cargar", 0 );
}


///
/**
\param row
\param col
\return
**/
void SubForm2Bt::pressedAsterisk ( int row, int col, SDBRecord *rec, SDBCampo *camp )
{
    _depura ( "SubForm2Bt::pressedAsterisk", 0 );
    _depura ( "END SubForm2Bt::pressedAsterisk", 0 );
}


///
/**
\param row
\param col
**/
void SubForm2Bt::on_mui_list_pressedSlash ( int row, int col )
{
    _depura ( "SubForm2Bt::pressedSlash", 0 );
    SDBCampo *camp = ( SDBCampo * ) item ( row, col );
    QString text = editaTexto ( camp->text() );
    camp->set ( text );
    _depura ( "END SubForm2Bt::pressedSlash", 0 );
}


///
/**
\param row
\param col
**/
void SubForm2Bt::on_mui_list_pressedMinus ( int row, int col )
{
    _depura ( "SubForm2Bt::pressedMinus", 0 );
    _depura ( "END SubForm2Bt::pressedMinus", 0 );
}


///
/**
\param row
\param col
\return
**/
void SubForm2Bt::on_mui_list_cellChanged ( int row, int col )
{
    _depura ( "SubForm2Bt::on_mui_list_cellChanged", 0, QString::number ( row ) + " " + QString::number ( col ) );
    _depura ( "END SubForm2Bt::on_mui_list_editFinished", 0 );
}


///
/**
\param id
\return
**/
void SubForm2Bt::setIdCliente ( QString id )
{
    _depura ( "SubForm2Bt::setIdCliente", 0, id );

    /// En la primera carga no hay reajustes, pero si actualizacion del cliente.
    if ( mdb_idcliente  == "" ) {
        mdb_idcliente = id;
        return;
    } // end if

    /// En las cargas sucesivas si el idcliente no ha cambiado no se hace nada
    if ( mdb_idcliente == id ) return;

    /// Reseteamos los valores.
    for ( int i = 0; i < rowCount() - 1; i++ ) {
        SDBRecord *rec = lineaat ( i );
        rec->setDBvalue ( "iva" + m_tablename, "0" );
        rec->setDBvalue ( "reqeq" + m_tablename, "0" );
    } // end for

    mdb_idcliente = id;

    /// Si el idcliente no existe salimos.
    if ( id == "" ) {
        _depura ( "END SubForm2Bt::setIdCliente", 0, "idcliente invalido" );
        return;
    } // end if

    cursor2 *curcliente = empresaBase() ->cargacursor ( "SELECT recargoeqcliente, regimenfiscalcliente FROM cliente WHERE idcliente = " + mdb_idcliente );

    if ( !curcliente->eof() ) {
        /// Cuando se cambia el cliente se deben recalcular las lineas por si hay Recargo Equivalente
        for ( int i = 0; i < rowCount() - 1; i++ ) {
            SDBRecord *rec = lineaat ( i );
            cursor2 *cur = empresaBase() ->cargacursor ( "SELECT * FROM articulo WHERE idarticulo = " + rec->DBvalue ( "idarticulo" ) );
            cursor2 *cur1 = empresaBase() ->cargacursor ( "SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->valor ( "idtipo_iva" ) + " ORDER BY fechatasa_iva LIMIT 1" );
            if ( !cur->eof() ) {

                if ( curcliente->valor ( "regimenfiscalcliente" ) == "Normal" ) {
                    rec->setDBvalue ( "iva" + m_tablename, cur1->valor ( "porcentasa_iva" ) );
                } // end if

                if ( curcliente->valor ( "recargoeqcliente" ) == "t" ) {
                    rec->setDBvalue ( "reqeq" + m_tablename, cur1->valor ( "porcentretasa_iva" ) );
                } // end if

            } // end if
            delete cur1;
            delete cur;
        } // end for
    } // end if
    delete curcliente;
    _depura ( "END SubForm2Bt::setIdCliente", 0 );
}


///
/**
\param id
\return
**/
void SubForm2Bt::setIdProveedor ( QString id )
{
    _depura ( "SubForm2Bt::setIdProveedor", 0, id );

    /// En la primera carga no hay reajustes, pero si actualizacion del proveedor.
    if ( mdb_idproveedor  == "" ) {
        mdb_idproveedor = id;
        return;
    } // end if

    /// En las cargas sucesivas si el idproveedor no ha cambiado no se hace nada
    if ( mdb_idproveedor == id ) return;

    mdb_idproveedor = id;

    /// Reseteamos los valores
    for ( int i = 0; i < rowCount() - 1; i++ ) {
        SDBRecord *rec = lineaat ( i );
        rec->setDBvalue ( "iva" + m_tablename, "0" );
        rec->setDBvalue ( "reqeq" + m_tablename, "0" );
    } // end for

    cursor2 *curproveedor = empresaBase() ->cargacursor ( "SELECT recargoeqproveedor, regimenfiscalproveedor FROM proveedor WHERE idproveedor=" + mdb_idproveedor );
    if ( !curproveedor->eof() ) {
        /// Cuando se cambia el cliente se deben recalcular las lineas por si hay Recargo Equivalente
        for ( int i = 0; i < rowCount() - 1; i++ ) {
            SDBRecord *rec = lineaat ( i );
            cursor2 *cur = empresaBase() ->cargacursor ( "SELECT * FROM articulo WHERE idarticulo = " + rec->DBvalue ( "idarticulo" ) );
            cursor2 *cur1 = empresaBase() ->cargacursor ( "SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->valor ( "idtipo_iva" ) + " ORDER BY fechatasa_iva LIMIT 1" );
            if ( !cur->eof() ) {
                if ( curproveedor->valor ( "regimenfiscalproveedor" ) == "Normal" ) {
                    rec->setDBvalue ( "iva" + m_tablename, cur1->valor ( "porcentasa_iva" ) );
                } // end if
                if ( curproveedor->valor ( "recargoeqproveedor" ) == "t" ) {
                    rec->setDBvalue ( "reqeq" + m_tablename, cur1->valor ( "porcentretasa_iva" ) );
                } // end if

            } // end if
            delete cur1;
            delete cur;
        } // end for
    } // end if
    delete curproveedor;
    _depura ( "END SubForm2Bt::setIdProveedor", 0 );
}


///
/**
\param c
**/
void SubForm2Bt::setEmpresaBase ( EmpresaBase *c )
{
    _depura ( "SubForm2Bt::setcompany", 0 );
    SubForm3::setEmpresaBase ( c );
    m_delegate->setEmpresaBase ( c );
    _depura ( "END SubForm2Bt::setcompany", 0 );
}


///
/**
\return
**/
int SubForm2Bt::cerrarEditor()
{
    _depura ( "SubForm2Bt::cerrarEditor", 0 );
    QWidget *editor = mui_list->QAbstractItemView::indexWidget ( mui_list->currentIndex() );
    m_delegate->cerrarEditor ( editor );
    _depura ( "END SubForm2Bt::cerrarEditor", 0 );
    return 0;
}


/// ===============================================================
///  Tratamientos del Item Delegate
/// ===============================================================
///
/**
\param parent
**/
QSubForm2BtDelegate::QSubForm2BtDelegate ( QObject *parent = 0 ) : QItemDelegate ( parent ), PEmpresaBase()
{
    _depura ( "QSubForm2BtDelegate::QSubForm2BtDelegate", 0 );
    m_subform = ( SubForm2Bt * ) parent;
    installEventFilter ( this );
    _depura ( "END QSubForm2BtDelegate::QSubForm2BtDelegate", 0 );
}


///
/**
**/
QSubForm2BtDelegate::~QSubForm2BtDelegate()
{
    _depura ( "QSubForm2BtDelegate::~QSubForm2BtDelegate", 0 );
    _depura ( "END QSubForm2BtDelegate::~QSubForm2BtDelegate", 0 );
}


///
/**
\param parent
\param option
\param index
**/
QWidget *QSubForm2BtDelegate::createEditor ( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    _depura ( "QSubForm2BtDelegate::createEditor", 0 );
    SHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    _depura ( "QSubForm2BtDelegate::createEditor", 0, "CurrentColumn: " + QString::number ( index.column() ) );
    _depura ( "QSubForm2BtDelegate::createEditor", 0, "CurrentRow" + QString::number ( index.row() ) );

    if ( linea->nomcampo() == "desc" + m_subform->tableName() ) {
        QTextEditDelegate * editor = new QTextEditDelegate ( parent );
        editor->setObjectName ( "QTextEditDelegate" );
        return editor;

    } else if ( linea->nomcampo() == "cant" + m_subform->tableName()
                || linea->nomcampo() == "pvp" + m_subform->tableName()
                || linea->nomcampo() == "descuento" + m_subform->tableName()
                || linea->nomcampo() == "iva" + m_subform->tableName()
                || linea->nomcampo() == "reqeq" + m_subform->tableName() ) {

        QDoubleSpinBox2 * editor = new QDoubleSpinBox2 ( parent );
        editor->setMinimum ( -1000000 );
        editor->setMaximum ( 1000000 );
        return editor;

        /*
            } else if (linea->nomcampo() == "codigocompletoarticulo") {
                BusquedaArticuloDelegate *editor = new BusquedaArticuloDelegate(parent);
                editor->setEmpresaBase((Company *)m_subform->empresaBase());
                return editor;
            } else if (linea->nomcampo() == "desctipo_iva") {
                BusquedaTipoIVADelegate *editor = new BusquedaTipoIVADelegate(parent);
                editor->setEmpresaBase((Company *)m_subform->empresaBase());
                return editor;
            } else if (linea->nomcampo() == "nomtrabajador") {
                BusquedaTrabajadorDelegate *editor = new BusquedaTrabajadorDelegate(parent);
                editor->setEmpresaBase((Company *)m_subform->empresaBase());
                return editor;
            } else if (linea->nomcampo() == "nomalmacen") {
                BusquedaAlmacenDelegate *editor = new BusquedaAlmacenDelegate(parent);
                editor->setEmpresaBase((Company *)m_subform->empresaBase());
                return editor;
            } else  {
        //        QWidget *it = QItemDelegate::createEditor(parent, option, index);
        //        if (linea->tipo() == DBint) {
                    /// es un numero y se alinea a la derecha.
        //        } else {
         
        //        } // end if
        */
        return QItemDelegate::createEditor ( parent, option, index );
    } // end if
    _depura ( "END QSubForm2BtDelegate::createEditor", 0 );
}


///
/**
\param editor
\param model
\param index
\return
**/
void QSubForm2BtDelegate::setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    _depura ( "QSubForm2BtDelegate::setModelData", 0 );
    _depura ( "QSubForm2BtDelegate::setModelData", 0, "CurrentColumn: " + QString::number ( index.column() ) );
    _depura ( "QSubForm2BtDelegate::setModelData", 0, "CurrentRow: " + QString::number ( index.row() ) );

    /// Si la fila o columna pasadas son inv&aacute;lidas salimos.
    if ( index.column() < 0 || index.row() < 0 )
        return;

    SHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    if ( linea->nomcampo() == "desc" + m_subform->tableName() ) {
        QTextEditDelegate * textedit = qobject_cast<QTextEditDelegate *> ( editor );
        model->setData ( index, textedit->toPlainText() );
        return;

    } else if ( linea->nomcampo() == "cant" + m_subform->tableName()
                || linea->nomcampo() == "pvp" + m_subform->tableName()
                || linea->nomcampo() == "descuento" + m_subform->tableName()
                || linea->nomcampo() == "reqeq" + m_subform->tableName()
                || linea->nomcampo() == "iva" + m_subform->tableName() ) {

        QDoubleSpinBox2 * spinBox = static_cast<QDoubleSpinBox2*> ( editor );
        spinBox->interpretText();
        QString value = spinBox->text();
        model->setData ( index, value );
        /*
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
            } else if (linea->nomcampo() == "nomalmacen") {
                BusquedaAlmacenDelegate *comboBox = static_cast<BusquedaAlmacenDelegate*>(editor);
                QString value = comboBox->currentText();
                model->setData(index, value);
        */
    } else {
        QItemDelegate::setModelData ( editor, model, index );
    } // end if
    _depura ( "END QSubForm2BtDelegate::setModelData", 0 );
}


///
/**
\param editor
\param index
**/
void QSubForm2BtDelegate::setEditorData ( QWidget* editor, const QModelIndex& index ) const
{
    _depura ( "QSubForm2BtDelegate::setEditorData", 0 );
    _depura ( "QSubForm2BtDelegate::setEditorData", 0, "CurrentColumn: " + QString::number ( index.column() ) );
    _depura ( "QSubForm2BtDelegate::setEditorData", 0, "CurrentRow: " + QString::number ( index.row() ) );
    SHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    if ( linea->nomcampo() == "desc" + m_subform->tableName() ) {
        QString data = index.model() ->data ( index, Qt::DisplayRole ).toString();
        QTextEditDelegate *textedit = qobject_cast<QTextEditDelegate*> ( editor );
        textedit->setText ( data );

    } else if ( linea->nomcampo() == "cant" + m_subform->tableName()
                || linea->nomcampo() == "pvp" + m_subform->tableName()
                || linea->nomcampo() == "descuento" + m_subform->tableName()
                || linea->nomcampo() == "reqeq" + m_subform->tableName()
                || linea->nomcampo() == "iva" + m_subform->tableName() ) {

        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        QDoubleSpinBox2 *spinBox = static_cast<QDoubleSpinBox2*> ( editor );
        spinBox->setValue ( value.toDouble() );
        spinBox->selectAll();

        /*
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
            } else if (linea->nomcampo() == "nomalmacen") {
                QString value = index.model()->data(index, Qt::DisplayRole).toString();
                BusquedaAlmacenDelegate *comboBox = static_cast<BusquedaAlmacenDelegate*>(editor);
                comboBox->set(value);
        */
    } else {
        QItemDelegate::setEditorData ( editor, index );
    } // end if
    _depura ( "END QSubForm2BtDelegate::setEditorData", 0 );
}


///
/**
\param obj
\param event
\return
**/
bool QSubForm2BtDelegate::eventFilter ( QObject *obj, QEvent *event )
{
    /// Si es un release de tecla se hace la funcionalidad especificada.
    if ( event->type() == QEvent::KeyPress ) {
        _depura ( "QSubForm2BtDelegate::eventFilter", 0, obj->objectName() + " --> " + QString::number ( event->type() ) );
        QKeyEvent *keyEvent = static_cast<QKeyEvent *> ( event );
        int key = keyEvent->key();
        _depura ( "QSubForm2BtDelegate::key = : ", 0, QString::number ( key ) );
        Qt::KeyboardModifiers mod = keyEvent->modifiers();
        /// Anulamos el caso de una pulsacion de tabulador o de enter
        switch ( key ) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            if ( obj->objectName() == "QTextEditDelegate" ) {
                obj->event ( event );
                return TRUE;
            } // end if
        case Qt::Key_Tab:
            return TRUE;
        } // end switch
        return QItemDelegate::eventFilter ( obj, event );
    } // end if

    if ( event->type() == QEvent::KeyRelease ) {
        _depura ( "QSubForm2BtDelegate::eventFilter", 0, obj->objectName() + " --> " + QString::number ( event->type() ) );
        QKeyEvent *keyEvent = static_cast<QKeyEvent *> ( event );
        int key = keyEvent->key();
        _depura ( "QSubForm2BtDelegate::key = : ", 0, QString::number ( key ) );
        Qt::KeyboardModifiers mod = keyEvent->modifiers();
        /// En caso de pulsacion de un retorno de carro o similar procesamos por nuestra cuenta.
        switch ( key ) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            if ( obj->objectName() == "QTextEditDelegate" ) {
                obj->event ( event );
                return TRUE;
            } // end if
        case Qt::Key_Tab:
            QApplication::sendEvent ( m_subform->mui_list, event );
            return TRUE;
        } // end switch
        return QItemDelegate::eventFilter ( obj, event );
    } // end if

    return QItemDelegate::eventFilter ( obj, event );
}


///
/**
\param editor
\return
**/
int QSubForm2BtDelegate::cerrarEditor ( QWidget *editor )
{
    _depura ( "QSubForm2BtDelegate::cerrarEditor", 0 );
    emit closeEditor ( editor, QAbstractItemDelegate::NoHint );
    _depura ( "END QSubForm2BtDelegate::cerrarEditor", 0 );
    return 0;
}

