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
#include "funcaux.h"
#include "aplinteligentesview.h"


#define COL_FECHA                  0
#define COL_CUOTA                  1
#define COL_ASIENTO                2
#define COL_EJERCICIO              3
#define COL_IDLINAMORTIZACION      4

amortizacionview::amortizacionview(empresa *emp, QWidget *parent, const char *name, bool flag ) 
: amortizaciondlg(parent,name,flag,0) {
      empresaactual = emp;
      conexionbase = empresaactual->bdempresa();
      idamortizacion = "";
      QString query = "SELECT * FROM ainteligente, configuracion WHERE descripcion=valor AND configuracion.nombre='Amortizacion'";
      conexionbase->begin();
      cursor2 *cur = conexionbase->cargacursor(query,"hola");
      conexionbase->commit();
      if (!cur->eof()) {
         idainteligente = cur->valor("idainteligente");
         fprintf(stderr,"El asiento de amortización es:%s\n", idainteligente.ascii());
      }// end if
      delete cur;
      table1->hideColumn(COL_IDLINAMORTIZACION);
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
                query.sprintf("SELECT max(idamortizacion) AS idamortizacion FROM amortizacion)");
                conexionbase->begin();
                cursor2 *cur = conexionbase->cargacursor(query,"unquery");
                conexionbase->commit();
                if (!cur->eof()) {
                   idamortizacion = cur->valor("idamortizacion");
                }// end if
                //Iteramos para cada linea en el subformulario.
                for(int i=0; i<table1->numRows(); i++) {
                   //Insertamos en la base de datos cada linea de amortizacion.
                   query.sprintf("INSERT INTO linamortizacion (idamortizacion, fechaprevista, cantidad) VALUES (%s, %s, %s)", idamortizacion.ascii(), table1->text(i,COL_FECHA).ascii(), table1->text(i, COL_CUOTA).ascii());
                   conexionbase->begin();
                   conexionbase->ejecuta(query);
                   conexionbase->commit();
                }// end for
		done(1);
	} else {
		fprintf(stderr,"Se trata de una modificación\n");
		query.sprintf("UPDATE amortizacion SET nomamortizacion='%s', valorcompra=%f, numcuotas=%d, fechacompra='%s' WHERE idamortizacion=%s", namortizacion.ascii(), valorcompradbl, numcuotasint,fechacomprastr.ascii(), idamortizacion.ascii());
		fprintf(stderr,"El query es: %s\n", query.ascii());
		conexionbase->begin();
		conexionbase->ejecuta(query);
		conexionbase->commit();
                conexionbase->begin();
                conexionbase->ejecuta("DELETE FROM linamortizacion WHERE idamortizacion="+idamortizacion);
                conexionbase->commit();
                //Iteramos para cada linea en el subformulario.
                for(int i=0; i<table1->numRows(); i++) {
                   //Insertamos en la base de datos cada linea de amortizacion.
                   query.sprintf("INSERT INTO linamortizacion (idamortizacion, fechaprevista, cantidad) VALUES (%s, '%s', %s)", idamortizacion.ascii(), table1->text(i,COL_FECHA).ascii(), table1->text(i, COL_CUOTA).ascii());
                   conexionbase->begin();
                   conexionbase->ejecuta(query);
                   conexionbase->commit();
                }// end for
                
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
   
   query = "SELECT * FROM linamortizacion WHERE idamortizacion ="+idamortizacion;
   conexionbase->begin();
   curs = conexionbase->cargacursor(query, "otroquery");
   conexionbase->commit();
   table1->setNumRows(curs->numregistros());
   for (int i=0; i<curs->numregistros(); i++) {
      table1->setText(i,COL_FECHA,curs->valor("fechaprevista"));
      table1->setText(i,COL_CUOTA, curs->valor("cantidad"));
      curs->siguienteregistro();
   }// end for
}// end inicializa


void amortizacionview::calculaamortizacion() {
   // Para hacer el cálculo de los plazos de cada amortizacion
   // Hay que obtener diversos datos.
   QDate f1cuota = normalizafecha(fecha1cuota->text());
   int ncuotas = numcuotas->text().toInt();
   QString periodicidadtxt = periodicidad->currentText();
   table1->setNumRows(ncuotas);
   double valcuota = valorcompra->text().toDouble() / ncuotas;
   QString valcuotastr ;
   valcuotastr.sprintf("%10.2f",valcuota);
   for (int i = 0; i<ncuotas; i++) {
      fprintf(stderr,"calculo de una cuota %s\n", f1cuota.toString("dd/MM/yyyy").ascii());
      table1->setText(i,0,f1cuota.toString("dd/MM/yyyy"));
      table1->setText(i,1,valcuotastr);
      // Dependiendo de la periodicidad actualizamos la fecha.
      if (periodicidadtxt == tr("Anual")) {
         f1cuota = f1cuota.addYears(1);
      } else if (periodicidadtxt == tr("Mensual")) {
         f1cuota = f1cuota.addMonths(1);
      } else if (periodicidadtxt == tr("Semestral")) {
         f1cuota = f1cuota.addMonths(6);
      } else if (periodicidadtxt == tr("Trimestral")) {
         f1cuota = f1cuota.addMonths(3);
      }// end if
   }// end for
}// end calculaamortizacion


void amortizacionview::cambiofechacompra() {
  fechacompra->setText(normalizafecha(fechacompra->text()).toString("dd/MM/yyyy"));
}// end cambiofechacompra


void amortizacionview::cambiofecha1cuota() {
  fecha1cuota->setText(normalizafecha(fecha1cuota->text()).toString("dd/MM/yyyy"));
}// end cambiofechacompra



// Datos para el asiento de amortización.
//El asiento inteligente debe llamarse como el parametro Amortizacion de confpr.
// $cuenta$ Cuenta de Amortización.
// $cuentabien$ Cuenta del Bien.
// $cuota$ Cuota a pagar.
void amortizacionview::contextMenuRequested(int row, int col, const QPoint &poin) {
   QPopupMenu *popup;
   popup = new QPopupMenu;
   int opcion;
   popup->insertItem(tr("Generar Asiento"),4);
   popup->insertSeparator();
   popup->insertItem(tr("Ver Asiento"),1);
/*   popup->insertItem(tr("--"),6);
   popup->insertSeparator();
   popup->insertItem(tr("--"),2);
   popup->insertItem(tr("--"),3);
*/
   opcion = popup->exec(poin);
   delete popup;
   
   if (opcion == 4) {
      QString fecha= table1->text(row,COL_FECHA);
      fprintf(stderr,"Fecha: %s\n", fecha.ascii());
      QString cant= table1->text(row, COL_CUOTA);
      fprintf(stderr,"Cuota: %s\n", cant.ascii());
      int numasiento = 0; //El asiento debe ser uno nuevo.
      aplinteligentesview *nueva=new aplinteligentesview(0,"");
      nueva->inicializa(conexionbase, numasiento, empresaactual->intapuntsempresa());
      nueva->muestraplantilla(idainteligente.toInt());
      nueva->setvalores("$cuenta$","19000");
      nueva->setvalores("$cuentabien$","999999");
      nueva->setvalores("$cuota$", "900");
      nueva->setvalores("$fechaasiento$",table1->text(row,COL_FECHA));
      nueva->setvalores("$cuota$",table1->text(row,COL_CUOTA));    
      nueva->exec();
      delete nueva;
   }// end if
}// end contextMenuRequested

