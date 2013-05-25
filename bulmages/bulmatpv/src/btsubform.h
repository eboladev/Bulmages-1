/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#ifndef BTSUBFORM_H
#define BTSUBFORM_H

#include <QtCore/QEvent>

#include "blfunctions.h"
#include "btcompany.h"
#include "bltablewidget.h"
#include "bldbsubform.h"
#include "blsubform.h"
#include "bldoublespinbox.h"


class BtSubForm;


/// Clase BtSubFormDelegate
/** Se encarga del control de los 'Widgets' de edici&oacute;n del sistema.*/
class BT_EXPORT BtSubFormDelegate : public QStyledItemDelegate, public BlMainCompanyPointer
{
protected:
    BtSubForm *m_subform;

public:
    BtSubFormDelegate ( QObject * );
    virtual ~BtSubFormDelegate();
    virtual void setEditorData ( QWidget *, const QModelIndex &index ) const;
    virtual void setModelData ( QWidget *editor,  QAbstractItemModel *model, const QModelIndex &index ) const;
    virtual QWidget *createEditor ( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
    virtual bool eventFilter ( QObject *obj, QEvent *event );
    virtual int cerrarEditor ( QWidget *editor );
};


/// Clase BtSubForm
/** Es la primera abstraccion de los subformularios que incluye todas las caracter&iacute;sticas
    comunes de la facturaci&oacute;n.
*/
class BT_EXPORT BtSubForm : public BlSubForm
{
    Q_OBJECT

public:
    BtSubFormDelegate *m_delegate;

protected:
    QString mdb_idcliente;
    QString mdb_idproveedor;

public:
    BtSubForm ( QWidget *parent = 0 );
    virtual ~BtSubForm();
    virtual void load ( QString query );
    virtual void setMainCompany ( BlMainCompany * );
    virtual void on_mui_list_cellChanged ( int row, int col );
    virtual int cerrarEditor();
    void setIdCliente ( QString id );
    void setIdProveedor ( QString id );
    virtual void pressedAsterisk ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp );

public slots:
    virtual void on_mui_list_pressedSlash ( int row, int col );
    virtual void on_mui_list_pressedMinus ( int row, int col );
};

#endif

