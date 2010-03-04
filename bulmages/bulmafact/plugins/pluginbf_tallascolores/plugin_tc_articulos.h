/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include "bfbulmafact.h"
#include "articuloview.h"
#include "blsearchwidget.h"
#include "bfproveedoralbaransubform.h"
#include "bfclientealbaransubform.h"
#include "listlinfacturaview.h"
#include "listlinfacturapview.h"
#include "pdefs_pluginbf_tallascolores.h"


extern "C" PLUGINBF_TALLASCOLORES_EXPORT int entryPoint ( BfBulmaFact * );

extern "C" PLUGINBF_TALLASCOLORES_EXPORT int ArticuloView_ArticuloView ( ArticuloView * );
extern "C" PLUGINBF_TALLASCOLORES_EXPORT int ArticuloView_cargar ( ArticuloView * );
extern "C" PLUGINBF_TALLASCOLORES_EXPORT int ArticuloView_guardar_post ( ArticuloView * );

extern "C" PLUGINBF_TALLASCOLORES_EXPORT int Busqueda_on_m_inputBusqueda_textChanged ( BlSearchWidget * );

extern "C" PLUGINBF_TALLASCOLORES_EXPORT int BfProveedorAlbaranSubForm_BfProveedorAlbaranSubForm ( BfProveedorAlbaranSubForm * );
extern "C" PLUGINBF_TALLASCOLORES_EXPORT int BfClienteAlbaranSubForm_BfClienteAlbaranSubForm ( BfClienteAlbaranSubForm * );
extern "C" PLUGINBF_TALLASCOLORES_EXPORT int ListLinFacturaView_ListLinFacturaView ( ListLinFacturaView * );
extern "C" PLUGINBF_TALLASCOLORES_EXPORT int ListLinFacturaProveedorView_ListLinFacturaProveedorView ( ListLinFacturaProveedorView * );

extern "C" PLUGINBF_TALLASCOLORES_EXPORT int BfClienteAlbaranSubForm_cargar ( BfClienteAlbaranSubForm * );

extern "C" PLUGINBF_TALLASCOLORES_EXPORT int ListLinFacturaView_cargar ( ListLinFacturaView * );

