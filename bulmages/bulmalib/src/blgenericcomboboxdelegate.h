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

#include "blsubform.h"
#include "blcombobox.h"


/** Lista desplegable en campo de listado.
Modo de empleo:
Agregar estas l&iacute;neas en el m&eacute;todo cargar del listado que hereda de BlSubForm,
justo despu&eucte;s de BfSubForm::cargar ( consulta ):
BlGenericComboBoxDelegate *cbd = new BlGenericComboBoxDelegate ( mainCompany(), this );
cbd->set_foreign_table ( <tabla ajena>, <nombre del campo identificador>, <valor a mostrar> );
cbd->set_foreign_field ( <nº de la columna>, <nombre del campo> );
cbd->setAllowNull ( <true | false> );
cbd->initialize ( <nombre del campo virtual para el desplegable> );
mui_list->setItemDelegateForColumn ( <columna>, cbd );
*/
class BlGenericComboBoxDelegate: public BlSubFormDelegate
{
   public:
	BlGenericComboBoxDelegate ( BlMainCompany *comp, QObject *parent );
	void set_foreign_table ( const QString &table, const QString &id_field, const QString &text_field );
	void set_foreign_field ( unsigned int fk_column, QString fk_field_name );
	void set_filter_id ( const QString &fi_field_name, const QString &fi_fk_field_name );
	void set_where_condition ( const QString &cond );
	void setAllowNull ( bool v );
	void initialize ( const QString &combo_field_name );
	QWidget *createEditor ( QWidget *parent, const QStyleOptionViewItem &vis, const QModelIndex &index ) const;
	void setEditorData ( QWidget *editor, const QModelIndex &index ) const;
	void setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const;
	void updateEditorGeometry ( QWidget *editor, const QStyleOptionViewItem &vis, const QModelIndex &index ) const;
	QString query ( int row = -1 ) const;
	QString query_only_one ( const QString &fk_id ) const;

   private:
	BlMainCompany *m_company;
	unsigned int m_fk_column;
	QString m_fk_field_name;
	QString m_fi_field_name;
	QString m_fi_fk_field_name;
	QString m_table;
	QString m_id_field;
	QString m_text_field;
	bool m_allowNull;
	QString m_cond;
};

#endif
