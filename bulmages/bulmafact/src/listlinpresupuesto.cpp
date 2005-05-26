//
// C++ Implementation: listlinpresupuesto
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listlinpresupuesto.h"
#include "company.h"
#include "linpresupuesto.h"


listlinpresupuesto::listlinpresupuesto(company *comp) {
    companyact = comp;
}

listlinpresupuesto::~listlinpresupuesto() {}


// Carga líneas de presupuesto
void listlinpresupuesto::chargeBudgetLines(QString idbudget) {
	companyact->begin();
	cursor2 * cur= companyact->cargacursor("SELECT * FROM lpresupuesto, articulo WHERE idpresupuesto="+idbudget+" AND articulo.idarticulo=lpresupuesto.idarticulo","unquery");
	companyact->commit();
	int i=0;
	while (!cur->eof()) {
		/// Creamos un elemento del tipo linpresupuesto y lo agregamos a la lista.
		linpresupuesto *lin = new linpresupuesto(companyact,
		cur->valor("idlpresupuesto"),
		cur->valor("desclpresupuesto"),
		cur->valor("cantlpresupuesto"),
		cur->valor("pvplpresupuesto"),
		cur->valor("descuentolpresupuesto"),
		cur->valor("idpresupuesto"),
		cur->valor("idarticulo"),
		cur->valor("codigocompletoarticulo"),
		cur->valor("nomarticulo")
		);
		m_lista.append(lin);
		i++;
		cur->siguienteregistro();
	}// end while
	delete cur;
}// end chargeBudgetLines
