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
 : selectcanaldlg(parent, name)
{
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
// Rellenamnos la listbox que va a sustituir al combobox correspondiente.
// Para que en los listados puedan salir más cosas de las que se dicen.   
   fprintf(stderr,"Ahora nos toca rellenar las listas.\n");
   
//    QListViewItem * it;
//    QListViewItem *Lista[10000];
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
//        padre = atoi( cursoraux1->valor("padre").ascii());
        idcanal = atoi( cursoraux1->valor("idcanal").ascii());
//        it =new QListViewItem(m_listCanales);
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
