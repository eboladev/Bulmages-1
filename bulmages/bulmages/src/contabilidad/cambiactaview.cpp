//
//
// C++ Implementation: $MODULE$
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "cambiactaview.h"
#include "listcuentasview1.h"
#include "calendario.h"


#define CONEXIONBASE empresaactual->bdempresa()
#define NUMDIGITOS   empresaactual->numdigitosempresa()

cambiactaview::cambiactaview(empresa *emp, QWidget *parent, const char *name, bool flag ) : cambiactadlg(parent,name,flag,0) {
	empresaactual = emp;
}// end cambiactaview

cambiactaview::~cambiactaview() {
}

void cambiactaview::boton_buscactaorigen() {
   listcuentasview1 *listcuentas = new listcuentasview1();
   listcuentas->modo=1;
   listcuentas->inicializa(CONEXIONBASE);
   listcuentas->exec();
   codigoorigen->setText(listcuentas->codcuenta);
   delete listcuentas;
}// end boton_buscacuentainicial


void cambiactaview::boton_buscactadestino() {
   listcuentasview1 *listcuentas = new listcuentasview1();
   listcuentas->modo=1;
   listcuentas->inicializa(CONEXIONBASE);
   listcuentas->exec();
   codigodestino->setText(listcuentas->codcuenta);
   delete listcuentas;
}// end boton_buscacuentafinal

void cambiactaview::accept() {
	QString origen = codigoorigen->text();
	QString destino = codigodestino->text();
	QString ainicial = asientoinicial->text();
	QString afinal = asientofinal->text();
	QString finicial = fechainicial->text();
	QString ffinal = fechafinal->text();
	
	// Modificamos los borradores
	QString query = "UPDATE borrador SET idcuenta = id_cuenta('"+destino+"') WHERE idcuenta = id_cuenta('"+origen+"')";
	if (ainicial != "") {
		query = query + " AND idasiento IN (SELECT idasiento FROM asiento WHERE ordenasiento >= "+ainicial+")";
	}// end if
	if (afinal != "") {
		query = query + " AND idasiento IN (SELECT idasiento FROM asiento WHERE ordenasiento <= "+afinal+")";
	}// end if
	if (finicial != "") {
		query = query + " AND fecha >= '"+finicial+"'";
	}// end if
	if (ffinal != "") {
		query = query + " AND fecha <= '"+ffinal+"'";
	}// end if
	
	// Modificamos también los apuntes
	QString query1 = "UPDATE apunte SET idcuenta = id_cuenta('"+destino+"') WHERE idcuenta = id_cuenta('"+origen+"')";
	if (ainicial != "") {
		query1 = query1 + " AND idasiento IN (SELECT idasiento FROM asiento WHERE ordenasiento >= "+ainicial+")";
	}// end if
	if (afinal != "") {
		query1 = query1 + " AND idasiento IN (SELECT idasiento FROM asiento WHERE ordenasiento <= "+afinal+")";
	}// end if
	if (finicial != "") {
		query = query + " AND fecha >= '"+finicial+"'";
	}// end if
	if (ffinal != "") {
		query = query + " AND fecha <= '"+ffinal+"'";
	}// end if
	
	CONEXIONBASE->begin();
	CONEXIONBASE->ejecuta(query);
	CONEXIONBASE->ejecuta(query1);
	CONEXIONBASE->commit();
	
	done(1);
}// end accept


void cambiactaview::return_codigoinicial() {
   QString cad = codigoorigen->text();
   if (cad != "") {
      cad = extiendecodigo(cad,NUMDIGITOS);
      CONEXIONBASE->begin();
      cursor2 *cursorcta = CONEXIONBASE->cargacuenta(0, cad );
      CONEXIONBASE->commit();
      int num = cursorcta->numregistros();
      if (num >0) {
         codigoorigen->setText(cursorcta->valor(1));
         // Simulamos la pulsacion del boton recargar.
      } else {
        codigoorigen->selectAll();
        codigoorigen->setFocus();
      }// end if
      delete cursorcta;
   }// end if
}// end return_codigoinicial


void cambiactaview::return_codigofinal() {
   QString cad = codigodestino->text();
   if (cad != "") {
      cad = extiendecodigo(cad,NUMDIGITOS);
      CONEXIONBASE->begin();
      cursor2 *cursorcta = CONEXIONBASE->cargacuenta(0, cad );
      CONEXIONBASE->commit();
      int num = cursorcta->numregistros();
      if (num >0) {
         codigodestino->setText(cursorcta->valor(1));
      } else {
        codigodestino->selectAll();
        codigodestino->setFocus();
      }// end if
      delete cursorcta;
   }// end if
}// end return_codigofinal


void cambiactaview::return_fechainicial() {
  fechainicial->setText(normalizafecha(fechainicial->text()).toString("dd/MM/yyyy"));
}// end return_fechainicial



void cambiactaview::return_fechafinal() {
  fechafinal->setText(normalizafecha(fechafinal->text()).toString("dd/MM/yyyy"));
}// end return_fechafinal

void cambiactaview::codigo_textChanged(const QString &texto) {
    QLineEdit *codigo = (QLineEdit *) sender();
    if (texto == "+") {
        // Hacemos aparecer la ventana de cuentas
        listcuentasview1 *listcuentas = new listcuentasview1();
        listcuentas->modo=1;
        listcuentas->inicializa(CONEXIONBASE);
        listcuentas->exec();
        codigo->setText(listcuentas->codcuenta);
        delete listcuentas;
    }// end if
}// end codigo_textChanged

void cambiactaview::fecha_textChanged( const QString & texto ) {
	QLineEdit *fecha = (QLineEdit *) sender();
    if (texto=="+") {
        QList<QDate> a;
        fecha->setText("");
        calendario *cal = new calendario(0,0);
        cal->exec();
        a = cal->dn->selectedDates();
        fecha->setText(a.first()->toString("dd/MM/yyyy"));
        delete cal;
    }
    if (texto=="*")
        fecha->setText(QDate::currentDate().toString("dd/MM/yyyy") );
}//fin fechaasiento1_textChanged( const QString &texto )



