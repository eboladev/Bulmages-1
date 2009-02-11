/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Francisco Javier M. C.                          *
 *   fcojavmc@todo-redes.com                                               *
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

#include <fstream>
#include <stdio.h>

#include <QLineEdit>
#include <QMessageBox>
#include <QString>
#include <QList>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QStringList>
#include <QHeaderView>
#include <QKeyEvent>
#include <QEvent>
#include <QTextStream>
#include <QTextCodec>

#include "abreempresaview.h"
#include "blpostgresqlclient.h"
#include "blconfiguration.h"
#include "funcaux.h"


/// Este es el archivo en el que se almacenan las mui_empresas que existen.
/// Es un archivo separado por tabuladores, que se suele alojar en
/// el 'home/.bulmages/' del usuario.
#define LISTEMPRESAS QString("listempresas.lst")
/// N&uacute;mero de columna para el nombre en la lista.
#define ABRE_NOMBRE  0
/// N&uacute;mero de columna que almacena el ejercicio de la empresa.
#define ABRE_ANO     1
/// N&uacute;mero de columna que almacena el nombre de la base de datos de la empresa.
#define ABRE_ARCHIVO 2
/// N&uacute;mero de columna que almacena el tipo de datos al que hace referencia la l&iacute;nea
/// (BulmaCont o BulmaFact).
#define ABRE_TIPO    3


///
/**
\param pNomDB
**/
void abreempresaview::setNomDB ( QString pNomDB )
{
    _depura ( "abreempresaview::setNomDB", 0 );
    m_empresabd = pNomDB;
    _depura ( "END abreempresaview::setNomDB", 0 );
}


///
/**
\param pNomEmpresa
**/
void abreempresaview::setNomEmpresa ( QString pNomEmpresa )
{
    _depura ( "abreempresaview::setNomEmpresa", 0 );
    m_nombreempresa = pNomEmpresa;
    _depura ( "END abreempresaview::setNomEmpresa", 0 );
}


///
/**
\param pTipoEmpresa
**/
void abreempresaview::setTipoEmpresa ( QString pTipoEmpresa )
{
    _depura ( "abreempresaview::setTipoEmpresa", 0 );
    m_tipoempresa = pTipoEmpresa;
    _depura ( "END abreempresaview::setTipoEmpresa", 0 );

}


///
/**
\return
**/
bool abreempresaview::modoDestructivo()
{
    _depura ( "abreempresaview::modoDestructivo", 0 );
    _depura ( "END abreempresaview::modoDestructivo", 0 );
    return m_modo;
}


///
/**
\param pModo
**/
void abreempresaview::setModoDestructivo ( bool pModo )
{
    _depura ( "abreempresaview::setModoDestructivo", 0 );
    m_modo = pModo;
    _depura ( "END abreempresaview::setModoDestructivo", 0 );
}


///
/**
\return
**/
QString abreempresaview::nomDB()
{
    _depura ( "abreempresaview::nomDB", 0 );
    _depura ( "END abreempresaview::nomDB", 0 );
    return m_empresabd;
}


///
/**
\return
**/
QString abreempresaview::nomEmpresa()
{
    _depura ( "abreempresaview::nomEmpresa", 0 );
    _depura ( "END abreempresaview::nomEmpresa", 0 );
    return m_nombreempresa;
}


///
/**
\return
**/
QString abreempresaview::tipoEmpresa()
{
    _depura ( "abreempresaview::tipoEmpresa", 0 );
    _depura ( "END abreempresaview::tipoEmpresa", 0 );
    return m_tipoempresa;
}


///
/**
**/
void abreempresaview::s_botonCancelar()
{
    _depura ( "abreempresaview::s_botonCancelar", 0 );
    if ( !modoDestructivo() )
        exit ( 1 );
    else
        done ( 1 );
    // end if
    _depura ( "END abreempresaview::s_botonCancelar", 0 );
}


///
/**
\return
**/
bool abreempresaview::close ( bool )
{
    _depura ( "abreempresaview::close", 0 );
    s_botonCancelar();
    _depura ( "END abreempresaview::close", 0 );
    return TRUE;
}


/// Inicia las variables m_tipo y m_tipoempresa y carga el archivo para
/// hacer la presentaci&oacute;n.
/// Llama a la funci&oacute;n 'cargaArchivo'.
/// @param parent La ventana que hace la llamada.
/// @param tipo Cadena de texto que indica si es contabilidad o facturaci&oacute;n.
/// @param name Nombre de la ventana.
/// @param modal Indica si la ventana debe comportar de forma 'modal' o no. (por defecto s&iacute;).
abreempresaview::abreempresaview ( QWidget *parent, QString tipo, const char *name, bool modal )
        : QDialog ( parent )
{
    _depura ( "abreempresaview::abreempresaview", 0 );
    setModal ( modal );
    this->setWindowTitle ( name );
    setupUi ( this );
    centrarEnPantalla ( this );

    mui_empresas->installEventFilter ( this );
    QObject::connect ( botonCancelar, SIGNAL ( clicked ( bool ) ), this, SLOT ( s_botonCancelar() ) );
    QObject::connect ( botonAceptar, SIGNAL ( clicked ( bool ) ), this, SLOT ( accept() ) );
    QObject::connect ( mui_empresas, SIGNAL ( itemDoubleClicked ( QTableWidgetItem * ) ), this,
                       SLOT ( mui_empresasdobleclick() ) );

    m_tipo = tipo;
    setTipoEmpresa ( "" );
    setModoDestructivo ( FALSE );
    /// Si el m_modo es false (salir del programa si se cancela la ventana) entonces se pone
    /// un t&iacute;tulo adecuado al bot&oacute;n.
    if ( !modoDestructivo() )
        botonCancelar->setText ( _( "&Cierra el programa" ) );
    // end if
    cargaArchivo();
    _depura ( "END abreempresaview::abreempresaview", 0 );
}


///
/**
**/
abreempresaview::~abreempresaview()
{
    _depura ( "abreempresaview::~abreempresaview", 0 );
    _depura ( "END abreempresaview::~abreempresaview", 0 );

}


/// Inserta una compa&ntilde;ia en el 'QList' mui_empresas definido en el di&aacute;logo.
/// Crea un objeto QListViewItem para la QListView 'mui_empresas' y rellena sus
/// columnas con los datos pasados al m&eacute;todo.
/// @param nombre Nombre de la empresa.
/// @param ano Ejercicio de la empresa (aunque pueden ser varios).
/// @param archivo Nombre de la base de datos.
/// @param tipo Tipo de base de datos (BulmaCont o BulmaFact).
void abreempresaview::insertCompany ( QString nombre, QString ano, QString archivo,
                                      QString tipo )
{
    _depura ( "abreempresaview::insertCompany", 0, nombre );
    mui_empresas->insertRow ( mui_empresas->rowCount() );
    QTableWidgetItem *nuevoItemNombre = new QTableWidgetItem ( nombre );
    mui_empresas->setItem ( mui_empresas->rowCount() - 1, 0, nuevoItemNombre );
    QTableWidgetItem *nuevoItemAno = new QTableWidgetItem ( ano );
    mui_empresas->setItem ( mui_empresas->rowCount() - 1, 1, nuevoItemAno );
    QTableWidgetItem *nuevoItemArchivo = new QTableWidgetItem ( archivo );
    mui_empresas->setItem ( mui_empresas->rowCount() - 1, 2, nuevoItemArchivo );
    QTableWidgetItem *nuevoItemTipo = new QTableWidgetItem ( tipo );
    mui_empresas->setItem ( mui_empresas->rowCount() - 1, 3, nuevoItemTipo );
    _depura ( "END abreempresaview::insertCompany", 0 );
}


/// Se ha pulsado sobre el bot&oacute;n de aceptar con lo que iniciamos la variables y
/// cerramos esta ventana ya que ha cumplido con su cometido.
/**
**/
void abreempresaview::accept()
{
    _depura ( "abreempresaview::accept", 0 );
    /// Columna 0: Nombre
    /// Columna 1: anyo.
    /// Columna 2: Archivo.
    /// Columna 3: Tipo.
    QTableWidgetItem *itemfilaactual0;
    QTableWidgetItem *itemfilaactual2;
    QTableWidgetItem *itemfilaactual3;
    /// Comprueba que la fila est&aacute; seleccionada.
    /// (S&oacute;lo necesitamos comprobar una celda de la fila) y luego
    /// recupera la informaci&oacute;n de la fila actual.
    if ( ( mui_empresas->currentRow() != -1 ) &&
            ( mui_empresas->isItemSelected ( mui_empresas->item ( mui_empresas->currentRow(), 0 ) ) == TRUE ) ) {
        itemfilaactual0 = mui_empresas->item ( mui_empresas->currentRow(), 0 );
        setNomEmpresa ( itemfilaactual0->text() );
        itemfilaactual2 = mui_empresas->item ( mui_empresas->currentRow(), 2 );
        setNomDB ( itemfilaactual2->text() );
        itemfilaactual3 = mui_empresas->item ( mui_empresas->currentRow(), 3 );
        setTipoEmpresa ( itemfilaactual3->text() );
        done ( 0 );
    } else
        mensajeInfo ( _( "Para entrar, antes tiene que seleccionar una empresa." ) );
    _depura ( "END abreempresaview::accept", 0 );
}


/// Evento que se dispara al hacer doble click sobre el listado de mui_empresas.
/**
**/
void abreempresaview::mui_empresasdobleclick()
{
    _depura ( "abreempresaview::mui_empresasdobleclick", 0 );
    accept();
    _depura ( "END abreempresaview::mui_empresasdobleclick", 0 );
}


/// Carga del archivo de mui_empresas las mui_empresas disponibles.
/**
\return
**/
void abreempresaview::cargaArchivo()
{
    _depura ( "abreempresaview::cargaArchivo", 0 );
    QString dir1 = confpr->valor ( CONF_DIR_USER ) + LISTEMPRESAS;

    /// Si el archivo no existe hacemos una recarga.
    _depura ( "Vamos a comprobar la existencia", 1 );
    /// Comprobamos la existencia del directorio personalizado de BulmaG&eacute;s. Y si no...
    if ( !QFile::exists ( dir1 ) )
        /// Hacemos una recarga de mui_empresas porque sabemos a que ha cambiado el listado.
        on_mui_actualizar_clicked();
    // end if

    QFile file ( dir1 );
    if ( !file.open ( QIODevice::ReadOnly | QIODevice::Text ) )
        return;
    // end if

    preparamui_empresas();
    QTextStream filestr ( &file );
    QString lineatexto, nombre, ano, nombd, tipo;

    while ( !filestr.atEnd() ) {
        /// Lee una l&iacute;nea completa del archivo.
        lineatexto = filestr.readLine();
        /// Separa los diferentes campos de la l&iacute;nea de texto y la asigna a las variables.
        QStringList listacampos = lineatexto.split ( "\t" );
        if ( listacampos.count() == 4 ) {
            nombre = listacampos[0];
            ano = listacampos[1];
            nombd = listacampos[2];
            tipo = listacampos[3];
            if ( tipo == m_tipo || m_tipo == "" )
                insertCompany ( nombre, ano, nombd, tipo );
            // end if
        } else {
            // TODO Si listacampos.count()!=4 tratar error
        } // end if
    } // end while
    file.close();
    _depura ( "END abreempresaview::cargaArchivo", 0 );
}


/// Guarda en el archivo de mui_empresas las mui_empresas disponibles
/// Tambi&eacute;n actualiza el listado de mui_empresas visibles.
/**
\return
**/
void abreempresaview::guardaArchivo()
{
    _depura ( "abreempresaview::guardaArchivo", 0 );
#ifndef WINDOWS
    QString dir1 = getenv ( "HOME" );
    dir1 = dir1 + "/.bulmages/" + LISTEMPRESAS;
#else
    QString dir1 = "C:\\bulmages\\" + LISTEMPRESAS;
#endif

    QFile file ( dir1 );
    if ( !file.open ( QIODevice::WriteOnly | QIODevice::Text ) )
        return;
    // end if

    /// Deshabilitamos las alertas para que no aparezcan avisos con bases de datos
    /// que no son del sistema.
    QString alertas = confpr->valor ( CONF_ALERTAS_DB );
    confpr->setValor ( CONF_ALERTAS_DB, "No" );
    /// Nos conectamos a la base de datos 'template1' para obtener un listado de todas
    /// las bases de datos existentes.
    BlPostgreSqlClient *db;
    db = new BlPostgreSqlClient();
    if ( db->inicializa ( QString ( "template1" ) ) )
        if ( db->inicializa ( QString ( "bulmafact" ) ) )
            if ( db->inicializa ( QString ( "bulmacont" ) ) )
                return;
    cursor2 *curs = db->cargacursor ( "SELECT datname FROM pg_database" );

    /// Preparamos el listado
    preparamui_empresas();

    /// Para cada base de datos nos intentamos conectanos y mirar de qu&eacute; tipo es.
    while ( !curs->eof() ) {
        trataEmpresa ( curs->valor ( "datname" ), &file );
        curs->siguienteregistro();
    } // end while
    delete curs;
    delete db;
    confpr->setValor ( CONF_ALERTAS_DB, alertas );
    file.close();
    _depura ( "END abreempresaview::guardaArchivo", 0 );
}


///
/**
\param empresa
\param file
\return
**/
void abreempresaview::trataEmpresa ( QString empresa, QFile *file )
{
    _depura ( "abreempresaview::trataEmpresa", 0, empresa );
    QTextStream filestr ( file );
    BlPostgreSqlClient *db1;
    QString nombre;
    QString nomdb = "";
    QString ano;
    QString tipo;
    if ( !empresa.startsWith ( "template" ) ) {
        db1 = new BlPostgreSqlClient();
        db1->inicializa ( empresa );
        try {
            cursor2 *cursa = db1->cargacursor ( "SELECT * FROM pg_tables WHERE tablename = 'configuracion'" );
            if ( !cursa ) return;
            if ( cursa->eof() ) {
                delete cursa;
                return;
            } // end if
            delete cursa;
            cursa = db1->cargacursor ( "SELECT * FROM configuracion WHERE nombre = 'Tipo'" );
            if ( !cursa ) return;
            if ( !cursa->eof() ) {
                tipo = cursa->valor ( "valor" );
                nomdb = empresa;
                delete cursa;
            } else {
                delete cursa;
                return;
            } // end if
            cursa = db1->cargacursor ( "SELECT * FROM configuracion WHERE nombre = 'NombreEmpresa'" );
            if ( !cursa ) return;
            if ( !cursa->eof() ) {
                nombre = cursa->valor ( "valor" );
                delete cursa;
            } else {
                delete cursa;
                return;
            } // end if
            cursa = db1->cargacursor ( "SELECT * FROM configuracion WHERE nombre = 'Ejercicio'" );
            if ( cursa ) {
                if ( !cursa->eof() )
                    ano = cursa->valor ( "valor" );
                // end if
                delete cursa;
            } // end if
        } catch ( ... ) {}
        if ( nomdb != "" ) {
            if ( tipo == m_tipo || m_tipo == "" )
                insertCompany ( nombre, ano, nomdb, tipo );
            // end if
            /// Independientemente de si deben mostrarse o no hay que guardarlas
            /// en el archivo.
            filestr << nombre.toAscii() << "\t";
            filestr << ano.toAscii() << "\t";
            filestr << nomdb.toAscii() << "\t";
            filestr << tipo.toAscii() << endl;
            nomdb = "";
        } // end if

        delete db1;
    } // end if
    _depura ( "END abreempresaview::trataEmpresa", 0, empresa );
}


/// Recarga la lista de mui_empresas haciendo las gestiones necesarias con el motor de
/// base de datos. Al mismo tiempo guarda el archivo de bases de datos en el
/// archivo 'LISTEMPRESAS'.
/**
**/
void abreempresaview::on_mui_actualizar_clicked()
{
    _depura ( "abreempresaview::on_mui_actualizar_clicked", 0 );
    guardaArchivo();
    _depura ( "END abreempresaview::on_mui_actualizar_clicked", 0 );
}


///
/**
**/
void abreempresaview::preparamui_empresas()
{
    _depura ( "abreempresaview::preparamui_empresas", 0 );
    mui_empresas->clear();
    mui_empresas->setRowCount ( 0 );
    mui_empresas->setColumnCount ( 4 );
    QStringList nombrecolumnas;
    nombrecolumnas << _( "Nombre" ) << _( "Anyo" ) << _( "Archivo" ) << _( "Tipo" );
    mui_empresas->setHorizontalHeaderLabels ( nombrecolumnas );
    mui_empresas->horizontalHeader() ->setResizeMode ( 0, QHeaderView::Stretch );

    mui_empresas->verticalHeader() ->hide();
    mui_empresas->setShowGrid ( FALSE );
    /// Si es el modo de facturaci&oacute;n ocultamos la fecha.
    if ( m_tipo == "BulmaFact" )
        mui_empresas->hideColumn ( ABRE_ANO );
    // end if
    _depura ( "END abreempresaview::preparamui_empresas", 0 );
}


///
/**
\param  obj
\param ev
\return
**/
bool abreempresaview::eventFilter ( QObject *obj, QEvent *ev )
{
    _depura ( "abreempresaview::eventFilter", 0 );
    if ( obj == mui_empresas ) {
        if ( ev->type() == QEvent::KeyPress ) {
            QKeyEvent * keyEvent = static_cast<QKeyEvent*> ( ev );
            switch ( keyEvent->key() ) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                /// Se comprueba que se ha seleccionado una entrada en la lista mui_empresas.
                if ( mui_empresas->currentRow() != -1 ) /// Entramos en el programa.
                    this->accept();
                // end if
                break;
            } // end switch
            _depura ( "END abreempresaview::eventFilter", 0, "Pulsacion de Tecla" );
            return false;
        } else {
            _depura ( "END abreempresaview::eventFilter", 0, "Evento sin interes" );
            return false;
        } // end if
    } // end if
    _depura ( "END abreempresaview::eventFilter", 0, "Objeto no deseado" );
    return QDialog::eventFilter ( obj, ev );
    // end if
}
