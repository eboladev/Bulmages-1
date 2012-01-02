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

#ifndef PLUGINBF_TIPOTRABAJO_H
#define PLUGINBF_TIPOTRABAJO_H

#include <QObject>


#include "bfbulmafact.h"
#include "trabajadorview.h"
#include "almacenview.h"
#include "blform.h"
#include "bfsubform.h"
#include "pdefs_pluginbf_tipotrabajo.h"
#include "blaction.h"

extern "C" PLUGINBF_TIPOTRABAJO_EXPORT int entryPoint ( BfBulmaFact * );
extern "C" PLUGINBF_TIPOTRABAJO_EXPORT int TrabajadorView_TrabajadorView_Post ( TrabajadorView * );
extern "C" PLUGINBF_TIPOTRABAJO_EXPORT int TrabajadorView_on_mui_lista_currentItemChanged_Post ( TrabajadorView * );
extern "C" PLUGINBF_TIPOTRABAJO_EXPORT int TrabajadorView_on_mui_guardar_clicked ( TrabajadorView * );
extern "C" PLUGINBF_TIPOTRABAJO_EXPORT int AlmacenView_AlmacenView ( AlmacenView * );
extern "C" PLUGINBF_TIPOTRABAJO_EXPORT int BlForm_load ( BlForm * );
extern "C" PLUGINBF_TIPOTRABAJO_EXPORT int BlForm_guardar_Post ( BlForm * );
extern "C" PLUGINBF_TIPOTRABAJO_EXPORT int BlAction_actionTriggered(BlAction *);

/// Clase BfSubFormDelegate
/** Se encarga del control de los Widgets de Edicion del sistema.*/
class QSubForm3BfDelegate : public BfSubFormDelegate
{
public:
    QSubForm3BfDelegate ( QObject * );
    virtual ~QSubForm3BfDelegate();
    virtual void setEditorData ( QWidget *, const QModelIndex &index ) const;
    virtual void setModelData ( QWidget *editor,  QAbstractItemModel *model, const QModelIndex &index ) const;
    virtual QWidget *createEditor ( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
};

#endif

