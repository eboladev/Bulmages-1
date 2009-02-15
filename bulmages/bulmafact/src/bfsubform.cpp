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
#include "busquedatipoiva.h"
#include "qtexteditdelegate.h"
#include "busquedatrabajador.h"
#include "busquedaalmacen.h"
#include "busquedaarticulo.h"
#include "plugins.h"


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

    BlDbRecordSet *cur = empresaBase() ->cargacursor ( "SELECT * FROM articulo WHERE idarticulo = " + rec->DBvalue ( "idarticulo" ) );
    if ( !cur ) {
        _depura ( "END BfSubForm::pressedMinus", 0, "No hay un idarticulo" );
        return;
    } // end if

    if ( !cur->eof() ) {
        rec->setDBvalue ( camp->nomcampo(), cur->valor ( "obserarticulo" ) );
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

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "BfSubForm_on_mui_list_editFinished", this );
    if ( res != 0 ) {
        _depura ( "END BfSubForm::editFinished", 0, "Salida por plugins" );
        return;
    } // end if

    if ( camp->nomcampo() == "desctipo_iva" ) {
        cur = empresaBase() ->cargacursor ( "SELECT * FROM tipo_iva WHERE desctipo_iva = '" + camp->text() + "'" );
        if ( !cur->eof() ) {
            rec->setDBvalue ( "idtipo_iva", cur->valor ( "idtipo_iva" ) );
        } // end if
    } // end if

    if ( camp->nomcampo() == "nomtrabajador" ) {
        cur = empresaBase() ->cargacursor ( "SELECT * FROM trabajador WHERE apellidostrabajador ||', '||nomtrabajador = '" + camp->text() + "'" );
        if ( !cur->eof() ) {
            rec->setDBvalue ( "idtrabajador", cur->valor ( "idtrabajador" ) );
        } // end if
    } // end if

    if ( camp->nomcampo() == "nomalmacen" ) {
        cur = empresaBase() ->cargacursor ( "SELECT * FROM almacen WHERE nomalmacen ||', '||codigoalmacen = '" + camp->text() + "'" );
        if ( !cur->eof() ) {
            rec->setDBvalue ( "idalmacen", cur->valor ( "idalmacen" ) );
        } // end if
    } // end if




    if ( camp->nomcampo() == "codigocompletoarticulo" ) {
        cur = empresaBase() ->cargacursor ( "SELECT * FROM articulo WHERE codigocompletoarticulo = '" + camp->text() + "'" );
        if ( !cur->eof() ) {
            rec->setDBvalue ( "idarticulo", cur->valor ( "idarticulo" ) );
            rec->setDBvalue ( "codigocompletoarticulo", cur->valor ( "codigocompletoarticulo" ) );
            rec->setDBvalue ( "nomarticulo", cur->valor ( "nomarticulo" ) );
            if ( m_tablename == "lpresupuesto"
                    || m_tablename == "lpedidoproveedor"
                    || m_tablename == "lpedidocliente"
                    || m_tablename == "lalbaranp"
                    || m_tablename == "lfacturap"
                    || m_tablename == "lalbaran"
                    || m_tablename == "lfactura" ) {
                rec->setDBvalue ( "desc" + m_tablename, cur->valor ( "nomarticulo" ) );
                rec->setDBvalue ( "cant" + m_tablename, "1.00" );
                rec->setDBvalue ( "descuento" + m_tablename, "0.00" );

		calculaPVP(rec);

            } // end if
        } else {
            mensajeAviso ( _( "El codigo del articulo no existe.\nATENCION: No se guadara bien el documento hasta que sea valido." ) );
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

	// Miramos el IVA del articulo y lo ponemos.
        cur1 = empresaBase() ->cargacursor ( "SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->valor ( "idtipo_iva" ) + " ORDER BY fechatasa_iva LIMIT 1" );
        if ( !cur1->eof() ) {
            if ( m_tablename == "lpresupuesto"
                    || m_tablename == "lpedidocliente"
                    || m_tablename == "lpedidoproveedor"
                    || m_tablename == "lalbaranp"
                    || m_tablename == "lfacturap"
                    || m_tablename == "lalbaran"
                    || m_tablename == "lfactura" ) {
                rec->setDBvalue ( "iva" + m_tablename, cur1->valor ( "porcentasa_iva" ) );

                /// Calculamos el recargo equivalente.
                if ( mdb_idcliente != "" ) {
                    cur2 = empresaBase() ->cargacursor ( "SELECT recargoeqcliente FROM cliente WHERE idcliente = " + mdb_idcliente );
                    if ( !cur2->eof() ) {
                        if ( cur2->valor ( "recargoeqcliente" ) == "t" ) {
                            rec->setDBvalue ( "reqeq" + m_tablename, cur1->valor ( "porcentretasa_iva" ) );
                        } // end if
                    } // end if
                    delete cur2;
                } else if ( mdb_idproveedor != "" ) {
                    cur2 = empresaBase() ->cargacursor ( "SELECT recargoeqproveedor FROM proveedor WHERE idproveedor = " + mdb_idproveedor );
                    if ( !cur2->eof() ) {
                        if ( cur2->valor ( "recargoeqproveedor" ) == "t" ) {
                            rec->setDBvalue ( "reqeq" + m_tablename, cur1->valor ( "porcentretasa_iva" ) );
                        } // end if
                    } // end if
                    delete cur2;
                } else {
                    rec->setDBvalue ( "reqeq" + m_tablename, "0" );
                } // end if

            } // end if
        } // end if
        if ( cur1 != NULL )
            delete cur1;
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
        rec->setDBvalue ( "iva" + m_tablename, "0" );
        rec->setDBvalue ( "reqeq" + m_tablename, "0" );
    } // end for

    mdb_idcliente = id;

    /// Si el idcliente no existe salimos.
    if ( id == "" ) {
        _depura ( "END BfSubForm::setIdCliente", 0, "idcliente invalido" );
        return;
    } // end if

    BlDbRecordSet *curcliente = empresaBase() ->cargacursor ( "SELECT recargoeqcliente, regimenfiscalcliente FROM cliente WHERE idcliente = " + mdb_idcliente );

    if ( !curcliente->eof() ) {
        /// Cuando se cambia el cliente se deben recalcular las lineas por si hay Recargo Equivalente
        for ( int i = 0; i < rowCount() - 1; i++ ) {
            BlDbSubFormRecord *rec = lineaat ( i );
            BlDbRecordSet *cur = empresaBase() ->cargacursor ( "SELECT * FROM articulo WHERE idarticulo = " + rec->DBvalue ( "idarticulo" ) );
            BlDbRecordSet *cur1 = empresaBase() ->cargacursor ( "SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->valor ( "idtipo_iva" ) + " ORDER BY fechatasa_iva LIMIT 1" );
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
        rec->setDBvalue ( "iva" + m_tablename, "0" );
        rec->setDBvalue ( "reqeq" + m_tablename, "0" );
    } // end for

    /// Si el idproveedor no existe salimos.
    if ( id == "" ) {
        _depura ( "END BfSubForm::setIdProveedor", 0, "idproveedor invalido" );
        return;
    } // end if

    BlDbRecordSet *curproveedor = empresaBase() ->cargacursor ( "SELECT recargoeqproveedor, regimenfiscalproveedor FROM proveedor WHERE idproveedor=" + mdb_idproveedor );

    if ( !curproveedor->eof() ) {
        /// Cuando se cambia el cliente se deben recalcular las lineas por si hay Recargo Equivalente
        for ( int i = 0; i < rowCount() - 1; i++ ) {
            BlDbSubFormRecord *rec = lineaat ( i );
            BlDbRecordSet *cur = empresaBase() ->cargacursor ( "SELECT * FROM articulo WHERE idarticulo = " + rec->DBvalue ( "idarticulo" ) );
            BlDbRecordSet *cur1 = empresaBase() ->cargacursor ( "SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->valor ( "idtipo_iva" ) + " ORDER BY fechatasa_iva LIMIT 1" );
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
    _depura ( "END BfSubForm::setIdProveedor", 0 );
}


///
/**
\param id
**/
void BfSubForm::setIdAlmacen ( QString id )
{
	_depura("BfSubForm::setIdAlmacen", 0);
	m_idAlmacen = id;
	_depura("END BfSubForm::setIdAlmacen", 0);
}


///
/**
\param c
**/
void BfSubForm::setEmpresaBase ( EmpresaBase *c )
{
    _depura ( "BfSubForm::setcompany", 0 );
    BlSubForm::setEmpresaBase ( c );
    m_delegate->setEmpresaBase ( c );
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
	_depura("BfSubForm::idAlmacen", 0);
	_depura("END BfSubForm::idAlmacen", 0);
	return m_idAlmacen;
}


/// ===============================================================
///  Tratamientos del Item Delegate
/// ===============================================================
///
/**
\param parent
**/
BfSubFormDelegate::BfSubFormDelegate ( QObject *parent = 0 ) : QItemDelegate ( parent ), PEmpresaBase()
{
    _depura ( "BfSubFormDelegate::BfSubFormDelegate", 0 );
    m_subform = ( BfSubForm * ) parent;
    installEventFilter ( this );
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
        QTextEditDelegate * editor = new QTextEditDelegate ( parent );
        editor->setObjectName ( "QTextEditDelegate" );
        _depura ( "END BfSubFormDelegate::createEditor", 0, "QTextEdit" );
        return editor;

    } else if ( linea->nomcampo() == "cant" + m_subform->tableName()
                || linea->nomcampo() == "pvp" + m_subform->tableName()
                || linea->nomcampo() == "descuento" + m_subform->tableName()
                || linea->nomcampo() == "iva" + m_subform->tableName()
                || linea->nomcampo() == "reqeq" + m_subform->tableName() ) {

        BlDoubleSpinBox * editor = new BlDoubleSpinBox ( parent );
        editor->setMinimum ( -1000000 );
        editor->setMaximum ( 1000000 );
        _depura ( "END BfSubFormDelegate::createEditor", 0, "QSPinBox" );
        return editor;

    } else if ( linea->nomcampo() == "codigocompletoarticulo" ) {
        BusquedaArticuloDelegate * editor = new BusquedaArticuloDelegate ( parent );
        editor->setEmpresaBase ( ( Company * ) m_subform->empresaBase() );
        _depura ( "END BfSubFormDelegate::createEditor", 0, "BusquedaArticulo" );
        return editor;
    } else if ( linea->nomcampo() == "desctipo_iva" ) {
        BusquedaTipoIVADelegate * editor = new BusquedaTipoIVADelegate ( parent );
        editor->setEmpresaBase ( ( Company * ) m_subform->empresaBase() );
        _depura ( "END BfSubFormDelegate::createEditor", 0, "BusquedaTipoIVA" );
        return editor;
    } else if ( linea->nomcampo() == "nomtrabajador" && m_subform->tableName() != "trabajador" ) {
        BusquedaTrabajadorDelegate * editor = new BusquedaTrabajadorDelegate ( parent );
        editor->setEmpresaBase ( ( Company * ) m_subform->empresaBase() );
        _depura ( "END BfSubFormDelegate::createEditor", 0, "BusquedaTrabajadorDelegate" );
        return editor;
    } else if ( linea->nomcampo() == "nomalmacen"  && m_subform->tableName() != "almacen" ) {
        BusquedaAlmacenDelegate * editor = new BusquedaAlmacenDelegate ( parent );
        editor->setEmpresaBase ( ( Company * ) m_subform->empresaBase() );
        _depura ( "END BfSubFormDelegate::createEditor", 0, "BusquedaAlmacenDelegate" );
        return editor;
    } else  {
        _depura ( "END BfSubFormDelegate::createEditor", 0, "Default Editor" );
        return QItemDelegate::createEditor ( parent, option, index );
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
        QTextEditDelegate * textedit = qobject_cast<QTextEditDelegate *> ( editor );
        model->setData ( index, textedit->toPlainText() );
    } else if ( linea->nomcampo() == "cant" + m_subform->tableName()
                || linea->nomcampo() == "pvp" + m_subform->tableName()
                || linea->nomcampo() == "descuento" + m_subform->tableName()
                || linea->nomcampo() == "reqeq" + m_subform->tableName()
                || linea->nomcampo() == "iva" + m_subform->tableName() ) {

        BlDoubleSpinBox * spinBox = static_cast<BlDoubleSpinBox*> ( editor );
        spinBox->interpretText();
        QString value = spinBox->text();
	QString valueanterior = m_subform->lista()->at ( index.row() )->DBvalue ( "cant" + m_subform->tableName());

        model->setData ( index, value );

	/// Dispara senyales:
	if ( linea->nomcampo() == "cant" + m_subform->tableName() ) {
		/// Mira si se ha cambiado la cantidad o es la misma que habia.
		if (valueanterior.toDouble() != value.toDouble()) {
			emit cant_changed(m_subform->lista()->at ( index.row() ));
		} // end if
	} // end if

    } else if ( linea->nomcampo() == "codigocompletoarticulo" ) {
        BusquedaArticuloDelegate * comboBox = static_cast<BusquedaArticuloDelegate*> ( editor );
        QString value = comboBox->currentText();
        value = value.left ( value.indexOf ( ".-" ) );
        model->setData ( index, value );
    } else if ( linea->nomcampo() == "desctipo_iva" ) {
        BusquedaTipoIVADelegate * comboBox = static_cast<BusquedaTipoIVADelegate*> ( editor );
        QString value = comboBox->currentText();
        model->setData ( index, value );
    } else if ( linea->nomcampo() == "nomtrabajador"  && m_subform->tableName() != "trabajador" ) {
        BusquedaTrabajadorDelegate * comboBox = static_cast<BusquedaTrabajadorDelegate*> ( editor );
        QString value = comboBox->currentText();
        model->setData ( index, value );
    } else if ( linea->nomcampo() == "nomalmacen" && m_subform->tableName() != "almacen" ) {
        BusquedaAlmacenDelegate * comboBox = static_cast<BusquedaAlmacenDelegate*> ( editor );
        QString value = comboBox->currentText();
        model->setData ( index, value );
    } else {
        QItemDelegate::setModelData ( editor, model, index );
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
        QTextEditDelegate *textedit = qobject_cast<QTextEditDelegate*> ( editor );
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

    } else if ( linea->nomcampo() == "codigocompletoarticulo" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BusquedaArticuloDelegate *comboBox = static_cast<BusquedaArticuloDelegate*> ( editor );
        comboBox->addItem ( value );
    } else if ( linea->nomcampo() == "desctipo_iva" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BusquedaTipoIVADelegate *comboBox = static_cast<BusquedaTipoIVADelegate*> ( editor );
        comboBox->set ( value );
    } else if ( linea->nomcampo() == "nomtrabajador"  && m_subform->tableName() != "trabajador" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BusquedaTrabajadorDelegate *comboBox = static_cast<BusquedaTrabajadorDelegate*> ( editor );
        comboBox->set ( value );
    } else if ( linea->nomcampo() == "nomalmacen" && m_subform->tableName() != "almacen" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BusquedaAlmacenDelegate *comboBox = static_cast<BusquedaAlmacenDelegate*> ( editor );
        comboBox->set ( value );
    } else {
        QItemDelegate::setEditorData ( editor, index );
    } // end if
    _depura ( "END BfSubFormDelegate::setEditorData", 0 );
}


///
/**
\param obj
\param event
\return
**/
bool BfSubFormDelegate::eventFilter ( QObject *obj, QEvent *event )
{
    /// Si es un release de tecla se hace la funcionalidad especificada.
    if ( event->type() == QEvent::KeyPress ) {
        _depura ( "BfSubFormDelegate::eventFilter", 0, obj->objectName() + " --> " + QString::number ( event->type() ) );
        QKeyEvent *keyEvent = static_cast<QKeyEvent *> ( event );
        int key = keyEvent->key();
        _depura ( "BfSubFormDelegate::key = : ", 0, QString::number ( key ) );
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
        _depura ( "BfSubFormDelegate::eventFilter", 0, obj->objectName() + " --> " + QString::number ( event->type() ) );
        QKeyEvent *keyEvent = static_cast<QKeyEvent *> ( event );
        int key = keyEvent->key();
        _depura ( "BfSubFormDelegate::key = : ", 0, QString::number ( key ) );
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
int BfSubFormDelegate::cerrarEditor ( QWidget *editor )
{
    _depura ( "BfSubFormDelegate::cerrarEditor", 0 );
    emit closeEditor ( editor, QAbstractItemDelegate::NoHint );
    _depura ( "END BfSubFormDelegate::cerrarEditor", 0 );
    return 0;
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
void BfSubForm::calculaPVP(BlDbSubFormRecord *rec) {

	_depura("BfSubForm::calculaPVP" , 0);

	BlDbRecordSet *cur = NULL;
	BlDbRecordSet *cur3 = NULL;

	/// Saca 'codigocompletoarticulo' del BlDbSubFormRecord pasado como parametro.
	QString codigocompleto = rec->DBvalue("codigocompletoarticulo");

        cur = empresaBase() ->cargacursor ( "SELECT * FROM articulo WHERE codigocompletoarticulo = '" + codigocompleto + "'" );
        if ( !cur->eof() ) {
		/// Aqui se establece el precio del articulo. Se tiene que tener en cuenta
		/// el cliente y la tarifa asignada si procede.
		if (!mdb_idcliente.isEmpty() && !m_idAlmacen.isEmpty()) {
			/// Se ha seleccionado un cliente.
			m_idArticulo = cur->valor ( "idarticulo" );
			cur3 = empresaBase() ->cargacursor ( "SELECT cliente.idtarifa, ltarifa.pvpltarifa, ltarifa.idalmacen FROM cliente INNER JOIN ltarifa ON (cliente.idtarifa = ltarifa.idtarifa) WHERE ltarifa.idalmacen = " + m_idAlmacen + " AND cliente.idcliente = " + mdb_idcliente + " AND ltarifa.idarticulo = " + m_idArticulo );
			m_idTarifa = cur3->valor ( "idtarifa" );
			if (cur3->numregistros() > 0) {
				/// A) Se dispone de tarifa especial.
				rec->setDBvalue ( "pvp" + m_tablename, cur3->valor ( "pvpltarifa" ) );

				/// Disparamos los plugins.
				int res = g_plugins->lanza ( "BfSubForm_calculaPVP", this );
				if ( res != 0 ) {
					_depura ( "END BfSubForm::calculapvp", 0, "Salida por plugins" );
					return;
				} // end if

			} else {
				/// B) No tiene tarifa especial se usa la asignada por defecto.
				rec->setDBvalue ( "pvp" + m_tablename, cur->valor ( "pvparticulo" ) );
			} // end if
		} else {
			/// Sin cliente asignado se usa la tarifa asignada por defecto.
			rec->setDBvalue ( "pvp" + m_tablename, cur->valor ( "pvparticulo" ) );
		} // end if
		
	} // end if

	delete cur;
	delete cur3;
	_depura("END BfSubForm::calculaPVP",0);
}


