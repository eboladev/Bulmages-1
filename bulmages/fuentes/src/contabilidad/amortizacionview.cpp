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
   idamortizacion = "";
}

void amortizacionview::accept() {
	QString query;
	QString namortizacion = nomamortizacion->text();
	double valorcompradbl = valorcompra->text().toDouble();
	int numcuotasint = numcuotas->text().toInt();
	QString fechacomprastr = fechacompra->text();

	
	fprintf(stderr,"Vamos a hacer un accept\n");
	if (idamortizacion == "") {
		fprintf(stderr,"Se trata de una inserción");
		query.sprintf("INSERT INTO amortizacion (nomamortizacion, valorcompra, numcuotas, fechacompra) VALUES ('%s', %f, %d, '%s')", namortizacion.ascii(), valorcompradbl, numcuotasint, fechacomprastr.ascii());
		conexionbase->begin();
		conexionbase->ejecuta(query);
		conexionbase->commit();
		done(1);
	} else {
		fprintf(stderr,"Se trata de una modificación\n");
		query.sprintf("UPDATE amortizacion SET nomamortizacion='%s', valorcompra=%f, numcuotas=%d, fechacompra='%s' WHERE idamortizacion=%s", namortizacion.ascii(), valorcompradbl, numcuotasint,fechacomprastr.ascii(), idamortizacion.ascii());
		fprintf(stderr,"El query es: %s\n", query.ascii());
		conexionbase->begin();
		conexionbase->ejecuta(query);
		conexionbase->commit();
		done(1);
	}// end if
}// end accept

amortizacionview::~amortizacionview() {
}

void amortizacionview::inicializa(QString idamortiza) {
	idamortizacion = idamortiza;
	fprintf(stderr,"Inicializamos el formulario %s\n", idamortizacion.ascii());
   QString query = "SELECT * FROM amortizacion WHERE idamortizacion="+idamortizacion;
   
   conexionbase->begin();
   cursor2 *curs=conexionbase->cargacursor(query,"unquery");
   conexionbase->commit();
   
   // Si existe el registro que se pasa como parámetro.
   if (!curs->eof()) {
      nomamortizacion->setText(curs->valor("nomamortizacion"));
      valorcompra->setText(curs->valor("valorcompra"));
      numcuotas->setText(curs->valor("numcuotas"));
      QString cadena;
      cadena.sprintf("%10.10s",curs->valor("fechacompra").ascii());
      fechacompra->setText(cadena);
      cadena.sprintf("%10.10s",curs->valor("fecha1cuota").ascii());
      fecha1cuota->setText(cadena);
   
   
   }// end if
   delete curs;
}// end inicializa

