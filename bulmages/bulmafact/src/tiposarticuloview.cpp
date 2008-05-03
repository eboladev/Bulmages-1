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

#include <QMap>
#include <QLineEdit>
#include <QMessageBox>

#include "tiposarticuloview.h"
#include "company.h"
#include "funcaux.h"

#define COL_IDTIPOARTICULO    0
#define COL_CODTIPOARTICULO   1
#define COL_DESCTIPOARTICULO  2


///
/**
\param comp
\param parent
\param modoConsultas
**/
TipoArticuloList::TipoArticuloList ( Company *comp, QWidget *parent, bool modoConsulta )
        : FichaBf ( comp, parent )
{
    _depura ( "TipoArticuloList::TipoArticuloList", 0 );
    setupUi ( this );
    m_listTipos->setColumnCount ( 3 );
    QStringList headers;
    headers << tr ( "Id" ) << tr ( "Codigo" ) << tr ( "Descripcion" );
    m_listTipos->setHeaderLabels ( headers );
    m_semaforoPintar = FALSE;
    m_idtipo = "";
    if ( modoConsulta ) {
        setModoConsulta();
        groupBox1->hide();
        mui_detalles->hide();
        mui_crear->hide();
        mui_guardar->hide();
        mui_borrar->hide();
        mui_cancelar->hide();
        mui_aceptar->hide();
    } else {
        setModoEdicion();
        setAttribute ( Qt::WA_DeleteOnClose );
        empresaBase()->meteWindow ( windowTitle(), this, FALSE );
    } // end if
    pintar();
    _depura ( "END TipoArticuloList::TipoArticuloList", 0 );
}


///
/**
**/
TipoArticuloList::~TipoArticuloList()
{
    _depura ( "TipoArticuloList::~TipoArticuloList", 0 );
    _depura ( "END TipoArticuloList::~TipoArticuloList", 0 );
}


///
/**
**/
void TipoArticuloList::setModoConsulta()
{
    _depura ( "TipoArticuloList::setModoConsulta", 0 );
    m_modoConsulta = TRUE;
    _depura ( "TipoArticuloList::setModoConsulta", 0 );
}


///
/**
**/
void TipoArticuloList::setModoEdicion()
{
    _depura ( "TipoArticuloList::setModoEdicion", 0 );
    m_modoConsulta = FALSE;
    _depura ( "END TipoArticuloList::setModoEdicion", 0 );
}



///
/**
**/
void TipoArticuloList::pintar()
{
    _depura ( "TipoArticuloList::pintar", 0 );
    m_semaforoPintar = TRUE;
    cursor2 *cursoraux1 = NULL;
    try {
        QTreeWidgetItem * it;


    /// Vaciamos el arbol.
    while ( m_listTipos->topLevelItemCount() > 0 ) {
        it = m_listTipos->takeTopLevelItem ( 0 );
        delete it;
    } // end while

        cursoraux1 = empresaBase()->cargacursor ( "SELECT * FROM tipo_articulo ORDER BY codtipo_articulo" );
        while ( !cursoraux1->eof() ) {
            it = new QTreeWidgetItem ( m_listTipos );
            it->setText ( COL_IDTIPOARTICULO, cursoraux1->valor ( "idtipo_articulo" ) );
            it->setText ( COL_CODTIPOARTICULO, cursoraux1->valor ( "codtipo_articulo" ) );
            it->setText ( COL_DESCTIPOARTICULO, cursoraux1->valor ( "desctipo_articulo" ) );
            //it->setOpen(true);
            cursoraux1->siguienteregistro();
        } // end while
        delete cursoraux1;
        m_idtipo = "";
        /// Comprobamos cual es la cadena inicial.
        dialogChanges_cargaInicial();
        m_semaforoPintar = FALSE;
        _depura ( "END TipoArticuloList::pintar", 0 );
    } catch ( ... ) {
        mensajeInfo ( "Error en la carga" );
        if ( cursoraux1 ) delete cursoraux1;
    } // end try
}


///
/**
\return
**/
QString TipoArticuloList::codtipo_articulo()
{
    _depura ( "TipoArticuloList::codtipo_articulo", 0 );
    QTreeWidgetItem *it = m_listTipos->currentItem();
    if ( it ) {
        return it->text ( COL_CODTIPOARTICULO );
    } else {
        return "";
    } // end if
    _depura ( "END TipoArticuloList::codtipo_articulo", 0 );
}


///
/**
\return
**/
QString TipoArticuloList::idtipo_articulo()
{
    _depura ( "TipoArticuloList::idtipo_articulo", 0 );
    QTreeWidgetItem *it = m_listTipos->currentItem();
    if ( it ) {
        return it->text ( COL_IDTIPOARTICULO );
    } else {
        return "";
    } // end if
    _depura ( "TipoArticuloList::idtipo_articulo", 0 );
}


///
/**
\return
**/
QString TipoArticuloList::desctipo_articulo()
{
    _depura ( "TipoArticuloList::desctipo_articulo", 0 );
    QTreeWidgetItem *it = m_listTipos->currentItem();
    if ( it ) {
        return it->text ( COL_DESCTIPOARTICULO );
    } else {
        return "";
    } // end if
    _depura ( "END TipoArticuloList::desctipo_articulo", 0 );
}


/// Se ha seleccionado un item en la lista
/// Lo que hacemos es mostar el elemento
/// Si el anterior ha sido modificado pedimos para actuar en consecuencia.
/**
\param item
**/
void TipoArticuloList::on_m_listTipos_itemDoubleClicked ( QTreeWidgetItem *item, int )
{
    if ( m_modoConsulta ) {
        _depura ( "TipoArticuloList::on_m_listTipos_itemDoubleClicked", 0 );
        m_idtipo = item->text ( COL_IDTIPOARTICULO );
        emit selected ( m_idtipo );
    } // end if
    _depura ( "END TipoArticuloList::on_m_listTipos_itemDoubleClicked", 0 );
}


/// Se ha seleccionado un item en la lista
/// Lo que hacemos es mostar el elemento
/// Si el anterior ha sido modificado pedimos para actuar en consecuencia.
/**
\param current
**/
void TipoArticuloList::on_m_listTipos_currentItemChanged ( QTreeWidgetItem *current, QTreeWidgetItem *previous )
{
    _depura ( "TipoArticuloList::on_m_listTipos_currentItemChanged", 0 );
    /// Si estamos dentro del proceso de pintado salimos sin hacer nada ya que puede haber problemas.
    if ( m_semaforoPintar ) return;

    QString idtipoold = "";
    if ( previous ) {
        m_idtipo = previous->text ( COL_IDTIPOARTICULO );
    } // end if
    if ( m_idtipo != "" ) {
        trataModificado();
        pintar ( previous );
    } // end if
    m_idtipo = current->text ( COL_IDTIPOARTICULO );
    mostrarplantilla();
    _depura ( "END TipoArticuloList::on_m_listTipos_currentItemChanged", 0 );
}


///
/**
**/
void TipoArticuloList::mostrarplantilla()
{
    _depura ( "TipoArticuloList::mostrarplantilla", 0 );
    QString query;
    query = "SELECT * from tipo_articulo WHERE idtipo_articulo = " + m_idtipo;
    cursor2 *cursortipo = empresaBase()->cargacursor ( query );
    if ( !cursortipo->eof() ) {
        mui_codigotipo_articulo->setText ( cursortipo->valor ( "codtipo_articulo" ) );
        mui_desctipo_articulo->setPlainText ( cursortipo->valor ( "desctipo_articulo" ) );
    } // end if
    delete cursortipo;
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    _depura ( "END TipoArticuloList::mostrarplantilla", 0 );
}



///
/**
\return
**/
bool TipoArticuloList::trataModificado()
{
    _depura ( "TipoArticuloList::trataModificado", 0 );
    /// Si se ha modificado el contenido advertimos y guardamos.
    if ( dialogChanges_hayCambios() ) {
        if ( QMessageBox::warning ( this,
                                    tr ( "Guardar Tipo de Articulo" ),
                                    tr ( "Desea guardar los cambios?" ),
                                    QMessageBox::Ok,
                                    QMessageBox::Cancel ) == QMessageBox::Ok )
            on_mui_guardar_clicked();
        _depura ( "END TipoArticuloList::trataModificado", 0 );
        return TRUE;
    } // end if
    return FALSE;
}


/// SLOT que responde a la pulsacion del boton de guardar el tipo de iva que se esta editando.
/// Lo que hace es que se hace un update de todos los campos
/**
\return
**/
int TipoArticuloList::guardar()
{
    QString query = "UPDATE tipo_articulo SET codtipo_articulo = '" +
                    empresaBase()->sanearCadena ( mui_codigotipo_articulo->text() ) + "', desctipo_articulo = '" +
                    empresaBase()->sanearCadena ( mui_desctipo_articulo->toPlainText() ) + "' WHERE idtipo_articulo = " + m_idtipo;
    empresaBase()->begin();
    int error = empresaBase()->ejecuta ( query );
    if ( error ) {
        empresaBase()->rollback();
        return -1;
    } // end if
    empresaBase()->commit();
    //pintar();

    /// Vamos a hacer algo no reentrante.
    QList<QTreeWidgetItem *> listit =  m_listTipos->findItems ( m_idtipo, Qt::MatchExactly, COL_IDTIPOARTICULO );
    QTreeWidgetItem *it = listit.first();
    cursor2 *cursoraux1 = empresaBase()->cargacursor ( "SELECT * FROM tipo_articulo WHERE idtipo_articulo = " + m_idtipo );
    if ( !cursoraux1->eof() ) {
        it->setText ( COL_IDTIPOARTICULO, cursoraux1->valor ( "idtipo_articulo" ) );
        it->setText ( COL_CODTIPOARTICULO, cursoraux1->valor ( "codtipo_articulo" ) );
        it->setText ( COL_DESCTIPOARTICULO, cursoraux1->valor ( "desctipo_articulo" ) );
    } // end if
    delete cursoraux1;
    return 0;
}


/// SLOT que responde a la pulsacion del boton de nuevo tipo de iva
/// Inserta en la tabla de ivas
/**
\return
**/
void TipoArticuloList::on_mui_crear_clicked()
{
    _depura ( "TipoArticuloList::on_mui_crear_clicked", 0 );
    cursor2 *cur = NULL;
    /// Si se ha modificado el contenido advertimos y guardamos.
    trataModificado();
    try {
        QString query = "INSERT INTO tipo_articulo (codtipo_articulo, desctipo_articulo) VALUES ('XXXXXX', 'Descripcion del tipo')";
        empresaBase()->begin();
        int error = empresaBase()->ejecuta ( query );
        if ( error ) throw - 1;
        cur = empresaBase()->cargacursor ( "SELECT max(idtipo_articulo) AS idtipo FROM tipo_articulo" );
        empresaBase()->commit();
        m_idtipo = cur->valor ( "idtipo" );
        delete cur;
        pintar();
        _depura ( "END TipoArticuloList::on_mui_crear_clicked", 0 );
    } catch ( ... ) {
        mensajeInfo ( "Error en la creacion" );
        empresaBase()->rollback();
        return;
    } // end try
}


/// SLOT que responde a la pulsacion del boton de borrar la familia que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
/**
\return
**/
void TipoArticuloList::on_mui_borrar_clicked()
{
    _depura ( "TipoArticuloList::on_mui_borrar_clicked", 0 );


    int val = QMessageBox::question ( this,
                                      tr ( "Borrar" ) + " " + windowTitle(),
                                      tr ( "Desea eliminar" ) + " " + windowTitle(),
                                      QMessageBox::Yes,
                                      QMessageBox::Cancel | QMessageBox::Escape | QMessageBox::Default );

    if ( val == QMessageBox::Yes ) {
        if ( !borrar() ) {
            dialogChanges_cargaInicial();
            _depura ( windowTitle() + " " + "borrado satisfactoriamente.", 10 );
        } else {
            mensajeInfo ( windowTitle() + tr ( "No se ha podido borrar" ) );
        } // end if
    } // end if

    _depura ( "END TipoArticuloList::on_mui_borrar_clicked", 0 );
}


/// SLOT que responde a la pulsacion del botón de borrar la familia que se está editando.
/// Lo que hace es que se hace un update de todos los campos.
/**
\return
**/
int TipoArticuloList::borrar()
{
    _depura ( "TipoArticuloList::borrar", 0 );
    if ( m_idtipo == "" ) {
        mensajeInfo ( tr ( "Debe seleccionar un tipo" ) );
        return -1;
    } // end if
    try {
        QString query = "DELETE FROM tipo_articulo WHERE idtipo_articulo = " + m_idtipo;
        int error = empresaBase()->ejecuta ( query );
        if ( error ) {
            throw - 1;
        } // end if
        m_idtipo = "";
        dialogChanges_cargaInicial();
        pintar();
        _depura ( "END TipoArticuloList::borrar", 0 );
    } catch ( ... ) {
        mensajeInfo ( "Error al borrar la familia" );
        return -1;
    } // end try
    return 0;
}


/// Pinta una fila especifica del listado de familias presentado.
/**
\param it
**/
void TipoArticuloList::pintar ( QTreeWidgetItem *it )
{
    QString idtipo = it->text ( COL_IDTIPOARTICULO );
    if ( it ) {
        cursor2 * cursoraux1 = empresaBase()->cargacursor ( "SELECT * FROM tipo_articulo WHERE idtipo_articulo = " + idtipo );
        if ( !cursoraux1->eof() ) {
            it->setText ( COL_IDTIPOARTICULO, cursoraux1->valor ( "idtipo_articulo" ) );
            it->setText ( COL_CODTIPOARTICULO, cursoraux1->valor ( "codtipo_articulo" ) );
            it->setText ( COL_DESCTIPOARTICULO, cursoraux1->valor ( "desctipo_articulo" ) );
        } // end if
        delete cursoraux1;
    } // end if
}
