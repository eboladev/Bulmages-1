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

#ifndef __SUBFORM2BC1_H
#define __SUBFORM2BC1_H

#include "empresa.h"
#include "qtable2.h"
#include "subform.h"
#include "subform3.h"

#include "busquedafecha2.h"


class SubForm2Bc;

/// Clase SubForm2BcDelegate
/** Se encarga del control de los Widgets de Edicion del sistema.*/
class QSubForm2BcDelegate : public QItemDelegate, public PEmpresaBase
{
private:
    /// Clase padre y acceso a base de datos.
    SubForm2Bc *m_subform;

public:
    QSubForm2BcDelegate ( QObject * );
    ~QSubForm2BcDelegate();
    void setEditorData ( QWidget *, const QModelIndex &index ) const;
    void setModelData ( QWidget *editor,  QAbstractItemModel *model, const QModelIndex &index ) const;
    QWidget *createEditor ( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
    virtual bool eventFilter ( QObject *obj, QEvent *event );
};


class SubForm2Bc : public SubForm3
{
    Q_OBJECT

public:
    QSubForm2BcDelegate *m_delegate;

public:
    Empresa *empresaBase();
    SubForm2Bc ( QWidget *parent = 0 );
    virtual ~SubForm2Bc();
    virtual void boton_asiento();
    virtual void boton_extracto1 ( int );
    virtual void boton_balance1 ( int );
    virtual void boton_diario1 ( int );
    virtual void creaMenu ( QMenu * );
    virtual void procesaMenu ( QAction * );
    virtual void setEmpresaBase ( EmpresaBase *c );
    virtual void on_mui_list_cellChanged ( int row, int col );

public slots:
    virtual void on_mui_list_pressedSlash ( int row, int col );
    virtual void on_mui_list_pressedAsterisk ( int row, int col );
};

#endif

