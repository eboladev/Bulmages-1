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

#include "listlvartarifaview.h"
#include "company.h"
#include "blfunctions.h"
#include "busquedaalmacen.h"
#include "busquedatarifa.h"


/// ============================= SUBFORM3BFDELEGATE =============================================
/// ===============================================================
///  Tratamientos del Item Delegate
/// ===============================================================


///
/**
\param parent
**/
QSubFormVarTarifaBfDelegate::QSubFormVarTarifaBfDelegate ( QObject *parent = 0 ) : BfSubFormDelegate ( parent )
{
    _depura ( "QSubFormVarTarifaBfDelegate::QSubFormVarTarifaBfDelegate", 0 );
    _depura ( "END QSubFormVarTarifaBfDelegate::QSubFormVarTarifaBfDelegate", 0 );
}


///
/**
**/
QSubFormVarTarifaBfDelegate::~QSubFormVarTarifaBfDelegate()
{
    _depura ( "QSubFormVarTarifaBfDelegate::~QSubFormVarTarifaBfDelegate", 0 );
    _depura ( "END QSubFormVarTarifaBfDelegate::~QSubFormVarTarifaBfDelegate", 0 );
}


///
/**
\param parent
\param option
\param index
\return
**/
QWidget *QSubFormVarTarifaBfDelegate::createEditor ( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    _depura ( "QSubFormVarTarifaBfDelegate::createEditor", 0 );
    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );

    if ( linea->nomcampo() == "nomtarifa" ) {
        BusquedaTarifa *editor = new BusquedaTarifa ( parent );
        editor->setMainCompany ( ( Company * ) m_subform->empresaBase() );
        return editor;
    } else if ( linea->nomcampo() == "nomalmacen" ) {
        BusquedaAlmacen *editor = new BusquedaAlmacen ( parent );
        editor->setMainCompany ( ( Company * ) m_subform->empresaBase() );
        return editor;

    } else if ( linea->nomcampo() == "cantidadmayoroigualque"
                || linea->nomcampo() == "porcentajevariacion") {

        BlDoubleSpinBox * editor = new BlDoubleSpinBox ( parent );
        editor->setMinimum ( -1000000 );
        editor->setMaximum ( 1000000 );
        _depura ( "END QSubFormVarTarifaBfDelegate::createEditor", 0, "QSPinBox" );
        return editor;

    } else  {
        return BfSubFormDelegate::createEditor ( parent, option, index );
    } // end if
    _depura ( "END QSubFormVarTarifaBfDelegate::createEditor", 0 );
}


///
/**
\param editor
\param model
\param index
\return
**/
void QSubFormVarTarifaBfDelegate::setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    _depura ( "QSubFormVarTarifaBfDelegate::setModelData", 0 );

    /// Si la fila o columna pasadas son invalidas salimos.
    if ( index.column() < 0 || index.row() < 0 )
        return;

    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    if ( linea->nomcampo() == "nomtarifa" ) {
        BusquedaTarifa *comboBox = static_cast<BusquedaTarifa*> ( editor );
        QString value = comboBox->currentText();
	QString idvalue = comboBox->idtarifa();
        model->setData ( index, value );
	m_subform->lineaat ( index.row() ) ->setDBvalue ( "idtarifa", idvalue );
    } else if ( linea->nomcampo() == "nomalmacen" ) {
        BusquedaAlmacen *comboBox = static_cast<BusquedaAlmacen*> ( editor );
        QString value = comboBox->currentText();
	QString idvalue = comboBox->id();
        model->setData ( index, value );
        m_subform->lineaat ( index.row() ) ->setDBvalue ( "idalmacen", idvalue );

    } else if ( linea->nomcampo() == "cantidadmayoroigualque"
                || linea->nomcampo() == "porcentajevariacion") {

        BlDoubleSpinBox * spinBox = static_cast<BlDoubleSpinBox*> ( editor );
        spinBox->interpretText();
        QString value = spinBox->text();
        model->setData ( index, value );

    } else {
        BfSubFormDelegate::setModelData ( editor, model, index );
    } // end if
    _depura ( "END QSubFormVarTarifaBfDelegate::setModelData", 0 );
}


///
/**
\param editor
\param index
**/
void QSubFormVarTarifaBfDelegate::setEditorData ( QWidget* editor, const QModelIndex& index ) const
{
    _depura ( "QSubFormVarTarifaBfDelegate::setEditorData", 0 );
    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    if ( linea->nomcampo() == "nomtarifa" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BusquedaTarifa *comboBox = static_cast<BusquedaTarifa*> ( editor );
	comboBox->setIdTarifa(m_subform->lineaat ( index.row() ) ->DBvalue("idtarifa"));
    } else if ( linea->nomcampo() == "nomalmacen" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BusquedaAlmacen *comboBox = static_cast<BusquedaAlmacen*> ( editor );
	comboBox->setId(m_subform->lineaat ( index.row() ) ->DBvalue("idalmacen"));

    } else if ( linea->nomcampo() == "cantidadmayoroigualque"
                || linea->nomcampo() == "porcentajevariacion") {

        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BlDoubleSpinBox *spinBox = static_cast<BlDoubleSpinBox*> ( editor );
        spinBox->setValue ( value.toDouble() );
        spinBox->selectAll();

    } else {
        BfSubFormDelegate::setEditorData ( editor, index );
    } // end if
    _depura ( "END QSubFormVarTarifaBfDelegate::setEditorData", 0 );
}




