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

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMenu>
#include <QtGui/QKeyEvent>
#include <QtCore/QEvent>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QAbstractItemView>

#include "btsubform.h"
#include "bltexteditdelegate.h"
#include "blplugins.h"


///
/**
\param parent
**/
BtSubForm::BtSubForm ( QWidget *parent ) : BlSubForm ( parent )
{
    BL_FUNC_DEBUG
    setDelete ( true );
    m_delegate = new BtSubFormDelegate ( this );
    mui_list->setItemDelegate ( m_delegate );
    mdb_idcliente = "";
    
}


///
/**
**/
BtSubForm::~BtSubForm()
{
    BL_FUNC_DEBUG
    delete m_delegate;
    
}


///
/**
\param query
**/
void BtSubForm::load ( QString query )
{
    BL_FUNC_DEBUG
    BlSubForm::load ( query );
    
}


///
/**
\param row
\param col
\return
**/
void BtSubForm::pressedAsterisk ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp )
{
    BL_FUNC_DEBUG
    
}


///
/**
\param row
\param col
**/
void BtSubForm::on_mui_list_pressedSlash ( int row, int col )
{
    BL_FUNC_DEBUG
    BlDbSubFormField *camp = ( BlDbSubFormField * ) item ( row, col );
    QString text = blTextEditor ( camp->text() );
    camp->set ( text );
    
}


///
/**
\param row
\param col
**/
void BtSubForm::on_mui_list_pressedMinus ( int row, int col )
{
    BL_FUNC_DEBUG
    
}


///
/**
\param row
\param col
\return
**/
void BtSubForm::on_mui_list_cellChanged ( int row, int col )
{
    BlDebug::blDebug ( "BtSubForm::on_mui_list_cellChanged", 0, QString::number ( row ) + " " + QString::number ( col ) );
    
}


///
/**
\param id
\return
**/
void BtSubForm::setIdCliente ( QString id )
{
    BlDebug::blDebug ( "BtSubForm::setIdCliente", 0, id );

    /// En la primera carga no hay reajustes, pero si actualizacion del cliente.
    if ( mdb_idcliente  == "" ) {
        mdb_idcliente = id;
        return;
    } // end if

    /// En las cargas sucesivas si el idcliente no ha cambiado no se hace nada
    if ( mdb_idcliente == id ) return;

    /// Reseteamos los valores.
    for ( int i = 0; i < rowCount() - 1; i++ ) {
        BlDbSubFormRecord *rec = lineaat ( i );
        rec->setDbValue ( "iva" + m_tablename, "0" );
        rec->setDbValue ( "reqeq" + m_tablename, "0" );
    } // end for

    mdb_idcliente = id;

    /// Si el idcliente no existe salimos.
    if ( id == "" ) {
	
        return;
    } // end if

    BlDbRecordSet *curcliente = mainCompany() ->loadQuery ( "SELECT recargoeqcliente, regimenfiscalcliente FROM cliente WHERE idcliente = " + mdb_idcliente );

    if ( !curcliente->eof() ) {
        /// Cuando se cambia el cliente se deben recalcular las lineas por si hay Recargo Equivalente
        for ( int i = 0; i < rowCount() - 1; i++ ) {
            BlDbSubFormRecord *rec = lineaat ( i );
            BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM articulo WHERE idarticulo = " + rec->dbValue ( "idarticulo" ) );
            BlDbRecordSet *cur1 = mainCompany() ->loadQuery ( "SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->value( "idtipo_iva" ) + " ORDER BY fechatasa_iva LIMIT 1" );
            if ( !cur->eof() ) {

                if ( curcliente->value( "regimenfiscalcliente" ) == "Normal" ) {
                    rec->setDbValue ( "iva" + m_tablename, cur1->value( "porcentasa_iva" ) );
                } // end if

                if ( curcliente->value( "recargoeqcliente" ) == "t" ) {
                    rec->setDbValue ( "reqeq" + m_tablename, cur1->value( "porcentretasa_iva" ) );
                } // end if

            } // end if
            delete cur1;
            delete cur;
        } // end for
    } // end if
    delete curcliente;
    
}


///
/**
\param id
\return
**/
void BtSubForm::setIdProveedor ( QString id )
{
    BlDebug::blDebug ( "BtSubForm::setIdProveedor", 0, id );

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
        BlDbSubFormRecord *rec = lineaat ( i );
        rec->setDbValue ( "iva" + m_tablename, "0" );
        rec->setDbValue ( "reqeq" + m_tablename, "0" );
    } // end for

    BlDbRecordSet *curproveedor = mainCompany() ->loadQuery ( "SELECT recargoeqproveedor, regimenfiscalproveedor FROM proveedor WHERE idproveedor=" + mdb_idproveedor );
    if ( !curproveedor->eof() ) {
        /// Cuando se cambia el cliente se deben recalcular las lineas por si hay Recargo Equivalente
        for ( int i = 0; i < rowCount() - 1; i++ ) {
            BlDbSubFormRecord *rec = lineaat ( i );
            BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM articulo WHERE idarticulo = " + rec->dbValue ( "idarticulo" ) );
            BlDbRecordSet *cur1 = mainCompany() ->loadQuery ( "SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->value( "idtipo_iva" ) + " ORDER BY fechatasa_iva LIMIT 1" );
            if ( !cur->eof() ) {
                if ( curproveedor->value( "regimenfiscalproveedor" ) == "Normal" ) {
                    rec->setDbValue ( "iva" + m_tablename, cur1->value( "porcentasa_iva" ) );
                } // end if
                if ( curproveedor->value( "recargoeqproveedor" ) == "t" ) {
                    rec->setDbValue ( "reqeq" + m_tablename, cur1->value( "porcentretasa_iva" ) );
                } // end if

            } // end if
            delete cur1;
            delete cur;
        } // end for
    } // end if
    delete curproveedor;
    
}


///
/**
\param c
**/
void BtSubForm::setMainCompany ( BlMainCompany *c )
{
    BL_FUNC_DEBUG
    BlSubForm::setMainCompany ( c );
    m_delegate->setMainCompany ( c );
    
}


///
/**
\return
**/
int BtSubForm::cerrarEditor()
{
    BL_FUNC_DEBUG
    QWidget *editor = mui_list->QAbstractItemView::indexWidget ( mui_list->currentIndex() );
    m_delegate->cerrarEditor ( editor );
    
    return 0;
}


/// ===============================================================
///  Tratamientos del Item Delegate
/// ===============================================================
///
/**
\param parent
**/
BtSubFormDelegate::BtSubFormDelegate ( QObject *parent = 0 ) : QStyledItemDelegate ( parent ), BlMainCompanyPointer()
{
    BL_FUNC_DEBUG
    m_subform = ( BtSubForm * ) parent;
    installEventFilter ( this );
    
}


///
/**
**/
BtSubFormDelegate::~BtSubFormDelegate()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param parent
\param option
\param index
**/
QWidget *BtSubFormDelegate::createEditor ( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    BL_FUNC_DEBUG
    BlSubFormHeader *linea;
    linea = m_subform->headerList() ->at ( index.column() );
    BlDebug::blDebug ( "BtSubFormDelegate::createEditor", 0, "CurrentColumn: " + QString::number ( index.column() ) );
    BlDebug::blDebug ( "BtSubFormDelegate::createEditor", 0, "CurrentRow" + QString::number ( index.row() ) );

    if ( linea->fieldName() == "desc" + m_subform->tableName() ) {
        BlTextEditDelegate * editor = new BlTextEditDelegate ( parent );
        editor->setObjectName ( "BlTextEditDelegate" );
        return editor;

    } else if ( linea->fieldName() == "cant" + m_subform->tableName()
                || linea->fieldName() == "pvp" + m_subform->tableName()
                || linea->fieldName() == "descuento" + m_subform->tableName()
                || linea->fieldName() == "iva" + m_subform->tableName()
                || linea->fieldName() == "reqeq" + m_subform->tableName() ) {

        BlDoubleSpinBox * editor = new BlDoubleSpinBox ( parent );
        editor->setMinimum ( -1000000 );
        editor->setMaximum ( 1000000 );
        editor->setDecimals(linea->numericPrecision());
        return editor;
    } // end if
    return QStyledItemDelegate::createEditor ( parent, option, index );
    
   return 0;
}


///
/**
\param editor
\param model
\param index
\return
**/
void BtSubFormDelegate::setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BtSubFormDelegate::setModelData", 0, "CurrentColumn: " + QString::number ( index.column() ) );
    BlDebug::blDebug ( "BtSubFormDelegate::setModelData", 0, "CurrentRow: " + QString::number ( index.row() ) );

    /// Si la fila o columna pasadas son inv&aacute;lidas salimos.
    if ( index.column() < 0 || index.row() < 0 )
        return;

    BlSubFormHeader *linea;
    linea = m_subform->headerList() ->at ( index.column() );
    if ( linea->fieldName() == "desc" + m_subform->tableName() ) {
        BlTextEditDelegate * textedit = qobject_cast<BlTextEditDelegate *> ( editor );
        model->setData ( index, textedit->toPlainText() );
        return;

    } else if ( linea->fieldName() == "cant" + m_subform->tableName()
                || linea->fieldName() == "pvp" + m_subform->tableName()
                || linea->fieldName() == "descuento" + m_subform->tableName()
                || linea->fieldName() == "reqeq" + m_subform->tableName()
                || linea->fieldName() == "iva" + m_subform->tableName() ) {

        BlDoubleSpinBox * spinBox = static_cast<BlDoubleSpinBox*> ( editor );
        spinBox->interpretText();
        QString value = spinBox->text();
        model->setData ( index, value );
        /*
            } else if (linea->fieldName() == "codigocompletoarticulo") {
                BfBuscarArticuloDelegate *comboBox = static_cast<BfBuscarArticuloDelegate*>(editor);
                QString value = comboBox->currentText();
                value = value.left(value.indexOf(".-"));
                model->setData(index, value);
            } else if (linea->fieldName() == "desctipo_iva") {
                BfBuscarTipoIVADelegate *comboBox = static_cast<BfBuscarTipoIVADelegate*>(editor);
                QString value = comboBox->currentText();
                model->setData(index, value);
            } else if (linea->fieldName() == "nomtrabajador") {
                BfBuscarTrabajadorDelegate *comboBox = static_cast<BfBuscarTrabajadorDelegate*>(editor);
                QString value = comboBox->currentText();
                model->setData(index, value);
            } else if (linea->fieldName() == "nomalmacen") {
                BfBuscarAlmacenDelegate *comboBox = static_cast<BfBuscarAlmacenDelegate*>(editor);
                QString value = comboBox->currentText();
                model->setData(index, value);
        */
    } else {
        QStyledItemDelegate::setModelData ( editor, model, index );
    } // end if
    
}


///
/**
\param editor
\param index
**/
void BtSubFormDelegate::setEditorData ( QWidget* editor, const QModelIndex& index ) const
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BtSubFormDelegate::setEditorData", 0, "CurrentColumn: " + QString::number ( index.column() ) );
    BlDebug::blDebug ( "BtSubFormDelegate::setEditorData", 0, "CurrentRow: " + QString::number ( index.row() ) );
    BlSubFormHeader *linea;
    linea = m_subform->headerList() ->at ( index.column() );
    if ( linea->fieldName() == "desc" + m_subform->tableName() ) {
        QString data = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BlTextEditDelegate *textedit = qobject_cast<BlTextEditDelegate*> ( editor );
        textedit->setText ( data );

    } else if ( linea->fieldName() == "cant" + m_subform->tableName()
                || linea->fieldName() == "pvp" + m_subform->tableName()
                || linea->fieldName() == "descuento" + m_subform->tableName()
                || linea->fieldName() == "reqeq" + m_subform->tableName()
                || linea->fieldName() == "iva" + m_subform->tableName() ) {

        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BlDoubleSpinBox *spinBox = static_cast<BlDoubleSpinBox*> ( editor );
        spinBox->setValue ( value.toDouble() );
        spinBox->selectAll();

        /*
            } else if (linea->fieldName() == "codigocompletoarticulo") {
                QString value = index.model()->data(index, Qt::DisplayRole).toString();
                BfBuscarArticuloDelegate *comboBox = static_cast<BfBuscarArticuloDelegate*>(editor);
                comboBox->addItem(value);
            } else if (linea->fieldName() == "desctipo_iva") {
                QString value = index.model()->data(index, Qt::DisplayRole).toString();
                BfBuscarTipoIVADelegate *comboBox = static_cast<BfBuscarTipoIVADelegate*>(editor);
                comboBox->set(value);
            } else if (linea->fieldName() == "nomtrabajador") {
                QString value = index.model()->data(index, Qt::DisplayRole).toString();
                BfBuscarTrabajadorDelegate *comboBox = static_cast<BfBuscarTrabajadorDelegate*>(editor);
                comboBox->set(value);
            } else if (linea->fieldName() == "nomalmacen") {
                QString value = index.model()->data(index, Qt::DisplayRole).toString();
                BfBuscarAlmacenDelegate *comboBox = static_cast<BfBuscarAlmacenDelegate*>(editor);
                comboBox->set(value);
        */
    } else {
        QStyledItemDelegate::setEditorData ( editor, index );
    } // end if
    
}


///
/**
\param obj
\param event
\return
**/
bool BtSubFormDelegate::eventFilter ( QObject *obj, QEvent *event )
{
    /// Si es un release de tecla se hace la funcionalidad especificada.
    if ( event->type() == QEvent::KeyPress ) {
        BlDebug::blDebug ( "BtSubFormDelegate::eventFilter", 0, obj->objectName() + " --> " + QString::number ( event->type() ) );
        QKeyEvent *keyEvent = static_cast<QKeyEvent *> ( event );
        int key = keyEvent->key();
        BlDebug::blDebug ( "BtSubFormDelegate::key = : ", 0, QString::number ( key ) );
        Qt::KeyboardModifiers mod = keyEvent->modifiers();
        /// Anulamos el caso de una pulsacion de tabulador o de enter
        switch ( key ) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            if ( obj->objectName() == "BlTextEditDelegate" ) {
                obj->event ( event );
                return true;
            } // end if
        case Qt::Key_Tab:
            return true;
        } // end switch
        return QStyledItemDelegate::eventFilter ( obj, event );
    } // end if

    if ( event->type() == QEvent::KeyRelease ) {
        BlDebug::blDebug ( "BtSubFormDelegate::eventFilter", 0, obj->objectName() + " --> " + QString::number ( event->type() ) );
        QKeyEvent *keyEvent = static_cast<QKeyEvent *> ( event );
        int key = keyEvent->key();
        BlDebug::blDebug ( "BtSubFormDelegate::key = : ", 0, QString::number ( key ) );
        Qt::KeyboardModifiers mod = keyEvent->modifiers();
        /// En caso de pulsacion de un retorno de carro o similar procesamos por nuestra cuenta.
        switch ( key ) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            if ( obj->objectName() == "BlTextEditDelegate" ) {
                obj->event ( event );
                return true;
            } // end if
        case Qt::Key_Tab:
            QApplication::sendEvent ( m_subform->mui_list, event );
            return true;
        } // end switch
        return QStyledItemDelegate::eventFilter ( obj, event );
    } // end if

    return QStyledItemDelegate::eventFilter ( obj, event );
}


///
/**
\param editor
\return
**/
int BtSubFormDelegate::cerrarEditor ( QWidget *editor )
{
    BL_FUNC_DEBUG
    emit closeEditor ( editor, QAbstractItemDelegate::NoHint );
    
    return 0;
}

