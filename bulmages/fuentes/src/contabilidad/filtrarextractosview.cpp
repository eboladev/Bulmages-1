/***************************************************************************
                          filtrarextractosview.cpp  -  description
                             -------------------
    begin                : dom jul 27 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera
    email                : tborras@conetxia.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "filtrarextractosview.h"
#include "empresa.h"

filtrarextractosview::filtrarextractosview(empresa *emp,QWidget *parent, const char *name ) : filtrarextractosdlg(parent,name) {
   fprintf(stderr,"Constructor de filtrarextractosview\n");
   empresaactual = emp;
   conexionbase = empresaactual->bdempresa();
   numdigitos = empresaactual->numdigitosempresa();
   fprintf(stderr,"Fin del constructor de fitrarextractosview\n");
}// end filtrarextractosview


filtrarextractosview::~filtrarextractosview(){
}

void filtrarextractosview::boton_ccostes() {
   fprintf(stderr,"Boton ccostes\n");
   selectccosteview *selccostes = empresaactual->getselccostes();
   selccostes->exec();
}// end boton_ccostes


void filtrarextractosview::boton_canales() {
   fprintf(stderr,"Boton canales\n");
   selectcanalview *selcanales = empresaactual->getselcanales();
   selcanales->exec();
}// end boton_ccostes

void filtrarextractosview::codigo_textChanged(const QString &texto) {
    QLineEdit *codigo = (QLineEdit *) sender();
    if (texto == "+") {
        // Hacemos aparecer la ventana de cuentas
        listcuentasview1 *listcuentas = new listcuentasview1(empresaactual);
        listcuentas->modo=1;
        listcuentas->inicializa();
        listcuentas->exec();
        codigo->setText(listcuentas->codcuenta);
        delete listcuentas;
    }// end if
}// end codigo_textChanged

void filtrarextractosview::boton_buscacontrapartida() {
   listcuentasview1 *listcuentas = new listcuentasview1(empresaactual);
   listcuentas->modo=1;
   listcuentas->inicializa();
   listcuentas->exec();
   codigocontrapartida->setText(listcuentas->codcuenta);
   delete listcuentas;
}// end boton_buscacuentainicial


void filtrarextractosview::return_codigo() {
   QString cad = codigocontrapartida->text();
   if (cad != "") {
      cad = extiendecodigo(cad,numdigitos);
      conexionbase->begin();
      cursor2 *cursorcta = conexionbase->cargacuenta(0, cad );
      conexionbase->commit();
      int num = cursorcta->numregistros();
      if (num >0) {
         codigocontrapartida->setText(cursorcta->valor(1));
      } else {
        codigocontrapartida->selectAll();
        codigocontrapartida->setFocus();
      }// end if
      delete cursorcta;
   }// end if
}// end return_codigofinal



