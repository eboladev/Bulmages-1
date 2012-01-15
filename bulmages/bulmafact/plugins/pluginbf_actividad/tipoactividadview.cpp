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

#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>
#include <QLabel>
#include <QDialog>
#include <QCheckBox>

#include "tipoactividadview.h"
#include "bfcompany.h"


/// Constructor de la clase inicializa la clase y llama a la clase de pintar para que pinte.
/**
\param emp
\param parent
\return
**/
TipoActividadView::TipoActividadView ( BfCompany *emp, QWidget *parent )
        : BfForm ( emp, parent )
{
    BL_FUNC_DEBUG

    setTitleName ( _ ( "TipoActividad" ) );
    setDbTableName ( "tipoactividad" );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_tab->setDisabled ( TRUE );
    /// Disparamos los plugins.
    int res = g_plugins->run ( "TipoActividadView_TipoActividadView", this );
    if ( res != 0 ) {
        return;
    } // end if
    m_archivoimagen = "";
    setEditMode();
    m_cursortipoactividades = NULL;
    m_item = NULL;
    res = g_plugins->run ( "TipoActividadView_TipoActividadView_Post", this );
    if ( res != 0 ) {
        return;
    } // end if
    pintar();
    insertWindow ( windowTitle(), this, FALSE );
    blScript(this);
    
}


///
/**
**/
void TipoActividadView::imprimir()
{
    BL_FUNC_DEBUG
    
}

/// Carga el query de la base de datos y carga el qlistview.
/**
**/
void TipoActividadView::pintar()
{
    BL_FUNC_DEBUG

    mui_lista->clear();

    if ( m_cursortipoactividades != NULL ) {
        delete m_cursortipoactividades;
    } // end if
    m_cursortipoactividades = mainCompany() ->loadQuery ( "SELECT * FROM tipoactividad ORDER BY ascii(nombretipoactividad), nombretipoactividad" );
    while ( !m_cursortipoactividades->eof() ) {
        new QListWidgetItem ( m_cursortipoactividades->value( "nombretipoactividad" ), mui_lista );
        m_cursortipoactividades->nextRecord();
    } // end while

    /// Comprobamos cual es la cadena inicial.
    dialogChanges_readValues();
    
}


///
/**
**/
TipoActividadView::~TipoActividadView()
{
    BL_FUNC_DEBUG
    if ( m_cursortipoactividades != NULL ) {
        delete m_cursortipoactividades;
    } // end if
    
}


///
/**
\param cur
\return
**/
void TipoActividadView::on_mui_lista_currentItemChanged ( QListWidgetItem *cur, QListWidgetItem * )
{
    BL_FUNC_DEBUG
    if ( !cur ) return;
    mui_tab->setEnabled ( TRUE );

    int row = mui_lista->row ( cur );
    trataModificado();
    mdb_idtipoactividad = m_cursortipoactividades->value( "idtipoactividad", row );
    m_nombretipoactividad->setText ( m_cursortipoactividades->value( "nombretipoactividad", row ) );

    m_item = cur;
    /// Comprobamos cual es la cadena inicial.
    /// Disparamos los plugins.
    int res = g_plugins->run ( "TipoActividadView_on_mui_lista_currentItemChanged_Post", this );
    if ( res != 0 ) {
        return;
    } // end if

    dialogChanges_readValues();

    
}


///
/**
\return
**/
void TipoActividadView::on_mui_guardar_clicked()
{
    BL_FUNC_DEBUG


    try {
        /// Disparamos los plugins.
        int res = g_plugins->run ( "TipoActividadView_on_mui_guardar_clicked", this );
        if ( res != 0 ) {
            return;
        } // end if

        QString query = "UPDATE tipoactividad SET ";
        query += "  nombretipoactividad='" + mainCompany() ->sanearCadena ( m_nombretipoactividad->text() ) + "'";
        query += " WHERE idtipoactividad=" + mainCompany() ->sanearCadena ( mdb_idtipoactividad );

        mainCompany() ->begin();
        mainCompany() ->runQuery ( query );
        mainCompany() ->commit();

        /// Si hay un cursor en activo lo borramos para recargarlo
        pintar();


        /// Emitimos la se&ntilde;al apropiada en el BlApplication.
        g_theApp->emitDbTableChanged ( "tipoactividad" );

        /// Comprobamos cual es la cadena inicial.
        dialogChanges_readValues();
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al guardar el tipoactividad" ) );
        mainCompany() ->rollback();
    } // end try


    
}


///
/**
\return
**/
bool TipoActividadView::trataModificado()
{
    BL_FUNC_DEBUG
    /// Si se ha modificado el contenido advertimos y guardamos.
    if ( dialogChanges_isChanged() ) {
        if ( QMessageBox::warning ( this,
                                    _ ( "Guardar datos del tipoactividad" ),
                                    _ ( "Desea guardar los cambios?" ),
                                    _ ( "&Si" ), _ ( "&No" ), 0, 0, 1 ) == 0 )
            on_mui_guardar_clicked();
        return ( TRUE );
    } // end if
    
    return ( FALSE );
}


/// SLOT que responde a la pulsacion del boton de nuevo tipo de iva.
/// Inserta en la tabla de ivas
/**
\return
**/
void TipoActividadView::on_mui_nuevo_clicked()
{
    BL_FUNC_DEBUG
    try {
        /// Si se ha modificado el contenido advertimos y guardamos.
        trataModificado();
        QString query = "INSERT INTO tipoactividad (nombretipoactividad) VALUES ('*** NUEVO TIPO DE ACTIVIDAD ***')";
        mainCompany() ->begin();
        mainCompany() ->runQuery ( query );
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT max(idtipoactividad) AS idtipoactividad FROM tipoactividad" );
        mainCompany() ->commit();
        mdb_idtipoactividad = cur->value( "idtipoactividad" );
        delete cur;
        pintar();
        
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear un nuevo TipoActividad" ) );
        mainCompany() ->rollback();
    } // end try
}


/// SLOT que responde a la pulsacion del boton de borrar la familia que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
/**
\return
**/
void TipoActividadView::on_mui_borrar_clicked()
{
    BL_FUNC_DEBUG
    try {
        mui_tab->setDisabled ( TRUE );
        trataModificado();
        mainCompany() ->begin();
        QString query = "DELETE FROM tipoactividad WHERE idtipoactividad = " + mdb_idtipoactividad;
        mainCompany() ->runQuery ( query );
        mainCompany() ->commit();
        mdb_idtipoactividad = "";
        pintar();
        
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al borrar el TipoActividad" ) );
        mainCompany() ->rollback();
    }// end try
}




///
/**
\return
**/
QString TipoActividadView::idtipoactividad()
{
    BL_FUNC_DEBUG
    
    return mdb_idtipoactividad;
}



