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
#include <q3filedialog.h>
#include <qlineedit.h>
#include <q3table.h>
#include <q3textedit.h>
#include <qcombobox.h>
#include <qtabwidget.h>

#include "importainteligente.h"
#include "empresa.h"
#include "postgresiface2.h"


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



/** \brief El constructor de la clase debe inicializar todos los componentes de la pantalla
  *
  * Inicializa la \brief conexionbase y \ref empresaactual
  * Crea las filas y columnas en la tabla de detalle de los asientos inteligentes.
  * Inicializa las variables auxiliares que usa la clase
  */
ainteligentesview::ainteligentesview(empresa *emp, QWidget *parent, const char *name, bool modal ) : ainteligentesdlg(parent,name, modal) {
   empresaactual = emp;
   conexionbase = empresaactual->bdempresa();
   m_oldRow=-2;

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
   
   /// El cursor que recorre los asientos inteligentes debe iniciarse a NULL
   m_cAInteligentes = NULL;
   
   /// Hacemos la inicialización inicial.
   m_oldRow=-1;
   cargacombo();
   /// Llamamos a boton_fin para que la pantalla aparezca con un asiento inteligente inicial.
   boton_fin();
}// end ainteligentesview


/** \brief Cargar el combo box \ref m_ainteligente que contiene todos los asientos cargados
  *
  * Certifica que el \ref cursor2 m_cAInteligentes este liberado.
  * Crea un query cargando todos los asientos inteligentes que haya en la base de datos
  * Agrega todos los componentes al comoboBox
  */
void ainteligentesview::cargacombo() {
   m_ainteligente->clear();
   if (m_cAInteligentes != NULL) {
      delete m_cAInteligentes;
   }// end if
   /// Vamos a cargar el comboBox para que la cosa pinte mejor.
   QString SQLQuery = "SELECT * FROM ainteligente";
   m_cAInteligentes = conexionbase->cargacursor(SQLQuery);
   while (!m_cAInteligentes->eof()) {
      m_ainteligente->insertItem(m_cAInteligentes->valor("idainteligente")+m_cAInteligentes->valor("descripcion"));
      m_cAInteligentes->siguienteregistro();
   }// end while
}// end if


/** \brief Cuando se cambia el contenido del comoboBox de los asientos se cambia toda la pantalla
  *
  * Esta función es llamada cuando se ha seleccionado un item diferente en el comboBox m_aintelignete
  * Entonces cambia la variable de clase \ref m_idAsientoInteligente con el valor que le corresponde y
  * llama a la función de repintar la pantalla.
  \todo No hay verificación ni control de cambios.
  */
void ainteligentesview::comboActivated(const QString &) {
   QString idasiento = m_cAInteligentes->valor("idainteligente",m_ainteligente->currentItem());
   m_idAsientoInteligente=idasiento.toInt();
   repinta();
}// end comboActivated


void ainteligentesview::cargacanales() {
   // Hacemos la carga de los canales. Rellenamos el combobox correspondiente.
   QString query1;
   combocanal->clear();
   query1="SELECT * FROM canal";
   cursor2 *cursorcanals = conexionbase->cargacursor(query1);
   combocanal->insertItem("--",-1);
   m_cCanales[0]=0;
   int j = 1;
   while (!cursorcanals->eof()) {
      combocanal->insertItem((cursorcanals->valor("nombre")),-1);
      m_cCanales[j++] = atoi(cursorcanals->valor("idcanal").ascii());
      cursorcanals->siguienteregistro();
   }// end while
   delete cursorcanals;
}// end cargacanales


void ainteligentesview::cargacostes() {
   // Hacemos la carga de los centros de coste. Rellenamos el combobox correspondiente.
   combocoste->clear();
   string query="SELECT * FROM c_coste ORDER BY nombre";
   cursor2 *cursorcoste = conexionbase->cargacursor(query.c_str(),"costes");
   combocoste->insertItem("--",0);
   m_cCostes[0]=0;
   int i=1;
   while (!cursorcoste->eof()) {
      combocoste->insertItem(cursorcoste->valor("nombre"),-1);
      m_cCostes[i++] = atoi(cursorcoste->valor("idc_coste").ascii());
      cursorcoste->siguienteregistro();
   }// end while
   delete cursorcoste;
}// end cargacostes


ainteligentesview::~ainteligentesview(){
   if (m_cAInteligentes != NULL) {
      delete m_cAInteligentes;
   }// end if
}// end ~ainteligentesview


void ainteligentesview::return_asiento() {
  QString cadena;
  cadena = idainteligenteedit->text();
  m_idAsientoInteligente = atoi(cadena.ascii());
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
    m_idAsientoInteligente= atoi(aux->valor(0).ascii());
    repinta();
  }// end if
  delete aux;
}// end boton_nuevo


void ainteligentesview::boton_inicio() {
  QString query;
  query.sprintf("SELECT MIN(idainteligente) AS id FROM ainteligente");
  cursor2 * cur = conexionbase->cargacursor(query,"miquery");
  if (atoi(cur->valor(0).ascii()) != 0) {
    m_idAsientoInteligente = atoi(cur->valor(0).ascii());
    repinta();
  }// end if
}// end boton_inicio


void ainteligentesview::boton_fin() {
  QString query;
  query.sprintf("SELECT MAX(idainteligente) AS id FROM ainteligente");
  cursor2 * cur = conexionbase->cargacursor(query,"miquery");
  if (atoi(cur->valor(0).ascii()) != 0) {
    m_idAsientoInteligente = atoi(cur->valor(0).ascii());
    repinta();
  }// end if
}// end boton_inicio


void ainteligentesview::boton_siguiente() {
  QString query;
  query.sprintf("SELECT MIN(idainteligente) AS id FROM ainteligente WHERE idainteligente > %d",m_idAsientoInteligente);
  cursor2 * cur = conexionbase->cargacursor(query,"miquery");
  if (atoi(cur->valor(0).ascii()) != 0) {
    m_idAsientoInteligente = atoi(cur->valor(0).ascii());
    repinta();
  }// end if
}// end boton_siguiente


void ainteligentesview::boton_anterior() {
  QString query;
  query.sprintf("SELECT MAX(idainteligente) AS id FROM ainteligente WHERE idainteligente < %d",m_idAsientoInteligente);
  cursor2 * cur = conexionbase->cargacursor(query,"miquery");
  if (atoi(cur->valor(0).ascii()) != 0) {
    m_idAsientoInteligente = atoi(cur->valor(0).ascii());
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
  query.sprintf("DELETE FROM binteligente WHERE idainteligente=%d",m_idAsientoInteligente);
  fprintf(stderr,"%s\n",query.ascii());
  conexionbase->begin();
  conexionbase->ejecuta(query);
  query.sprintf("DELETE FROM ainteligente WHERE idainteligente=%d",m_idAsientoInteligente);
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
  }// end if
}// end boton_igualant
  


/******************************************************
 * Esta función se activa cuando pulsamos sobre el    *
 * boton guardar                                      *
 ******************************************************/
void ainteligentesview::boton_save() {
  int i;
  
  QString codigocta;

  
  QString desc;
  QString cod;
  QString fecha;
  QString concontable;
  QString debe;
  QString haber;
  QString contrapartida;
  QString comentario;
  QString canal;
  QString marcaconciliacion;
  QString idc_coste;
  QString iddiario;
  QString query;
  
  QString canalillo;

  cursor2 *cur;

  query.sprintf("SELECT * FROM ainteligente WHERE idainteligente=%d",m_idAsientoInteligente);
  cursor2 *asiento = conexionbase->cargacursor(query,"micursor");
  if (asiento->eof()) {
      query.sprintf("INSERT INTO ainteligente (idainteligente,descripcion, comenatiosasiento) VALUES   (%d,'%s', '%s')",
      m_idAsientoInteligente,
      conexionbase->sanearCadena(descasiento->text()).ascii(), 
      conexionbase->sanearCadena(comainteligente->text()).ascii());
      conexionbase->ejecuta(query);
  } else {
    query.sprintf("UPDATE ainteligente SET descripcion='%s', comentariosasiento='%s' WHERE idainteligente=%d",
    conexionbase->sanearCadena(descasiento->text()).ascii(),
    conexionbase->sanearCadena(comainteligente->text()).ascii(),
     m_idAsientoInteligente);
    conexionbase->ejecuta(query);
  }// end if
  delete asiento;
  

  // Hacemos la grabacion de los apuntes (tapunts)
  query.sprintf("DELETE FROM binteligente WHERE idainteligente=%d", m_idAsientoInteligente);
  conexionbase->ejecuta(query);
  // Cambiamos el foco de tapunts para que coja el ultimo cambio realizado.
  tapunts->setCurrentCell(0,0);
  for (i=0;i<tapunts->numRows();i++) {
    if (!tapunts->text(i,COL_CODCUENTA).isNull()) {
        codigocta = tapunts->text(i,COL_CODCUENTA).ascii();
        if (codigocta.length() != 0) {
           if (!tapunts->text(i,COL_DESCRIPCION).isNull())
             desc ="'"+tapunts->text(i,COL_DESCRIPCION)+"'";
           else
             desc = "NULL";
             
           if (!tapunts->text(i,COL_CODCUENTA).isNull())
             cod = "'"+tapunts->text(i,COL_CODCUENTA)+"'";
           else
             cod = "NULL";
           
		fprintf(stderr,"Codigo cuenta: %s\n",cod.ascii());

           if (!tapunts->text(i,COL_FECHA).isNull())
             fecha = "'"+tapunts->text(i,COL_FECHA)+"'";
           else
             fecha = "NULL";
           
           if (!tapunts->text(i,COL_CONCEPTOCONTABLE).isNull())
             concontable = "'"+tapunts->text(i,COL_CONCEPTOCONTABLE)+"'";
           else
             concontable = "NULL";
             
           if (!tapunts->text(i,COL_DEBE).isNull())
             debe = "'"+tapunts->text(i,COL_DEBE)+"'";
           else
             debe = "NULL";
             
           if (!tapunts->text(i,COL_HABER).isNull())
             haber = "'"+tapunts->text(i,COL_HABER)+"'";
           else
             haber = "NULL";
           
           if (!tapunts->text(i,COL_CONTRAPARTIDA).isNull())
             contrapartida = "'"+tapunts->text(i,COL_CONTRAPARTIDA)+"'";
           else
             contrapartida = "NULL";

           if (!tapunts->text(i,COL_COMENTARIO).isNull())
             comentario = "'"+tapunts->text(i,COL_COMENTARIO)+"'";
           else
             comentario = "NULL";

           if (!tapunts->text(i,COL_CANAL).isNull() && (string) tapunts->text(i,COL_CANAL).ascii() != "0")
             canal = "'"+tapunts->text(i,COL_CANAL)+"'";
           else
             canal = "NULL";
           
           if (!tapunts->text(i,COL_MARCACONCILIACION).isNull())
             marcaconciliacion = "'"+tapunts->text(i,COL_MARCACONCILIACION)+"'";
           else
             marcaconciliacion = "NULL";

           if (!tapunts->text(i,COL_IDC_COSTE).isNull() && (string) tapunts->text(i,COL_IDC_COSTE).ascii() != "0")
             idc_coste = "'"+tapunts->text(i,COL_IDC_COSTE)+"'";
           else
             idc_coste = "NULL";

           if (!tapunts->text(i,COL_IDDIARIO).isNull())
             iddiario = "'"+tapunts->text(i,COL_IDDIARIO)+ "'";
           else
             iddiario = "NULL";

// AL SANEAR LAS CADENAS DE CARACTERES DA ALGUNOS ERRORES.
/*
	   cod = conexionbase->sanearCadena(cod);
	   desc = conexionbase->sanearCadena(desc);
	   fecha = conexionbase->sanearCadena(fecha);
	   concontable = conexionbase->sanearCadena(concontable);
	   debe = conexionbase->sanearCadena(debe);
	   haber = conexionbase->sanearCadena(haber);
	   contrapartida = conexionbase->sanearCadena(contrapartida);
	   comentario = conexionbase->sanearCadena(comentario);
	   canal = conexionbase->sanearCadena(canal);
	   marcaconciliacion = conexionbase->sanearCadena(marcaconciliacion);
*/

           query.sprintf("INSERT INTO binteligente (idainteligente, codcuenta, descripcion, fecha, conceptocontable, debe, haber, contrapartida, comentario, canal, marcaconciliacion, idc_coste, iddiario) VALUES (%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)",m_idAsientoInteligente, cod.ascii(), desc.ascii(), fecha.ascii(),concontable.ascii(), debe.ascii(), haber.ascii(), contrapartida.ascii(), comentario.ascii(), canal.ascii(), marcaconciliacion.ascii(), idc_coste.ascii(), iddiario.ascii());
           conexionbase->begin();
           conexionbase->ejecuta(query);
           query.sprintf("SELECT max(idbinteligente) AS id FROM binteligente");
           cur = conexionbase->cargacursor(query,"identificador");
           if (atoi(cur->valor(0).ascii()) != 0) 
             tapunts->setText(i,COL_IDBINTELIGENTE, cur->valor(0));
           delete cur;
           conexionbase->commit();
        }// end if
     }// end if
  }// end for
  cargacombo();
  repinta();
}// end boton_save


/*********************************************************
 * Repintado del asiento inteligente                     *
 *********************************************************/
void ainteligentesview::repinta() {
  int idainteligente = m_idAsientoInteligente;
  char query[300];
  char cadena[300];
  cursor2 *cur1;

  
  // Hacemos el que combo Box apunte al registo que se pasa.
  m_cAInteligentes->primerregistro();
  while (!m_cAInteligentes->eof() && m_cAInteligentes->valor("idainteligente").toInt() != idainteligente ) {
    m_cAInteligentes->siguienteregistro();
  }// end while
  m_ainteligente->setCurrentItem(m_cAInteligentes->regactual());
  
  
  // Como se va a perder el centro actual hacemos que no se intente hacer un delete del mismo.
  if (m_oldRow != -1) {
    tapunts->clearCellWidget(m_oldRow,COL_NOMCOSTE);
    tapunts->clearCellWidget(m_oldRow,COL_NOMCANAL);
    delete combocoste;
    delete combocanal;
  }// end if
  m_oldRow=-2;
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
  m_oldRow=-1;
}// end repinta



void ainteligentesview::currentChanged(int row, int) {
  char cadena[50];
  int  coste, canal;
  int i;
  
  if (m_oldRow != -2) {
     if (m_oldRow != row ) {
       if (m_oldRow != -1) {
         coste = combocoste->currentItem();
         fprintf(stderr,"pos:%d ,idcentrocoste: %d \n",coste, m_cCostes[coste]);
         sprintf(cadena,"%d",m_cCostes[coste]);
         tapunts->setText(m_oldRow, COL_IDC_COSTE, cadena);
         
         canal = combocanal->currentItem();
         sprintf(cadena,"%d",m_cCanales[canal]);
         tapunts->setText(m_oldRow, COL_CANAL, cadena);
                           
         tapunts->clearCellWidget(m_oldRow,COL_NOMCOSTE);
         tapunts->clearCellWidget(m_oldRow,COL_NOMCANAL);
         tapunts->setText(m_oldRow,COL_NOMCOSTE,combocoste->currentText());
         tapunts->setText(m_oldRow,COL_NOMCANAL,combocanal->currentText());
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
         while (m_cCostes[i] != coste && i < 100)
           i++;
         if (i < 100)
          combocoste->setCurrentItem(i);
       }// end if

       if (!tapunts->text(row,COL_CANAL).isNull()) {
          canal = atoi(tapunts->text(row,COL_CANAL).ascii());
          i=0;
          while (m_cCanales[i] != canal && i < 100)
            i++;
          if (i < 100)
            combocanal->setCurrentItem(i);
       }// end if
         
     }// end if
     m_oldRow=row;
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
   listcuentasview1 *listcuentas = new listcuentasview1(empresaactual);
   listcuentas->setModoLista();
   listcuentas->inicializa();
   listcuentas->exec();
   if (pestanas->currentPageIndex() == 0) {
     tapunts->setText(tapunts->currentRow(), tapunts->currentColumn(), listcuentas->codcuenta());
   }// end if
   delete listcuentas;  
}// end boton_cuentas

void ainteligentesview::boton_exportar() {
   QString fn = Q3FileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER), tr("AInteligente (*.xml)"), 0,tr("Guardar Asiento Inteligente"),tr("Elige el nombre de archivo"));
   if (!fn.isEmpty()) {
      FILE *mifile;
      mifile = fopen((char *) fn.ascii(),"wt");
      if (mifile != NULL) {
          QString SQlQuery;
          SQlQuery.sprintf("SELECT * FROM ainteligente WHERE idainteligente=%d", m_idAsientoInteligente);
         conexionbase->begin();
         cursor2 *cursp = conexionbase->cargacursor(SQlQuery,"asiento");
         conexionbase->commit();
         fprintf(mifile,"<?xml version=\"1.0\"?>\n");
         fprintf(mifile,"<DOCUMENT>\n");
         while (!cursp->eof()) {
            fprintf(mifile,"   <ainteligente>\n");
            fprintf(mifile,"      <descripcion>%s</descripcion>\n", XMLProtect(cursp->valor("descripcion")).ascii());
            fprintf(mifile,"      <comentariosasiento>%s</comentariosasiento>\n", XMLProtect(cursp->valor("comentariosasiento")).ascii());
            QString SQlQuery1;
            SQlQuery1.sprintf("SELECT * FROM binteligente WHERE idainteligente=%d", m_idAsientoInteligente);
            conexionbase->begin();
            cursor2 *cursp1 = conexionbase->cargacursor(SQlQuery1,"a1siento");
            conexionbase->commit();
            while (!cursp1->eof()) {
               fprintf(mifile,"      <binteligente>\n");
               fprintf(mifile,"         <fecha>%s</fecha>\n", XMLProtect(cursp1->valor("fecha")).ascii());
               fprintf(mifile,"         <conceptocontable>%s</conceptocontable>\n", XMLProtect(cursp1->valor("conceptocontable")).ascii());
               fprintf(mifile,"         <codcuenta>%s</codcuenta>\n", XMLProtect(cursp1->valor("codcuenta")).ascii());
               fprintf(mifile,"         <descripcionb>%s</descripcionb>\n", XMLProtect(cursp1->valor("descripcion")).ascii());
               fprintf(mifile,"         <debe>%s</debe>\n", XMLProtect(cursp1->valor("debe")).ascii());
               fprintf(mifile,"         <haber>%s</haber>\n", XMLProtect(cursp1->valor("haber")).ascii());
               fprintf(mifile,"         <contrapartida>%s</contrapartida>\n", XMLProtect(cursp1->valor("contrapartida")).ascii());
               fprintf(mifile,"         <comentario>%s</comentario>\n", XMLProtect(cursp1->valor("comentario")).ascii());
               fprintf(mifile,"         <canal>%s</canal>\n", XMLProtect(cursp1->valor("canal")).ascii());
               fprintf(mifile,"         <marcaconciliacion>%s</marcaconciliacion>\n", XMLProtect(cursp1->valor("marcaconciliacion")).ascii());
               fprintf(mifile,"         <idc_coste>%s</idc_coste>\n", XMLProtect(cursp1->valor("idc_coste")).ascii());
               QString SQlQuery2;
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
   QString fn = Q3FileDialog::getOpenFileName("/usr/share/bulmages/ainteligentes", tr("Asientos Inteligentes (*.xml)"), 0,tr("Cargar Asientos Inteligentes"),tr("Elige el nombre de archivo"));
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
