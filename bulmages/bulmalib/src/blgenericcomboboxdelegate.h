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

#ifndef BLGENERICCOMBOBOXDELEGATE_H
#define BLGENERICCOMBOBOXDELEGATE_H

#include "blcombobox.h"
#include "tsystem.h"


/** Lista desplegable en campo de listado.
Modo de empleo:
Agregar esta l&iacute;nea en el m&eacute;todo cargar del listado que hereda de BlSubForm:
mui_list->setItemDelegateForColumn ( <n&uacute;mero de columna>, new BlGenericComboBoxDelegate ( mainCompany(), this, <tabla>, <campo id>, <campo visible> ) );
*/
class BlGenericComboBoxDelegate: public QItemDelegate
{
   public:
	BlGenericComboBoxDelegate ( BlMainCompany *comp, QObject *parent, QString table, QString id_field, QString text_field, bool allow_null = true, QString cond = "" ) ;
	void paint ( QPainter *pintor, const QStyleOptionViewItem &vis, const QModelIndex &index ) const;
	QWidget *createEditor ( QWidget *parent, const QStyleOptionViewItem &vis, const QModelIndex &index ) const;
	void setEditorData ( QWidget *editor, const QModelIndex &index ) const;
	void setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const;
	void updateEditorGeometry ( QWidget *editor, const QStyleOptionViewItem &vis, const QModelIndex &index ) const;

   private:
	BlMainCompany *m_company;
	QString m_table;
	QString m_id_field;
	QString m_text_field;
	bool m_allow_null;
	QString m_cond;
};

#endif
