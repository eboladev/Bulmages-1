/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#ifdef Q_WS_WIN
# define MY_EXPORT __declspec(dllexport)
#else
# define MY_EXPORT
#endif

#include "bulmacont.h"
#include "postgresiface2.h"
#include "empresa.h"
#include "asiento1.h"
#include "listlinasiento1view.h"
#include "asiento1view.h"

extern "C" MY_EXPORT int entryPoint ( Bulmacont * );
extern "C" MY_EXPORT int Asiento1_guardaAsiento1_post ( Asiento1 * );
extern "C" MY_EXPORT int empresa_cobPag ( Empresa * );
extern "C" MY_EXPORT int empresa_registroiva ( Empresa * );
extern "C" MY_EXPORT int ListLinAsiento1View_boton_iva ( ListLinAsiento1View * );
extern "C" MY_EXPORT int Asiento1View_Asiento1View ( Asiento1View * );
