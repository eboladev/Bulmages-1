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
#include "funcaux.h"
#include "cuentaview.h"
#include "empresa.h"

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
listcuentasview1::listcuentasview1 ( Empresa *emp, QWidget *parent, Qt::WFlags flag, edmode editmode )
        : FichaBc ( emp, parent, flag ), pgimportfiles ( emp )
{
    _depura ( "listcuentasview1::listcuentasview1", 0 );
    setupUi ( this );

    setTitleName ( tr ( "Cuenta" ) );
    /// Establezco cual es la tabla en la que basarse para el sistema de permisos
    setDBTableName ( "cuenta" );

    m_modo = editmode;
    /// Para el listado de columnas hacemos una inicializaci&oacute;n.
    QStringList headers;
    headers << tr ( "Codigo cuenta" ) << tr ( "Nombre cuenta" ) << tr ( "Debe" ) << tr ( "Haber" ) << tr ( "ID cuenta" ) << tr ( "Bloqueada" ) << tr ( "Nodebe" ) << tr ( "Nohaber" ) << tr ( "Regularizacion" ) << tr ( "Imputacion" ) << tr ( "Grupo" ) << tr ( "Tipo cuenta" );
    ListView1->setHeaderLabels ( headers );
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

    ListView1->setColumnWidth ( ccuenta, 175 );
    ListView1->setColumnWidth ( cdesccuenta, 400 );
    ListView1->hideColumn ( cidcuenta );
    ListView1->hideColumn ( cnodebe );
    ListView1->hideColumn ( cnohaber );
    ListView1->hideColumn ( cregularizacion );
    ListView1->hideColumn ( cimputacion );
    ListView1->hideColumn ( cgrupo );
    ListView1->hideColumn ( ctipocuenta );

    tablacuentas->setColumnCount ( 3 );
    headers << tr ( "CODIGO" ) << tr ( "NOMBRE" );
    tablacuentas->setHorizontalHeaderLabels ( headers );

    tablacuentas->hideColumn ( 2 );
    tablacuentas->setColumnWidth ( 1, 400 );
    tablacuentas->setColumnWidth ( 0, 100 );

    installEventFilter ( this );
    if ( m_modo == EditMode )
        empresaBase() ->meteWindow ( windowTitle(), this );
    _depura ( "END listcuentasview1::listcuentasview1", 0 );
}


///
/**
**/
listcuentasview1::~listcuentasview1()
{
    _depura ( "listcuentasview1::~listcuentasview1", 10 );
    if ( m_modo == EditMode ) {
        empresaBase() ->sacaWindow ( this );
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
    QTreeWidgetItem *it;
    QMap <int, QTreeWidgetItem*> Lista1;
    int idcuenta;
    int padre;
    ListView1->clear();

    /// Cargamos y pintamos las cuentas hijas.
    cursor2 *ctas = empresaBase() ->cargacursor ( "SELECT * FROM cuenta ORDER BY codigo" );
    if ( ctas->eof() ) {
        _depura ( "El query esta vacio\n", 0 );
    } else {
        fprintf ( stderr, "El query tiene registros \n" );
    } // end if
    while ( !ctas->eof() ) {
        idcuenta = ctas->valor ( "idcuenta" ).toInt();
        padre = ctas->valor ( "padre" ).toInt();
        if ( padre == 0 ) {
            it = new QTreeWidgetItem ( ListView1 );
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
        ListView1->setItemExpanded ( it, TRUE );
        ctas->siguienteregistro();
    } // end while
    delete ctas;

    /// Cargamos el n&uacute;mero de d&iacute;gitos de cuenta para poder hacer una
    /// introducci&oacute;n de n&uacute;meros de cuenta m&aacute;s pr&aacute;ctica.
    numdigitos = empresaBase() ->numdigitosempresa();

    inicializatabla();
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
    cursor2 *cursoraux1 = empresaBase() ->cargacursor ( query );
    tablacuentas->setRowCount ( cursoraux1->numregistros() );
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
        tablacuentas->setItem ( i, 0, dato );
        dato = new QTableWidgetItem ( cursoraux1->valor ( "descripcion" ) );
        dato->setFlags ( Qt::ItemIsEnabled );
        tablacuentas->setItem ( i, 1, dato );
        dato->setFlags ( Qt::ItemIsEnabled );
        dato = new QTableWidgetItem ( cursoraux1->valor ( "idcuenta" ) );
        tablacuentas->setItem ( i, 2, dato );

        QString codigo = cursoraux1->valor ( "codigo" );
        if ( ( unsigned int ) codigo.length() != numdigitos ) {
            tablacuentas->hideRow ( i );
        } // end if
        cursoraux1->siguienteregistro();
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
void listcuentasview1::on_ListView1_itemClicked ( QTreeWidgetItem *it, int )
{
    _depura ( "listcuentasview1::on_ListView1_itemClicked", 0 );
    QString idcuenta = it->text ( cidcuenta );
    QString cad;
    for ( int i = 0; i < tablacuentas->rowCount() - 1; i++ ) {
        cad = tablacuentas->item ( i, 2 ) ->text();
        if ( cad == idcuenta ) {
            tablacuentas->setCurrentCell ( i, 2 );
            tablacuentas->scrollToItem ( tablacuentas->item ( i, 2 ), QAbstractItemView::EnsureVisible );
            break;
        } // end if
    } // end for
    _depura ( "END listcuentasview1::on_ListView1_itemClicked", 0 );
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
    it = ListView1->findItems ( cod, Qt::MatchStartsWith, ccuenta );
    if ( it.count() > 0 ) {
        ListView1->setCurrentItem ( it.first() );
    } else {
        QTreeWidgetItemIterator it ( ListView1 );
        QRegExp patron ( "^.*" + string1 + ".*$" );
        patron.setCaseSensitivity ( Qt::CaseInsensitive );
        while ( *it ) {
            if ( patron.exactMatch ( ( *it ) ->text ( cdesccuenta ) ) )
                ListView1->setCurrentItem ( *it );
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
void listcuentasview1::on_ListView1_itemDoubleClicked ( QTreeWidgetItem *it, int )
{
    _depura ( "listcuentasview1::on_ListView1_doubleClicked", 0 );
    on_ListView1_itemClicked ( it, 0 );
    mdb_codcuenta = it->text ( ccuenta );
    mdb_idcuenta = it->text ( cidcuenta );
    mdb_desccuenta = it->text ( cdesccuenta );
    if ( m_modo == EditMode ) {
        CuentaView * nuevae = new CuentaView ( empresaBase(), 0 );
        nuevae->cargar ( idcuenta() );
        inicializa();
        empresaBase() ->pWorkspace() ->addWindow ( nuevae );
        nuevae->show();
    } else {
        emit ( selected ( mdb_idcuenta ) );
    } // end if
    _depura ( "listcuentasview1::on_ListView1_doubleClicked", 0 );
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

    CuentaView *nuevae = new CuentaView ( empresaBase(), 0 );

    if ( ListView1->selectedItems().count() > 0 ) {
        QTreeWidgetItem * it;
        it = ListView1->currentItem();
        if ( it ) {
            codigo = it->text ( ccuenta );
            nuevae->nuevacuenta ( codigo );
        } // end if
    } // end if

    empresaBase() ->pWorkspace() ->addWindow ( nuevae );
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
    it = ListView1->currentItem();
    if ( !it ) {
        mensajeInfo ( tr ( "Debe seleccionar una cuenta" ) );
        return;
    }
    on_ListView1_itemClicked ( it, 0 );
    mdb_codcuenta = it->text ( ccuenta );
    mdb_idcuenta = it->text ( cidcuenta );
    mdb_desccuenta = it->text ( cdesccuenta );
    CuentaView *nuevae = new CuentaView ( empresaBase(), 0 );
    nuevae->cargar ( idcuenta() );
    empresaBase() ->pWorkspace() ->addWindow ( nuevae );
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
    it = ListView1->currentItem();
    if ( !it ) {
        mensajeInfo ( tr ( "Debe seleccionar una cuenta" ) );
        return;
    } // end if
    int valor = QMessageBox::warning ( 0,
                                       tr ( "Borrar cuenta" ),
                                       tr ( "Se procedera a borrar la cuenta." ),
                                       QMessageBox::Yes, QMessageBox::No );
    if ( valor ==  QMessageBox::Yes ) {
        int idcuenta = atoi ( ( char * ) it->text ( cidcuenta ).toAscii().constData() );
        empresaBase() ->begin();
        if ( empresaBase() ->borrarcuenta ( idcuenta ) == 0 ) {
            delete it;
        } else {
            mensajeInfo ( "No se ha podido borrar la cuenta." );
        } // end if
        empresaBase() ->commit();
    } // end if
    _depura ( "END listcuentasview1::on_mui_borrar_clicked", 0 );
}


/// Se ha hecho una doble click sobre la tabla de cuentas.
/** Al hacer doble click sobre la tabla de cuentas, se encuentra el elemento an&aacute;logo
    en el &aacute;rbol contable y se simula una doble pulsaci&oacute;n sobre ese elemento. */
/**
\param row
**/
void listcuentasview1::on_tablacuentas_doubleClicked ( int row, int, int, const QPoint & )
{
    _depura ( "listcuentasview1::on_tablacuentas_doubleClicked", 0 );
    QString idcuenta = tablacuentas->item ( row, 2 ) ->text();
    QList <QTreeWidgetItem *> it;
    it = ListView1->findItems ( idcuenta, Qt::MatchExactly, cidcuenta );
    ListView1->setCurrentItem ( it.first() );
    on_ListView1_itemDoubleClicked ( it.first(), 0 );
    _depura ( "END listcuentasview1::on_tablacuentas_doubleClicked", 0 );
}


/// Cuando se pulsa el Return sobre la b&uacute;squeda de cuentas.
/** Act&uacute;a como si fuese una doble pulsaci&oacute;n con el rat&oacute;n sobre la
    tabla de cuentas. */
void listcuentasview1::on_mui_busqueda_editFinished()
{
    _depura ( "listcuentasview1::on_mui_busqueda_editFinished", 0 );
    QTreeWidgetItem *it = ListView1->currentItem();
    if ( it != 0 ) {
        on_ListView1_itemDoubleClicked ( it, 0 );
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
void listcuentasview1::on_mui_imprimir_clicked()
{
    _depura ( "listcuentasview1::on_mui_imprimir_clicked", 0 );
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
    cursor2 *cur = empresaBase() ->cargacursor ( query );

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
        cur->siguienteregistro();
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
    _depura ( "END listcuentasview1::on_mui_imprimir_clicked", 0 );
}


/**
**/
void listcuentasview1::on_mui_exportar_clicked()
{
    _depura ( "listcuentasview1::on_mui_exportar_clicked", 0 );
    QFile filexml ( QFileDialog::getSaveFileName ( this,
                    tr ( "Elija el archivo" ),
                    confpr->valor ( CONF_DIR_USER ),
                    tr ( "Plan contable (*.xml)" ) ) );
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
                    tr ( "Elija el archivo" ),
                    confpr->valor ( CONF_DIR_USER ),
                    tr ( "Plan contable (*.xml)" ) ) );
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
    _depura ( "listcuentasview1::selectMode", 0 );
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
    _depura ( "listcuentasview1::codcuenta", 0 );
}


///
/**
\return
**/
QString listcuentasview1::idcuenta()
{
    _depura ( "listcuentasview1::idcuenta", 0 );
    _depura ( "listcuentasview1::idcuenta", 0 );
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

