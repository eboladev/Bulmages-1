/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#ifndef PLUGINBF_ALIAS_H
#define PLUGINBF_ALIAS_H

#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>

#include "bfcompany.h"
#include "bfbulmafact.h"
#include "articuloview.h"
#include "blsearchwidget.h"
#include "bfbuscararticulo.h"
#include "bfsubform.h"
#include "pdefs_pluginbf_alias.h"


// cuando la usuaria escribe en la celda de código de artículo y este plugin
// está instalado, buscaremos además de por código de artículo por alias. 
// Como que buscar a cada tecla puede ser lento, comprobamos que la longitud
// y la posición allfabética de la cadena entrada esté entre los máximos y 
// mínimos de la tabla de alias antes de consultar la base de datos. Para esta
// optimización necesitamos unas estadísticas de la tabla de alias que mantenemos
// en memoria en variables globales hasta que se modifica/añade un alias desde la 
// misma aplicación. Pero como puede que se toque la base de datos des de otra
// aplicación, terminal o usuario, no nos fiamos de que las estadísticas en memoría estén
// al día y las refrescamos si tienen antiguedad superior a
// MILISEG_REFRESCO_ESTAD_ALIAS. Para una instalación
// monopuesto querremos un valor 0 (no refrescarlas nunca). Para una instalación
// multipuesto donde no sea frecuente que se añadan artículos mientras se compran/venden 
// querremos un valor muy alto y para una en la que a menudo haya 
// gente dando de alta artículos mientras otros hacen albaranes, etc. pondremos
// un valor bajo. No se si debería ser una constante o un valor más configurable...
#define MILISEG_REFRESCO_ESTAD_ALIAS 60000 // 1 min


extern "C" PLUGINBF_ALIAS_EXPORT int entryPoint ( BfBulmaFact * );
extern "C" PLUGINBF_ALIAS_EXPORT int ArticuloView_ArticuloView ( ArticuloView * );
extern "C" PLUGINBF_ALIAS_EXPORT int ArticuloView_load ( ArticuloView * );
extern "C" PLUGINBF_ALIAS_EXPORT int ArticuloView_guardar_post ( ArticuloView * );
extern "C" PLUGINBF_ALIAS_EXPORT int Busqueda_on_m_inputBusqueda_textChanged ( BlSearchWidget * );
extern "C" PLUGINBF_ALIAS_EXPORT int BlSubForm_editFinished(BlSubForm *);
extern "C" PLUGINBF_ALIAS_EXPORT int BlDbCompleterComboBox_textChanged (BlDbCompleterComboBox *);


#endif

