/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
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

#include <stdlib.h>

#include <QMap>
#include <QPixmap>
#include <QKeyEvent>
#include <QFileDialog>
#include <QTextStream>

#include "bcplancontablelistview.h"
#include "blfunctions.h"
#include "bccuentaview.h"
#include "bfcompany.h"
#include "bcimages.h"

///
/**
\param emp
\param parent
\param flag
\param editmode
**/
BcPlanContableListView::BcPlanContableListView ( BfCompany *emp, QWidget *parent, Qt::WFlags flag, edmode editmode )
        : BfForm ( emp, parent, flag ), BlImportExport ( emp )
{
    BL_FUNC_DEBUG
    setupUi ( this );

    setAttribute(Qt::WA_DeleteOnClose);
    setTitleName ( _ ( "Cuenta" ) );
    /// Establezco cual es la tabla en la que basarse para el sistema de permisos
    setDbTableName ( "cuenta" );

    m_modo = editmode;
    /// Para el listado de columnas hacemos una inicializaci&oacute;n.
    QStringList headers;
    headers << _ ( "Codigo cuenta" ) << _ ( "Nombre cuenta" ) << _ ( "Debe" ) << _ ( "Haber" ) << _ ( "ID cuenta" ) << _ ( "Bloqueada" ) << _ ( "Nodebe" ) << _ ( "Nohaber" ) << _ ( "Regularizacion" ) << _ ( "Imputacion" ) << _ ( "Grupo" ) << _ ( "Tipo cuenta" );
    mui_arbolcuentas->setHeaderLabels ( headers );
    ccuenta = 0;
    cdesccuenta = 1;
    cdebe = 2;
    chaber = 3;
    cidcuenta = 4;
    cbloqueada = 5;
    cnodebe = 6;
    cnohaber = 7;
    cregularizacion = 8;
    cimputacion = 9;
    cgrupo = 10;
    ctipocuenta = 11;

    mui_arbolcuentas->setColumnWidth ( ccuenta, 175 );
    mui_arbolcuentas->setColumnWidth ( cdesccuenta, 400 );
    mui_arbolcuentas->hideColumn ( cidcuenta );
    mui_arbolcuentas->hideColumn ( cnodebe );
    mui_arbolcuentas->hideColumn ( cnohaber );
    mui_arbolcuentas->hideColumn ( cregularizacion );
    mui_arbolcuentas->hideColumn ( cimputacion );
    mui_arbolcuentas->hideColumn ( cgrupo );
    mui_arbolcuentas->hideColumn ( ctipocuenta );



    mui_tablacuentas->setColumnCount ( 3 );
    headers << _ ( "CODIGO" ) << _ ( "NOMBRE" );
    mui_tablacuentas->setHorizontalHeaderLabels ( headers );

    mui_tablacuentas->hideColumn ( 2 );
    mui_tablacuentas->setColumnWidth ( 1, 400 );
    mui_tablacuentas->setColumnWidth ( 0, 100 );

    installEventFilter ( this );
    if ( m_modo == BL_EDIT_MODE )
        mainCompany() ->insertWindow ( windowTitle(), this );

    /// De forma predeterminada los botones de busqueda 'anterior' y 'siguiente' estan deshabilitados.
    mui_anterior->setDisabled(true);
    mui_siguiente->setDisabled(true);
    /// Llamamos a los scripts
    blScript(this);

    
}


///
/**
**/
BcPlanContableListView::~BcPlanContableListView()
{
    BL_FUNC_DEBUG
    
    if ( m_modo == BL_EDIT_MODE ) {
        mainCompany() ->removeWindow ( this );
    } // end if
}


///
/**
\return
**/
bool BcPlanContableListView::eventFilter ( QObject *obj, QEvent *event )
{
    BL_FUNC_DEBUG
    
    return QWidget::eventFilter ( obj, event );
}


/// Se encarga de hacer las inicializaciones de todo el formulario.
/** Se llama as&iacute; y no desde el constructor porque as&iacute; la podemos llamar
    desde dentro de la misma clase, etc, etc, etc. */
/**
\return
**/
int BcPlanContableListView::inicializa()
{
    BL_FUNC_DEBUG
    try {
        QTreeWidgetItem *it;
        QMap <int, QTreeWidgetItem*> Lista1;
        int idcuenta;
        int padre;
        mui_arbolcuentas->clear();

        /// Cargamos y pintamos las cuentas hijas.
        BlDbRecordSet *ctas = mainCompany() ->loadQuery ( "SELECT * FROM cuenta ORDER BY codigo" );
        while ( !ctas->eof() ) {

            idcuenta = ctas->value( "idcuenta" ).toInt();
            padre = ctas->value( "padre" ).toInt();

            if ( padre == 0 ) {

                it = new QTreeWidgetItem ( mui_arbolcuentas );

                Lista1[idcuenta] = it;
            } else {
                it = new QTreeWidgetItem ( Lista1[padre] );
                Lista1[idcuenta] = it;
            } // end if


            it->setText ( ccuenta, ctas->value( "codigo" ) );
            it->setText ( cdesccuenta, ctas->value( "descripcion" ) );
            it->setText ( cidcuenta, ctas->value( "idcuenta" ) );
            it->setText ( cbloqueada, ctas->value( "bloqueada" ) );
            it->setText ( cnodebe, ctas->value( "nodebe" ) );
            it->setText ( cnohaber, ctas->value( "nohaber" ) );
            it->setText ( cregularizacion, ctas->value( "regularizacion" ) );
            it->setText ( cimputacion, ctas->value( "imputacion" ) );
            it->setText ( cdebe, ctas->value( "debe" ) );
            it->setText ( chaber, ctas->value( "haber" ) );

	    it->setTextAlignment(2, Qt::AlignRight);
	    it->setTextAlignment(3, Qt::AlignRight);

            /// Ponemos los iconos.
            if ( ctas->value( "tipocuenta" ) == "1" )
                it->setIcon ( ccuenta, QPixmap ( cactivo ) );
            else if ( ctas->value( "tipocuenta" ) == "2" )
                it->setIcon ( ccuenta, QPixmap ( cpasivo ) );
            else if ( ctas->value( "tipocuenta" ) == "3" )
                it->setIcon ( ccuenta, QPixmap ( cneto ) );
            else if ( ctas->value( "tipocuenta" ) == "4" )
                it->setIcon ( ccuenta, QPixmap ( cingresos ) );
            else if ( ctas->value( "tipocuenta" ) == "5" )
                it->setIcon ( ccuenta, QPixmap ( cgastos ) );

            ctas->nextRecord();
        } // end while
        mui_arbolcuentas->expandAll();
        delete ctas;

        /// Cargamos el n&uacute;mero de d&iacute;gitos de cuenta para poder hacer una
        /// introducci&oacute;n de n&uacute;meros de cuenta m&aacute;s pr&aacute;ctica.

	numDigitos = g_confpr->value(CONF_CONT_NUMDIGITOSEMPRESA).toInt();

        inicializaTabla();
    } catch ( ... ) {
        blMsgInfo ( "Error en la carga" );
        
        return -1;
    } // end try
    
    return 0;
}


/// Inicializa la tabla de cuentas, que es la segunda pesta&ntilde;a de la pantalla.
/** Dicha tabla s&oacute;lo muestra las cuentas hoja del plan contable, y deja los
    dem&aacute;s elementos ocultos. */
/**
**/
void BcPlanContableListView::inicializaTabla()
{
    BL_FUNC_DEBUG
    QString query;
    query = "SELECT * FROM cuenta ORDER BY codigo";
    BlDbRecordSet *cursoraux1 = mainCompany() ->loadQuery ( query );
    mui_tablacuentas->setRowCount ( cursoraux1->numregistros() );
    int i = 0;
    QTableWidgetItem *dato;
    while ( !cursoraux1->eof() ) {
        dato = new QTableWidgetItem ( cursoraux1->value( "codigo" ) );
        dato->setFlags ( Qt::ItemIsEnabled );
        /// Ponemos los iconos.
        if ( cursoraux1->value( "tipocuenta" ) == "1" )
            dato->setIcon ( QPixmap ( cactivo ) );
        else if ( cursoraux1->value( "tipocuenta" ) == "2" )
            dato->setIcon ( QPixmap ( cpasivo ) );
        else if ( cursoraux1->value( "tipocuenta" ) == "3" )
            dato->setIcon ( QPixmap ( cneto ) );
        else if ( cursoraux1->value( "tipocuenta" ) == "4" )
            dato->setIcon ( QPixmap ( cingresos ) );
        else if ( cursoraux1->value( "tipocuenta" ) == "5" )
            dato->setIcon ( QPixmap ( cgastos ) );
        mui_tablacuentas->setItem ( i, 0, dato );
        dato = new QTableWidgetItem ( cursoraux1->value( "descripcion" ) );
        dato->setFlags ( Qt::ItemIsEnabled );
        mui_tablacuentas->setItem ( i, 1, dato );
        dato->setFlags ( Qt::ItemIsEnabled );
        dato = new QTableWidgetItem ( cursoraux1->value( "idcuenta" ) );
        mui_tablacuentas->setItem ( i, 2, dato );

        QString codigo = cursoraux1->value( "codigo" );
        if ( ( unsigned int ) codigo.length() != numDigitos ) {
            mui_tablacuentas->hideRow ( i );
        } // end if
        cursoraux1->nextRecord();
        i++;
    } // end while
    delete cursoraux1;
    
}


/// Este es el SLOT que se activa al hacer click sobre &eacute;l.
/** ListView del formulario. Lo que hace es actualizar los valores
    de la derecha del formulario con los valores almacenados en el
    item que se acaba de pulsar. */
/**
\param it
**/
void BcPlanContableListView::on_mui_arbolcuentas_itemClicked ( QTreeWidgetItem *it, int )
{
    BL_FUNC_DEBUG
    QString idcuenta = it->text ( cidcuenta );
    QString cad;
    for ( int i = 0; i < mui_tablacuentas->rowCount() - 1; i++ ) {
        cad = mui_tablacuentas->item ( i, 2 ) ->text();
        if ( cad == idcuenta ) {
            mui_tablacuentas->setCurrentCell ( i, 2 );
            mui_tablacuentas->scrollToItem ( mui_tablacuentas->item ( i, 2 ), QAbstractItemView::EnsureVisible );
            break;
        } // end if
    } // end for
    
}


/// La pantalla lleva implicito un buscador de cuentas, cuando cambia el contenido
/** del QLineEdit del buscador se lanza esta funci&oacute;n que hace una b&uacute;squeda
    sobre el &aacute;rbol de cuentas. */
/**
\param string1
**/
void BcPlanContableListView::on_mui_busqueda_textChanged ( const QString &string1 )
{
    BL_FUNC_DEBUG
    QString cod = string1;
    
    if (cod.trimmed().isEmpty()) {

        /// Si la cadena de texto esta vacia entonces elimina el resultado de la busqueda.
	m_itemList.clear();

    } else {
    
        m_itemList = mui_arbolcuentas->findItems ( cod, Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive, ccuenta );
        m_itemListPosition = 0;
    
        if ( m_itemList.count() > 0 ) {
	    mui_arbolcuentas->setCurrentItem ( m_itemList.first() );
        } else {
	    // Si no se encuentra coincidencia en la primera columna 'ccuenta' entonces se busca en la columna de descripcion 'cdesccuenta'.
	    m_itemList = mui_arbolcuentas->findItems ( cod, Qt::MatchContains | Qt::MatchWrap | Qt::MatchRecursive, cdesccuenta );
	    m_itemListPosition = 0;
    
	    if ( m_itemList.count() > 0 ) {
	        mui_arbolcuentas->setCurrentItem ( m_itemList.first() );
	    } // end if

	} // end if

    } // end if

    setSearchButtonsState();

    
}


/// Este es el SLOT que se activa al hacer doble click sobre &eacute;l.
/** ListView del formulario. Lo que hace es abrir la ventana de
    detalle de cuenta para que se puedan modificar los par&aacute;metros
    y una vez terminado refresca el formulario para que aparezcan
    los datos actualizados. */
/**
\param it
**/
void BcPlanContableListView::on_mui_arbolcuentas_itemDoubleClicked ( QTreeWidgetItem *it, int )
{
    BL_FUNC_DEBUG
    on_mui_arbolcuentas_itemClicked ( it, 0 );
    mdb_codcuenta = it->text ( ccuenta );
    mdb_idcuenta = it->text ( cidcuenta );
    mdb_desccuenta = it->text ( cdesccuenta );
    if ( m_modo == BL_EDIT_MODE ) {
        BcCuentaView * nuevae = new BcCuentaView ( mainCompany(), 0 );
        nuevae->load ( idcuenta() );
        inicializa();
        mainCompany() ->pWorkspace() ->addSubWindow ( nuevae );
        nuevae->show();
    } else {
        emit ( selected ( mdb_idcuenta ) );
    } // end if
    
}


/// Esta funci&oacute;n es el SLOT que se activa al pulsar sobre el
/// bot&oacute;n nueva cuenta.
/** Su funci&oacute;n es crear una nueva cuenta desde la ventana del plan de cuentas.
    La inserci&oacute;n de la nueva se hace como cuenta hija de la cuenta actualmente
    seleccionada por lo que se hace que la ventana que se habre tenga el campo
    del padre de la cuenta rellenado. */
/**
\return
**/
void BcPlanContableListView::on_mui_crear_clicked()
{
    BL_FUNC_DEBUG
    QString codigo;

    BcCuentaView *nuevae = new BcCuentaView ( mainCompany(), 0 );

    if ( mui_arbolcuentas->selectedItems().count() > 0 ) {
        QTreeWidgetItem * it;
        it = mui_arbolcuentas->currentItem();
        if ( it ) {
            codigo = it->text ( ccuenta );
            nuevae->nuevacuenta ( codigo );
        } // end if
    } // end if

    mainCompany() ->pWorkspace() ->addSubWindow ( nuevae );
    nuevae->show();
    
}


void BcPlanContableListView::setSearchButtonsState()
{
    // Cuantos elementos de la lista hay?
    int pos = m_itemListPosition;
    int tamLista = m_itemList.size();
    
    if (tamLista <= 1) {
	/// No hay elementos en la lista o solo hay 1.
        mui_anterior->setDisabled(true);
	mui_siguiente->setDisabled(true);
    } else {
    
	if (pos == 0) {
	    /// Estamos al principio de la lista.
            mui_anterior->setDisabled(true);
	    mui_siguiente->setDisabled(false);
	    
	} else if (pos == tamLista - 1) {
	    /// Estamos al final de la lista.
            mui_anterior->setDisabled(false);
	    mui_siguiente->setDisabled(true);
	    
	} else {
	    /// Estamos en medio de la lista.
            mui_anterior->setDisabled(false);
	    mui_siguiente->setDisabled(false);
	    
	} // end if

    } // end if
    
}


void BcPlanContableListView::on_mui_anterior_clicked()
{
    if (m_itemListPosition > 0) {
        m_itemListPosition--;
        mui_arbolcuentas->setCurrentItem ( m_itemList[m_itemListPosition] );
    } // end if

    setSearchButtonsState();
}


void BcPlanContableListView::on_mui_siguiente_clicked()
{

    if (m_itemListPosition < m_itemList.size() - 1) {
        m_itemListPosition++;
        mui_arbolcuentas->setCurrentItem ( m_itemList[m_itemListPosition] );
    } // end if

    setSearchButtonsState();
}


/// Esta funci&oacute;n es el SLOT que se activa al pulsar sobre el
/// boton nueva cuenta.
/** Su funci&oacute;n es crear una nueva cuenta desde la ventana del plan de cuentas
    La inserci&oacute;n de la nueva se hace como cuenta hija de la cuenta actualmente
    seleccionada por lo que se hace que la ventana que se habre tenga el campo
    del padre de la cuenta rellenado. */
/**
\return
**/
void BcPlanContableListView::on_mui_editar_clicked()
{
    BL_FUNC_DEBUG
    QTreeWidgetItem *it;
    it = mui_arbolcuentas->currentItem();
    if ( !it ) {
        blMsgInfo ( _ ( "Debe seleccionar una cuenta" ) );
        
        return;
    }
    on_mui_arbolcuentas_itemClicked ( it, 0 );
    mdb_codcuenta = it->text ( ccuenta );
    mdb_idcuenta = it->text ( cidcuenta );
    mdb_desccuenta = it->text ( cdesccuenta );
    BcCuentaView *nuevae = new BcCuentaView ( mainCompany(), 0 );
    nuevae->load ( idcuenta() );
    mainCompany() ->pWorkspace() ->addSubWindow ( nuevae );
    nuevae->show();
    
}


/// Esta funci&oacute;n es el SLOT que se activa al pulsar sobre el boton borrar cuenta.
/**
\return
**/
void BcPlanContableListView::on_mui_borrar_clicked()
{
    BL_FUNC_DEBUG
    QTreeWidgetItem *it;
    it = mui_arbolcuentas->currentItem();
    if ( !it ) {
        blMsgInfo ( _ ( "Debe seleccionar una cuenta" ) );
        return;
    } // end if
    int valor = QMessageBox::warning ( 0,
                                       _ ( "Borrar cuenta" ),
                                       _ ( "Se procedera a borrar la cuenta." ),
                                       QMessageBox::Yes, QMessageBox::No );

    if ( valor ==  QMessageBox::Yes ) {

	try {
	
	    mainCompany() ->begin();
	    mainCompany() ->runQuery ( "DELETE FROM cuenta WHERE idcuenta = " + it->text ( cidcuenta ) );
	    mainCompany() ->commit();
	    delete it;
	
	} catch (...) {
	
	    mainCompany()->rollback();
	    blMsgInfo ( "No se ha podido borrar la cuenta." );
	
	} // end try

    } // end if

    
}


/// Se ha hecho una doble click sobre la tabla de cuentas.
/** Al hacer doble click sobre la tabla de cuentas, se encuentra el elemento an&aacute;logo
    en el &aacute;rbol contable y se simula una doble pulsaci&oacute;n sobre ese elemento. */
/**
\param row
**/
void BcPlanContableListView::on_mui_tablacuentas_cellDoubleClicked ( int row, int )
{
    BL_FUNC_DEBUG
    QString idcuenta = mui_tablacuentas->item ( row, 2 ) ->text();
    QList <QTreeWidgetItem *> it;

    it = mui_arbolcuentas->findItems ( idcuenta, Qt::MatchRecursive | Qt::MatchExactly, cidcuenta );
    if ( ! it.isEmpty() ) {
        mui_arbolcuentas->setCurrentItem ( it.first() );
        on_mui_arbolcuentas_itemDoubleClicked ( it.first(), 0 );
    } // end if
    
}


/// Cuando se pulsa el Return sobre la b&uacute;squeda de cuentas.
/** Act&uacute;a como si fuese una doble pulsaci&oacute;n con el rat&oacute;n sobre la
    tabla de cuentas. */
void BcPlanContableListView::on_mui_busqueda_editFinished()
{
    BL_FUNC_DEBUG
    QTreeWidgetItem *it = mui_arbolcuentas->currentItem();
    if ( it != 0 ) {
        on_mui_arbolcuentas_itemDoubleClicked ( it, 0 );
    } // end if
    
}


/// Responde a la pulsaci&oacute;n del bot&oacute;n de imprimir en la ventana de cuentas.
/** Crea un string de llamada a rtkview y lo lanza como llamada de sistema.
    \todo La plantilla podr&iacute;a tener contenidos din&aacute;micos mendiante
    marcas sustituibles por un egrep, o un sedit que aun no est&aacute;n realizados.
    \todo Esta funci&oacute;n deber&iacute;a implementarse con una clase nueva de Qt
    que solicitase el rango de cuentas entre el que se quiere el listado. */
/**
**/
void BcPlanContableListView::imprimir()
{
    BL_FUNC_DEBUG
    /// Copiamos el archivo.
    QString archivo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "listado.rml";
    QString archivod = g_confpr->value( CONF_DIR_USER ) + "listado.rml";
    blCopyFile(archivo, archivod);

    /// Copiamos el logo.
    QString archivologo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "logo.jpg";
    QString logousuario = g_confpr->value( CONF_DIR_USER ) + "logo.jpg";
    blCopyFile(archivologo, logousuario);

    QFile file;
    file.setFileName ( archivod );
    file.open ( QIODevice::ReadOnly );
    QTextStream stream ( &file );
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt;

    QString query = "SELECT * FROM cuenta ORDER BY codigo";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );

    /// L&iacute;nea de totales del presupuesto.
    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr><td>Codigo</td>\n";
    fitxersortidatxt += "<td>Descripcion</td>\n";
    fitxersortidatxt += "<td>Debe</td>\n";
    fitxersortidatxt += "<td>Haber</td></tr>\n";
    while ( !cur->eof() ) {
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "<td>" + cur->value( "codigo" ) + "</td>\n";
        fitxersortidatxt += "<td>" + cur->value( "descripcion" ) + "</td>\n";
        fitxersortidatxt += "<td>" + cur->value( "debe" ) + "</td>\n";
        fitxersortidatxt += "<td>" + cur->value( "haber" ) + "</td>\n";
        fitxersortidatxt += "</tr>\n";
        cur->nextRecord();
    } // end while
    fitxersortidatxt += "</blockTable>";

    delete cur;

    buff.replace ( "[story]", fitxersortidatxt );
    buff.replace ( "[titulo]", "Cuentas" );

    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << buff;
        file.close();
    } // end if

    blCreateAndLoadPDF ( "listado" );
    
}


/**
**/
void BcPlanContableListView::on_mui_exportar_clicked()
{
    BL_FUNC_DEBUG
    QFile filexml ( QFileDialog::getSaveFileName ( this,
                    _ ( "Elija el archivo" ),
                    g_confpr->value( CONF_DIR_USER ),
                    _ ( "Plan contable (*.xml)" ) ) );
    if ( filexml.open ( QIODevice::WriteOnly ) ) {
        bulmages2XML ( filexml, IMPORT_CUENTAS );
        filexml.close();
    } else {
        blMsgInfo ( "Error al abrir archivo\n" );
    } // end if
    
}


/**
**/
void BcPlanContableListView::on_mui_importar_clicked()
{
    BL_FUNC_DEBUG
    QFile filexml ( QFileDialog::getOpenFileName ( this,
                    _ ( "Elija el archivo" ),
                    "/usr/share/bulmages",
                    _ ( "Plan contable (*.xml)" ) ) );
    if ( filexml.open ( QIODevice::ReadOnly ) ) {
        XML2Bulmages ( filexml, IMPORT_CUENTAS );
        filexml.close();
        inicializa();
    } else {
        blMsgInfo ( "Error al abrir archivo\n" );
    } // end if
    
}


///
/**
**/
void BcPlanContableListView::selectMode()
{
    BL_FUNC_DEBUG
    m_modo = BL_SELECT_MODE;
    
}


///
/**
**/
void BcPlanContableListView::editMode()
{
    BL_FUNC_DEBUG
    m_modo = BL_EDIT_MODE;
    
}


///
/**
**/
QString BcPlanContableListView::codcuenta()
{
    BL_FUNC_DEBUG
    return mdb_codcuenta;
    
}


///
/**
\return
**/
QString BcPlanContableListView::idcuenta()
{
    BL_FUNC_DEBUG
    
    return mdb_idcuenta;
}

///
/**
\return
**/
QString BcPlanContableListView::desccuenta()
{
    BL_FUNC_DEBUG
    
    return mdb_desccuenta;
}


void BcPlanContableListView::on_mui_actualizar_clicked()
{
    BL_FUNC_DEBUG
    
    inicializa();
}

