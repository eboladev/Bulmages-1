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
#include "bmodcontabilidad.h"


BModContabilidad::BModContabilidad(BSelector * ref, QWidget * parent, const char * name, WFlags f) 
: Bulmages01(parent,name,f,&ref->NombreBaseDatos,&ref->NombreUsuario,&ref->PasswordUsuario) 
{
  PunteroAlSelector=ref;
}


BModContabilidad::~BModContabilidad()
{
  PunteroAlSelector->ModuloContabilidad=NULL;
  PunteroAlSelector->showNormal();
}


void BModContabilidad::mostrar_selector()
{
//Muestra el selector si está oculto
PunteroAlSelector->hide(); 
PunteroAlSelector->showNormal();
}
 
void BModContabilidad::closeEvent(QCloseEvent * e) {
  int exit=QMessageBox::information(this, tr("Cerrar..."),
                                    tr("Realmente deseas Salir?"),
                                    QMessageBox::Ok, QMessageBox::Cancel);
  if (exit==1) e->accept();
  else e->ignore();
}
