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

   m_colNomCoste = m_listCostes->addColumn("nom_coste",-1);
   m_colDescCoste = m_listCostes->addColumn("desc_coste",-1);
   m_colStatusCoste = m_listCostes->addColumn("Status",-1);
   m_colIdCoste = m_listCostes->addColumn("idc_coste",0);

   selccostes=new selectccosteview(empresaactual,0,0);   
   
   // Hacemos la carga de los centros de coste. Rellenamos el combobox
   cargacostes();
 
   fprintf(stderr,"Fin del constructor de fitrarextractosview\n");

}// end filtrarextractosview


filtrarextractosview::~filtrarextractosview(){
}

void filtrarextractosview::boton_ccostes() {
   fprintf(stderr,"Boton ccostes\n");
   selccostes->exec();
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
   
// Rellenamnos la listbox que va a sustituir al combobox correspondiente.
// Para que en los listados puedan salir más cosas de las que se dicen.   
   fprintf(stderr,"Ahora nos toca rellenar las listas.\n");
   
    QListViewItem * it;
    QListViewItem *Lista[10000];
    int padre;
    int idc_coste=0;
    cursor2 *cursoraux1, *cursoraux2;

    // Cogemos los centros de coste principales y los ponemos donde toca.
    m_listCostes->clear();
    conexionbase->begin();
    cursoraux1 = conexionbase->cargacursor("SELECT * FROM c_coste WHERE padre ISNULL ORDER BY idc_coste","centroscoste");
    conexionbase->commit();
    while (!cursoraux1->eof()) {
        padre = atoi( cursoraux1->valor("padre").ascii());
        idc_coste = atoi( cursoraux1->valor("idc_coste").ascii());
        it =new QListViewItem(m_listCostes);
        Lista[idc_coste]=it;
        it->setText(m_colIdCoste, cursoraux1->valor("idc_coste"));
        it->setText(m_colDescCoste,cursoraux1->valor("descripcion"));
        it->setText(m_colNomCoste, cursoraux1->valor("nombre"));
        it->setOpen(true);
        cursoraux1->siguienteregistro ();
    }// end while
    delete cursoraux1;

    // Una vez que hemos puesto los centros de coste padre, todo lo demás es una
    // Tarea de ir colocando centros de coste a sus respectivos padres. El algoritmo
    // deja de ser recursivo y pasa a ser lineal.
    conexionbase->begin();
    cursoraux2= conexionbase->cargacursor("SELECT * FROM c_coste WHERE padre IS NOT NULL ORDER BY idc_coste","mascostes");
    conexionbase->commit();
    while (!cursoraux2->eof()) {
        padre = atoi(cursoraux2->valor("padre").ascii());
        idc_coste = atoi(cursoraux2->valor("idc_coste").ascii());
        fprintf(stderr,"Cuentas de subnivel:%d",padre);
            it = new QListViewItem(Lista[padre]);
            Lista[idc_coste]=it;
            it->setText(m_colIdCoste,cursoraux2->valor("idc_coste"));
            it->setText(m_colDescCoste,cursoraux1->valor("descripcion"));
            it->setText(m_colNomCoste,cursoraux1->valor("nombre"));
            it->setOpen(true);
        cursoraux2->siguienteregistro();
    }// end while
    delete cursoraux2;
   
   
   
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



