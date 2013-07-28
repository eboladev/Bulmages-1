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

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtCore/QObject>

#include "listlvartarifaview.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "bfbuscaralmacen.h"
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
    BL_FUNC_DEBUG
    
}


///
/**
**/
QSubFormVarTarifaBfDelegate::~QSubFormVarTarifaBfDelegate()
{
    BL_FUNC_DEBUG
    
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
    BL_FUNC_DEBUG
    BlSubFormHeader *linea;
    linea = m_subform->headerList() ->at ( index.column() );

    if ( linea->fieldName() == "nomtarifa" ) {
        BusquedaTarifa *editor = new BusquedaTarifa ( parent );
        editor->setMainCompany ( ( BfCompany * ) m_subform->mainCompany() );
	
        return editor;
    } else if ( linea->fieldName() == "nomalmacen" ) {
        BfBuscarAlmacen *editor = new BfBuscarAlmacen ( parent );
        editor->setMainCompany ( ( BfCompany * ) m_subform->mainCompany() );
	
        return editor;

    } else if ( linea->fieldName() == "cantidadmayoroigualque"
                || linea->fieldName() == "porcentajevariacion" ) {

        BlDoubleSpinBox * editor = new BlDoubleSpinBox ( parent );
        editor->setMinimum ( -1000000 );
        editor->setMaximum ( 1000000 );
	
        return editor;

    } else  {
        return BfSubFormDelegate::createEditor ( parent, option, index );
    } // end if
    
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
    BL_FUNC_DEBUG
    /// Si la fila o columna pasadas son invalidas salimos.
    if ( index.column() < 0 || index.row() < 0 )
        return;

    BlSubFormHeader *linea;
    linea = m_subform->headerList() ->at ( index.column() );
    if ( linea->fieldName() == "nomtarifa" ) {
        BusquedaTarifa *comboBox = static_cast<BusquedaTarifa*> ( editor );
        QString value = comboBox->currentText();
        QString idvalue = comboBox->id();
        model->setData ( index, value );
        m_subform->lineaat ( index.row() ) ->setDbValue ( "idtarifa", idvalue );
    } else if ( linea->fieldName() == "nomalmacen" ) {
        BfBuscarAlmacen *comboBox = static_cast<BfBuscarAlmacen*> ( editor );
        QString value = comboBox->currentText();
        QString idvalue = comboBox->id();
        model->setData ( index, value );
        m_subform->lineaat ( index.row() ) ->setDbValue ( "idalmacen", idvalue );

    } else if ( linea->fieldName() == "cantidadmayoroigualque"
                || linea->fieldName() == "porcentajevariacion" ) {

        BlDoubleSpinBox * spinBox = static_cast<BlDoubleSpinBox*> ( editor );
        spinBox->interpretText();
        QString value = spinBox->text();
        model->setData ( index, value );

    } else {
        BfSubFormDelegate::setModelData ( editor, model, index );
    } // end if
    
}


///
/**
\param editor
\param index
**/
void QSubFormVarTarifaBfDelegate::setEditorData ( QWidget* editor, const QModelIndex& index ) const
{
    BL_FUNC_DEBUG
    BlSubFormHeader *linea;
    linea = m_subform->headerList() ->at ( index.column() );
    if ( linea->fieldName() == "nomtarifa" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BusquedaTarifa *comboBox = static_cast<BusquedaTarifa*> ( editor );
        comboBox->setId ( m_subform->lineaat ( index.row() ) ->dbValue ( "idtarifa" ) );
    } else if ( linea->fieldName() == "nomalmacen" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BfBuscarAlmacen *comboBox = static_cast<BfBuscarAlmacen*> ( editor );
        comboBox->setId ( m_subform->lineaat ( index.row() ) ->dbValue ( "idalmacen" ) );

    } else if ( linea->fieldName() == "cantidadmayoroigualque"
                || linea->fieldName() == "porcentajevariacion" ) {

        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BlDoubleSpinBox *spinBox = static_cast<BlDoubleSpinBox*> ( editor );
        spinBox->setValue ( value.toDouble() );
        spinBox->selectAll();

    } else {
        BfSubFormDelegate::setEditorData ( editor, index );
    } // end if
    
}




