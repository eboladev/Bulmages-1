/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borrás                                    *
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
#ifndef AMORTIZACIONESVIEW_H
#define AMORTIZACIONESVIEW_H

#include <amortizacionesdlg.h>

#include "postgresiface2.h"

/**@author Tomeu Borras */
class empresa;

class amortizacionesview : public amortizacionesdlg
{
Q_OBJECT
public:
	empresa *empresaactual;
   postgresiface2 *conexionbase;
   QString idamortizacion;		// Esta variable contiene el idamortizacion cuando se devuelve un parametro
   QString nomamortizacion;	// Esta variable contiene el nomamortizacion cuando se devuelve un parametro
   int modo;
public:
	amortizacionesview(empresa *,QWidget *, const char *, bool );
	~amortizacionesview();
   void inicializatabla();
   
public slots:
   virtual void dbtabla(int, int, int, const QPoint &);
   virtual void nuevo();
   virtual void borrar();

};

#endif
