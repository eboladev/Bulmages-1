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
#include "blfile.h"
#include <QtCore/QTextStream>

#include "familiasview.h"
#include "bfcompany.h"

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
\param selectMode
**/
FamiliasView::FamiliasView ( BfCompany *comp, QWidget *parent, bool selectMode )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    m_listFamilias->setColumnCount ( 3 );
    QStringList headers;
    headers << _ ( "Nombre" ) << _ ( "Codigo" ) << _ ( "Descripcion" ) << _ ( "Id familia" ) << _ ( "Codigo completo" );
    m_listFamilias->setHeaderLabels ( headers );

    m_listFamilias->setColumnWidth ( 0, 200 );
    m_listFamilias->setColumnWidth ( 1, 100 );

    m_listFamilias->setColumnHidden ( COL_IDFAMILIA, true );
    m_listFamilias->setColumnHidden ( COL_CODFAMILIA, true );
    m_listFamilias->setColumnHidden ( COL_PRODUCTOFISICOFAMILIA, true );

    m_semaforoPintar = false;

    m_idfamilia = "";

    mui_nomFamilia->setEnabled ( false );
    mui_descFamilia->setEnabled ( false );
    mui_codCompletoFamilia->setEnabled ( false );
    mui_codFamilia->setEnabled ( false );
    
    /// Disparamos los plugins.
    int res = g_plugins->run ( "FamiliasView_FamiliasView", this );
    if ( res != 0 ) {
	return;
    } // end if

    if ( selectMode ) {
        setSelectMode();
        mui_tab->hide();
        mui_borrar->hide();
        mui_cancelar->hide();
        mui_aceptar->hide();
        mui_imprimir->hide();
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
FamiliasView::~FamiliasView()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void FamiliasView::pintar()
{
    BL_FUNC_DEBUG
    /// Activamos el semaforo de pintado para que no haya slots concurrentes.
    m_semaforoPintar = true;
    QTreeWidgetItem *it;
    QMap <int, QTreeWidgetItem*> Lista1;
    int padre;
    int idfamilia = 0;
    BlDbRecordSet *cursoraux1, *cursoraux2;

    /// Vaciamos el arbol.
    while ( m_listFamilias->topLevelItemCount() > 0 ) {
        it = m_listFamilias->takeTopLevelItem ( 0 );
        delete it;
    } // end while

    /// Parece que lo mas rentable es crear una inicial que siempre este y a partir de la que carguen las demas
    m_init = new QTreeWidgetItem ( m_listFamilias );
    m_init->setText ( COL_NOMFAMILIA, _("Familias") );
    m_listFamilias->expandItem ( m_init );



    cursoraux1 = mainCompany()->loadQuery ( "SELECT * FROM familia WHERE padrefamilia IS NULL ORDER BY idfamilia" );
    while ( !cursoraux1->eof() ) {
        padre = cursoraux1->value( "padrefamilia" ).toInt();
        idfamilia = cursoraux1->value( "idfamilia" ).toInt();
        it = new QTreeWidgetItem ( m_init );
        Lista1[idfamilia] = it;
        it->setText ( COL_NOMFAMILIA, cursoraux1->value( "nombrefamilia" ) );
        it->setText ( COL_CODFAMILIA, cursoraux1->value( "codigofamilia" ) );
        it->setText ( COL_DESCFAMILIA, cursoraux1->value( "descfamilia" ) );
        it->setText ( COL_IDFAMILIA, cursoraux1->value( "idfamilia" ) );
        it->setText ( COL_CODCOMPLETOFAMILIA, cursoraux1->value( "codigocompletofamilia" ) );
        it->setText ( COL_PRODUCTOFISICOFAMILIA, cursoraux1->value( "productofisicofamilia" ) );
        m_listFamilias->expandItem ( it );
        cursoraux1->nextRecord();
    } // end while
    delete cursoraux1;
    cursoraux2 = mainCompany()->loadQuery ( "SELECT * FROM familia WHERE padrefamilia IS NOT NULL ORDER BY idfamilia" );
    while ( !cursoraux2->eof() ) {
        padre = cursoraux2->value( "padrefamilia" ).toInt();
        idfamilia = cursoraux2->value( "idfamilia" ).toInt();
        it = new QTreeWidgetItem ( Lista1[padre] );
        Lista1[idfamilia] = it;
        it->setText ( COL_NOMFAMILIA, cursoraux2->value( "nombrefamilia" ) );
        it->setText ( COL_CODFAMILIA, cursoraux2->value( "codigofamilia" ) );
        it->setText ( COL_DESCFAMILIA, cursoraux2->value( "descfamilia" ) );
        it->setText ( COL_IDFAMILIA, cursoraux2->value( "idfamilia" ) );
        it->setText ( COL_CODCOMPLETOFAMILIA, cursoraux2->value( "codigocompletofamilia" ) );
        it->setText ( COL_PRODUCTOFISICOFAMILIA, cursoraux2->value( "productofisicofamilia" ) );
        m_listFamilias->expandItem ( it );
        cursoraux2->nextRecord();
    } // end while
    delete cursoraux2;
    m_idfamilia = "";
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_readValues();
    m_semaforoPintar = false; /// Desactivamos el semaforo de pintado.
    
}


///
/**
\return
**/
QString FamiliasView::codigoCompletoFamilia()
{
    BL_FUNC_DEBUG
    QTreeWidgetItem *it = m_listFamilias->currentItem();
    if ( it ) {
        return it->text ( COL_CODCOMPLETOFAMILIA );
    } // end if
    
    return "";
}


///
/**
\return
**/
QString FamiliasView::idFamilia()
{
    BL_FUNC_DEBUG
    QTreeWidgetItem *it = m_listFamilias->currentItem();
    if ( it ) {
        return it->text ( COL_IDFAMILIA );
    } // end if
    
    return "";
}


///
/**
\return
**/
QString FamiliasView::idFamiliaModified()
{
    return m_idfamilia;
}


///
/**
\return
**/
QString FamiliasView::nombreFamilia()
{
    BL_FUNC_DEBUG
    QTreeWidgetItem *it = m_listFamilias->currentItem();
    if ( it ) {
        return it->text ( COL_NOMFAMILIA );
    } // end if
    
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
    BL_FUNC_DEBUG
    if ( m_selectMode ) {
        m_idfamilia = it->text ( COL_IDFAMILIA );
        emit selected ( m_idfamilia );
    } // end if
    
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
    BL_FUNC_DEBUG

    if ( m_semaforoPintar ) return;
    m_semaforoPintar = true;

    /// Si estamos dentro del proceso de pintado salimos sin hacer nada ya que puede haber problemas.    
    if ( previous ) {
        m_idfamilia = previous->text ( COL_IDFAMILIA );
    } // end if

    if ( !m_idfamilia.isEmpty() ) {
        trataModificado();
        if ( previous )
            pintar ( previous );

    } // end if

    /// Disparamos los plugins.
    int res = g_plugins->run ( "FamiliasView_currentItemChanged_Post", this );
    if ( res != 0 ) {
	return;
    } // end if

    if ( current ) {
        m_idfamilia = current->text ( COL_IDFAMILIA );
    } else {
        m_idfamilia = "";
    } // end if

    mostrarplantilla();
    m_semaforoPintar = false;

    
}


///
/**
**/
void FamiliasView::mostrarplantilla()
{
    BL_FUNC_DEBUG
    QString query;
    if ( !m_idfamilia.isEmpty() ) {
        mui_nomFamilia->setEnabled ( true );
        mui_descFamilia->setEnabled ( true );
        mui_codCompletoFamilia->setEnabled ( true );
        mui_codFamilia->setEnabled ( true );

        query = "SELECT * from familia WHERE idfamilia = " + m_idfamilia;
        BlDbRecordSet *cursorfamilia = mainCompany()->loadQuery ( query );
        if ( !cursorfamilia->eof() ) {
            mui_nomFamilia->setText ( cursorfamilia->value( "nombrefamilia" ) );
            mui_descFamilia->setPlainText ( cursorfamilia->value( "descfamilia" ) );
            mui_codCompletoFamilia->setText ( cursorfamilia->value( "codigocompletofamilia" ) );
            mui_codFamilia->setText ( cursorfamilia->value( "codigofamilia" ) );

            if ( cursorfamilia->value( "productofisicofamilia" ) == "t" ) {
                mui_productofamilia->setChecked ( true );
            } else {
                mui_serviciofamilia->setChecked ( true );
            } // end if
        } // end if
        delete cursorfamilia;
    } else {
        mui_nomFamilia->setEnabled ( false );
        mui_descFamilia->setEnabled ( false );
        mui_codCompletoFamilia->setEnabled ( false );
        mui_codFamilia->setEnabled ( false );
        mui_nomFamilia->setText ( "" );
        mui_descFamilia->setPlainText ( "" );
        mui_codCompletoFamilia->setText ( "" );
        mui_codFamilia->setText ( "" );
    } // end if
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_readValues();
    
}


///
/**
\return
**/
bool FamiliasView::trataModificado()
{
    BL_FUNC_DEBUG
    /// Si se ha modificado el contenido advertimos y guardamos.
    if ( dialogChanges_isChanged() ) {
        if ( QMessageBox::warning ( this,
                                    _ ( "Guardar familia" ),
                                    _ ( "Desea guardar los cambios?" ),
                                    QMessageBox::Ok,
                                    QMessageBox::Cancel ) == QMessageBox::Ok ) {
            on_mui_guardar_clicked();
        } // end if
    } // end if
    
    return ( false );
}


/// SLOT que responde a la pulsacion del boton de guardar el tipo de IVA que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
/**
\return
**/
int FamiliasView::save()
{
    BL_FUNC_DEBUG
    QString prodfam;
    try {
        if ( m_idfamilia.isEmpty() ) {
            blMsgInfo ( _ ( "Debe seleccionar una familia" ) );
            return -1;
        } // end if

	mainCompany()->begin();

        /// Disparamos los plugins.
        int res1 = g_plugins->run ( "FamiliasView_Guardar_Pre", this );
	if ( res1 != 0 ) {
	    throw -1;
	} // end if

        if ( mui_productofamilia->isChecked() ) {
            prodfam = " TRUE ";
        } else {
            prodfam = " FALSE ";
        } // end if
        QString query = "UPDATE familia SET nombrefamilia = '" +
                        mainCompany()->sanearCadena ( mui_nomFamilia->text() ) + "', descfamilia = '" +
                        mainCompany()->sanearCadena ( mui_descFamilia->toPlainText() ) + "' , codigofamilia = '" +
                        mainCompany()->sanearCadena ( mui_codFamilia->text() ) + "', productofisicofamilia= " + prodfam + " WHERE idfamilia =" + m_idfamilia;

        int error = mainCompany()->runQuery ( query );
        if ( error ) {
            throw -1;
        } // end if

        /// Disparamos los plugins.
        int res2 = g_plugins->run ( "FamiliasView_Guardar_Post", this );
	if ( res2 != 0 ) {
	    throw -1;
	} // end if

        /// Guardamos la informacion de la fila que esta seleccionada para volver
        /// a ponerla despues.
        QTreeWidgetItem *posicionCursor;
        posicionCursor = m_listFamilias->currentItem();
        if ( posicionCursor ) {
            posicionCursor->setSelected ( true );
            /// Pintamos los datos en el listado.
            pintar ( posicionCursor );
        } // end if
        dialogChanges_readValues();
        

	mainCompany()->commit();
        return 0;
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al guardar la familia" ) );
	mainCompany()->rollback();
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
        BlDbRecordSet * cursoraux1 = mainCompany()->loadQuery ( "SELECT * FROM familia WHERE idfamilia = " + idfamilia );
        if ( !cursoraux1->eof() ) {
            it->setText ( COL_NOMFAMILIA, cursoraux1->value( "nombrefamilia" ) );
            it->setText ( COL_CODFAMILIA, cursoraux1->value( "codigofamilia" ) );
            it->setText ( COL_DESCFAMILIA, cursoraux1->value( "descfamilia" ) );
            it->setText ( COL_IDFAMILIA, cursoraux1->value( "idfamilia" ) );
            it->setText ( COL_CODCOMPLETOFAMILIA, cursoraux1->value( "codigocompletofamilia" ) );
            it->setText ( COL_PRODUCTOFISICOFAMILIA, cursoraux1->value( "productofisicofamilia" ) );
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
    BL_FUNC_DEBUG
    try {
        mainCompany()->begin();
        /// Si se ha modificado el contenido advertimos y guardamos.
        trataModificado();
        QString padrefamilia;
        if ( m_idfamilia != "" ) {
            padrefamilia = m_idfamilia;
        } else {
            padrefamilia = "NULL";
        } // end if

        QString query = "INSERT INTO familia (nombrefamilia, descfamilia, padrefamilia, codigofamilia) VALUES ('NUEVA FAMILIA', 'Descripcion de la familia', " + padrefamilia + ", 'XXX')";

        int error = mainCompany()->runQuery ( query );
        if ( error ) {
            throw - 1;
        } // end if
        BlDbRecordSet *cur = mainCompany()->loadQuery ( "SELECT max(idfamilia) AS idfamilia FROM familia" );
        mainCompany()->commit();
        m_idfamilia = cur->value( "idfamilia" );
        delete cur;
        pintar();
        
    } catch ( ... ) {
        mainCompany()->rollback();
        blMsgInfo ( _ ( "Error al crear la familia" ) );
    } // end try
}


///
/**
**/
void FamiliasView::on_mui_borrar_clicked()
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
int FamiliasView::remove()
{
    BL_FUNC_DEBUG
    if ( m_idfamilia == "" ) {
        blMsgInfo ( _ ( "Debe seleccionar una familia" ) );
        return -1;
    } // end if
    try {
        QString query = "DELETE FROM FAMILIA WHERE idfamilia = " + m_idfamilia;
        int error = mainCompany()->runQuery ( query );
        if ( error ) {
            throw - 1;
        } // end if
        m_idfamilia = "";
        dialogChanges_readValues();
        pintar();
        
    } catch ( ... ) {
	
        blMsgInfo ( _ ( "Error al borrar la familia" ) );
        return -1;
    } // end try
    return 0;
}


///
/**
**/
void FamiliasView::on_mui_imprimir_clicked()
{
    BL_FUNC_DEBUG


    /// Copiamos el archivo.
    QString archivo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "familias.rml";
    QString archivod = g_confpr->value( CONF_DIR_USER ) + "familias.rml";
    blCopyFile(archivo, archivod);

    /// Copiamos el logo.
    QString archivologo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "logo.jpg";
    QString logousuario = g_confpr->value( CONF_DIR_USER ) + "logo.jpg";
    blCopyFile(archivologo, logousuario);
    
    BlFile file;
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

    BlDbRecordSet *cur = mainCompany()->loadQuery ( "SELECT * FROM familia ORDER BY codigocompletofamilia" );
    while ( !cur->eof() ) {
        fitxersortidatxt += "<tr>";
        fitxersortidatxt += "        <td>" + cur->value( "codigocompletofamilia" ) + "</td>";
        fitxersortidatxt += "        <td>" + cur->value( "nombrefamilia" ) + "</td>";
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
    blCreateAndLoadPDF ( "familias" );
    
}


///
/**
**/
void FamiliasView::on_mui_aceptar_clicked()
{
    BL_FUNC_DEBUG
    QTreeWidgetItem *it = m_listFamilias->currentItem();
    if ( it ) {
        m_idfamilia = it->text ( COL_IDFAMILIA );
    } else {
        m_idfamilia = "";
    } // end if
    BfForm::on_mui_aceptar_clicked();
    
}


///
/**
**/
void FamiliasView::setSelectMode()
{
    m_selectMode = true;
}


///
/**
**/
void FamiliasView::setEditMode()
{
    m_selectMode = false;
}




