/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras                                    *
 *   tborras@conetxia.com                                                  *
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
#ifndef SELECTCANALVIEW_H
#define SELECTCANALVIEW_H

#include "selectcanaldlg.h"
#include "postgresiface2.h"
#include <qlistview.h>
#include <qdialog.h>

class empresa;
/**@author Tomeu Borras*/
class selectcanalview : public QDialof, public Ui_selectcanaldlg {
Q_OBJECT
public:
    selectcanalview(empresa *emp, QWidget *parent = 0, const char *name = 0);
    ~selectcanalview();
private:
   empresa *empresaactual;
   int m_colNomCoste;
   int m_colDescCoste;
   int m_colIdCoste;
   int m_colStatusCoste;  
   int m_colCheck;
   postgresiface2* conexionbase;
   int numdigitos;
   QListViewItemIterator* m_iterador;
public:
   void cargacanales();
   int firstcanal();
   int nextcanal();
   QString cadcanal();
   QString nomcanal();
   
public slots:
   void boton_todo();
   void boton_nada();
   void boton_invertir();
};

#endif
