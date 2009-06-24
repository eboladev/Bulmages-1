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

#include "bfsubform.h"
#include "blfunctions.h"
#include "bfbuscartipoiva.h"
#include "bltexteditdelegate.h"
#include "bfbuscartrabajador.h"
#include "bfbuscaralmacen.h"
#include "bfbuscararticulo.h"
#include "blplugins.h"


/// Clase de subformularios especificos.
/**
\param parent El widget propietario del subformulario.
**/
BfSubForm::BfSubForm ( QWidget *parent ) : BlSubForm ( parent )
{
    _depura ( "BfSubForm::BfSubForm", 0 );
    setDelete ( TRUE );
    m_delegate = new BfSubFormDelegate ( this );
    mui_list->setItemDelegate ( m_delegate );
    mdb_idcliente = "";

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "BfSubForm_BfSubForm", this );
    if ( res != 0 ) {
        _depura ( "END BfSubForm::BfSubForm", 0, "Salida por plugins" );
        return;
    } // end if

    _depura ( "END BfSubForm::BfSubForm", 0 );
}


/// Destructor de la clase.
/**
**/
BfSubForm::~BfSubForm()
{
    _depura ( "BfSubForm::~BfSubForm", 0 );
    delete m_delegate;
    _depura ( "END BfSubForm::~BfSubForm", 0 );
}


/// Carga un Query
/**
\param query Una consulta SQL valida.
**/
void BfSubForm::cargar ( QString query )
{
    _depura ( "BfSubForm::cargar", 0, query );
    BlSubForm::cargar ( query );
    _depura ( "END BfSubForm::cargar", 0 );
}


///
/**
\param row
\param col
\return
**/
void BfSubForm::pressedAsterisk ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp )
{
    _depura ( "BfSubForm::pressedAsterisk", 0 );

    /// Establezco las variables de clase para que los plugins puedan operar.
    m_registrolinea = rec;
    m_campoactual = camp;

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "BfSubForm_pressedAsterisk", this );
    if ( res != 0 ) {
        _depura ( "END BfSubForm::pressedAsterisk", 0, "Salida por plugins" );
        return;
    } // end if




    _depura ( "END BfSubForm::pressedAsterisk", 0 );
}


///
/**
\param row
\param col
**/
void BfSubForm::pressedSlash ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp )
{
    _depura ( "BfSubForm::pressedSlash", 0 );
    QString text = editaTexto ( camp->text() );
    camp->set ( text );
    _depura ( "END BfSubForm::pressedSlash", 0 );
}


///
/**
\param row
\param col
**/
void BfSubForm::pressedMinus ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp )
{
    _depura ( "BfSubForm::pressedMinus", 0 );

    /// Como no invoca llamadas al listado de articulos lo podemos dejar aqui aunque en pluginbf_articulo estaria mucho mejor.

    if ( !rec->exists ( "idarticulo" ) ) {
        _depura ( "END BfSubForm::pressedMinus", 0, "No hay un idarticulo" );
        return;
    } // end if

    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM articulo WHERE idarticulo = " + rec->dbValue ( "idarticulo" ) );
    if ( !cur ) {
        _depura ( "END BfSubForm::pressedMinus", 0, "No hay un idarticulo" );
        return;
    } // end if

    if ( !cur->eof() ) {
        rec->setDbValue ( camp->nomcampo(), cur->valor ( "obserarticulo" ) );
    } // end if
    delete cur;

    _depura ( "END BfSubForm::pressedMinus", 0 );
}


///
/**
\param row
\param col
\return
**/
void BfSubForm::editFinished ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp )
{
    _depura ( "BfSubForm::editFinished", 0, QString::number ( row ) + " " + QString::number ( col ) );

    m_registrolinea = rec;
    m_campoactual = camp;

    BlDbRecordSet *cur2 = NULL;
    BlDbRecordSet *cur = NULL;
    BlDbRecordSet *cur1 = NULL;

    /// Lanzamos el manejador de la SuperClase para que se atiendan las opciones mas genericas.
    BlSubForm::editFinished ( row, col, rec, camp );

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "BfSubForm_on_mui_list_editFinished", this );
    if ( res != 0 ) {
        _depura ( "END BfSubForm::editFinished", 0, "Salida por plugins" );
        return;
    } // end if

    if ( camp->nomcampo() == "desctipo_iva" ) {
        cur = mainCompany() ->loadQuery ( "SELECT * FROM tipo_iva WHERE desctipo_iva = '" + camp->text() + "'" );
        if ( !cur->eof() ) {
            rec->setDbValue ( "idtipo_iva", cur->valor ( "idtipo_iva" ) );
        } // end if
    } // end if

    if ( camp->nomcampo() == "nomtrabajador" ) {
        cur = mainCompany() ->loadQuery ( "SELECT * FROM trabajador WHERE apellidostrabajador ||', '||nomtrabajador = '" + camp->text() + "'" );
        if ( !cur->eof() ) {
            rec->setDbValue ( "idtrabajador", cur->valor ( "idtrabajador" ) );
        } // end if
    } // end if

    if ( camp->nomcampo() == "nomalmacen" ) {
        cur = mainCompany() ->loadQuery ( "SELECT * FROM almacen WHERE nomalmacen ||', '||codigoalmacen = '" + camp->text() + "'" );
        if ( !cur->eof() ) {
            rec->setDbValue ( "idalmacen", cur->valor ( "idalmacen" ) );
        } // end if
    } // end if

    if ( camp->nomcampo() == "codigocompletoarticulo" ) {
        cur = mainCompany() ->loadQuery ( "SELECT * FROM articulo WHERE codigocompletoarticulo = '" + camp->text() + "'" );
        if ( !cur->eof() ) {
            rec->setDbValue ( "idarticulo", cur->valor ( "idarticulo" ) );
            rec->setDbValue ( "codigocompletoarticulo", cur->valor ( "codigocompletoarticulo" ) );
            rec->setDbValue ( "nomarticulo", cur->valor ( "nomarticulo" ) );
            if ( m_tablename == "lpresupuesto"
                    || m_tablename == "lpedidoproveedor"
                    || m_tablename == "lpedidocliente"
                    || m_tablename == "lalbaranp"
                    || m_tablename == "lfacturap"
                    || m_tablename == "lalbaran"
                    || m_tablename == "lfactura" ) {
                rec->setDbValue ( "desc" + m_tablename, cur->valor ( "nomarticulo" ) );
                rec->setDbValue ( "cant" + m_tablename, "1.00" );
                rec->setDbValue ( "descuento" + m_tablename, "0.00" );
                calculaPVP ( rec );
            } // end if
        } else {
            mensajeAviso ( _ ( "El codigo del articulo no existe.\nATENCION: No se guadara bien el documento hasta que sea valido." ) );
            /// \TODO Preparo para revisarlo para la version 0.11.2 o posterior.
            /** El codigo introducido no existe en la base de datos.
            Aqui se tiene que dar la siguiente informacion:
            - "El uso de codigos de articulos inexistentes esta prohibido. Corrija
            esta situacion para poder guardar el documento."
            Se tiene que informar de las siguientes opciones:
            - Crear articulo con ese codigo.
            - Mostrar el listado de articulos para seleccionar uno valido.
            - Editar codigo para corregirlo.
            **/
            /*
            QMessageBox msgBox(this);
            msgBox.setIcon(QMessageBox::Question);
            msgBox.setWindowTitle(_("Articulo inexistente"));
            msgBox.setText(_("El uso de codigos de articulos inexistentes esta prohibido.\nCorrija esta situacion para poder guardar el documento."));
            QPushButton *botonCrear = msgBox.addButton(_("&Crear articulo"), QMessageBox::ActionRole);
            QPushButton *botonSeleccionar = msgBox.addButton(_("&Seleccionar articulo"), QMessageBox::ActionRole);
            QPushButton *botonEditar = msgBox.addButton(_("&Editar codigo"), QMessageBox::ActionRole);
            msgBox.setDefaultButton(botonCrear);
            msgBox.setEscapeButton(botonEditar);

            msgBox.exec();

            if (msgBox.clickedButton() == botonCrear) {
             mensajeInfo("Crear", this);

            } else if (msgBox.clickedButton() == botonSeleccionar) {
             mensajeInfo("Seleccionar", this);

            } else if (msgBox.clickedButton() == botonEditar) {


            }
            */

            delete cur;
            _depura ( "END BfSubForm::editFinished", 0, "Articulo inexistente" );
            return;
        } // end if


        if ( cur != NULL )
            delete cur;
    } // end if

    /*
        BlSubForm::on_mui_list_cellChanged ( row, col );
    */
    /// Refrescamos el registro.
    rec->refresh();
    _depura ( "END BfSubForm::editFinished", 0 );
}


///
/**
\param id
\return
**/
void BfSubForm::setIdCliente ( QString id )
{
    _depura ( "BfSubForm::setIdCliente", 0, id );

    /// En la primera carga no hay reajustes, pero si actualizacion del cliente.
    if ( mdb_idcliente  == "" ) {
        mdb_idcliente = id;
        _depura ( "END BfSubForm::setIdCliente", 0, "Primera carga" );
        return;
    } // end if

    /// En las cargas sucesivas si el idcliente no ha cambiado no se hace nada
    if ( mdb_idcliente == id ) {
        _depura ( "END BfSubForm::setIdCliente", 0, "cliente sin cambios" );
        return;
    } // end if

    /// Reseteamos los valores.
    for ( int i = 0; i < rowCount() - 1; i++ ) {
        BlDbSubFormRecord *rec = lineaat ( i );
        rec->setDbValue ( "iva" + m_tablename, "0" );
        rec->setDbValue ( "reqeq" + m_tablename, "0" );
    } // end for

    mdb_idcliente = id;

    /// Si el idcliente no existe salimos.
    if ( id == "" ) {
        _depura ( "END BfSubForm::setIdCliente", 0, "idcliente invalido" );
        return;
    } // end if

    BlDbRecordSet *curcliente = mainCompany() ->loadQuery ( "SELECT recargoeqcliente, regimenfiscalcliente FROM cliente WHERE idcliente = " + mdb_idcliente );

    if ( !curcliente->eof() ) {
        /// Cuando se cambia el cliente se deben recalcular las lineas por si hay Recargo Equivalente
        for ( int i = 0; i < rowCount() - 1; i++ ) {
            BlDbSubFormRecord *rec = lineaat ( i );
            BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM articulo WHERE idarticulo = " + rec->dbValue ( "idarticulo" ) );
            BlDbRecordSet *cur1 = mainCompany() ->loadQuery ( "SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->valor ( "idtipo_iva" ) + " ORDER BY fechatasa_iva LIMIT 1" );
            if ( !cur->eof() ) {

                if ( curcliente->valor ( "regimenfiscalcliente" ) == "Normal" ) {
                    rec->setDbValue ( "iva" + m_tablename, cur1->valor ( "porcentasa_iva" ) );
                } // end if

                if ( curcliente->valor ( "recargoeqcliente" ) == "t" ) {
                    rec->setDbValue ( "reqeq" + m_tablename, cur1->valor ( "porcentretasa_iva" ) );
                } // end if

            } // end if
            delete cur1;
            delete cur;
        } // end for
    } // end if
    delete curcliente;
    _depura ( "END BfSubForm::setIdCliente", 0 );
}


///
/**
\param id
\return
**/
void BfSubForm::setIdProveedor ( QString id )
{
    _depura ( "BfSubForm::setIdProveedor", 0, id );

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

    /// Si el idproveedor no existe salimos.
    if ( id == "" ) {
        _depura ( "END BfSubForm::setIdProveedor", 0, "idproveedor invalido" );
        return;
    } // end if

    BlDbRecordSet *curproveedor = mainCompany() ->loadQuery ( "SELECT recargoeqproveedor, regimenfiscalproveedor FROM proveedor WHERE idproveedor=" + mdb_idproveedor );

    if ( !curproveedor->eof() ) {
        /// Cuando se cambia el cliente se deben recalcular las lineas por si hay Recargo Equivalente
        for ( int i = 0; i < rowCount() - 1; i++ ) {
            BlDbSubFormRecord *rec = lineaat ( i );
            BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM articulo WHERE idarticulo = " + rec->dbValue ( "idarticulo" ) );
            BlDbRecordSet *cur1 = mainCompany() ->loadQuery ( "SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->valor ( "idtipo_iva" ) + " ORDER BY fechatasa_iva LIMIT 1" );
            if ( !cur->eof() ) {
                if ( curproveedor->valor ( "regimenfiscalproveedor" ) == "Normal" ) {
                    rec->setDbValue ( "iva" + m_tablename, cur1->valor ( "porcentasa_iva" ) );
                } // end if
                if ( curproveedor->valor ( "recargoeqproveedor" ) == "t" ) {
                    rec->setDbValue ( "reqeq" + m_tablename, cur1->valor ( "porcentretasa_iva" ) );
                } // end if

            } // end if
            delete cur1;
            delete cur;
        } // end for
    } // end if
    delete curproveedor;
    _depura ( "END BfSubForm::setIdProveedor", 0 );
}


///
/**
\param id
**/
void BfSubForm::setIdAlmacen ( QString id )
{
    _depura ( "BfSubForm::setIdAlmacen", 0 );
    m_idAlmacen = id;
    _depura ( "END BfSubForm::setIdAlmacen", 0 );
}


///
/**
\param c
**/
void BfSubForm::setMainCompany ( BlMainCompany *c )
{
    _depura ( "BfSubForm::setcompany", 0 );
    BlSubForm::setMainCompany ( c );
    m_delegate->setMainCompany ( c );
    _depura ( "END BfSubForm::setcompany", 0 );
}


///
/**
\return
**/
int BfSubForm::cerrarEditor()
{
    _depura ( "BfSubForm::cerrarEditor", 0 );
    QWidget *editor = mui_list->QAbstractItemView::indexWidget ( mui_list->currentIndex() );
    m_delegate->cerrarEditor ( editor );
    _depura ( "END BfSubForm::cerrarEditor", 0 );
    return 0;
}


/// Devuelve el identificador de cliente, si es un subformulario de cliente.
/**
\return El identificador de cliente
**/
QString BfSubForm::idcliente()
{
    return mdb_idcliente;
}


/// Devuelve el identificador de proveedor, si es un subformulario de proveedor.
/**
\return El identificador de proveedor
**/
QString BfSubForm::idproveedor()
{
    return mdb_idproveedor;
}


///
/**
**/
QString BfSubForm::idAlmacen ()
{
    _depura ( "BfSubForm::idAlmacen", 0 );
    _depura ( "END BfSubForm::idAlmacen", 0 );
    return m_idAlmacen;
}


/// ===============================================================
///  Tratamientos del Item Delegate
/// ===============================================================
///
/**
\param parent
**/
BfSubFormDelegate::BfSubFormDelegate ( QObject *parent = 0 ) : BlSubFormDelegate ( parent )
{
    _depura ( "BfSubFormDelegate::BfSubFormDelegate", 0 );
//    m_subform = ( BlSubForm * ) parent;
//    installEventFilter ( this );
    _depura ( "END BfSubFormDelegate::BfSubFormDelegate", 0 );
}


///
/**
**/
BfSubFormDelegate::~BfSubFormDelegate()
{
    _depura ( "BfSubFormDelegate::~BfSubFormDelegate", 0 );
    _depura ( "END BfSubFormDelegate::~BfSubFormDelegate", 0 );
}


///
/**
\param parent
\param option
\param index
**/
QWidget *BfSubFormDelegate::createEditor ( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    _depura ( "BfSubFormDelegate::createEditor", 0, "CurrentColumn: " + QString::number ( index.column() ) + "CurrentRow" + QString::number ( index.row() )  );
    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );

    if ( linea->nomcampo() == "desc" + m_subform->tableName() ) {
        BlTextEditDelegate * editor = new BlTextEditDelegate ( parent );
        editor->setObjectName ( "BlTextEditDelegate" );
        _depura ( "END BfSubFormDelegate::createEditor", 0, "QTextEdit" );
        return editor;
    } else if ( linea->nomcampo() == "cant" + m_subform->tableName()
                || linea->nomcampo() == "pvp" + m_subform->tableName()
                || linea->nomcampo() == "descuento" + m_subform->tableName()
                || linea->nomcampo() == "iva" + m_subform->tableName()
                || linea->nomcampo() == "reqeq" + m_subform->tableName() ) {

        BlDoubleSpinBox * editor = new BlDoubleSpinBox ( parent );
        editor->setMinimum ( -10000000 );
        editor->setMaximum ( 10000000 );
        _depura ( "END BfSubFormDelegate::createEditor", 0, "QSPinBox" );
        return editor;
/*
    } else if ( linea->nomcampo() == "codigocompletoarticulo" ) {
        BfBuscarArticuloDelegate * editor = new BfBuscarArticuloDelegate ( parent );
        editor->setMainCompany ( ( BfCompany * ) m_subform->mainCompany() );
        _depura ( "END BfSubFormDelegate::createEditor", 0, "BfBuscarArticulo" );
        return editor;
*/
    } else if ( linea->nomcampo() == "desctipo_iva" ) {
        BfBuscarTipoIVADelegate * editor = new BfBuscarTipoIVADelegate ( parent );
        editor->setMainCompany ( ( BfCompany * ) m_subform->mainCompany() );
        _depura ( "END BfSubFormDelegate::createEditor", 0, "BfBuscarTipoIVA" );
        return editor;
    } else if ( linea->nomcampo() == "nomtrabajador" && m_subform->tableName() != "trabajador" ) {
        BfBuscarTrabajadorDelegate * editor = new BfBuscarTrabajadorDelegate ( parent );
        editor->setMainCompany ( ( BfCompany * ) m_subform->mainCompany() );
        _depura ( "END BfSubFormDelegate::createEditor", 0, "BfBuscarTrabajadorDelegate" );
        return editor;
    } else if ( linea->nomcampo() == "nomalmacen"  && m_subform->tableName() != "almacen" ) {
        BfBuscarAlmacenDelegate * editor = new BfBuscarAlmacenDelegate ( parent );
        editor->setMainCompany ( ( BfCompany * ) m_subform->mainCompany() );
        _depura ( "END BfSubFormDelegate::createEditor", 0, "BfBuscarAlmacenDelegate" );
        return editor;
    } else  {
        _depura ( "END BfSubFormDelegate::createEditor", 0, "Default Editor" );
        return BlSubFormDelegate::createEditor ( parent, option, index );
    } // end if
}


///
/**
\param editor
\param model
\param index
\return
**/
void BfSubFormDelegate::setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    _depura ( "BfSubFormDelegate::setModelData", 0, "CurrentColumn: " + QString::number ( index.column() ) + "CurrentRow: " + QString::number ( index.row() ) );

    /// Si la fila o columna pasadas son inv&aacute;lidas salimos.
    if ( index.column() < 0 || index.row() < 0 ) {
        _depura ( "END BfSubFormDelegate::setModelData", 0, "Fila o columna invalida" );
        return;
    } // end if

    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    if ( linea->nomcampo() == "desc" + m_subform->tableName() ) {
        BlTextEditDelegate * textedit = qobject_cast<BlTextEditDelegate *> ( editor );
        model->setData ( index, textedit->toPlainText() );
    } else if ( linea->nomcampo() == "cant" + m_subform->tableName()
                || linea->nomcampo() == "pvp" + m_subform->tableName()
                || linea->nomcampo() == "descuento" + m_subform->tableName()
                || linea->nomcampo() == "reqeq" + m_subform->tableName()
                || linea->nomcampo() == "iva" + m_subform->tableName() ) {

        BlDoubleSpinBox * spinBox = static_cast<BlDoubleSpinBox*> ( editor );
        spinBox->interpretText();
        QString value = spinBox->text();
        QString valueanterior = m_subform->lista()->at ( index.row() )->dbValue ( "cant" + m_subform->tableName() );

        model->setData ( index, value );

        /// Dispara senyales:
        if ( linea->nomcampo() == "cant" + m_subform->tableName() ) {
            /// Mira si se ha cambiado la cantidad o es la misma que habia.
            if ( valueanterior.toDouble() != value.toDouble() ) {
                emit cant_changed ( m_subform->lista()->at ( index.row() ) );
            } // end if
        } // end if
/*
    } else if ( linea->nomcampo() == "codigocompletoarticulo" ) {
        BfBuscarArticuloDelegate * comboBox = static_cast<BfBuscarArticuloDelegate*> ( editor );
        QString value = comboBox->currentText();
        value = value.left ( value.indexOf ( ".-" ) );
        model->setData ( index, value );
*/
    } else if ( linea->nomcampo() == "desctipo_iva" ) {
        BfBuscarTipoIVADelegate * comboBox = static_cast<BfBuscarTipoIVADelegate*> ( editor );
        QString value = comboBox->currentText();
        model->setData ( index, value );
    } else if ( linea->nomcampo() == "nomtrabajador"  && m_subform->tableName() != "trabajador" ) {
        BfBuscarTrabajadorDelegate * comboBox = static_cast<BfBuscarTrabajadorDelegate*> ( editor );
        QString value = comboBox->currentText();
        model->setData ( index, value );
    } else if ( linea->nomcampo() == "nomalmacen" && m_subform->tableName() != "almacen" ) {
        BfBuscarAlmacenDelegate * comboBox = static_cast<BfBuscarAlmacenDelegate*> ( editor );
        QString value = comboBox->currentText();
        model->setData ( index, value );
    } else {
        BlSubFormDelegate::setModelData ( editor, model, index );
    } // end if
    _depura ( "END BfSubFormDelegate::setModelData", 0 );
}


///
/**
\param editor
\param index
**/
void BfSubFormDelegate::setEditorData ( QWidget* editor, const QModelIndex& index ) const
{
    _depura ( "BfSubFormDelegate::setEditorData", 0, "CurrentColumn: " + QString::number ( index.column() ) +  "CurrentRow: " + QString::number ( index.row() )  );
    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    if ( linea->nomcampo() == "desc" + m_subform->tableName() ) {
        QString data = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BlTextEditDelegate *textedit = qobject_cast<BlTextEditDelegate*> ( editor );
        textedit->setText ( data );
    } else if ( linea->nomcampo() == "cant" + m_subform->tableName()
                || linea->nomcampo() == "pvp" + m_subform->tableName()
                || linea->nomcampo() == "descuento" + m_subform->tableName()
                || linea->nomcampo() == "reqeq" + m_subform->tableName()
                || linea->nomcampo() == "iva" + m_subform->tableName() ) {

        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BlDoubleSpinBox *spinBox = static_cast<BlDoubleSpinBox*> ( editor );
        spinBox->setValue ( value.toDouble() );
        spinBox->selectAll();
/*
    } else if ( linea->nomcampo() == "codigocompletoarticulo" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BfBuscarArticuloDelegate *comboBox = static_cast<BfBuscarArticuloDelegate*> ( editor );
        comboBox->addItem ( value );
*/
    } else if ( linea->nomcampo() == "desctipo_iva" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BfBuscarTipoIVADelegate *comboBox = static_cast<BfBuscarTipoIVADelegate*> ( editor );
        comboBox->set ( value );
    } else if ( linea->nomcampo() == "nomtrabajador"  && m_subform->tableName() != "trabajador" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BfBuscarTrabajadorDelegate *comboBox = static_cast<BfBuscarTrabajadorDelegate*> ( editor );
        comboBox->set ( value );
    } else if ( linea->nomcampo() == "nomalmacen" && m_subform->tableName() != "almacen" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BfBuscarAlmacenDelegate *comboBox = static_cast<BfBuscarAlmacenDelegate*> ( editor );
        comboBox->set ( value );
    } else {
        BlSubFormDelegate::setEditorData ( editor, index );
    } // end if
    _depura ( "END BfSubFormDelegate::setEditorData", 0 );
}




///
/**
**/
QString BfSubForm::idArticulo()
{
    return m_idArticulo;
}


///
/**
**/
QString BfSubForm::idTarifa()
{
    return m_idTarifa;
}


///
/**
**/
void BfSubForm::calculaPVP ( BlDbSubFormRecord *rec )
{

    _depura ( "BfSubForm::calculaPVP" , 0 );

    BlDbRecordSet *cur = NULL;
    BlDbRecordSet *cur3 = NULL;
    BlDbRecordSet *cur1 = NULL;
    BlDbRecordSet *cur2 = NULL;

    /// Saca 'codigocompletoarticulo' del BlDbSubFormRecord pasado como parametro.
    QString codigocompleto = rec->dbValue ( "codigocompletoarticulo" );

    cur = mainCompany() ->loadQuery ( "SELECT * FROM articulo WHERE codigocompletoarticulo = '" + codigocompleto + "'" );

    if ( !cur->eof() ) {

        /// Aqui se establece el precio del articulo. Se tiene que tener en cuenta
        /// el cliente y la tarifa asignada si procede.
        if ( !mdb_idcliente.isEmpty() && !m_idAlmacen.isEmpty() ) {
            /// Se ha seleccionado un cliente.
            m_idArticulo = cur->valor ( "idarticulo" );
            cur3 = mainCompany() ->loadQuery ( "SELECT cliente.idtarifa, ltarifa.pvpltarifa, ltarifa.idalmacen FROM cliente INNER JOIN ltarifa ON (cliente.idtarifa = ltarifa.idtarifa) WHERE ltarifa.idalmacen = " + m_idAlmacen + " AND cliente.idcliente = " + mdb_idcliente + " AND ltarifa.idarticulo = " + m_idArticulo );
            m_idTarifa = cur3->valor ( "idtarifa" );
            if ( cur3->numregistros() > 0 ) {
                /// A) Se dispone de tarifa especial.
                rec->setDbValue ( "pvp" + m_tablename, cur3->valor ( "pvpltarifa" ) );

                /// Disparamos los plugins.
                int res = g_plugins->lanza ( "BfSubForm_calculaPVP", this );
                if ( res != 0 ) {
                    _depura ( "END BfSubForm::calculapvp", 0, "Salida por plugins" );
                    return;
                } // end if

            } else {
                /// B) No tiene tarifa especial se usa la asignada por defecto.
                rec->setDbValue ( "pvp" + m_tablename, cur->valor ( "pvparticulo" ) );
            } // end if
        } else {
            /// Sin cliente asignado se usa la tarifa asignada por defecto.
            rec->setDbValue ( "pvp" + m_tablename, cur->valor ( "pvparticulo" ) );
        } // end if





// ==============================
        // Miramos el IVA del articulo y lo ponemos.
	QString query = "SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->valor ( "idtipo_iva" ) + " ORDER BY fechatasa_iva LIMIT 1";

        cur1 = mainCompany() ->loadQuery ( query );
        if ( !cur1->eof() ) {

            if ( m_tablename == "lpresupuesto"
                    || m_tablename == "lpedidocliente"
                    || m_tablename == "lpedidoproveedor"
                    || m_tablename == "lalbaranp"
                    || m_tablename == "lfacturap"
                    || m_tablename == "lalbaran"
                    || m_tablename == "lfactura" ) {
                rec->setDbValue ( "iva" + m_tablename, cur1->valor ( "porcentasa_iva" ) );

                /// Calculamos el recargo equivalente.
                if ( mdb_idcliente != "" ) {
                    cur2 = mainCompany() ->loadQuery ( "SELECT recargoeqcliente FROM cliente WHERE idcliente = " + mdb_idcliente );
                    if ( !cur2->eof() ) {
                        if ( cur2->valor ( "recargoeqcliente" ) == "t" ) {
                            rec->setDbValue ( "reqeq" + m_tablename, cur1->valor ( "porcentretasa_iva" ) );
                        } // end if
                    } // end if
                    delete cur2;
                } else if ( mdb_idproveedor != "" ) {
                    cur2 = mainCompany() ->loadQuery ( "SELECT recargoeqproveedor FROM proveedor WHERE idproveedor = " + mdb_idproveedor );
                    if ( !cur2->eof() ) {
                        if ( cur2->valor ( "recargoeqproveedor" ) == "t" ) {
                            rec->setDbValue ( "reqeq" + m_tablename, cur1->valor ( "porcentretasa_iva" ) );
                        } // end if
                    } // end if
                    delete cur2;
                } else {
                    rec->setDbValue ( "reqeq" + m_tablename, "0" );
                } // end if

            } // end if
        } // end if
        if ( cur1 != NULL )
            delete cur1;
// ==============================

    } // end if

    delete cur;
    delete cur3;
    _depura ( "END BfSubForm::calculaPVP", 0 );
}


