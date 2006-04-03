/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras                               *
 *   tborras@conetxia.com                                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "selectcanalview.h"
#include "empresa.h"
#include <qlistview.h>


selectcanalview::selectcanalview(empresa *emp,QWidget *parent, const char
*name)
 : QDialog(parent, name)
{
  setupUi(this);
  fprintf(stderr,"Inicializacion del selector de canales\n");
  empresaactual = emp;
  conexionbase = empresaactual->bdempresa();

  numdigitos = empresaactual->numdigitosempresa();
//   m_iterador = new QListViewItemIterator (m_listCanales, 0x00001000);
  m_iterador = new QListViewItemIterator (m_listCanales);
  
  m_colNomCoste = m_listCanales->addColumn("nom_canal",-1);
  m_colDescCoste = m_listCanales->addColumn("desc_canal",-1);
  m_colStatusCoste = m_listCanales->addColumn("Status",-1);
  m_colIdCoste = m_listCanales->addColumn("idcanal",0);
  m_colCheck = m_listCanales->addColumn("Seleccion",-1);

  cargacanales();
  fprintf(stderr,"Fin del Inicializacion del selector de canales\n");
}// end selectccsotedlg


selectcanalview::~selectcanalview() {
   delete m_iterador;
}// end selectccosteview


void selectcanalview::cargacanales() {
   QCheckListItem *it;
   QCheckListItem *Lista[10000];
    int idcanal=0;
    cursor2 *cursoraux1;

    // Cogemos los centros de coste principales y los ponemos donde toca.
    m_listCanales->clear();
    conexionbase->begin();
    cursoraux1 = conexionbase->cargacursor("SELECT * FROM canal","canalillos");
    conexionbase->commit();
    while (!cursoraux1->eof()) {
        idcanal = atoi( cursoraux1->valor("idcanal").ascii());
        it =new QCheckListItem(m_listCanales,"hola pepsi",QCheckListItem::CheckBox);
        Lista[idcanal]=it;
        it->setText(m_colIdCoste, cursoraux1->valor("idcanal"));
        it->setText(m_colDescCoste,cursoraux1->valor("descripcion"));
        it->setText(m_colNomCoste, cursoraux1->valor("nombre"));
        it->setOpen(true);
        cursoraux1->siguienteregistro ();
    }// end while
    delete cursoraux1;
}// end cargacostes


// Esta función devuelve el primer centro de coste seleccionado de la vita.
// Devuelve el idc_coste. Si no hay ningun centro de coste seleccionado devuelve
// cero
int selectcanalview::firstcanal() {
   delete m_iterador;
   m_iterador = new QListViewItemIterator (m_listCanales);
   return nextcanal();
}// end firstccoste

// Esta función devuelve el siguiente centro de coste seleccionado de la vista.
int selectcanalview::nextcanal() {
   int idcanal=0;
   QCheckListItem *item;
   fprintf(stderr,"nextcanal\n");
   while (m_iterador->current() && idcanal==0) {
      item = (QCheckListItem *) m_iterador->current();
      if (item->isOn()) {
         idcanal = item->text(m_colIdCoste).toInt();
         fprintf(stderr,"primer canal:%d\n",idcanal);
      }// end if
     (*m_iterador)++;
   }// end while
  return idcanal;
}// end nextccoste


QString selectcanalview::cadcanal() {
   int idcanal;
  QString  ccanales="";
  
  idcanal = firstcanal();
  while (idcanal) {
     if (ccanales != "") 
        ccanales.sprintf("%s, %d",ccanales.ascii(), idcanal);
     else
        ccanales.sprintf("%d", idcanal);
     idcanal= nextcanal();
  }// end while
  return ccanales; ;
}// end cadcoste


// Esta función devuelve el nombre de un canal determinado
QString selectcanalview::nomcanal() {
   int idcanal=0;
   QCheckListItem *item;
   (*m_iterador)--;
   item = (QCheckListItem *) m_iterador->current();
   (*m_iterador)++;
   if (item->isOn()) {
         fprintf(stderr,"nomcanal: %s\n", item->text(m_colNomCoste).ascii());
         return item->text(m_colNomCoste);
   } else {
         return "";
   }// end if
}// end nomcanal


void selectcanalview::boton_todo() {
   QListViewItemIterator* m_iterador;
   m_iterador = new QListViewItemIterator (m_listCanales);
   QCheckListItem *item;
   while (m_iterador->current()) {
      item = (QCheckListItem *) m_iterador->current();
      item->setOn(TRUE);
     (*m_iterador)++;
   }// end while
}// end boton_todo

void selectcanalview::boton_nada() {
   QListViewItemIterator* m_iterador;
   m_iterador = new QListViewItemIterator (m_listCanales);
   QCheckListItem *item;
   while (m_iterador->current()) {
      item = (QCheckListItem *) m_iterador->current();
      item->setOn(FALSE);
     (*m_iterador)++;
   }// end while
}// end boton_todo


void selectcanalview::boton_invertir() {
   QListViewItemIterator* m_iterador;
   m_iterador = new QListViewItemIterator (m_listCanales);
   QCheckListItem *item;
   while (m_iterador->current()) {
      item = (QCheckListItem *) m_iterador->current();
      if (item->isOn()) 
         item->setOn(FALSE);
      else 
         item->setOn(TRUE);
     (*m_iterador)++;
   }//end while
}// end boton_invertir
