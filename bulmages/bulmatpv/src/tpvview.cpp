/***************************************************************************
 *   Copyright (C) 2004 by Bulmages Developer Team                         *
 *   info@bulmages.org                                                     *
 *   Created by Fco. Javier M. C. <fcojavmc@todo-redes.com>                *
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
#include "tpvview.h"
#include <qlcdnumber.h>

tpvview::tpvview(QWidget *parent, const char *name)
 : QDialog(parent, name)
{
  setupUi(this);
  display->display(0.00);
}


tpvview::~tpvview()
{
}


void tpvview::pseudo_teclado(int valor)
{   
   display_2->display((display_2->value()*10) + valor);
}

void tpvview::pseudo_teclado_si()
{

}

void tpvview::pseudo_teclado_atras()
{

}

void tpvview::pseudo_teclado_borrar()
{
   display_2->display(0);
}

void tpvview::pseudo_teclado_00()
{
   pseudo_teclado(0);
   pseudo_teclado(0);
}

void tpvview::pseudo_teclado_0()
{
   pseudo_teclado(0);
}

void tpvview::pseudo_teclado_1()
{
   pseudo_teclado(1);
}

void tpvview::pseudo_teclado_2()
{
   pseudo_teclado(2);
}

void tpvview::pseudo_teclado_3()
{
   pseudo_teclado(3);
}

void tpvview::pseudo_teclado_4()
{
   pseudo_teclado(4);
}

void tpvview::pseudo_teclado_5()
{
   pseudo_teclado(5);
}

void tpvview::pseudo_teclado_6()
{
   pseudo_teclado(6);
}

void tpvview::pseudo_teclado_7()
{
   pseudo_teclado(7);
}

void tpvview::pseudo_teclado_8()
{
   pseudo_teclado(8);
}

void tpvview::pseudo_teclado_9()
{
   pseudo_teclado(9);
}
