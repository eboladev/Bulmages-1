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

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMessageBox>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QHeaderView>
#include <QtGui/QKeyEvent>
#include <QtCore/QEvent>
#include <QtCore/QTextStream>
#include <QtCore/QTextCodec>

#include "blcompanydialog.h"
#include "blpostgresqlclient.h"
#include "blconfiguration.h"


/// Este es el archivo en el que se almacenan las mui_empresas que existen.
/// Es un archivo separado por tabuladores, que se suele alojar en
/// el directorio '.bulmages/' del home del usuario (perfil del usuario en Windows).
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
void BlCompanyDialog::setDbName ( QString pNomDB )
{
    BL_FUNC_DEBUG
    m_empresabd = pNomDB;
}


///
/**
\param pNomEmpresa
**/
void BlCompanyDialog::setCompanyName ( QString pNomEmpresa )
{
    BL_FUNC_DEBUG
    m_nombreempresa = pNomEmpresa;
}


///
/**
\param pTipoEmpresa
**/
void BlCompanyDialog::setCompanyType ( QString pTipoEmpresa )
{
    BL_FUNC_DEBUG
    m_tipoempresa = pTipoEmpresa;
}


///
/**
\return
**/
bool BlCompanyDialog::isDestroyMode()
{
    BL_FUNC_DEBUG
    return m_modo;
}


///
/**
\param pModo
**/
void BlCompanyDialog::setDestroyMode ( bool pModo )
{
    BL_FUNC_DEBUG
    m_modo = pModo;
}


///
/**
\return
**/
QString BlCompanyDialog::dbName()
{
    BL_FUNC_DEBUG
    return m_empresabd;
}


///
/**
\return
**/
QString BlCompanyDialog::companyName()
{
    BL_FUNC_DEBUG
    return m_nombreempresa;
}


///
/**
\return
**/
QString BlCompanyDialog::companyType()
{
    BL_FUNC_DEBUG
    return m_tipoempresa;
}


///
/**
**/
void BlCompanyDialog::on_mui_aceptar_clicked()
{
    BL_FUNC_DEBUG
    accept();
}


///
/**
**/
void BlCompanyDialog::on_mui_cancelar_clicked()
{
    BL_FUNC_DEBUG
    if ( !isDestroyMode() )
        exit ( 1 );
    else
        done ( 1 );
}


///
/**
\return
**/
bool BlCompanyDialog::close ( bool )
{
    BL_FUNC_DEBUG
    on_mui_cancelar_clicked();
    
    return true;
}


/// Inicia las variables m_tipo y m_tipoempresa y carga el archivo para
/// hacer la presentaci&oacute;n.
/// Llama a la funci&oacute;n 'loadFile'.
/// @param parent La ventana que hace la llamada.
/// @param tipo Cadena de texto que indica si es contabilidad o facturaci&oacute;n.
/// @param name Nombre de la ventana.
/// @param modal Indica si la ventana debe comportar de forma 'modal' o no. (por defecto s&iacute;).
BlCompanyDialog::BlCompanyDialog ( QWidget *parent, QString tipo, const char *name, bool modal )
        : QDialog ( parent )
{
    BL_FUNC_DEBUG
    setModal ( modal );
    this->setWindowTitle ( name );
    setupUi ( this );
    blCenterOnScreen ( this );

    mui_empresas->installEventFilter ( this );
    QObject::connect ( mui_empresas, SIGNAL ( itemDoubleClicked ( QTableWidgetItem * ) ), this,
                       SLOT ( mui_empresasdobleclick() ) );

    m_tipo = tipo;
    setCompanyType ( "" );
    setDestroyMode ( false );
    /// Si el m_modo es false (salir del programa si se cancela la ventana) entonces se pone
    /// un t&iacute;tulo adecuado al bot&oacute;n.
    if ( !isDestroyMode() )
        mui_cancelar->setText ( _ ( "&Cierra el programa" ) );
    // end if
    loadFile();
    /// Seleccionar la primera empresa de la lista inicialmente, de este modo,
    /// si s&oacute;lo hay una empresa o se quiere entrar a la primera bastar&aacute; con pulsar la tecla intro.
    mui_empresas->selectRow(0);
    
}


///
/**
**/
BlCompanyDialog::~BlCompanyDialog()
{
    BL_FUNC_DEBUG
}


/// Inserta una compa&ntilde;ia en el 'QList' mui_empresas definido en el di&aacute;logo.
/// Crea un objeto QListViewItem para la QListView 'mui_empresas' y rellena sus
/// columnas con los datos pasados al m&eacute;todo.
/// @param nombre Nombre de la empresa.
/// @param ano Ejercicio de la empresa (aunque pueden ser varios).
/// @param archivo Nombre de la base de datos.
/// @param tipo Tipo de base de datos (BulmaCont o BulmaFact).
void BlCompanyDialog::insertCompany ( QString nombre, QString ano, QString archivo,
                                      QString tipo )
{
    BL_FUNC_DEBUG
    mui_empresas->insertRow ( mui_empresas->rowCount() );
    QTableWidgetItem *nuevoItemNombre = new QTableWidgetItem ( nombre );
    mui_empresas->setItem ( mui_empresas->rowCount() - 1, 0, nuevoItemNombre );
    QTableWidgetItem *nuevoItemAno = new QTableWidgetItem ( ano );
    mui_empresas->setItem ( mui_empresas->rowCount() - 1, 1, nuevoItemAno );
    QTableWidgetItem *nuevoItemArchivo = new QTableWidgetItem ( archivo );
    mui_empresas->setItem ( mui_empresas->rowCount() - 1, 2, nuevoItemArchivo );
    QTableWidgetItem *nuevoItemTipo = new QTableWidgetItem ( tipo );
    mui_empresas->setItem ( mui_empresas->rowCount() - 1, 3, nuevoItemTipo );
    
}


/// Se ha pulsado sobre el bot&oacute;n de aceptar con lo que iniciamos la variables y
/// cerramos esta ventana ya que ha cumplido con su cometido.
/**
**/
void BlCompanyDialog::accept()
{
    BL_FUNC_DEBUG
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
            ( mui_empresas->isItemSelected ( mui_empresas->item ( mui_empresas->currentRow(), 0 ) ) == true ) ) {
        itemfilaactual0 = mui_empresas->item ( mui_empresas->currentRow(), 0 );
        setCompanyName ( itemfilaactual0->text() );
        itemfilaactual2 = mui_empresas->item ( mui_empresas->currentRow(), 2 );
        setDbName ( itemfilaactual2->text() );
        itemfilaactual3 = mui_empresas->item ( mui_empresas->currentRow(), 3 );
        setCompanyType ( itemfilaactual3->text() );
        done ( 0 );
    } else
        blMsgInfo ( _ ( "Para entrar, antes tiene que seleccionar una empresa." ) );
    
}


/// Evento que se dispara al hacer doble click sobre el listado de mui_empresas.
/**
**/
void BlCompanyDialog::mui_empresasdobleclick()
{
    BL_FUNC_DEBUG
    accept();
    
}


/// Carga del archivo de mui_empresas las mui_empresas disponibles.
/**
\return
**/
void BlCompanyDialog::loadFile()
{
    BL_FUNC_DEBUG
    mui_empresas->setSortingEnabled(false);
    QString dir1 = g_confpr->value( CONF_DIR_USER ) + LISTEMPRESAS;

    /// Si el archivo no existe hacemos una recarga.
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
    mui_empresas->setSortingEnabled(true);
    
}


/// Guarda en el archivo de mui_empresas las mui_empresas disponibles
/// Tambi&eacute;n actualiza el listado de mui_empresas visibles.
/**
\return
**/
void BlCompanyDialog::saveFile()
{
    BL_FUNC_DEBUG

#ifndef Q_OS_WIN32
    QString dirusuario = getenv ( "HOME" );
#else
    QString dirusuario = getenv ( "UserProfile" );
#endif
    QString dir1 = dirusuario + "/.bulmages/" + LISTEMPRESAS;

    QFile file ( dir1 );
    if ( !file.open ( QIODevice::WriteOnly | QIODevice::Text ) )
        return;

    /// Deshabilitamos las alertas para que no aparezcan avisos con bases de datos
    /// que no son del sistema.
    QString alertas = g_confpr->value( CONF_ALERTAS_DB );
    g_confpr->setValue ( CONF_ALERTAS_DB, "No" );
    /// Nos conectamos a la base de datos 'template1' para obtener un listado de todas
    /// las bases de datos existentes.
    BlPostgreSqlClient *db;
    db = new BlPostgreSqlClient();
    if ( db->inicializa ( QString ( "template1" ) ) )
        if ( db->inicializa ( QString ( "bulmafact" ) ) )
            if ( db->inicializa ( QString ( "bulmacont" ) ) )
                return;
    BlDbRecordSet *curs = db->loadQuery ( "SELECT datname FROM pg_database" );

    /// Preparamos el listado
    preparamui_empresas();

    /// Para cada base de datos nos intentamos conectanos y mirar de qu&eacute; tipo es.
    while ( !curs->eof() ) {
        findCompanyDatabase ( curs->value( "datname" ), &file );
        curs->nextRecord();
    } // end while
    delete curs;
    delete db;
    g_confpr->setValue ( CONF_ALERTAS_DB, alertas );
    file.close();
}


///
/**
\param empresa
\param file
\return
**/
void BlCompanyDialog::findCompanyDatabase ( QString empresa, QFile *file )
{
    BL_FUNC_DEBUG
    QTextStream filestr ( file );
    BlPostgreSqlClient *db1;
    QString nombre;
    QString nomdb = "";
    QString ano;
    QString tipo;
    mui_empresas->setSortingEnabled(false);
    if ( !empresa.startsWith ( "template" ) ) {
        db1 = new BlPostgreSqlClient();
        db1->inicializa ( empresa );
        try {
            BlDbRecordSet *cursa = db1->loadQuery ( "SELECT * FROM pg_tables WHERE tablename = 'configuracion'" );
            if ( !cursa ) return;
            if ( cursa->eof() ) {
                delete cursa;
                return;
            } // end if
            delete cursa;
            cursa = db1->loadQuery ( "SELECT * FROM configuracion WHERE nombre = 'Tipo'" );
            if ( !cursa ) return;
            if ( !cursa->eof() ) {
                tipo = cursa->value( "valor" );
                nomdb = empresa;
                delete cursa;
            } else {
                delete cursa;
                return;
            } // end if
            cursa = db1->loadQuery ( "SELECT * FROM configuracion WHERE nombre = 'NombreEmpresa'" );
            if ( !cursa ) return;
            if ( !cursa->eof() ) {
                nombre = cursa->value( "valor" );
                delete cursa;
            } else {
                delete cursa;
                return;
            } // end if
            cursa = db1->loadQuery ( "SELECT * FROM configuracion WHERE nombre = 'Ejercicio'" );
            if ( cursa ) {
                if ( !cursa->eof() )
                    ano = cursa->value( "valor" );
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
            filestr << nombre.toLatin1() << "\t";
            filestr << ano.toLatin1() << "\t";
            filestr << nomdb.toLatin1() << "\t";
            filestr << tipo.toLatin1() << endl;
            nomdb = "";
        } // end if

        delete db1;
    } // end if
    mui_empresas->setSortingEnabled(true);
    
}


/// Recarga la lista de mui_empresas haciendo las gestiones necesarias con el motor de
/// base de datos. Al mismo tiempo guarda el archivo de bases de datos en el
/// archivo 'LISTEMPRESAS'.
/**
**/
void BlCompanyDialog::on_mui_actualizar_clicked()
{
    BL_FUNC_DEBUG
    saveFile();
    
}


///
/**
**/
void BlCompanyDialog::preparamui_empresas()
{
    BL_FUNC_DEBUG
    mui_empresas->clear();
    mui_empresas->setRowCount ( 0 );
    mui_empresas->setColumnCount ( 4 );
    QStringList nombrecolumnas;
    nombrecolumnas << _ ( "Nombre" ) << _ ( "Anyo" ) << _ ( "Archivo" ) << _ ( "Tipo" );
    mui_empresas->setHorizontalHeaderLabels ( nombrecolumnas );
    mui_empresas->horizontalHeader() ->setSectionResizeMode ( 0, QHeaderView::Stretch );

    mui_empresas->verticalHeader() ->hide();
    mui_empresas->setShowGrid ( false );
    /// Si es el modo de facturaci&oacute;n ocultamos la fecha.
    if ( m_tipo == "BulmaFact" )
        mui_empresas->hideColumn ( ABRE_ANO );    
}


///
/**
\param  obj
\param ev
\return
**/
bool BlCompanyDialog::eventFilter ( QObject *obj, QEvent *ev )
{
    BL_FUNC_DEBUG
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
        } // end if
        return false;
    } // end if
    
    return QDialog::eventFilter ( obj, ev );
}
