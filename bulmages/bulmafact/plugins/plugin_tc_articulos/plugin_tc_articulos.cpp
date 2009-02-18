/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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


#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>

#include "plugin_tc_articulos.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "plugarticulos.h"
#include "busquedacolor.h"
#include "busquedatalla.h"


///
/**
\param bges
**/
void entryPoint ( Bulmafact *bges )
{
    _depura ( "Estoy dentro del plugin de tallas y colores", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("plugin_tc_articulos", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    plugin_tc_articulos *plug = new plugin_tc_articulos();
    plug->inicializa ( bges );
}


///
/**
\param art
\return
**/
int ArticuloView_ArticuloView ( ArticuloView *art )
{
    _depura ( "ArticuloView_ArticuloView", 0 );

    /// Agregamos el subformulario de validaciones.
    BfSubForm *l = new BfSubForm ( art );

    /// Ponemos un delegate al subformulario para que coja los combos asignados.
    delete l->m_delegate;
    l->m_delegate = new QSubForm3BfDelegate ( l );
    l->mui_list->setItemDelegate ( l->m_delegate );

    l->setObjectName ( QString::fromUtf8 ( "laliastc" ) );
    l->setMainCompany ( art->empresaBase() );
    l->setDBTableName ( "tc_articulo_alias" );
    l->setDBCampoId ( "idarticulo" );
    l->addSubFormHeader ( "aliastc_articulo_tallacolor", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNone , _( "Alias" ) );
    l->addSubFormHeader ( "idarticulo", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite , _( "Id Articulo" ) );
    l->addSubFormHeader ( "idtc_color", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "color" ) );
    l->addSubFormHeader ( "idtc_talla", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Talla" ) );


    l->addSubFormHeader ( "nomtc_color", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Nombre color" ) );

    l->addSubFormHeader ( "nomtc_talla", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Nombre talla" ) );

    l->setinsercion ( TRUE );
    l->setDelete ( TRUE );
    l->setSortingEnabled ( FALSE );
    art->dialogChanges_setQObjectExcluido ( l->mui_list );

    art->mui_tab->addTab ( l, "Tallas y Colores" );

    _depura ( "END ArticuloView_ArticuloView", 0 );
    return 0;
}


///
/**
\param art
\return
**/
int ArticuloView_cargar ( ArticuloView *art )
{
    _depura ( "ArticuloView_cargar", 0 );
    BfSubForm *l = art->findChild<BfSubForm *> ( "laliastc" );
    if ( l ) {
        QString query = "SELECT * FROM tc_articulo_alias LEFT JOIN tc_talla AS t1 ON tc_articulo_alias.idtc_talla = t1.idtc_talla LEFT JOIN tc_color AS t2 ON tc_articulo_alias.idtc_color = t2.idtc_color WHERE tc_articulo_alias.idarticulo = " + art->DBvalue ( "idarticulo" );
        l->cargar ( query );
    } // end if
    _depura ( "END ArticuloView_cargar", 0 );
    return 0;
}



///
/**
\param art
\return
**/
int ArticuloView_guardar_post ( ArticuloView *art )
{
    _depura ( "ArticuloView_guardar_post", 0 );
    try {
        BfSubForm *l = art->findChild<BfSubForm *> ( "laliastc" );
        l->setColumnValue ( "idarticulo", art->DBvalue ( "idarticulo" ) );
        l->guardar();
        return 0;
    } catch ( ... ) {
        _depura ( "Hubo un error al guardar los alias", 2 );
        return 0;
    }
}




/// ============================= SUBFORM3BFDELEGATE =============================================
/// ===============================================================
///  Tratamientos del Item Delegate
/// ===============================================================


///
/**
\param parent
**/
QSubForm3BfDelegate::QSubForm3BfDelegate ( QObject *parent = 0 ) : BfSubFormDelegate ( parent )
{
    _depura ( "QSubForm3BfDelegate::QSubForm3BfDelegate", 0 );
    _depura ( "END QSubForm3BfDelegate::QSubForm3BfDelegate", 0 );
}


///
/**
**/
QSubForm3BfDelegate::~QSubForm3BfDelegate()
{
    _depura ( "QSubForm3BfDelegate::~QSubForm3BfDelegate", 0 );
    _depura ( "END QSubForm3BfDelegate::~QSubForm3BfDelegate", 0 );
}


///
/**
\param parent
\param option
\param index
\return
**/
QWidget *QSubForm3BfDelegate::createEditor ( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    _depura ( "QSubForm3BfDelegate::createEditor", 0 );
    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );

    if ( linea->nomcampo() == "nomtc_color" ) {
        BusquedaColorDelegate * editor = new BusquedaColorDelegate ( parent );
        editor->setMainCompany ( ( BfCompany * ) m_subform->empresaBase() );
        return editor;
    } else if ( linea->nomcampo() == "nomtc_talla" ) {
        BusquedaTallaDelegate * editor = new BusquedaTallaDelegate ( parent );
        editor->setMainCompany ( ( BfCompany * ) m_subform->empresaBase() );
        return editor;
    } else  {
        return BfSubFormDelegate::createEditor ( parent, option, index );
    } // end if
    _depura ( "END QSubForm3BfDelegate::createEditor", 0 );
}


///
/**
\param editor
\param model
\param index
\return
**/
void QSubForm3BfDelegate::setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    _depura ( "QSubForm3BfDelegate::setModelData", 0 );

    /// Si la fila o columna pasadas son invalidas salimos.
    if ( index.column() < 0 || index.row() < 0 )
        return;

    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    if ( linea->nomcampo() == "nomtc_color" ) {
        BusquedaColorDelegate * comboBox = static_cast<BusquedaColorDelegate*> ( editor );
        QString value = comboBox->currentText();
        model->setData ( index, value );
        m_subform->lineaat ( index.row() ) ->setDBvalue ( "idtc_color", comboBox->id() );
    } else     if ( linea->nomcampo() == "nomtc_talla" ) {
        BusquedaTallaDelegate * comboBox = static_cast<BusquedaTallaDelegate*> ( editor );
        QString value = comboBox->currentText();
        model->setData ( index, value );
        m_subform->lineaat ( index.row() ) ->setDBvalue ( "idtc_talla", comboBox->id() );
    } else {
        BfSubFormDelegate::setModelData ( editor, model, index );
    } // end if
    _depura ( "END QSubForm3BfDelegate::setModelData", 0 );
}


///
/**
\param editor
\param index
**/
void QSubForm3BfDelegate::setEditorData ( QWidget* editor, const QModelIndex& index ) const
{
    _depura ( "QSubForm3BfDelegate::setEditorData", 0 );
    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    if ( linea->nomcampo() == "nomtc_color" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BusquedaColorDelegate *comboBox = static_cast<BusquedaColorDelegate*> ( editor );
        comboBox->set ( value );
    } else if ( linea->nomcampo() == "nomtc_talla" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BusquedaTallaDelegate *comboBox = static_cast<BusquedaTallaDelegate*> ( editor );
        comboBox->set ( value );
    } else {
        BfSubFormDelegate::setEditorData ( editor, index );
    } // end if
    _depura ( "END QSubForm3BfDelegate::setEditorData", 0 );
}


int Busqueda_on_m_inputBusqueda_textChanged ( Busqueda *busc )
{
    bool encontrado = FALSE;

	if (busc->tableName() == "articulo") {
	
		QString val = busc->m_inputBusqueda->text();
	
		QString SQLQuery = "SELECT * FROM tc_articulo_alias LEFT JOIN articulo ON tc_articulo_alias.idarticulo = articulo.idarticulo WHERE aliastc_articulo_tallacolor = '" + val + "'";
		BlDbRecordSet *cur = busc->empresaBase() ->cargacursor ( SQLQuery );
		if ( !cur->eof() ) {
			busc->setId ( cur->valor ( "idarticulo" ) );
			encontrado = TRUE;
		}
		delete cur;
	
		if ( encontrado ) {
			return -1;
		} // end if
	} // end if
    return 0;
}



///
/**
\param subform
\return
**/
int ListLinAlbaranProveedorView_ListLinAlbaranProveedorView ( ListLinAlbaranProveedorView *subform )
{
    _depura ( "ListLinAlbaranProveedorView_ListLinAlbaranProveedorView", 0 );

    subform->addSubFormHeader ( "idtc_color", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "color" ) );
    subform->addSubFormHeader ( "idtc_talla", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Talla" ) );
    subform->addSubFormHeader ( "nomtc_color", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Nombre color" ) );
    subform->addSubFormHeader ( "nomtc_talla", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Nombre talla" ) );

    delete subform->m_delegate;
    subform->m_delegate = new QSubForm3BfDelegate ( subform );
    subform->mui_list->setItemDelegate ( subform->m_delegate );

    _depura ( "END ListLinAlbaranProveedorView_ListLinAlbaranProveedorView", 0 );
    return 0;
}


///
/**
\param subform
\return
**/
int ListLinFacturaProveedorView_ListLinFacturaProveedorView ( ListLinFacturaProveedorView *subform )
{
    _depura ( "ListLinFacturaProveedorView_ListLinFacturaProveedorView", 0 );
    subform->addSubFormHeader ( "idtc_color", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "color" ) );
    subform->addSubFormHeader ( "idtc_talla", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Talla" ) );
    subform->addSubFormHeader ( "nomtc_color", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Nombre color" ) );
    subform->addSubFormHeader ( "nomtc_talla", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Nombre talla" ) );

    delete subform->m_delegate;
    subform->m_delegate = new QSubForm3BfDelegate ( subform );
    subform->mui_list->setItemDelegate ( subform->m_delegate );
    _depura ( "END ListLinFacturaProveedorView_ListLinFacturaProveedorView", 0 );
    return 0;
}




///
/**
\param subform
\return
**/
int ListLinAlbaranClienteView_ListLinAlbaranClienteView ( ListLinAlbaranClienteView *subform )
{
    _depura ( "ListLinAlbaranClienteView_ListLinAlbaranClienteView", 0 );
    subform->addSubFormHeader ( "idtc_color", BlDbField::DBint, BlDbField::DBNothing, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "color" ) );
    subform->addSubFormHeader ( "idtc_talla", BlDbField::DBint, BlDbField::DBNothing, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Talla" ) );
    subform->addSubFormHeader ( "nomtc_color", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Nombre color" ) );
    subform->addSubFormHeader ( "nomtc_talla", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Nombre talla" ) );

    delete subform->m_delegate;
    subform->m_delegate = new QSubForm3BfDelegate ( subform );
    subform->mui_list->setItemDelegate ( subform->m_delegate );
    _depura ( "END ListLinAlbaranClienteView_ListLinAlbaranClienteView", 0 );
    return 0;
}


int ListLinAlbaranClienteView_cargar ( ListLinAlbaranClienteView *subform )
{
    QString query = "SELECT * FROM lalbaran LEFT JOIN articulo AS t1 ON lalbaran.idarticulo = t1.idarticulo LEFT JOIN tc_color AS t2 on t2.idtc_color = lalbaran.idtc_color LEFT JOIN tc_talla AS t3 ON t3.idtc_talla= lalbaran.idtc_talla WHERE idalbaran=" + subform->mdb_idalbaran + "   ORDER BY ordenlalbaran";
    subform->BlSubForm::cargar ( query );
    return -1;
}


///
/**
\param subform
\return
**/
int ListLinFacturaView_ListLinFacturaView ( ListLinFacturaView *subform )
{
    _depura ( "ListLinFacturaView_ListLinFacturaView", 0 );
    subform->addSubFormHeader ( "idtc_color", BlDbField::DBint, BlDbField::DBNothing, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "color" ) );
    subform->addSubFormHeader ( "idtc_talla", BlDbField::DBint, BlDbField::DBNothing, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Talla" ) );
    subform->addSubFormHeader ( "nomtc_color", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Nombre color" ) );
    subform->addSubFormHeader ( "nomtc_talla", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Nombre talla" ) );

    delete subform->m_delegate;
    subform->m_delegate = new QSubForm3BfDelegate ( subform );
    subform->mui_list->setItemDelegate ( subform->m_delegate );
    _depura ( "END ListLinFacturaView_ListLinFacturaView", 0 );
    return 0;
}



int ListLinFacturaView_cargar ( ListLinFacturaView *subform )
{
    QString query = "SELECT * FROM lfactura LEFT JOIN articulo AS t1 ON lfactura.idarticulo = t1.idarticulo LEFT JOIN tc_color AS t2 on t2.idtc_color = lfactura.idtc_color LEFT JOIN tc_talla AS t3 ON t3.idtc_talla= lfactura.idtc_talla WHERE idfactura=" + subform->mdb_idfactura + "   ORDER BY ordenlfactura";
    subform->BlSubForm::cargar ( query );
    return -1;
}
