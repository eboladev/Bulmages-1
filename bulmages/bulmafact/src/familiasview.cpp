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

#include "familiasview.h"
#include "company.h"
#include "funcaux.h"

#define COL_NOMFAMILIA            0
#define COL_CODCOMPLETOFAMILIA    1
#define COL_DESCFAMILIA           2
#define COL_IDFAMILIA             3
#define COL_CODFAMILIA            4
#define COL_PRODUCTOFISICOFAMILIA 5


///
/**
\param comp
\param parent
\param modoConsulta
**/
FamiliasView::FamiliasView ( Company *comp, QWidget *parent, bool modoConsulta )
        : FichaBf ( comp, parent )
{
    _depura ( "FamiliasView::FamiliasView", 0 );
    setupUi ( this );
    m_listFamilias->setColumnCount ( 3 );
    QStringList headers;
    headers << tr ( "Nombre" ) << tr ( "Codigo" ) << tr ( "Descripcion" ) << tr ( "Id familia" ) << tr ( "Codigo completo" );
    m_listFamilias->setHeaderLabels ( headers );

    m_listFamilias->setColumnWidth ( 0, 200 );
    m_listFamilias->setColumnWidth ( 1, 100 );

    m_listFamilias->setColumnHidden ( COL_IDFAMILIA, TRUE );
    m_listFamilias->setColumnHidden ( COL_CODFAMILIA, TRUE );
    m_listFamilias->setColumnHidden ( COL_PRODUCTOFISICOFAMILIA, TRUE );

    m_semaforoPintar = FALSE;

    m_idfamilia = "";

    if ( modoConsulta ) {
        setModoConsulta();
        groupBox1->hide();
        mui_detalles->hide();
        mui_crear->hide();
        mui_guardar->hide();
        mui_borrar->hide();
        mui_cancelar->hide();
        mui_aceptar->hide();
        mui_imprimir->hide();
    } else {
        setModoEdicion();
        setAttribute ( Qt::WA_DeleteOnClose );
        empresaBase()->meteWindow ( windowTitle(), this, FALSE );
    } // end if

    pintar();
    _depura ( "END FamiliasView::FamiliasView", 0 );
}


///
/**
**/
FamiliasView::~FamiliasView()
{
    _depura ( "FamiliasView::~FamiliasView", 0 );
    _depura ( "END FamiliasView::~FamiliasView", 0 );
}


///
/**
**/
void FamiliasView::pintar()
{
    _depura ( "FamiliasView::pintar", 0 );
    /// Activamos el semaforo de pintado para que no haya slots concurrentes.
    m_semaforoPintar = TRUE;
    QTreeWidgetItem *it;
    QMap <int, QTreeWidgetItem*> Lista1;
    int padre;
    int idfamilia = 0;
    cursor2 *cursoraux1, *cursoraux2;

    /// Vaciamos el arbol.
    while ( m_listFamilias->topLevelItemCount() > 0 ) {
        it = m_listFamilias->takeTopLevelItem ( 0 );
        delete it;
    } // end while

    cursoraux1 = empresaBase()->cargacursor ( "SELECT * FROM familia WHERE padrefamilia IS NULL ORDER BY idfamilia" );
    while ( !cursoraux1->eof() ) {
        padre = cursoraux1->valor ( "padrefamilia" ).toInt();
        idfamilia = cursoraux1->valor ( "idfamilia" ).toInt();
        it = new QTreeWidgetItem ( m_listFamilias );
        Lista1[idfamilia] = it;
        it->setText ( COL_NOMFAMILIA, cursoraux1->valor ( "nombrefamilia" ) );
        it->setText ( COL_CODFAMILIA, cursoraux1->valor ( "codigofamilia" ) );
        it->setText ( COL_DESCFAMILIA, cursoraux1->valor ( "descfamilia" ) );
        it->setText ( COL_IDFAMILIA, cursoraux1->valor ( "idfamilia" ) );
        it->setText ( COL_CODCOMPLETOFAMILIA, cursoraux1->valor ( "codigocompletofamilia" ) );
        it->setText ( COL_PRODUCTOFISICOFAMILIA, cursoraux1->valor ( "productofisicofamilia" ) );
        m_listFamilias->expandItem ( it );
        cursoraux1->siguienteregistro();
    } // end while
    delete cursoraux1;
    cursoraux2 = empresaBase()->cargacursor ( "SELECT * FROM familia WHERE padrefamilia IS NOT NULL ORDER BY idfamilia" );
    while ( !cursoraux2->eof() ) {
        padre = cursoraux2->valor ( "padrefamilia" ).toInt();
        idfamilia = cursoraux2->valor ( "idfamilia" ).toInt();
        it = new QTreeWidgetItem ( Lista1[padre] );
        Lista1[idfamilia] = it;
        it->setText ( COL_NOMFAMILIA, cursoraux2->valor ( "nombrefamilia" ) );
        it->setText ( COL_CODFAMILIA, cursoraux2->valor ( "codigofamilia" ) );
        it->setText ( COL_DESCFAMILIA, cursoraux2->valor ( "descfamilia" ) );
        it->setText ( COL_IDFAMILIA, cursoraux2->valor ( "idfamilia" ) );
        it->setText ( COL_CODCOMPLETOFAMILIA, cursoraux2->valor ( "codigocompletofamilia" ) );
        it->setText ( COL_PRODUCTOFISICOFAMILIA, cursoraux2->valor ( "productofisicofamilia" ) );
        m_listFamilias->expandItem ( it );
        cursoraux2->siguienteregistro();
    } // end while
    delete cursoraux2;
    m_idfamilia = "";
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    m_semaforoPintar = FALSE; /// Desactivamos el semaforo de pintado.
    _depura ( "END FamiliasView::pintar", 0 );
}


///
/**
\return
**/
QString FamiliasView::codigoCompletoFamilia()
{
    _depura ( "FamiliasView::codigoCompletoFamilia", 0 );
    QTreeWidgetItem *it = m_listFamilias->currentItem();
    if ( it ) {
        return it->text ( COL_CODCOMPLETOFAMILIA );
    } // end if
    _depura ( "END FamiliasView::codigoCompletoFamilia", 0 );
    return "";
}


///
/**
\return
**/
QString FamiliasView::idFamilia()
{
    _depura ( "FamiliasView::idFamilia", 0 );
    QTreeWidgetItem *it = m_listFamilias->currentItem();
    if ( it ) {
        return it->text ( COL_IDFAMILIA );
    } // end if
    _depura ( "END FamiliasView::idFamilia", 0 );
    return "";
}


///
/**
\return
**/
QString FamiliasView::nombreFamilia()
{
    _depura ( "FamiliasView::nombreFamilia", 0 );
    QTreeWidgetItem *it = m_listFamilias->currentItem();
    if ( it ) {
        return it->text ( COL_NOMFAMILIA );
    } // end if
    _depura ( "END FamiliasView::nombreFamilia", 0 );
    return "";
}


/// Se ha seleccionado un item en la lista.
/// Lo que hacemos es mostar el elemento.
/// Si el anterior ha sido modificado pedimos para actuar en consecuencia.
/**
\param it
**/
void FamiliasView::on_m_listFamilias_itemDoubleClicked ( QTreeWidgetItem *it )
{
    _depura ( "FamiliasView::on_m_listFamilias_itemDoubleClicked", 0 );
    if ( m_modoConsulta ) {
        m_idfamilia = it->text ( COL_IDFAMILIA );
        emit selected ( m_idfamilia );
    } // end if
    _depura ( "END FamiliasView::on_m_listFamilias_itemDoubleClicked", 0 );
}


/// Se ha seleccionado un item en la lista.
/// Lo que hacemos es mostar el elemento.
/// Si el anterior ha sido modificado pedimos para actuar en consecuencia.
/**
\param current
\param previos
\return
**/
void FamiliasView::on_m_listFamilias_currentItemChanged ( QTreeWidgetItem *current, QTreeWidgetItem *previous )
{
    _depura ( "FamiliasView::on_m_listFamilias_currentItemChanged", 0 );

    /// Si estamos dentro del proceso de pintado salimos sin hacer nada ya que puede haber problemas.
    if ( m_semaforoPintar ) return;

    QString idfamiliaold = "";
    if ( previous ) {
        m_idfamilia = previous->text ( COL_IDFAMILIA );
    } // end if
    if ( m_idfamilia != "" ) {
        trataModificado();
        pintar ( previous );
    } // end if
    m_idfamilia = current->text ( COL_IDFAMILIA );
    mostrarplantilla();
    _depura ( "END FamiliasView::on_m_listFamilias_currentItemChanged", 0 );
}


///
/**
**/
void FamiliasView::mostrarplantilla()
{
    _depura ( "FamiliasView::mostrarplantilla", 0 );
    QString query;
    query = "SELECT * from familia WHERE idfamilia = " + m_idfamilia;
    cursor2 *cursorfamilia = empresaBase()->cargacursor ( query );
    if ( !cursorfamilia->eof() ) {
        mui_nomFamilia->setText ( cursorfamilia->valor ( "nombrefamilia" ) );
        mui_descFamilia->setPlainText ( cursorfamilia->valor ( "descfamilia" ) );
        mui_codCompletoFamilia->setText ( cursorfamilia->valor ( "codigocompletofamilia" ) );
        mui_codFamilia->setText ( cursorfamilia->valor ( "codigofamilia" ) );

        if ( cursorfamilia->valor ( "productofisicofamilia" ) == "t" ) {
            mui_productofamilia->setChecked ( TRUE );
        } else {
            mui_serviciofamilia->setChecked ( TRUE );
        } // end if
    } // end if
    delete cursorfamilia;
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    _depura ( "END FamiliasView::mostrarplantilla", 0 );
}


///
/**
\return
**/
bool FamiliasView::trataModificado()
{
    /// Si se ha modificado el contenido advertimos y guardamos.
    if ( dialogChanges_hayCambios() ) {
        if ( QMessageBox::warning ( this,
                                    tr ( "Guardar familia" ),
                                    tr ( "Desea guardar los cambios?" ),
                                    QMessageBox::Ok,
                                    QMessageBox::Cancel ) == QMessageBox::Ok ) {
            on_mui_guardar_clicked();
        } // end if
        return ( TRUE );
    } // end if
    return ( FALSE );
}


/// SLOT que responde a la pulsacion del boton de guardar el tipo de IVA que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
/**
\return
**/
int FamiliasView::guardar()
{
    _depura ( "FamiliasView::guardar", 0 );
    QString prodfam;
    try {
        if ( m_idfamilia == "" ) {
            mensajeInfo ( tr ( "Debe seleccionar una familia" ) );
            return -1;
        } // end if
        if ( mui_productofamilia->isChecked() ) {
            prodfam = " TRUE ";
        } else {
            prodfam = " FALSE ";
        } // end if
        QString query = "UPDATE familia SET nombrefamilia = '" +
                        empresaBase()->sanearCadena ( mui_nomFamilia->text() ) + "', descfamilia = '" +
                        empresaBase()->sanearCadena ( mui_descFamilia->toPlainText() ) + "' , codigofamilia = '" +
                        empresaBase()->sanearCadena ( mui_codFamilia->text() ) + "', productofisicofamilia= " + prodfam + " WHERE idfamilia =" + m_idfamilia;
        int error = empresaBase()->ejecuta ( query );
        if ( error ) {
            throw - 1;
        } // end if
        /// Guardamos la informacion de la fila que esta seleccionada para volver
        /// a ponerla despues.
        QTreeWidgetItem *posicionCursor;
        posicionCursor = m_listFamilias->currentItem();
        posicionCursor->setSelected ( TRUE );
        /// Pintamos los datos en el listado.
        pintar ( posicionCursor );
        dialogChanges_cargaInicial();
        _depura ( "END FamiliasView::guardar", 0 );
        return 0;
    } catch ( ... ) {
        mensajeInfo ( "Error al guardar la familia" );
        return -1;
    } // end try
}


/// Pinta una fila especifica del listado de familias presentado.
/**
\param it
**/
void FamiliasView::pintar ( QTreeWidgetItem *it )
{
    QString idfamilia = it->text ( COL_IDFAMILIA );
    if ( it ) {
        cursor2 * cursoraux1 = empresaBase()->cargacursor ( "SELECT * FROM familia WHERE idfamilia = " + idfamilia );
        if ( !cursoraux1->eof() ) {
            it->setText ( COL_NOMFAMILIA, cursoraux1->valor ( "nombrefamilia" ) );
            it->setText ( COL_CODFAMILIA, cursoraux1->valor ( "codigofamilia" ) );
            it->setText ( COL_DESCFAMILIA, cursoraux1->valor ( "descfamilia" ) );
            it->setText ( COL_IDFAMILIA, cursoraux1->valor ( "idfamilia" ) );
            it->setText ( COL_CODCOMPLETOFAMILIA, cursoraux1->valor ( "codigocompletofamilia" ) );
            it->setText ( COL_PRODUCTOFISICOFAMILIA, cursoraux1->valor ( "productofisicofamilia" ) );
        } // end if
        delete cursoraux1;
    } // end if
}


/// SLOT que responde a la pulsacion del boton de nuevo
/// Inserta en la tabla de Familias
/**
**/
void FamiliasView::on_mui_crear_clicked()
{
    _depura ( "FamiliasView::on_mui_crear_clicked", 0 );
    try {
        empresaBase()->begin();
        /// Si se ha modificado el contenido advertimos y guardamos.
        trataModificado();
        QString padrefamilia;
        if ( m_idfamilia != "" ) {
            padrefamilia = m_idfamilia;
        } else {
            padrefamilia = "NULL";
        } // end if

        QString query = "INSERT INTO familia (nombrefamilia, descfamilia, padrefamilia, codigofamilia) VALUES ('NUEVA FAMILIA', 'Descripcion de la familia', " + padrefamilia + ", 'XXX')";

        int error = empresaBase()->ejecuta ( query );
        if ( error ) {
            throw - 1;
        } // end if
        cursor2 *cur = empresaBase()->cargacursor ( "SELECT max(idfamilia) AS idfamilia FROM familia" );
        empresaBase()->commit();
        m_idfamilia = cur->valor ( "idfamilia" );
        delete cur;
        pintar();
        _depura ( "END FamiliasView::on_mui_crear_clicked", 0 );
    } catch ( ... ) {
        empresaBase()->rollback();
        mensajeInfo ( "Error al crear la familia" );
    } // end try
}


/// SLOT que responde a la pulsacion del boton de nuevo desde Raiz
/// Inserta en la tabla de Familias
/**
**/
void FamiliasView::on_mui_crearRaiz_clicked()
{
    _depura ( "FamiliasView::on_mui_crearRaiz_clicked", 0 );
    try {
        empresaBase()->begin();
        /// Si se ha modificado el contenido advertimos y guardamos.
        trataModificado();

        QString query = "INSERT INTO familia (nombrefamilia, descfamilia, padrefamilia, codigofamilia) VALUES ('NUEVA FAMILIA', 'Descripcion de la familia',  NULL , 'XXX')";

        int error = empresaBase()->ejecuta ( query );
        if ( error ) {
            throw - 1;
        } // end if
        cursor2 *cur = empresaBase()->cargacursor ( "SELECT max(idfamilia) AS idfamilia FROM familia" );
        empresaBase()->commit();
        m_idfamilia = cur->valor ( "idfamilia" );
        delete cur;
        pintar();
        _depura ( "END FamiliasView::on_mui_crearRaiz_clicked", 0 );
    } catch ( ... ) {
        empresaBase()->rollback();
        mensajeInfo ( "Error al crear la familia" );
    } // end try
}




///
/**
**/
void FamiliasView::on_mui_borrar_clicked()
{
    _depura ( "FamiliasView::on_mui_borrar_clicked", 0 );

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
    _depura ( "END FamiliasView::on_mui_borrar_clicked", 0 );
}


/// SLOT que responde a la pulsacion del botón de borrar la familia que se está editando.
/// Lo que hace es que se hace un update de todos los campos.
/**
\return
**/
int FamiliasView::borrar()
{
    _depura ( "FamiliasView::borrar", 0 );
    if ( m_idfamilia == "" ) {
        mensajeInfo ( tr ( "Debe seleccionar una familia" ) );
        return -1;
    } // end if
    try {
        QString query = "DELETE FROM FAMILIA WHERE idfamilia = " + m_idfamilia;
        int error = empresaBase()->ejecuta ( query );
        if ( error ) {
            throw - 1;
        } // end if
        m_idfamilia = "";
        dialogChanges_cargaInicial();
        pintar();
        _depura ( "END FamiliasView::borrar", 0 );
    } catch ( ... ) {
        mensajeInfo ( "Error al borrar la familia" );
        return -1;
    } // end try
    return 0;
}


///
/**
**/
void FamiliasView::on_mui_imprimir_clicked()
{
    _depura ( "FamiliasView::on_mui_imprimir_clicked", 0 );

    QString archivo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "familias.rml";
    QString archivod = confpr->valor ( CONF_DIR_USER ) + "familias.rml";
    QString archivologo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system ( archivo.toAscii().constData() );
    /// Copiamos el logo.
#ifdef WINDOWS

    archivologo = "copy "  + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
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
    fitxersortidatxt += "        <td>" + tr ( "Codigo" ) + "</td>";
    fitxersortidatxt += "        <td>" + tr ( "Nombre" ) + "</td>";
    fitxersortidatxt += "</tr>";

    cursor2 *cur = empresaBase()->cargacursor ( "SELECT * FROM familia ORDER BY codigocompletofamilia" );
    while ( !cur->eof() ) {
        fitxersortidatxt += "<tr>";
        fitxersortidatxt += "        <td>" + cur->valor ( "codigocompletofamilia" ) + "</td>";
        fitxersortidatxt += "        <td>" + cur->valor ( "nombrefamilia" ) + "</td>";
        fitxersortidatxt += "</tr>";
        cur->siguienteregistro();
    } // end if
    delete cur;
    fitxersortidatxt += "</blockTable>";

    buff.replace ( "[story]", fitxersortidatxt );

    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << buff;
        file.close();
    } // end if
    invocaPDF ( "familias" );
    _depura ( "END FamiliasView::on_mui_imprimir_clicked", 0 );
}


///
/**
**/
void FamiliasView::on_mui_aceptar_clicked()
{
    _depura ( "FamiliasView::on_mui_aceptar_clicked", 0 );
    QTreeWidgetItem *it = m_listFamilias->currentItem();
    if ( it ) {
        m_idfamilia = it->text ( COL_IDFAMILIA );
    } else {
        m_idfamilia = "";
    } // end if
    FichaBf::on_mui_aceptar_clicked();
    _depura ( "END FamiliasView::on_mui_aceptar_clicked", 0 );
}


///
/**
**/
void FamiliasView::setModoConsulta()
{
    m_modoConsulta = TRUE;
}


///
/**
**/
void FamiliasView::setModoEdicion()
{
    m_modoConsulta = FALSE;
}




