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

#include "listcuentasview1.h"
#include "blfunctions.h"
#include "cuentaview.h"
#include "bccompany.h"

/// Incluimos las im&aacute;genes que catalogan los tipos de cuentas.
#include "images/cactivo.xpm"
#include "images/cpasivo.xpm"
#include "images/cneto.xpm"
#include "images/cingresos.xpm"
#include "images/cgastos.xpm"


///
/**
\param emp
\param parent
\param flag
\param editmode
**/
listcuentasview1::listcuentasview1 ( BcCompany *emp, QWidget *parent, Qt::WFlags flag, edmode editmode )
        : BcForm ( emp, parent, flag ), pgimportfiles ( emp )
{
    _depura ( "listcuentasview1::listcuentasview1", 0 );
    setupUi ( this );

    setTitleName ( _( "Cuenta" ) );
    /// Establezco cual es la tabla en la que basarse para el sistema de permisos
    setDbTableName ( "cuenta" );

    m_modo = editmode;
    /// Para el listado de columnas hacemos una inicializaci&oacute;n.
    QStringList headers;
    headers << _( "Codigo cuenta" ) << _( "Nombre cuenta" ) << _( "Debe" ) << _( "Haber" ) << _( "ID cuenta" ) << _( "Bloqueada" ) << _( "Nodebe" ) << _( "Nohaber" ) << _( "Regularizacion" ) << _( "Imputacion" ) << _( "Grupo" ) << _( "Tipo cuenta" );
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
    headers << _( "CODIGO" ) << _( "NOMBRE" );
    mui_tablacuentas->setHorizontalHeaderLabels ( headers );

    mui_tablacuentas->hideColumn ( 2 );
    mui_tablacuentas->setColumnWidth ( 1, 400 );
    mui_tablacuentas->setColumnWidth ( 0, 100 );

    installEventFilter ( this );
    if ( m_modo == EditMode )
        mainCompany() ->meteWindow ( windowTitle(), this );
    _depura ( "END listcuentasview1::listcuentasview1", 0 );
}


///
/**
**/
listcuentasview1::~listcuentasview1()
{
    _depura ( "listcuentasview1::~listcuentasview1", 10 );
    if ( m_modo == EditMode ) {
        mainCompany() ->sacaWindow ( this );
    }// end if
    _depura ( "END listcuentasview1::~listcuentasview1", 10 );
}


///
/**
\return
**/
bool listcuentasview1::eventFilter ( QObject *obj, QEvent *event )
{
    _depura ( "listcuentasview1::eventFilter", 0 );
    _depura ( "END listcuentasview1::eventFilter", 0 );
    return QWidget::eventFilter ( obj, event );
}


/// Se encarga de hacer las inicializaciones de todo el formulario.
/** Se llama as&iacute; y no desde el constructor porque as&iacute; la podemos llamar
    desde dentro de la misma clase, etc, etc, etc. */
/**
\return
**/
int listcuentasview1::inicializa()
{
    _depura ( "listcuentasview1::inicializa", 0 );
    try {
        QTreeWidgetItem *it;
        QMap <int, QTreeWidgetItem*> Lista1;
        int idcuenta;
        int padre;
        mui_arbolcuentas->clear();

        /// Cargamos y pintamos las cuentas hijas.
        BlDbRecordSet *ctas = mainCompany() ->loadQuery ( "SELECT * FROM cuenta ORDER BY codigo" );
        while ( !ctas->eof() ) {

            idcuenta = ctas->valor ( "idcuenta" ).toInt();
            padre = ctas->valor ( "padre" ).toInt();

            if ( padre == 0 ) {

                it = new QTreeWidgetItem ( mui_arbolcuentas );

                Lista1[idcuenta] = it;
            } else {
                it = new QTreeWidgetItem ( Lista1[padre] );
                Lista1[idcuenta] = it;
            } // end if


            it->setText ( ccuenta, ctas->valor ( "codigo" ) );
            it->setText ( cdesccuenta, ctas->valor ( "descripcion" ) );
            it->setText ( cidcuenta, ctas->valor ( "idcuenta" ) );
            it->setText ( cbloqueada, ctas->valor ( "bloqueada" ) );
            it->setText ( cnodebe, ctas->valor ( "nodebe" ) );
            it->setText ( cnohaber, ctas->valor ( "nohaber" ) );
            it->setText ( cregularizacion, ctas->valor ( "regularizacion" ) );
            it->setText ( cimputacion, ctas->valor ( "imputacion" ) );
            it->setText ( cdebe, ctas->valor ( "debe" ) );
            it->setText ( chaber, ctas->valor ( "haber" ) );

            /// Ponemos los iconos.
            if ( ctas->valor ( "tipocuenta" ) == "1" )
                it->setIcon ( ccuenta, QPixmap ( cactivo ) );
            else if ( ctas->valor ( "tipocuenta" ) == "2" )
                it->setIcon ( ccuenta, QPixmap ( cpasivo ) );
            else if ( ctas->valor ( "tipocuenta" ) == "3" )
                it->setIcon ( ccuenta, QPixmap ( cneto ) );
            else if ( ctas->valor ( "tipocuenta" ) == "4" )
                it->setIcon ( ccuenta, QPixmap ( cingresos ) );
            else if ( ctas->valor ( "tipocuenta" ) == "5" )
                it->setIcon ( ccuenta, QPixmap ( cgastos ) );

            ctas->nextRecord();
        } // end while
        mui_arbolcuentas->expandAll();
        delete ctas;

        /// Cargamos el n&uacute;mero de d&iacute;gitos de cuenta para poder hacer una
        /// introducci&oacute;n de n&uacute;meros de cuenta m&aacute;s pr&aacute;ctica.
        numdigitos = mainCompany() ->numdigitosempresa();

        inicializatabla();
    } catch ( ... ) {
        mensajeInfo ( "Error en la carga" );
        _depura ( "END listcuentasview1::inicializa", 0, "Error en la carga" );
        return -1;
    } // end try
    _depura ( "END listcuentasview1::inicializa", 0 );
    return 0;
}


/// Inicializa la tabla de cuentas, que es la segunda pesta&ntilde;a de la pantalla.
/** Dicha tabla s&oacute;lo muestra las cuentas hoja del plan contable, y deja los
    dem&aacute;s elementos ocultos. */
/**
**/
void listcuentasview1::inicializatabla()
{
    _depura ( "listcuentasview1::inicializatabla", 0 );
    QString query;
    query = "SELECT * FROM cuenta ORDER BY codigo";
    BlDbRecordSet *cursoraux1 = mainCompany() ->loadQuery ( query );
    mui_tablacuentas->setRowCount ( cursoraux1->numregistros() );
    int i = 0;
    QTableWidgetItem *dato;
    while ( !cursoraux1->eof() ) {
        dato = new QTableWidgetItem ( cursoraux1->valor ( "codigo" ) );
        dato->setFlags ( Qt::ItemIsEnabled );
        /// Ponemos los iconos.
        if ( cursoraux1->valor ( "tipocuenta" ) == "1" )
            dato->setIcon ( QPixmap ( cactivo ) );
        else if ( cursoraux1->valor ( "tipocuenta" ) == "2" )
            dato->setIcon ( QPixmap ( cpasivo ) );
        else if ( cursoraux1->valor ( "tipocuenta" ) == "3" )
            dato->setIcon ( QPixmap ( cneto ) );
        else if ( cursoraux1->valor ( "tipocuenta" ) == "4" )
            dato->setIcon ( QPixmap ( cingresos ) );
        else if ( cursoraux1->valor ( "tipocuenta" ) == "5" )
            dato->setIcon ( QPixmap ( cgastos ) );
        mui_tablacuentas->setItem ( i, 0, dato );
        dato = new QTableWidgetItem ( cursoraux1->valor ( "descripcion" ) );
        dato->setFlags ( Qt::ItemIsEnabled );
        mui_tablacuentas->setItem ( i, 1, dato );
        dato->setFlags ( Qt::ItemIsEnabled );
        dato = new QTableWidgetItem ( cursoraux1->valor ( "idcuenta" ) );
        mui_tablacuentas->setItem ( i, 2, dato );

        QString codigo = cursoraux1->valor ( "codigo" );
        if ( ( unsigned int ) codigo.length() != numdigitos ) {
            mui_tablacuentas->hideRow ( i );
        } // end if
        cursoraux1->nextRecord();
        i++;
    } // end while
    delete cursoraux1;
    _depura ( "END listcuentasview1::inicializatabla", 0 );
}


/// Este es el SLOT que se activa al hacer click sobre &eacute;l.
/** ListView del formulario. Lo que hace es actualizar los valores
    de la derecha del formulario con los valores almacenados en el
    item que se acaba de pulsar. */
/**
\param it
**/
void listcuentasview1::on_mui_arbolcuentas_itemClicked ( QTreeWidgetItem *it, int )
{
    _depura ( "listcuentasview1::on_mui_arbolcuentas_itemClicked", 0 );
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
    _depura ( "END listcuentasview1::on_mui_arbolcuentas_itemClicked", 0 );
}


/// La pantalla lleva implicito un buscador de cuentas, cuando cambia el contenido
/** del QLineEdit del buscador se lanza esta funci&oacute;n que hace una b&uacute;squeda
    sobre el &aacute;rbol de cuentas. */
/**
\param string1
**/
void listcuentasview1::on_mui_busqueda_textChanged ( const QString &string1 )
{
    _depura ( "listcuentasview1::on_mui_busqueda_textChanged", 0 );
    QList<QTreeWidgetItem *> it;
    QString cod = extiendecodigo ( string1, ( int ) numdigitos );
    it = mui_arbolcuentas->findItems ( cod, Qt::MatchStartsWith, ccuenta );
    if ( it.count() > 0 ) {
        mui_arbolcuentas->setCurrentItem ( it.first() );
    } else {
        QTreeWidgetItemIterator it ( mui_arbolcuentas );
        QRegExp patron ( "^.*" + string1 + ".*$" );
        patron.setCaseSensitivity ( Qt::CaseInsensitive );
        while ( *it ) {
            if ( patron.exactMatch ( ( *it ) ->text ( cdesccuenta ) ) )
                mui_arbolcuentas->setCurrentItem ( *it );
            ++it;
        } // end while
    } // end if
    _depura ( "END listcuentasview1::on_mui_busqueda_textChanged", 0 );
}


/// Este es el SLOT que se activa al hacer doble click sobre &eacute;l.
/** ListView del formulario. Lo que hace es abrir la ventana de
    detalle de cuenta para que se puedan modificar los par&aacute;metros
    y una vez terminado refresca el formulario para que aparezcan
    los datos actualizados. */
/**
\param it
**/
void listcuentasview1::on_mui_arbolcuentas_itemDoubleClicked ( QTreeWidgetItem *it, int )
{
    _depura ( "listcuentasview1::on_mui_arbolcuentas_doubleClicked", 0 );
    on_mui_arbolcuentas_itemClicked ( it, 0 );
    mdb_codcuenta = it->text ( ccuenta );
    mdb_idcuenta = it->text ( cidcuenta );
    mdb_desccuenta = it->text ( cdesccuenta );
    if ( m_modo == EditMode ) {
        CuentaView * nuevae = new CuentaView ( mainCompany(), 0 );
        nuevae->cargar ( idcuenta() );
        inicializa();
        mainCompany() ->pWorkspace() ->addWindow ( nuevae );
        nuevae->show();
    } else {
        emit ( selected ( mdb_idcuenta ) );
    } // end if
    _depura ( "END listcuentasview1::on_mui_arbolcuentas_doubleClicked", 0 );
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
void listcuentasview1::on_mui_crear_clicked()
{
    _depura ( "listcuentasview1::on_mui_crear_clicked", 0 );
    QString codigo;

    CuentaView *nuevae = new CuentaView ( mainCompany(), 0 );

    if ( mui_arbolcuentas->selectedItems().count() > 0 ) {
        QTreeWidgetItem * it;
        it = mui_arbolcuentas->currentItem();
        if ( it ) {
            codigo = it->text ( ccuenta );
            nuevae->nuevacuenta ( codigo );
        } // end if
    } // end if

    mainCompany() ->pWorkspace() ->addWindow ( nuevae );
    nuevae->show();
    _depura ( "END listcuentasview1::on_mui_crear_clicked", 0 );
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
void listcuentasview1::on_mui_editar_clicked()
{
    _depura ( "listcuentasview1::on_mui_editar_clicked", 0 );
    QTreeWidgetItem *it;
    it = mui_arbolcuentas->currentItem();
    if ( !it ) {
        mensajeInfo ( _( "Debe seleccionar una cuenta" ) );
        _depura ( "END listcuentasview1::on_mui_editar_clicked", 0, "Debe seleccionar una cuenta" );
        return;
    }
    on_mui_arbolcuentas_itemClicked ( it, 0 );
    mdb_codcuenta = it->text ( ccuenta );
    mdb_idcuenta = it->text ( cidcuenta );
    mdb_desccuenta = it->text ( cdesccuenta );
    CuentaView *nuevae = new CuentaView ( mainCompany(), 0 );
    nuevae->cargar ( idcuenta() );
    mainCompany() ->pWorkspace() ->addWindow ( nuevae );
    nuevae->show();
    _depura ( "END listcuentasview1::on_mui_editar_clicked", 0 );
}


/// Esta funci&oacute;n es el SLOT que se activa al pulsar sobre el boton borrar cuenta.
/**
\return
**/
void listcuentasview1::on_mui_borrar_clicked()
{
    _depura ( "listcuentasview1::on_mui_borrar_clicked", 0 );
    QTreeWidgetItem *it;
    it = mui_arbolcuentas->currentItem();
    if ( !it ) {
        mensajeInfo ( _( "Debe seleccionar una cuenta" ) );
        return;
    } // end if
    int valor = QMessageBox::warning ( 0,
                                       _( "Borrar cuenta" ),
                                       _( "Se procedera a borrar la cuenta." ),
                                       QMessageBox::Yes, QMessageBox::No );
    if ( valor ==  QMessageBox::Yes ) {
        int idcuenta = atoi ( ( char * ) it->text ( cidcuenta ).toAscii().constData() );
        mainCompany() ->begin();
        if ( mainCompany() ->borrarcuenta ( idcuenta ) == 0 ) {
            delete it;
        } else {
            mensajeInfo ( "No se ha podido borrar la cuenta." );
        } // end if
        mainCompany() ->commit();
    } // end if
    _depura ( "END listcuentasview1::on_mui_borrar_clicked", 0 );
}


/// Se ha hecho una doble click sobre la tabla de cuentas.
/** Al hacer doble click sobre la tabla de cuentas, se encuentra el elemento an&aacute;logo
    en el &aacute;rbol contable y se simula una doble pulsaci&oacute;n sobre ese elemento. */
/**
\param row
**/
void listcuentasview1::on_mui_tablacuentas_cellDoubleClicked ( int row, int )
{
    _depura ( "listcuentasview1::on_mui_tablacuentas_doubleClicked", 0 );
    QString idcuenta = mui_tablacuentas->item ( row, 2 ) ->text();
    QList <QTreeWidgetItem *> it;

    it = mui_arbolcuentas->findItems ( idcuenta, Qt::MatchRecursive | Qt::MatchExactly, cidcuenta );
    if ( ! it.isEmpty() ) {
        mui_arbolcuentas->setCurrentItem ( it.first() );
        on_mui_arbolcuentas_itemDoubleClicked ( it.first(), 0 );
    } // end if
    _depura ( "END listcuentasview1::on_mui_tablacuentas_doubleClicked", 0 );
}


/// Cuando se pulsa el Return sobre la b&uacute;squeda de cuentas.
/** Act&uacute;a como si fuese una doble pulsaci&oacute;n con el rat&oacute;n sobre la
    tabla de cuentas. */
void listcuentasview1::on_mui_busqueda_editFinished()
{
    _depura ( "listcuentasview1::on_mui_busqueda_editFinished", 0 );
    QTreeWidgetItem *it = mui_arbolcuentas->currentItem();
    if ( it != 0 ) {
        on_mui_arbolcuentas_itemDoubleClicked ( it, 0 );
    } // end if
    _depura ( "END listcuentasview1::on_mui_busqueda_editFinished", 0 );
}


/// Responde a la pulsaci&oacute;n del bot&oacute;n de imprimir en la ventana de cuentas.
/** Crea un string de llamada a rtkview y lo lanza como llamada de sistema.
    \todo La plantilla podr&iacute;a tener contenidos din&aacute;micos mendiante
    marcas sustituibles por un egrep, o un sedit que aun no est&aacute;n realizados.
    \todo Esta funci&oacute;n deber&iacute;a implementarse con una clase nueva de Qt
    que solicitase el rango de cuentas entre el que se quiere el listado. */
/**
**/
void listcuentasview1::imprimir()
{
    _depura ( "listcuentasview1::imprimir", 0 );
    QString archivo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "listado.rml";
    QString archivod = confpr->valor ( CONF_DIR_USER ) + "listado.rml";
    QString archivologo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "logo.jpg";
    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system ( archivo.toAscii() );
    /// Copiamos el logo.
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
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
        fitxersortidatxt += "<td>" + cur->valor ( "codigo" ) + "</td>\n";
        fitxersortidatxt += "<td>" + cur->valor ( "descripcion" ) + "</td>\n";
        fitxersortidatxt += "<td>" + cur->valor ( "debe" ) + "</td>\n";
        fitxersortidatxt += "<td>" + cur->valor ( "haber" ) + "</td>\n";
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

    invocaPDF ( "listado" );
    _depura ( "END listcuentasview1::imprimir", 0 );
}


/**
**/
void listcuentasview1::on_mui_exportar_clicked()
{
    _depura ( "listcuentasview1::on_mui_exportar_clicked", 0 );
    QFile filexml ( QFileDialog::getSaveFileName ( this,
                    _( "Elija el archivo" ),
                    confpr->valor ( CONF_DIR_USER ),
                    _( "Plan contable (*.xml)" ) ) );
    if ( filexml.open ( QIODevice::WriteOnly ) ) {
        bulmages2XML ( filexml, IMPORT_CUENTAS );
        filexml.close();
    } else {
        mensajeInfo ( "Error al abrir archivo\n" );
    } // end if
    _depura ( "END listcuentasview1::on_mui_exportar_clicked", 0 );
}


/**
**/
void listcuentasview1::on_mui_importar_clicked()
{
    _depura ( "listcuentasview1::on_mui_importar_clicked", 0 );
    QFile filexml ( QFileDialog::getOpenFileName ( this,
                    _( "Elija el archivo" ),
                    "/usr/share/bulmages",
                    _( "Plan contable (*.xml)" ) ) );
    if ( filexml.open ( QIODevice::ReadOnly ) ) {
        XML2Bulmages ( filexml, IMPORT_CUENTAS );
        filexml.close();
        inicializa();
    } else {
        mensajeInfo ( "Error al abrir archivo\n" );
    } // end if
    _depura ( "END listcuentasview1::on_mui_importar_clicked", 0 );
}


///
/**
**/
void listcuentasview1::selectMode()
{
    _depura ( "listcuentasview1::selectMode", 0 );
    m_modo = SelectMode;
    _depura ( "END listcuentasview1::selectMode", 0 );
}


///
/**
**/
void listcuentasview1::editMode()
{
    _depura ( "listcuentasview1::editMode", 0 );
    m_modo = EditMode;
    _depura ( "END listcuentasview1::editMode", 0 );
}


///
/**
**/
QString listcuentasview1::codcuenta()
{
    _depura ( "listcuentasview1::codcuenta", 0 );
    return mdb_codcuenta;
    _depura ( "END listcuentasview1::codcuenta", 0 );
}


///
/**
\return
**/
QString listcuentasview1::idcuenta()
{
    _depura ( "listcuentasview1::idcuenta", 0 );
    _depura ( "END listcuentasview1::idcuenta", 0 );
    return mdb_idcuenta;
}

///
/**
\return
**/
QString listcuentasview1::desccuenta()
{
    _depura ( "listcuentasview1::desccuenta", 0 );
    _depura ( "listcuentasview1::desccuenta", 0 );
    return mdb_desccuenta;
}


void listcuentasview1::on_mui_actualizar_clicked()
{
    _depura ( "listcuentasview1::on_mui_actualizar_clicked", 0 );
    _depura ( "END listcuentasview1::on_mui_actualizar_clicked", 0 );
    inicializa();
}

