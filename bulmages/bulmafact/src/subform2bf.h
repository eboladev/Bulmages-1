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

#ifndef SUBFORM2BF_H
#define SUBFORM2BF_H

#include <QEvent>

#include "company.h"
#include "qtable2.h"
#include "subform.h"
#include "subform3.h"

#include "qdoublespinbox2.h"

class SubForm2Bf;


/// Clase SubForm2BfDelegate
/** Se encarga del control de los Widgets de Edicion del sistema.*/
class QSubForm2BfDelegate : public QItemDelegate, public PEmpresaBase {
private:
    SubForm2Bf *m_subform;

public:
    QSubForm2BfDelegate(QObject *);
    ~QSubForm2BfDelegate();
    void setEditorData(QWidget *, const QModelIndex &index) const;
    void setModelData(QWidget *editor,  QAbstractItemModel *model, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual bool eventFilter(QObject *obj, QEvent *event);
};


/// Clase SubForm2Bf
/** Es la primera abstraccion de los subformularios que incluye todas las caracteristicas comunes de la facturacion
*/
class SubForm2Bf : public SubForm3 {
    Q_OBJECT

private:
    bool m_delete;
    QSubForm2BfDelegate *m_delegate;
    QString mdb_idcliente;
    QString mdb_idproveedor;

public:
    void setIdCliente(QString id);
    void setIdProveedor(QString id);
    SubForm2Bf(QWidget *parent = 0);
    virtual ~SubForm2Bf();
    void setDelete(bool);
    virtual void cargar(QString query);
    virtual void setEmpresaBase(EmpresaBase *);


public slots:
//    virtual void contextMenuEvent(QContextMenuEvent *);
    virtual void on_mui_list_editFinished(int row, int col, int key);
    virtual void on_mui_list_pressedSlash(int row, int col);
    virtual void on_mui_list_pressedAsterisk(int row, int col);
    virtual void on_mui_list_pressedMinus(int row, int col);
};

#endif

