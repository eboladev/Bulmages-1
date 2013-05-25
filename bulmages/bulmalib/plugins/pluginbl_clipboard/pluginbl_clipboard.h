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

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtCore/QObject>

#include "blfunctions.h"
#include "blapplication.h"
#include "blsubform.h"
#include "blpostgresqlclient.h"
#include "blwidget.h"
#include "blmaincompanypointer.h"
#include "blmaincompany.h"
#include "pdefs_pluginbl_clipboard.h"
#include "bltoolbutton.h"

extern "C" PLUGINBL_CLIPBOARD_EXPORT int entryPoint ( QApplication * );
extern "C" PLUGINBL_CLIPBOARD_EXPORT int BlSubForm_BlSubForm_Post ( BlSubForm * );
extern "C" PLUGINBL_CLIPBOARD_EXPORT int BlSubForm_BlSubForm_Post ( BlSubForm * );
extern "C" PLUGINBL_CLIPBOARD_EXPORT int BlSubForm_preparaMenu ( BlSubForm * );
extern "C" PLUGINBL_CLIPBOARD_EXPORT int BlToolButton_released(BlToolButton *);


class BlMainCompany;


class PluginBl_Clipboard : public QObject, public BlMainCompanyPointer
{
    Q_OBJECT

public:
    PluginBl_Clipboard ( BlSubForm * );
    ~PluginBl_Clipboard();
    void pegaSXC();
    void pegaODS();
    void pegaActualizaODS();

public slots:
    virtual void s_pintaMenu ( QMenu * );
    virtual void s_trataMenu ( QAction * );
};
