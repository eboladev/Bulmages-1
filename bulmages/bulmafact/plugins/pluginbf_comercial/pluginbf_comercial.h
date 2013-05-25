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

#ifndef PLUGINBF_COMERCIAL_H
#define PLUGINBF_COMERCIAL_H

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtCore/QObject>

#include "blpostgresqlclient.h"
#include "clienteview.h"
#include "bfbulmafact.h"
#include "blwidget.h"

#include "comercial.h"

#include "pdefs_pluginbf_comercial.h"


extern "C" PLUGINBF_COMERCIAL_EXPORT int entryPoint ( BfBulmaFact * );
extern "C" PLUGINBF_COMERCIAL_EXPORT int ClienteView_ClienteView_Post ( ClienteView * );
extern "C" PLUGINBF_COMERCIAL_EXPORT int ClienteView_Des_ClienteView ( ClienteView * );
extern "C" PLUGINBF_COMERCIAL_EXPORT int BfCompany_createMainWindows_Post ( BfCompany * );

/*
extern "C" PLUGINBF_COMERCIAL_EXPORT int ClienteView_saveClient(ClienteView *);
extern "C" PLUGINBF_COMERCIAL_EXPORT int Cliente_pintaCliente(Cliente *);
*/


#endif

