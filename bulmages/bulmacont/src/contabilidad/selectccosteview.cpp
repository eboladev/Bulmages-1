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

#include "selectccosteview.h"
#include "empresa.h"
#include "funcaux.h"


selectccosteview::selectccosteview(empresa *emp, QWidget *parent)
        : QDialog(parent) {
    _depura("selectccosteview::selectccosteview", 0);
    setupUi(this);
    empresaactual = emp;
    numdigitos = empresaactual->numdigitosempresa();
    m_iterador = new Q3ListViewItemIterator (m_listCostes);

    m_colNomCoste = m_listCostes->addColumn("nom_coste", -1);
    m_colDescCoste = m_listCostes->addColumn("desc_coste", -1);
    m_colStatusCoste = m_listCostes->addColumn("Status", -1);
    m_colIdCoste = m_listCostes->addColumn("idc_coste", 0);
    m_colCheck = m_listCostes->addColumn("Seleccion", -1);

    cargacostes();
    fprintf(stderr, "FIN de Constructor de selectccosteview\n");
}


selectccosteview::~selectccosteview() {
    delete m_iterador;
}


void selectccosteview::cargacostes() {
    /// Rellenamnos la listbox que va a sustituir al combobox correspondiente.
    /// Para que en los listados puedan salir m&aacute;s cosas de las que se dicen.
    fprintf(stderr, "Ahora nos toca rellenar las listas.\n");
    Q3CheckListItem *it;
    QMap <int,Q3ListViewItem *> Lista;
    int padre;
    int idc_coste = 0;
    cursor2 *cursoraux1, *cursoraux2;

    /// Cogemos los centros de coste principales y los ponemos donde toca.
    m_listCostes->clear();
    cursoraux1 = empresaactual->cargacursor("SELECT * FROM c_coste WHERE padre ISNULL ORDER BY idc_coste");
    while (!cursoraux1->eof()) {
        idc_coste = cursoraux1->valor("idc_coste").toInt();
        it = new Q3CheckListItem(m_listCostes, "hola", Q3CheckListItem::CheckBox);
        Lista[idc_coste] = it;
        it->setText(m_colIdCoste, cursoraux1->valor("idc_coste"));
        it->setText(m_colDescCoste, cursoraux1->valor("descripcion"));
        it->setText(m_colNomCoste, cursoraux1->valor("nombre"));
        it->setOpen(true);
        cursoraux1->siguienteregistro ();
    } // end while
    delete cursoraux1;
    /// Una vez que hemos puesto los centros de coste padre, todo lo dem&aacute;s es una
    /// Tarea de ir colocando centros de coste a sus respectivos
    /// deja de ser recursivo y pasa a ser lineal.
    cursoraux2 = empresaactual->cargacursor("SELECT * FROM c_coste WHERE padre IS NOT NULL ORDER BY idc_coste");
    while (!cursoraux2->eof()) {
        padre = cursoraux2->valor("padre").toInt();
        idc_coste = cursoraux2->valor("idc_coste").toInt();
        fprintf(stderr, "Cuentas de subnivel:%d", padre);
        it = new Q3CheckListItem(Lista[padre], "hola", Q3CheckListItem::CheckBox);
        Lista[idc_coste] = it;
        it->setText(m_colIdCoste, cursoraux2->valor("idc_coste"));
        it->setText(m_colDescCoste, cursoraux2->valor("descripcion"));
        it->setText(m_colNomCoste, cursoraux2->valor("nombre"));
        it->setOpen(true);
        cursoraux2->siguienteregistro();
    } // end while
    delete cursoraux2;
}

/// Esta funci&oaqcute;n devuelve el primer centro de coste seleccionado de la vista.
/// Devuelve el idc_coste. Si no hay ning&uacute;n centro de coste seleccionado devuelve
/// cero
int selectccosteview::firstccoste() {
    delete m_iterador;
    m_iterador = new Q3ListViewItemIterator(m_listCostes);
    int idccoste = 0;
    Q3CheckListItem *item;
    fprintf(stderr, "nextccoste\n");
    while (m_iterador->current() && idccoste == 0) {
        item = (Q3CheckListItem *) m_iterador->current();
        if (item->isOn()) {
            idccoste = item->text(m_colIdCoste).toInt();
            fprintf(stderr, "primer centro de coste:%d\n", idccoste);
            return idccoste;
        } // end if
        (*m_iterador)++;
    } // end while
    return idccoste;
}


/// Esta funci&oacute;n devuelve el siguiente centro de coste seleccionado de la vista.
int selectccosteview::nextccoste() {
    int idccoste = 0;
    Q3CheckListItem *item;
    fprintf(stderr, "nextccoste\n");
    while (m_iterador->current() && idccoste == 0) {
        item = (Q3CheckListItem *) m_iterador->current();
        if (item->isOn()) {
            idccoste = item->text(m_colIdCoste).toInt();
            fprintf(stderr, "primer centro de coste:%d\n", idccoste);
            return idccoste;
        } // end if
        (*m_iterador)++;
    } // end while
    return idccoste;
}


/// Esta funci&oacute;n prepara una lista separada por comas de los costes seleccionados.
/// Sirve para generar sentencias SQL.
QString selectccosteview::cadcoste() {
    int idc_coste;
    QString ccostes = "";
    idc_coste = firstccoste();
    while (idc_coste) {
        if (ccostes != "")
            ccostes.sprintf("%s, %d", ccostes.toAscii().constData(), idc_coste);
        else
            ccostes.sprintf("%d", idc_coste);
        idc_coste = nextccoste();
    } /// end while
    fprintf(stderr, " cadcoste: %s\n", ccostes.toAscii().constData());
    return ccostes;
}


/// Esta funci&oacute;n devuelve el nombre del centro de coste actual
/// Si no existe devuelve ""
QString selectccosteview::nomcoste() {
    Q3CheckListItem *item;
    fprintf(stderr, "nomcoste()\n");
    item = (Q3CheckListItem *) m_iterador->current();
    if (item->isOn()) {
        fprintf(stderr, "nomcoste: %s\n", item->text(m_colNomCoste).toAscii().constData());
        return item->text(m_colNomCoste);
    } else {
        return "";
    } // end if
}


void selectccosteview::boton_todo() {
    Q3ListViewItemIterator *m_iterador;
    m_iterador = new Q3ListViewItemIterator(m_listCostes);
    Q3CheckListItem *item;
    while (m_iterador->current()) {
        item = (Q3CheckListItem *) m_iterador->current();
        item->setOn(TRUE);
        (*m_iterador)++;
    }
    delete m_iterador;
}


void selectccosteview::boton_nada() {
    Q3ListViewItemIterator *m_iterador;
    m_iterador = new Q3ListViewItemIterator(m_listCostes);
    Q3CheckListItem *item;
    while (m_iterador->current()) {
        item = (Q3CheckListItem *) m_iterador->current();
        item->setOn(FALSE);
        (*m_iterador)++;
    }
    delete m_iterador;
}


void selectccosteview::boton_invertir() {
    Q3ListViewItemIterator *m_iterador;
    m_iterador = new Q3ListViewItemIterator(m_listCostes);
    Q3CheckListItem *item;
    while (m_iterador->current()) {
        item = (Q3CheckListItem *) m_iterador->current();
        if (item->isOn())
            item->setOn(FALSE);
        else
            item->setOn(TRUE);
        (*m_iterador)++;
    } //end while
    delete m_iterador;
}

