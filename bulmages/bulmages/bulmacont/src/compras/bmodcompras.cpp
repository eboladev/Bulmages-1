/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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
#include "bmodcompras.h"


BModCompras::BModCompras(BSelector * ref, QWidget * parent, const char * name, WFlags f)
 : UIcompras(parent,name,f)
{
ctllog->add(LOG_SEG | LOG_TRA, 2,"CmpCtt015","El usuario entra en el Módulo de Compras." );
PunteroAlSelector=ref;
QVBox* vb = new QVBox( this );
vb->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
zona0 = new QWorkspace (vb);
zona0->setScrollBarsEnabled( TRUE );
setCentralWidget( vb );
}


BModCompras::~BModCompras()
{
ctllog->add(LOG_SEG | LOG_TRA, 2,"CmpDtt016","El usuario sale   del  Módulo de Compras." );
PunteroAlSelector->ModuloCompras=NULL;
PunteroAlSelector->showNormal();
}

void BModCompras::mostrar_selector()
{
//Muestra el selector si está oculto
PunteroAlSelector->hide();
PunteroAlSelector->showNormal();
}

