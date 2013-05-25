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

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QtGui/QPixmap>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDialog>
#include <QtWidgets/QCheckBox>

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
    BL_FUNC_DEBUG
    setTitleName ( _ ( "Tipos de socio" ) );
    /// Indicamos que vamos a operar con la tabla tiposocio para que los permisos se traten adecuadamente.
    setDbTableName ( "tiposocio" );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    groupBox1->setDisabled ( true );
    setEditMode();
    m_cursortiposocio = NULL;
    m_item = NULL;
    pintar();
    insertWindow ( windowTitle(), this );
    blScript(this);
    
}


/// Carga el query de la base de datos y carga el qlistview.
/**
**/
void TiposocioView::pintar()
{
    BL_FUNC_DEBUG
    mui_lista->clear();
    if ( m_cursortiposocio != NULL ) {
        delete m_cursortiposocio;
    } // end if
    m_cursortiposocio = mainCompany() ->loadQuery ( "SELECT * FROM tiposocio ORDER BY nombretiposocio" );
    while ( !m_cursortiposocio->eof() ) {
        new QListWidgetItem ( m_cursortiposocio->value( "nombretiposocio" ) , mui_lista );
        m_cursortiposocio->nextRecord();
    } // end while

    /// Comprobamos cual es la cadena inicial.
    dialogChanges_readValues();
    
}


///
/**
**/
TiposocioView::~TiposocioView()
{
    BL_FUNC_DEBUG
    if ( m_cursortiposocio != NULL ) {
        delete m_cursortiposocio;
    } // end if
    
}


///
/**
**/
void TiposocioView::on_mui_lista_currentItemChanged ( QListWidgetItem *cur, QListWidgetItem * )
{
    BL_FUNC_DEBUG
    if ( cur )
        groupBox1->setEnabled ( true );
    int row = mui_lista->row ( cur );
    trataModificado();
    mui_nombretiposocio->setText ( m_cursortiposocio->value( "nombretiposocio", row ) );
    mdb_idtiposocio = m_cursortiposocio->value( "idtiposocio", row );
    mui_cuotatiposocio->setText ( m_cursortiposocio->value( "cuotatiposocio", row ) );
    mui_comentariotiposocio->setText ( m_cursortiposocio->value( "comentariotiposocio", row ) );
    m_item = cur;
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_readValues();
    
}


///
/**
\return
**/
int TiposocioView::save()
{
    BL_FUNC_DEBUG
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
        dialogChanges_readValues();
        
        return 0;
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al guardar" ) );
        return -1;
    } // end try
}


///
/**
\return
**/
bool TiposocioView::trataModificado()
{
    BL_FUNC_DEBUG
    /// Si se ha modificado el contenido advertimos y guardamos.
    if ( dialogChanges_isChanged() ) {
        if ( QMessageBox::warning ( this,
                                    _ ( "Guardar datos del tipo de socio" ),
                                    _ ( "Desea guardar los cambios?" ),
                                    _ ( "&Si" ), _ ( "&No" ), 0, 0, 1 ) == 0 )
            on_mui_guardar_clicked();
        return ( true );
    } // end if
    
    return ( false );
}


/// SLOT que responde a la pulsacion del boton de nuevo tipo socio.
/// Inserta en la tabla tiposocio
/**
\return
**/
void TiposocioView::on_mui_nuevo_clicked()
{
    BL_FUNC_DEBUG
    try {
        /// Si se ha modificado el contenido advertimos y guardamos.
        trataModificado();
        QString query = "INSERT INTO tiposocio (nombretiposocio) VALUES ('NUEVO TIPO DE SOCIO')";
        mainCompany() ->begin();
        mainCompany() ->runQuery ( query );
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT max(idtiposocio) AS idtiposocio FROM tiposocio" );
        mainCompany() ->commit();
        mdb_idtiposocio = cur->value( "idtiposocio" );
        delete cur;
        pintar();
        
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error inesperado al crear el Tipo de socio" ) );
        mainCompany() ->rollback();
    } // end catch
}


/// SLOT que responde a la pulsacion del boton de borrar el tiposocio que se esta editando.
/**
\return
**/
void TiposocioView::on_mui_borrar_clicked()
{
    BL_FUNC_DEBUG
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
        
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error inesperado al borrar el Tipo de socio" ) );
        mainCompany() ->rollback();
    } // end try
}


///
/**
**/
void TiposocioView::imprimir()
{
    BL_FUNC_DEBUG
    
}
