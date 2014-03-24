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

#ifndef PLUGINBF_PROVEEDOR_H
#define PLUGINBF_PROVEEDOR_H

#include "blpostgresqlclient.h"
#include "bfbulmafact.h"
#include "blwidget.h"
#include "bfbuscarreferencia.h"
#include "blsearchwidget.h"
#include "pdefs_pluginbf_proveedor.h"
#include "blaction.h"
#include "bfsubform.h"


extern "C" PLUGINBF_PROVEEDOR_EXPORT int entryPoint ( BfBulmaFact * );
extern "C" PLUGINBF_PROVEEDOR_EXPORT int BfCompany_createMainWindows_Post(BfCompany *);
extern "C" PLUGINBF_PROVEEDOR_EXPORT int Busqueda_on_mui_buscar_clicked(BlSearchWidget *);
extern "C" PLUGINBF_PROVEEDOR_EXPORT int SNewProveedorView (BfCompany *);
extern "C" PLUGINBF_PROVEEDOR_EXPORT int BlAction_actionTriggered(BlAction *);

extern "C" PLUGINBF_PROVEEDOR_EXPORT int BlSubForm_BlSubForm_Post ( BlSubForm * );
extern "C" PLUGINBF_PROVEEDOR_EXPORT int BlSubFormDelegate_createEditor(BlSubFormDelegate *);
extern "C" PLUGINBF_PROVEEDOR_EXPORT int BlSubFormDelegate_setModelData(BlSubFormDelegate *);
extern "C" PLUGINBF_PROVEEDOR_EXPORT int BlSubFormDelegate_setEditorData(BlSubFormDelegate *);
extern "C" PLUGINBF_PROVEEDOR_EXPORT int BlSubForm_editFinished(BlSubForm *);
extern "C" PLUGINBF_PROVEEDOR_EXPORT int BlDbCompleterComboBox_textChanged (BlDbCompleterComboBox *);
extern "C" PLUGINBF_PROVEEDOR_EXPORT int BlSubForm_preparaMenu ( BlSubForm *);
extern "C" PLUGINBF_PROVEEDOR_EXPORT int CorrectorWidget_corregir(BlWidget *);
extern "C" PLUGINBF_PROVEEDOR_EXPORT int Plugin_open(BfCompany *);

class SubForm_Proveedor : public QObject
{
    Q_OBJECT

public:
    SubForm_Proveedor ( BlSubForm * );
    ~SubForm_Proveedor();


public slots:
    virtual void s_pintaMenu ( QMenu * );
    virtual void s_trataMenu ( QAction * );
    void editarProveedor( QString);
    void nuevoProveedor();
    void seleccionarProveedor(BfSubForm *sub=NULL);
};

#endif

