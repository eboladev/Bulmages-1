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
// Si se va a compilar con el modulo de estadisticas se ponen las estadisticas
#ifdef ESTADISTICAS
#include "estadisticas/tolinechart.h"
#include "estadisticas/topiechart.h"
#endif

#ifdef GDCHART
#include "gdcchart/gdc.h"
#include "gdcchart/gdchart.h"
#endif

#include "qdatetime.h"
#include "qstring.h"
#include "qlineedit.h"
#include "qobject.h"
#include "listcuentasview1.h"
#include "mpatrimonialesview.h"


resmensualview::resmensualview(QWidget *parent, const char *name ) : resmensualdlg(parent,name) {

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
         string cod = codigo[j].ascii();
         string cod1 = codigomasa[j].ascii();
         if (cod != "") {
            query.sprintf( "SELECT debetotal(id_cuenta('%s'),'%s','%s') as tdebe, habertotal(id_cuenta('%s'),'%s','%s') as thaber",codigo[j].ascii(),(char *)finicial.ascii(),(char *)ffinal.ascii(), codigo[j].ascii(),(char *)finicial.ascii(),(char *)ffinal.ascii());
//         } else if (cod1 != "" ) {
         } else {
            query.sprintf( "SELECT debempatrimonial(%s,'%s','%s') as tdebe, habermpatrimonial(%s,'%s','%s') as thaber",codigomasa[j].ascii(),(char *)finicial.ascii(),(char *)ffinal.ascii(), codigomasa[j].ascii(),(char *)finicial.ascii(),(char *)ffinal.ascii());
         }// end if
         fprintf(stderr,"%s\n",query.ascii());
         conexionbase->begin();
         cursor2 *curs = conexionbase->cargacursor(query,"midursor");
         conexionbase->commit();
         if (!curs->eof()) {
            milistad[i].push_back(movant + atof(curs->valor("tdebe").ascii()) +atof(curs->valor("thaber").ascii()));
            milistas[i].push_back(saldoant + atof(curs->valor("tdebe").ascii()) - atof(curs->valor("thaber").ascii()));
            saldoant = saldoant + atof(curs->valor("tdebe").ascii()) - atof(curs->valor("thaber").ascii());
            movant = movant +  atof(curs->valor("tdebe").ascii()) + atof(curs->valor("thaber").ascii());
            fprintf(stderr,"metido en el gráfico%s\n", curs->valor("tdebe").ascii());
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
#ifdef ESTADISTICAS
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
#endif
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
   		sprintf(query,"SELECT sum(debe) as tdebe, sum(haber) as thaber, contrapartida FROM apunte WHERE apunte.idcuenta=id_cuenta('%s') GROUP BY contrapartida", codigo[i].ascii());
   		cursorapt = conexionbase->cargacursor(query,"mycursor");
         conexionbase->commit();
         // Calculamos cuantos registros van a crearse y dimensionamos la tabla.
         std::list<double> valores;
         std::list<QString> labels;

         while (!cursorapt->eof()) {
            QString nomcuenta;
            // Acumulamos los totales para al final poder escribirlos
            sprintf(query,"SELECT * FROM cuenta WHERE idcuenta = %s",cursorapt->valor("contrapartida").ascii());
            conexionbase->begin();
            cursor2 *micurs= conexionbase->cargacursor(query,"mioldcursor");
            if (!micurs->eof()) {
               nomcuenta = micurs->valor("codigo") +" "+ micurs->valor("descripcion");
            }// end if
            delete micurs;

            float valor =  atof(cursorapt->valor("tdebe").ascii()) + atof(cursorapt->valor("thaber").ascii());
            if (valor > 0) {
   //                  pie->addValue(valor,nomcuenta.substr(0,25).c_str());
                     valores.push_back(valor);
                     labels.push_back(nomcuenta.mid(0,25).ascii());
            } else {
   //                  pie->addValue(-valor,nomcuenta.substr(0,25).c_str());
                     valores.push_back(-valor);
                     labels.push_back(nomcuenta.mid(0,25).ascii());
            }// end if

            // Calculamos la siguiente cuenta registro y finalizamos el bucle
            cursorapt->siguienteregistro();
         }// end while

         // Vaciamos el cursor de la base de datos.
         delete cursorapt;
#ifdef ESTADISTICAS         
         pies[i]->setValues(valores,labels);
#endif

      }// end for    
      
#ifdef GDCHART
generargrafico();
#endif
        
}// end presentarpie

void resmensualview::generargrafico() {
#ifdef GDCHART

	/* set some sample data points */
	float	h[12]  = {	17.8,  17.1,  17.3,  0,  17.2,  17.1,
						17.3,  17.3,  17.3,  17.1,         17.5,  17.4 };

	float	c[12]  =  { 17.0,  16.8,  16.9,  0,  16.9,  16.8,
					    17.2,  16.8,  17.0,  16.9,         16.4,  16.1 };

	float	l[12]  = {  16.8,  16.8,  16.7,  0,  16.5,  16.0,
						16.1,  16.8,  16.5,  16.9,         16.2,  16.0 };

	float	v[12]  = {  150.0, 100.0, 340.0,  0, 999.0, 390.0,
						420.0, 150.0, 100.0,  340.0,       1590.0, 700.0 };
	char	*t[12] = {	"May", "Jun", "Jul",  "Aug",       "Sep",  "Oct",
						"Nov", "Dec", "Jan", "Feb",        "Mar",  "Apr" };

//	/* set color RGB as ulong array */
//	unsigned long	setcolor[3]    = { 0xC0C0FF, 0xFF4040, 0xFFFFFF };

	GDC_ANNOTATION_T	anno;

	/* need an open FILE pointer  - can be stdout */
	FILE				*outgif1 = fopen( "/tmp/g2.gif", "wb" );	/* rem: test open() fail */


	anno.color = 0x00FF00;
	strncpy( anno.note, "Did Not\nTrade", MAX_NOTE_LEN );	/* don't exceed MAX_NOTE_LEN */
	anno.point = 3;											/* first is 0 */
	GDC_annotation_font = GDC_TINY;
	GDC_annotation = &anno;									/* set annote option */

	GDC_HLC_style = (GDC_HLC_STYLE_T) (GDC_HLC_I_CAP | GDC_HLC_CLOSE_CONNECTED);
	GDC_HLC_cap_width = 45;

	GDC_bar_width     = 75;									/* % */

//	GDC_BGImage = "W.gif";

	GDC_title = "Widget Corp.";
	GDC_ytitle = "Price ($)";
	GDC_ytitle2 = "Volume (K)";
	GDC_ytitle_size = GDC_SMALL;
	GDC_VolColor = 0x4040FFL;								/* aka combo */
	GDC_3d_depth  = 4.0;									/* % entire gif */

//	GDC_SetColor  = setcolor;								/* see README */
	GDC_PlotColor = 0xFFFFFF;
	GDC_grid = FALSE;

//	GDC_xtitle="fy.1998";

//	fprintf( stdout, "Content-Type: image/gif\n\n" );		/* rem: for web use */
															/* finally: make the call */
	out_graph( 200, 175,									// overall width, height
			   outgif1,										// open FILE pointer
			   GDC_COMBO_HLC_AREA,							// chart type
			   12,											// number of points
			   t,											// X axis label array
			   1,											// number of sets (see README)
			   h,											// set 1 (high)
			   l,											// low
			   c,											// close
			   v );											// combo/volume

	fclose( outgif1 );

#endif
}// end generargrafico
