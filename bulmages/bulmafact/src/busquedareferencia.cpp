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

#include "busquedareferencia.h"
#include "company.h"
#include "funcaux.h"
#include "informereferencia.h"


BusquedaReferencia::BusquedaReferencia(QWidget *parent, const char *name)
        : QWidget(parent, name) {
    _depura("BusquedaReferencia::BusquedaReferencia", 0);
    setupUi(this);
    m_companyact = NULL;
    m_semaforo = FALSE;
    _depura("END BusquedaReferencia::BusquedaReferencia", 0);
}


BusquedaReferencia::~BusquedaReferencia() {}






/// Busqueda de clientes.
void BusquedaReferencia::on_mui_informe_clicked() {
    _depura("BusquedaReferencia::on_mui_buscar_clicked\n", 0);
    InformeReferencia *inf = new InformeReferencia(m_companyact);
    inf->setreferencia(mui_referencia->text());
    inf->generarinforme();
    delete inf;
    _depura("END BusquedaReferencia::on_mui_buscar_clicked\n", 0);

}


void BusquedaReferencia::on_mui_referencia_editingFinished() {
    _depura("BusquedaReferencia::on_m_cifcliente_editingFinished", 0);
    emit(valueChanged(mui_referencia->text()));
    _depura("END BusquedaReferencia::on_m_cifcliente_editingFinished", 0);

}


