/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include <QComboBox>

#include "busquedatrabajador.h"
#include "company.h"


/** Inicializa todos los componentes a NULL para que no haya confusiones sobre
    si un elemento ha sido inicializado o no.
    Hace la conexion del SIGNAL activated con m_activated para tratar el evento.
*/
BusquedaTrabajador::BusquedaTrabajador(QWidget *parent)
        : QComboBox(parent) {
    _depura("BusquedaTrabajador::BusquedaTrabajador", 0);
    companyact = NULL;
    m_cursorcombo = NULL;
    connect(this, SIGNAL(activated(int)), this, SLOT(m_activated(int)));
    _depura("END BusquedaTrabajador::BusquedaTrabajador", 0);
}


/** Libera la memoria dinamica que se estaba utilizando.
*/
BusquedaTrabajador::~BusquedaTrabajador() {
    _depura("BusquedaTrabajador::~BusquedaTrabajador", 0);
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    _depura("END BusquedaTrabajador::~BusquedaTrabajador", 0);
}


/** Con este metodo se puede indicar al Widget cual es el trabajador seleccionado.
    Recarga el cursor de trabajadores y cuando encuentra uno con el mismo identificador
    que el que se ha pasado por parametro lo establece como elemento seleccionado.
*/
void BusquedaTrabajador::setidtrabajador(QString idtrabajador) {
    _depura("BusquedaTrabajador::setidtrabajador", 0);
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    m_cursorcombo = companyact->cargacursor("SELECT * FROM trabajador");
    int i = 0;
    int i1 = 0;
    clear();
    addItem("--");
    while (!m_cursorcombo->eof()) {
        i ++;
        if (m_cursorcombo->valor("idtrabajador") == idtrabajador)
            i1 = i;
        addItem(m_cursorcombo->valor("apellidostrabajador") + ", " + m_cursorcombo->valor("nomtrabajador"));
        m_cursorcombo->siguienteregistro();
    } // end while
    setCurrentIndex(i1);
    _depura("END BusquedaTrabajador::setidtrabajador", 0);
}


/** Inicializa la clase con el puntero a la empresa que se esta utilizando.
**/
void BusquedaTrabajador::setcompany(company *comp) {
    _depura("BusquedaTrabajador::setcompany", 0);
    companyact = comp;
    _depura("END BusquedaTrabajador::setcompany", 0);
}

/** Devuelve el identificador del trabajador seleccionado
**/
QString BusquedaTrabajador::idtrabajador() {
    _depura("BusquedaTrabajador::idtrabajador", 0);
    _depura("END BusquedaTrabajador::idtrabajador", 0);
    return m_cursorcombo->valor("idtrabajador", currentIndex() - 1);
}

/** SLOT que responde a la activacion de un elemento en el QComboBox y que hace que se emita el SIGNAL valueChanged
**/
void BusquedaTrabajador::m_activated(int index) {
    _depura("BusquedaTrabajador::m_activated", 0);
    if (index > 0) {
        emit(valueChanged(m_cursorcombo->valor("idtrabajador", index - 1)));
    } else {
        emit(valueChanged(""));
    }
    _depura("END BusquedaTrabajador::m_activated", 0);
}


