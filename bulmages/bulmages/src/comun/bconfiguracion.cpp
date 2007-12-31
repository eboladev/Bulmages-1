/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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

#include <unistd.h>
#include <errno.h>

#include <QMessageBox>
#include <QTabWidget>
#include <QFile>

#include "bconfiguracion.h"
#include "bnuevaempresa.h"
#include "nuevafact.h"
#include "funcaux.h"

#ifdef WIN32
#include <process.h>
#endif

#ifndef WIN32
#include <map>
#include <set>
#endif

///
/**
\param ref
\param parent
\param f
**/
BConfiguracion::BConfiguracion ( BSelector *ref, QWidget *parent, Qt::WFlags f = 0 )
        : QDialog ( parent, f )
{
    _depura ( "BConfiguracion::BConfiguracion", 0 );
    setupUi ( this );

    QObject::connect ( pushButtonF_X, SIGNAL ( clicked() ), this, SLOT ( cerrar() ) );
    QObject::connect ( pushButtonA_1_2, SIGNAL ( clicked() ), this, SLOT ( nuevaFacturacion() ) );
    QObject::connect ( pushButton32, SIGNAL ( clicked() ), this, SLOT ( BotonContaplus() ) );
    QObject::connect ( pushButtonA_1, SIGNAL ( clicked() ), this, SLOT ( nuevaEmpresa() ) );
    QObject::connect ( m_backup, SIGNAL ( clicked() ), this, SLOT ( salvarEmpresa() ) );
    QObject::connect ( pushButton32_2, SIGNAL ( clicked() ), this, SLOT ( s_importexportbulmafact() ) );

    PunteroAlSelector = ref;
    if ( PunteroAlSelector->tipoEmpresa() == "BulmaCont" ) {
        cargarFichaBulmages();
        m_tab->setTabEnabled ( 1, FALSE );
        m_tab->setTabEnabled ( 2, FALSE );
        m_tab->setTabEnabled ( 3, FALSE );
    } // end if
    if ( PunteroAlSelector->tipoEmpresa() == "BulmaFact" ) {
        cargarFichaBulmages();
        m_tab->setTabEnabled ( 0, FALSE );
        m_tab->setTabEnabled ( 2, FALSE );
        m_tab->setTabEnabled ( 3, FALSE );
    } // end if
    cargarFichaUsuarios();
    tablaconfiguracion();
    _depura ( "END BConfiguracion::BConfiguracion", 0 );
}


///
/**
**/
BConfiguracion::~BConfiguracion()
{
    _depura ( "BConfiguracion::~BConfiguracion", 0 );
    _depura ( "END BConfiguracion::~BConfiguracion", 0 );
}


///
/**
**/
void BConfiguracion::cerrar()
{
    _depura ( "BConfiguracion::cerrar", 0 );
    /// Guardamos la configuracion.
    for ( int i = 0; i < 1000; i++ ) {
        if ( confpr->nombre ( i ) != "" ) {
            confpr->setValor ( i, m_tconfiguracion->item ( i, 1 ) ->text().toAscii().constData() );
        } // end if
    } // end for

    confpr->saveconfig();
    //traductor->load(archivo.c_str(), confpr->valor(CONF_DIR_TRADUCCION).c_str());
    //Cargo la nueva fuente
    close();
    _depura ( "END BConfiguracion::cerrar", 0 );
}


/// Esta funcion rellena la tabla de configuracion del programa.
/// Para luego poder guardar los parametros que haga falta.
/**
**/
void BConfiguracion::tablaconfiguracion()
{
    _depura ( "BConfiguracion::tablaconfiguracion", 0 );
    QTableWidgetItem *item0;
    QTableWidgetItem *item1;
    QTableWidgetItem *item2;
    m_tconfiguracion->setColumnCount ( 3 );
    m_tconfiguracion->hideColumn ( 2 );
    m_tconfiguracion->setRowCount ( 1000 );
    QString cad;
    for ( int i = 0; i < 1000; i++ ) {
        cad = cad.setNum ( i );
        item0 = new QTableWidgetItem ( confpr->nombre ( i ) );
        item1 = new QTableWidgetItem ( confpr->valor ( i ) );
        item2 = new QTableWidgetItem ( cad );
        m_tconfiguracion->setItem ( i, 0, item0 );
        m_tconfiguracion->setItem ( i, 1, item1 );
        m_tconfiguracion->setItem ( i, 2, item2 );
        if ( confpr->nombre ( i ) == "" )
            m_tconfiguracion->hideRow ( i );
    } // end for
    m_tconfiguracion->resizeColumnToContents ( 0 );
    m_tconfiguracion->resizeColumnToContents ( 1 );
    _depura ( "END BConfiguracion::tablaconfiguracion", 0 );
}


///
/**
**/
void BConfiguracion::cargarFichaBulmages()
{
    _depura ( "BConfiguracion::cargarFichaBulmages", 0 );
    lineEditA_1->setText ( PunteroAlSelector->nombreempresa->text() );
    _depura ( "END BConfiguracion::cargarFichaBulmages", 0 );
}


///
/**
**/
void BConfiguracion::FontChanged ( const QString & )
{
    _depura ( "BConfiguracion::FontChanged", 0 );
    _depura ( "END BConfiguracion::FontChanged", 0 );
}


///
/**
**/
void BConfiguracion::FontSizeChanged ( int )
{
    _depura ( "BConfiguracion::FontSizeChanged", 0 );
    _depura ( "END BConfiguracion::FontSizeChanged", 0 );
}


///
/**
**/
void BConfiguracion::BotonA_10aceptar()
{
    _depura ( "BConfiguracion::BotonA_10aceptar", 0 );
    _depura ( "END BConfiguracion::BotonA_10aceptar", 0 );
}


#include "importContaplus.h"


/// Responde a la pusacion de importar datos de Contaplus a BulmaGes.
/**
**/
void BConfiguracion::BotonContaplus()
{
    _depura ( "BConfiguracion::BotonContaplus", 0 );
    postgresiface2 *DBconn = new postgresiface2();
    DBconn->inicializa ( PunteroAlSelector->empresaDB() );
    importContaplus *import = new importContaplus ( DBconn, 0, 0 );
    import->exec();
    delete import;
    delete DBconn;
    _depura ( "END BConfiguracion::BotonContaplus", 0 );
}


#include "importexportbulmafactview.h"


/// Responde a la pusacion de importar datos de Contaplus a BulmaGes.
/**
**/
void BConfiguracion::s_importexportbulmafact()
{
    _depura ( "BConfiguracion::s_importexportbulmafact", 0 );
    postgresiface2 *DBconn = new postgresiface2();
    DBconn->inicializa ( PunteroAlSelector->empresaDB() );
    ImportExportBulmafactView *import = new ImportExportBulmafactView ( DBconn, 0, 0 );
    import->exec();
    delete import;
    delete DBconn;
    _depura ( "END BConfiguracion::s_importexportbulmafact", 0 );
}


///
/**
**/
void BConfiguracion::BotonA_11rechazar()
{
    _depura ( "BConfiguracion::BotonA_11rechazar", 0 );
    /// Poner el comboBoxFuente y el comboBoxIdioma a sus valores anteriores.
    if ( PunteroAlSelector->tipoEmpresa() == "BulmaCont" ) {
        cargarFichaBulmages();
    } // end if
    _depura ( "END BConfiguracion::BotonA_11rechazar", 0 );
}


/// Esta funcion se ejecuta cuando se lanza el cambio de nombre de la empresa.
/**
**/
void BConfiguracion::BotonA_6nombreEmpresa()
{
    _depura ( "BConfiguracion::BotonA_6nombreEmpresa", 0 );
    /// Activa el line edit para que pueda ser editado.
    if ( lineEditA_1->isReadOnly() ) {
        lineEditA_1->setReadOnly ( false );
    } else { /// Y guarda el nuevo nombre de empresa.
        lineEditA_1->setReadOnly ( true );
        PunteroAlSelector->nombreempresa->setText ( lineEditA_1->text() );
    } // end if
    _depura ( "BConfiguracion::BotonA_6nombreEmpresa", 0 );
}


/// Aqui abrimos el cuador de dialogo que nos permite crear una empresa nueva basada en la
/// BASE DE DATOS bgplangcont.
/**
**/
void BConfiguracion::nuevaEmpresa()
{
    _depura ( "BConfiguracion::nuevaEmpresa", 0 );
    BNuevaEmpresa *n = new BNuevaEmpresa ( this );
    n->exec();
    delete n;
    _depura ( "END BConfiguracion::nuevaEmpresa", 0 );
}


/// Aqui abrimos el cuadro de dialogo que nos permite crear una empresa nueva basada en la
/// BASE DE DATOS bgplangcont.º
/**
**/
void BConfiguracion::nuevaFacturacion()
{
    _depura ( "BConfiguracion::nuevaFacturacion", 0 );
    nuevafact *n = new nuevafact ( this );
    n->exec();
    delete n;
    _depura ( "END BConfiguracion::nuevaFacturacion", 0 );
}


/// Aqui creamos una nueva empresa que es una copia exacta de otra empresa que ya existe.
/**
**/
void BConfiguracion::BotonA_61clonarEmpresa()
{
    _depura ( "BConfiguracion::BotonA_61clonarEmpresa", 0 );
    QString dbEmpresa;
    if ( dbEmpresa != "" ) {
        QMessageBox::information ( this, "Debug", "Clonando la base de datos: " + dbEmpresa + "\n\nProceso no disponible...\n", QMessageBox::Ok );
    } // end if
    _depura ( "END BConfiguracion::BotonA_61clonarEmpresa", 0 );
}


/// Aqui borramos una empresa entera. No nos permite borrar la base de datos bgplangcont
/// ni la base de datos de la empresa que tengamos abierta en este momento.
/// \todo Solo borra bases de datos en local, con bases de datos remotas no funciona.
/**
**/
void BConfiguracion::borrarEmpresa()
{
    _depura ( "BConfiguracion::borrarEmpresa", 0 );
    QString dbEmpresa;
    QString nombreEmpresa;
    QString idEmpresa;
    QString ejercicio;
    /// Siempre se borra la empresa actual.
    dbEmpresa = PunteroAlSelector->empresaDB();
    if ( dbEmpresa != "" ) {
        if ( dbEmpresa == "bgplangcont" ) {
            QMessageBox::information ( this, tr ( "Atencion" ), tr ( "Esta base de datos no puede ser eliminada.\n\r Es la plantilla para generar nuevas empresas." ), QMessageBox::Ok );
        } else if ( dbEmpresa != PunteroAlSelector->empresaDB() ) {
            QMessageBox::warning ( this, tr ( "Atencion" ), tr ( "No esta permitido eliminar la base \n\r de datos actualmente abierta." ), QMessageBox::Ok, 0 );
        } else {
            int mensaje = QMessageBox::warning ( this, tr ( "Atencion" ), tr ( "Borrar una empresa puede suponer perdida de datos\n Desea continuar?\n" ), QMessageBox::Yes, QMessageBox::No, 0 );
            if ( mensaje == QMessageBox::Yes ) {
                QString sentencia = "dropdb " + dbEmpresa;
                system ( sentencia.toAscii().constData() );
                /// Hacemos una recarga de empresas porque sabemos a ciencia cierta que
                /// ha cambiado el listado.
                abreempresaview *abre = new abreempresaview ( NULL, "hola", "hola" );
                abre->on_mui_actualizar_clicked();
                delete abre;
                done ( 1 );
            } // end if
        } // end if
    } // end if
    close();
    _depura ( "END BConfiguracion::borrarEmpresa", 0 );
}


/// Creamos una copia de seguridad de una base de datos.
/**
**/
void BConfiguracion::salvarEmpresa()
{
    _depura ( "BConfiguracion::salvarEmpresa", 0 );
    QString dbEmpresa;
    QString PGserver;
    //  PGserver = "-h ";

    PGserver = confpr->valor ( CONF_SERVIDOR ).toAscii().constData();
    dbEmpresa = PunteroAlSelector->empresaDB();
    fprintf ( stderr, "Vamos a guardar la empresa %s\n", dbEmpresa.toAscii().constData() );

    if ( dbEmpresa != "" ) {
        QString fn = QFileDialog::getSaveFileName ( this, tr ( "Guardar empresa" ),
                     confpr->valor ( CONF_DIR_USER ),
                     tr ( "Empresas (*.pgdump)" ) );

        if ( !fn.isEmpty() ) {
            if ( fn.right ( 7 ) != ".pgdump" )
                fn = fn + ".pgdump";
            fprintf ( stderr, "Vamos a guardar la empresa en el fichero %s\n", fn.toAscii().constData() );
            QString cadena;
            cadena.sprintf ( "pg_dump %s > %s",  dbEmpresa.toAscii().constData(), fn.toAscii().constData() );
            fprintf ( stderr, "%s\n", cadena.toAscii().constData() );
            system ( cadena.toAscii().constData() );
        } // end if
    } // end if
    _depura ( "END BConfiguracion::salvarEmpresa", 0 );
}


/// Restauramos una copia de seguridad de una base de datos.
/// Para cargar la empresa debe estar sin ningun usuario dentro de ella.
/**
**/
void BConfiguracion::restaurarEmpresa()
{
    _depura ( "BConfiguracion::restaurarEmpresa", 0 );
    QString dbEmpresa;
    QString PGserver;
    PGserver = confpr->valor ( CONF_SERVIDOR ).toAscii().constData();
    QString usuario;

    dbEmpresa = PunteroAlSelector->empresaDB();
    if ( dbEmpresa != "" ) {
        QString fn = QFileDialog::getOpenFileName ( this, tr ( "Cargar empresa" ),
                     confpr->valor ( CONF_DIR_USER ),
                     tr ( "Empresas (*.pgdump)" ) );

        if ( !fn.isEmpty() ) {

            QString comando = "dropdb " + dbEmpresa;
            system ( comando.toAscii().constData() );
            comando = "createdb " + dbEmpresa;
            system ( comando.toAscii().constData() );
            comando = "psql " + dbEmpresa + "< " + fn;
            system ( comando.toAscii().constData() );
        } // end if
    } // end if
    _depura ( "END BConfiguracion::restaurarEmpresa", 0 );
}


///
/**
**/
void BConfiguracion::cargarFichaUsuarios()
{
    _depura ( "BConfiguracion::cargarFichaUsuarios", 0 );
    _depura ( "END BConfiguracion::cargarFichaUsuarios", 0 );
}


///
/**
**/
void BConfiguracion::listView1_currentChanged ( QListWidgetItem * )
{
    _depura ( "BConfiguracion::listView1_currentChanged", 0 );
#ifndef WIN32
#endif
    _depura ( "END BConfiguracion::listView1_currentChanged", 0 );
}


/// Salvamos los usuarios en la base de datos.
/**
**/
void BConfiguracion::BotonB_1Aplicar()
{
    _depura ( "BConfiguracion::BotonB_1Aplicar", 0 );
    _depura ( "END BConfiguracion::BotonB_1Aplicar", 0 );
}


/// Desacemos los cambios que hemos hecho (UNDO).
/**
**/
void BConfiguracion::BotonB_2Desacer()
{
    _depura ( "BConfiguracion::BotonB_2Desacer", 0 );
    cargarFichaUsuarios();
    _depura ( "END BConfiguracion::BotonB_2Desacer", 0 );
}


///
/**
**/
void BConfiguracion::listView2_clickBotonDerecho ( QListWidgetItem *, const QPoint&, int )
{
    _depura ( "BConfiguracion::listView2_clickBotonDerecho", 0 );
    _depura ( "END BConfiguracion::listView2_clickBotonDerecho", 0 );
}


/// Por conveniencia (Bug QT??).
/**
**/
void BConfiguracion::listiView2_clickMouse ( int, QListWidgetItem *, const QPoint&, int )
{
    _depura ( "BConfiguracion::listiView2_clickMouse", 0 );
    _depura ( "END BConfiguracion::listiView2_clickMouse", 0 );
}


///
/**
**/
void BConfiguracion::on_mui_restaurarc_clicked()
{
    _depura ( "BConfiguracion::on_mui_restaurarc_clicked", 0 );
    restaurarEmpresa();
    _depura ( "END BConfiguracion::on_mui_restaurarc_clicked", 0 );
}


///
/**
**/
void BConfiguracion::on_mui_restaurarf_clicked()
{
    _depura ( "BConfiguracion::on_mui_restaurarf_clicked", 0 );
    restaurarEmpresa();
    _depura ( "END BConfiguracion::on_mui_restaurarf_clicked", 0 );
}


///
/**
**/
void BConfiguracion::on_mui_borrarempresabc_clicked()
{
    _depura ( "BConfiguracion::on_mui_borrarempresabc_clicked", 0 );
    borrarEmpresa();
    _depura ( "END BConfiguracion::on_mui_borrarempresabc_clicked", 0 );
}


///
/**
**/
void BConfiguracion::on_mui_borrarempresabf_clicked()
{
    _depura ( "BConfiguracion::on_mui_borrarempresabf_clicked", 0 );
    borrarEmpresa();
    _depura ( "END BConfiguracion::on_mui_borrarempresabf_clicked", 0 );
}
