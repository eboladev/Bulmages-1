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
   
   // Hacemos la carga de los centros de coste. Rellenamos el combobox
   // Esto es obsoleto pq hay un formulario que se encarga de ello.
   // cargacostes();
 
   fprintf(stderr,"Fin del constructor de fitrarextractosview\n");
}// end filtrarextractosview


filtrarextractosview::~filtrarextractosview(){
}

void filtrarextractosview::boton_ccostes() {
   fprintf(stderr,"Boton ccostes\n");
   selectccosteview *selccostes = empresaactual->getselccostes();
   selccostes->exec();
   fprintf(stderr,"---------------INICIO---------------------\n");
   selccostes->firstccoste();
   while (selccostes->nextccoste());
   fprintf(stderr,"-----------------FIN---------------------\n");
//   selccostes->show();
}// end boton_ccostes


void filtrarextractosview::boton_canales() {
   fprintf(stderr,"Boton canales\n");
   selectcanalview *selcanales = empresaactual->getselcanales();
   selcanales->exec();
   fprintf(stderr,"---------------INICIO---------------------\n");
   selcanales->firstcanal();
   while (selcanales->nextcanal());
   fprintf(stderr,"-----------------FIN---------------------\n");
//   selccostes->show();
}// end boton_ccostes


void filtrarextractosview::cargacostes() {
   // Hacemos la carga de los centros de coste. Rellenamos el combobox correspondiente.
   combocoste->clear();
   QString query="SELECT * FROM c_coste ORDER BY nombre";
   conexionbase->begin();
   cursor2 *cursorcoste = conexionbase->cargacursor(query,"costes");
   conexionbase->commit();
   fprintf(stderr,"Terminada la operativa con la base de datos\n");
   combocoste->insertItem("--",0);
   ccostes[0]=0;
   int i=1;
   fprintf(stderr,"Vamos a iterear\n");
   while (!cursorcoste->eof()) {
      combocoste->insertItem(cursorcoste->valor(2),-1);
      ccostes[i++] = atoi(cursorcoste->valor(0).ascii());
      cursorcoste->siguienteregistro();
   }// end while
   delete cursorcoste;
   fprintf(stderr,"Terminada la carga de los centros de coste en el combobox\n");
   
}// end cargacostes



void filtrarextractosview::setccoste(int idc_coste) {
   // Establecemos el centro de coste correspondiente.
   int i=0;
   while (ccostes[i]!=idc_coste && i<100) i++;
   if (i<100) combocoste->setCurrentItem(i);
}// end if


void filtrarextractosview::codigo_textChanged(const QString &texto) {
    QLineEdit *codigo = (QLineEdit *) sender();
    if (texto == "+") {
        // Hacemos aparecer la ventana de cuentas
        listcuentasview1 *listcuentas = new listcuentasview1();
        listcuentas->modo=1;
        listcuentas->inicializa(conexionbase);
        listcuentas->exec();
        codigo->setText(listcuentas->codcuenta);
        delete listcuentas;
    }// end if
}// end codigo_textChanged

void filtrarextractosview::boton_buscacontrapartida() {
   listcuentasview1 *listcuentas = new listcuentasview1();
   listcuentas->modo=1;
   listcuentas->inicializa(conexionbase);
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



