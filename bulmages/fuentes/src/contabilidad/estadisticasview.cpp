/***************************************************************************
                          estadisticasview.cpp  -  description
                             -------------------
    begin                : mar jul 15 2003
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

#include "estadisticasview.h"


int estadisticasview::inicializa(postgresiface2 *conexion) {
    conexionbase = conexion;
    presentar();
//    pie->show();
    return(0);
}// end inicializa

estadisticasview::estadisticasview(QWidget *parent, const char *name ) : estadisticasdlg(parent,name) {
/*
pie = new toPieChart(this,0);
pie->resize(550,400);
//pie->show();
*/

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

bar = new toBarChart(this,0);
bar->move(300,20);
bar->resize(250,250);
bar->setMinValue(1);
bar->setMaxValue(100);
bar->setTitle("hola como mola");
bar->setSamples(3);
bar->addValues(milista1,"adios");
bar->addValues(milista,"hola");
bar->addValues(milista2,"fin");
//bar->show();

line = new toLineChart(this,0);
line->move(10,200);
line->resize(250,250);
line->setMinValue(1);
line->setMaxValue(100);
line->setTitle("hola");
line->setSamples(3);
line->addValues(milista1,"hola");
line->addValues(milista,"adios");
line->addValues(milista2,"fin");
//line->show();

*/
}


estadisticasview::~estadisticasview(){
//   delete pie;
//   delete bar;
}


void estadisticasview::presentar() {
      int j,num1;
      QString query;
      cursor2 *cursorapt;
/*      QString finicial = fechainicial1->text();
      QString ffinal = fechafinal1->text();
      QString cinicial = codigoinicial->text();
      QString cfinal = codigofinal->text();
*/
      QString finicial = "01/01/2003";
      QString ffinal = "31/12/2003";
      QString cinicial = "10";
      QString cfinal = "99";

      // Hacemos la consulta de los apuntes a listar en la base de datos.
 //     int idc_coste = ccostes[combocoste->currentItem()];

      // La consulta es compleja, requiere la creación de una tabla temporal y de cierta mandanga por lo que puede
		// Causar problemas con el motor de base de datos.
		fprintf(stderr,"BALANCE: Empezamos a hacer la presentacion\n");
      conexionbase->begin();
		query.sprintf("CREATE TEMPORARY TABLE balance AS SELECT cuenta.idcuenta, codigo, nivel(codigo) AS nivel, cuenta.descripcion, padre, tipocuenta ,debe, haber, tdebe, thaber,(tdebe-thaber) AS tsaldo, (debe-haber) AS saldo, adebe, ahaber, (adebe-ahaber) AS asaldo FROM cuenta LEFT JOIN (SELECT idcuenta, sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte WHERE fecha >= '%s' AND fecha<= '%s' GROUP BY idcuenta) AS t1 ON t1.idcuenta = cuenta.idcuenta LEFT JOIN (SELECT idcuenta, sum(debe) AS adebe, sum(haber) AS ahaber FROM apunte WHERE fecha < '%s' GROUP BY idcuenta) AS t2 ON t2.idcuenta = cuenta.idcuenta", finicial.latin1(), ffinal.latin1(), finicial.latin1() );
      conexionbase->ejecuta(query);
      query.sprintf("UPDATE BALANCE SET padre=0 WHERE padre ISNULL");
      conexionbase->ejecuta(query);
      query.sprintf("DELETE FROM balance WHERE debe=0 AND haber =0");
      conexionbase->ejecuta(query);

      conexionbase->commit();
      conexionbase->begin();
      // Para evitar problemas con los nulls hacemos algunos updates
      query.sprintf("UPDATE BALANCE SET tsaldo=0 WHERE tsaldo ISNULL");
      conexionbase->ejecuta(query);
      query.sprintf("UPDATE BALANCE SET tdebe=0 WHERE tdebe ISNULL");
      conexionbase->ejecuta(query);
      query.sprintf("UPDATE BALANCE SET thaber=0 WHERE thaber ISNULL");
      conexionbase->ejecuta(query);
      query.sprintf("UPDATE BALANCE SET asaldo=0 WHERE asaldo ISNULL");
      conexionbase->ejecuta(query);

      conexionbase->commit();
      conexionbase->begin();

		query.sprintf("SELECT idcuenta FROM balance ORDER BY padre DESC");
		cursorapt = conexionbase->cargacursor(query,"Balance1view");
      conexionbase->commit();
      conexionbase->begin();

      while (!cursorapt->eof())  {
         query.sprintf("SELECT * FROM balance WHERE idcuenta=%s",cursorapt->valor("idcuenta").latin1());
         cursor2 *mycur = conexionbase->cargacursor(query,"cursorrefresco");
			conexionbase->commit();
	      conexionbase->begin();

         query.sprintf("UPDATE balance SET tsaldo = tsaldo + (%2.2f), tdebe = tdebe + (%2.2f), thaber = thaber +(%2.2f), asaldo= asaldo+(%2.2f) WHERE idcuenta = %d",atof(mycur->valor("tsaldo").latin1()), atof(mycur->valor("tdebe").latin1()), atof(mycur->valor("thaber").latin1()),atof(mycur->valor("asaldo").latin1()),  atoi(mycur->valor("padre").latin1()));
//			fprintf(stderr,"%s para el código\n",query, cursorapt->valor("codigo").c_str());
			conexionbase->ejecuta(query);
         delete mycur;
			cursorapt->siguienteregistro();
		}// end while
		delete cursorapt;
      
      conexionbase->commit();
      conexionbase->begin();

      // Borramos todo lo que no es de este nivel
      query.sprintf("DELETE FROM balance where nivel(codigo)>%s","2");
      conexionbase->ejecuta(query);

      //Borramos todo lo que tiene un hijo en el balance
      query.sprintf("DELETE FROM balance WHERE idcuenta IN (SELECT padre FROM balance)");
      conexionbase->ejecuta(query);

      conexionbase->commit();
      conexionbase->begin();

		query.sprintf("SELECT * FROM balance WHERE debe <> 0  OR haber <> 0 ORDER BY codigo");
		cursorapt = conexionbase->cargacursor(query,"mycursor");
		query.sprintf("DROP TABLE balance");
		conexionbase->ejecuta(query);
      conexionbase->commit();


      // Calculamos cuantos registros van a crearse y dimensionamos la tabla.
      num1 = cursorapt->numregistros();
      j=0;
      while (!cursorapt->eof()) {
         // Acumulamos los totales para al final poder escribirlos

         float valor =  atof(cursorapt->valor("tsaldo").latin1());
         if (valor > 0)
                  pie->addValue(valor,cursorapt->valor("descripcion").mid(0,25).latin1());
         else
                  pie->addValue(-valor,cursorapt->valor("descripcion").mid(0,25).latin1());
         // end if

         // Calculamos la siguiente cuenta registro y finalizamos el bucle
         cursorapt->siguienteregistro();
         j++;
      }// end while

      // Vaciamos el cursor de la base de datos.
      delete cursorapt;

}// end presentar

