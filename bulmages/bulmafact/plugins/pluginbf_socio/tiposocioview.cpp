/***************************************************************************
*   Copyright (C) 2009 by Xavier Rusiñol Escriu                           *
*   russi7@gmail.com                                                      *
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
#include <QFileDialog>
#include <QPixmap>
#include <QLabel>
#include <QDialog>
#include <QCheckBox>

#include "tiposocioview.h"
#include "bfcompany.h"


/// Constructor de la clase inicializa la clase y llama a la clase de pintar para que pinte.
/**
\param emp
\param parent
**/
TiposocioView::TiposocioView ( BfCompany *emp, QWidget *parent )
        : BfForm ( emp, parent )
{
    _depura ( "TiposocioView::TiposocioView", 0 );
    setTitleName ( _ ( "Tipos de socio" ) );
    /// Indicamos que vamos a operar con la tabla tiposocio para que los permisos se traten adecuadamente.
    setDbTableName ( "tiposocio" );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    groupBox1->setDisabled ( TRUE );
    setModoEdicion();
    m_cursortiposocio = NULL;
    m_item = NULL;
    pintar();
    meteWindow ( windowTitle(), this );
    _depura ( "END TiposocioView::TiposocioView", 0 );
}


/// Carga el query de la base de datos y carga el qlistview.
/**
**/
void TiposocioView::pintar()
{
    _depura ( "TiposocioView::pintar", 0 );
    mui_lista->clear();
    if ( m_cursortiposocio != NULL ) {
        delete m_cursortiposocio;
    } // end if
    m_cursortiposocio = mainCompany() ->loadQuery ( "SELECT * FROM tiposocio ORDER BY nombretiposocio" );
    while ( !m_cursortiposocio->eof() ) {
        new QListWidgetItem ( m_cursortiposocio->valor ( "nombretiposocio" ) , mui_lista );
        m_cursortiposocio->nextRecord();
    } // end while

    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    _depura ( "END TiposocioView::pintar", 0 );
}


///
/**
**/
TiposocioView::~TiposocioView()
{
    _depura ( "TiposocioView::~TiposocioView", 0 );
    if ( m_cursortiposocio != NULL ) {
        delete m_cursortiposocio;
    } // end if
    _depura ( "END TiposocioView::~TiposocioView", 0 );
}


///
/**
**/
void TiposocioView::on_mui_lista_currentItemChanged ( QListWidgetItem *cur, QListWidgetItem * )
{
    _depura ( "on_mui_lista_currentItemChanged", 0 );
    if ( cur )
        groupBox1->setEnabled ( TRUE );
    int row = mui_lista->row ( cur );
    trataModificado();
    mui_nombretiposocio->setText ( m_cursortiposocio->valor ( "nombretiposocio", row ) );
    mdb_idtiposocio = m_cursortiposocio->valor ( "idtiposocio", row );
    mui_cuotatiposocio->setText ( m_cursortiposocio->valor ( "cuotatiposocio", row ) );
    mui_comentariotiposocio->setText ( m_cursortiposocio->valor ( "comentariotiposocio", row ) );
    m_item = cur;
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    _depura ( "END on_mui_lista_currentItemChanged", 0 );
}


///
/**
\return
**/
int TiposocioView::guardar()
{
    _depura ( "TiposocioView::on_mui_guardar_clicked", 0 );
    try {
        QString query = "UPDATE tiposocio SET ";
        query += "nombretiposocio='" + mainCompany() ->sanearCadena ( mui_nombretiposocio->text() ) + "'";
        query += ", cuotatiposocio=" + mainCompany() ->sanearCadena ( mui_cuotatiposocio->text() );
	query += ", comentariotiposocio='" + mainCompany() ->sanearCadena ( mui_comentariotiposocio->toPlainText() ) + "'";
        query += " WHERE idtiposocio=" + mainCompany() ->sanearCadena ( mdb_idtiposocio );

        int error = mainCompany() ->runQuery ( query );
        if ( error ) {
            mainCompany() ->rollback();
            return -1;
        } // end if
        if ( m_cursortiposocio != NULL ) {
            delete m_cursortiposocio;
        } // end if

	mainCompany()->commit();
        m_cursortiposocio = mainCompany() ->loadQuery ( "SELECT * FROM tiposocio ORDER BY nombretiposocio" );

        if ( m_item ) {
            m_item->setText ( mui_nombretiposocio->text() );
        } // end if
        /// Comprobamos cual es la cadena inicial.
        dialogChanges_cargaInicial();
        _depura ( "END TiposocioView::on_mui_guardar_clicked", 0 );
        return 0;
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al guardar" ) );
        return -1;
    } // end try
}


///
/**
\return
**/
bool TiposocioView::trataModificado()
{
    _depura ( "TiposocioView::trataModificado", 0 );
    /// Si se ha modificado el contenido advertimos y guardamos.
    if ( dialogChanges_hayCambios() ) {
        if ( QMessageBox::warning ( this,
                                    _ ( "Guardar datos del tipo de socio" ),
                                    _ ( "Desea guardar los cambios?" ),
                                    _ ( "&Si" ), _ ( "&No" ), 0, 0, 1 ) == 0 )
            on_mui_guardar_clicked();
        return ( TRUE );
    } // end if
    _depura ( "END TiposocioView::trataModificado", 0 );
    return ( FALSE );
}


/// SLOT que responde a la pulsacion del boton de nuevo tipo socio.
/// Inserta en la tabla tiposocio
/**
\return
**/
void TiposocioView::on_mui_nuevo_clicked()
{
    _depura ( "TiposocioView::on_mui_nuevo_clicked", 0 );
    try {
        /// Si se ha modificado el contenido advertimos y guardamos.
        trataModificado();
        QString query = "INSERT INTO tiposocio (nombretiposocio) VALUES ('NUEVO TIPO DE SOCIO')";
        mainCompany() ->begin();
        mainCompany() ->runQuery ( query );
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT max(idtiposocio) AS idtiposocio FROM tiposocio" );
        mainCompany() ->commit();
        mdb_idtiposocio = cur->valor ( "idtiposocio" );
        delete cur;
        pintar();
        _depura ( "END TiposocioView::on_mui_nuevo_clicked", 0 );
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error inesperado al crear el Tipo de socio" ) );
        mainCompany() ->rollback();
    } // end catch
}


/// SLOT que responde a la pulsacion del boton de borrar el tiposocio que se esta editando.
/**
\return
**/
void TiposocioView::on_mui_borrar_clicked()
{
    _depura ( "TiposocioView::on_mui_borrar_clicked", 0 );
    if ( mdb_idtiposocio == "" ) return;
    try {
        trataModificado();
        mainCompany() ->begin();
        QString query = "DELETE FROM tiposocio WHERE idtiposocio = " + mdb_idtiposocio;
        int error = mainCompany() ->runQuery ( query );
        if ( error ) {
            mainCompany() ->rollback();
            return;
        } // end if
        mainCompany() ->commit();
        pintar();
        _depura ( "END TiposocioView::on_mui_borrar_clicked", 0 );
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error inesperado al borrar el Tipo de socio" ) );
        mainCompany() ->rollback();
    } // end try
}


///
/**
**/
void TiposocioView::imprimir()
{
    _depura ( "TiposocioView::imprimir", 0 );
    _depura ( "END TiposocioView::imprimir", 0 );
}
