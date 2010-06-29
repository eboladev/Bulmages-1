/*****************************************************************************
*  This file is part of Cdigés.
*
*  Copyright (C) 2010 by Creación y Diseño Ibense S.L.
*
*  Cdigés is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Cdigés is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with Cdigés. If not, see http://www.gnu.org/copyleft/gpl.html.
*****************************************************************************/

#include "blgenericcomboboxdelegate.h"

//! Constructor
/*!
   \param comp
   \param padre
   \param table
   \param id_field
   \param text_field
   \param cond Condición opcional para un WHERE de consulta
*/
BlGenericComboBoxDelegate::BlGenericComboBoxDelegate(BlMainCompany *comp, QObject *parent, QString table, QString id_field, QString text_field, QString cond)
   : QItemDelegate(parent)
   , m_company(comp)
   , m_table(table)
   , m_id_field(id_field)
   , m_text_field(text_field)
   , m_cond(cond)
{
   _depura(QString(metaObject()->className()) + "::BlGenericComboBoxDelegate", 0);

   _depura(QString(metaObject()->className()).prepend("END ") + "::BlGenericComboBoxDelegate", 0);
}


//! Personalizar el aspecto del campo
/*!
   \param painter
   \param vis Parámetros de visualización para el elemento en la vista
   \param index Índice en el modelo del dato a editar
*/
void BlGenericComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &vis, const QModelIndex &index) const
{
   _depura(QString(metaObject()->className()) + "::paint", 0);

   // La posición no coincide: no tiene en cuenta las cabeceras
   QPoint pos = vis.rect.topLeft();
   pos.setX(pos.x() + 5);
   pos.setY(pos.y() + 17);

   QString id = index.model()->data(index).toString();

   if (!id.isEmpty())
   {
	QString consulta = QString("SELECT %1 FROM %2 WHERE %3 = %4")
				 .arg(m_text_field)
				 .arg(m_table)
				 .arg(m_id_field)
				 .arg(index.model()->data(index).toString());

	if (!m_cond.isEmpty())
	{
	   consulta += QString(" AND %1")
			   .arg(m_cond);
	}

	painter->drawText(pos, m_company->loadQuery(consulta)->valor(m_text_field));
   }
   else
   {
	QItemDelegate::paint(painter, vis, index);
   }

   _depura(QString(metaObject()->className()).prepend("END ") + "::paint", 0);
}


//! Crear la lista desplegable que servirá para editar el campo
/*!
   \param padre
   \param vis Parámetros de visualización para el elemento en la vista
   \param index Índice en el modelo del dato a editar
*/
QWidget *BlGenericComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &vis, const QModelIndex &index) const
{
   _depura(QString(metaObject()->className()) + "::createEditor", 0);

   QString consulta = QString("SELECT %1, %2 FROM %3")
			    .arg(m_id_field)
			    .arg(m_text_field)
			    .arg(m_table);

   if (!m_cond.isEmpty())
   {
	consulta += QString(" WHERE %1")
			.arg(m_cond);
   }

   consulta += QString(" ORDER BY %2")
		   .arg(m_text_field);

   BlComboBox *cbox = new BlComboBox(parent);
   cbox->setMainCompany(m_company);
   cbox->setQuery(consulta);
   cbox->setFieldId(m_id_field);
   cbox->m_valores[m_text_field] = "";
   cbox->setAllowNull(true);

   _depura(QString(metaObject()->className()).prepend("END ") + "::createEditor", 0);

   return cbox;
}


//! Poner la lista desplegable en la posición indicada por el valor del campo a editar
/*!
   \param editor
   \param index Índice en el modelo del dato a editar
*/
void BlGenericComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   _depura(QString(metaObject()->className()) + "::setEditorData", 0);

   ((BlComboBox *)editor)->setId(index.model()->data(index).toString());

   _depura(QString(metaObject()->className()).prepend("END ") + "::setEditorData", 0);
}


//! Guarda los cambios realizados en el widget sobre los datos asociados en la columna
/*!
   \param editor
   \param model
   \param index Índice en el modelo del dato a editar
*/
void BlGenericComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
   _depura(QString(metaObject()->className()) + "::setModelData", 0);

   model->setData(index, ((BlComboBox *)editor)->id());

   _depura(QString(metaObject()->className()).prepend("END ") + "::setModelData", 0);
}


//! Establece el tama&ntilde;o y posición del widget editor
/*!
   \param editor
   \param vis Parámetros de visualización para el elemento en la vista
   \param index Índice en el modelo del dato a editar
*/
void BlGenericComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &vis, const QModelIndex &index) const
{
   _depura(QString(metaObject()->className()) + "::updateEditorGeometry", 0);

   editor->setGeometry(vis.rect);

   _depura(QString(metaObject()->className()).prepend("END ") + "::updateEditorGeometry", 0);
}
