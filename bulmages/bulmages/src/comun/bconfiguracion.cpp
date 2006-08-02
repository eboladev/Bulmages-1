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
 ***************************************************************************/
#include "bconfiguracion.h"
#include <unistd.h>
#include <qmessagebox.h>
#include <errno.h>
#include <q3table.h>
#include <qtabwidget.h>
#include <qfile.h>

#include "bnuevaempresa.h"
#include "nuevafact.h"

#ifdef WIN32
#include <process.h>
#endif

#ifndef WIN32
#include <map>
#include <set>
#endif

BConfiguracion::BConfiguracion(BSelector * ref, QWidget * parent, const char * name, Qt::WFlags f=0) 
  : QDialog(parent,name,f) {
  setupUi(this);

  QObject::connect(pushButtonF_X, SIGNAL(clicked()), this, SLOT(cerrar()));
  QObject::connect(pushButtonA_1_2, SIGNAL(clicked()), this, SLOT(nuevaFacturacion()));
  QObject::connect(pushButton32, SIGNAL(clicked()), this, SLOT(BotonContaplus()));
  QObject::connect(pushButtonA_4, SIGNAL(clicked()), this, SLOT(BotonA_4restaurarEmpresa()));
  QObject::connect(m_borrarempresa, SIGNAL(clicked()), this, SLOT(borrarEmpresa()));
  QObject::connect(pushButtonA_1, SIGNAL(clicked()), this, SLOT(nuevaEmpresa()));
  QObject::connect(m_backup, SIGNAL(clicked()), this, SLOT(salvarEmpresa()));
  QObject::connect(pushButtonA_5, SIGNAL(clicked()), this, SLOT(nuevoEjercicio()));
  QObject::connect(pushButton32_2, SIGNAL(clicked()), this, SLOT(s_importexportbulmafact()));

  PunteroAlSelector=ref;
  if (PunteroAlSelector->tipoEmpresa() == "BulmaCont") {
    cargarFichaBulmages();
    m_tab->setTabEnabled(m_tab->page(1),FALSE);
    m_tab->setTabEnabled(m_tab->page(2),FALSE);
    m_tab->setTabEnabled(m_tab->page(3),FALSE);
  }// end if
  if (PunteroAlSelector->tipoEmpresa() == "BulmaFact") {
    cargarFichaBulmages();
    m_tab->setTabEnabled(m_tab->page(0),FALSE);
    m_tab->setTabEnabled(m_tab->page(2),FALSE);
    m_tab->setTabEnabled(m_tab->page(3),FALSE);
  }// end if
  cargarFichaUsuarios();
  tablaconfiguracion();
}// end BConfiguracion

BConfiguracion::~BConfiguracion() {}

void BConfiguracion::cerrar() {
    // Guardamos la configuraci�.
    for (int i =0;i<1000;i++) {
        if (confpr->nombre(i) != "") {
            confpr->setValor(i,m_tconfiguracion->text(i,1).ascii());
        }// end if
    }// end for

    confpr->saveconfig();
    //  traductor->load(archivo.c_str(),confpr->valor(CONF_DIR_TRADUCCION).c_str());
    //Cargo la nueva fuente
    //Cierro la ventana de Configuraci�
    close();
}


/// Esta funci� rellena la tabla de configuraci� del programa.
/// Para luego poder guardar los parametros que haga falta.
void BConfiguracion::tablaconfiguracion() {
    m_tconfiguracion->setLeftMargin(0);
    m_tconfiguracion->hideColumn(2);
    m_tconfiguracion->setNumRows(1000);
    QString cad;
    for (int i =0;i<1000;i++) {
        cad= cad.setNum(i);
        m_tconfiguracion->setText(i,0,confpr->nombre(i));
        m_tconfiguracion->setText(i,1,confpr->valor(i));
        m_tconfiguracion->setText(i,2,cad);
        if (confpr->nombre(i) == "")
            m_tconfiguracion->hideRow(i);
    }// end for
    m_tconfiguracion->adjustColumn(0);
    m_tconfiguracion->adjustColumn(1);
}// end tablaconfiguracion




/**********************************************************************************************/
/* BLOQUE UTILIDADES                                                                          */
/**********************************************************************************************/
void BConfiguracion::cargarFichaBulmages() {
    lineEditA_1->setText(PunteroAlSelector->nombreempresa->text());
}// end cargarFichaBulmages


void BConfiguracion::FontChanged(const QString & ) {}// end FontChanged

void BConfiguracion::FontSizeChanged(int ) {}// end FontSizeChanged

void BConfiguracion::BotonA_10aceptar() {}// BotonA_10aceptar



#include "importContaplus.h"
/// Responde a la pusaci� de importar datos de Contaplus a BulmaG�
void BConfiguracion::BotonContaplus() {
    postgresiface2 *DBconn = new postgresiface2();
    DBconn->inicializa(PunteroAlSelector->empresaDB());
    importContaplus *import= new importContaplus(DBconn,0,0,0);
    import->exec();
    delete import;
    delete DBconn;
}// end BotonContaplus



#include "importexportbulmafactview.h"
/// Responde a la pusaci� de importar datos de Contaplus a BulmaG�
void BConfiguracion::s_importexportbulmafact() {
    postgresiface2 *DBconn = new postgresiface2();
    DBconn->inicializa(PunteroAlSelector->empresaDB());
    ImportExportBulmafactView *import= new ImportExportBulmafactView(DBconn,0,0,0);
    import->exec();
    delete import;
    delete DBconn;
}// end BotonContaplus

void BConfiguracion::BotonA_11rechazar() {
    //poner el comboBoxFuente y el comboBoxIdioma a sus valores anteriores.
    if (PunteroAlSelector->tipoEmpresa() == "BulmaCont") {
        cargarFichaBulmages();
    }// end if
}// end BotonA_11rechazar


/**
  * Esta funci� se ejecuta cuando se lanza el cambio de nombre de la empresa.|
  */
void BConfiguracion::BotonA_6nombreEmpresa() {
    if (lineEditA_1->isReadOnly() ) { //Activa el line edit para que pueda ser editado.
        lineEditA_1->setReadOnly(false);
        lineEditA_1->setPaletteBackgroundColor(QColor(255,255,255));
    } else { //Y guarda el nuevo nombre de empresa.
        lineEditA_1->setReadOnly(true);
        lineEditA_1->setPaletteBackgroundColor(QColor(255,255,0));
        PunteroAlSelector->nombreempresa->setText(lineEditA_1->text());
    }// end if
}// end BotonA_6nombreEmpresa


/**
  * Aqui abrimos el cuador de dialogo que nos permite crear una empresa nueva basada en la
  * BASE DE DATOS bgplangcont
  */
void BConfiguracion::nuevaEmpresa() {
    BNuevaEmpresa *n= new BNuevaEmpresa(this,"Creador");
    n->exec();
    delete n;
}//Fin nuevaEmpresa

/**
  * Aqui abrimos el cuadro de dialogo que nos permite crear una empresa nueva basada en la
  * BASE DE DATOS bgplangcont
  */
void BConfiguracion::nuevaFacturacion() {
    nuevafact *n= new nuevafact(this,"Creador");
    n->exec();
    delete n;
}//Fin nuevaEmpresa


/**
  * Aqui creamos una nueva empresa que es una copia exacta de otra empresa que ya existe.
  */
void BConfiguracion::BotonA_61clonarEmpresa() {
    QString dbEmpresa;
    if (dbEmpresa!="") {
        QMessageBox::information( this, "Debug", "Clonando la Base de Datos: " +dbEmpresa +"\n\nProceso no disponible...\n", QMessageBox::Ok);
    }// end if
}


/**
  * Aqui borramos una empresa entera. No nos permite borrar la base de datos bgplangcont ni la base      
  * de datos de la empresa que tengamos abierta en este momento.
  * \todo S�o borra bases de datos en local, con bases de datos remotas no funciona. 
  */
void BConfiguracion::borrarEmpresa() {
    QString dbEmpresa;
    QString nombreEmpresa;
    QString idEmpresa;
    QString ejercicio;
    /// Siempre se borra la empresa actual.
    dbEmpresa = PunteroAlSelector->empresaDB();
    if (dbEmpresa!="") {
        if (dbEmpresa=="bgplangcont") {
            QMessageBox::information( this, tr("Atencion"), tr("Esta Base de Datos no puede ser eliminada.\n\r Es la plantilla para generar nuevas empresas."), QMessageBox::Ok);
        } else if (dbEmpresa != PunteroAlSelector->empresaDB()) {
            QMessageBox::warning( this, tr("Atención"), tr("No Está permitido eliminar la base \n\r de datos actualmente abierta."), QMessageBox::Ok,0);
        } else {
            int mensaje = QMessageBox::warning( this, tr("Atención"), tr("Borrar una empresa puede suponer perdida de datos\n Desea continuar?\n"), QMessageBox::Yes,QMessageBox::No,0);
            if (mensaje == QMessageBox::Yes) {
                QString sentencia = "dropdb "+dbEmpresa;
                system(sentencia.ascii());
                /// Hacemos una recarga de empresas pq sabemos a ciencia cierta que ha cambiado el listado.
                abreempresaview *abre= new abreempresaview(NULL,"hola","hola");
		abre->on_mui_actualizar_clicked();
                delete abre;
                done(1);
            }// end if
        }// end if
    }// end if
    close();
}//Fin borrarEmpresa




/**
* Aqui creamos un nuevo ejercicio para la empresa que tengamos abierta en el momento de llamar          
* a este proceso.                                                                                       
*/
void BConfiguracion::nuevoEjercicio() {
    QString query;
    int x;
    QString ejer;
    postgresiface2 *DBconn = new postgresiface2();
    DBconn->inicializa(PunteroAlSelector->empresaDB());
    ejer = m_ejercicio->text();
    if (ejer == "") {
        DBconn->begin();
        query.sprintf("SELECT MAX(ejercicio) AS ejercicio FROM ejercicios WHERE periodo=0");
        cursor2 *curA = DBconn->cargacursor(query,"curA");
        if (!curA->eof())
            ejer = curA->valor("ejercicio");
        ejer.setNum(ejer.toInt() + 1);
        if (ejer.toInt()<2000)
            ejer="2003";
    }// end if
    for (x=0; x<=12; x++) {
        query.sprintf("INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES('%s', '%d', 'f')",ejer.ascii(),x);
        DBconn->ejecuta(query);
    }// endfor
    DBconn->commit();
    QMessageBox::information( this, tr("Nuevo Ejercicio"), "El ejercicio ("+ ejer + ") ha sido creado con exito" , "&Aceptar");
}//Fin nuevoEjercicio




/*********************************************************************************************************/
/* Creamos una copia de seguridad de una base de datos                                                   */
/*********************************************************************************************************/
void BConfiguracion::salvarEmpresa() {
    QString dbEmpresa;
    QString PGserver;
    //  PGserver = "-h ";

    PGserver = confpr->valor(CONF_SERVIDOR).ascii();
    dbEmpresa = PunteroAlSelector->empresaDB();
    fprintf(stderr,"VAmos a guardar la empresa %s\n", dbEmpresa.ascii());

    if (dbEmpresa!="") {
        QString fn = Q3FileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER), "Empresas (*.pgdump)", 0,"Guardar Empresa","Elige el nombre de empresa con el que guardar");
        if (!fn.isEmpty()) {
            if (fn.right(7)!= ".pgdump")
                fn = fn +".pgdump";
            fprintf(stderr,"Vamos a guardar la empresa en el fichero %s\n",fn.ascii());
            QString cadena;
            cadena.sprintf("%sguardaemp %s %s %s", confpr->valor(CONF_EJECUTABLES).ascii(), PGserver.ascii(), dbEmpresa.ascii(), fn.ascii() );
            fprintf(stderr,"%s\n", cadena.ascii());
            system(cadena.ascii());
        }// end if
    }// end if
}// end salvarEmpresa


/*********************************************************************************************************/
/* Restauramos una copia de seguridad de una base de datos                                               */
/*********************************************************************************************************/
// Para cargar la empresa debe estar sin ningun usuario dentro de ella.
void BConfiguracion::BotonA_4restaurarEmpresa() {
    fprintf(stderr,"Restaurar empresa \n");
    QString dbEmpresa;
    QString PGserver;
    PGserver = confpr->valor(CONF_SERVIDOR).ascii();
    QString usuario;

    dbEmpresa = PunteroAlSelector->empresaDB();
    if (dbEmpresa!="") {
        QString fn = Q3FileDialog::getOpenFileName(confpr->valor(CONF_DIR_USER), "Empresas (*.pgdump)", 0,"Cargar Empresa","Elige el fichero a cargar.");

        if (!fn.isEmpty()) {
            QString comando= "cargaemp "+PGserver+" "+dbEmpresa+" "+fn;
            fprintf(stderr,"%s\n", comando.ascii());
            system (comando.ascii());
        }// end if
    }// end if
}

/**********************************************************************************************/
/* BLOQUE USUARIOS                                                                            */
/**********************************************************************************************/

void BConfiguracion::cargarFichaUsuarios() {}


void BConfiguracion::listView1_currentChanged(Q3ListViewItem *) {
#ifndef WIN32
#endif
}// end listView1_currentChanged


//Salvamos los usuarios en la base de datos
void BConfiguracion::BotonB_1Aplicar() {}

//Desacemos los cambios que hemos hecho (UNDO).
void BConfiguracion::BotonB_2Desacer() {
    cargarFichaUsuarios();
}

void BConfiguracion::listView2_clickBotonDerecho(Q3ListViewItem* ,const QPoint&  ,int ) {}


//Por conveniencia (Bug QT??)
void BConfiguracion::listiView2_clickMouse(int,Q3ListViewItem * ,const QPoint& ,int ) {}
