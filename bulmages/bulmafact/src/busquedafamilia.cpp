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

#include "busquedafamilia.h"
#include "familiasview.h"
#include "company.h"


BusquedaFamilia::BusquedaFamilia(QWidget *parent, const char *name)
: QWidget(parent, name) {
    setupUi(this);
    companyact=NULL;
    mdb_idfamilia="";
    mdb_nombrefamilia="";
    mdb_codigocompletofamilia="";
}


BusquedaFamilia::~BusquedaFamilia() {}


void BusquedaFamilia::setidfamilia(QString val) {
    mdb_idfamilia=val;
    QString SQLQuery = "SELECT * FROM familia WHERE idfamilia='"+mdb_idfamilia+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_codigocompletofamilia = cur->valor("codigocompletofamilia");
        mdb_nombrefamilia = cur->valor("nombrefamilia");
    } else {
        mdb_idfamilia="";
        mdb_nombrefamilia="";
        mdb_codigocompletofamilia="";
    }// end if
    delete cur;
    m_codigocompletofamilia->setText(mdb_codigocompletofamilia);
    m_nombrefamilia->setText(mdb_nombrefamilia);
}


void BusquedaFamilia::setcodigocompletofamilia(QString val) {
    mdb_codigocompletofamilia=val;
    QString SQLQuery = "SELECT * FROM familia WHERE codigocompletofamilia='"+mdb_codigocompletofamilia+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idfamilia = cur->valor("idfamilia");
        mdb_nombrefamilia = cur->valor("nombrefamilia");
    } else {
        mdb_idfamilia="";
        mdb_nombrefamilia="";
    }// end if
    delete cur;
    m_codigocompletofamilia->setText(mdb_codigocompletofamilia);
    m_nombrefamilia->setText(mdb_nombrefamilia);
}


// Busqueda de familias.
void BusquedaFamilia::on_mui_buscar_clicked() {
	familiasview *fam = new familiasview(companyact, 0,0);
	fam->setModoConsulta();
	if (fam->exec() == 1) {
		m_codigocompletofamilia->setText(fam->codigoCompletoFamilia());
		mdb_codigocompletofamilia = fam->codigoCompletoFamilia();
		m_nombrefamilia->setText(fam->nombreFamilia());
		mdb_nombrefamilia = fam->nombreFamilia();
		mdb_idfamilia = fam->idFamilia();		
	}// end if
	delete fam;
    emit(valueChanged(mdb_idfamilia));	
}


void BusquedaFamilia::on_m_codigofamilia_textChanged(const QString &val) {
    mdb_codigocompletofamilia=val;
    QString SQLQuery = "SELECT * FROM familia WHERE codigocompletofamilia='"+mdb_codigocompletofamilia+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idfamilia = cur->valor("idfamilia");
        mdb_nombrefamilia = cur->valor("nombrefamilia");
    } else {
        mdb_idfamilia="";
        mdb_nombrefamilia="";
    }// end if
    delete cur;
    m_codigocompletofamilia->setText(mdb_codigocompletofamilia);
    m_nombrefamilia->setText(mdb_nombrefamilia);
    emit(valueChanged(mdb_idfamilia));
}

