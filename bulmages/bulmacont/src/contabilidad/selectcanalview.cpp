/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras                                    *
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

#include "selectcanalview.h"
#include "empresa.h"


/// Inicializa las variables de empresa y de base de datos.
/** Luego crea las columnas para el objeto m_listCanales que es la lista en que se basa
    el programa. Luego llama al m&eacute;todo cargacanales que hace la carga de los canales
    a partir de la base de datos. */
selectcanalview::selectcanalview(empresa *emp,QWidget *parent)
        : QDialog(parent) {
    _depura("selectcanalview::selectcanalview", 0);
    setupUi(this);
    empresaactual = emp;
    numdigitos = empresaactual->numdigitosempresa();
    m_iterador = new QTreeWidgetItemIterator(m_listCanales);

    m_listCanales->setColumnCount(5);

    QStringList etiquetas;
    etiquetas << tr("nom_canal") << tr("desc_canal") << tr("Status") << tr("idcanal") << tr("Seleccion");
    m_listCanales->setHeaderLabels(etiquetas);

    cargacanales();
    _depura("END selectcanalview::selectcanalview", 0);
}


selectcanalview::~selectcanalview() {
    delete m_iterador;
}


void selectcanalview::cargacanales() {
    QMap <int, QTreeWidgetItem *> Lista;
    QTreeWidgetItem *it;

    int idcanal = 0;
    cursor2 *cursoraux1;
    /// Cogemos los canales y los ponemos donde toca.
    m_listCanales->clear();
    empresaactual->begin();
    cursoraux1 = empresaactual->cargacursor("SELECT * FROM canal", "canalillos");
    empresaactual->commit();
    while (!cursoraux1->eof()) {
        idcanal = atoi(cursoraux1->valor("idcanal").toAscii());
        it = new QTreeWidgetItem(m_listCanales);
        Lista[idcanal] = it;
        it->setText(3, cursoraux1->valor("idcanal"));
        it->setText(1, cursoraux1->valor("descripcion"));
        it->setText(0, cursoraux1->valor("nombre"));
        it->setCheckState(0, Qt::Unchecked);
	it->setExpanded(TRUE);
        cursoraux1->siguienteregistro();
    } // end while

    delete cursoraux1;
}


/// Esta funci&oacute;n devuelve el primer canal seleccionado de la vista.
/// Devuelve el idcanal. Si no hay ning&uacute;n canal seleccionado devuelve
/// cero.
int selectcanalview::firstcanal() {
    delete m_iterador;
    m_iterador = new QTreeWidgetItemIterator(m_listCanales);

    return nextcanal();
}


/// Esta funci&oacute;n devuelve el siguiente canal seleccionado de la vista.
int selectcanalview::nextcanal() {
    int idcanal = 0;
    fprintf(stderr, "nextcanal\n");
    while ((**m_iterador) && idcanal == 0) {
        if ((**m_iterador)->checkState(0) == Qt::Checked) {
            idcanal = (**m_iterador)->text(3).toInt();
            fprintf(stderr, "siguiente canal:%d\n", idcanal);
            return idcanal;
        } // end if
        ++(*m_iterador);
    } // end while

    return idcanal;
}


QString selectcanalview::cadcanal() {
    int idcanal;
    QString ccanales = "";

    idcanal = firstcanal();
    while (idcanal) {
        if (ccanales != "")
            ccanales.sprintf("%s, %d", ccanales.toAscii().constData(), idcanal);
        else
            ccanales.sprintf("%d", idcanal);
        idcanal = nextcanal();
    } // end while

    return ccanales;
}


/// Esta funci&oacute;n devuelve el nombre de un canal determinado.
QString selectcanalview::nomcanal() {
    QTreeWidgetItemIterator m_iterador(m_listCanales);

    if ((*m_iterador)->checkState(0) == Qt::Checked) {
        fprintf(stderr, "nomcanal: %s\n", (*m_iterador)->text(0).toAscii().constData());
        return (*m_iterador)->text(0);
    } else {
        return "";
    } // end if

    delete *m_iterador;
}


void selectcanalview::on_mui_todo_clicked() {
    QTreeWidgetItemIterator m_iterador(m_listCanales);

    while (*m_iterador) {
        (*m_iterador)->setCheckState(0, Qt::Checked);
        ++m_iterador;
    }

    delete *m_iterador;
}


void selectcanalview::on_mui_nada_clicked() {
    QTreeWidgetItemIterator m_iterador(m_listCanales);

    while (*m_iterador) {
        (*m_iterador)->setCheckState(0, Qt::Unchecked);
        ++m_iterador;
    }

    delete *m_iterador;
}


void selectcanalview::on_mui_invertir_clicked() {
    QTreeWidgetItemIterator m_iterador(m_listCanales);

    while (*m_iterador) {
        if ((*m_iterador)->checkState(0) == Qt::Unchecked) {
            (*m_iterador)->setCheckState(0, Qt::Checked);
        } else {
            (*m_iterador)->setCheckState(0, Qt::Unchecked);
        }
        ++m_iterador;
    }

    delete *m_iterador;
}

