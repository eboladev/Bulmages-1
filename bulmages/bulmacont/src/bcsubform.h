/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org Asociación Iglues -- Contabilidad Linux         *
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

#ifndef BCSUBFORM_H
#define BCSUBFORM_H

#include "bccompany.h"
#include "bltablewidget.h"
#include "bldbsubform.h"
#include "blsubform.h"

#include "bldatelineedit.h"


class BcSubForm;

/// Clase BcSubFormDelegate
/** Se encarga del control de los Widgets de Edicion del sistema.*/
class BC_EXPORT BcSubFormDelegate : public BlSubFormDelegate
{

public:
    BcSubFormDelegate ( QObject * );
    ~BcSubFormDelegate();
    void setEditorData ( QWidget *, const QModelIndex &index ) const;
    void setModelData ( QWidget *editor,  QAbstractItemModel *model, const QModelIndex &index ) const;
    QWidget *createEditor ( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
};


class BC_EXPORT BcSubForm : public BlSubForm
{
    Q_OBJECT

public:
    BcSubFormDelegate *m_delegate;

public:
    BcCompany *mainCompany();
    BcSubForm ( QWidget *parent = 0 );
    virtual ~BcSubForm();
    virtual void createMenu ( QMenu * );
    virtual void execMenuAction ( QAction * );
    virtual void setMainCompany ( BlMainCompany *c );
    virtual void editFinished ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp );
    virtual void pressedAsterisk ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp );
    virtual void pressedPlus ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp );
    virtual void pressedSlash ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp );
};

#endif

