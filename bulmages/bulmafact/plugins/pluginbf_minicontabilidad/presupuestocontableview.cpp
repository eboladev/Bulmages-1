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
#include <QFile>
#include <QTextStream>

#include "presupuestocontableview.h"
#include "bfcompany.h"

#define COL_NOMBREPRESUPUESTOCONTABLE            0
#define COL_CODIGOCOMPLETOPRESUPUESTOCONTABLE    1
#define COL_DESCPRESUPUESTOCONTABLE           2
#define COL_IDPRESUPUESTOCONTABLE             3
#define COL_CODIGOPRESUPUESTOCONTABLE            4


///
/**
\param comp
\param parent
\param modoConsulta
**/
PresupuestoContableView::PresupuestoContableView ( BfCompany *comp, QWidget *parent, bool modoConsulta )
        : BfForm ( comp, parent )
{
    _depura ( "PresupuestoContableView::PresupuestoContableView", 0 );
    setupUi ( this );
    m_list->setColumnCount ( 3 );
    QStringList headers;
    headers << _ ( "Nombre" ) << _ ( "Codigo" ) << _ ( "Descripcion" ) << _ ( "Id lpresupuestocontable" ) << _ ( "Codigo completo" );
    m_list->setHeaderLabels ( headers );

    m_list->setColumnWidth ( 0, 200 );
    m_list->setColumnWidth ( 1, 100 );

    m_list->setColumnHidden ( COL_IDPRESUPUESTOCONTABLE, TRUE );
    m_list->setColumnHidden ( COL_CODIGOPRESUPUESTOCONTABLE, TRUE );

    m_semaforoPintar = FALSE;

    m_idlpresupuestocontable = "";



    if ( modoConsulta ) {
        setModoConsulta();
        mui_crear->hide();
        mui_guardar->hide();
        mui_borrar->hide();
        mui_cancelar->hide();
        mui_aceptar->hide();
        mui_imprimir->hide();
    } else {
        setModoEdicion();
        setAttribute ( Qt::WA_DeleteOnClose );
        mainCompany()->meteWindow ( windowTitle(), this, FALSE );
    } // end if

    pintar();
    _depura ( "END PresupuestoContableView::PresupuestoContableView", 0 );
}


///
/**
**/
PresupuestoContableView::~PresupuestoContableView()
{
    _depura ( "PresupuestoContableView::~PresupuestoContableView", 0 );
    _depura ( "END PresupuestoContableView::~PresupuestoContableView", 0 );
}


///
/**
**/
void PresupuestoContableView::pintar()
{
    _depura ( "PresupuestoContableView::pintar", 0 );
    /// Activamos el semaforo de pintado para que no haya slots concurrentes.
    m_semaforoPintar = TRUE;
    QTreeWidgetItem *it;
    QMap <int, QTreeWidgetItem*> Lista1;
    int padre;
    int idlpresupuestocontable = 0;
    BlDbRecordSet *cursoraux1, *cursoraux2;

    /// Vaciamos el arbol.
    while ( m_list->topLevelItemCount() > 0 ) {
        it = m_list->takeTopLevelItem ( 0 );
        delete it;
    } // end while

    cursoraux1 = mainCompany()->loadQuery ( "SELECT * FROM lpresupuestocontable WHERE padre IS NULL ORDER BY idlpresupuestocontable" );
    while ( !cursoraux1->eof() ) {
        padre = cursoraux1->valor ( "padre" ).toInt();
        idlpresupuestocontable = cursoraux1->valor ( "idlpresupuestocontable" ).toInt();
        it = new QTreeWidgetItem ( m_list );
        Lista1[idlpresupuestocontable] = it;
        it->setText ( COL_NOMBREPRESUPUESTOCONTABLE, cursoraux1->valor ( "nombrelpresupuestocontable" ) );
        it->setText ( COL_CODIGOPRESUPUESTOCONTABLE, cursoraux1->valor ( "codigolpresupuestocontable" ) );
        it->setText ( COL_DESCPRESUPUESTOCONTABLE, cursoraux1->valor ( "desclpresupuestocontable" ) );
        it->setText ( COL_IDPRESUPUESTOCONTABLE, cursoraux1->valor ( "idlpresupuestocontable" ) );
        it->setText ( COL_CODIGOCOMPLETOPRESUPUESTOCONTABLE, cursoraux1->valor ( "codigocompletolpresupuestocontable" ) );

        m_list->expandItem ( it );
        cursoraux1->nextRecord();
    } // end while
    delete cursoraux1;
    cursoraux2 = mainCompany()->loadQuery ( "SELECT * FROM lpresupuestocontable WHERE padre IS NOT NULL ORDER BY idlpresupuestocontable" );
    while ( !cursoraux2->eof() ) {
        padre = cursoraux2->valor ( "padre" ).toInt();
        idlpresupuestocontable = cursoraux2->valor ( "idlpresupuestocontable" ).toInt();
        it = new QTreeWidgetItem ( Lista1[padre] );
        Lista1[idlpresupuestocontable] = it;
        it->setText ( COL_NOMBREPRESUPUESTOCONTABLE, cursoraux2->valor ( "nombrelpresupuestocontable" ) );
        it->setText ( COL_CODIGOPRESUPUESTOCONTABLE, cursoraux2->valor ( "codigolpresupuestocontable" ) );
        it->setText ( COL_DESCPRESUPUESTOCONTABLE, cursoraux2->valor ( "desclpresupuestocontable" ) );
        it->setText ( COL_IDPRESUPUESTOCONTABLE, cursoraux2->valor ( "idlpresupuestocontable" ) );
        it->setText ( COL_CODIGOCOMPLETOPRESUPUESTOCONTABLE, cursoraux2->valor ( "codigocompletolpresupuestocontable" ) );
        m_list->expandItem ( it );
        cursoraux2->nextRecord();
    } // end while
    delete cursoraux2;
    m_idlpresupuestocontable = "";
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    m_semaforoPintar = FALSE; /// Desactivamos el semaforo de pintado.
    _depura ( "END PresupuestoContableView::pintar", 0 );
}


///
/**
\return
**/
QString PresupuestoContableView::codigoCompletoPresupuestoContable()
{
    _depura ( "PresupuestoContableView::codigoCompletoPresupuestoContable", 0 );
    QTreeWidgetItem *it = m_list->currentItem();
    if ( it ) {
        return it->text ( COL_CODIGOCOMPLETOPRESUPUESTOCONTABLE );
    } // end if
    _depura ( "END PresupuestoContableView::codigoCompletoPresupuestoContable", 0 );
    return "";
}


///
/**
\return
**/
QString PresupuestoContableView::idPresupuestoContable()
{
    _depura ( "PresupuestoContableView::idPresupuestoContable", 0 );
    QTreeWidgetItem *it = m_list->currentItem();
    if ( it ) {
        return it->text ( COL_IDPRESUPUESTOCONTABLE );
    } // end if
    _depura ( "END PresupuestoContableView::idPresupuestoContable", 0 );
    return "";
}


///
/**
\return
**/
QString PresupuestoContableView::nombrePresupuestoContable()
{
    _depura ( "PresupuestoContableView::nombrePresupuestoContable", 0 );
    QTreeWidgetItem *it = m_list->currentItem();
    if ( it ) {
        return it->text ( COL_NOMBREPRESUPUESTOCONTABLE );
    } // end if
    _depura ( "END PresupuestoContableView::nombrePresupuestoContable", 0 );
    return "";
}


/// Se ha seleccionado un item en la lista.
/// Lo que hacemos es mostar el elemento.
/// Si el anterior ha sido modificado pedimos para actuar en consecuencia.
/**
\param it
**/
void PresupuestoContableView::on_m_list_itemDoubleClicked ( QTreeWidgetItem *it )
{
    _depura ( "PresupuestoContableView::on_m_list_itemDoubleClicked", 0 );
    if ( m_modoConsulta ) {
        m_idlpresupuestocontable = it->text ( COL_IDPRESUPUESTOCONTABLE );
        emit selected ( m_idlpresupuestocontable );
    } // end if
    _depura ( "END PresupuestoContableView::on_m_list_itemDoubleClicked", 0 );
}


/// Se ha seleccionado un item en la lista.
/// Lo que hacemos es mostar el elemento.
/// Si el anterior ha sido modificado pedimos para actuar en consecuencia.
/**
\param current
\param previos
\return
**/
void PresupuestoContableView::on_m_list_currentItemChanged ( QTreeWidgetItem *current, QTreeWidgetItem *previous )
{

    if ( m_semaforoPintar ) return;
    m_semaforoPintar = TRUE;
    _depura ( "PresupuestoContableView::on_m_list_currentItemChanged", 0, m_idlpresupuestocontable );
    /// Si estamos dentro del proceso de pintado salimos sin hacer nada ya que puede haber problemas.

    if ( previous ) {
        m_idlpresupuestocontable = previous->text ( COL_IDPRESUPUESTOCONTABLE );
    } // end if

    if ( !m_idlpresupuestocontable.isEmpty() ) {
        trataModificado();
        if ( previous )
            pintar ( previous );

    } // end if
    if ( current ) {
        m_idlpresupuestocontable = current->text ( COL_IDPRESUPUESTOCONTABLE );
    } else {
        m_idlpresupuestocontable = "";
    } // end if

    mostrarplantilla();
    _depura ( "END PresupuestoContableView::on_m_list_currentItemChanged", 0 );
    m_semaforoPintar = FALSE;
}


///
/**
**/
void PresupuestoContableView::mostrarplantilla()
{
    _depura ( "PresupuestoContableView::mostrarplantilla", 0 );
    QString query;
    if ( !m_idlpresupuestocontable.isEmpty() ) {
/*
        mui_nombrePresupuestoContable->setEnabled ( TRUE );
        mui_descPresupuestoContable->setEnabled ( TRUE );
        mui_codigoCompletoPresupuestoContable->setEnabled ( TRUE );
        mui_codigoPresupuestoContable->setEnabled ( TRUE );
*/
        query = "SELECT * from lpresupuestocontable WHERE idlpresupuestocontable = " + m_idlpresupuestocontable;
        BlDbRecordSet *cursorlpresupuestocontable = mainCompany()->loadQuery ( query );
        if ( !cursorlpresupuestocontable->eof() ) {
/*
            mui_nombrePresupuestoContable->setText ( cursorlpresupuestocontable->valor ( "nombrelpresupuestocontable" ) );
            mui_descPresupuestoContable->setPlainText ( cursorlpresupuestocontable->valor ( "desclpresupuestocontable" ) );
            mui_codigoCompletoPresupuestoContable->setText ( cursorlpresupuestocontable->valor ( "codigocompletolpresupuestocontable" ) );
            mui_codigoPresupuestoContable->setText ( cursorlpresupuestocontable->valor ( "codigolpresupuestocontable" ) );
*/
        } // end if
        delete cursorlpresupuestocontable;
    } else {
/*
        mui_nombrePresupuestoContable->setEnabled ( FALSE );
        mui_descPresupuestoContable->setEnabled ( FALSE );
        mui_codigoCompletoPresupuestoContable->setEnabled ( FALSE );
        mui_codigoPresupuestoContable->setEnabled ( FALSE );
        mui_nombrePresupuestoContable->setText ( "" );
        mui_descPresupuestoContable->setPlainText ( "" );
        mui_codigoCompletoPresupuestoContable->setText ( "" );
        mui_codigoPresupuestoContable->setText ( "" );
*/
    } // end if
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    _depura ( "END PresupuestoContableView::mostrarplantilla", 0 );
}


///
/**
\return
**/
bool PresupuestoContableView::trataModificado()
{
    _depura ( "PresupuestoContableView::trataModificado", 0 );
    /// Si se ha modificado el contenido advertimos y guardamos.
    if ( dialogChanges_hayCambios() ) {
        if ( QMessageBox::warning ( this,
                                    _ ( "Guardar lpresupuestocontable" ),
                                    _ ( "Desea guardar los cambios?" ),
                                    QMessageBox::Ok,
                                    QMessageBox::Cancel ) == QMessageBox::Ok ) {
            on_mui_guardar_released();
        } // end if
    } // end if
    _depura ( "END PresupuestoContableView::trataModificado", 0 );
    return ( FALSE );
}


/// SLOT que responde a la pulsacion del boton de guardar el tipo de IVA que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
/**
\return
**/
int PresupuestoContableView::guardar()
{
    _depura ( "PresupuestoContableView::guardar", 0 );
/*
    QString prodfam;
    try {
        if ( m_idlpresupuestocontable.isEmpty() ) {
            mensajeInfo ( _ ( "Debe seleccionar una lpresupuestocontable" ) );
            return -1;
        } // end if

        QString query = "UPDATE lpresupuestocontable SET nombrelpresupuestocontable = '" +
                        mainCompany()->sanearCadena ( mui_nombrePresupuestoContable->text() ) + "', desclpresupuestocontable = '" +
                        mainCompany()->sanearCadena ( mui_descPresupuestoContable->toPlainText() ) + "' , codigolpresupuestocontable = '" + mui_codigoPresupuestoContable->text() + "' WHERE idlpresupuestocontable =" + m_idlpresupuestocontable;
        int error = mainCompany()->runQuery ( query );
        if ( error ) {
            throw - 1;
        } // end if
        /// Guardamos la informacion de la fila que esta seleccionada para volver
        /// a ponerla despues.
        QTreeWidgetItem *posicionCursor;
        posicionCursor = m_list->currentItem();
        if ( posicionCursor ) {
            posicionCursor->setSelected ( TRUE );
            /// Pintamos los datos en el listado.
            pintar ( posicionCursor );
        } // end if
        dialogChanges_cargaInicial();

        return 0;
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al guardar la lpresupuestocontable" ) );
        return -1;
    } // end try
*/
    _depura ( "END PresupuestoContableView::guardar", 0 );
}


/// Pinta una fila especifica del listado de lpresupuestocontables presentado.
/**
\param it
**/
void PresupuestoContableView::pintar ( QTreeWidgetItem *it )
{
    QString idlpresupuestocontable = it->text ( COL_IDPRESUPUESTOCONTABLE );
    if ( it ) {
        BlDbRecordSet * cursoraux1 = mainCompany()->loadQuery ( "SELECT * FROM lpresupuestocontable WHERE idlpresupuestocontable = " + idlpresupuestocontable );
        if ( !cursoraux1->eof() ) {
            it->setText ( COL_NOMBREPRESUPUESTOCONTABLE, cursoraux1->valor ( "nombrelpresupuestocontable" ) );
            it->setText ( COL_CODIGOPRESUPUESTOCONTABLE, cursoraux1->valor ( "codigolpresupuestocontable" ) );
            it->setText ( COL_DESCPRESUPUESTOCONTABLE, cursoraux1->valor ( "desclpresupuestocontable" ) );
            it->setText ( COL_IDPRESUPUESTOCONTABLE, cursoraux1->valor ( "idlpresupuestocontable" ) );
            it->setText ( COL_CODIGOCOMPLETOPRESUPUESTOCONTABLE, cursoraux1->valor ( "codigocompletolpresupuestocontable" ) );
        } // end if
        delete cursoraux1;
    } // end if
}


/// SLOT que responde a la pulsacion del boton de nuevo
/// Inserta en la tabla de PresupuestoContable
/**
**/
void PresupuestoContableView::on_mui_crear_released()
{
    _depura ( "PresupuestoContableView::on_mui_crear_released", 0 );
    try {
        mainCompany()->begin();
        /// Si se ha modificado el contenido advertimos y guardamos.
        trataModificado();
        QString padre;
        if ( m_idlpresupuestocontable != "" ) {
            padre = m_idlpresupuestocontable;
        } else {
            padre = "NULL";
        } // end if

        QString query = "INSERT INTO lpresupuestocontable (nombrelpresupuestocontable, desclpresupuestocontable, padre, codigolpresupuestocontable) VALUES ('NUEVA PRESUPUESTOCONTABLE', 'Descripcion de la lpresupuestocontable', " + padre + ", 'XXX')";

        int error = mainCompany()->runQuery ( query );
        if ( error ) {
            throw - 1;
        } // end if
        BlDbRecordSet *cur = mainCompany()->loadQuery ( "SELECT max(idlpresupuestocontable) AS idlpresupuestocontable FROM lpresupuestocontable" );
        mainCompany()->commit();
        m_idlpresupuestocontable = cur->valor ( "idlpresupuestocontable" );
        delete cur;
        pintar();
        _depura ( "END PresupuestoContableView::on_mui_crear_released", 0 );
    } catch ( ... ) {
        mainCompany()->rollback();
        mensajeInfo ( _ ( "Error al crear la lpresupuestocontable" ) );
    } // end try
}


/// SLOT que responde a la pulsacion del boton de nuevo desde Raiz
/// Inserta en la tabla de PresupuestoContable
/**
**/
/** No es necesario con las Qt 4.4
void PresupuestoContableView::on_mui_crearRaiz_released()
{
    _depura ( "PresupuestoContableView::on_mui_crearRaiz_released", 0 );
    try {
        mainCompany()->begin();
        /// Si se ha modificado el contenido advertimos y guardamos.
        trataModificado();

        QString query = "INSERT INTO lpresupuestocontable (nombrelpresupuestocontable, desclpresupuestocontable, padre, codigolpresupuestocontable) VALUES ('NUEVA FAMILIA', 'Descripcion de la lpresupuestocontable',  NULL , 'XXX')";

        int error = mainCompany()->runQuery ( query );
        if ( error ) {
            throw - 1;
        } // end if
        BlDbRecordSet *cur = mainCompany()->loadQuery ( "SELECT max(idlpresupuestocontable) AS idlpresupuestocontable FROM lpresupuestocontable" );
        mainCompany()->commit();
        m_idlpresupuestocontable = cur->valor ( "idlpresupuestocontable" );
        delete cur;
        pintar();
        _depura ( "END PresupuestoContableView::on_mui_crearRaiz_released", 0 );
    } catch ( ... ) {
        mainCompany()->rollback();
        mensajeInfo ( _("Error al crear la lpresupuestocontable") );
    } // end try
}

*/


///
/**
**/
void PresupuestoContableView::on_mui_borrar_released()
{
    _depura ( "PresupuestoContableView::on_mui_borrar_released", 0 );

    int val = QMessageBox::question ( this,
                                      _ ( "Borrar" ) + " " + windowTitle(),
                                      _ ( "Desea eliminar" ) + " " + windowTitle(),
                                      QMessageBox::Yes,
                                      QMessageBox::Cancel | QMessageBox::Escape | QMessageBox::Default );

    if ( val == QMessageBox::Yes ) {
        if ( !borrar() ) {
            dialogChanges_cargaInicial();
            _depura ( windowTitle() + " " + "borrado satisfactoriamente.", 10 );
        } else {
            mensajeInfo ( windowTitle() + " " + _ ( "no se ha podido borrar" ) );
        } // end if
    } // end if
    _depura ( "END PresupuestoContableView::on_mui_borrar_released", 0 );
}


/// SLOT que responde a la pulsacion del boton de borrar la lpresupuestocontable que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
/**
\return
**/
int PresupuestoContableView::borrar()
{
    _depura ( "PresupuestoContableView::borrar", 0 );
    if ( m_idlpresupuestocontable == "" ) {
        mensajeInfo ( _ ( "Debe seleccionar una lpresupuestocontable" ) );
        return -1;
    } // end if
    try {
        QString query = "DELETE FROM lpresupuestocontable WHERE idlpresupuestocontable = " + m_idlpresupuestocontable;
        int error = mainCompany()->runQuery ( query );
        if ( error ) {
            throw - 1;
        } // end if
        m_idlpresupuestocontable = "";
        dialogChanges_cargaInicial();
        pintar();
        _depura ( "END PresupuestoContableView::borrar", 0 );
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al borrar la lpresupuestocontable" ) );
        return -1;
    } // end try
    return 0;
}


///
/**
**/
void PresupuestoContableView::on_mui_imprimir_released()
{
    _depura ( "PresupuestoContableView::on_mui_imprimir_released", 0 );

    QString archivo = g_confpr->valor ( CONF_DIR_OPENREPORTS ) + "lpresupuestocontables.rml";
    QString archivod = g_confpr->valor ( CONF_DIR_USER ) + "lpresupuestocontables.rml";
    QString archivologo = g_confpr->valor ( CONF_DIR_OPENREPORTS ) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system ( archivo.toAscii().constData() );
    /// Copiamos el logo.
#ifdef WINDOWS

    archivologo = "copy "  + archivologo + " " + g_confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + g_confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#endif

    system ( archivologo.toAscii().constData() );
    QFile file;
    file.setFileName ( archivod );
    file.open ( QIODevice::ReadOnly );
    QTextStream stream ( &file );
    QString buff = stream.readAll();
    file.close();

    QString fitxersortidatxt = "";

    /// Linea de totales del presupuesto.
    fitxersortidatxt = "<blockTable style=\"tabla\" colWidths=\"3cm, 15cm\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "        <td>" + _ ( "Codigo" ) + "</td>";
    fitxersortidatxt += "        <td>" + _ ( "Nombre" ) + "</td>";
    fitxersortidatxt += "</tr>";

    BlDbRecordSet *cur = mainCompany()->loadQuery ( "SELECT * FROM lpresupuestocontable ORDER BY codigocompletolpresupuestocontable" );
    while ( !cur->eof() ) {
        fitxersortidatxt += "<tr>";
        fitxersortidatxt += "        <td>" + cur->valor ( "codigocompletolpresupuestocontable" ) + "</td>";
        fitxersortidatxt += "        <td>" + cur->valor ( "nombrelpresupuestocontable" ) + "</td>";
        fitxersortidatxt += "</tr>";
        cur->nextRecord();
    } // end if
    delete cur;
    fitxersortidatxt += "</blockTable>";

    buff.replace ( "[story]", fitxersortidatxt );

    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << buff;
        file.close();
    } // end if
    invocaPDF ( "lpresupuestocontables" );
    _depura ( "END PresupuestoContableView::on_mui_imprimir_released", 0 );
}


///
/**
**/
void PresupuestoContableView::on_mui_aceptar_released()
{
    _depura ( "PresupuestoContableView::on_mui_aceptar_released", 0 );
    QTreeWidgetItem *it = m_list->currentItem();
    if ( it ) {
        m_idlpresupuestocontable = it->text ( COL_IDPRESUPUESTOCONTABLE );
    } else {
        m_idlpresupuestocontable = "";
    } // end if
    BfForm::on_mui_aceptar_released();
    _depura ( "END PresupuestoContableView::on_mui_aceptar_released", 0 );
}


///
/**
**/
void PresupuestoContableView::setModoConsulta()
{
    m_modoConsulta = TRUE;
}


///
/**
**/
void PresupuestoContableView::setModoEdicion()
{
    m_modoConsulta = FALSE;
}




