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

#include "subform2bc.h"
#include "funcaux.h"
#include "empresa.h"
#include "asiento1view.h"
#include "extractoview1.h"
#include "qtexteditdelegate.h"
#include "bldoublespinbox.h"
#include "busquedacanal.h"
#include "busquedaccoste.h"
#include "cuentalistview.h"
#include "diarioview.h"


/// Constructor de la clase
/**
\param parent Widget padre del SubFormulario
**/
SubForm2Bc::SubForm2Bc ( QWidget *parent ) : BlSubForm ( parent )
{
    _depura ( "SubForm2Bc::SubForm2Bc", 0 );
    m_delegate = new QSubForm2BcDelegate ( this );
    mui_list->setItemDelegate ( m_delegate );
    _depura ( "END SubForm2Bc::SubForm2Bc", 0 );
}


/// Destructor del Subformulario
/**
Libera la memoria que se haya reservado durante el funcionamiento de la clase.
**/
SubForm2Bc::~SubForm2Bc()
{
    _depura ( "SubForm2Bc::~SubForm2Bc", 0 );
    delete m_delegate;
    _depura ( "END SubForm2Bc::~SubForm2Bc", 0 );
}


///
/**
\param c
**/
void SubForm2Bc::setEmpresaBase ( EmpresaBase *c )
{
    _depura ( "SubForm2Bc::setEmpresaBase", 0 );
    PEmpresaBase::setEmpresaBase ( c );
    m_delegate->setEmpresaBase ( c );
    _depura ( "END SubForm2Bc::setEmpresaBase", 0 );
}


///
/**
\return
**/
Empresa *SubForm2Bc::empresaBase()
{
    _depura ( "SubForm2Bc::empresaBase", 0 );
    _depura ( "END SubForm2Bc::empresaBase", 0 );
    return ( ( Empresa * ) PEmpresaBase::empresaBase() );
}

/// Se ha pulsado la combinacion de teclas Ctrl + +
/**
\param row Fila en la que se ha hecho la pulsacion
\param col Columna en la que se ha hecho la pulsacion
\return
**/
void SubForm2Bc::pressedPlus ( int row, int col, SDBRecord *rec, SDBCampo *camp )
{
    _depura ( "SubForm2Bc::pressedPlus", 0 );

    /// Si no es un campo de tipo debe o haber salimos.
    /*
        if ( camp->nomcampo() != "debe" && camp->nomcampo() != "haber" && camp->nomcampo() != "fecha" ) {
            _depura ( "END SubForm2Bc::pressedPlus", 0, "Campo incorrecto" );
            return;
        } // end if
    */

    if ( camp->nomcampo() == "fecha" ) {
        SDBRecord *recant = lineaat ( row - 1 );
        if ( recant ) {
            rec->setDBvalue ( "fecha", recant->DBvalue ( "fecha" ) );
        } // end if
        return;
    } // end if

    if ( camp->nomcampo() == "debe" || camp->nomcampo() == "haber" ) {

        /// Ponemos los campos a cero en esta fila
        rec->setDBvalue ( "debe", "0" );
        rec->setDBvalue ( "haber", "0" );

        /// Hacemos las sumas y las restamos
        BlFixed debe = sumarCampo ( "debe" );
        BlFixed haber = sumarCampo ( "haber" );
        BlFixed result = debe - haber;

        /// Segun el resultado imputamos al debe o al haber para que la cosa cuadre.
        if ( result > 0 ) {
            rec->setDBvalue ( "haber", result.toQString() );
        } // end if

        if ( result < 0 ) {
            result = result * -1;
            rec->setDBvalue ( "debe", result.toQString() );
        } // end if
        return;
    } // end if

    SDBRecord *recant = lineaat ( row - 1 );
    if ( recant ) {
        rec->setDBvalue ( camp->nomcampo(), recant->DBvalue ( camp->nomcampo() ) );
        return;
    } // end if


    _depura ( "END SubForm2Bc::pressedPlus", 0 );
}

/// Se ha pulsado la combinacion de teclas Ctrl + *
/**
\param row Fila en la que se ha hecho la pulsacion
\param col Columna en la que se ha hecho la pulsacion
\return
**/
void SubForm2Bc::pressedAsterisk ( int row, int col, SDBRecord *rec, SDBCampo *camp )
{
    _depura ( "SubForm2Bc::pressedAsterisk", 0 );

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
    CuentaListView *listcuentas = new CuentaListView ( ( Empresa * ) empresaBase(), diag, 0, CuentaListView::SelectMode );
//    listcuentas->inicializa();
    connect ( listcuentas, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

    diag->exec();
    QString codigo = listcuentas->codigocuenta();
    delete diag;

    if ( codigo != "" ) {
        QString query = "SELECT * FROM cuenta WHERE codigo = '" + codigo + "'";
        BlDbRecordSet *cur = empresaBase() ->cargacursor ( query );
        if ( !cur->eof() ) {
            if ( camp->nomcampo() == "codigo" ) {
                rec->setDBvalue ( "idcuenta", cur->valor ( "idcuenta" ) );
                rec->setDBvalue ( "codigo", cur->valor ( "codigo" ) );
                rec->setDBvalue ( "tipocuenta", cur->valor ( "tipocuenta" ) );
                rec->setDBvalue ( "descripcion", cur->valor ( "descripcion" ) );
                if ( rec->exists ( "idc_coste" ) && cur->valor ( "idc_coste" ) != "" ) {
                    rec->setDBvalue ( "idc_coste", cur->valor ( "idc_coste" ) );
                    QString query1 = "SELECT * FROM c_coste WHERE idc_coste = " + cur->valor ( "idc_coste" );
                    BlDbRecordSet *curss = empresaBase() ->cargacursor ( query1 );
                    rec->setDBvalue ( "nomc_coste", curss->valor ( "nombre" ) );
                    delete curss;
                } // end if

            } // end if
            if ( camp->nomcampo() == "codigoctacliente" ) {
                rec->setDBvalue ( "idctacliente", cur->valor ( "idcuenta" ) );
                rec->setDBvalue ( "codigoctacliente", cur->valor ( "codigo" ) );
                rec->setDBvalue ( "tipoctacliente", cur->valor ( "tipocuenta" ) );
                rec->setDBvalue ( "nomctacliente", cur->valor ( "descripcion" ) );

            } // end if
        } // end if
        delete cur;
    } // end if


    _depura ( "END SubForm2Bc::pressedAsterisk", 0 );
}


/// Se ha pulsado Ctrl + / en el subformulario
/**
\param row Fila en la que se ha hecho la pulsacion
\param col1 Columna en la que se ha hecho la pulsacion
\return
**/
void SubForm2Bc::pressedSlash ( int row, int col, SDBRecord *rec, SDBCampo *camp )
{
    _depura ( "SubForm2Bc::pressedSlash", 0 );

    if ( camp->nomcampo() == "fecha" ) {
        if ( row > 0 ) {
            SDBCampo * campoant = ( SDBCampo * ) item ( row - 1, col );
            camp->set ( campoant->text() );
        } // end if
        return;
    } // end if
    if ( camp->nomcampo() == "descripcion" ) {
        if ( row > 0 ) {
            SDBCampo * campoant = ( SDBCampo * ) item ( row - 1, col );
            camp->set ( campoant->text() );
        } // end if
        return;
    } // end if
    if ( camp->nomcampo() == "conceptocontable" ) {
        if ( row > 0 ) {
            SDBCampo * campoant = ( SDBCampo * ) item ( row - 1, col );
            camp->set ( campoant->text() );
        } // end if
        return;
    } // end if
    QString text = editaTexto ( camp->text() );
    camp->set ( text );
    _depura ( "END SubForm2Bc::pressedSlash", 0 );
}


/// Slot que se activa al cambiar de celda dentro del SubFormulario
/**
\param row
\param col
\return
**/
void SubForm2Bc::editFinished ( int row, int col, SDBRecord *rec, SDBCampo *camp )
{
    _depura ( "SubForm2Bc::editFinished", 0, "Row: " + QString::number ( row ) + " Col: " + QString::number ( col ) );



    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "SubForm2Bc_on_mui_list_cellChanged", this );
    if ( res != 0 ) {
        return;
    } // end if

    if ( camp->nomcampo() == "codigo" && camp->text() != "*" ) {
        QString codigoext = extiendecodigo ( camp->text(), ( ( Empresa * ) empresaBase() ) ->numdigitosempresa() );
        QString query = "SELECT idcuenta, codigo, tipocuenta, descripcion, idc_coste FROM cuenta WHERE codigo = '" + codigoext + "'";
        BlDbRecordSet *cur = empresaBase() ->cargacursor ( query );
        if ( !cur->eof() ) {
            rec->setDBvalue ( "idcuenta", cur->valor ( "idcuenta" ) );
            rec->setDBvalue ( "codigo", cur->valor ( "codigo" ) );
            rec->setDBvalue ( "tipocuenta", cur->valor ( "tipocuenta" ) );
            rec->setDBvalue ( "descripcioncuenta", cur->valor ( "descripcion" ) );
            if ( rec->exists ( "idc_coste" ) && cur->valor ( "idc_coste" ) != "" ) {
                rec->setDBvalue ( "idc_coste", cur->valor ( "idc_coste" ) );
                QString query1 = "SELECT * FROM c_coste WHERE idc_coste = " + cur->valor ( "idc_coste" );
                BlDbRecordSet *curss = empresaBase() ->cargacursor ( query1 );
                rec->setDBvalue ( "nomc_coste", curss->valor ( "nombre" ) );
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
        BlDbRecordSet *cur = empresaBase() ->cargacursor ( query );
        if ( !cur->eof() ) {
            rec->setDBvalue ( "idcanal", cur->valor ( "idcanal" ) );
        } else {
            rec->setDBvalue ( "idcanal", "" );
        } // end if
        delete cur;
    } // end if
    if ( camp->nomcampo() == "nomc_coste" ) {
        QString query = "SELECT idc_coste FROM c_coste WHERE nombre = '" + camp->text() + "'";
        BlDbRecordSet *cur = empresaBase() ->cargacursor ( query );
        if ( !cur->eof() ) {
            rec->setDBvalue ( "idc_coste", cur->valor ( "idc_coste" ) );
        } else {
            rec->setDBvalue ( "idc_coste", "" );
        } // end if
        delete cur;
    } // end if
    if ( camp->nomcampo() == "fecha" ) {
        QString nfecha = normalizafecha ( camp->text() ).toString ( "dd/MM/yyyy" );
        rec->setDBvalue ( "fecha", nfecha );
    } // end if
    if ( camp->nomcampo() == "debe" ) {
        if ( BlFixed ( camp->text() ) != BlFixed ( "0.00" ) ) {
            rec->setDBvalue ( "haber", "0.00" );
        } // end if
    } // end if
    if ( camp->nomcampo() == "haber" ) {
        if ( BlFixed ( camp->text() ) != BlFixed ( "0.00" ) ) {
            rec->setDBvalue ( "debe", "0.00" );
        } // end if
    } // end if

    g_plugins->lanza ( "SubForm2Bc_on_mui_list_cellChanged_post", this );

    BlSubForm::on_mui_list_cellChanged ( row, col );
    _depura ( "END SubForm2Bc::on_mui_list_cellChanged", 0 );
}


/// Muestra la ventana de asiento.
/**
**/
void SubForm2Bc::boton_asiento()
{
    _depura ( "SubForm2Bc::boton_asiento", 0 );
    Empresa *companyact = ( Empresa * ) empresaBase();
    QString numasiento = DBvalue ( "idasiento" );
    if ( numasiento != "" ) {
        companyact->intapuntsempresa() ->muestraasiento ( numasiento.toInt() );
        companyact->muestraapuntes1();
    } // end if
    _depura ( "END SubForm2Bc::boton_asiento", 0 );
}


/// Si el parametro pasado es un:
/// 0 -> del d&iacute;a actual
/// 1 -> del mes actual
/// 2 -> del a&ntilde;o actual
/**
\param tipo
**/
void SubForm2Bc::boton_extracto1 ( int tipo )
{
    _depura ( "SubForm2Bc::boton_extracto1", 0 );
    Empresa *companyact = ( Empresa * ) empresaBase();
    QDate fecha1, fecha2, fechaact;
    QString fecha = DBvalue ( "fecha" ).left ( 10 );
    QString codigo = DBvalue ( "codigo" );
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
    _depura ( "END SubForm2Bc::boton_extracto1", 0 );
}


/// Si el parametro pasado es un:
/// 0 -> del d&iacute;a actual
/// 1 -> del mes actual
/// 2 -> del a&ntilde;o actual
/**
\param tipo
**/
void SubForm2Bc::boton_diario1 ( int tipo )
{
    _depura ( "SubForm2Bc::boton_diario1", 0 );
    Empresa *companyact = ( Empresa * ) empresaBase();
    QDate fecha1, fecha2, fechaact, fechaact1;
    fechaact = normalizafecha ( DBvalue ( "fecha" ).left ( 10 ) );
    fechaact1 = normalizafecha ( DBvalue ( "fecha" ).left ( 10 ) );
    if ( DBvalue ( "fecha" ).left ( 10 ) != "" ) {
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
    _depura ( "SubForm2Bc::boton_diario1", 0 );
}


/// Si el parametro pasado es un:
/// 0 -> del per&iacute;odo actual.
/// 1 -> del mes actual mirado a partir de la fecha de inicio.
/// 2 -> del a&ntilde;o actual mirado a partir de la fecha de inicio.
/**
\param tipo
**/
/*
void SubForm2Bc::boton_balance1 ( int tipo )
{
    _depura ( "SubForm2Bc::boton_balance1", 0 );
    Empresa *companyact = ( Empresa * ) empresaBase();
    QString fecha = DBvalue ( "fecha" ).left ( 10 );
    QString codigo = DBvalue ( "codigo" );
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
        empresaBase() ->balanceempresa() ->inicializa1 ( codigo, codigo, fecha1.toString ( "dd/MM/yyyy" ), fecha2.toString ( "dd/MM/yyyy" ), "0" );
        companyact->balanceempresa() ->accept();
        companyact->librobalance();
    } // end if
    _depura ( "END SubForm2Bc::boton_balance1", 0 );
}
*/


/// Se ha dado la orden de crear el menu contextual. Y esta funcion funciona con
/// Un sistema en cascada en lugar del tipico signal-slot
/**
\param menu
**/
void SubForm2Bc::creaMenu ( QMenu *menu )
{
    _depura ( "SubForm2Bc::pintaMenu", 0 );
    menu->addAction ( _( "Submenu de contabilidad" ) );
    menu->addSeparator();
    _depura ( "END SubForm2Bc::pintaMenu", 0 );
}


///
/**
**/
void SubForm2Bc::procesaMenu ( QAction * )
{
    _depura ( "SubForm2Bc::procesaMenu", 0 );
    _depura ( "END SubForm2Bc::procesaMenu", 0 );
}


/// ===============================================================
///  Tratamientos del Item Delegate
/// ===============================================================
///
/**
\param parent
**/
QSubForm2BcDelegate::QSubForm2BcDelegate ( QObject *parent = 0 ) : QItemDelegate ( parent ), PEmpresaBase()
{
    _depura ( "QSubForm2BcDelegate::QSubForm2BcDelegate", 0 );
    m_subform = ( SubForm2Bc * ) parent;
    installEventFilter ( this );
    _depura ( "END QSubForm2BcDelegate::QSubForm2BcDelegate", 0 );
}


///
/**
**/
QSubForm2BcDelegate::~QSubForm2BcDelegate()
{
    _depura ( "QSubForm2BcDelegate::~QSubForm2BcDelegate", 0 );
    _depura ( "END QSubForm2BcDelegate::~QSubForm2BcDelegate", 0 );
}


///
/**
\param parent
\param index
**/
QWidget *QSubForm2BcDelegate::createEditor ( QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index ) const
{
    _depura ( "QSubForm2BcDelegate::createEditor", 0 );
    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    _depura ( "QSubForm2BcDelegate::createEditor", 0, "CurrentColumn: " + QString::number ( index.column() ) );
    _depura ( "QSubForm2BcDelegate::createEditor", 0, "CurrentRow: " + QString::number ( index.row() ) );

    if ( linea->nomcampo().startsWith ( "desc" ) ) {
        QTextEditDelegate * editor = new QTextEditDelegate ( parent );
        editor->setObjectName ( "QTextEditDelegate" );
        _depura ( "END QSubForm2BcDelegate::createEditor", 0, "QTextEditDelegate" );
        return editor;
    } else if ( linea->nomcampo() == "debe" || linea->nomcampo() == "haber" ) {
        BlDoubleSpinBox * editor = new BlDoubleSpinBox ( parent );
        editor->setMinimum ( -1000000 );
        editor->setMaximum ( 1000000 );
        _depura ( "END QSubForm2BcDelegate::createEditor", 0, "BlDoubleSpinBox" );
        return editor;
    } else if ( linea->nomcampo() == "codigo" ) {
        BusquedaCuentaDelegate * editor = new BusquedaCuentaDelegate ( parent );
        editor->setEmpresaBase ( ( Empresa * ) m_subform->empresaBase() );
        _depura ( "END QSubForm2BcDelegate::createEditor", 0, "BusquedaCuentaDelegate" );
        return editor;
    } else if ( linea->nomcampo() == "nomcanal" ) {
        BusquedaCanalDelegate * editor = new BusquedaCanalDelegate ( parent );
        editor->setEmpresaBase ( m_subform->empresaBase() );
        _depura ( "END QSubForm2BcDelegate::createEditor", 0, "BusquedaCanalDelegate" );
        return editor;
    } else if ( linea->nomcampo() == "nomc_coste" ) {
        BusquedaCCosteDelegate * editor = new BusquedaCCosteDelegate ( parent );
        editor->setEmpresaBase ( m_subform->empresaBase() );
        _depura ( "END QSubForm2BcDelegate::createEditor", 0, "BusquedaCCosteDelegate" );
        return editor;
    } else if ( linea->nomcampo().startsWith ( "fecha" ) ) {
        BusquedaFecha2 * editor = new BusquedaFecha2 ( parent );
        _depura ( "END QSubForm2BcDelegate::createEditor", 0, "BusquedaFecha2" );
        return editor;
    } else {
        /// DBint = 1, DBvarchar = 2, DBdate = 3, DBnumeric = 4, DBboolean
        //if (linea->tipo() == BlDbField::DBint) {
        //QSpinBox *editor = new QSpinBox(parent);
        //return editor;
        QLineEdit *editor = new QLineEdit ( parent );
        _depura ( "END QSubForm2BcDelegate::createEditor", 0, "QLineEdit" );
        return editor;
        //} else {
        //    return QItemDelegate::createEditor(parent, option, index);
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
void QSubForm2BcDelegate::setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    _depura ( "QSubForm2BcDelegate::setModelData", 0 );

    /// Si la fila o columna pasadas son invalidas salimos.
    if ( index.column() < 0 || index.row() < 0 ) {
        _depura ( "END QSubForm2BcDelegate::setModelData", 0, "Fila o columna invalidas" );
        return;
    } // end if

    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    if ( linea->nomcampo().startsWith ( "desc" ) ) {

//    if (linea->nomcampo() == "desc" + m_subform->tableName()) {
        QTextEditDelegate * textedit = qobject_cast<QTextEditDelegate *> ( editor );
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
    } else if ( linea->nomcampo() == "codigo" ) {
        BusquedaCuentaDelegate * comboBox = static_cast<BusquedaCuentaDelegate*> ( editor );
        QString value = comboBox->currentText();
        value = value.left ( value.indexOf ( ".-" ) );
        QString codigoext = extiendecodigo ( value,  m_subform->empresaBase() ->numdigitosempresa() );
        model->setData ( index, codigoext );
    } else if ( linea->nomcampo() == "nomcanal" ) {
        BusquedaCanalDelegate * comboBox = static_cast<BusquedaCanalDelegate*> ( editor );
        QString value = comboBox->currentText();
        model->setData ( index, value );
    } else if ( linea->nomcampo() == "nomc_coste" ) {
        BusquedaCCosteDelegate * comboBox = static_cast<BusquedaCCosteDelegate*> ( editor );
        QString value = comboBox->currentText();
        model->setData ( index, value );
    } else if ( linea->nomcampo().startsWith ( "fecha" ) ) {
        BusquedaFecha2 * comboBox = static_cast<BusquedaFecha2*> ( editor );
        QString value = ( ( QLineEdit * ) comboBox ) ->text();
        model->setData ( index, value );
    } else {
        /// DBint = 1, DBvarchar = 2, DBdate = 3, DBnumeric = 4, DBboolean
        //if (linea->tipo() == BlDbField::DBint) {
        //    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        //    spinBox->interpretText();
        //    int value = spinBox->value();
        //    model->setData(index, value);
        //} else {
        //    QItemDelegate::setModelData(editor, model, index);
        //} // end if
        QLineEdit *lineedit = static_cast<QLineEdit*> ( editor );
        QString value = lineedit->text();
        model->setData ( index, value );
    } // end if


    _depura ( "END QSubForm2BcDelegate::setModelData", 0 );
}


///
/**
\param editor
\param index
**/
void QSubForm2BcDelegate::setEditorData ( QWidget *editor, const QModelIndex &index ) const
{
    _depura ( "QSubForm2BcDelegate::setEditorData", 0 );
    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    //if (linea->nomcampo() == "desc" + m_subform->tableName()) {
    if ( linea->nomcampo().startsWith ( "desc" ) ) {
        QString data = index.model() ->data ( index, Qt::DisplayRole ).toString();
        QTextEditDelegate *textedit = qobject_cast<QTextEditDelegate*> ( editor );
        textedit->setText ( data );
        textedit->selectAll();
    } else if ( linea->nomcampo() == "debe" || linea->nomcampo() == "haber" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BlDoubleSpinBox *spinBox = static_cast<BlDoubleSpinBox*> ( editor );
        spinBox->setValue ( value.toDouble() );
        spinBox->selectAll();
    } else if ( linea->nomcampo() == "codigo" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BusquedaCuentaDelegate *comboBox = static_cast<BusquedaCuentaDelegate*> ( editor );
        comboBox->addItem ( value );
        //comboBox->lineEdit()->setText(value);
        comboBox->lineEdit() ->selectAll();
    } else if ( linea->nomcampo() == "nomcanal" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BusquedaCanalDelegate *comboBox = static_cast<BusquedaCanalDelegate*> ( editor );
        comboBox->set ( value );
    } else if ( linea->nomcampo() == "nomc_coste" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BusquedaCCosteDelegate *comboBox = static_cast<BusquedaCCosteDelegate*> ( editor );
        comboBox->set ( value );
    } else if ( linea->nomcampo().startsWith ( "fecha" ) ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BusquedaFecha2 *bf = static_cast<BusquedaFecha2*> ( editor );
        ( ( QLineEdit * ) bf ) ->setText ( value );
        ( ( QLineEdit * ) bf ) ->selectAll();
    } else {
        /// DBint = 1, DBvarchar = 2, DBdate = 3, DBnumeric = 4, DBboolean
        //if (linea->tipo() == BlDbField::DBint) {
        //    int value = index.model()->data(index, Qt::DisplayRole).toInt();
        //    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        //    spinBox->setValue(value);
        //} else {
        //    QItemDelegate::setEditorData(editor, index);
        //} // end if
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        QLineEdit *lineedit = static_cast<QLineEdit*> ( editor );
        lineedit->setText ( value );
    } // end if
    _depura ( "END QSubForm2BcDelegate::setEditorData", 0 );
}


///
/**
\param obj
\param event
\return
**/
bool QSubForm2BcDelegate::eventFilter ( QObject *obj, QEvent *event )
{
    /// Si es un release de tecla se hace la funcionalidad especificada.
    if ( event->type() == QEvent::KeyPress ) {
        _depura ( "QSubForm2BcDelegate::eventFilter", 0, obj->objectName() + " --> " + QString::number ( event->type() ) );
        QKeyEvent *keyEvent = static_cast<QKeyEvent *> ( event );
        int key = keyEvent->key();
        _depura ( "QSubForm2BcDelegate::key = : ", 0, QString::number ( key ) );
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
        _depura ( "QSubForm2BcDelegate::eventFilter", 0, obj->objectName() + " --> " + QString::number ( event->type() ) );
        QKeyEvent *keyEvent = static_cast<QKeyEvent *> ( event );
        int key = keyEvent->key();
        _depura ( "QSubForm2BcDelegate::key = : ", 0, QString::number ( key ) );
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


