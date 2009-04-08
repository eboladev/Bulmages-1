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
#include <QFileDialog>
#include <QPixmap>
#include <QLabel>
#include <QDialog>
#include <QCheckBox>

#include "trabajadorview.h"
#include "bfcompany.h"


/// Constructor de la clase inicializa la clase y llama a la clase de pintar para que pinte.
/**
\param emp
\param parent
\return
**/
TrabajadorView::TrabajadorView ( BfCompany *emp, QWidget *parent )
        : BfForm ( emp, parent )
{
    _depura ( "TrabajadorView::TrabajadorView", 0 );

    setTitleName ( _ ( "Trabajador" ) );
    setDbTableName ( "trabajador" );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_tab->setDisabled ( TRUE );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "TrabajadorView_TrabajadorView", this );
    if ( res != 0 ) {
        return;
    } // end if
    m_archivoimagen = "";
    setModoEdicion();
    m_cursortrabajadores = NULL;
    m_item = NULL;
    res = g_plugins->lanza ( "TrabajadorView_TrabajadorView_Post", this );
    if ( res != 0 ) {
        return;
    } // end if
    pintar();
    meteWindow ( windowTitle(), this, FALSE );
    _depura ( "END TrabajadorView::TrabajadorView", 0 );
}


///
/**
**/
void TrabajadorView::imprimir()
{
    _depura ( "TrabajadorView::imprimir", 0 );
    _depura ( "END TrabajadorView::imprimir", 0 );
}

/// Carga el query de la base de datos y carga el qlistview.
/**
**/
void TrabajadorView::pintar()
{
    _depura ( "TrabajadorView::pintar", 0 );

    mui_lista->clear();

    if ( m_cursortrabajadores != NULL ) {
        delete m_cursortrabajadores;
    } // end if
    m_cursortrabajadores = mainCompany() ->loadQuery ( "SELECT * FROM trabajador ORDER BY apellidostrabajador" );
    while ( !m_cursortrabajadores->eof() ) {
        new QListWidgetItem ( m_cursortrabajadores->valor ( "apellidostrabajador" ) + " " + m_cursortrabajadores->valor ( "nomtrabajador" ), mui_lista );
        m_cursortrabajadores->nextRecord();
    } // end while

    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    _depura ( "END TrabajadorView::pintar", 0 );
}


///
/**
**/
TrabajadorView::~TrabajadorView()
{
    _depura ( "TrabajadorView::~TrabajadorView", 0 );
    if ( m_cursortrabajadores != NULL ) {
        delete m_cursortrabajadores;
    } // end if
    _depura ( "END TrabajadorView::~TrabajadorView", 0 );
}


///
/**
\param cur
\return
**/
void TrabajadorView::on_mui_lista_currentItemChanged ( QListWidgetItem *cur, QListWidgetItem * )
{
    _depura ( "on_mui_lista_currentItemChanged", 0 );
    if ( !cur ) return;
    mui_tab->setEnabled ( TRUE );

    int row = mui_lista->row ( cur );
    trataModificado();
    m_nomtrabajador->setText ( m_cursortrabajadores->valor ( "nomtrabajador", row ) );
    mdb_idtrabajador = m_cursortrabajadores->valor ( "idtrabajador", row );
    m_apellidostrabajador->setText ( m_cursortrabajadores->valor ( "apellidostrabajador", row ) );
    m_nsstrabajador->setText ( m_cursortrabajadores->valor ( "nsstrabajador", row ) );
    m_dirtrabajador->setText ( m_cursortrabajadores->valor ( "dirtrabajador", row ) );
    m_teltrabajador->setText ( m_cursortrabajadores->valor ( "teltrabajador", row ) );
    m_moviltrabajador->setText ( m_cursortrabajadores->valor ( "moviltrabajador", row ) );
    m_emailtrabajador->setText ( m_cursortrabajadores->valor ( "emailtrabajador", row ) );
    if ( m_cursortrabajadores->valor ( "activotrabajador", row ) == "t" ) {
        m_activotrabajador->setChecked ( TRUE );
    } else {
        m_activotrabajador->setChecked ( FALSE );
    } // end if
    m_item = cur;
    /// Comprobamos cual es la cadena inicial.
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "TrabajadorView_on_mui_lista_currentItemChanged_Post", this );
    if ( res != 0 ) {
        return;
    } // end if
    dialogChanges_cargaInicial();
    m_imagen->setPixmap ( QPixmap ( g_confpr->valor ( CONF_DIR_IMG_PERSONAL ) + mdb_idtrabajador + ".jpg" ) );
    _depura ( "END on_mui_lista_currentItemChanged", 0 );
}


///
/**
\return
**/
void TrabajadorView::on_mui_guardar_clicked()
{
    _depura ( "TrabajadorView::on_mui_guardar_clicked", 0 );
    try {
        /// Disparamos los plugins.
        int res = g_plugins->lanza ( "TrabajadorView_on_mui_guardar_clicked", this );
        if ( res != 0 ) {
            return;
        } // end if
        QString m_textactivotrabajador = "FALSE";
        if ( m_activotrabajador->isChecked() ) {
            m_textactivotrabajador = "TRUE";
        } // end if
        QString query = "UPDATE trabajador SET ";
        query += "  nomtrabajador='" + mainCompany() ->sanearCadena ( m_nomtrabajador->text() ) + "'";
        query += ", apellidostrabajador= '" + mainCompany() ->sanearCadena ( m_apellidostrabajador->text() ) + "'";
        query += ", nsstrabajador = '" + mainCompany() ->sanearCadena ( m_nsstrabajador->text() ) + "'";
        query += ", dirtrabajador = '" + mainCompany() ->sanearCadena ( m_dirtrabajador->text() ) + "'";
        query += ", teltrabajador = '" + mainCompany() ->sanearCadena ( m_teltrabajador->text() ) + "'";
        query += ", moviltrabajador = '" + mainCompany() ->sanearCadena ( m_moviltrabajador->text() ) + "'";
        query += ", emailtrabajador = '" + mainCompany() ->sanearCadena ( m_emailtrabajador->text() ) + "'";
        query += ", activotrabajador = " + mainCompany() ->sanearCadena ( m_textactivotrabajador );
        query += " WHERE idtrabajador=" + mainCompany() ->sanearCadena ( mdb_idtrabajador );

        mainCompany() ->begin();
        mainCompany() ->runQuery ( query );
        mainCompany() ->commit();
        if ( m_cursortrabajadores != NULL ) {
            delete m_cursortrabajadores;
        } // end if

        m_cursortrabajadores = mainCompany() ->loadQuery ( "SELECT * FROM trabajador ORDER BY apellidostrabajador" );

        if ( m_item ) {
            m_item->setText ( m_apellidostrabajador->text() + m_nomtrabajador->text() );
        } // end if
        if ( m_archivoimagen != "" ) {
            QString cadena = "cp " + m_archivoimagen + " " + g_confpr->valor ( CONF_DIR_IMG_PERSONAL ) + mdb_idtrabajador + ".jpg";
            system ( cadena.toAscii().constData() );
        } // end if

        /// Emitimos la senyal apropiada en el qapplication2
        g_theApp->tablaCambiada1 ( "trabajador" );

        /// Comprobamos cual es la cadena inicial.
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al guardar el trabajador" ) );
        mainCompany() ->rollback();
    } // end try
    _depura ( "END TrabajadorView::on_mui_guardar_clicked", 0 );
}


///
/**
\return
**/
bool TrabajadorView::trataModificado()
{
    _depura ( "TrabajadorView::trataModificado", 0 );
    /// Si se ha modificado el contenido advertimos y guardamos.
    if ( dialogChanges_hayCambios() ) {
        if ( QMessageBox::warning ( this,
                                    _ ( "Guardar datos del trabajador" ),
                                    _ ( "Desea guardar los cambios?" ),
                                    _ ( "&Si" ), _ ( "&No" ), 0, 0, 1 ) == 0 )
            on_mui_guardar_clicked();
        return ( TRUE );
    } // end if
    _depura ( "END TrabajadorView::trataModificado", 0 );
    return ( FALSE );
}


/// SLOT que responde a la pulsacion del boton de nuevo tipo de iva.
/// Inserta en la tabla de ivas
/**
\return
**/
void TrabajadorView::on_mui_nuevo_clicked()
{
    _depura ( "TrabajadorView::on_mui_nuevo_clicked", 0 );
    try {
        /// Si se ha modificado el contenido advertimos y guardamos.
        trataModificado();
        QString query = "INSERT INTO trabajador (nomtrabajador, apellidostrabajador, nsstrabajador) VALUES ('NUEVO TRABAJADOR','NUEVO TRABAJADOR','000000000000')";
        mainCompany() ->begin();
        mainCompany() ->runQuery ( query );
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT max(idtrabajador) AS idtrabajador FROM trabajador" );
        mainCompany() ->commit();
        mdb_idtrabajador = cur->valor ( "idtrabajador" );
        delete cur;
        pintar();
        _depura ( "END TrabajadorView::on_mui_nuevo_clicked", 0 );
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al crear un nuevo Trabajador" ) );
        mainCompany() ->rollback();
    } // end try
}


/// SLOT que responde a la pulsacion del boton de borrar la familia que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
/**
\return
**/
void TrabajadorView::on_mui_borrar_clicked()
{
    _depura ( "TrabajadorView::on_mui_borrar_clicked", 0 );
    try {
        mui_tab->setDisabled ( TRUE );
        trataModificado();
        mainCompany() ->begin();
        QString query = "DELETE FROM trabajador WHERE idtrabajador = " + mdb_idtrabajador;
        mainCompany() ->runQuery ( query );
        mainCompany() ->commit();
        mdb_idtrabajador = "";
        pintar();
        _depura ( "END TrabajadorView::on_mui_borrar_clicked", 0 );
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al borrar el Trabajador" ) );
        mainCompany() ->rollback();
    }// end try
}


///
/**
\return
**/
void TrabajadorView::on_mui_imagen_clicked()
{
    _depura ( "TrabajadorView::on_mui_imagen_clicked", 0 );
    m_archivoimagen = QFileDialog::getOpenFileName ( this,
                      _ ( "Seleccione archivo" ),
                      "",
                      _ ( "Imagenes (*.jpg)" ) );
    m_imagen->setPixmap ( QPixmap ( m_archivoimagen ) );
    _depura ( "END TrabajadorView::on_mui_imagen_clicked", 0 );
}


///
/**
\return
**/
QString TrabajadorView::idtrabajador()
{
    _depura ( "TrabajadorView::idtrabajador", 0 );
    _depura ( "END TrabajadorView::idtrabajador", 0 );
    return mdb_idtrabajador;
}



