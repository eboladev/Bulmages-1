/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <QMessageBox>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>
#include <QDrag>
#include <QMenu>

#include <fstream>

#include "cuadranteview.h"
#include "cuadrante1view.h"
#include "company.h"
#include "configuracion.h"
#include "plugins.h"

#include "funcaux.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
CuadranteView::CuadranteView(Company *comp, QWidget *parent)
        : FichaBf(comp, parent) {
    _depura("CuadranteView::CuadranteView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    try {
        setupUi(this);

        /// Lanzamos los plugins.
        if (g_plugins->lanza("CuadranteView_CuadranteView", this)) return;

        setTitleName(tr("Almacen"));
        setDBTableName("almacen");

        mui_listtrabajadores->setDragEnabled(TRUE);
        mui_cuadrante->setAcceptDrops(TRUE);

        inicializaTrabajadores();
        inicializaCuadrante(QDate::currentDate());

//        dialogChanges_cargaInicial();
        meteWindow(windowTitle(), this, FALSE);
    } catch (...) {
        mensajeInfo(tr("Error al crear el almacen"));
    } // end try
    _depura("END CuadranteView::CuadranteView", 0);
}


/** No precisa acciones adicionales en el destructor.
*/
CuadranteView::~CuadranteView() {
    _depura("CuadranteView::~CuadranteView", 0);
    _depura("END CuadranteView::~CuadranteView", 0);
}


/** Inicializa la selección de trabajadores
*/
void CuadranteView::inicializaTrabajadores() {

    mui_listtrabajadores->setColumnCount(2);
    cursor2 *cur = empresaBase()->cargacursor("SELECT * FROM tipotrabajo");
    if (cur) {
        while (!cur->eof()) {
            QTreeWidgetItem *it = new QTreeWidgetItem(mui_listtrabajadores);
            it->setText(0, cur->valor("nomtipotrabajo"));
            it->setTextColor(0, QColor("#FF0000"));

            /// Cargamos los trabajadores correspondientes a este tipo de trabajo.
            cursor2 *curtrab = empresaBase()->cargacursor("SELECT * FROM trabajador WHERE idtipotrabajo = "+cur->valor("idtipotrabajo"));
            if (curtrab) {
                while (!curtrab->eof()) {
                    QTreeWidgetItem *itt = new QTreeWidgetItem(it);
                    itt->setText(0, curtrab->valor("nomtrabajador"));
                    itt->setText(1, curtrab->valor("idtrabajador"));
                    itt->setTextColor(0, QColor("#000000"));
                    curtrab->siguienteregistro();
                } // end while
                delete curtrab;
            } // end if

            cur->siguienteregistro();
        } // end if
        delete cur;
    } // end if
}


/** Inicializa la selección de trabajadores
*/
void CuadranteView::inicializaCuadrante(const QDate &dateorig) {
    _depura("CuadranteView::inicializaCuadrante", 0);
    try {

        mui_cuadrante->clear();

        cursor2 *cur = empresaBase()->cargacursor("SELECT idalmacen FROM almacen");
        if (!cur) throw -1;


        mui_cuadrante->setRowCount(cur->numregistros());
        mui_cuadrante->setColumnCount(7);

        QStringList headers;
        headers << tr("Lunes") << tr("Martes") << tr("Miercoles") << tr("Jueves") << tr("Viernes") << tr("Sabado") << tr("Domingo");
        mui_cuadrante->setHorizontalHeaderLabels(headers);


        int row = 0;
        while (!cur->eof()) {
            /// Establecemos la altura de los campos.
            mui_cuadrante->setRowHeight(row, 100);

            /// Buscamos el Lunes de la Semana
            QDate date = dateorig.addDays(-dateorig.dayOfWeek() + 1);

            for (int column = 0; column < 7; column ++) {

                mui_cuadrante->setColumnWidth(column, 250);

                CuadranteQTextDocument *newItem = new CuadranteQTextDocument(empresaBase(), mui_cuadrante);

                newItem->setAlmFecha(cur->valor("idalmacen"), date);

                mui_cuadrante->setCellWidget(row, column, newItem);

                date = date.addDays(1);
            } // end for
            cur->siguienteregistro();
            row++;
        } // end while
    } catch (...) {
        _depura("Error en la carga del calendario", 2);
        return;
    }
}


void CuadranteView::on_mui_calendario_clicked(const QDate &date) {
    _depura("CuadranteView::on_mui_calendario_clicked", 0, date.toString("dd/MM/yyyy"));
    inicializaCuadrante(date);
}


void CuadranteView::on_mui_listtrabajadores_itemDoubleClicked(QTreeWidgetItem *item, int ) {
    _depura("elementos dobleclickados", 0);
    QString item1 = item->text(1);

    QList<QTableWidgetSelectionRange> selectionranges = mui_cuadrante->selectedRanges();
    for (int i = 0; i < selectionranges.size(); ++i) {
        for (int x = selectionranges.at(i).topRow(); x <= selectionranges.at(i).bottomRow(); x++) {
            for (int y = selectionranges.at(i).leftColumn(); y <= selectionranges.at(i).rightColumn(); y++) {
                CuadranteQTextDocument *newItem = (CuadranteQTextDocument *) mui_cuadrante->cellWidget(x, y);
                newItem->addTrabajador(item1);
            } // end for
        } // end for
    }
}

void CuadranteView::on_mui_editar_clicked() {
    _depura ("editar cuadrante", 0);
    Cuadrante1View *cuad = new Cuadrante1View(empresaBase(), 0);
    empresaBase()->pWorkspace()->addWindow(cuad);
    cuad->show();
    CuadranteQTextDocument *newItem = (CuadranteQTextDocument *) mui_cuadrante->cellWidget(mui_cuadrante->currentRow(), mui_cuadrante->currentColumn());
    newItem->connect (cuad, SIGNAL(save()), newItem, SLOT(refresh()));
    cuad->cargar(newItem->idcuadrante());
}

void CuadranteView::on_mui_calendario_customContextMenuRequested ( const QPoint & pos ) {
    QMenu *popup = new QMenu(mui_calendario);

    popup->addSeparator();
    QAction *norm = popup->addAction(tr("Normal"));
    QAction *fiesta = popup->addAction(tr("Fiesta General"));

    QAction *opcion = popup->exec(pos);
    if (opcion == norm) {
        empresaBase()->begin();
        empresaBase()->ejecuta("UPDATE CUADRANTE SET fiestacuadrante = FALSE WHERE fechacuadrante = '"+mui_calendario->selectedDate().toString("dd/MM/yyyy")+"'");
        empresaBase()->commit();
    } // end if

    if (opcion == fiesta) {
        empresaBase()->begin();
        empresaBase()->ejecuta("UPDATE CUADRANTE SET fiestacuadrante = TRUE WHERE fechacuadrante = '"+mui_calendario->selectedDate().toString("dd/MM/yyyy")+"'");
        empresaBase()->commit();
    } // end if

    inicializaCuadrante(mui_calendario->selectedDate());
}

void CuadranteView::on_mui_imprimir_clicked() {

    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "cuadrante.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "cuadrante.rml";
    QString archivologo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system (archivo.toAscii().constData());

    /// Copiamos el logo.
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#endif

    QFile file;
    file.setFileName(archivod);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt = "";


    ///Ponemos la tabla
    fitxersortidatxt += "<para>Cuadrante</para>\n";
    fitxersortidatxt += "<blockTable style=\"tablacontenido\" colWidths=\"4cm, 4cm, 4cm, 4cm, 4cm, 4cm, 4cm\" repeatRows=\"1\">\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "    <td>Lunes</td>\n";
    fitxersortidatxt += "    <td>Martes</td>\n";
    fitxersortidatxt += "    <td>Miercoles</td>\n";
    fitxersortidatxt += "    <td>Jueves</td>\n";
    fitxersortidatxt += "    <td>Viernes</td>\n";
    fitxersortidatxt += "    <td>Sabado</td>\n";
    fitxersortidatxt += "    <td>Domingo</td>\n";
    fitxersortidatxt += "</tr>\n";


// ================================================================

    /// Buscamos el Lunes de la Semana
//    QDate date = mui_calendario->selectedDate().addDays(-mui_calendario->selectedDate().dayOfWeek() + 1);


    cursor2 *cur = empresaBase()->cargacursor("SELECT idalmacen FROM almacen");
    if (!cur) throw -1;

    mui_cuadrante->setRowCount(cur->numregistros());
    mui_cuadrante->setColumnCount(7);


    int row = 0;
    while (!cur->eof()) {
	fitxersortidatxt += "<tr>\n";
        for (int column = 0; column < 7; column ++) {

            CuadranteQTextDocument *newItem = (CuadranteQTextDocument *) mui_cuadrante->cellWidget(row, column);
	    fitxersortidatxt += newItem->impresion();

        } // end for
        cur->siguienteregistro();
        row++;
	fitxersortidatxt += "</tr>\n";
    } // end while



    fitxersortidatxt += "</blockTable>\n";




    buff.replace("[story]", fitxersortidatxt);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    } // end if
    invocaPDF("cuadrante");

}


