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
#include "selectccosteview.h"
#include "empresa.h"


#include <qlistview.h>


selectccosteview::selectccosteview(empresa *emp,QWidget *parent, const char
*name) : selectccostedlg(parent, name) {
   fprintf(stderr,"Constructor de selectccosteview\n");
   empresaactual = emp;
   conexionbase = empresaactual->bdempresa();

   numdigitos = empresaactual->numdigitosempresa();
//   m_iterador = new QListViewItemIterator (m_listCostes, 0x00001000);
   m_iterador = new QListViewItemIterator (m_listCostes);

   m_colNomCoste = m_listCostes->addColumn("nom_coste",-1);
   m_colDescCoste = m_listCostes->addColumn("desc_coste",-1);
   m_colStatusCoste = m_listCostes->addColumn("Status",-1);
   m_colIdCoste = m_listCostes->addColumn("idc_coste",0);
   m_colCheck = m_listCostes->addColumn("Seleccion",-1);

   cargacostes();
   fprintf(stderr,"FIN de Constructor de selectccosteview\n");

}// end selectccsotedlg


selectccosteview::~selectccosteview() {
   delete m_iterador;
}// end selectccosteview


void selectccosteview::cargacostes() {
// Rellenamnos la listbox que va a sustituir al combobox correspondiente.
// Para que en los listados puedan salir más cosas de las que se dicen.   
   fprintf(stderr,"Ahora nos toca rellenar las listas.\n");
   
//    QListViewItem * it;
//    QListViewItem *Lista[10000];
   QCheckListItem *it;
   QCheckListItem *Lista[10000];
    int padre;
    int idc_coste=0;
    cursor2 *cursoraux1, *cursoraux2;

    // Cogemos los centros de coste principales y los ponemos donde toca.
    m_listCostes->clear();
    conexionbase->begin();
    cursoraux1 = conexionbase->cargacursor("SELECT * FROM c_coste WHERE padre ISNULL ORDER BY idc_coste","ceroscoste");
    conexionbase->commit();
    while (!cursoraux1->eof()) {
//        padre = atoi( cursoraux1->valor("padre").ascii());
        idc_coste = atoi( cursoraux1->valor("idc_coste").ascii());
//        it =new QListViewItem(m_listCostes);
        it =new QCheckListItem(m_listCostes,"hola pepsi",QCheckListItem::CheckBox);
        Lista[idc_coste]=it;
        it->setText(m_colIdCoste, cursoraux1->valor("idc_coste"));
        it->setText(m_colDescCoste,cursoraux1->valor("descripcion"));
        it->setText(m_colNomCoste, cursoraux1->valor("nombre"));
        it->setOpen(true);
        cursoraux1->siguienteregistro ();
    }// end while
    delete cursoraux1;
    // Una vez que hemos puesto los centros de coste padre, todo lo demás es una
    // Tarea de ir colocando centros de coste a sus respectivos
    // deja de ser recursivo y pasa a ser lineal.
    conexionbase->begin();
    cursoraux2= conexionbase->cargacursor("SELECT * FROM c_coste WHERE padre IS NOT NULL ORDER BY idc_coste","mescostes");
    conexionbase->commit();
    while (!cursoraux2->eof()) {
         padre = atoi(cursoraux2->valor("padre").ascii());
         idc_coste = atoi(cursoraux2->valor("idc_coste").ascii());
         fprintf(stderr,"Cuentas de subnivel:%d",padre);
//         it = new QListViewItem(Lista[padre]);
         it = new QCheckListItem(Lista[padre],"hola",QCheckListItem::CheckBox);
         Lista[idc_coste]=it;
         it->setText(m_colIdCoste,cursoraux2->valor("idc_coste"));
         it->setText(m_colDescCoste,cursoraux2->valor("descripcion"));
         it->setText(m_colNomCoste,cursoraux2->valor("nombre"));
         it->setOpen(true);
         cursoraux2->siguienteregistro();
    }// end while
    delete cursoraux2;
}// end cargacostes

// Esta función devuelve el primer centro de coste seleccionado de la vita.
// Devuelve el idc_coste. Si no hay ningun centro de coste seleccionado devuelve
// cero
int selectccosteview::firstccoste() {
   delete m_iterador;
   m_iterador = new QListViewItemIterator (m_listCostes);
   return nextccoste();
}// end firstccoste

// Esta función devuelve el siguiente centro de coste seleccionado de la vista.
int selectccosteview::nextccoste() {
   int idccoste=0;
   QCheckListItem *item;
   fprintf(stderr,"nextccoste\n");
   while (m_iterador->current() && idccoste==0) {
      item = (QCheckListItem *) m_iterador->current();
      if (item->isOn()) {
         idccoste = item->text(m_colIdCoste).toInt();
         fprintf(stderr,"primer centro de coste:%d\n",idccoste);
      }// end if
     (*m_iterador)++;
   }
  return idccoste;
}// end nextccoste
