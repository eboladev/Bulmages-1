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

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QtGui/QPixmap>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDialog>
#include <QtWidgets/QCheckBox>

#include "bancoview.h"
#include "bfcompany.h"


/// Constructor de la clase inicializa la clase y llama a la clase de pintar para que pinte.
/**
\param emp
\param parent
**/
BancoView::BancoView ( BfCompany *emp, QWidget *parent )
        : BfForm ( emp, parent )
{
    BL_FUNC_DEBUG
    setTitleName ( _ ( "Banco" ) );
    /// Indicamos que vamos a operar con la tabla banco para que los permisos se traten adecuadamente.
    setDbTableName ( "banco" );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    groupBox1->setDisabled ( true );
    setEditMode();
    m_cursorbancos = NULL;
    m_item = NULL;

    /// Disparamos los plugins.
    int res = g_plugins->run ( "BancoView_BancoView", this );
    if ( res != 0 ) {
        return;
    } // end if

    pintar();
    insertWindow ( windowTitle(), this );
    blScript(this);
    
}


/// Carga el query de la base de datos y carga el qlistview.
/**
**/
void BancoView::pintar()
{
    BL_FUNC_DEBUG
    mui_lista->clear();
    if ( m_cursorbancos != NULL ) {
        delete m_cursorbancos;
    } // end if
    m_cursorbancos = mainCompany() ->loadQuery ( "SELECT * FROM banco ORDER BY nombanco" );
    while ( !m_cursorbancos->eof() ) {
        new QListWidgetItem ( m_cursorbancos->value( "nombanco" ) , mui_lista );
        m_cursorbancos->nextRecord();
    } // end while

    /// Comprobamos cual es la cadena inicial.
    dialogChanges_readValues();
    
}


///
/**
**/
BancoView::~BancoView()
{
    BL_FUNC_DEBUG
    if ( m_cursorbancos != NULL ) {
        delete m_cursorbancos;
    } // end if
    
}


///
/**
**/
QString BancoView::idBanco()
{
    BL_FUNC_DEBUG
    
    return mdb_idbanco;
}


///
/**
**/
void BancoView::on_mui_lista_currentItemChanged ( QListWidgetItem *cur, QListWidgetItem * )
{
    BL_FUNC_DEBUG
    if ( cur )
        groupBox1->setEnabled ( true );
    int row = mui_lista->row ( cur );
    trataModificado();
    mui_nombanco->setText ( m_cursorbancos->value( "nombanco", row ) );
    mdb_idbanco = m_cursorbancos->value( "idbanco", row );
    mui_dirbanco->setText ( m_cursorbancos->value( "dirbanco", row ) );
    mui_poblbanco->setText ( m_cursorbancos->value( "poblbanco", row ) );
    mui_cpbanco->setText ( m_cursorbancos->value( "cpbanco", row ) );
    mui_telbanco->setText ( m_cursorbancos->value( "telbanco", row ) );
    mui_faxbanco->setText ( m_cursorbancos->value( "faxbanco", row ) );
    mui_emailbanco->setText ( m_cursorbancos->value( "emailbanco", row ) );
    mui_contactobanco->setText ( m_cursorbancos->value( "contactobanco", row ) );
    mui_codentidadbanco->setText ( m_cursorbancos->value( "codentidadbanco", row ) );
    mui_codagenciabanco->setText ( m_cursorbancos->value( "codagenciabanco", row ) );
    mui_numcuentabanco->setText ( m_cursorbancos->value( "numcuentabanco", row ) );
    mui_dcbanco->setText ( m_cursorbancos->value( "dcbanco", row ) );
    mui_comentbanco->setText ( m_cursorbancos->value( "comentbanco", row ) );
    mui_webbanco->setText ( m_cursorbancos->value( "webbanco", row ) );
    mui_sufijobanco->setValue ( m_cursorbancos->valorInt ( "sufijobanco", row ) );
    m_item = cur;
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_readValues();
    
}


///
/**
\return
**/
int BancoView::save()
{
    BL_FUNC_DEBUG
    try {

        if ( mdb_idbanco.isEmpty() ) {
            blMsgInfo ( _ ( "Debe seleccionar un banco" ) );
            return -1;
        } // end if
        
//	mainCompany()->begin();
	
        /// Disparamos los plugins.
        int res1 = g_plugins->run ( "BancoView_Guardar_Pre", this );
        if ( res1 != 0 ) {
            return -1;
        } // end if
    
        QString query = "UPDATE banco SET ";
        query += "nombanco='" + mainCompany() ->sanearCadena ( mui_nombanco->text() ) + "'";
        query += ", dirbanco='" + mainCompany() ->sanearCadena ( mui_dirbanco->text() ) + "'";
        query += ", poblbanco='" + mainCompany() ->sanearCadena ( mui_poblbanco->text() ) + "'";
        query += ", cpbanco='" + mainCompany() ->sanearCadena ( mui_cpbanco->text() ) + "'";
        query += ", telbanco='" + mainCompany() ->sanearCadena ( mui_telbanco->text() ) + "'";
        query += ", faxbanco='" + mainCompany() ->sanearCadena ( mui_faxbanco->text() ) + "'";
        query += ", emailbanco='" + mainCompany() ->sanearCadena ( mui_emailbanco->text() ) + "'";
        query += ", contactobanco='" + mainCompany() ->sanearCadena ( mui_contactobanco->text() ) + "'";
        query += ", codentidadbanco='" + mainCompany() ->sanearCadena ( mui_codentidadbanco->text() ) + "'";
        query += ", codagenciabanco='" + mainCompany() ->sanearCadena ( mui_codagenciabanco->text() ) + "'";
        query += ", numcuentabanco='" + mainCompany() ->sanearCadena ( mui_numcuentabanco->text() ) + "'";
        query += ", dcbanco='" + mainCompany() ->sanearCadena ( mui_dcbanco->text() ) + "'";
        query += ", comentbanco='" + mainCompany() ->sanearCadena ( mui_comentbanco->toPlainText() ) + "'";
        query += ",  webbanco='" + mainCompany() ->sanearCadena ( mui_webbanco->text() ) + "'";
        query += ",  sufijobanco=" +  mui_sufijobanco->cleanText() ;
        query += " WHERE idbanco=" + mainCompany() ->sanearCadena ( mdb_idbanco );

        int error = mainCompany() ->runQuery ( query );

        if ( error ) {
            mainCompany() ->rollback();
            return -1;
        } // end if

        /// Disparamos los plugins.
        int res2 = g_plugins->run ( "BancoView_Guardar_Post", this );
        if ( res2 != 0 ) {
//            mainCompany() ->rollback();
            return -1;
        } // end if
        if ( m_cursorbancos != NULL ) {
            delete m_cursorbancos;
        } // end if

        m_cursorbancos = mainCompany() ->loadQuery ( "SELECT * FROM banco ORDER BY nombanco" );

        if ( m_item ) {
            m_item->setText ( mui_nombanco->text() );
        } // end if

        /// Comprobamos cual es la cadena inicial.
        dialogChanges_readValues();
        

//	mainCompany()->commit();

        return 0;
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al guardar" ) );
//	mainCompany()->rollback();
        return -1;
    } // end try
}


///
/**
\return
**/
bool BancoView::trataModificado()
{
    BL_FUNC_DEBUG
    /// Si se ha modificado el contenido advertimos y guardamos.
    if ( dialogChanges_isChanged() ) {
        if ( QMessageBox::warning ( this,
                                    _ ( "Guardar datos del trabajador" ),
                                    _ ( "Desea guardar los cambios?" ),
                                    _ ( "&Si" ), _ ( "&No" ), 0, 0, 1 ) == 0 )
            on_mui_guardar_clicked();
        return ( true );
    } // end if
    
    return ( false );
}


/// SLOT que responde a la pulsacion del boton de nuevo tipo de iva.
/// Inserta en la tabla de ivas
/**
\return
**/
void BancoView::on_mui_nuevo_clicked()
{
    BL_FUNC_DEBUG
    try {
        /// Si se ha modificado el contenido advertimos y guardamos.
        trataModificado();
        QString query = "INSERT INTO banco (nombanco) VALUES ('NUEVO BANCO')";
        mainCompany() ->begin();
        mainCompany() ->runQuery ( query );
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT max(idbanco) AS idbanco FROM banco" );
        mainCompany() ->commit();
        mdb_idbanco = cur->value( "idbanco" );
        delete cur;
        pintar();
        
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error inesperado el crear el Banco" ) );
        mainCompany() ->rollback();
    } // end catch
}


/// SLOT que responde a la pulsacion del boton de borrar la familia que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
/**
\return
**/
void BancoView::on_mui_borrar_clicked()
{
    BL_FUNC_DEBUG
    if ( mdb_idbanco == "" ) return;
    try {
        trataModificado();
        mainCompany() ->begin();
        QString query = "DELETE FROM banco WHERE idbanco = " + mdb_idbanco;
        int error = mainCompany() ->runQuery ( query );
        if ( error ) {
            mainCompany() ->rollback();
            return;
        } // end if
        mainCompany() ->commit();
        pintar();
        
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error inesperado al borrar el banco" ) );
        mainCompany() ->rollback();
    } // end try
}


///
/**
**/
void BancoView::imprimir()
{
    BL_FUNC_DEBUG
    
}


