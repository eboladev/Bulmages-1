/***************************************************************************
                          aboutview.cpp  -  description
                             -------------------
    begin                : Fri Dec 27 2004
    copyright            : (C) 2002 by Tomeu Borrás Riera
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

#include "adocumental.h"
#include "empresa.h"

#define COL_IDADOCUMENTAL 0
#define COL_IDASIENTO 1
#define COL_DESCRIPCIONADOCUMENTAL 2
#define COL_FECHAINTADOCUMENTAL 3
#define COL_FECHAASADOCUMENTAL 4
#define COL_ARCHIVOADOCUMENTAL 5


adocumental::adocumental(empresa *emp,QWidget *parent, const char *name ) : adocumentalbase(parent,name,false,0) {
   empresaactual=emp;
   conexionbase = emp->bdempresa();
   
  m_listado->setNumRows(0);
  m_listado->setNumCols(6);
  m_listado->horizontalHeader()->setLabel( COL_IDADOCUMENTAL, tr( "COL_IDADOCUMENTAL" ) );
  m_listado->horizontalHeader()->setLabel( COL_IDASIENTO, tr( "COL_IDASIENTO" ) );
  m_listado->horizontalHeader()->setLabel( COL_DESCRIPCIONADOCUMENTAL, tr( "COL_DESCRIPCIONADOCUMENTAL" ) );
  m_listado->horizontalHeader()->setLabel( COL_FECHAINTADOCUMENTAL, tr( "FECHAINTADOCUMENTAL" ) );
  m_listado->horizontalHeader()->setLabel( COL_FECHAASADOCUMENTAL, tr( "FECHAASADOCUMENTAL" ) );
  m_listado->horizontalHeader()->setLabel( COL_ARCHIVOADOCUMENTAL, tr( "COL_ARCHIVOADOCUMENTAL" ) );
  m_listado->setColumnWidth(COL_DESCRIPCIONADOCUMENTAL,200);
  QString query = "SELECT * FROM adocumental";
  conexionbase->begin();
  cursor2 *cursoraux1=conexionbase->cargacursor(query,"elquery");
  conexionbase->commit();
  m_listado->setNumRows(cursoraux1->numregistros());
  int i=0;
   while (!cursoraux1->eof()) {
      m_listado->setText(i,COL_IDADOCUMENTAL, cursoraux1->valor("idadocumental"));
      m_listado->setText(i,COL_IDASIENTO, cursoraux1->valor("idasiento"));
      m_listado->setText(i,COL_DESCRIPCIONADOCUMENTAL, cursoraux1->valor("descripcionadocumental"));
      m_listado->setText(i,COL_FECHAINTADOCUMENTAL, cursoraux1->valor("fechaintadocumental"));
      m_listado->setText(i,COL_FECHAASADOCUMENTAL, cursoraux1->valor("fechaasadocumental"));
      m_listado->setText(i,COL_ARCHIVOADOCUMENTAL, cursoraux1->valor("archivoadocumental"));
      cursoraux1->siguienteregistro ();
      i++;
   }// end while
   delete cursoraux1;
   
  query = "SELECT * FROM configuracion WHERE nombre='RutaADocumental'";
  conexionbase->begin();
  cursor2 *cursoraux2=conexionbase->cargacursor(query,"thequery");
  conexionbase->commit();
  if (!cursoraux2->eof()) {
      RutaADocumental = cursoraux2->valor("valor");
  } else {
      RutaADocumental = "/tmp";
  }// end if
   
}// end adocumental

adocumental::~adocumental(){
}// end adocumental



void adocumental::doubleclicked(int row, int, int, const QPoint &) {
   QString archivo = m_listado->text(row, COL_ARCHIVOADOCUMENTAL);
   QString comando = "konqueror "+archivo+" &";
   system(comando.ascii());
}// end dobleclicked


void adocumental::boton_newadocumental() {
   QString fn = QFileDialog::getOpenFileName(0, tr("Punteos (*.*)"), 0,tr("Agregar Documento"),tr("Elige el nombre de archivo"));
   if (!fn.isEmpty()) {
      fprintf(stderr,"%s\n",fn.ascii());
      QString comando= "cp "+fn+" "+RutaADocumental;
      QString archivo = fn.right( fn.length()-fn.find('/',-1));
      system(comando.ascii());
      QString SQLQuery = "INSERT INTO adocumental (archivoadocumental) VALUES ('"+archivo+"')";
      conexionbase->begin();
      conexionbase->ejecuta(SQLQuery);
      conexionbase->commit();
      fprintf(stderr,"%s\n", SQLQuery.ascii());
   }// end if
}// end boton_newadocumental

