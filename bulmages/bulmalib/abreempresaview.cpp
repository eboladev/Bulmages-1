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

using namespace std;

#include <QFile>
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

#include <stdio.h>

#include "abreempresaview.h"
#include "postgresiface2.h"
#include "configuracion.h"
#include "funcaux.h"

/// Este es el archivo en el que se almacenan las mui_empresas que existen.
/// Es un archivo separado por comas, que se suele alojar en el 'home/.bulmages' del usuario.
#define LISTEMPRESAS QString("listempresas.lst")
/// Numero de columna para el nombre en la lista.
#define ABRE_NOMBRE  0
/// Numero de columna que almacena el ejercicio de la empresa.
#define ABRE_ANO     1
/// Numero de columna que almacena el nombre de la base de datos de la empresa.
#define ABRE_ARCHIVO 2
/// Numero de columna que almacena el tipo de datos al que hace referencia la linea
/// (bulmacont o bulmafact).
#define ABRE_TIPO    3


/// Inicia las variables m_tipo y m_tipoempresa y carga el archivo para hacer la presentacion.
/// Llama a la funcion 'cargaArchivo'.
/// @param parent La ventana que hace la llamada.
/// @param tipo String que indica si es contabilidad o facturacion (bulmacont, bulmafact).
/// @param name nombre de la ventana.
/// @param modal Indica si la ventana debe comportar de forma modal o no. (por defecto si).
abreempresaview::abreempresaview(QWidget *parent, QString tipo, const char *name, bool modal)
        : QDialog(parent, name, modal) {
    _depura("abreempresaview::abreempresaview", 0);
    setupUi(this);

    mui_empresas->installEventFilter(this);
    QObject::connect(botonCancelar, SIGNAL(clicked(bool)), this, SLOT(s_botonCancelar()));
    QObject::connect(botonAceptar, SIGNAL(clicked(bool)), this, SLOT(accept()));
    QObject::connect(mui_empresas, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(mui_empresasdobleclick()));

    m_tipo = tipo;
    m_tipoempresa = "";
    m_modo = 0;
    /// si el m_modo es 0 (salir del programa si se cancela la ventana) entonces poner
    /// un titulo adecuado al boton.
    if (m_modo == 0) {
        botonCancelar->setText(tr("&Cierra el programa"));
    }
    cargaArchivo();
    _depura("END abreempresaview::abreempresaview", 0);
}


abreempresaview::~abreempresaview() {
    _depura("abreempresaview::~abreempresaview", 0);
}


/// Inserta una companya en el QList mui_empresas definido en el dialogo.
/// Crea un objeto QListViewItem para la QListView 'mui_empresas' y rellena sus
/// columnas con los datos pasados al metodo.
/// @param nombre Nombre de la empresa.
/// @param ano Ejercicio de la empresa (aunque pueden ser varios).
/// @param archivo Nombre de la base de datos.
/// @param tipo Tipo de base de datos (BulmaCont o BulmaFact).
void abreempresaview::insertCompany(QString nombre, QString ano, QString archivo, QString tipo) {
    _depura("abreempresaview::insertCompany", 0, nombre);
    mui_empresas->insertRow(mui_empresas->rowCount());
    QTableWidgetItem *nuevoItemNombre = new QTableWidgetItem(nombre);
    mui_empresas->setItem(mui_empresas->rowCount() - 1, 0, nuevoItemNombre);
    QTableWidgetItem *nuevoItemAno = new QTableWidgetItem(ano);
    mui_empresas->setItem(mui_empresas->rowCount() - 1, 1, nuevoItemAno);
    QTableWidgetItem *nuevoItemArchivo = new QTableWidgetItem(archivo);
    mui_empresas->setItem(mui_empresas->rowCount() - 1, 2, nuevoItemArchivo);
    QTableWidgetItem *nuevoItemTipo = new QTableWidgetItem(tipo);
    mui_empresas->setItem(mui_empresas->rowCount() - 1, 3, nuevoItemTipo);
    _depura("END abreempresaview::insertCompany", 0);
}


/// Se ha pulsado sobre el boton de aceptar con lo que iniciamos la variables y
/// cerramos esta ventana ya que ha cumplico con su cometido.
void abreempresaview::accept() {
    _depura("abreempresaview::accept", 0);
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
    if ((mui_empresas->currentRow() != -1) && (mui_empresas->isItemSelected(mui_empresas->item(mui_empresas->currentRow(), 0)) == TRUE)) {
        itemfilaactual0 = mui_empresas->item(mui_empresas->currentRow(), 0);
        textoitem0 = itemfilaactual0->text();
        itemfilaactual1 = mui_empresas->item(mui_empresas->currentRow(), 1);
        textoitem1 = itemfilaactual1->text();
        itemfilaactual2 = mui_empresas->item(mui_empresas->currentRow(), 2);
        textoitem2 = itemfilaactual2->text();
        itemfilaactual3 = mui_empresas->item(mui_empresas->currentRow(), 3);
        textoitem3 = itemfilaactual3->text();
        m_nombreempresa = textoitem0;
        m_empresabd = textoitem2;
        m_tipoempresa = textoitem3;
        done(0);
    } else {
        mensajeInfo(tr("Para entrar, antes tiene que seleccionar una empresa."));
    }
    _depura("abreempresaview::accept", 0);
}


/// Evento que se dispara al hacer doble click sobre el listado de mui_empresas.
void abreempresaview::mui_empresasdobleclick() {
    _depura("abreempresaview::mui_empresasdobleclick", 0);
    accept();
    _depura("END abreempresaview::mui_empresasdobleclick", 0);
}


/// Carga del archivo de mui_empresas las mui_empresas disponibles.
void abreempresaview::cargaArchivo() {
    _depura("abreempresaview::cargaArchivo", 0);
#ifndef WINDOWS

    QString dir1 = getenv("HOME");
    dir1 = dir1 + "/.bulmages/" + LISTEMPRESAS;
#else

    QString dir1 = "C:\\.bulmages\\" + LISTEMPRESAS;
#endif
    /// Si el archivo no existe hacemos una recarga.
    _depura("Vamos a comprobar la existencia\n", 1);
    /// Comprobamos la existencia del directorio personalizado de bulmages. Y si no
    if (!QFile::exists(dir1)) {
        /// Hacemos una recarga de mui_empresas pq sabemos a ciencia cierta que ha cambiado el listado.
        on_mui_actualizar_clicked();
    } // end if

    preparamui_empresas();
    ifstream filestr(dir1.toAscii().data());
    string nombre, ano, nombd, tipo;
    while (filestr.good()) {
        getline(filestr, nombre, ',');
        getline(filestr, ano, ',');
        getline(filestr, nombd, ',');
        if (getline(filestr, tipo, '\n') ) {
            QString eltipo = tipo.c_str();
            if (eltipo == m_tipo || m_tipo == "") {
                insertCompany(nombre.c_str(), ano.c_str(), nombd.c_str(), tipo.c_str());
            } // end if
        } // end if
    } // end while
    filestr.close();
    _depura("abreempresaview::cargaArchivo", 0);
}


/// Guarda en el archivo de mui_empresas las mui_empresas disponibles
/// Tambien actualiza el listado de mui_empresas visibles.
void abreempresaview::guardaArchivo() {
    _depura("abreempresaview::guardaArchivo", 0);
#ifndef WINDOWS

    QString dir1 = getenv("HOME");
    dir1 = dir1 + "/.bulmages/" + LISTEMPRESAS;
#else

    QString dir1 = "C:\\.bulmages\\" + LISTEMPRESAS;
#endif

    ofstream filestr((char *) dir1.toAscii().data());
    /// Deshabilitamos las alertas para que no aparezcan warnings con bases de datos
    /// que no son del sistema.
    confpr->setValor(CONF_ALERTAS_DB, "No");
    /// Nos conectamos a template1 para obtener un listado de todas las bases de datos.
    postgresiface2 *db, *db1;
    db = new postgresiface2();
    db->inicializa(QString("template1"));
    QString nombre;
    QString nomdb = "";
    QString ano;
    QString tipo;
    cursor2 *curs = db->cargacursor("SELECT datname FROM pg_database");
    /// Preparamos el listado
    preparamui_empresas();
    /// Para cada base de datos nos intentamos conectar y miramos de que tipo es.
    while (!curs->eof()) {
        db1 = new postgresiface2();
        db1->inicializa(curs->valor("datname"));
        try {
            cursor2 *curs1 = db1->cargacursor("SELECT * FROM configuracion WHERE nombre = 'Tipo'");
            if (!curs1->eof()) {
                tipo = curs1->valor("valor");
                nomdb = curs->valor("datname");
            } // end if
            delete curs1;
            curs1 = db1->cargacursor("SELECT * FROM configuracion WHERE nombre = 'NombreEmpresa'");
            if (!curs1->eof()) {
                nombre = curs1->valor("valor");
            } // end if
            delete curs1;
            curs1 = db1->cargacursor("SELECT * FROM configuracion WHERE nombre = 'Ejercicio'");
            if (!curs1->eof()) {
                ano = curs1->valor("valor");
            } // end if
            delete curs1;
        } catch (...) {}
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
        delete db1;
        curs->siguienteregistro();
    } // end while
    delete curs;
    delete db;
    confpr->setValor(CONF_ALERTAS_DB, "Yes");
    filestr.close();
    _depura("END abreempresaview::guardaArchivo", 0);
}


/// Recarga la lista de mui_empresas haciendo las gestiones necesarios con el motor de
/// Base de Datos. Al mismo tiempo guarda el archivo de bases de datos en el
/// archivo 'LISTEMPRESAS'.
void abreempresaview::on_mui_actualizar_clicked() {
    _depura("abreempresaview::on_mui_actualizar_clicked", 0);
    guardaArchivo();
    _depura("END abreempresaview::on_mui_actualizar_clicked", 0);
}


void abreempresaview::preparamui_empresas() {
    _depura("abreempresaview::preparamui_empresas", 0);
    mui_empresas->clear();
    mui_empresas->setRowCount(0);
    mui_empresas->setColumnCount(4);
    QStringList nombrecolumnas;
    nombrecolumnas << tr("Nombre") << tr("Anyo") << tr("Archivo") << tr("Tipo");
    mui_empresas->setHorizontalHeaderLabels(nombrecolumnas);
    mui_empresas->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);

    mui_empresas->verticalHeader()->hide();
    mui_empresas->setShowGrid(FALSE);
    /// Si es el modo de facturacion ocultamos la fecha.
    if (m_tipo == "BulmaFact")
        mui_empresas->hideColumn(ABRE_ANO);
    _depura("END abreempresaview::preparamui_empresas", 0);
}


bool abreempresaview::eventFilter(QObject *obj, QEvent *ev) {
    if (obj == mui_empresas) {
        if (ev->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(ev);
            switch (keyEvent->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                /// Se comprueba que se ha seleccionado una entrada en la lista mui_empresas.
                if (mui_empresas->currentRow() != -1) {
                    /// Entramos en el programa.
                    this->accept();
                }
                break;
            }
            return false;
        } else {
            return false;
        }
    } else {
        /// Si no se ha tratado el evento, se deja pasar.
        return QDialog::eventFilter(obj, ev);
    }
}

