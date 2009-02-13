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
#include "bldbsubform.h"
#include "blsubform.h"
#include "bldoublespinbox.h"


class SubForm2Bf;


/// Clase SubForm2BfDelegate
/** Se encarga del control de los 'Widgets' de edici&oacute;n del sistema.*/
class QSubForm2BfDelegate : public QItemDelegate, public PEmpresaBase
{
    Q_OBJECT

protected:
    SubForm2Bf *m_subform;

public:
    QSubForm2BfDelegate ( QObject * );
    virtual ~QSubForm2BfDelegate();
    virtual void setEditorData ( QWidget *, const QModelIndex &index ) const;
    virtual void setModelData ( QWidget *editor,  QAbstractItemModel *model, const QModelIndex &index ) const;
    virtual QWidget *createEditor ( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
    virtual bool eventFilter ( QObject *obj, QEvent *event );
    virtual int cerrarEditor ( QWidget *editor );

signals:
     void cant_changed(BlDbSubFormRecord *) const;
};


/// Clase SubForm2Bf
/** Es la primera abstraccion de los subformularios que incluye todas las caracter&iacute;sticas
    comunes de la facturaci&oacute;n.
*/
class SubForm2Bf : public BlSubForm
{
    Q_OBJECT

public:
    QSubForm2BfDelegate *m_delegate;
    BlDbSubFormRecord *m_registrolinea;
    SDBCampo  *m_campoactual;			/// Usada para pasar parametros a los plugins.

protected:
    QString mdb_idcliente;
    QString mdb_idproveedor;
    QString m_idArticulo;
    QString m_idTarifa;
    QString m_idAlmacen;

public:
    SubForm2Bf ( QWidget *parent = 0 );
    virtual ~SubForm2Bf();
    virtual void cargar ( QString query );
    virtual void setEmpresaBase ( EmpresaBase * );
    virtual void editFinished ( int row, int col, BlDbSubFormRecord *rec, SDBCampo *camp );
    virtual void pressedAsterisk ( int row, int col, BlDbSubFormRecord *rec, SDBCampo *camp );
    virtual void pressedMinus ( int row, int col, BlDbSubFormRecord *rec, SDBCampo *camp );
    virtual void pressedSlash ( int row, int col, BlDbSubFormRecord *rec, SDBCampo *camp );
    virtual int cerrarEditor();
    void setIdCliente ( QString id );
    void setIdProveedor ( QString id );
    void setIdAlmacen ( QString id );
    QString idcliente();
    QString idproveedor();
    QString idArticulo();
    QString idTarifa();
    QString idAlmacen();

public slots:
    virtual void calculaPVP(BlDbSubFormRecord *rec);
};

#endif

