/***************************************************************************
 *   Copyright (C) 2004 by Bulmages Developer Team                         *
 *   info@bulmages.org                                                     *
 *   Modified by Fco. Javier M. C. <fcojavmc@todo-redes.com>               *
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
#ifndef TPVVIEW_H
#define TPVVIEW_H

#include <tpvdlg.h>

/**
@author Bulmages Developer Team
*/
class tpvview : public tpvdlg
{
Q_OBJECT
public:
    tpvview(QWidget *parent = 0, const char *name = 0);

    ~tpvview();

public slots:
   virtual void pseudo_teclado_00();
   virtual void pseudo_teclado_0();
   virtual void pseudo_teclado_1();
   virtual void pseudo_teclado_2();
   virtual void pseudo_teclado_3();
   virtual void pseudo_teclado_4();
   virtual void pseudo_teclado_5();
   virtual void pseudo_teclado_6();
   virtual void pseudo_teclado_7();
   virtual void pseudo_teclado_8();
   virtual void pseudo_teclado_9();
   virtual void pseudo_teclado_atras();
   virtual void pseudo_teclado_si();
   virtual void pseudo_teclado_borrar();


private:
   void pseudo_teclado(int);       
};

#endif
