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

/** \file abreempresaview.cpp
* Contiene la implementacion de la clase \ref abreempresaview y sus metodos.
*/

#include <fstream>

using namespace std;

#include <QFile>
#include <QLineEdit>
#include <QMessageBox>
#include <QString>
#include <QList>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QStringList>

#include <stdio.h>

#include "abreempresaview.h"
#include "postgresiface2.h"
#include "configuracion.h"
#include "funcaux.h"

/// \brief Este es el archivo en el que se almacenan las empresas que existen. Es un archivo separado por comas, que se suele alojar en el <i>home/.bulmages</i> del usuario.
#define LISTEMPRESAS QString("listempresas.lst")
/// \brief Nmero de columna para el nombre en la lista.
#define ABRE_NOMBRE  0
/// \brief Numero de columna que almacena el ejercicio de la empresa.
#define ABRE_ANO     1
/// \brief Numero de columna que almacena el nombre de la base de datos de la empresa.
#define ABRE_ARCHIVO 2
/// \brief Numero de columna que almacena el tipo de datos al que hace referencia la linea (bulmacont o bulmafact).
#define ABRE_TIPO    3


/** Inicia las variables m_tipo y m_tipoempresa y carga el archivo para hacer la presentaci�.
* Llama a la funcion \ref cargaArchivo
* @param parent La ventana que hace la llamada
* @param tipo String que indica si es contabilidad o facturacion (bulmacont, bulmafact)
* @param name nombre de la ventana
* @param modal Indica si la ventana debe comportar de forma modal o no. (por defecto si)
*/
abreempresaview::abreempresaview(QWidget *parent, QString tipo, const char *name, bool modal)
        : QDialog(parent,name,modal) {
    setupUi(this);

    QObject::connect(botonCancelar, SIGNAL(clicked(bool)), this, SLOT(s_botonCancelar()));
    QObject::connect(botonAceptar, SIGNAL(clicked(bool)), this, SLOT(accept()));
    QObject::connect(empresas, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(empresasdobleclick()));
    QObject::connect(toolButton1, SIGNAL(clicked(bool)), this, SLOT(s_reloadButton()));

    m_tipo = tipo;
    m_tipoempresa = "";
    m_modo = 0;

    cargaArchivo();
}


abreempresaview::~abreempresaview() {}


/** Inserta una companya en el QList empresas definido en el dialogo
* Crea un objeto QListViewItem para la QListView \ref empresas y rellena sus columnas con los datos pasados al metodo.
* @param nombre Nombre de la empresa
* @param ano Ejercicio de la empresa (aunque pueden ser varios)
* @param archivo Nombre de la base de datos
* @param tipo Tipo de base de datos (BulmaCont o BulmaFact)
*/
void abreempresaview::insertCompany(QString nombre, QString ano, QString archivo, QString tipo) {
    empresas->insertRow(empresas->rowCount() + 1);

    QTableWidgetItem *nuevoItemNombre = new QTableWidgetItem(nombre);
    empresas->setItem(empresas->rowCount() - 1, 0, nuevoItemNombre);

    QTableWidgetItem *nuevoItemAno = new QTableWidgetItem(ano);
    empresas->setItem(empresas->rowCount() - 1, 1, nuevoItemAno);

    QTableWidgetItem *nuevoItemArchivo = new QTableWidgetItem(archivo);
    empresas->setItem(empresas->rowCount() - 1, 2, nuevoItemArchivo);

    QTableWidgetItem *nuevoItemTipo = new QTableWidgetItem(tipo);
    empresas->setItem(empresas->rowCount() - 1, 3, nuevoItemTipo);
}


/// Se ha pulsado sobre el boton de aceptar con lo que iniciamos la variables y
/// cerramos esta ventana ya que ha cumplico con su cometido.
void abreempresaview::accept() {
    /// Columna 0: Nombre
    /// Columna 1: anyo.
    /// Columna 2: Archivo.
    /// Columna 3: Tipo.

    QTableWidgetItem *itemfilaactual0;
    QTableWidgetItem *itemfilaactual1;
    QTableWidgetItem *itemfilaactual2;
    QTableWidgetItem *itemfilaactual3;
    QString textoitem0;
    QString textoitem1;
    QString textoitem2;
    QString textoitem3;

    /// Comprueba que la fila esta seleccionada.
    /// (solo necesitamos comprobar una celda de la fila) y luego
    /// recupera la informacion de la fila actual.
    if ((empresas->currentRow() != -1) && (empresas->isItemSelected(empresas->item(empresas->currentRow(), 0)) == TRUE)) {
        itemfilaactual0 = empresas->item(empresas->currentRow(), 0);
        textoitem0 = itemfilaactual0->text();
        itemfilaactual1 = empresas->item(empresas->currentRow(), 1);
        textoitem1 = itemfilaactual1->text();
        itemfilaactual2 = empresas->item(empresas->currentRow(), 2);
        textoitem2 = itemfilaactual2->text();
        itemfilaactual3 = empresas->item(empresas->currentRow(), 3);
        textoitem3 = itemfilaactual3->text();

        m_nombreempresa = textoitem0;
        m_empresabd = textoitem2;
        m_tipoempresa = textoitem3;

        done(0);
    } else {
        mensajeInfo(tr("Hay que seleccionar una empresa para entrar en el programa."));
    }
}

/// Evento que se dispara al hacer doble click sobre el listado de empresas.
void abreempresaview::empresasdobleclick() {
    accept();
}


/// Carga del archivo de empresas las empresas disponibles.
void abreempresaview::cargaArchivo() {
#ifndef WINDOWS
    QString dir1 = getenv("HOME");
    dir1 = dir1 + "/.bulmages/" + LISTEMPRESAS;
#else

    QString dir1 = "C:\\.bulmages\\" + LISTEMPRESAS;
#endif

    /// Si el archivo no existe hacemos una recarga.
    _depura("Vamos a comprobar la existencia\n", 1);
    /// Comprobamos la existencia del directorio personalizado de bulmages. Y si no
    if (! QFile::exists(dir1)) {
        /// Hacemos una recarga de empresas pq sabemos a ciencia cierta que ha cambiado el listado.
        s_reloadButton();
    } // end if

    preparaempresas();

    ifstream filestr(dir1.toAscii().data());
    string nombre, ano, nombd, tipo;
    while (filestr.good()) {
        getline(filestr, nombre, ',');
        getline(filestr, ano, ',');
        getline(filestr, nombd, ',');
        if (getline(filestr, tipo, '\n') ) {
            QString eltipo = tipo.c_str();
            fprintf(stderr, "%s\n", tipo.c_str());
            if (eltipo == m_tipo || m_tipo == "") {
                insertCompany(nombre.c_str(), ano.c_str(), nombd.c_str(), tipo.c_str());
            } // end if
        } // end if
    } // end while
    filestr.close();
}


/// Guarda en el archivo de empresas las empresas disponibles
/// Tambien actualiza el listado de empresas visibles.
void abreempresaview::guardaArchivo() {
#ifndef WINDOWS
    QString dir1 = getenv("HOME");
    dir1 = dir1 + "/.bulmages/" + LISTEMPRESAS;
#else

    QString dir1 = "C:\\.bulmages\\" + LISTEMPRESAS;
#endif

    ofstream filestr((char *) dir1.toAscii().data());
    /// Desabilitamos las alertas para que no aparezcan warnings con bases de datos que no son del sistema.
    confpr->setValor(CONF_ALERTAS_DB, "No");
    postgresiface2 *db, *db1;
    db = new postgresiface2();
    db->inicializa(QString("template1"));
    db->begin();
    QString nombre;
    QString nomdb = "";
    QString ano;
    QString tipo;
    cursor2 *curs = db->cargacursor("SELECT datname FROM pg_database", "otroquery");
    db->commit();

    preparaempresas();

    while (! curs->eof()) {
        db1 = new postgresiface2();
        db1->inicializa(curs->valor("datname"));
        db1->begin();
        cursor2 *curs1 = db1->cargacursor("SELECT * FROM configuracion WHERE nombre='Tipo'", "masquery");
        if (!curs1->eof()) {
            tipo = curs1->valor("valor");
            nomdb = curs->valor("datname");
        } // end if

        delete curs1;
        curs1 = db1->cargacursor("SELECT * FROM configuracion WHERE nombre='NombreEmpresa'", "masquery1");

        if (!curs1->eof() ) {
            nombre = curs1->valor("valor");
        } // end if

        delete curs1;
        curs1 = db1->cargacursor("SELECT * FROM configuracion WHERE nombre='Ejercicio'", "masquery2");

        if (!curs1->eof() ) {
            ano = curs1->valor("valor");
        } // end if

        delete curs1;
        if (nomdb != "") {
            if (tipo == m_tipo || m_tipo == "") {
                insertCompany(nombre, ano, nomdb, tipo);
            }// end if
            /// Independientemente de si deben mostrarse o no hay que guardarlas en el archivo.
            filestr << nombre.toAscii().data() << ",";
            filestr << ano.toAscii().data() << ",";
            filestr << nomdb.toAscii().data() << ",";
            filestr << tipo.toAscii().data()  << endl;
            nomdb = "";
        } // end if
        db1->commit();
        delete db1;
        curs->siguienteregistro();
    } // end while
    delete curs;
    delete db;
    confpr->setValor(CONF_ALERTAS_DB, "Yes");
    filestr.close();
}


/// Recarga la lista de empresas haciendo las gestiones necesarios con el motor de Base de Datos.
/// Al mismo tiempo guarda el archivo de bases de datos en el archivo \ref LISTEMPRESAS
void abreempresaview::s_reloadButton() {
    preparaempresas();
    guardaArchivo();
}


void abreempresaview::preparaempresas() {
    empresas->clear();
    empresas->setRowCount(0);
    empresas->setColumnCount(4);
    QStringList nombrecolumnas;
    nombrecolumnas << tr("Nombre") << tr("Anyo") << tr("Archivo") << tr("Tipo");
    empresas->setHorizontalHeaderLabels(nombrecolumnas);
}
