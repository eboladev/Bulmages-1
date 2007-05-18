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

#ifdef WIN32
#include <process.h>
#endif

#ifndef WIN32
#include <map>
#include <set>
#endif


BConfiguracion::BConfiguracion(BSelector *ref, QWidget *parent, Qt::WFlags f = 0)
        : QDialog(parent, f) {
    setupUi(this);

    QObject::connect(pushButtonF_X, SIGNAL(clicked()), this, SLOT(cerrar()));
    QObject::connect(pushButtonA_1_2, SIGNAL(clicked()), this, SLOT(nuevaFacturacion()));
    QObject::connect(pushButton32, SIGNAL(clicked()), this, SLOT(BotonContaplus()));
    QObject::connect(m_borrarempresa, SIGNAL(clicked()), this, SLOT(borrarEmpresa()));
    QObject::connect(pushButtonA_1, SIGNAL(clicked()), this, SLOT(nuevaEmpresa()));
    QObject::connect(m_backup, SIGNAL(clicked()), this, SLOT(salvarEmpresa()));
    QObject::connect(pushButton32_2, SIGNAL(clicked()), this, SLOT(s_importexportbulmafact()));

    PunteroAlSelector = ref;
    if (PunteroAlSelector->tipoEmpresa() == "BulmaCont") {
        cargarFichaBulmages();
        m_tab->setTabEnabled(1, FALSE);
        m_tab->setTabEnabled(2, FALSE);
        m_tab->setTabEnabled(3, FALSE);
    } // end if
    if (PunteroAlSelector->tipoEmpresa() == "BulmaFact") {
        cargarFichaBulmages();
        m_tab->setTabEnabled(0, FALSE);
        m_tab->setTabEnabled(2, FALSE);
        m_tab->setTabEnabled(3, FALSE);
    } // end if
    cargarFichaUsuarios();
    tablaconfiguracion();
}


BConfiguracion::~BConfiguracion() {}


void BConfiguracion::cerrar() {
    /// Guardamos la configuracion.
    for (int i = 0; i < 1000; i++) {
        if (confpr->nombre(i) != "") {
            confpr->setValor(i, m_tconfiguracion->item(i, 1)->text().toAscii().constData());
        } // end if
    } // end for

    confpr->saveconfig();
    //traductor->load(archivo.c_str(), confpr->valor(CONF_DIR_TRADUCCION).c_str());
    //Cargo la nueva fuente
    close();
}


/// Esta funcion rellena la tabla de configuracion del programa.
/// Para luego poder guardar los parametros que haga falta.
void BConfiguracion::tablaconfiguracion() {
    QTableWidgetItem *item0;
    QTableWidgetItem *item1;
    QTableWidgetItem *item2;
    m_tconfiguracion->setColumnCount(3);
    m_tconfiguracion->hideColumn(2);
    m_tconfiguracion->setRowCount(1000);
    QString cad;
    for (int i = 0; i < 1000; i++) {
        cad = cad.setNum(i);
        item0 = new QTableWidgetItem(confpr->nombre(i));
        item1 = new QTableWidgetItem(confpr->valor(i));
        item2 = new QTableWidgetItem(cad);
        m_tconfiguracion->setItem(i, 0, item0);
        m_tconfiguracion->setItem(i, 1, item1);
        m_tconfiguracion->setItem(i, 2, item2);
        if (confpr->nombre(i) == "")
            m_tconfiguracion->hideRow(i);
    } // end for
    m_tconfiguracion->resizeColumnToContents(0);
    m_tconfiguracion->resizeColumnToContents(1);
}


void BConfiguracion::cargarFichaBulmages() {
    lineEditA_1->setText(PunteroAlSelector->nombreempresa->text());
}


void BConfiguracion::FontChanged(const QString &) {}


void BConfiguracion::FontSizeChanged(int) {}


void BConfiguracion::BotonA_10aceptar() {}


#include "importContaplus.h"


/// Responde a la pusacion de importar datos de Contaplus a BulmaGes.
void BConfiguracion::BotonContaplus() {
    postgresiface2 *DBconn = new postgresiface2();
    DBconn->inicializa(PunteroAlSelector->empresaDB());
    importContaplus *import = new importContaplus(DBconn, 0, 0);
    import->exec();
    delete import;
    delete DBconn;
}


#include "importexportbulmafactview.h"


/// Responde a la pusacion de importar datos de Contaplus a BulmaGes.
void BConfiguracion::s_importexportbulmafact() {
    postgresiface2 *DBconn = new postgresiface2();
    DBconn->inicializa(PunteroAlSelector->empresaDB());
    ImportExportBulmafactView *import= new ImportExportBulmafactView(DBconn, 0, 0);
    import->exec();
    delete import;
    delete DBconn;
}


void BConfiguracion::BotonA_11rechazar() {
    /// Poner el comboBoxFuente y el comboBoxIdioma a sus valores anteriores.
    if (PunteroAlSelector->tipoEmpresa() == "BulmaCont") {
        cargarFichaBulmages();
    } // end if
}


/// Esta funcion se ejecuta cuando se lanza el cambio de nombre de la empresa.
void BConfiguracion::BotonA_6nombreEmpresa() {
    /// Activa el line edit para que pueda ser editado.
    if (lineEditA_1->isReadOnly() ) {
        lineEditA_1->setReadOnly(false);
    } else { /// Y guarda el nuevo nombre de empresa.
        lineEditA_1->setReadOnly(true);
        PunteroAlSelector->nombreempresa->setText(lineEditA_1->text());
    } // end if
}


/// Aqui abrimos el cuador de dialogo que nos permite crear una empresa nueva basada en la
/// BASE DE DATOS bgplangcont.
void BConfiguracion::nuevaEmpresa() {
    BNuevaEmpresa *n = new BNuevaEmpresa(this);
    n->exec();
    delete n;
}


/// Aqui abrimos el cuadro de dialogo que nos permite crear una empresa nueva basada en la
/// BASE DE DATOS bgplangcont.º
void BConfiguracion::nuevaFacturacion() {
    nuevafact *n = new nuevafact(this);
    n->exec();
    delete n;
}


/// Aqui creamos una nueva empresa que es una copia exacta de otra empresa que ya existe.
void BConfiguracion::BotonA_61clonarEmpresa() {
    QString dbEmpresa;
    if (dbEmpresa != "") {
        QMessageBox::information(this, "Debug", "Clonando la base de datos: " + dbEmpresa + "\n\nProceso no disponible...\n", QMessageBox::Ok);
    } // end if
}


/// Aqui borramos una empresa entera. No nos permite borrar la base de datos bgplangcont
/// ni la base de datos de la empresa que tengamos abierta en este momento.
/// \todo Solo borra bases de datos en local, con bases de datos remotas no funciona.
void BConfiguracion::borrarEmpresa() {
    QString dbEmpresa;
    QString nombreEmpresa;
    QString idEmpresa;
    QString ejercicio;
    /// Siempre se borra la empresa actual.
    dbEmpresa = PunteroAlSelector->empresaDB();
    if (dbEmpresa != "") {
        if (dbEmpresa == "bgplangcont") {
            QMessageBox::information(this, tr("Atencion"), tr("Esta base de datos no puede ser eliminada.\n\r Es la plantilla para generar nuevas empresas."), QMessageBox::Ok);
        } else if (dbEmpresa != PunteroAlSelector->empresaDB()) {
            QMessageBox::warning(this, tr("Atencion"), tr("No esta permitido eliminar la base \n\r de datos actualmente abierta."), QMessageBox::Ok, 0);
        } else {
            int mensaje = QMessageBox::warning(this, tr("Atencion"), tr("Borrar una empresa puede suponer perdida de datos\n Desea continuar?\n"), QMessageBox::Yes,QMessageBox::No, 0);
            if (mensaje == QMessageBox::Yes) {
                QString sentencia = "dropdb " + dbEmpresa;
                system(sentencia.toAscii().constData());
                /// Hacemos una recarga de empresas porque sabemos a ciencia cierta que
                /// ha cambiado el listado.
                abreempresaview *abre = new abreempresaview(NULL, "hola", "hola");
                abre->on_mui_actualizar_clicked();
                delete abre;
                done(1);
            } // end if
        } // end if
    } // end if
    close();
}


/// Creamos una copia de seguridad de una base de datos.
void BConfiguracion::salvarEmpresa() {
    QString dbEmpresa;
    QString PGserver;
    //  PGserver = "-h ";

    PGserver = confpr->valor(CONF_SERVIDOR).toAscii().constData();
    dbEmpresa = PunteroAlSelector->empresaDB();
    fprintf(stderr, "Vamos a guardar la empresa %s\n", dbEmpresa.toAscii().constData());

    if (dbEmpresa != "") {
        QString fn = QFileDialog::getSaveFileName(this, tr("Guardar empresa"),
                     confpr->valor(CONF_DIR_USER),
                     tr("Empresas (*.pgdump)"));

        if (!fn.isEmpty()) {
            if (fn.right(7) != ".pgdump")
                fn = fn + ".pgdump";
            fprintf(stderr, "Vamos a guardar la empresa en el fichero %s\n", fn.toAscii().constData());
            QString cadena;
            cadena.sprintf("pg_dump %s > %s",  dbEmpresa.toAscii().constData(), fn.toAscii().constData());
            fprintf(stderr, "%s\n", cadena.toAscii().constData());
            system(cadena.toAscii().constData());
        } // end if
    } // end if
}


/// Restauramos una copia de seguridad de una base de datos.
/// Para cargar la empresa debe estar sin ningun usuario dentro de ella.
void BConfiguracion::restaurarEmpresa() {
    _depura( "BConfiguracion::restaurarEmpresa", 0);
    QString dbEmpresa;
    QString PGserver;
    PGserver = confpr->valor(CONF_SERVIDOR).toAscii().constData();
    QString usuario;

    dbEmpresa = PunteroAlSelector->empresaDB();
    if (dbEmpresa != "") {
        QString fn = QFileDialog::getOpenFileName(this, tr("Cargar empresa"),
                     confpr->valor(CONF_DIR_USER),
                     tr("Empresas (*.pgdump)"));

        if (!fn.isEmpty()) {

	    QString comando = "dropdb " + dbEmpresa;
            system (comando.toAscii().constData());
	    comando = "createdb "+dbEmpresa;
            system (comando.toAscii().constData());
            comando = "psql " + dbEmpresa + "< " + fn;
            system (comando.toAscii().constData());
        } // end if
    } // end if
    _depura( "END BConfiguracion::restaurarEmpresa", 0);
}


void BConfiguracion::cargarFichaUsuarios() {}


void BConfiguracion::listView1_currentChanged(QListWidgetItem *) {
#ifndef WIN32
#endif
}


/// Salvamos los usuarios en la base de datos.
void BConfiguracion::BotonB_1Aplicar() {}


/// Desacemos los cambios que hemos hecho (UNDO).
void BConfiguracion::BotonB_2Desacer() {
    cargarFichaUsuarios();
}


void BConfiguracion::listView2_clickBotonDerecho(QListWidgetItem *, const QPoint&, int) {}


/// Por conveniencia (Bug QT??).
void BConfiguracion::listiView2_clickMouse(int, QListWidgetItem *, const QPoint&, int) {}

    void BConfiguracion::on_mui_restaurarc_clicked() {restaurarEmpresa();}
    void BConfiguracion::on_mui_restaurarf_clicked() {restaurarEmpresa();}

