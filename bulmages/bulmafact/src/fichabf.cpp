/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#include "fichabf.h"

class Fixed;



FichaBf::FichaBf(company *comp, QWidget *parent, Qt::WFlags f) : Ficha(parent, f), DBRecord(comp) {
    _depura("FichaBf::FichaBf", 0);
    m_listalineas = NULL;
    m_listadescuentos = NULL;
    companyact = comp;
    m_companyact = comp;
    _depura("END FichaBf::FichaBf", 0);
}

FichaBf::~FichaBf() {
	_depura("FichaBf::~FichaBf", 0);
	_depura("END FichaBf::~FichaBf", 0);
}


/** Calcula los totales de factura, descuentos e impuestos y invoca al metodo de pintaTotales para
    que se pinten dichos valores en la pantalla.
*/
void FichaBf::calculaypintatotales() {
    _depura("FichaBf::calculaypintatotales", 0);
    base basesimp;
    base basesimpreqeq;
    SDBRecord *linea;
    /// Impresion de los contenidos.
    QString l;
    Fixed irpf("0");

    cursor2 *cur = m_companyact->cargacursor("SELECT * FROM configuracion WHERE nombre='IRPF'");
    if (!cur->eof()) {
       irpf = Fixed(cur->valor("valor"));
    } // end if
    delete cur;

    for (int i = 0; i < m_listalineas->rowCount(); ++i) {
        linea = m_listalineas->lineaat(i);
        Fixed cant(linea->DBvalue("cant"+m_listalineas->tableName()).toAscii().constData());
        Fixed pvpund(linea->DBvalue("pvp"+m_listalineas->tableName()).toAscii().constData());
        Fixed desc1(linea->DBvalue("descuento"+m_listalineas->tableName()).toAscii().constData());
        Fixed cantpvp = cant * pvpund;
        Fixed base = cantpvp - cantpvp * desc1 / 100;
        basesimp[linea->DBvalue("iva"+m_listalineas->tableName())] = basesimp[linea->DBvalue("iva"+m_listalineas->tableName())] + base;
        basesimpreqeq[linea->DBvalue("reqeq"+m_listalineas->tableName())] = basesimpreqeq[linea->DBvalue("reqeq"+m_listalineas->tableName())] + base;
    } // end for

    Fixed basei("0.00");
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it) {
        basei = basei + it.value();
    } // end for

    /// Impresion de los descuentos.
    Fixed porcentt("0.00");
    SDBRecord *linea1;
    if (m_listadescuentos->rowCount()) {
        for (int i = 0; i < m_listadescuentos->rowCount(); ++i) {
            linea1 = m_listadescuentos->lineaat(i);
            Fixed propor(linea1->DBvalue("proporcion"+m_listadescuentos->tableName()).toAscii().constData());
            porcentt = porcentt + propor;
        } // end for
    } // end if

    /// Calculamos el total de base imponible.
    Fixed totbaseimp("0.00");
    Fixed parbaseimp("0.00");
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        if (porcentt > Fixed("0.00") ) {
            parbaseimp = it.value() - it.value() * porcentt / 100;
        } else {
            parbaseimp = it.value();
        } // end if
        totbaseimp = totbaseimp + parbaseimp;
    } // end for

    /// Calculamos el total de IVA.
    Fixed totiva("0.00");
    Fixed pariva("0.00");
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        Fixed piva(it.key().toAscii().constData());
        if (porcentt > Fixed("0.00")) {
            pariva = (it.value() - it.value() * porcentt / 100) * piva / 100;
        } else {
            pariva = it.value() * piva / 100;
        } // end if
        totiva = totiva + pariva;
    } // end for


    /// Calculamos el total de recargo de equivalencia.
    Fixed totreqeq("0.00");
    Fixed parreqeq("0.00");
    for (it = basesimpreqeq.begin(); it != basesimpreqeq.end(); ++it) {
        Fixed preqeq(it.key().toAscii().constData());
        if (porcentt > Fixed("0.00")) {
            parreqeq = (it.value() - it.value() * porcentt / 100) * preqeq / 100;
        } else {
            parreqeq = it.value() * preqeq / 100;
        } // end if
        totreqeq = totreqeq + parreqeq;
    } // end for

    Fixed totirpf = totbaseimp * irpf / 100;

    pintatotales(totiva, totbaseimp, totiva+totbaseimp+totreqeq-totirpf, basei * porcentt / 100, totirpf, totreqeq);

    _depura("FichaBf::calculaypintatotales", 0);
}
