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

#ifndef PLUGINBC_CUENTA_H
#define PLUGINBC_CUENTA_H

#include "bcbulmacont.h"
#include "blmaincompanypointer.h"
#include "blmaincompany.h"
#include "blsubform.h"
#include "bcsubform.h"
#include "blsearchwidget.h"
#include "pdefs_pluginbc_cuenta.h"
#include "blaction.h"


extern "C" PLUGINBC_CUENTA_EXPORT int entryPoint ( BcBulmaCont * );
extern "C" PLUGINBC_CUENTA_EXPORT int BlSubForm_BlSubForm_Post ( BlSubForm * );
extern "C" PLUGINBC_CUENTA_EXPORT int Busqueda_on_mui_buscar_clicked(BlSearchWidget *);
extern "C" PLUGINBC_CUENTA_EXPORT int BlSubFormDelegate_createEditor ( BlSubFormDelegate * );
extern "C" PLUGINBC_CUENTA_EXPORT int BlSubFormDelegate_setModelData ( BlSubFormDelegate * );
extern "C" PLUGINBC_CUENTA_EXPORT int BlSubFormDelegate_setEditorData ( BlSubFormDelegate * );
extern "C" PLUGINBC_CUENTA_EXPORT int BlSubForm_editFinished ( BlSubForm * );
extern "C" PLUGINBC_CUENTA_EXPORT int BlDbCompleterComboBox_textChanged (BlDbCompleterComboBox *);
extern "C" PLUGINBC_CUENTA_EXPORT int BcSubForm_pressedAsterisk ( BcSubForm *, void **);
extern "C" PLUGINBC_CUENTA_EXPORT int Busqueda_on_m_inputBusqueda_textChanged (BlSearchWidget *);
extern "C" PLUGINBC_CUENTA_EXPORT int BlAction_actionTriggered ( BlAction *);
extern "C" PLUGINBC_CUENTA_EXPORT int BlSubForm_preparaMenu ( BlSubForm * );

  
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

    virtual void s_pintaMenu ( QMenu * );
    virtual void s_trataMenu ( QAction * );
};



#endif

