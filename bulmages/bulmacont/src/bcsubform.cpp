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
    _depura ( "BcSubForm::BcSubForm", 0 );
    m_delegate = new BcSubFormDelegate ( this );
    mui_list->setItemDelegate ( m_delegate );
    _depura ( "END BcSubForm::BcSubForm", 0 );
}


/// Destructor del Subformulario
/**
Libera la memoria que se haya reservado durante el funcionamiento de la clase.
**/
BcSubForm::~BcSubForm()
{
    _depura ( "BcSubForm::~BcSubForm", 0 );
    delete m_delegate;
    _depura ( "END BcSubForm::~BcSubForm", 0 );
}


///
/**
\param c
**/
void BcSubForm::setMainCompany ( BlMainCompany *c )
{
    _depura ( "BcSubForm::setMainCompany", 0 );
    BlMainCompanyPointer::setMainCompany ( c );
    m_delegate->setMainCompany ( c );
    _depura ( "END BcSubForm::setMainCompany", 0 );
}


///
/**
\return
**/
BcCompany *BcSubForm::mainCompany()
{
    _depura ( "BcSubForm::mainCompany", 0 );
    _depura ( "END BcSubForm::mainCompany", 0 );
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
    _depura ( "BcSubForm::pressedPlus", 0 );

    /// Si no es un campo de tipo debe o haber salimos.
    /*
        if ( camp->nomcampo() != "debe" && camp->nomcampo() != "haber" && camp->nomcampo() != "fecha" ) {
            _depura ( "END BcSubForm::pressedPlus", 0, "Campo incorrecto" );
            return;
        } // end if
    */

    if ( camp->nomcampo() == "fecha" ) {
        BlDbSubFormRecord *recant = lineaat ( row - 1 );
        if ( recant ) {
            rec->setDbValue ( "fecha", recant->dbValue ( "fecha" ) );
        } // end if
        return;
    } // end if

    if ( camp->nomcampo() == "debe" || camp->nomcampo() == "haber" ) {

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

    BlDbSubFormRecord *recant = lineaat ( row - 1 );
    if ( recant ) {
        rec->setDbValue ( camp->nomcampo(), recant->dbValue ( camp->nomcampo() ) );
        return;
    } // end if


    _depura ( "END BcSubForm::pressedPlus", 0 );
}

/// Se ha pulsado la combinacion de teclas Ctrl + *
/**
\param row Fila en la que se ha hecho la pulsacion
\param col Columna en la que se ha hecho la pulsacion
\return
**/
void BcSubForm::pressedAsterisk ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp )
{
    _depura ( "BcSubForm::pressedAsterisk", 0 );

    /// Si no es un campo de tipo codigo salimos.
    if ( camp->nomcampo() != "codigo" && camp->nomcampo() != "codigoctacliente" )
        return;

    /// Nos llevamos el foco para que no haya un EditorDelegado que no se actualice bien.

    mui_list->setCurrentCell ( row, col + 1 );
    QDialog *diag = new QDialog ( 0 );
    diag->setModal ( true );
    ///TODO: De esta manera se recarga de la base de datos toda la info de las cuentas cada
    /// vez que se necesita la lista de cuentas. Hay que buscar la manera de que este siempre
    /// disponible para no cargar el trabajo a la red ni al gestor de base de datos.
/* TBR
    BcCuentaListView *listcuentas = new BcCuentaListView ( ( BcCompany * ) mainCompany(), diag, 0, BL_SELECT_MODE );
    connect ( listcuentas, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );
*/
    diag->exec();
// TBR
//    QString codigo = listcuentas->codigocuenta();
    QString codigo = "";
    delete diag;

    if ( codigo != "" ) {
        QString query = "SELECT * FROM cuenta WHERE codigo = '" + codigo + "'";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            if ( camp->nomcampo() == "codigo" ) {
                rec->setDbValue ( "idcuenta", cur->valor ( "idcuenta" ) );
                rec->setDbValue ( "codigo", cur->valor ( "codigo" ) );
                rec->setDbValue ( "tipocuenta", cur->valor ( "tipocuenta" ) );
                rec->setDbValue ( "descripcion", cur->valor ( "descripcion" ) );
                if ( rec->exists ( "idc_coste" ) && cur->valor ( "idc_coste" ) != "" ) {
                    rec->setDbValue ( "idc_coste", cur->valor ( "idc_coste" ) );
                    QString query1 = "SELECT * FROM c_coste WHERE idc_coste = " + cur->valor ( "idc_coste" );
                    BlDbRecordSet *curss = mainCompany() ->loadQuery ( query1 );
                    rec->setDbValue ( "nomc_coste", curss->valor ( "nombre" ) );
                    delete curss;
                } // end if

            } // end if
            if ( camp->nomcampo() == "codigoctacliente" ) {
                rec->setDbValue ( "idctacliente", cur->valor ( "idcuenta" ) );
                rec->setDbValue ( "codigoctacliente", cur->valor ( "codigo" ) );
                rec->setDbValue ( "tipoctacliente", cur->valor ( "tipocuenta" ) );
                rec->setDbValue ( "nomctacliente", cur->valor ( "descripcion" ) );

            } // end if
        } // end if
        delete cur;
    } // end if


    _depura ( "END BcSubForm::pressedAsterisk", 0 );
}


/// Se ha pulsado Ctrl + / en el subformulario
/**
\param row Fila en la que se ha hecho la pulsacion
\param col1 Columna en la que se ha hecho la pulsacion
\return
**/
void BcSubForm::pressedSlash ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp )
{
    _depura ( "BcSubForm::pressedSlash", 0 );

    if ( camp->nomcampo() == "fecha" ) {
        if ( row > 0 ) {
            BlDbSubFormField * campoant = ( BlDbSubFormField * ) item ( row - 1, col );
            camp->set ( campoant->text() );
        } // end if
        return;
    } // end if
    if ( camp->nomcampo() == "descripcion" ) {
        if ( row > 0 ) {
            BlDbSubFormField * campoant = ( BlDbSubFormField * ) item ( row - 1, col );
            camp->set ( campoant->text() );
        } // end if
        return;
    } // end if
    if ( camp->nomcampo() == "conceptocontable" ) {
        if ( row > 0 ) {
            BlDbSubFormField * campoant = ( BlDbSubFormField * ) item ( row - 1, col );
            camp->set ( campoant->text() );
        } // end if
        return;
    } // end if
    QString text = editaTexto ( camp->text() );
    camp->set ( text );
    _depura ( "END BcSubForm::pressedSlash", 0 );
}


/// Slot que se activa al cambiar de celda dentro del SubFormulario
/**
\param row
\param col
\return
**/
void BcSubForm::editFinished ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp )
{
    _depura ( "BcSubForm::editFinished", 0, "Row: " + QString::number ( row ) + " Col: " + QString::number ( col ) );

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "BcSubForm_on_mui_list_cellChanged", this );
    if ( res != 0 ) {
        return;
    } // end if

    if ( camp->nomcampo() == "codigo" && camp->text() != "*" ) {
        QString codigoext = extiendecodigo ( camp->text(), ( ( BcCompany * ) mainCompany() ) ->numdigitosempresa() );
        QString query = "SELECT idcuenta, codigo, tipocuenta, descripcion, idc_coste FROM cuenta WHERE codigo = '" + codigoext + "'";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            rec->setDbValue ( "idcuenta", cur->valor ( "idcuenta" ) );
            rec->setDbValue ( "codigo", cur->valor ( "codigo" ) );
            rec->setDbValue ( "tipocuenta", cur->valor ( "tipocuenta" ) );
            rec->setDbValue ( "descripcioncuenta", cur->valor ( "descripcion" ) );
            if ( rec->exists ( "idc_coste" ) && cur->valor ( "idc_coste" ) != "" ) {
                rec->setDbValue ( "idc_coste", cur->valor ( "idc_coste" ) );
                QString query1 = "SELECT * FROM c_coste WHERE idc_coste = " + cur->valor ( "idc_coste" );
                BlDbRecordSet *curss = mainCompany() ->loadQuery ( query1 );
                rec->setDbValue ( "nomc_coste", curss->valor ( "nombre" ) );
                delete curss;
            } // end if
        } else {
            _depura ( "No existe cuenta", 2 );
            return;
        } // end if
        delete cur;
    } // end if
    if ( camp->nomcampo() == "nomcanal" ) {
        QString query = "SELECT idcanal FROM canal WHERE nombre = '" + camp->text() + "'";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            rec->setDbValue ( "idcanal", cur->valor ( "idcanal" ) );
        } else {
            rec->setDbValue ( "idcanal", "" );
        } // end if
        delete cur;
    } // end if
    if ( camp->nomcampo() == "nomc_coste" ) {
        QString query = "SELECT idc_coste FROM c_coste WHERE nombre = '" + camp->text() + "'";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            rec->setDbValue ( "idc_coste", cur->valor ( "idc_coste" ) );
        } else {
            rec->setDbValue ( "idc_coste", "" );
        } // end if
        delete cur;
    } // end if
    if ( camp->nomcampo() == "fecha" ) {
        QString nfecha = normalizafecha ( camp->text() ).toString ( "dd/MM/yyyy" );
        rec->setDbValue ( "fecha", nfecha );
    } // end if
    if ( camp->nomcampo() == "debe" ) {
        if ( BlFixed ( camp->text() ) != BlFixed ( "0.00" ) ) {
            rec->setDbValue ( "haber", "0.00" );
        } // end if
    } // end if
    if ( camp->nomcampo() == "haber" ) {
        if ( BlFixed ( camp->text() ) != BlFixed ( "0.00" ) ) {
            rec->setDbValue ( "debe", "0.00" );
        } // end if
    } // end if

    g_plugins->lanza ( "BcSubForm_on_mui_list_cellChanged_post", this );

    BlSubForm::on_mui_list_cellChanged ( row, col );
    _depura ( "END BcSubForm::on_mui_list_cellChanged", 0 );
}


/// Muestra la ventana de asiento.
/**
**/
void BcSubForm::boton_asiento()
{
    _depura ( "BcSubForm::boton_asiento", 0 );
/*
    BcCompany *companyact = ( BcCompany * ) mainCompany();
    QString numasiento = dbValue ( "idasiento" );
    if ( numasiento != "" ) {
        companyact->intapuntsempresa() ->muestraasiento ( numasiento.toInt() );
        companyact->muestraapuntes1();
    } // end if
*/
    _depura ( "END BcSubForm::boton_asiento", 0 );
}


/// Si el parametro pasado es un:
/// 0 -> del d&iacute;a actual
/// 1 -> del mes actual
/// 2 -> del a&ntilde;o actual
/**
\param tipo
**/
void BcSubForm::boton_extracto1 ( int tipo )
{
    _depura ( "BcSubForm::boton_extracto1", 0 );
/*
    BcCompany *companyact = ( BcCompany * ) mainCompany();
    QDate fecha1, fecha2, fechaact;
    QString fecha = dbValue ( "fecha" ).left ( 10 );
    QString codigo = dbValue ( "codigo" );
    if ( fecha != "" && codigo != "" ) {
        fechaact = normalizafecha ( fecha );
        switch ( tipo ) {
        case 0:
            fecha1.setYMD ( fechaact.year(), fechaact.month(), fechaact.day() );
            fecha2.setYMD ( fechaact.year(), fechaact.month(), fechaact.day() );
            break;
        case 1:
            fecha1.setYMD ( fechaact.year(), fechaact.month(), 1 );
            fecha2.setYMD ( fechaact.year(), fechaact.month(), fechaact.daysInMonth() );
            break;
        case 2:
            fecha1.setYMD ( fechaact.year(), 1, 1 );
            fecha2.setYMD ( fechaact.year(), 12, 31 );
            break;
        } // end switch
        companyact->extractoempresa() ->inicializa1 ( codigo, codigo, fecha1.toString ( "dd/MM/yyyy" ), fecha2.toString ( "dd/MM/yyyy" ), 0 );
        companyact->extractoempresa() ->accept();
        companyact->libromayor();
    } // end if
*/
    _depura ( "END BcSubForm::boton_extracto1", 0 );
}


/// Si el parametro pasado es un:
/// 0 -> del d&iacute;a actual
/// 1 -> del mes actual
/// 2 -> del a&ntilde;o actual
/**
\param tipo
**/
void BcSubForm::boton_diario1 ( int tipo )
{
    _depura ( "BcSubForm::boton_diario1", 0 );
/*
    BcCompany *companyact = ( BcCompany * ) mainCompany();
    QDate fecha1, fecha2, fechaact, fechaact1;
    fechaact = normalizafecha ( dbValue ( "fecha" ).left ( 10 ) );
    fechaact1 = normalizafecha ( dbValue ( "fecha" ).left ( 10 ) );
    if ( dbValue ( "fecha" ).left ( 10 ) != "" ) {
        switch ( tipo ) {
        case 0:
            fecha1.setYMD ( fechaact.year(), fechaact.month(), fechaact.day() );
            fecha2.setYMD ( fechaact1.year(), fechaact1.month(), fechaact1.day() );
            break;
        case 1:
            fecha1.setYMD ( fechaact.year(), fechaact.month(), 1 );
            fecha2.setYMD ( fechaact.year(), fechaact.month(), fechaact.daysInMonth() );
            break;
        case 2:
            fecha1.setYMD ( fechaact.year(), 1, 1 );
            fecha2.setYMD ( fechaact.year(), 12, 31 );
            break;
        } // end switch
        companyact->diarioempresa() ->inicializa1 ( ( char * ) fecha1.toString ( "dd/MM/yyyy" ).toAscii().constData(), ( char * ) fecha2.toString ( "dd/MM/yyyy" ).toAscii().constData(), 0 );
    } // end if
    companyact->diarioempresa() ->accept();
    companyact->librodiario();
*/
    _depura ( "BcSubForm::boton_diario1", 0 );
}


/// Si el parametro pasado es un:
/// 0 -> del per&iacute;odo actual.
/// 1 -> del mes actual mirado a partir de la fecha de inicio.
/// 2 -> del a&ntilde;o actual mirado a partir de la fecha de inicio.
/**
\param tipo
**/
/*
void BcSubForm::boton_balance1 ( int tipo )
{
    _depura ( "BcSubForm::boton_balance1", 0 );
    BcCompany *companyact = ( BcCompany * ) mainCompany();
    QString fecha = dbValue ( "fecha" ).left ( 10 );
    QString codigo = dbValue ( "codigo" );
    QDate fecha1, fecha2, fechaact, fechaact1;
    if ( fecha != "" && codigo != "" ) {
        fechaact = normalizafecha ( fecha );
        switch ( tipo ) {
        case 0:
            fecha1.setYMD ( fechaact.year(), fechaact.month(), fechaact.day() );
            fecha2.setYMD ( fechaact1.year(), fechaact.month(), fechaact.day() );
            break;
        case 1:
            fecha1.setYMD ( fechaact.year(), fechaact.month(), 1 );
            fecha2.setYMD ( fechaact.year(), fechaact.month(), fechaact.daysInMonth() );
            break;
        case 2:
            fecha1.setYMD ( fechaact.year(), 1, 1 );
            fecha2.setYMD ( fechaact.year(), 12, 31 );
            break;
        } // end switch
        mainCompany() ->balanceempresa() ->inicializa1 ( codigo, codigo, fecha1.toString ( "dd/MM/yyyy" ), fecha2.toString ( "dd/MM/yyyy" ), "0" );
        companyact->balanceempresa() ->accept();
        companyact->librobalance();
    } // end if
    _depura ( "END BcSubForm::boton_balance1", 0 );
}
*/


/// Se ha dado la orden de crear el menu contextual. Y esta funcion funciona con
/// Un sistema en cascada en lugar del tipico signal-slot
/**
\param menu
**/
void BcSubForm::creaMenu ( QMenu *menu )
{
    _depura ( "BcSubForm::pintaMenu", 0 );
    menu->addAction ( _ ( "Submenu de contabilidad" ) );
    menu->addSeparator();
    _depura ( "END BcSubForm::pintaMenu", 0 );
}


///
/**
**/
void BcSubForm::procesaMenu ( QAction * )
{
    _depura ( "BcSubForm::procesaMenu", 0 );
    _depura ( "END BcSubForm::procesaMenu", 0 );
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
    _depura ( "BcSubFormDelegate::BcSubFormDelegate", 0 );
//    m_subform = ( BlSubForm * ) parent;
//    installEventFilter ( this );
    _depura ( "END BcSubFormDelegate::BcSubFormDelegate", 0 );
}


///
/**
**/
BcSubFormDelegate::~BcSubFormDelegate()
{
    _depura ( "BcSubFormDelegate::~BcSubFormDelegate", 0 );
    _depura ( "END BcSubFormDelegate::~BcSubFormDelegate", 0 );
}


///
/**
\param parent
\param index
**/
QWidget *BcSubFormDelegate::createEditor ( QWidget *parent, const QStyleOptionViewItem &opcion, const QModelIndex &index ) const
{
    _depura ( "BcSubFormDelegate::createEditor", 0 );
    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    _depura ( "BcSubFormDelegate::createEditor", 0, "CurrentColumn: " + QString::number ( index.column() ) );
    _depura ( "BcSubFormDelegate::createEditor", 0, "CurrentRow: " + QString::number ( index.row() ) );

    if ( linea->nomcampo().startsWith ( "desc" ) ) {
        BlTextEditDelegate * editor = new BlTextEditDelegate ( parent );
        editor->setObjectName ( "BlTextEditDelegate" );
        _depura ( "END BcSubFormDelegate::createEditor", 0, "BlTextEditDelegate" );
        return editor;
    } else if ( linea->nomcampo() == "debe" || linea->nomcampo() == "haber" ) {
        BlDoubleSpinBox * editor = new BlDoubleSpinBox ( parent );
        editor->setMinimum ( -1000000 );
        editor->setMaximum ( 1000000 );
        _depura ( "END BcSubFormDelegate::createEditor", 0, "BlDoubleSpinBox" );
        return editor;
    } else if ( linea->nomcampo() == "nomcanal" ) {
        BcBuscarCanalDelegate * editor = new BcBuscarCanalDelegate ( parent );
        editor->setMainCompany ( m_subform->mainCompany() );
        _depura ( "END BcSubFormDelegate::createEditor", 0, "BcBuscarCanalDelegate" );
        return editor;
    } else if ( linea->nomcampo() == "nomc_coste" ) {
        BcBuscarCentroCosteDelegate * editor = new BcBuscarCentroCosteDelegate ( parent );
        editor->setMainCompany ( m_subform->mainCompany() );
        _depura ( "END BcSubFormDelegate::createEditor", 0, "BcBuscarCentroCosteDelegate" );
        return editor;
    } else if ( linea->nomcampo().startsWith ( "fecha" ) ) {
        BlDateLineEdit * editor = new BlDateLineEdit ( parent );
        _depura ( "END BcSubFormDelegate::createEditor", 0, "BlDateLineEdit" );
        return editor;
    } else {
        /// DbInt = 1, DbVarChar = 2, DbDate = 3, DbNumeric = 4, DbBoolean
        //if (linea->dbFieldType() == BlDbField::DbInt) {
        //QSpinBox *editor = new QSpinBox(parent);
        //return editor;
        /*        QLineEdit *editor = new QLineEdit ( parent );
                _depura ( "END BcSubFormDelegate::createEditor", 0, "QLineEdit" );
                return editor;
        */
        //} else {
        return BlSubFormDelegate::createEditor ( parent, opcion, index );
        //} // end if
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
    _depura ( "BcSubFormDelegate::setModelData", 0 );

    /// Si la fila o columna pasadas son invalidas salimos.
    if ( index.column() < 0 || index.row() < 0 ) {
        _depura ( "END BcSubFormDelegate::setModelData", 0, "Fila o columna invalidas" );
        return;
    } // end if

    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    if ( linea->nomcampo().startsWith ( "desc" ) ) {

//    if (linea->nomcampo() == "desc" + m_subform->tableName()) {
        BlTextEditDelegate * textedit = qobject_cast<BlTextEditDelegate *> ( editor );
        model->setData ( index, textedit->toPlainText() );

//    Este return sobra.
//        return;

        // TODO: 04/05/07 Se quita esta linea porque hacia que los valores en la celda "haber" no apareciese
        //        con decimales. Ademas no se que es lo que tiene que hacer
        //
        //    } else if (linea->nomcampo() == "debe" || linea->nomcampo() == "haber" + m_subform->tableName()) {
    } else if ( linea->nomcampo() == "debe" || linea->nomcampo() == "haber" ) {
        BlDoubleSpinBox * spinBox = static_cast<BlDoubleSpinBox*> ( editor );
        spinBox->interpretText();
        QString value = spinBox->text();
        model->setData ( index, value );
    } else if ( linea->nomcampo() == "nomcanal" ) {
        BcBuscarCanalDelegate * comboBox = static_cast<BcBuscarCanalDelegate*> ( editor );
        QString value = comboBox->currentText();
        model->setData ( index, value );
    } else if ( linea->nomcampo() == "nomc_coste" ) {
        BcBuscarCentroCosteDelegate * comboBox = static_cast<BcBuscarCentroCosteDelegate*> ( editor );
        QString value = comboBox->currentText();
        model->setData ( index, value );
    } else if ( linea->nomcampo().startsWith ( "fecha" ) ) {
        BlDateLineEdit * comboBox = static_cast<BlDateLineEdit*> ( editor );
        QString value = ( ( QLineEdit * ) comboBox ) ->text();
        model->setData ( index, value );
    } else {
        /// DbInt = 1, DbVarChar = 2, DbDate = 3, DbNumeric = 4, DbBoolean
        //if (linea->dbFieldType() == BlDbField::DbInt) {
        //    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        //    spinBox->interpretText();
        //    int value = spinBox->value();
        //    model->setData(index, value);
        //} else {
        BlSubFormDelegate::setModelData ( editor, model, index );
        //} // end if

        /*QLineEdit *lineedit = static_cast<QLineEdit*> ( editor );
                QString value = lineedit->text();
                model->setData ( index, value );
        */
    } // end if


    _depura ( "END BcSubFormDelegate::setModelData", 0 );
}


///
/**
\param editor
\param index
**/
void BcSubFormDelegate::setEditorData ( QWidget *editor, const QModelIndex &index ) const
{
    _depura ( "BcSubFormDelegate::setEditorData", 0 );
    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    //if (linea->nomcampo() == "desc" + m_subform->tableName()) {
    if ( linea->nomcampo().startsWith ( "desc" ) ) {
        QString data = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BlTextEditDelegate *textedit = qobject_cast<BlTextEditDelegate*> ( editor );
        textedit->setText ( data );
        textedit->selectAll();
    } else if ( linea->nomcampo() == "debe" || linea->nomcampo() == "haber" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BlDoubleSpinBox *spinBox = static_cast<BlDoubleSpinBox*> ( editor );
        spinBox->setValue ( value.toDouble() );
        spinBox->selectAll();
    } else if ( linea->nomcampo() == "nomcanal" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BcBuscarCanalDelegate *comboBox = static_cast<BcBuscarCanalDelegate*> ( editor );
        comboBox->set ( value );
    } else if ( linea->nomcampo() == "nomc_coste" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BcBuscarCentroCosteDelegate *comboBox = static_cast<BcBuscarCentroCosteDelegate*> ( editor );
        comboBox->set ( value );
    } else if ( linea->nomcampo().startsWith ( "fecha" ) ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BlDateLineEdit *bf = static_cast<BlDateLineEdit*> ( editor );
        ( ( QLineEdit * ) bf ) ->setText ( value );
        ( ( QLineEdit * ) bf ) ->selectAll();
    } else {
        /// DbInt = 1, DbVarChar = 2, DbDate = 3, DbNumeric = 4, DbBoolean
        //if (linea->dbFieldType() == BlDbField::DbInt) {
        //    int value = index.model()->data(index, Qt::DisplayRole).toInt();
        //    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        //    spinBox->setValue(value);
        //} else {
        BlSubFormDelegate::setEditorData ( editor, index );
        //} // end if
        /*        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
                QLineEdit *lineedit = static_cast<QLineEdit*> ( editor );
                lineedit->setText ( value );
        */
    } // end if
    _depura ( "END BcSubFormDelegate::setEditorData", 0 );
}




