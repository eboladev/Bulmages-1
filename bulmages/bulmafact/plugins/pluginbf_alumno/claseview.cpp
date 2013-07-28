/***************************************************************************
*   Copyright (C) 2009 by Tomeu Borras Riera                              *
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

#include "claseview.h"
#include "bfcompany.h"


/// Constructor de la clase inicializa la clase y llama a la clase de pintar para que pinte.
/**
\param emp
\param parent
\return
**/
ClaseView::ClaseView ( BfCompany *emp, QWidget *parent )
        : BfForm ( emp, parent )
{
    BL_FUNC_DEBUG

    setTitleName ( _ ( "Clase" ) );
    setDbTableName ( "clase" );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_tab->setDisabled ( true );
    /// Disparamos los plugins.
    int res = g_plugins->run ( "ClaseView_ClaseView", this );
    if ( res != 0 ) {
        return;
    } // end if
    m_archivoimagen = "";
    setEditMode();
    m_cursorclasees = NULL;
    m_item = NULL;
    res = g_plugins->run ( "ClaseView_ClaseView_Post", this );
    if ( res != 0 ) {
        return;
    } // end if
    pintar();
    insertWindow ( windowTitle(), this, false );
    blScript(this);
    
}


///
/**
**/
void ClaseView::imprimir()
{
    BL_FUNC_DEBUG
    
}

/// Carga el query de la base de datos y carga el qlistview.
/**
**/
void ClaseView::pintar()
{
    BL_FUNC_DEBUG

    mui_lista->clear();

    if ( m_cursorclasees != NULL ) {
        delete m_cursorclasees;
    } // end if
    m_cursorclasees = mainCompany() ->loadQuery ( "SELECT * FROM clase ORDER BY ascii(nomclase), nomclase" );
    while ( !m_cursorclasees->eof() ) {
        new QListWidgetItem ( m_cursorclasees->value( "nomclase" ), mui_lista );
        m_cursorclasees->nextRecord();
    } // end while

    /// Comprobamos cual es la cadena inicial.
    dialogChanges_readValues();
    
}


///
/**
**/
ClaseView::~ClaseView()
{
    BL_FUNC_DEBUG
    if ( m_cursorclasees != NULL ) {
        delete m_cursorclasees;
    } // end if
    
}


///
/**
\param cur
\return
**/
void ClaseView::on_mui_lista_currentItemChanged ( QListWidgetItem *cur, QListWidgetItem * )
{
    BL_FUNC_DEBUG
    if ( !cur ) return;
    mui_tab->setEnabled ( true );

    int row = mui_lista->row ( cur );
    trataModificado();
    mdb_idclase = m_cursorclasees->value( "idclase", row );
    m_nomclase->setText ( m_cursorclasees->value( "nomclase", row ) );

    m_item = cur;
    /// Comprobamos cual es la cadena inicial.
    /// Disparamos los plugins.
    int res = g_plugins->run ( "ClaseView_on_mui_lista_currentItemChanged_Post", this );
    if ( res != 0 ) {
        return;
    } // end if

    dialogChanges_readValues();

    
}


///
/**
\return
**/
void ClaseView::on_mui_guardar_clicked()
{
    BL_FUNC_DEBUG

    try {
        /// Disparamos los plugins.
        int res = g_plugins->run ( "ClaseView_on_mui_guardar_clicked", this );
        if ( res != 0 ) {
            return;
        } // end if
	if (mdb_idclase != "") {

	    QString query = "UPDATE clase SET ";
	    query += "  nomclase='" + mainCompany() ->sanearCadena ( m_nomclase->text() ) + "'";
	    query += " WHERE idclase=" + mainCompany() ->sanearCadena ( mdb_idclase );

	    mainCompany() ->begin();
	    mainCompany() ->runQuery ( query );
	    mainCompany() ->commit();

	    /// Si hay un cursor en activo lo borramos para recargarlo
	    pintar();
	    /// Emitimos la se&ntilde;al apropiada en el BlApplication.
	    g_theApp->emitDbTableChanged ( "clase" );

	    /// Comprobamos cual es la cadena inicial.
	    dialogChanges_readValues();
	} // end if
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al guardar el clase" ) );
        mainCompany() ->rollback();
    } // end try

    
}


///
/**
\return
**/
bool ClaseView::trataModificado()
{
    BL_FUNC_DEBUG
    /// Si se ha modificado el contenido advertimos y guardamos.
    if ( dialogChanges_isChanged() ) {
        if ( QMessageBox::warning ( this,
                                    _ ( "Guardar datos del clase" ),
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
void ClaseView::on_mui_nuevo_clicked()
{
    BL_FUNC_DEBUG
    try {
        /// Si se ha modificado el contenido advertimos y guardamos.
        trataModificado();
        QString query = "INSERT INTO clase (nomclase) VALUES ('*** NUEVA AULA ***')";
        mainCompany() ->begin();
        mainCompany() ->runQuery ( query );
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT max(idclase) AS idclase FROM clase" );
        mainCompany() ->commit();
        mdb_idclase = cur->value( "idclase" );
        delete cur;
        pintar();
        
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear un nuevo Clase" ) );
        mainCompany() ->rollback();
    } // end try
}


/// SLOT que responde a la pulsacion del boton de borrar la familia que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
/**
\return
**/
void ClaseView::on_mui_borrar_clicked()
{
    BL_FUNC_DEBUG
    try {
        mui_tab->setDisabled ( true );
        trataModificado();
        mainCompany() ->begin();
        QString query = "DELETE FROM clase WHERE idclase = " + mdb_idclase;
        mainCompany() ->runQuery ( query );
        mainCompany() ->commit();
        mdb_idclase = "";
	m_nomclase->setText ( "" );
        pintar();
        
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al borrar el Clase" ) );
        mainCompany() ->rollback();
    }// end try
}




///
/**
\return
**/
QString ClaseView::idclase()
{
    BL_FUNC_DEBUG
    
    return mdb_idclase;
}



