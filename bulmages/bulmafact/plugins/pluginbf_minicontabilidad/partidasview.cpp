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

#include "partidasview.h"
#include "bfcompany.h"

#define COL_NOMBREPARTIDA            0
#define COL_CODIGOCOMPLETOPARTIDA    1
#define COL_DESCPARTIDA              2
#define COL_IDPARTIDA                3
#define COL_CODIGOPARTIDA            4


///
/**
\param comp
\param parent
\param selectMode
**/
PartidasView::PartidasView ( BfCompany *comp, QWidget *parent, bool selectMode )
        : BfForm ( comp, parent )
{
    blDebug ( "PartidasView::PartidasView", 0 );
    setupUi ( this );
    m_listPartidas->setColumnCount ( 3 );
    QStringList headers;
    headers << _ ( "Nombre" ) << _ ( "Codigo" ) << _ ( "Descripcion" ) << _ ( "Id partida" ) << _ ( "Codigo completo" );
    m_listPartidas->setHeaderLabels ( headers );

    m_listPartidas->setColumnWidth ( 0, 200 );
    m_listPartidas->setColumnWidth ( 1, 100 );

    m_listPartidas->setColumnHidden ( COL_IDPARTIDA, TRUE );
    m_listPartidas->setColumnHidden ( COL_CODIGOPARTIDA, TRUE );

    m_semaforoPintar = FALSE;

    m_idpartida = "";

    mui_nombrePartida->setEnabled ( FALSE );
    mui_descPartida->setEnabled ( FALSE );
    mui_codigoCompletoPartida->setEnabled ( FALSE );
    mui_codigoPartida->setEnabled ( FALSE );


    if ( selectMode ) {
        setSelectMode();
        groupBox1->hide();
        mui_detalles->hide();
        mui_crear->hide();
        mui_guardar->hide();
        mui_borrar->hide();
        mui_cancelar->hide();
        mui_aceptar->hide();
        mui_imprimir->hide();
    } else {
        setEditMode();
        setAttribute ( Qt::WA_DeleteOnClose );
        mainCompany()->insertWindow ( windowTitle(), this, FALSE );
    } // end if

    pintar();
    blScript(this);
    blDebug ( "END PartidasView::PartidasView", 0 );
}


///
/**
**/
PartidasView::~PartidasView()
{
    blDebug ( "PartidasView::~PartidasView", 0 );
    blDebug ( "END PartidasView::~PartidasView", 0 );
}


///
/**
**/
void PartidasView::pintar()
{
    blDebug ( "PartidasView::pintar", 0 );
    /// Activamos el semaforo de pintado para que no haya slots concurrentes.
    m_semaforoPintar = TRUE;
    QTreeWidgetItem *it;
    QMap <int, QTreeWidgetItem*> Lista1;
    int padre;
    int idpartida = 0;
    BlDbRecordSet *cursoraux1, *cursoraux2;

    /// Vaciamos el arbol.
    while ( m_listPartidas->topLevelItemCount() > 0 ) {
        it = m_listPartidas->takeTopLevelItem ( 0 );
        delete it;
    } // end while

    /// Parece que lo mas rentable es crear una inicial que siempre este y a partir de la que carguen las demas
    m_init = new QTreeWidgetItem ( m_listPartidas );
    m_init->setText ( COL_NOMBREPARTIDA, _("Partidas Contables") );
    m_listPartidas->expandItem ( m_init );

    cursoraux1 = mainCompany()->loadQuery ( "SELECT * FROM partida WHERE padre IS NULL ORDER BY idpartida" );
    while ( !cursoraux1->eof() ) {
        padre = cursoraux1->value( "padre" ).toInt();
        idpartida = cursoraux1->value( "idpartida" ).toInt();
        it = new QTreeWidgetItem ( m_init );
        Lista1[idpartida] = it;
        it->setText ( COL_NOMBREPARTIDA, cursoraux1->value( "nombrepartida" ) );
        it->setText ( COL_CODIGOPARTIDA, cursoraux1->value( "codigopartida" ) );
        it->setText ( COL_DESCPARTIDA, cursoraux1->value( "descpartida" ) );
        it->setText ( COL_IDPARTIDA, cursoraux1->value( "idpartida" ) );
        it->setText ( COL_CODIGOCOMPLETOPARTIDA, cursoraux1->value( "codigocompletopartida" ) );

        m_listPartidas->expandItem ( it );
        cursoraux1->nextRecord();
    } // end while
    delete cursoraux1;
    cursoraux2 = mainCompany()->loadQuery ( "SELECT * FROM partida WHERE padre IS NOT NULL ORDER BY idpartida" );
    while ( !cursoraux2->eof() ) {
        padre = cursoraux2->value( "padre" ).toInt();
        idpartida = cursoraux2->value( "idpartida" ).toInt();
        it = new QTreeWidgetItem ( Lista1[padre] );
        Lista1[idpartida] = it;
        it->setText ( COL_NOMBREPARTIDA, cursoraux2->value( "nombrepartida" ) );
        it->setText ( COL_CODIGOPARTIDA, cursoraux2->value( "codigopartida" ) );
        it->setText ( COL_DESCPARTIDA, cursoraux2->value( "descpartida" ) );
        it->setText ( COL_IDPARTIDA, cursoraux2->value( "idpartida" ) );
        it->setText ( COL_CODIGOCOMPLETOPARTIDA, cursoraux2->value( "codigocompletopartida" ) );
        m_listPartidas->expandItem ( it );
        cursoraux2->nextRecord();
    } // end while
    delete cursoraux2;
    m_idpartida = "";
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_readValues();
    m_semaforoPintar = FALSE; /// Desactivamos el semaforo de pintado.
    blDebug ( "END PartidasView::pintar", 0 );
}


///
/**
\return
**/
QString PartidasView::codigoCompletoPartida()
{
    blDebug ( "PartidasView::codigoCompletoPartida", 0 );
    QTreeWidgetItem *it = m_listPartidas->currentItem();
    if ( it ) {
        return it->text ( COL_CODIGOCOMPLETOPARTIDA );
    } // end if
    blDebug ( "END PartidasView::codigoCompletoPartida", 0 );
    return "";
}


///
/**
\return
**/
QString PartidasView::idPartida()
{
    blDebug ( "PartidasView::idPartida", 0 );
    QTreeWidgetItem *it = m_listPartidas->currentItem();
    if ( it ) {
        return it->text ( COL_IDPARTIDA );
    } // end if
    blDebug ( "END PartidasView::idPartida", 0 );
    return "";
}


///
/**
\return
**/
QString PartidasView::nombrePartida()
{
    blDebug ( "PartidasView::nombrePartida", 0 );
    QTreeWidgetItem *it = m_listPartidas->currentItem();
    if ( it ) {
        return it->text ( COL_NOMBREPARTIDA );
    } // end if
    blDebug ( "END PartidasView::nombrePartida", 0 );
    return "";
}


/// Se ha seleccionado un item en la lista.
/// Lo que hacemos es mostar el elemento.
/// Si el anterior ha sido modificado pedimos para actuar en consecuencia.
/**
\param it
**/
void PartidasView::on_m_listPartidas_itemDoubleClicked ( QTreeWidgetItem *it )
{
    blDebug ( "PartidasView::on_m_listPartidas_itemDoubleClicked", 0 );
    if ( m_selectMode ) {
        m_idpartida = it->text ( COL_IDPARTIDA );
	if ( !m_idpartida.isEmpty() ) {
	  emit selected ( m_idpartida );
	} // end if
    } // end if
    blDebug ( "END PartidasView::on_m_listPartidas_itemDoubleClicked", 0 );
}


/// Se ha seleccionado un item en la lista.
/// Lo que hacemos es mostar el elemento.
/// Si el anterior ha sido modificado pedimos para actuar en consecuencia.
/**
\param current
\param previos
\return
**/
void PartidasView::on_m_listPartidas_currentItemChanged ( QTreeWidgetItem *current, QTreeWidgetItem *previous )
{

    if ( m_semaforoPintar ) return;
    m_semaforoPintar = TRUE;
    blDebug ( "PartidasView::on_m_listPartidas_currentItemChanged", 0, m_idpartida );
    /// Si estamos dentro del proceso de pintado salimos sin hacer nada ya que puede haber problemas.

    if ( previous ) {
        m_idpartida = previous->text ( COL_IDPARTIDA );
    } // end if

    if ( !m_idpartida.isEmpty() ) {
        trataModificado();
        if ( previous )
            pintar ( previous );

    } // end if
    if ( current ) {
        m_idpartida = current->text ( COL_IDPARTIDA );
    } else {
        m_idpartida = "";
    } // end if

    mostrarplantilla();
    blDebug ( "END PartidasView::on_m_listPartidas_currentItemChanged", 0 );
    m_semaforoPintar = FALSE;
}


///
/**
**/
void PartidasView::mostrarplantilla()
{
    blDebug ( "PartidasView::mostrarplantilla", 0 );
    QString query;
    if ( !m_idpartida.isEmpty() ) {
        mui_nombrePartida->setEnabled ( TRUE );
        mui_descPartida->setEnabled ( TRUE );
        mui_codigoCompletoPartida->setEnabled ( TRUE );
        mui_codigoPartida->setEnabled ( TRUE );

        query = "SELECT * from partida WHERE idpartida = " + m_idpartida;
        BlDbRecordSet *cursorpartida = mainCompany()->loadQuery ( query );
        if ( !cursorpartida->eof() ) {
            mui_nombrePartida->setText ( cursorpartida->value( "nombrepartida" ) );
            mui_descPartida->setPlainText ( cursorpartida->value( "descpartida" ) );
            mui_codigoCompletoPartida->setText ( cursorpartida->value( "codigocompletopartida" ) );
            mui_codigoPartida->setText ( cursorpartida->value( "codigopartida" ) );
        } // end if
        delete cursorpartida;
    } else {
        mui_nombrePartida->setEnabled ( FALSE );
        mui_descPartida->setEnabled ( FALSE );
        mui_codigoCompletoPartida->setEnabled ( FALSE );
        mui_codigoPartida->setEnabled ( FALSE );
        mui_nombrePartida->setText ( "" );
        mui_descPartida->setPlainText ( "" );
        mui_codigoCompletoPartida->setText ( "" );
        mui_codigoPartida->setText ( "" );
    } // end if
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_readValues();
    blDebug ( "END PartidasView::mostrarplantilla", 0 );
}


///
/**
\return
**/
bool PartidasView::trataModificado()
{
    blDebug ( "PartidasView::trataModificado", 0 );
    /// Si se ha modificado el contenido advertimos y guardamos.
    if ( dialogChanges_isChanged() ) {
        if ( QMessageBox::warning ( this,
                                    _ ( "Guardar partida" ),
                                    _ ( "Desea guardar los cambios?" ),
                                    QMessageBox::Ok,
                                    QMessageBox::Cancel ) == QMessageBox::Ok ) {
            on_mui_guardar_clicked();
        } // end if
    } // end if
    blDebug ( "END PartidasView::trataModificado", 0 );
    return ( FALSE );
}


/// SLOT que responde a la pulsacion del boton de guardar el tipo de IVA que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
/**
\return
**/
int PartidasView::save()
{
    blDebug ( "PartidasView::guardar", 0 );
    QString prodfam;
    try {
        if ( m_idpartida.isEmpty() ) {
            blMsgInfo ( _ ( "Debe seleccionar una partida" ) );
            return -1;
        } // end if

        QString query = "UPDATE partida SET nombrepartida = '" +
                        mainCompany()->sanearCadena ( mui_nombrePartida->text() ) + "', descpartida = '" +
                        mainCompany()->sanearCadena ( mui_descPartida->toPlainText() ) + "' , codigopartida = '" + mui_codigoPartida->text() + "' WHERE idpartida =" + m_idpartida;
        int error = mainCompany()->runQuery ( query );
        if ( error ) {
            throw - 1;
        } // end if
        /// Guardamos la informacion de la fila que esta seleccionada para volver
        /// a ponerla despues.
        QTreeWidgetItem *posicionCursor;
        posicionCursor = m_listPartidas->currentItem();
        if ( posicionCursor ) {
            posicionCursor->setSelected ( TRUE );
            /// Pintamos los datos en el listado.
            pintar ( posicionCursor );
        } // end if
        dialogChanges_readValues();
        blDebug ( "END PartidasView::guardar", 0 );
        return 0;
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al guardar la partida" ) );
        return -1;
    } // end try
}


/// Pinta una fila especifica del listado de partidas presentado.
/**
\param it
**/
void PartidasView::pintar ( QTreeWidgetItem *it )
{
    QString idpartida = it->text ( COL_IDPARTIDA );
    if ( it ) {
        BlDbRecordSet * cursoraux1 = mainCompany()->loadQuery ( "SELECT * FROM partida WHERE idpartida = " + idpartida );
        if ( !cursoraux1->eof() ) {
            it->setText ( COL_NOMBREPARTIDA, cursoraux1->value( "nombrepartida" ) );
            it->setText ( COL_CODIGOPARTIDA, cursoraux1->value( "codigopartida" ) );
            it->setText ( COL_DESCPARTIDA, cursoraux1->value( "descpartida" ) );
            it->setText ( COL_IDPARTIDA, cursoraux1->value( "idpartida" ) );
            it->setText ( COL_CODIGOCOMPLETOPARTIDA, cursoraux1->value( "codigocompletopartida" ) );
        } // end if
        delete cursoraux1;
    } // end if
}


/// SLOT que responde a la pulsacion del boton de nuevo
/// Inserta en la tabla de Partidas
/**
**/
void PartidasView::on_mui_crear_clicked()
{
    blDebug ( "PartidasView::on_mui_crear_clicked", 0 );
    try {
        mainCompany()->begin();
        /// Si se ha modificado el contenido advertimos y guardamos.
        trataModificado();
        QString padre;
        if ( m_idpartida != "" ) {
            padre = m_idpartida;
        } else {
            padre = "NULL";
        } // end if

        QString query = "INSERT INTO partida (nombrepartida, descpartida, padre, codigopartida) VALUES ('NUEVA PARTIDA', 'Descripcion de la partida', " + padre + ", 'XXX')";

        int error = mainCompany()->runQuery ( query );
        if ( error ) {
            throw - 1;
        } // end if
        BlDbRecordSet *cur = mainCompany()->loadQuery ( "SELECT max(idpartida) AS idpartida FROM partida" );
        mainCompany()->commit();
        m_idpartida = cur->value( "idpartida" );
        delete cur;
        pintar();
        blDebug ( "END PartidasView::on_mui_crear_clicked", 0 );
    } catch ( ... ) {
        mainCompany()->rollback();
        blMsgInfo ( _ ( "Error al crear la partida" ) );
    } // end try
}


///
/**
**/
void PartidasView::on_mui_borrar_clicked()
{
    blDebug ( "PartidasView::on_mui_borrar_clicked", 0 );

    int val = QMessageBox::question ( this,
                                      _ ( "Borrar" ) + " " + windowTitle(),
                                      _ ( "Desea eliminar" ) + " " + windowTitle(),
                                      QMessageBox::Yes,
                                      QMessageBox::Cancel | QMessageBox::Escape | QMessageBox::Default );

    if ( val == QMessageBox::Yes ) {
        if ( !remove() ) {
            dialogChanges_readValues();
            blDebug ( windowTitle() + " " + "borrado satisfactoriamente.", 10 );
        } else {
            blMsgInfo ( windowTitle() + " " + _ ( "no se ha podido borrar" ) );
        } // end if
    } // end if
    blDebug ( "END PartidasView::on_mui_borrar_clicked", 0 );
}


/// SLOT que responde a la pulsacion del boton de borrar la partida que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
/**
\return
**/
int PartidasView::remove()
{
    blDebug ( "PartidasView::borrar", 0 );
    if ( m_idpartida == "" ) {
        blMsgInfo ( _ ( "Debe seleccionar una partida" ) );
        return -1;
    } // end if
    try {
        QString query = "DELETE FROM partida WHERE idpartida = " + m_idpartida;
        int error = mainCompany()->runQuery ( query );
        if ( error ) {
            throw - 1;
        } // end if
        m_idpartida = "";
        dialogChanges_readValues();
        pintar();
        blDebug ( "END PartidasView::borrar", 0 );
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al borrar la partida" ) );
        return -1;
    } // end try
    return 0;
}


///
/**
**/
void PartidasView::on_mui_imprimir_clicked()
{
    blDebug ( "PartidasView::on_mui_imprimir_clicked", 0 );

    QString archivo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "partidas.rml";
    QString archivod = g_confpr->value( CONF_DIR_USER ) + "partidas.rml";
    QString archivologo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef Q_OS_WIN32

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system ( archivo.toAscii().constData() );
    /// Copiamos el logo.
#ifdef Q_OS_WIN32

    archivologo = "copy "  + archivologo + " " + g_confpr->value( CONF_DIR_USER ) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + g_confpr->value( CONF_DIR_USER ) + "logo.jpg";
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

    BlDbRecordSet *cur = mainCompany()->loadQuery ( "SELECT * FROM partida ORDER BY codigocompletopartida" );
    while ( !cur->eof() ) {
        fitxersortidatxt += "<tr>";
        fitxersortidatxt += "        <td>" + cur->value( "codigocompletopartida" ) + "</td>";
        fitxersortidatxt += "        <td>" + cur->value( "nombrepartida" ) + "</td>";
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
    blCreateAndLoadPDF ( "partidas" );
    blDebug ( "END PartidasView::on_mui_imprimir_clicked", 0 );
}


///
/**
**/
void PartidasView::on_mui_aceptar_clicked()
{
    blDebug ( "PartidasView::on_mui_aceptar_clicked", 0 );
    QTreeWidgetItem *it = m_listPartidas->currentItem();
    if ( it ) {
        m_idpartida = it->text ( COL_IDPARTIDA );
    } else {
        m_idpartida = "";
    } // end if
    BfForm::on_mui_aceptar_clicked();
    blDebug ( "END PartidasView::on_mui_aceptar_clicked", 0 );
}


///
/**
**/
void PartidasView::setSelectMode()
{
    m_selectMode = TRUE;
}


///
/**
**/
void PartidasView::setEditMode()
{
    m_selectMode = FALSE;
}




