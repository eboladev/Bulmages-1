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
   idamortizacion = "";
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

void amortizacionview::inicializa(QString idamortiza) {
	idamortizacion = idamortiza;
	fprintf(stderr,"Inicializamos el formulario %s\n", idamortizacion.latin1());
   QString query = "SELECT * FROM amortizacion WHERE idamortizacion="+idamortizacion;
   
   conexionbase->begin();
   cursor2 *curs=conexionbase->cargacursor(query,"unquery");
   conexionbase->commit();
   
   // Si existe el registro que se pasa como parámetro.
   if (!curs->eof()) {
   	nomamortizacion->setText(curs->valor("nomamortizacion"));
      valorcompra->setText(curs->valor("valorcompra"));
      numcuotas->setText(curs->valor("numcuotas"));
   }// end if
   delete curs;
}// end inicializa

