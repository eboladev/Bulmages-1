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

#include <QLineEdit>
#include <QMessageBox>
#include <QCloseEvent>

#include "fpagoview.h"
#include "bfcompany.h"


/// Constructor de la clase inicializa la clase y llama a la clase de pintar para que pinte.
/**
\param emp
\param parent
**/
FPagoView::FPagoView ( BfCompany *emp, QWidget *parent )
        : BfForm ( emp, parent )
{
    _depura ( "FPagoView::FPagoView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    groupBox1->setDisabled ( TRUE );
    setTitleName ( _( "Forma de Pago" ) );
    setDbTableName ( "forma_pago" );
    setModoEdicion();
    m_cursorFPagoView = NULL;
    m_item = NULL;
    meteWindow ( windowTitle(), this );
    pintar();
    _depura ( "END FPagoView::FPagoView", 0 );
}


/// Carga el query de la base de datos y carga el qlistview.
/**
\return
**/
void FPagoView::pintar()
{
    _depura ( "FPagoView::pintar", 0 );
    mui_lista->clear();
    if ( m_cursorFPagoView != NULL )
        delete m_cursorFPagoView;
    m_cursorFPagoView = mainCompany() ->loadQuery ( "SELECT * FROM forma_pago ORDER BY idforma_pago" );
    if ( !m_cursorFPagoView ) return;
    mui_lista->clear();
    while ( !m_cursorFPagoView->eof() ) {
        new QListWidgetItem ( m_cursorFPagoView->valor ( "descforma_pago" ), mui_lista );
        m_cursorFPagoView->nextRecord();
    } // end while
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    _depura ( "END FPagoView::pintar", 0 );
}


///
/**
**/
FPagoView::~FPagoView()
{
    _depura ( "FPagoView::~FPagoView", 0 );
    if ( m_cursorFPagoView != NULL )
        delete m_cursorFPagoView;
    _depura ( "END FPagoView::~FPagoView", 0 );

}


///
/**
\param cur
**/
void FPagoView::on_mui_lista_currentItemChanged ( QListWidgetItem *cur, QListWidgetItem * )
{
    _depura ( "on_mui_lista_currentItemChanged", 0 );
    if ( !cur ) return;
    groupBox1->setEnabled ( TRUE );
    int row = mui_lista->row ( cur );
    trataModificado();
    mui_descforma_pago->setText ( m_cursorFPagoView->valor ( "descforma_pago", row ) );
    mui_dias1tforma_pago->setText ( m_cursorFPagoView->valor ( "dias1tforma_pago", row ) );
    mui_descuentoforma_pago->setText ( m_cursorFPagoView->valor ( "descuentoforma_pago", row ) );
    mdb_idforma_pago = m_cursorFPagoView->valor ( "idforma_pago", row );
    m_item = cur;

    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    _depura ( "END on_mui_lista_currentItemChanged", 0 );
}

///
/**
\return
**/
int FPagoView::guardar()
{
    _depura ( "FPagoView::on_mui_guardar_clicked", 0 );
    if ( mdb_idforma_pago == "" || mdb_idforma_pago == "0" ) return 0;
    try {
        QString query = "UPDATE forma_pago SET descforma_pago = '" +
                        mainCompany() ->sanearCadena ( mui_descforma_pago->text() ) + "', dias1tforma_pago= " +
                        mainCompany() ->sanearCadena ( mui_dias1tforma_pago->text() ) + " , descuentoforma_pago = " +
                        mainCompany() ->sanearCadena ( mui_descuentoforma_pago->text() ) + " WHERE idforma_pago =" + mdb_idforma_pago;
        mainCompany() ->runQuery ( query );
        if ( m_cursorFPagoView != NULL ) {
            delete m_cursorFPagoView;
        } // end if
        m_cursorFPagoView = mainCompany() ->loadQuery ( "SELECT * FROM forma_pago ORDER BY idforma_pago" );
        if ( m_item ) {
            m_item->setText ( mui_descforma_pago->text() );
        } // end if
        dialogChanges_cargaInicial();

	/// Emitimos la senyal apropiada en el qapplication2
	theApp->tablaCambiada1("forma_pago");

        _depura ( "END FPagoView::on_mui_guardar_clicked", 0 );
        return 0;
    } catch ( ... ) {
        _depura ( "error guardando la forma de pago", 1 );
        return -1;
    } // end try
}


///
/**
\return
**/
bool FPagoView::trataModificado()
{
    _depura ( "FPagoView::trataModificado", 0 );
    /// Si se ha modificado el contenido advertimos y guardamos.
    if ( dialogChanges_hayCambios() ) {
        if ( QMessageBox::warning ( this,
                                    _( "Guardar forma de pago" ),
                                    _( "Desea guardar los cambios." ),
                                    QMessageBox::Ok,
                                    QMessageBox::Cancel ) == QMessageBox::Ok )
            on_mui_guardar_clicked();
        _depura ( "END FPagoView::trataModificado", 0 );
        return ( TRUE );
    } // end if
    return ( FALSE );
}


/// SLOT que responde a la pulsacion del boton de nuevo tipo de iva
/// Inserta en la tabla de IVAs.
/**
\return
**/
void FPagoView::on_mui_crear_clicked()
{
    _depura ( "FPagoView::on_mui_crear_clicked", 0 );
    /// Si se ha modificado el contenido advertimos y guardamos.
    trataModificado();
    QString query = "INSERT INTO forma_pago (descforma_pago, dias1tforma_pago, descuentoforma_pago) VALUES ('NUEVA FORMA DE PAGO', 0, 0)";
    mainCompany() ->begin();
    int error = mainCompany() ->runQuery ( query );
    if ( error ) {
        mainCompany() ->rollback();
        return;
    } // end if
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT max(idforma_pago) AS idFPagoView FROM forma_pago" );
    mainCompany() ->commit();
    mdb_idforma_pago = cur->valor ( "idFPagoView" );
    delete cur;
    pintar();
    _depura ( "END FPagoView::on_mui_crear_clicked", 0 );
}


/// SLOT que responde a la pulsacion del boton de borrar la familia que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
/**
\return
**/
void FPagoView::on_mui_borrar_clicked()
{
    _depura ( "FPagoView::on_mui_borrar_clicked", 0 );
    trataModificado();
    try {
        mainCompany() ->begin();
        QString query = "DELETE FROM forma_pago WHERE idforma_pago = " + mdb_idforma_pago;
        mainCompany() ->runQuery ( query );
        mainCompany() ->commit();
        pintar();
        groupBox1->setDisabled ( TRUE );
        _depura ( "END FPagoView::on_mui_borrar_clicked", 0 );
    } catch ( ... ) {
        mensajeInfo ( _( "Error al intentar borrar la forma de pago" ) );
        mainCompany() ->rollback();
    } // end try
}


///
/**
**/
void FPagoView::setModoConsulta()
{
    _depura ( "FPagoView::setModoConsulta", 0 );
    m_modoConsulta = TRUE;
    _depura ( "END FPagoView::setModoConsulta", 0 );
}


///
/**
**/
void FPagoView::setModoEdicion()
{
    _depura ( "FPagoView::setModoEdicion", 0 );
    m_modoConsulta = FALSE;
    _depura ( "END FPagoView::setModoEdicion", 0 );
}


/// SLOT que responde a la pulsacion del boton de borrar la familia que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
/**
\return
**/
void FPagoView::on_mui_aceptar_clicked()
{
    _depura ( "FPagoView::on_mui_aceptar_clicked", 0 );
    trataModificado();
    close();
    _depura ( "END FPagoView::on_mui_aceptar_clicked", 0 );
}



