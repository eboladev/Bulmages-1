/***************************************************************************
 *   Copyright (C) 2010 by Aron Galdon                                     *
 *   auryn@wanadoo.es                                                      *
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

#ifndef BLDATEDELEGATE_H
#define BLDATEDELEGATE_H

#include "bldatesearch.h"
#include <QStyledItemDelegate>


/** Calendario flotante para seleccionar fechas en un campo de listado.
Modo de empleo:
Agregar esta l&iacute;nea en el m&eacute;todo "cargar" del listado que hereda de BlSubForm,
justo despu&eucte;s de BfSubForm::load ( consulta ):
mui_list->setItemDelegateForColumn ( <columna>, new BlDateDelegate ( mainCompany(), this ) );
*/
class BL_EXPORT BlDateDelegate: public QStyledItemDelegate
{
   public:
	BlDateDelegate ( BlMainCompany *company, QObject *parent, bool allowNull = true ) ;
	QWidget *createEditor ( QWidget *parent, const QStyleOptionViewItem &vis, const QModelIndex &index )  const;
	void setEditorData ( QWidget *editor, const QModelIndex &index )  const;
	void setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index )  const;
	void updateEditorGeometry ( QWidget *editor, const QStyleOptionViewItem &vis, const QModelIndex &index )  const;
	void setAllowNull ( bool allowNull );

   private:
	BlMainCompany *m_company;
	bool m_allowNull;
};

#endif
