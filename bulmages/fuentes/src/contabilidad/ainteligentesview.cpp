/***************************************************************************
                          ainteligentesview.cpp  -  description
                             -------------------
    begin                : Thu Feb 6 2003
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

#include "ainteligentesview.h"
#include <listcuentasview1.h>
#include <qfiledialog.h>
#include <qlineedit.h>
#include <qtable.h>
#include <qtextedit.h>
#include <qcombobox.h>
#include <qtabwidget.h>

#include "importainteligente.h"
#include "empresa.h"


// Estas son las columnas que van con la talba de apuntes.
#define COL_IDBINTELIGENTE 0
#define COL_IDAINTELIGENTE 1
#define COL_IDDIARIO 2
#define COL_FECHA 3
#define COL_CONCEPTOCONTABLE 4
#define COL_CODCUENTA 5
#define COL_DESCRIPCION 6
#define COL_DEBE 7
#define COL_HABER 8
#define COL_CONTRAPARTIDA 9
#define COL_COMENTARIO 10
#define COL_CANAL 11
#define COL_NOMCANAL 12
#define COL_MARCACONCILIACION 13
#define COL_IDC_COSTE 14
#define COL_NOMCOSTE 15

// Estas son las columnas que van con la tabla del registro de iva.
#define COL_IDIVAINTELIGENTE      0
#define COL_IDAINTELIGENTEIVA     1
#define COL_IDBINTELIGENTEIVA     2
#define COL_CONTRAPARTIDAIVA      3
#define COL_BASEIMPIVA            4
#define COL_IVAIVA                5
#define COL_FACTURAIVA            6
#define COL_IDBORRADORIVA         7
#define COL_INCREGISTROIVA        8
#define COL_REGULARIZACIONIVA     9
#define COL_PLAN349IVA           10 
#define COL_NUMORDENIVA          11
#define COL_CIFIVA               12




ainteligentesview::ainteligentesview(empresa *emp, QWidget *parent, const char *name, bool modal ) : ainteligentesdlg(parent,name, modal) {
   empresaactual = emp;
   conexionbase = empresaactual->bdempresa();
   numdigitos = empresaactual->numdigitosempresa();
   oldrow=-2;
   oldcol=-2;

   tapunts->setNumCols(16);
   tapunts->setNumRows(100);

   tapunts->horizontalHeader()->setLabel( COL_IDBINTELIGENTE, tr( "IDBINTELIGENTE" ) );
   tapunts->horizontalHeader()->setLabel( COL_IDAINTELIGENTE, tr( "IDAINTELIGENTE" ) );
   tapunts->horizontalHeader()->setLabel( COL_IDDIARIO, tr( "IDDIARIO" ) );
   tapunts->horizontalHeader()->setLabel( COL_FECHA, tr( "FECHA" ) );
   tapunts->horizontalHeader()->setLabel( COL_CONCEPTOCONTABLE, tr( "CONCEPTOCONTABLE" ) );
   tapunts->horizontalHeader()->setLabel( COL_CODCUENTA, tr( "CODCUENTA" ) );
   tapunts->horizontalHeader()->setLabel( COL_DESCRIPCION, tr( "DESCRIPCION" ) );
   tapunts->horizontalHeader()->setLabel( COL_DEBE, tr( "DEBE" ) );
   tapunts->horizontalHeader()->setLabel( COL_HABER, tr( "HABER" ) );
   tapunts->horizontalHeader()->setLabel( COL_CONTRAPARTIDA, tr( "CONTRAPARTIDA" ) );
   tapunts->horizontalHeader()->setLabel( COL_COMENTARIO, tr( "COMENTARIO" ) );
   tapunts->horizontalHeader()->setLabel( COL_CANAL, tr( "CANAL" ) );
   tapunts->horizontalHeader()->setLabel( COL_NOMCANAL, tr( "CANAL" ) );
   tapunts->horizontalHeader()->setLabel( COL_MARCACONCILIACION, tr( "MARCACONCILIACION" ) );
   tapunts->horizontalHeader()->setLabel( COL_IDC_COSTE, tr( "IDC_COSTE" ) );
   tapunts->horizontalHeader()->setLabel( COL_NOMCOSTE, tr( "CENTRO COSTE" ) );
   tapunts->setColumnWidth(COL_IDBINTELIGENTE,10);
   tapunts->setColumnWidth(COL_IDAINTELIGENTE,10);
   tapunts->setColumnWidth(COL_IDDIARIO,10);
   tapunts->setColumnWidth(COL_FECHA,75);
   tapunts->setColumnWidth(COL_CONCEPTOCONTABLE,175);
   tapunts->setColumnWidth(COL_CODCUENTA,75);
   tapunts->setColumnWidth(COL_DESCRIPCION,75);
   tapunts->setColumnWidth(COL_DEBE,75);
   tapunts->setColumnWidth(COL_HABER,75);
   tapunts->setColumnWidth(COL_CONTRAPARTIDA,75);
   tapunts->setColumnWidth(COL_COMENTARIO,75);
   tapunts->setColumnWidth(COL_CANAL,75);
   tapunts->setColumnWidth(COL_MARCACONCILIACION,75);
   tapunts->setColumnWidth(COL_IDC_COSTE,75);
   tapunts->hideColumn(COL_IDBINTELIGENTE);
   tapunts->hideColumn(COL_IDAINTELIGENTE);
   tapunts->hideColumn(COL_MARCACONCILIACION);
   tapunts->hideColumn(COL_IDDIARIO);
   tapunts->hideColumn(COL_DESCRIPCION);
   tapunts->hideColumn(COL_COMENTARIO);
   tapunts->hideColumn(COL_CONTRAPARTIDA);
   tapunts->hideColumn(COL_CANAL);
   tapunts->hideColumn(COL_IDC_COSTE);



   tiva->setNumCols(13);
   tiva->setNumRows(100);
   tiva->horizontalHeader()->setLabel( COL_IDIVAINTELIGENTE, tr( "COL_IDIVAINTELIGENTE" ) );
   tiva->horizontalHeader()->setLabel( COL_IDAINTELIGENTEIVA, tr( "COL_IDAINTELIGENTEIVA" ) );
   tiva->horizontalHeader()->setLabel( COL_IDBINTELIGENTEIVA, tr( "COL_IDBINTELIGENTEIVA" ) );
   tiva->horizontalHeader()->setLabel( COL_CONTRAPARTIDAIVA, tr( "CONTRAPARTIDA" ) );
   tiva->horizontalHeader()->setLabel( COL_BASEIMPIVA, tr( "BASE IMP." ) );
   tiva->horizontalHeader()->setLabel( COL_IVAIVA, tr( "IVA" ) );
   tiva->horizontalHeader()->setLabel( COL_FACTURAIVA, tr( "NUM. FACTURA" ) );
   tiva->horizontalHeader()->setLabel( COL_IDBORRADORIVA, tr( "Col. Borrador" ) );
   tiva->horizontalHeader()->setLabel( COL_INCREGISTROIVA, tr( "Registro" ) );
   tiva->horizontalHeader()->setLabel( COL_REGULARIZACIONIVA, tr( "Regularizacion" ) );
   tiva->horizontalHeader()->setLabel( COL_PLAN349IVA, tr( "Plan 349" ) );
   tiva->horizontalHeader()->setLabel( COL_NUMORDENIVA, tr( "Num. Orden" ) );
   tiva->horizontalHeader()->setLabel( COL_CIFIVA, tr( "CIF Empresa" ) );
   tiva->hideColumn(COL_IDIVAINTELIGENTE);
   tiva->hideColumn(COL_IDAINTELIGENTEIVA);
   tiva->hideColumn(COL_IDBINTELIGENTE);
   
   // El cursor que recorre los asientos inteligentes debe iniciarse a NULL
   cainteligentes = NULL;
   
   // Hacemos la inicialización inicial.
   oldrow=-1;
   oldcol=-1;
   cargacombo();
   // Llamamos a boton_fin para que la pantalla aparezca con un asiento inteligente inicial.
   boton_fin();
   
}// end ainteligentesview

void ainteligentesview::cargacombo() {
   m_ainteligente->clear();
   if (cainteligentes != NULL) {
      delete cainteligentes;
   }// end if
   // Vamos a cargar el comboBox para que la cosa pinte mejor.
   QString SQLQuery = "SELECT * FROM ainteligente";
   conexionbase->begin();
   cainteligentes = conexionbase->cargacursor(SQLQuery,"uncursormas");
   conexionbase->commit();
   while (!cainteligentes->eof()) {
      m_ainteligente->insertItem(cainteligentes->valor("idainteligente")+cainteligentes->valor("descripcion"));
      cainteligentes->siguienteregistro();
   }// end while
}// end if


void ainteligentesview::comboActivated(const QString &) {
   QString idasiento = cainteligentes->valor("idainteligente",m_ainteligente->currentItem());
   idasientointeligente=idasiento.toInt();
   repinta();
}// end comboActivated

void ainteligentesview::cargacanales() {
   // Hacemos la carga de los canales. Rellenamos el combobox correspondiente.
   string query1;
   combocanal->clear();
   query1="SELECT * FROM canal";
   conexionbase->begin();
   cursor2 *cursorcanals = conexionbase->cargacursor(query1.c_str(),"canales");
   conexionbase->commit();
   combocanal->insertItem("--",-1);
   ccanales[0]=0;
   int j = 1;
   while (!cursorcanals->eof()) {
      combocanal->insertItem((cursorcanals->valor(2)),-1);
      ccanales[j++] = atoi(cursorcanals->valor(0).ascii());
      cursorcanals->siguienteregistro();
   }// end while
   delete cursorcanals;
}// end cargacanales


void ainteligentesview::cargacostes() {
   // Hacemos la carga de los centros de coste. Rellenamos el combobox correspondiente.
   combocoste->clear();
   string query="SELECT * FROM c_coste ORDER BY nombre";
   conexionbase->begin();
   cursor2 *cursorcoste = conexionbase->cargacursor(query.c_str(),"costes");
   conexionbase->commit();
   combocoste->insertItem("--",0);
   ccostes[0]=0;
   int i=1;
   while (!cursorcoste->eof()) {
      combocoste->insertItem(cursorcoste->valor(2),-1);
      ccostes[i++] = atoi(cursorcoste->valor(0).ascii());
      cursorcoste->siguienteregistro();
   }// end while
   delete cursorcoste;
}// end cargacostes


ainteligentesview::~ainteligentesview(){
   if (cainteligentes != NULL) {
      delete cainteligentes;
   }// end if
}// end ~ainteligentesview


void ainteligentesview::return_asiento() {
  QString cadena;
  cadena = idainteligenteedit->text();
  idasientointeligente = atoi(cadena.ascii());
  repinta();
}// end return_asiento


/*****************************************************************
 * Se ha pulsado sobre el boton nuevo. Asi que creamos una nueva *
 * plantilla.                                                    *
 *****************************************************************/
void ainteligentesview::boton_nuevo() {
  QString query;
  query.sprintf("INSERT INTO ainteligente (descripcion) VALUES ('%s')",tr("Nuevo Asiento Inteligente").ascii());
  fprintf(stderr,query.ascii());
  conexionbase->begin();
  conexionbase->ejecuta(query);
  query.sprintf("SELECT max(idainteligente) AS id FROM ainteligente");
  cursor2 *aux = conexionbase->cargacursor(query,"querysupersuper");
  conexionbase->commit();
  if (!aux->eof()) {
    cargacombo();
    idasientointeligente= atoi(aux->valor(0).ascii());
    repinta();
  }// end if
  delete aux;
}// end boton_nuevo


void ainteligentesview::boton_inicio() {
  QString query;
  query.sprintf("SELECT MIN(idainteligente) AS id FROM ainteligente");
  conexionbase->begin();
  cursor2 * cur = conexionbase->cargacursor(query,"miquery");
  conexionbase->commit();
  if (atoi(cur->valor(0).ascii()) != 0) {
    idasientointeligente = atoi(cur->valor(0).ascii());
    repinta();
  }// end if
}// end boton_inicio


void ainteligentesview::boton_fin() {
  QString query;
  query.sprintf("SELECT MAX(idainteligente) AS id FROM ainteligente");
  conexionbase->begin();
  cursor2 * cur = conexionbase->cargacursor(query,"miquery");
  conexionbase->commit();
  if (atoi(cur->valor(0).ascii()) != 0) {
    idasientointeligente = atoi(cur->valor(0).ascii());
    repinta();
  }// end if
}// end boton_inicio


void ainteligentesview::boton_siguiente() {
  QString query;
  query.sprintf("SELECT MIN(idainteligente) AS id FROM ainteligente WHERE idainteligente > %d",idasientointeligente);
  conexionbase->begin();
  cursor2 * cur = conexionbase->cargacursor(query,"miquery");
  conexionbase->commit();
  if (atoi(cur->valor(0).ascii()) != 0) {
    idasientointeligente = atoi(cur->valor(0).ascii());
    repinta();
  }// end if
}// end boton_siguiente


void ainteligentesview::boton_anterior() {
  QString query;
  query.sprintf("SELECT MAX(idainteligente) AS id FROM ainteligente WHERE idainteligente < %d",idasientointeligente);
  conexionbase->begin();
  cursor2 * cur = conexionbase->cargacursor(query,"miquery");
  conexionbase->commit();
  if (atoi(cur->valor(0).ascii()) != 0) {
    idasientointeligente = atoi(cur->valor(0).ascii());
    repinta();
  }// end if
}// end boton_anterior


/**************************************************************
 * Se ha pulsado sobre el boton borrar, con lo que borraremos *
 * la plantilla actual.                                       *
 **************************************************************/
void ainteligentesview::boton_borrar() {
  fprintf(stderr,"Boton de Borrar\n");
  QString query;
  query.sprintf("DELETE FROM binteligente WHERE idainteligente=%d",idasientointeligente);
  fprintf(stderr,"%s\n",query.ascii());
  conexionbase->begin();
  conexionbase->ejecuta(query);
  query.sprintf("DELETE FROM ainteligente WHERE idainteligente=%d",idasientointeligente);
  conexionbase->ejecuta(query);
  conexionbase->commit();
  fprintf(stderr,"%s\n",query.ascii());
  repinta();
}// end boton_borrar


/******************************************************
 * Con esta funcion hacemos que el valor del campo de *
 * la posicion actual valga igual que el anterior.    *
 ******************************************************/
void ainteligentesview::boton_igualant() {
  fprintf(stderr,"currentPAgeIndes:%d",pestanas->currentPageIndex());
  if (pestanas->currentPageIndex() == 0) {
    int currow = tapunts->currentRow();
    int curcol = tapunts->currentColumn();
    if (currow > 0) {
      tapunts->setText(currow, curcol, tapunts->text(currow-1, curcol));
    }// end if
  } else if(pestanas->currentPageIndex() == 1) {
    int currow = tiva->currentRow();
    int curcol = tiva->currentColumn();
    if (currow > 0) {
      tiva->setText(currow, curcol, tiva->text(currow-1, curcol));
    }// end if
  }// end if
  
}// end boton_igualant
  


/******************************************************
 * Esta función se activa cuando pulsamos sobre el    *
 * boton guardar                                      *
 ******************************************************/
void ainteligentesview::boton_save() {
  int i;
  string cadena;
  string codigocta;
  char desc[300];
  char cod[100];
  char fecha[100];
  char concontable[300];
  char debe[100];
  char haber[100];
  char contrapartida[100];
  char comentario[300];
  char canal[100];
  char marcaconciliacion[100];
  char idc_coste[100];
  char iddiario[100];
  char query[1000];

  string contrapartidaiva;
  string baseimp;
  string iva;
  string factura;
  string idborrador;
  string incregistro;
  string regularizacion;
  string plan349;
  string numorden;
  string cif;
  
  QString canalillo;

  cursor2 *cur;

  conexionbase->begin();
  sprintf(query,"SELECT * FROM ainteligente WHERE idainteligente=%d",idasientointeligente);
  cursor2 *asiento = conexionbase->cargacursor(query,"micursor");
  conexionbase->commit();
  if (asiento->eof()) {
      sprintf(query,"INSERT INTO ainteligente (idainteligente,descripcion, comenatiosasiento) VALUES   (%d,'%s', '%s')",idasientointeligente,descasiento->text().ascii(), comainteligente->text().ascii());
      fprintf(stderr,"%s\n",query);
      conexionbase->ejecuta(query);
  } else {
    sprintf(query,"UPDATE ainteligente SET descripcion='%s', comentariosasiento='%s' WHERE idainteligente=%d",descasiento->text().ascii(),comainteligente->text().ascii(), idasientointeligente);
    fprintf(stderr,"%s\n",query);
    conexionbase->ejecuta(query);
  }// end if
  delete asiento;
  

  // Hacemos la grabacion de los apuntes (tapunts)
  sprintf(query,"DELETE FROM binteligente WHERE idainteligente=%d", idasientointeligente);
  conexionbase->ejecuta(query);
  conexionbase->commit();
  // Cambiamos el foco de tapunts para que coja el ultimo cambio realizado.
  tapunts->setCurrentCell(0,0);
  for (i=0;i<tapunts->numRows();i++) {
    if (!tapunts->text(i,COL_CODCUENTA).isNull()) {
        codigocta = tapunts->text(i,COL_CODCUENTA).ascii();
        if (codigocta.length() != 0) {
           if (!tapunts->text(i,COL_DESCRIPCION).isNull())
             sprintf(desc,"'%s'",tapunts->text(i,COL_DESCRIPCION).ascii());
           else
             sprintf(desc,"NULL");
             
           if (!tapunts->text(i,COL_CODCUENTA).isNull())
             sprintf(cod,"'%s'",tapunts->text(i,COL_CODCUENTA).ascii());
           else
             sprintf(cod,"NULL");
           
           if (!tapunts->text(i,COL_FECHA).isNull())
             sprintf(fecha,"'%s'",tapunts->text(i,COL_FECHA).ascii());
           else
             sprintf(fecha,"NULL");
           
           if (!tapunts->text(i,COL_CONCEPTOCONTABLE).isNull())
             sprintf(concontable,"'%s'",tapunts->text(i,COL_CONCEPTOCONTABLE).ascii());
           else
             sprintf(concontable,"NULL");
             
           if (!tapunts->text(i,COL_DEBE).isNull())
             sprintf(debe,"'%s'",tapunts->text(i,COL_DEBE).ascii());
           else
             sprintf(debe,"NULL");
             
           if (!tapunts->text(i,COL_HABER).isNull())
             sprintf(haber,"'%s'",tapunts->text(i,COL_HABER).ascii());
           else
             sprintf(haber,"NULL");
           
           if (!tapunts->text(i,COL_CONTRAPARTIDA).isNull())
             sprintf(contrapartida,"'%s'",tapunts->text(i,COL_CONTRAPARTIDA).ascii());
           else
             sprintf(contrapartida,"NULL");

           if (!tapunts->text(i,COL_COMENTARIO).isNull())
             sprintf(comentario,"'%s'",tapunts->text(i,COL_COMENTARIO).ascii());
           else
             sprintf(comentario,"NULL");

           if (!tapunts->text(i,COL_CANAL).isNull() && (string) tapunts->text(i,COL_CANAL).ascii() != "0")
             sprintf(canal,"'%s'",tapunts->text(i,COL_CANAL).ascii());
           else
             sprintf(canal,"NULL");
           
           if (!tapunts->text(i,COL_MARCACONCILIACION).isNull())
             sprintf(marcaconciliacion,"'%s'",tapunts->text(i,COL_MARCACONCILIACION).ascii());
           else
             sprintf(marcaconciliacion,"NULL");

           if (!tapunts->text(i,COL_IDC_COSTE).isNull() && (string) tapunts->text(i,COL_IDC_COSTE).ascii() != "0")
             sprintf(idc_coste,"'%s'",tapunts->text(i,COL_IDC_COSTE).ascii());
           else
             sprintf(idc_coste,"NULL");

           
           if (!tapunts->text(i,COL_IDDIARIO).isNull())
             sprintf(iddiario,"'%s'",tapunts->text(i,COL_IDDIARIO).ascii());
           else
             sprintf(iddiario,"NULL");

           sprintf(query,"INSERT INTO binteligente (idainteligente, codcuenta, descripcion, fecha, conceptocontable, debe, haber, contrapartida, comentario, canal, marcaconciliacion, idc_coste, iddiario) VALUES (%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)",idasientointeligente, cod, desc, fecha,concontable, debe, haber, contrapartida, comentario, canal, marcaconciliacion, idc_coste, iddiario);
           conexionbase->begin();
           conexionbase->ejecuta(query);

           sprintf(query,"SELECT max(idbinteligente) AS id FROM binteligente");
           cur = conexionbase->cargacursor(query,"identificador");
           if (atoi(cur->valor(0).ascii()) != 0) 
             tapunts->setText(i,COL_IDBINTELIGENTE, cur->valor(0));
           delete cur;
           conexionbase->commit();
        }// end if
     }// end if
  }// end for


  conexionbase->begin();
  // Hacemos la grabacion del registro de iva (tiva)
  sprintf(query,"DELETE FROM ivainteligente WHERE idainteligente=%d", idasientointeligente);
  conexionbase->ejecuta(query);
  // Cambiamos el foco de tapunts para que coja el ultimo cambio realizado.
  tiva->setCurrentCell(0,0);
  for (i=0;i<tiva->numRows();i++) {
    if (!tiva->text(i,COL_CONTRAPARTIDAIVA).isNull() && ( (string) tiva->text(i,COL_CONTRAPARTIDAIVA).ascii() != "") ) {
        contrapartidaiva = "'"+(string) tiva->text(i,COL_CONTRAPARTIDAIVA).ascii()+"'";
        if (contrapartidaiva.length() != 0) {

          
           if (!tiva->text(i,COL_BASEIMPIVA).isNull())
             baseimp = "'"+(string) tiva->text(i,COL_BASEIMPIVA).ascii()+"'";
           else
             baseimp="NULL";

           if (!tiva->text(i,COL_IVAIVA).isNull())
             iva = "'"+(string) tiva->text(i,COL_IVAIVA).ascii()+"'";
           else
             iva= "NULL";

           if (!tiva->text(i,COL_FACTURAIVA).isNull())
             factura = "'"+(string)tiva->text(i,COL_FACTURAIVA).ascii()+"'";
           else
             factura = "NULL";

           if (!tiva->text(i,COL_IDBORRADORIVA).isNull())
             idborrador = "'"+(string)tiva->text(i,COL_IDBORRADORIVA).ascii()+"'";
           else
             idborrador = "NULL";

           int algo = atoi(tiva->text(i,COL_IDBORRADORIVA).ascii());
           tiva->setText(i,COL_IDBINTELIGENTEIVA,tapunts->text(algo,COL_IDBINTELIGENTE));
           int idbinteligente = atoi(tiva->text(i,COL_IDBINTELIGENTEIVA).ascii());

           if (!tiva->text(i,COL_INCREGISTROIVA).isNull())
             incregistro = "'"+(string)tiva->text(i,COL_INCREGISTROIVA).ascii()+"'";
           else
             incregistro = "NULL";
             
           if (!tiva->text(i,COL_REGULARIZACIONIVA).isNull())
             regularizacion = "'"+(string)tiva->text(i,COL_REGULARIZACIONIVA).ascii()+"'";
           else
             regularizacion = "NULL";

           if (!tiva->text(i,COL_PLAN349IVA).isNull())
             plan349 = "'"+(string)tiva->text(i,COL_PLAN349IVA).ascii()+"'";
           else
             plan349 = "NULL";
                          
           if (!tiva->text(i,COL_NUMORDENIVA).isNull())
             numorden = "'"+(string)tiva->text(i,COL_NUMORDENIVA).ascii()+"'";
           else
             numorden = "NULL";

           if (!tiva->text(i,COL_CIFIVA).isNull())
             cif = "'"+(string)tiva->text(i,COL_CIFIVA).ascii()+"'";
           else
             cif = "NULL";
                          
           sprintf(query,"INSERT INTO ivainteligente (idainteligente, idbinteligente, contrapartida, baseimp, iva, factura, idborrador, incregistro, regularizacion, plan349, numorden, cif) VALUES (%d,%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)",idasientointeligente,idbinteligente, contrapartidaiva.c_str(), baseimp.c_str(), iva.c_str(), factura.c_str(), idborrador.c_str(), incregistro.c_str(), regularizacion.c_str(), plan349.c_str(), numorden.c_str(), cif.c_str());
           conexionbase->ejecuta(query);
        }// end if
     }// end if
  }// end for

  conexionbase->commit();
  cargacombo();
  repinta();
}// end boton_save


/*********************************************************
 * Repintado del asiento inteligente                     *
 *********************************************************/
void ainteligentesview::repinta() {
  int idainteligente = idasientointeligente;
  char query[300];
  char cadena[300];
  cursor2 *cur1;

  
  // Hacemos el que combo Box apunte al registo que se pasa.
  cainteligentes->primerregistro();
  while (!cainteligentes->eof() && cainteligentes->valor("idainteligente").toInt() != idainteligente ) {
    cainteligentes->siguienteregistro();
  }// end while
  m_ainteligente->setCurrentItem(cainteligentes->regactual());
  
  
  // Como se va a perder el centro actual hacemos que no se intente hacer un delete del mismo.
  if (oldrow != -1) {
    tapunts->clearCellWidget(oldrow,COL_NOMCOSTE);
    tapunts->clearCellWidget(oldrow,COL_NOMCANAL);
    delete combocoste;
    delete combocanal;
  }// end if
  oldrow=-2;
  oldcol=-2;
  // Hacemos un vaciado completo de todos los datos
  tapunts->setNumRows(0);
  tapunts->setNumRows(100);
  descasiento->setText("");
    
  sprintf(cadena,"%d",idainteligente);
  idainteligenteedit->setText(cadena);
  sprintf(query,"SELECT * FROM ainteligente WHERE idainteligente=%d",idainteligente);
  conexionbase->begin();
  cursor2 *cur = conexionbase->cargacursor(query,"supquery");
  conexionbase->commit();
  if (!cur->eof()) {
    descasiento->setText(cur->valor("descripcion"));
    comainteligente->setText(cur->valor("comentariosasiento"));
  }// end if
  delete cur;

  // Pintamos las entras de tapunts (apuntes)
  sprintf(query,"SELECT * FROM binteligente WHERE idainteligente=%d",idainteligente);
  conexionbase->begin();
  cur = conexionbase->cargacursor(query,"myquery1");
  conexionbase->commit();
  int i=0;
  while (!cur->eof()) {
    tapunts->setText(i,COL_IDBINTELIGENTE,   cur->valor("idbinteligente"));
    tapunts->setText(i,COL_IDAINTELIGENTE,   cur->valor("idainteligente"));
    tapunts->setText(i,COL_CODCUENTA,   cur->valor("codcuenta"));
    tapunts->setText(i,COL_DESCRIPCION, cur->valor("descripcion"));
    tapunts->setText(i,COL_FECHA, cur->valor("fecha"));
    tapunts->setText(i,COL_CONCEPTOCONTABLE, cur->valor("conceptocontable"));
    tapunts->setText(i,COL_DEBE, cur->valor("debe"));
    tapunts->setText(i,COL_HABER, cur->valor("haber"));
    tapunts->setText(i,COL_CONTRAPARTIDA, cur->valor("contrapartida"));
    tapunts->setText(i,COL_COMENTARIO, cur->valor("comentario"));
    tapunts->setText(i,COL_CANAL, cur->valor("canal"));
    tapunts->setText(i,COL_MARCACONCILIACION, cur->valor("marcaconciliacion"));
    tapunts->setText(i,COL_IDC_COSTE, cur->valor("idc_coste"));
    tapunts->setText(i,COL_IDDIARIO, cur->valor("iddiario"));
    if (cur->valor("canal") != "") {
      QString querycanal = "SELECT * FROM CANAL WHERE idcanal="+cur->valor("canal");
      conexionbase->begin();
      cur1 = conexionbase->cargacursor(querycanal,"canales");
      conexionbase->commit();
      tapunts->setText(i,COL_NOMCANAL,cur1->valor("nombre"));
      delete cur1;
    }// end if
    if (cur->valor("idc_coste") != "") {
      QString querycoste = "SELECT * FROM C_COSTE WHERE idc_coste="+cur->valor("idc_coste");
      conexionbase->begin();
      cur1 = conexionbase->cargacursor(querycoste,"costes");
      conexionbase->commit();
      tapunts->setText(i,COL_NOMCOSTE,cur1->valor("nombre"));
      delete cur1;
    }// end if
    cur->siguienteregistro();
    i++;
  }// end while
  delete cur;


  // Pintamos las entradas de tiva (registro de iva)
  tiva->setNumRows(0);
  tiva->setNumRows(100);
  sprintf(query,"SELECT * FROM ivainteligente WHERE idainteligente=%d",idainteligente);
  conexionbase->begin();
  cur = conexionbase->cargacursor(query,"myquery2");
  conexionbase->commit();
  i=0;
  while (!cur->eof()) {
    tiva->setText(i,COL_IDIVAINTELIGENTE,   cur->valor("idivainteligente"));
    tiva->setText(i,COL_IDAINTELIGENTEIVA,   cur->valor("idainteligente"));
    tiva->setText(i,COL_IDBINTELIGENTEIVA,   cur->valor("idbinteligente"));
    tiva->setText(i,COL_CONTRAPARTIDAIVA, cur->valor("contrapartida"));
    tiva->setText(i,COL_BASEIMPIVA, cur->valor("baseimp"));
    tiva->setText(i,COL_IVAIVA, cur->valor("iva")
	 );
    tiva->setText(i,COL_FACTURAIVA, cur->valor("factura"));
    tiva->setText(i,COL_IDBORRADORIVA, cur->valor("idborrador"));
    tiva->setText(i,COL_INCREGISTROIVA, cur->valor("incregistro"));
    tiva->setText(i,COL_REGULARIZACIONIVA, cur->valor("regularizacion"));
    tiva->setText(i,COL_PLAN349IVA, cur->valor("plan349"));
    tiva->setText(i,COL_NUMORDENIVA, cur->valor("numorden"));
    tiva->setText(i,COL_CIFIVA, cur->valor("cif"));
    cur->siguienteregistro();
    i++;
  }// end while
  delete cur;
  oldrow=-1;
  oldcol=-1;
}// end repinta



void ainteligentesview::currentChanged(int row, int col) {
  char cadena[50];
  int  coste, canal;
  int i;
  
  if (oldrow != -2) {
     if (oldrow != row ) {
       if (oldrow != -1) {
         coste = combocoste->currentItem();
         fprintf(stderr,"pos:%d ,idcentrocoste: %d \n",coste, ccostes[coste]);
         sprintf(cadena,"%d",ccostes[coste]);
         tapunts->setText(oldrow, COL_IDC_COSTE, cadena);
         
         canal = combocanal->currentItem();
         sprintf(cadena,"%d",ccanales[canal]);
         tapunts->setText(oldrow, COL_CANAL, cadena);
                           
         tapunts->clearCellWidget(oldrow,COL_NOMCOSTE);
         tapunts->clearCellWidget(oldrow,COL_NOMCANAL);
         tapunts->setText(oldrow,COL_NOMCOSTE,combocoste->currentText());
         tapunts->setText(oldrow,COL_NOMCANAL,combocanal->currentText());
         delete combocoste;
         delete combocanal;
       }// end if
       // Cargamos el combo auxiliar de centros de coste y el combo auxiliar de canales.
       combocoste= new QComboBox(0,"");
       combocanal = new QComboBox(0,"");
       cargacostes();
       cargacanales();
       tapunts->setCellWidget(row,COL_NOMCOSTE,combocoste);
       tapunts->setCellWidget(row,COL_NOMCANAL,combocanal);

       if (!tapunts->text(row,COL_IDC_COSTE).isNull()) {
         coste = atoi(tapunts->text(row,COL_IDC_COSTE).ascii());
         i=0;
         while (ccostes[i] != coste && i < 100)
           i++;
         if (i < 100)
          combocoste->setCurrentItem(i);
       }// end if

       if (!tapunts->text(row,COL_CANAL).isNull()) {
          canal = atoi(tapunts->text(row,COL_CANAL).ascii());
          i=0;
          while (ccanales[i] != canal && i < 100)
            i++;
          if (i < 100)
            combocanal->setCurrentItem(i);
       }// end if
         
     }// end if
     oldrow=row;
     oldcol=col;
  }// end if
}// end currentChanged


void ainteligentesview::selectionChanged(){
  fprintf(stderr,"Selection changed \n");
}// end selectionChanged


void ainteligentesview::accept() {
  boton_save();
  done(1);
}// end accept


void ainteligentesview::boton_cuentas() {
   listcuentasview1 *listcuentas = new listcuentasview1();
   listcuentas->modo=1;
   listcuentas->inicializa(conexionbase);
   listcuentas->exec();
   if (pestanas->currentPageIndex() == 0) {
     tapunts->setText(tapunts->currentRow(), tapunts->currentColumn(), listcuentas->codcuenta);
   } else if (pestanas->currentPageIndex() == 1) {
     tiva->setText(tiva->currentRow(), tiva->currentColumn(), listcuentas->codcuenta);
   }// end if
   delete listcuentas;  
}// end boton_cuentas

void ainteligentesview::boton_exportar() {
   QString fn = QFileDialog::getSaveFileName(0, tr("AInteligente (*.xml)"), 0,tr("Guardar Asiento Inteligente"),tr("Elige el nombre de archivo"));
   if (!fn.isEmpty()) {
      FILE *mifile;
      mifile = fopen((char *) fn.ascii(),"wt");
      if (mifile != NULL) {
          QString SQlQuery;
          SQlQuery.sprintf("SELECT * FROM ainteligente WHERE idainteligente=%d", idasientointeligente);
         conexionbase->begin();
         cursor2 *cursp = conexionbase->cargacursor(SQlQuery,"asiento");
         conexionbase->commit();
         fprintf(mifile,"<?xml version=\"1.0\"?>\n");
         fprintf(mifile,"<DOCUMENT>\n");
         while (!cursp->eof()) {
            fprintf(mifile,"   <ainteligente>\n");
            fprintf(mifile,"      <descripcion>%s</descripcion>\n", cursp->valor("descripcion").ascii());
            fprintf(mifile,"      <comentariosasiento>%s</comentariosasiento>\n", cursp->valor("comentariosasiento").ascii());
            QString SQlQuery1;
            SQlQuery1.sprintf("SELECT * FROM binteligente WHERE idainteligente=%d", idasientointeligente);
            conexionbase->begin();
            cursor2 *cursp1 = conexionbase->cargacursor(SQlQuery1,"a1siento");
            conexionbase->commit();
            while (!cursp1->eof()) {
               fprintf(mifile,"      <binteligente>\n");
               fprintf(mifile,"         <fecha>%s</fecha>\n", cursp1->valor("fecha").ascii());
               fprintf(mifile,"         <conceptocontable>%s</conceptocontable>\n", cursp1->valor("conceptocontable").ascii());
               fprintf(mifile,"         <codcuenta>%s</codcuenta>\n", cursp1->valor("codcuenta").ascii());
               fprintf(mifile,"         <descripcionb>%s</descripcionb>\n", cursp1->valor("descripcion").ascii());
               fprintf(mifile,"         <debe>%s</debe>\n", cursp1->valor("debe").ascii());
               fprintf(mifile,"         <haber>%s</haber>\n", cursp1->valor("haber").ascii());
               fprintf(mifile,"         <contrapartida>%s</contrapartida>\n", cursp1->valor("contrapartida").ascii());
               fprintf(mifile,"         <comentario>%s</comentario>\n", cursp1->valor("comentario").ascii());
               fprintf(mifile,"         <canal>%s</canal>\n", cursp1->valor("canal").ascii());
               fprintf(mifile,"         <marcaconciliacion>%s</marcaconciliacion>\n", cursp1->valor("marcaconciliacion").ascii());
               fprintf(mifile,"         <idc_coste>%s</idc_coste>\n", cursp1->valor("idc_coste").ascii());
               QString SQlQuery2;
               SQlQuery2.sprintf("SELECT * FROM ivainteligente WHERE idbinteligente=%s", cursp1->valor("idbinteligente").ascii());
               conexionbase->begin();
               cursor2 *cursp2 = conexionbase->cargacursor(SQlQuery2,"a2siento");
               conexionbase->commit();
               while (!cursp2->eof()) {
                  fprintf(mifile,"         <ivainteligente>\n");
                  fprintf(mifile,"            <contrapartida>%s</contrapartida>\n", cursp2->valor("contrapartida").ascii());
                  fprintf(mifile,"            <baseimp>%s</baseimp>\n", cursp2->valor("baseimp").ascii());
                  fprintf(mifile,"            <iva>%s</iva>\n", cursp2->valor("iva").ascii());
                  fprintf(mifile,"            <factura>%s</factura>\n", cursp2->valor("factura").ascii());
                  fprintf(mifile,"            <idborrador>%s</idborrador>\n", cursp2->valor("idborrador").ascii());
                  fprintf(mifile,"            <incregistro>%s</incregistro>\n", cursp2->valor("incregistro").ascii());
                  fprintf(mifile,"            <regularizacion>%s</regularizacion>\n", cursp2->valor("regularizacion").ascii());
                  fprintf(mifile,"            <plan349>%s</plan349>\n", cursp2->valor("plan349").ascii());
                  fprintf(mifile,"            <numorden>%s</numorden>\n", cursp2->valor("numorden").ascii());
                  fprintf(mifile,"            <cif>%s</cif>\n", cursp2->valor("cif").ascii());
                  fprintf(mifile,"         </ivainteligente>\n");
                  cursp2->siguienteregistro();
               }// end while
               delete cursp2;
               fprintf(mifile,"      </binteligente>\n");
               cursp1->siguienteregistro();
            }// end while
            delete cursp1;
            fprintf(mifile,"   </ainteligente>\n");            
            cursp->siguienteregistro();
         }// end while
         delete cursp;
         fprintf(mifile,"</DOCUMENT>\n");
         fclose(mifile);
      }// end if
  }// end if   
  QMessageBox::warning( this,"BulmaGés", "Se ha exportado el asiento inteligente.", "OK",  "No OK", 0, 0, 1 );  
}// end boton_exportar


void ainteligentesview::boton_importar() {
   QString fn = QFileDialog::getOpenFileName(0, tr("Asientos Inteligentes (*.xml)"), 0,tr("Cargar Asientos Inteligentes"),tr("Elige el nombre de archivo"));
   if (!fn.isEmpty()) {      
      // Hacemos el parsing del XML
      QFile xmlFile( fn);              // Declaramos el ficheros
      QXmlInputSource source( &xmlFile ); // Declaramos el inputsource, con el fichero como parámetro
      QXmlSimpleReader reader;            // Declaramos el lector
      
      importainteligente * handler = new importainteligente( empresaactual );
      reader.setContentHandler( handler );
      reader.parse( source );   
      
      cargacombo();
      boton_fin();
   }// end if
    
}// end boton_importar
