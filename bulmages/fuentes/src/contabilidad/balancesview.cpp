/***************************************************************************
                          balancesview.cpp  -  description
                             -------------------
    begin                : sáb oct 18 2003
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

#include "balancesview.h"
#include "compbalanceview.h"
#include "balancesprintview.h"
#include <qtable.h>
#include <qfiledialog.h>
#include <qmessagebox.h>

#include "importbalance.h"
#include "empresa.h"


#define COL_CODIGO 0
#define COL_NOMBRE 1

balancesview::balancesview(empresa *emp, QWidget *parent, const char *name ) : balancesdlg(parent,name) {
   empresaactual = emp;
   conexionbase = empresaactual->bdempresa();
   numdigitos = empresaactual->numdigitosempresa();
   modo = 0;
   inicializatabla();
}// end balancesview

balancesview::~balancesview(){
}


void balancesview::inicializatabla()  {
  listado->setNumRows(0);
  listado->setNumCols(2);
  listado->horizontalHeader()->setLabel( COL_CODIGO, tr( "CODIGO" ) );
  listado->horizontalHeader()->setLabel( COL_NOMBRE, tr( "Nombre Balance" ) );
  listado->hideColumn(2);
  listado->hideColumn(0);
  listado->setColumnWidth(1,400);
  string query = "SELECT * FROM balance";
  conexionbase->begin();
  cursor2 *cursoraux1=conexionbase->cargacursor(query.c_str(),"elquery");
  conexionbase->commit();
  listado->setNumRows(cursoraux1->numregistros());
  int i=0;
   while (!cursoraux1->eof()) {
      listado->setText(i,COL_CODIGO, cursoraux1->valor("idbalance"));
      listado->setText(i,COL_NOMBRE, cursoraux1->valor("nombrebalance"));
      cursoraux1->siguienteregistro ();
      i++;
   }// end while
   delete cursoraux1;
   listado->setReadOnly(TRUE);
}// end inicializatabla


void balancesview::nuevo() {
   QString query;
   // Insertamos el balance en su sitio
   query.sprintf("INSERT INTO balance (nombrebalance) VALUES ('Nuevo Balance')");
   conexionbase->begin();
   conexionbase->ejecuta(query);
   conexionbase->commit();
   // Cogemos el identificador del balance insertado y abrimos la ventana de edición de balances.
   query.sprintf("SELECT max(idbalance) AS idbalance FROM balance");
   conexionbase->begin();
   cursor2 *cursoraux = conexionbase->cargacursor(query, "cursor1");
   conexionbase->commit();
   
   compbalanceview * nuevae = new compbalanceview(empresaactual,this,"compbalance");
   nuevae->inicializa1(cursoraux->valor("idbalance").ascii());
   nuevae->exec();
   delete nuevae;   
   delete cursoraux;
   // Cuando hemos terminado de trabajar con el nuevo balance repintamos para que aparezcan bien las cosas
   inicializatabla();
}// end nuevo


void balancesview::borrar() {
   // Borrar un balance es borrar tanto la entrada de balance como todas las lineas asociadas a éste.
   // Borramos primero todas las compbalance y luego la entrada en balance.
   QString idbalance = listado->text(listado->currentRow(), COL_CODIGO);
   QString query;
   conexionbase->begin();
   query.sprintf("DELETE FROM compmasap WHERE idmpatrimonial IN (SELECT idmpatrimonial FROM mpatrimonial WHERE idbalance = %s)",idbalance.ascii());
   conexionbase->ejecuta(query.ascii());
   query.sprintf("DELETE FROM mpatrimonial WHERE idbalance = %s",idbalance.ascii());
   conexionbase->ejecuta(query);   
   query.sprintf("DELETE FROM compbalance WHERE idbalance=%s", idbalance.ascii());
   conexionbase->ejecuta(query.ascii());
   query.sprintf("DELETE FROM balance WHERE idbalance=%s",idbalance.ascii());
   conexionbase->ejecuta(query);
   conexionbase->commit();
   inicializatabla();
}// end borrar


void balancesview::dbtabla(int row, int colummn, int button,const QPoint &mouse) {
  fprintf(stderr,"Se ha hecho doble click sobre la tabla\n");
  // Dependiendo del modo hacemos una cosa u otra
  if (modo == 0) {
     string idbalance = listado->text(row,COL_CODIGO).ascii();
     // Creamos el objeto mpatrimonialview, y lo lanzamos.
     compbalanceview *bal=new compbalanceview(empresaactual,this,0);
     bal->inicializa1(idbalance);
     bal->exec();
     delete bal;
     // Como existe la posibilidad de que hayan cambiado las cosas forzamos un repintado
     inicializatabla();
   } else {
      idbalance = listado->text(listado->currentRow(),COL_CODIGO).ascii();
      nombalance = listado->text(listado->currentRow(),COL_NOMBRE).ascii();
      close();
   }// end if
   
   // Para quitar el warning
   colummn=button=0;
   mouse.isNull();
}// end dbtabla

void balancesview::imprimir() {
   QString idbalance = listado->text(listado->currentRow(),COL_CODIGO);
   fprintf(stderr,"Balance print\n");
   balancesprintview *b = new balancesprintview(empresaactual,0,0);
   b->setidbalance(idbalance);
   b->exec();
   delete b;
}// end imprimir


void balancesview::boton_exportar() {
   fprintf(stderr,"Boton de Exportar\n");
   QString idbalance = listado->text(listado->currentRow(),COL_CODIGO);
   QString fn = QFileDialog::getSaveFileName(0, tr("Balances (*.xml)"), 0,tr("Guardar Balance"),tr("Elige el nombre de archivo"));
   if (!fn.isEmpty()) {
      FILE *mifile;
      mifile = fopen((char *) fn.ascii(),"wt");
      if (mifile != NULL) {
          QString SQlQuery;
          SQlQuery.sprintf("SELECT * FROM balance WHERE idbalance=%s", idbalance.ascii());
         conexionbase->begin();
         cursor2 *cursp = conexionbase->cargacursor(SQlQuery,"balance");
         conexionbase->commit();
         fprintf(mifile,"<?xml version=\"1.0\"?>\n");
         fprintf(mifile,"<DOCUMENT>\n");
         while (!cursp->eof()) {
            fprintf(mifile,"   <balance>\n");
            fprintf(mifile,"      <nombrebalance>%s</nombrebalance>\n", cursp->valor("nombrebalance").ascii());
 // ----------------------
            QString SQlQuery;
            SQlQuery.sprintf("SELECT * FROM mpatrimonial WHERE idbalance=%s", idbalance.ascii());
            conexionbase->begin();
            cursor2 *cursp5 = conexionbase->cargacursor(SQlQuery,"balance5");
            conexionbase->commit();
            while (!cursp5->eof()) {
               fprintf(mifile,"      <mpatrimonial>\n");
               fprintf(mifile,"         <idmasa>%s</idmasa>\n", XMLProtect(cursp5->valor("idmpatrimonial")).ascii());
//               fprintf(mifile,"         <idmpatrimonial>%s</idmpatrimonial>\n", cursp5->valor("idmpatrimonial").ascii());
               fprintf(mifile,"         <descmpatrimonial>%s</descmpatrimonial>\n", XMLProtect(cursp5->valor("descmpatrimonial")).ascii());
               fprintf(mifile,"         <orden>%s</orden>\n", XMLProtect(cursp5->valor("orden")).ascii());
               fprintf(mifile,"         <tabulacion>%s</tabulacion>\n", XMLProtect(cursp5->valor("tabulacion")).ascii());
               fprintf(mifile,"         <saldo>%s</saldo>\n", XMLProtect(cursp5->valor("saldo")).ascii());
               fprintf(mifile,"         <opdesc>%s</opdesc>\n", XMLProtect(cursp5->valor("opdesc")).ascii());
               QString SQlQuery1;
               SQlQuery1.sprintf("SELECT * FROM compmasap LEFT JOIN cuenta ON cuenta.idcuenta=compmasap.idcuenta WHERE masaperteneciente=%s", cursp5->valor("idmpatrimonial").ascii());
               conexionbase->begin();
               cursor2 *cursp6 = conexionbase->cargacursor(SQlQuery1,"compabalance6");
               conexionbase->commit();
               while (!cursp6->eof()) {
                  fprintf(mifile,"         <compmasap>\n");
                  fprintf(mifile,"            <masaperteneciente>%s</masaperteneciente>\n", XMLProtect(cursp6->valor("masaperteneciente")).ascii());
                  fprintf(mifile,"            <codigo>%s</codigo>\n", XMLProtect(cursp6->valor("codigo")).ascii());
                  fprintf(mifile,"            <idmpatrimonial>%s</idmpatrimonial>\n", XMLProtect(cursp6->valor("idmpatrimonial")).ascii());
                  fprintf(mifile,"            <saldo>%s</saldo>\n", XMLProtect(cursp6->valor("saldo")).ascii());
                  fprintf(mifile,"            <signo>%s</signo>\n", XMLProtect(cursp6->valor("signo")).ascii());
                  fprintf(mifile,"            <nombre>%s</nombre>\n", XMLProtect(cursp6->valor("nombre")).ascii());
                  fprintf(mifile,"         </compmasap>\n");
                  cursp6->siguienteregistro();
               }// end while
               delete cursp6;
               fprintf(mifile,"      </mpatrimonial>\n");            
               cursp5->siguienteregistro();
            }// end while
            delete cursp5;
 // --------------           
            QString SQlQuery1;
            SQlQuery1.sprintf("SELECT * FROM compbalance WHERE idbalance=%s", idbalance.ascii());
            conexionbase->begin();
            cursor2 *cursp1 = conexionbase->cargacursor(SQlQuery1,"compabalance");
            conexionbase->commit();
            while (!cursp1->eof()) {
               fprintf(mifile,"      <compbalance>\n");
               fprintf(mifile,"         <idmpatrimonial>%s</idmpatrimonial>\n", XMLProtect(cursp1->valor("idmpatrimonial")).ascii());
               fprintf(mifile,"         <concepto>%s</concepto>\n", XMLProtect(cursp1->valor("concepto")).ascii());
               fprintf(mifile,"         <orden>%s</orden>\n", XMLProtect(cursp1->valor("orden")).ascii());
               fprintf(mifile,"         <tabulacion>%s</tabulacion>\n", XMLProtect(cursp1->valor("tabulacion")).ascii());
               fprintf(mifile,"      </compbalance>\n");
               cursp1->siguienteregistro();
            }// end while
            delete cursp1;
            fprintf(mifile,"   </balance>\n");            
            cursp->siguienteregistro();
         }// end while
         delete cursp;
         fprintf(mifile,"</DOCUMENT>\n");
         fclose(mifile);
      }// end if
  }// end if   
  QMessageBox::warning( this,"BulmaGés", "Se ha exportado el Balance.", "OK",  "No OK", 0, 0, 1 );  
}// end boton_exportar

void balancesview::boton_importar() {
   fprintf(stderr,"Boton de Importar\n");
   QString fn = QFileDialog::getOpenFileName(0, tr("Asientos Inteligentes (*.xml)"), 0,tr("Cargar Asientos Inteligentes"),tr("Elige el nombre de archivo"));
   if (!fn.isEmpty()) {      
      // Hacemos el parsing del XML
      QFile xmlFile( fn);                 // Declaramos el ficheros
      QXmlInputSource source( &xmlFile ); // Declaramos el inputsource, con el fichero como parámetro
      QXmlSimpleReader reader;            // Declaramos el lector
      
      importbalance * handler = new importbalance( empresaactual );
      reader.setContentHandler( handler );
      reader.parse( source );   
      handler->cambiapaso();
      source.reset();
      reader.parse( source );
   }// end if   
   inicializatabla();   
}// end boton_importar

