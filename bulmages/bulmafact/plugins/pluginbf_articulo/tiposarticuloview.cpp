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

#include <QtCore/QMap>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMessageBox>

#include "tiposarticuloview.h"
#include "bfcompany.h"

#define COL_IDTIPOARTICULO    0
#define COL_CODTIPOARTICULO   1
#define COL_DESCTIPOARTICULO  2


///
/**
\param comp
\param parent
\param selectMode
**/
TipoArticuloList::TipoArticuloList ( BfCompany *comp, QWidget *parent, bool selectMode )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    m_listTipos->setColumnCount ( 3 );
    QStringList headers;
    headers << _ ( "Id" ) << _ ( "Codigo" ) << _ ( "Descripcion" );
    m_listTipos->setHeaderLabels ( headers );
    m_semaforoPintar = false;
    m_idtipo = "";

    mui_codigotipo_articulo->setEnabled ( false );
    mui_desctipo_articulo->setEnabled ( false );
    if ( selectMode ) {
        setSelectMode();
        groupBox1->hide();
        mui_detalles->hide();
        mui_crear->hide();
        mui_guardar->hide();
        mui_borrar->hide();
        mui_cancelar->hide();
        mui_aceptar->hide();
    } else {
        setEditMode();
        setAttribute ( Qt::WA_DeleteOnClose );
        mainCompany()->insertWindow ( windowTitle(), this, false );
    } // end if
    pintar();
    blScript(this);
    
}


///
/**
**/
TipoArticuloList::~TipoArticuloList()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void TipoArticuloList::setSelectMode()
{
    BL_FUNC_DEBUG
    m_selectMode = true;
    
}


///
/**
**/
void TipoArticuloList::setEditMode()
{
    BL_FUNC_DEBUG
    m_selectMode = false;
    
}



///
/**
**/
void TipoArticuloList::pintar()
{
    BL_FUNC_DEBUG
    m_semaforoPintar = true;
    BlDbRecordSet *cursoraux1 = NULL;
    try {
        QTreeWidgetItem * it;


        /// Vaciamos el arbol.
        while ( m_listTipos->topLevelItemCount() > 0 ) {
            it = m_listTipos->takeTopLevelItem ( 0 );
            delete it;
        } // end while

        cursoraux1 = mainCompany()->loadQuery ( "SELECT * FROM tipo_articulo ORDER BY codtipo_articulo" );
        while ( !cursoraux1->eof() ) {
            it = new QTreeWidgetItem ( m_listTipos );
            it->setText ( COL_IDTIPOARTICULO, cursoraux1->value( "idtipo_articulo" ) );
            it->setText ( COL_CODTIPOARTICULO, cursoraux1->value( "codtipo_articulo" ) );
            it->setText ( COL_DESCTIPOARTICULO, cursoraux1->value( "desctipo_articulo" ) );
            //it->setOpen(true);
            cursoraux1->nextRecord();
        } // end while
        delete cursoraux1;
        m_idtipo = "";
        /// Comprobamos cual es la cadena inicial.
        dialogChanges_readValues();
        m_semaforoPintar = false;
        
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error en la carga" ) );
        if ( cursoraux1 ) delete cursoraux1;
    } // end try
}


///
/**
\return
**/
QString TipoArticuloList::codtipo_articulo()
{
    BL_FUNC_DEBUG
    QTreeWidgetItem *it = m_listTipos->currentItem();
    if ( it ) {
        return it->text ( COL_CODTIPOARTICULO );
    } else {
        return "";
    } // end if
    
}


///
/**
\return
**/
QString TipoArticuloList::idtipo_articulo()
{
    BL_FUNC_DEBUG
    QTreeWidgetItem *it = m_listTipos->currentItem();

    if ( it ) {
        return it->text ( COL_IDTIPOARTICULO );
    } else {
        return "";
    } // end if

    
}


///
/**
\return
**/
QString TipoArticuloList::desctipo_articulo()
{
    BL_FUNC_DEBUG
    QTreeWidgetItem *it = m_listTipos->currentItem();
    if ( it ) {
        return it->text ( COL_DESCTIPOARTICULO );
    } else {
        return "";
    } // end if
    
}


/// Se ha seleccionado un item en la lista
/// Lo que hacemos es mostar el elemento
/// Si el anterior ha sido modificado pedimos para actuar en consecuencia.
/**
\param item
**/
void TipoArticuloList::on_m_listTipos_itemDoubleClicked ( QTreeWidgetItem *item, int )
{
    BL_FUNC_DEBUG
    if ( m_selectMode ) {
        m_idtipo = item->text ( COL_IDTIPOARTICULO );
        emit selected ( m_idtipo );
    } // end if
    
}


/// Se ha seleccionado un item en la lista
/// Lo que hacemos es mostar el elemento
/// Si el anterior ha sido modificado pedimos para actuar en consecuencia.
/**
\param current
**/
void TipoArticuloList::on_m_listTipos_currentItemChanged ( QTreeWidgetItem *current, QTreeWidgetItem *previous )
{
    BL_FUNC_DEBUG
    if ( m_semaforoPintar ) return;
    m_semaforoPintar = true;

    /// Si estamos dentro del proceso de pintado salimos sin hacer nada ya que puede haber problemas.
    QString idtipoold = "";
    if ( previous ) {
        m_idtipo = previous->text ( COL_IDTIPOARTICULO );
    } // end if

    if ( !m_idtipo.isEmpty() ) {
        trataModificado();
        if ( previous )
            pintar ( previous );
    } // end if

    if ( current ) {
        m_idtipo = current->text ( COL_IDTIPOARTICULO );
    } else {
        m_idtipo = "";
    } // end if

    mostrarplantilla();
    m_semaforoPintar = false;
    
}


///
/**
**/
void TipoArticuloList::mostrarplantilla()
{
    BL_FUNC_DEBUG
    if ( !m_idtipo.isEmpty() ) {
        mui_codigotipo_articulo->setEnabled ( true );
        mui_desctipo_articulo->setEnabled ( true );

        QString query;
        query = "SELECT * from tipo_articulo WHERE idtipo_articulo = " + m_idtipo;
        BlDbRecordSet *cursortipo = mainCompany()->loadQuery ( query );
        if ( !cursortipo->eof() ) {
            mui_codigotipo_articulo->setText ( cursortipo->value( "codtipo_articulo" ) );
            mui_desctipo_articulo->setPlainText ( cursortipo->value( "desctipo_articulo" ) );
        } // end if
        delete cursortipo;

    } else {
        mui_codigotipo_articulo->setEnabled ( false );
        mui_desctipo_articulo->setEnabled ( false );
        mui_codigotipo_articulo->setText ( "" );
        mui_desctipo_articulo->setText ( "" );
    } // end if
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_readValues();
    
}



///
/**
\return
**/
bool TipoArticuloList::trataModificado()
{
    BL_FUNC_DEBUG
    /// Si se ha modificado el contenido advertimos y guardamos.
    if ( dialogChanges_isChanged() ) {
        if ( QMessageBox::warning ( this,
                                    _ ( "Guardar Tipo de Articulo" ),
                                    _ ( "Desea guardar los cambios?" ),
                                    QMessageBox::Ok,
                                    QMessageBox::Cancel ) == QMessageBox::Ok )
            on_mui_guardar_clicked();
        
        return true;
    } // end if
    return false;
}


/// SLOT que responde a la pulsacion del boton de guardar el tipo de iva que se esta editando.
/// Lo que hace es que se hace un update de todos los campos
/**
\return
**/
int TipoArticuloList::save()
{
    if ( m_idtipo.isEmpty() )
        return 0;

    if ( mui_codigotipo_articulo->text() == "" )
        return 0;

    QString query = "UPDATE tipo_articulo SET codtipo_articulo = '" +
                    mainCompany()->sanearCadena ( mui_codigotipo_articulo->text() ) + "', desctipo_articulo = '" +
                    mainCompany()->sanearCadena ( mui_desctipo_articulo->toPlainText() ) + "' WHERE idtipo_articulo = " + m_idtipo;
    mainCompany()->begin();
    int error = mainCompany()->runQuery ( query );
    if ( error ) {
        mainCompany()->rollback();
        return -1;
    } // end if
    mainCompany()->commit();
    //pintar();

    /// Vamos a hacer algo no reentrante.
    QList<QTreeWidgetItem *> listit =  m_listTipos->findItems ( m_idtipo, Qt::MatchExactly, COL_IDTIPOARTICULO );
    QTreeWidgetItem *it = listit.first();
    if ( it ) {
        BlDbRecordSet *cursoraux1 = mainCompany()->loadQuery ( "SELECT * FROM tipo_articulo WHERE idtipo_articulo = " + m_idtipo );
        if ( !cursoraux1->eof() ) {
            it->setText ( COL_IDTIPOARTICULO, cursoraux1->value( "idtipo_articulo" ) );
            it->setText ( COL_CODTIPOARTICULO, cursoraux1->value( "codtipo_articulo" ) );
            it->setText ( COL_DESCTIPOARTICULO, cursoraux1->value( "desctipo_articulo" ) );
        } // end if
        delete cursoraux1;
    } // end if
    return 0;
}


/// SLOT que responde a la pulsacion del boton de nuevo tipo de iva
/// Inserta en la tabla de ivas
/**
\return
**/
void TipoArticuloList::on_mui_crear_clicked()
{
    BL_FUNC_DEBUG
    BlDbRecordSet *cur = NULL;
    /// Si se ha modificado el contenido advertimos y guardamos.
    trataModificado();
    try {
        QString query = "INSERT INTO tipo_articulo (codtipo_articulo, desctipo_articulo) VALUES ('XXXXXX', 'Descripcion del tipo')";
        mainCompany()->begin();
        int error = mainCompany()->runQuery ( query );
        if ( error ) throw - 1;
        cur = mainCompany()->loadQuery ( "SELECT max(idtipo_articulo) AS idtipo FROM tipo_articulo" );
        mainCompany()->commit();
        m_idtipo = cur->value( "idtipo" );
        delete cur;
        pintar();
        
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error en la creacion" ) );
        mainCompany()->rollback();
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
    BL_FUNC_DEBUG

    int val = QMessageBox::question ( this,
                                      _ ( "Borrar" ) + " " + windowTitle(),
                                      _ ( "Desea eliminar" ) + " " + windowTitle(),
                                      QMessageBox::Yes,
                                      QMessageBox::Cancel | QMessageBox::Escape | QMessageBox::Default );

    if ( val == QMessageBox::Yes ) {
        if ( !remove() ) {
            dialogChanges_readValues();
	    BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("'%1' borrado.")).arg(windowTitle()) );
        } else {
	    BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("Error al borrar '%1'.")).arg(windowTitle()) );
            blMsgInfo ( windowTitle() + " " + _ ( "no se ha podido borrar" ) );
        } // end if
    } // end if

    
}


/// SLOT que responde a la pulsacion del boton de borrar la familia que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
/**
\return
**/
int TipoArticuloList::remove()
{
    BL_FUNC_DEBUG
    if ( m_idtipo == "" ) {
        blMsgInfo ( _ ( "Debe seleccionar un tipo" ) );
        return -1;
    } // end if
    try {
        QString query = "DELETE FROM tipo_articulo WHERE idtipo_articulo = " + m_idtipo;
        int error = mainCompany()->runQuery ( query );
        if ( error ) {
            throw - 1;
        } // end if
        m_idtipo = "";
        dialogChanges_readValues();
        pintar();
        
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al borrar la familia" ) );
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
        BlDbRecordSet * cursoraux1 = mainCompany()->loadQuery ( "SELECT * FROM tipo_articulo WHERE idtipo_articulo = " + idtipo );
        if ( !cursoraux1->eof() ) {
            it->setText ( COL_IDTIPOARTICULO, cursoraux1->value( "idtipo_articulo" ) );
            it->setText ( COL_CODTIPOARTICULO, cursoraux1->value( "codtipo_articulo" ) );
            it->setText ( COL_DESCTIPOARTICULO, cursoraux1->value( "desctipo_articulo" ) );
        } // end if
        delete cursoraux1;
    } // end if
}
