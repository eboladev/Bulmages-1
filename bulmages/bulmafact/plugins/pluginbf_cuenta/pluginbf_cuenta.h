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

#ifndef PLUGINBF_CUENTA_H
#define PLUGINBF_CUENTA_H

#include "bfbulmafact.h"
#include "blmaincompanypointer.h"
#include "blmaincompany.h"
#include "blsubform.h"
#include "bfsubform.h"
#include "blsearchwidget.h"
#include "pdefs_pluginbf_cuenta.h"
#include "blaction.h"

#include "familiasview.h"
#include "clienteview.h"
#include "provedit.h"
#include "fpagoview.h"

extern "C" PLUGINBF_CUENTA_EXPORT int entryPoint ( BlMainWindow * );
extern "C" PLUGINBF_CUENTA_EXPORT int BlSubForm_BlSubForm_Post ( BlSubForm * );
extern "C" PLUGINBF_CUENTA_EXPORT int Busqueda_on_mui_buscar_clicked(BlSearchWidget *);
extern "C" PLUGINBF_CUENTA_EXPORT int BlSubFormDelegate_createEditor ( BlSubFormDelegate * );
extern "C" PLUGINBF_CUENTA_EXPORT int BlSubFormDelegate_setModelData ( BlSubFormDelegate * );
extern "C" PLUGINBF_CUENTA_EXPORT int BlSubFormDelegate_setEditorData ( BlSubFormDelegate * );
extern "C" PLUGINBF_CUENTA_EXPORT int BlSubForm_editFinished ( BlSubForm * );
extern "C" PLUGINBF_CUENTA_EXPORT int BlDbCompleterComboBox_textChanged (BlDbCompleterComboBox *);
extern "C" PLUGINBF_CUENTA_EXPORT int BfSubForm_pressedAsterisk ( BfSubForm *);
extern "C" PLUGINBF_CUENTA_EXPORT int Busqueda_on_m_inputBusqueda_textChanged (BlSearchWidget *);
extern "C" PLUGINBF_CUENTA_EXPORT int BlAction_actionTriggered ( BlAction *);
extern "C" PLUGINBF_CUENTA_EXPORT int BlSubForm_preparaMenu ( BlSubForm * );
extern "C" PLUGINBF_CUENTA_EXPORT int ClienteView_ClienteView ( ClienteView * );
extern "C" PLUGINBF_CUENTA_EXPORT int ProveedorView_ProveedorView ( ProveedorView * );
extern "C" PLUGINBF_CUENTA_EXPORT int ClienteView_Guardar_Pre(ClienteView *);
extern "C" PLUGINBF_CUENTA_EXPORT int ProveedorView_Guardar_Pre(ProveedorView *);
extern "C" PLUGINBF_CUENTA_EXPORT int FamiliasView_FamiliasView ( FamiliasView * );
extern "C" PLUGINBF_CUENTA_EXPORT int FamiliasView_Guardar_Pre ( FamiliasView * );
extern "C" PLUGINBF_CUENTA_EXPORT int FamiliasView_currentItemChanged_Post ( FamiliasView * );

extern "C" PLUGINBF_CUENTA_EXPORT int FPagoView_FPagoView ( FPagoView * );
extern "C" PLUGINBF_CUENTA_EXPORT int FPagoView_Guardar_Pre ( FPagoView * );
extern "C" PLUGINBF_CUENTA_EXPORT int FPagoView_currentItemChanged_Post ( FPagoView * );

extern "C" PLUGINBF_CUENTA_EXPORT int Plugin_open(BfCompany * );

class SubForm_Cuenta : public QObject
{
    Q_OBJECT

public:
    SubForm_Cuenta ( BlSubForm * );
    ~SubForm_Cuenta();

public slots:
    void editarCuenta( QString);
    void nuevoCuenta();
    void seleccionarCuenta(BlSubForm *);
    void seleccionarCuenta();

    virtual void s_pintaMenu ( QMenu * );
    virtual void s_trataMenu ( QAction * );
};



#endif

