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
#include "bmodventas.h"


BModVentas::BModVentas(BSelector * ref, QWidget * parent, const char * name, WFlags f)
 : UIventas(parent,name,f)
{
PunteroAlSelector=ref;
QVBox* vb = new QVBox( this );
vb->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
zona0 = new QWorkspace (vb);
zona0->setScrollBarsEnabled( TRUE );
setCentralWidget( vb );  
}


BModVentas::~BModVentas()
{
PunteroAlSelector->ModuloVentas=NULL;
PunteroAlSelector->showNormal();
}

//Abro la ventana de los albaranes de Venta.
void BModVentas::albaranes()
{
BAlbaVenta * albaran = new BAlbaVenta(zona0,"albaran");
albaran->show();
}

//Abro la ventana de los pedidos de Venta.
void BModVentas::pedidos()
{
BPediVenta * pedido = new BPediVenta(zona0,"pedido");
pedido->show();
}


void BModVentas::mostrar_selector()
{
//Muestra el selector si está oculto
PunteroAlSelector->hide(); 
PunteroAlSelector->show();
}

