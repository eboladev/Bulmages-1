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

#include "bcsubform.h"
#include "blfunctions.h"
#include "bccompany.h"
#include "bltexteditdelegate.h"
#include "bldoublespinbox.h"
#include "bcbuscarcanal.h"
#include "bcbuscarcentrocoste.h"

/// Constructor de la clase
/**
\param parent Widget padre del SubFormulario
**/
BcSubForm::BcSubForm ( QWidget *parent ) : BlSubForm ( parent )
{
    BL_FUNC_DEBUG
    m_delegate = new BcSubFormDelegate ( this );
    mui_list->setItemDelegate ( m_delegate );
    
}


/// Destructor del Subformulario
/**
Libera la memoria que se haya reservado durante el funcionamiento de la clase.
**/
BcSubForm::~BcSubForm()
{
    BL_FUNC_DEBUG
    delete m_delegate;
    
}


///
/**
\param c
**/
void BcSubForm::setMainCompany ( BlMainCompany *c )
{
    BL_FUNC_DEBUG
    BlMainCompanyPointer::setMainCompany ( c );
    m_delegate->setMainCompany ( c );
    
}


///
/**
\return
**/
BcCompany *BcSubForm::mainCompany()
{
    BL_FUNC_DEBUG
    
    return ( ( BcCompany * ) BlMainCompanyPointer::mainCompany() );
}


/// Se ha pulsado la combinacion de teclas Ctrl + +
/**
\param row Fila en la que se ha hecho la pulsacion
\param col Columna en la que se ha hecho la pulsacion
\return
**/
void BcSubForm::pressedPlus ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp )
{
    BL_FUNC_DEBUG

    /// Si no es un campo de tipo debe o haber salimos.
    if ( camp->fieldName() == "fecha" ) {
        BlDbSubFormRecord *recant = lineaat ( row - 1 );
        if ( recant ) {
            rec->setDbValue ( "fecha", recant->dbValue ( "fecha" ) );
        } // end if
        return;
    } // end if

    if ( camp->fieldName() == "debe" || camp->fieldName() == "haber" ) {

        /// Ponemos los campos a cero en esta fila
        rec->setDbValue ( "debe", "0" );
        rec->setDbValue ( "haber", "0" );

        /// Hacemos las sumas y las restamos
        BlFixed debe = sumarCampo ( "debe" );
        BlFixed haber = sumarCampo ( "haber" );
        BlFixed result = debe - haber;

        /// Segun el resultado imputamos al debe o al haber para que la cosa cuadre.
        if ( result > 0 ) {
            rec->setDbValue ( "haber", result.toQString() );
        } // end if

        if ( result < 0 ) {
            result = result * -1;
            rec->setDbValue ( "debe", result.toQString() );
        } // end if
        return;
    } // end if

    BlSubForm::pressedPlus(row, col, rec, camp);

}

/// Se ha pulsado la combinacion de teclas Ctrl + *
/**
\param row Fila en la que se ha hecho la pulsacion
\param col Columna en la que se ha hecho la pulsacion
\return
**/
void BcSubForm::pressedAsterisk ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp )
{
    BL_FUNC_DEBUG

    /// Si no es un campo de tipo codigo salimos.
    if ( camp->fieldName() != "codigo" && camp->fieldName() != "codigoctacliente" )
        return;

    /// Nos llevamos el foco para que no haya un EditorDelegado que no se actualice bien.
    mui_list->setCurrentCell ( row, col + 1 );

    QString codigo = "";
    QString *pCodigo = &codigo;

    /// Disparamos los plugins.
    
    int res = g_plugins->run ( "BcSubForm_pressedAsterisk", this, ( void** ) &pCodigo );

    if ( res != 0 ) {
        return;
    } // end if
    
    if ( codigo != "" ) {
        QString query = "SELECT * FROM cuenta WHERE codigo = '" + codigo + "'";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            if ( camp->fieldName() == "codigo" ) {
                rec->setDbValue ( "idcuenta", cur->value( "idcuenta" ) );
                rec->setDbValue ( "codigo", cur->value( "codigo" ) );
                rec->setDbValue ( "tipocuenta", cur->value( "tipocuenta" ) );
                rec->setDbValue ( "descripcion", cur->value( "descripcion" ) );
                if ( rec->exists ( "idc_coste" ) && cur->value( "idc_coste" ) != "" ) {
                    rec->setDbValue ( "idc_coste", cur->value( "idc_coste" ) );
                    QString query1 = "SELECT * FROM c_coste WHERE idc_coste = " + cur->value( "idc_coste" );
                    BlDbRecordSet *curss = mainCompany() ->loadQuery ( query1 );
                    rec->setDbValue ( "nomc_coste", curss->value( "nombre" ) );
                    delete curss;
                } // end if

            } // end if
            if ( camp->fieldName() == "codigoctacliente" ) {
                rec->setDbValue ( "idctacliente", cur->value( "idcuenta" ) );
                rec->setDbValue ( "codigoctacliente", cur->value( "codigo" ) );
                rec->setDbValue ( "tipoctacliente", cur->value( "tipocuenta" ) );
                rec->setDbValue ( "nomctacliente", cur->value( "descripcion" ) );

            } // end if
        } // end if
        delete cur;
    } // end if

    
}


/// Se ha pulsado Ctrl + / en el subformulario
/**
\param row Fila en la que se ha hecho la pulsacion
\param col1 Columna en la que se ha hecho la pulsacion
\return
**/
void BcSubForm::pressedSlash ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp )
{
    BL_FUNC_DEBUG

    if ( camp->fieldName() == "fecha" ) {
        if ( row > 0 ) {
            BlDbSubFormField * campoant = ( BlDbSubFormField * ) item ( row - 1, col );
            camp->set ( campoant->text() );
        } // end if
        return;
    } // end if
    if ( camp->fieldName() == "descripcion" ) {
        if ( row > 0 ) {
            BlDbSubFormField * campoant = ( BlDbSubFormField * ) item ( row - 1, col );
            camp->set ( campoant->text() );
        } // end if
        return;
    } // end if
    if ( camp->fieldName() == "conceptocontable" ) {
        if ( row > 0 ) {
            BlDbSubFormField * campoant = ( BlDbSubFormField * ) item ( row - 1, col );
            camp->set ( campoant->text() );
        } // end if
        return;
    } // end if
    QString text = blTextEditor ( camp->text() );
    camp->set ( text );
    
}


/// Slot que se activa al cambiar de celda dentro del SubFormulario
/**
\param row
\param col
\return
**/
void BcSubForm::editFinished ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp )
{
    BL_FUNC_DEBUG

    /// Disparamos los plugins.
    int res = g_plugins->run ( "BcSubForm_on_mui_list_cellChanged", this );
    if ( res != 0 ) {
        return;
    } // end if

    if ( camp->fieldName() == "codigo" && camp->text() != "*" ) {
        QString codigoext = blExtendStringWithZeros ( camp->text(), ( ( BcCompany * ) mainCompany() ) ->numDigitosEmpresa() );
        QString query = "SELECT idcuenta, codigo, tipocuenta, descripcion, idc_coste FROM cuenta WHERE codigo = '" + codigoext + "'";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            rec->setDbValue ( "idcuenta", cur->value( "idcuenta" ) );
            rec->setDbValue ( "codigo", cur->value( "codigo" ) );
            rec->setDbValue ( "tipocuenta", cur->value( "tipocuenta" ) );
            rec->setDbValue ( "descripcioncuenta", cur->value( "descripcion" ) );
            if ( rec->exists ( "idc_coste" ) && cur->value( "idc_coste" ) != "" ) {
                rec->setDbValue ( "idc_coste", cur->value( "idc_coste" ) );
                QString query1 = "SELECT * FROM c_coste WHERE idc_coste = " + cur->value( "idc_coste" );
                BlDbRecordSet *curss = mainCompany() ->loadQuery ( query1 );
                rec->setDbValue ( "nomc_coste", curss->value( "nombre" ) );
                delete curss;
            } // end if
        } else {
	    BlDebug::blDebug ( Q_FUNC_INFO, 0, _("No existe la cuenta.") );
            return;
        } // end if
        delete cur;
    } // end if
    if ( camp->fieldName() == "nomcanal" ) {
        QString query = "SELECT idcanal FROM canal WHERE nombre = '" + camp->text() + "'";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            rec->setDbValue ( "idcanal", cur->value( "idcanal" ) );
        } else {
            rec->setDbValue ( "idcanal", "" );
        } // end if
        delete cur;
    } // end if
    if ( camp->fieldName() == "nomc_coste" ) {
        QString query = "SELECT idc_coste FROM c_coste WHERE nombre = '" + camp->text() + "'";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            rec->setDbValue ( "idc_coste", cur->value( "idc_coste" ) );
        } else {
            rec->setDbValue ( "idc_coste", "" );
        } // end if
        delete cur;
    } // end if
    if ( camp->fieldName() == "fecha" ) {
        QString nfecha = blNormalizeDate ( camp->text() ).toString ( "dd/MM/yyyy" );
        rec->setDbValue ( "fecha", nfecha );
    } // end if
    if ( camp->fieldName() == "debe" ) {
        if ( BlFixed ( camp->text() ) != BlFixed ( "0.00" ) ) {
            rec->setDbValue ( "haber", "0.00" );
        } // end if
    } // end if
    if ( camp->fieldName() == "haber" ) {
        if ( BlFixed ( camp->text() ) != BlFixed ( "0.00" ) ) {
            rec->setDbValue ( "debe", "0.00" );
        } // end if
    } // end if

    g_plugins->run ( "BcSubForm_on_mui_list_cellChanged_post", this );

    BlSubForm::on_mui_list_cellChanged ( row, col );
    
}



/// Se ha dado la orden de crear el menu contextual. Y esta funcion funciona con
/// Un sistema en cascada en lugar del tipico signal-slot
/**
\param menu
**/
void BcSubForm::createMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
}


///
/**
**/
void BcSubForm::execMenuAction ( QAction * )
{
    BL_FUNC_DEBUG
    
}


/// ===============================================================
///  Tratamientos del Item Delegate
/// ===============================================================
///
/**
\param parent
**/
BcSubFormDelegate::BcSubFormDelegate ( QObject *parent = 0 ) : BlSubFormDelegate ( parent )
{
    BL_FUNC_DEBUG
}


///
/**
**/
BcSubFormDelegate::~BcSubFormDelegate()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param parent
\param index
**/
QWidget *BcSubFormDelegate::createEditor ( QWidget *parent, const QStyleOptionViewItem &opcion, const QModelIndex &index ) const
{
    BL_FUNC_DEBUG
    BlSubFormHeader *linea;
    linea = m_subform->headerList() ->at ( index.column() );
    BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("Columna: '%1', fila: '%2'.")).arg(QString::number(index.column())).arg(QString::number(index.row())) );

    if ( linea->fieldName().startsWith ( "desc" ) ) {
        BlTextEditDelegate * editor = new BlTextEditDelegate ( parent );
        editor->setObjectName ( "BlTextEditDelegate" );
	
        return editor;
    } else if ( linea->fieldName() == "debe" || linea->fieldName() == "haber" ) {
        BlDoubleSpinBox * editor = new BlDoubleSpinBox ( parent );
        editor->setMinimum ( -100000000 );
        editor->setMaximum ( 100000000 );
        editor->setDecimals(linea->numericPrecision());
	
        return editor;
    } else if ( linea->fieldName() == "nomcanal" ) {
        BcBuscarCanalDelegate * editor = new BcBuscarCanalDelegate ( parent );
        editor->setMainCompany ( m_subform->mainCompany() );
	
        return editor;
    } else if ( linea->fieldName() == "nomc_coste" ) {
        BcBuscarCentroCosteDelegate * editor = new BcBuscarCentroCosteDelegate ( parent );
        editor->setMainCompany ( m_subform->mainCompany() );
	
        return editor;
    } else if ( linea->fieldName().startsWith ( "fecha" ) ) {
        BlDateLineEdit * editor = new BlDateLineEdit ( parent );
	
        return editor;
    } else {
        return BlSubFormDelegate::createEditor ( parent, opcion, index );
    } // end if
}


///
/**
\param editor
\param model
\param index
\return
**/
void BcSubFormDelegate::setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    BL_FUNC_DEBUG

    /// Si la fila o columna pasadas son invalidas salimos.
    if ( index.column() < 0 || index.row() < 0 ) {
        return;
    } // end if

    BlSubFormHeader *linea;
    linea = m_subform->headerList() ->at ( index.column() );
    if ( linea->fieldName().startsWith ( "desc" ) ) {
        BlTextEditDelegate * textedit = qobject_cast<BlTextEditDelegate *> ( editor );
        model->setData ( index, textedit->toPlainText() );
    } else if ( linea->fieldName() == "debe" || linea->fieldName() == "haber" ) {
        BlDoubleSpinBox * spinBox = static_cast<BlDoubleSpinBox*> ( editor );
        spinBox->interpretText();
        QString value = spinBox->text();
        model->setData ( index, value );
    } else if ( linea->fieldName() == "nomcanal" ) {
        BcBuscarCanalDelegate * comboBox = static_cast<BcBuscarCanalDelegate*> ( editor );
        QString value = comboBox->currentText();
        model->setData ( index, value );
        m_subform->lineaat ( index.row() ) ->setDbValue ( "idcanal", comboBox->id(value) );
    } else if ( linea->fieldName() == "nomc_coste" ) {
        BcBuscarCentroCosteDelegate * comboBox = static_cast<BcBuscarCentroCosteDelegate*> ( editor );
        QString value = comboBox->currentText();
        model->setData ( index, value );
        m_subform->lineaat ( index.row() ) ->setDbValue ( "idc_coste", comboBox->id(value) );
    } else if ( linea->fieldName().startsWith ( "fecha" ) ) {
        BlDateLineEdit * comboBox = static_cast<BlDateLineEdit*> ( editor );
        QString value = ( ( QLineEdit * ) comboBox ) ->text();
        model->setData ( index, value );
    } else {
        BlSubFormDelegate::setModelData ( editor, model, index );
    } // end if
}


///
/**
\param editor
\param index
**/
void BcSubFormDelegate::setEditorData ( QWidget *editor, const QModelIndex &index ) const
{
    BL_FUNC_DEBUG
    BlSubFormHeader *linea;
    linea = m_subform->headerList() ->at ( index.column() );
    if ( linea->fieldName().startsWith ( "desc" ) ) {
        QString data = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BlTextEditDelegate *textedit = qobject_cast<BlTextEditDelegate*> ( editor );
        textedit->setText ( data );
        textedit->selectAll();
    } else if ( linea->fieldName() == "debe" || linea->fieldName() == "haber" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BlDoubleSpinBox *spinBox = static_cast<BlDoubleSpinBox*> ( editor );
        spinBox->setValue ( value.toDouble() );
        spinBox->selectAll();
    } else if ( linea->fieldName() == "nomcanal" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BcBuscarCanalDelegate *comboBox = static_cast<BcBuscarCanalDelegate*> ( editor );
        comboBox->setId ( value, "nombre" );
    } else if ( linea->fieldName() == "nomc_coste" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BcBuscarCentroCosteDelegate *comboBox = static_cast<BcBuscarCentroCosteDelegate*> ( editor );
        comboBox->setId ( value, "nombre" );
    } else if ( linea->fieldName().startsWith ( "fecha" ) ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BlDateLineEdit *bf = static_cast<BlDateLineEdit*> ( editor );
        ( ( QLineEdit * ) bf ) ->setText ( value );
        ( ( QLineEdit * ) bf ) ->selectAll();
    } else {
        BlSubFormDelegate::setEditorData ( editor, index );
    } // end if
}




