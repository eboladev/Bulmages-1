//
//
// C++ Implementation: $MODULE$
//
// Description: 
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2003
// Copyright: See COPYING file that comes with this distribution
//
//
#include "amortizacionview.h"
#include "empresa.h"

amortizacionview::amortizacionview(empresa *emp, QWidget *parent, const char *name, bool flag ) 
: amortizaciondlg(parent,name,flag,0) {
	empresaactual = emp;
   conexionbase = empresaactual->bdempresa();
}

void amortizacionview::accept() {
	fprintf(stderr,"Vamos a hacer un accept\n");
	QString namortizacion = nomamortizacion->text();
	QString query = "INSERT INTO amortizacion (nomamortizacion) VALUES('"+namortizacion+"')";
	conexionbase->begin();
	conexionbase->ejecuta(query);
	conexionbase->commit();
	done(1);
}// end accept

amortizacionview::~amortizacionview() {
}
