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
}

cambiactaview::~cambiactaview() {
}

void cambiactaview::boton_buscactaorigen() {
	QDialog *diag = new QDialog(0);
	diag->setModal(true);
	listcuentasview1 *listcuentas = new listcuentasview1(empresaactual, diag, tr("Seleccione cuenta", "company"),0, listcuentasview1::SelectMode);
	connect(listcuentas, SIGNAL(selected(QString)), diag, SLOT(accept()));
	diag->exec();
	if (listcuentas->codcuenta() != "") {
		codigoorigen->setText(listcuentas->codcuenta());
	} // end if
	delete diag;
}


void cambiactaview::boton_buscactadestino() {
	QDialog *diag = new QDialog(0);
	diag->setModal(true);
	listcuentasview1 *listcuentas = new listcuentasview1(empresaactual, diag, tr("Seleccione cuenta", "company"),0, listcuentasview1::SelectMode);
	connect(listcuentas, SIGNAL(selected(QString)), diag, SLOT(accept()));
	diag->exec();
	if (listcuentas->codcuenta() != "") {
		codigodestino->setText(listcuentas->codcuenta());
	} // end if
	delete diag;
}


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

	
	// Modificamos las contrapartidas de los borradores
	QString query2 = "UPDATE borrador SET contrapartida = id_cuenta('"+destino+"') WHERE contrapartida = id_cuenta('"+origen+"')";
	if (ainicial != "") {
		query2 = query2 + " AND idasiento IN (SELECT idasiento FROM asiento WHERE ordenasiento >= "+ainicial+")";
	}// end if
	if (afinal != "") {
		query2 = query2 + " AND idasiento IN (SELECT idasiento FROM asiento WHERE ordenasiento <= "+afinal+")";
	}// end if
	if (finicial != "") {
		query2 = query2 + " AND fecha >= '"+finicial+"'";
	}// end if
	if (ffinal != "") {
		query2 = query2 + " AND fecha <= '"+ffinal+"'";
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
		query1 = query1 + " AND fecha >= '"+finicial+"'";
	}// end if
	if (ffinal != "") {
		query1 = query1 + " AND fecha <= '"+ffinal+"'";
	}// end if
	
	// Modificamos también las contrapartidas de los apuntes
	QString query3 = "UPDATE apunte SET contrapartida = id_cuenta('"+destino+"') WHERE contrapartida = id_cuenta('"+origen+"')";
	if (ainicial != "") {
		query3 = query3 + " AND idasiento IN (SELECT idasiento FROM asiento WHERE ordenasiento >= "+ainicial+")";
	}// end if
	if (afinal != "") {
		query3 = query3 + " AND idasiento IN (SELECT idasiento FROM asiento WHERE ordenasiento <= "+afinal+")";
	}// end if
	if (finicial != "") {
		query3 = query3 + " AND fecha >= '"+finicial+"'";
	}// end if
	if (ffinal != "") {
		query3 = query3 + " AND fecha <= '"+ffinal+"'";
	}// end if
	
	// Modificamos los registros de IVA
	QString query4 = "UPDATE registroiva SET contrapartida = id_cuenta('"+destino+"') WHERE contrapartida = id_cuenta('"+origen+"')";
	if (ainicial != "") {
		query4 = query4 + " AND idborrador IN (SELECT idborrador FROM borrador,asiento WHERE ordenasiento >= "+ainicial+" AND borrador.idasiento = asiento.idasiento)";
	}// end if
	if (afinal != "") {
		query4 = query4 + " AND idborrador IN (SELECT idborrador FROM borrador,asiento WHERE ordenasiento <= "+afinal+" AND borrador.idasiento = asiento.idasiento)";
	}// end if
	if (finicial != "") {
		query4 = query4 + " AND ffactura >= '"+finicial+"'";
	}// end if
	if (ffinal != "") {
		query4 = query4 + " AND ffactura <= '"+ffinal+"'";
	}// end if
	
	CONEXIONBASE->begin();
	CONEXIONBASE->ejecuta(query);
	CONEXIONBASE->ejecuta(query1);
	CONEXIONBASE->ejecuta(query2);
	CONEXIONBASE->ejecuta(query3);
	CONEXIONBASE->ejecuta(query4);
	CONEXIONBASE->commit();
	
	done(1);
}


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
}


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
}


void cambiactaview::return_fechainicial() {
  fechainicial->setText(normalizafecha(fechainicial->text()).toString("dd/MM/yyyy"));
}



void cambiactaview::return_fechafinal() {
  fechafinal->setText(normalizafecha(fechafinal->text()).toString("dd/MM/yyyy"));
}

void cambiactaview::codigo_textChanged(const QString &texto) {
    QLineEdit *codigo = (QLineEdit *) sender();
    if (texto == "+") {
        // Hacemos aparecer la ventana de cuentas
	QDialog *diag = new QDialog(0);
	diag->setModal(true);
	listcuentasview1 *listcuentas = new listcuentasview1(empresaactual, diag, tr("Seleccione cuenta", "company"),0, listcuentasview1::SelectMode);
	connect(listcuentas, SIGNAL(selected(QString)), diag, SLOT(accept()));
	diag->exec();
	if (listcuentas->codcuenta() != "") {
	        codigo->setText(listcuentas->codcuenta());
	} // end if
	delete diag;
    }// end if
}

void cambiactaview::fecha_textChanged( const QString & texto ) {
	QLineEdit *fecha = (QLineEdit *) sender();
    if (texto=="+") {
        Q3PtrList<QDate> a;
        fecha->setText("");
        calendario *cal = new calendario(0,0);
        cal->exec();
        a = cal->dn->selectedDates();
        fecha->setText(a.first()->toString("dd/MM/yyyy"));
        delete cal;
    }
    if (texto=="*")
        fecha->setText(QDate::currentDate().toString("dd/MM/yyyy") );
}



