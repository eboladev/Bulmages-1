/***************************************************************************
                          resmensualview.cpp  -  description
                             -------------------
    begin                : dom jul 20 2003
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

#include "resmensualview.h"
#include "estadisticas/tolinechart.h"
#include "estadisticas/topiechart.h"
#include "qdatetime.h"
#include "qstring.h"
#include "qlineedit.h"
#include "qobject.h"
#include "listcuentasview1.h"
#include "mpatrimonialesview.h"


resmensualview::resmensualview(QWidget *parent, const char *name ) : resmensualdlg(parent,name) {

/*
std::list<double> milista;
milista.push_back(45.0);
milista.push_back(60.3);
milista.push_back(80.5);
std::list<double> milista1;
milista1.push_back(85);
milista1.push_back(68);
milista1.push_back(50);
std::list<double> milista2;
milista2.push_back(55);
milista2.push_back(18);
milista2.push_back(60);
line->setMinValue(1);
line->setMaxValue(100);
line->setTitle("hola");
line->setSamples(3);
line->addValues(milista1,"hola");
line->addValues(milista,"adios");
line->addValues(milista2,"fin");

line1->setMinValue(1);
line1->setMaxValue(100);
line1->setTitle("hola");
line1->setSamples(3);
line1->addValues(milista1,"hola");
line1->addValues(milista,"adios");
line1->addValues(milista2,"fin");
//line->show();
*/

// Inicializamos los valores de las masas patrimoniales.
idmpatrimonial1 = "";
idmpatrimonial2 = "";
idmpatrimonial3 = "";

}
resmensualview::~resmensualview(){
}

int resmensualview::inicializa(postgresiface2 *conexion) {
    conexionbase = conexion;
//    presentar();
//    pie->show();
    return(0);
}// end inicializa

void resmensualview::accept() {
   presentar();
   presentarpie();
}// end accept

void resmensualview::presentar() {
   std::list<double> milistad[12];
//   std::list<double> milistah[12];
   std::list<double> milistas[12];
   char *mes[]={"ene","feb","mar","abr","may","jun","jul","ago","sep","oct","nov","dec"};
   

   QString codigo[3];
   codigo[0]=cod1->text();
   codigo[1]=cod2->text();
   codigo[2]=cod3->text();
   QString codigomasa[3];
   codigomasa[0]= idmpatrimonial1.c_str();
   codigomasa[1]= idmpatrimonial2.c_str();
   codigomasa[2]= idmpatrimonial3.c_str();
   
   string ano = "2003";

   for (int j=0; j<3; j ++) {
      QDate fecha(2003,1,1);
      QDate fecha1(2003,1,31);
      float saldoant=0;
      float movant=0;
      for(int i=0; i<12;i++) {
         // Buscamos los datos de enero.
         QString finicial = fecha.toString("dd/MM/yyyy");
         QString ffinal = fecha1.toString("dd/MM/yyyy");
         QString query;
         string cod = codigo[j].latin1();
         string cod1 = codigomasa[j].latin1();
         if (cod != "") {
            query.sprintf( "SELECT debetotal(id_cuenta('%s'),'%s','%s') as tdebe, habertotal(id_cuenta('%s'),'%s','%s') as thaber",codigo[j].latin1(),(char *)finicial.latin1(),(char *)ffinal.latin1(), codigo[j].latin1(),(char *)finicial.latin1(),(char *)ffinal.latin1());
//         } else if (cod1 != "" ) {
         } else {
            query.sprintf( "SELECT debempatrimonial(%s,'%s','%s') as tdebe, habermpatrimonial(%s,'%s','%s') as thaber",codigomasa[j].latin1(),(char *)finicial.latin1(),(char *)ffinal.latin1(), codigomasa[j].latin1(),(char *)finicial.latin1(),(char *)ffinal.latin1());
         }// end if
         fprintf(stderr,"%s\n",query.latin1());
         conexionbase->begin();
         cursor2 *curs = conexionbase->cargacursor(query,"midursor");
         conexionbase->commit();
         if (!curs->eof()) {
            milistad[i].push_back(movant + atof(curs->valor("tdebe").latin1()) +atof(curs->valor("thaber").latin1()));
            milistas[i].push_back(saldoant + atof(curs->valor("tdebe").latin1()) - atof(curs->valor("thaber").latin1()));
            saldoant = saldoant + atof(curs->valor("tdebe").latin1()) - atof(curs->valor("thaber").latin1());
            movant = movant +  atof(curs->valor("tdebe").latin1()) + atof(curs->valor("thaber").latin1());
            fprintf(stderr,"metido en el gráfico%s\n", curs->valor("tdebe").latin1());
         } else {
            milistad[i].push_back(movant);
//            milistad[i].push_back(0);
            milistas[i].push_back(saldoant);
         }// end if
         delete curs;
         fecha = fecha.addMonths(1);
         fecha1 = fecha1.addMonths(1);
      }// end for
   }// end for
   line->clear();   
   line->showGrid(12);
   line->setTitle(tr("Progresion de saldos"));
   line->setSamples(12);
   line1->clear();
   line1->showGrid(12);
   line1->setTitle(tr("Movimientos menusales"));
   line1->setSamples(12);
   for (int i=0; i<12;i++) {
      line->addValues(milistas[i],mes[i]);
      line1->addValues(milistad[i],mes[i]);
   }// end for
}// end presentar

void resmensualview::buscacodigocta() {
   listcuentasview1 *listcuentas = new listcuentasview1();
   listcuentas->modo=1;
   listcuentas->inicializa(conexionbase);
   listcuentas->exec();
   cod1->setText(listcuentas->codcuenta);
   delete listcuentas;   
}// end buscacodigocta


void resmensualview::buscacodigocta1() {
   listcuentasview1 *listcuentas = new listcuentasview1();
   listcuentas->modo=1;
   listcuentas->inicializa(conexionbase);
   listcuentas->exec();
   cod2->setText(listcuentas->codcuenta);
   delete listcuentas;}// end buscacodigocta

void resmensualview::buscacodigocta2() {
   listcuentasview1 *listcuentas = new listcuentasview1();
   listcuentas->modo=1;
   listcuentas->inicializa(conexionbase);
   listcuentas->exec();
   cod3->setText(listcuentas->codcuenta);
   delete listcuentas;
}// end buscacodigocta

void resmensualview::buscampatrimonial1() {
   mpatrimonialesview * nuevae = new mpatrimonialesview(0,"mpatrimoniales",true);
   nuevae->inicializa(conexionbase);
   nuevae->setmodoselector();
   nuevae->exec();
   mpatrimonial1->setText((char *) (nuevae->getnommasa()).c_str());
   idmpatrimonial1 = nuevae->getidmasa();
   delete nuevae;
   
}// end buscampatrimonial1

void resmensualview::buscampatrimonial2() {
   mpatrimonialesview * nuevae = new mpatrimonialesview(0,"mpatrimoniales",true);
   nuevae->inicializa(conexionbase);
   nuevae->setmodoselector();
   nuevae->exec();
   mpatrimonial2->setText((char *) (nuevae->getnommasa()).c_str());
   idmpatrimonial2 = nuevae->getidmasa();
   delete nuevae;

}// end buscampatrimonial1

void resmensualview::buscampatrimonial3() {
   mpatrimonialesview * nuevae = new mpatrimonialesview(0,"mpatrimoniales",true);
   nuevae->inicializa(conexionbase);
   nuevae->setmodoselector();
   nuevae->exec();
   mpatrimonial3->setText((char *) (nuevae->getnommasa()).c_str());
   idmpatrimonial3 = nuevae->getidmasa();
   delete nuevae;

}// end buscampatrimonial1

void resmensualview::presentarpie() {
      char query[2900];
      cursor2 *cursorapt;

      QString finicial = "01/01/2003";
      QString ffinal = "31/12/2003";
      QString cinicial = "10";
      QString cfinal = "99";
      QString codigo[3];
      codigo[0]=cod1->text();
      codigo[1]=cod2->text();
      codigo[2]=cod3->text();

      toPieChart *pies[3];
      pies[0] = pie;
      pies[1] = pie1;
      pies[2] = pie2;


      for (int i=0; i<3; i++) {      
         conexionbase->begin();
   		sprintf(query,"SELECT sum(debe) as tdebe, sum(haber) as thaber, contrapartida FROM apunte WHERE idcuenta=id_cuenta('%s') GROUP BY contrapartida", codigo[i].latin1());
   		fprintf(stderr,"%s\n",query);
   		cursorapt = conexionbase->cargacursor(query,"mycursor");
         conexionbase->commit();
         // Calculamos cuantos registros van a crearse y dimensionamos la tabla.
         std::list<double> valores;
         std::list<QString> labels;

         while (!cursorapt->eof()) {
            QString nomcuenta;
            // Acumulamos los totales para al final poder escribirlos
            sprintf(query,"SELECT * FROM cuenta WHERE idcuenta = %s",cursorapt->valor("contrapartida").latin1());
            conexionbase->begin();
            cursor2 *micurs= conexionbase->cargacursor(query,"mioldcursor");
            if (!micurs->eof()) {
               nomcuenta = micurs->valor("codigo") +" "+ micurs->valor("descripcion");
            }// end if
            delete micurs;

            float valor =  atof(cursorapt->valor("tdebe").latin1()) + atof(cursorapt->valor("thaber").latin1());
            if (valor > 0) {
   //                  pie->addValue(valor,nomcuenta.substr(0,25).c_str());
                     valores.push_back(valor);
                     labels.push_back(nomcuenta.mid(0,25).latin1());
            } else {
   //                  pie->addValue(-valor,nomcuenta.substr(0,25).c_str());
                     valores.push_back(-valor);
                     labels.push_back(nomcuenta.mid(0,25).latin1());
            }// end if

            // Calculamos la siguiente cuenta registro y finalizamos el bucle
            cursorapt->siguienteregistro();
         }// end while

         // Vaciamos el cursor de la base de datos.
         delete cursorapt;
         pies[i]->setValues(valores,labels);
      }// end for      
}// end presentarpie


