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

#include "subform2bf.h"
#include "funcaux.h"
#include "articulolist.h"
#include "busquedaarticulo.h"
#include "busquedatipoiva.h"
#include "qtexteditdelegate.h"
#include "busquedatrabajador.h"
#include "busquedaalmacen.h"
#include "plugins.h"


/// Clase de subformularios especificos.
/**
\param parent El widget propietario del subformulario.
**/
SubForm2Bf::SubForm2Bf ( QWidget *parent ) : SubForm3 ( parent )
{
    _depura ( "SubForm2Bf::SubForm2Bf", 0 );
    setDelete ( TRUE );
    m_delegate = new QSubForm2BfDelegate ( this );
    mui_list->setItemDelegate ( m_delegate );
    mdb_idcliente = "";
    _depura ( "END SubForm2Bf::SubForm2Bf", 0 );
}


/// Destructor de la clase.
/**
**/
SubForm2Bf::~SubForm2Bf()
{
    _depura ( "SubForm2Bf::~SubForm2Bf", 0 );
    delete m_delegate;
    _depura ( "END SubForm2Bf::~SubForm2Bf", 0 );
}


/// Carga un Query
/**
\param query Una consulta SQL valida.
**/
void SubForm2Bf::cargar ( QString query )
{
    _depura ( "SubForm2Bf::cargar", 0, query );
    SubForm3::cargar ( query );
    _depura ( "END SubForm2Bf::cargar", 0 );
}


///
/**
\param row
\param col
\return
**/
void SubForm2Bf::pressedAsterisk ( int row, int col, SDBRecord *rec, SDBCampo *camp )
{
    _depura ( "SubForm2Bf::pressedAsterisk", 0 );

    if ( camp->nomcampo() != "codigocompletoarticulo" ) {
        _depura ( "END SubForm2Bf::pressedAsterisk", 0 );
        return;
    } // end if


    ArticuloList *artlist = new ArticuloList ( ( Company * ) empresaBase(), NULL, 0, ArticuloList::SelectMode );
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    this->setEnabled ( false );
    artlist->show();
    while ( !artlist->isHidden() )
        theApp->processEvents();
    this->setEnabled ( true );
    QString idArticle = artlist->idarticulo();
    delete artlist;

    /// Si no tenemos un idarticulo salimos ya que significa que no se ha seleccionado ninguno.
    if ( idArticle == "" ) {
        _depura ( "END SubForm2Bf::pressedAsterisk", 0 );
        return;
    } // end if

    cursor2 *cur = empresaBase() ->cargacursor ( "SELECT * FROM articulo WHERE idarticulo = " + idArticle );
    if ( !cur->eof() ) {
        rec->setDBvalue ( "idarticulo", idArticle );
        rec->setDBvalue ( "codigocompletoarticulo", cur->valor ( "codigocompletoarticulo" ) );
        rec->setDBvalue ( "nomarticulo", cur->valor ( "nomarticulo" ) );
    } // end if
    delete cur;

    _depura ( "END SubForm2Bf::pressedAsterisk", 0 );
}


///
/**
\param row
\param col
**/
void SubForm2Bf::pressedSlash ( int row, int col, SDBRecord *rec, SDBCampo *camp )
{
    _depura ( "SubForm2Bf::pressedSlash", 0 );
    QString text = editaTexto ( camp->text() );
    camp->set ( text );
    _depura ( "END SubForm2Bf::pressedSlash", 0 );
}


///
/**
\param row
\param col
**/
void SubForm2Bf::pressedMinus ( int row, int col, SDBRecord *rec, SDBCampo *camp )
{
    _depura ( "SubForm2Bf::pressedMinus", 0 );

    if ( !rec->exists ( "idarticulo" ) ) {
        _depura ( "END SubForm2Bf::pressedMinus", 0, "No hay un idarticulo" );
        return;
    } // end if

    cursor2 *cur = empresaBase() ->cargacursor ( "SELECT * FROM articulo WHERE idarticulo = " + rec->DBvalue ( "idarticulo" ) );
    if ( !cur ) {
        _depura ( "END SubForm2Bf::pressedMinus", 0, "No hay un idarticulo" );
        return;
    } // end if

    if ( !cur->eof() ) {
        rec->setDBvalue ( camp->nomcampo(), cur->valor ( "obserarticulo" ) );
    } // end if
    delete cur;

    _depura ( "END SubForm2Bf::pressedMinus", 0 );
}


///
/**
\param row
\param col
\return
**/
void SubForm2Bf::editFinished ( int row, int col, SDBRecord *rec, SDBCampo *camp )
{
    _depura ( "SubForm2Bf::editFinished", 0, QString::number ( row ) + " " + QString::number ( col ) );

    cursor2 *cur2 = NULL;
    cursor2 *cur = NULL;
    cursor2 *cur1 = NULL;

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "SubForm2Bf_on_mui_list_editFinished", this );
    if ( res != 0 ) {
        _depura ( "END SubForm2Bf::editFinished", 0, "Salida por plugins" );
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
                rec->setDBvalue ( "pvp" + m_tablename, cur->valor ( "pvparticulo" ) );
            } // end if
        } else {
	    mensajeAviso(tr("El codigo del articulo no existe.\nATENCION: No se guadara bien el documento hasta que sea valido."), this);
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
		msgBox.setWindowTitle(tr("Articulo inexistente"));
		msgBox.setText(tr("El uso de codigos de articulos inexistentes esta prohibido.\nCorrija esta situacion para poder guardar el documento."));
		QPushButton *botonCrear = msgBox.addButton(tr("&Crear articulo"), QMessageBox::ActionRole);
		QPushButton *botonSeleccionar = msgBox.addButton(tr("&Seleccionar articulo"), QMessageBox::ActionRole);
		QPushButton *botonEditar = msgBox.addButton(tr("&Editar codigo"), QMessageBox::ActionRole);
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
            _depura ( "END SubForm2Bf::editFinished", 0, "Articulo inexistente" );
            return;
        } // end if

        cur1 = empresaBase() ->cargacursor ( "SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->valor ( "idtipo_iva" ) + " ORDER BY fechatasa_iva LIMIT 1" );
        if ( !cur->eof() ) {
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
                } else {
                    rec->setDBvalue ( "reqeq" + m_tablename, "0" );
                } // end if

                if ( mdb_idproveedor != "" ) {
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
        SubForm3::on_mui_list_cellChanged ( row, col );
    */
    /// Refrescamos el registro.
    rec->refresh();
    _depura ( "END SubForm2Bf::editFinished", 0 );
}


///
/**
\param id
\return
**/
void SubForm2Bf::setIdCliente ( QString id )
{
    _depura ( "SubForm2Bf::setIdCliente", 0, id );

    /// En la primera carga no hay reajustes, pero si actualización del cliente.
    if ( mdb_idcliente  == "" ) {
        mdb_idcliente = id;
        _depura ( "END SubForm2Bf::setIdCliente", 0, "Primera carga" );
        return;
    } // end if

    /// En las cargas sucesivas si el idcliente no ha cambiado no se hace nada
    if ( mdb_idcliente == id ) {
        _depura ( "END SubForm2Bf::setIdCliente", 0, "cliente sin cambios" );
        return;
    } // end if

    /// Reseteamos los valores.
    for ( int i = 0; i < rowCount() - 1; i++ ) {
        SDBRecord *rec = lineaat ( i );
        rec->setDBvalue ( "iva" + m_tablename, "0" );
        rec->setDBvalue ( "reqeq" + m_tablename, "0" );
    } // end for

    mdb_idcliente = id;

    /// Si el idcliente no existe salimos.
    if ( id == "" ) {
        _depura ( "END SubForm2Bf::setIdCliente", 0, "idcliente invalido" );
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
    _depura ( "END SubForm2Bf::setIdCliente", 0 );
}


///
/**
\param id
\return
**/
void SubForm2Bf::setIdProveedor ( QString id )
{
    _depura ( "SubForm2Bf::setIdProveedor", 0, id );

    /// En la primera carga no hay reajustes, pero si actualización del proveedor.
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

    /// Si el idproveedor no existe salimos.
    if ( id == "" ) {
        _depura ( "END SubForm2Bf::setIdProveedor", 0, "idproveedor invalido" );
        return;
    } // end if

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
    _depura ( "END SubForm2Bf::setIdProveedor", 0 );
}


///
/**
\param c
**/
void SubForm2Bf::setEmpresaBase ( EmpresaBase *c )
{
    _depura ( "SubForm2Bf::setcompany", 0 );
    SubForm3::setEmpresaBase ( c );
    m_delegate->setEmpresaBase ( c );
    _depura ( "END SubForm2Bf::setcompany", 0 );
}


///
/**
\return
**/
int SubForm2Bf::cerrarEditor()
{
    _depura ( "SubForm2Bf::cerrarEditor", 0 );
    QWidget *editor = mui_list->QAbstractItemView::indexWidget ( mui_list->currentIndex() );
    m_delegate->cerrarEditor ( editor );
    _depura ( "END SubForm2Bf::cerrarEditor", 0 );
    return 0;
}


/// Devuelve el identificador de cliente, si es un subformulario de cliente.
/**
\return El identificador de cliente
**/
QString SubForm2Bf::idcliente()
{
    return mdb_idcliente;
}


/// Devuelve el identificador de proveedor, si es un subformulario de proveedor.
/**
\return El identificador de proveedor
**/
QString SubForm2Bf::idproveedor()
{
    return mdb_idproveedor;
}


/// ===============================================================
///  Tratamientos del Item Delegate
/// ===============================================================
///
/**
\param parent
**/
QSubForm2BfDelegate::QSubForm2BfDelegate ( QObject *parent = 0 ) : QItemDelegate ( parent ), PEmpresaBase()
{
    _depura ( "QSubForm2BfDelegate::QSubForm2BfDelegate", 0 );
    m_subform = ( SubForm2Bf * ) parent;
    installEventFilter ( this );
    _depura ( "END QSubForm2BfDelegate::QSubForm2BfDelegate", 0 );
}


///
/**
**/
QSubForm2BfDelegate::~QSubForm2BfDelegate()
{
    _depura ( "QSubForm2BfDelegate::~QSubForm2BfDelegate", 0 );
    _depura ( "END QSubForm2BfDelegate::~QSubForm2BfDelegate", 0 );
}


///
/**
\param parent
\param option
\param index
**/
QWidget *QSubForm2BfDelegate::createEditor ( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    _depura ( "QSubForm2BfDelegate::createEditor", 0, "CurrentColumn: " + QString::number ( index.column() ) + "CurrentRow" + QString::number ( index.row() )  );
    SHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );

    if ( linea->nomcampo() == "desc" + m_subform->tableName() ) {
        QTextEditDelegate * editor = new QTextEditDelegate ( parent );
        editor->setObjectName ( "QTextEditDelegate" );
        _depura ( "END QSubForm2BfDelegate::createEditor", 0, "QTextEdit" );
        return editor;

    } else if ( linea->nomcampo() == "cant" + m_subform->tableName()
                || linea->nomcampo() == "pvp" + m_subform->tableName()
                || linea->nomcampo() == "descuento" + m_subform->tableName()
                || linea->nomcampo() == "iva" + m_subform->tableName()
                || linea->nomcampo() == "reqeq" + m_subform->tableName() ) {

        QDoubleSpinBox2 * editor = new QDoubleSpinBox2 ( parent );
        editor->setMinimum ( -1000000 );
        editor->setMaximum ( 1000000 );
        _depura ( "END QSubForm2BfDelegate::createEditor", 0, "QSPinBox" );
        return editor;

    } else if ( linea->nomcampo() == "codigocompletoarticulo" ) {
        BusquedaArticuloDelegate * editor = new BusquedaArticuloDelegate ( parent );
        editor->setEmpresaBase ( ( Company * ) m_subform->empresaBase() );
        _depura ( "END QSubForm2BfDelegate::createEditor", 0, "BusquedaArticulo" );
        return editor;
    } else if ( linea->nomcampo() == "desctipo_iva" ) {
        BusquedaTipoIVADelegate * editor = new BusquedaTipoIVADelegate ( parent );
        editor->setEmpresaBase ( ( Company * ) m_subform->empresaBase() );
        _depura ( "END QSubForm2BfDelegate::createEditor", 0, "BusquedaTipoIVA" );
        return editor;
    } else if ( linea->nomcampo() == "nomtrabajador" && m_subform->tableName() != "trabajador" ) {
        BusquedaTrabajadorDelegate * editor = new BusquedaTrabajadorDelegate ( parent );
        editor->setEmpresaBase ( ( Company * ) m_subform->empresaBase() );
        _depura ( "END QSubForm2BfDelegate::createEditor", 0, "BusquedaTrabajadorDelegate" );
        return editor;
    } else if ( linea->nomcampo() == "nomalmacen"  && m_subform->tableName() != "almacen" ) {
        BusquedaAlmacenDelegate * editor = new BusquedaAlmacenDelegate ( parent );
        editor->setEmpresaBase ( ( Company * ) m_subform->empresaBase() );
        _depura ( "END QSubForm2BfDelegate::createEditor", 0, "BusquedaAlmacenDelegate" );
        return editor;
    } else  {
        _depura ( "END QSubForm2BfDelegate::createEditor", 0, "Default Editor" );
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
void QSubForm2BfDelegate::setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    _depura ( "QSubForm2BfDelegate::setModelData", 0, "CurrentColumn: " + QString::number ( index.column() ) + "CurrentRow: " + QString::number ( index.row() ) );

    /// Si la fila o columna pasadas son inv&aacute;lidas salimos.
    if ( index.column() < 0 || index.row() < 0 ) {
        _depura ( "END QSubForm2BfDelegate::setModelData", 0, "Fila o columna invalida" );
        return;
    } // end if

    SHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    if ( linea->nomcampo() == "desc" + m_subform->tableName() ) {
        QTextEditDelegate * textedit = qobject_cast<QTextEditDelegate *> ( editor );
        model->setData ( index, textedit->toPlainText() );
    } else if ( linea->nomcampo() == "cant" + m_subform->tableName()
                || linea->nomcampo() == "pvp" + m_subform->tableName()
                || linea->nomcampo() == "descuento" + m_subform->tableName()
                || linea->nomcampo() == "reqeq" + m_subform->tableName()
                || linea->nomcampo() == "iva" + m_subform->tableName() ) {

        QDoubleSpinBox2 * spinBox = static_cast<QDoubleSpinBox2*> ( editor );
        spinBox->interpretText();
        QString value = spinBox->text();
        model->setData ( index, value );

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
    _depura ( "END QSubForm2BfDelegate::setModelData", 0 );
}


///
/**
\param editor
\param index
**/
void QSubForm2BfDelegate::setEditorData ( QWidget* editor, const QModelIndex& index ) const
{
    _depura ( "QSubForm2BfDelegate::setEditorData", 0, "CurrentColumn: " + QString::number ( index.column() ) +  "CurrentRow: " + QString::number ( index.row() )  );
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
    _depura ( "END QSubForm2BfDelegate::setEditorData", 0 );
}


///
/**
\param obj
\param event
\return
**/
bool QSubForm2BfDelegate::eventFilter ( QObject *obj, QEvent *event )
{
    /// Si es un release de tecla se hace la funcionalidad especificada.
    if ( event->type() == QEvent::KeyPress ) {
        _depura ( "QSubForm2BfDelegate::eventFilter", 0, obj->objectName() + " --> " + QString::number ( event->type() ) );
        QKeyEvent *keyEvent = static_cast<QKeyEvent *> ( event );
        int key = keyEvent->key();
        _depura ( "QSubForm2BfDelegate::key = : ", 0, QString::number ( key ) );
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
        _depura ( "QSubForm2BfDelegate::eventFilter", 0, obj->objectName() + " --> " + QString::number ( event->type() ) );
        QKeyEvent *keyEvent = static_cast<QKeyEvent *> ( event );
        int key = keyEvent->key();
        _depura ( "QSubForm2BfDelegate::key = : ", 0, QString::number ( key ) );
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
int QSubForm2BfDelegate::cerrarEditor ( QWidget *editor )
{
    _depura ( "QSubForm2BfDelegate::cerrarEditor", 0 );
    emit closeEditor ( editor, QAbstractItemDelegate::NoHint );
    _depura ( "END QSubForm2BfDelegate::cerrarEditor", 0 );
    return 0;
}


