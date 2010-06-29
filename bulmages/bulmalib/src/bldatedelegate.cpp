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

#include "bldatedelegate.h"

//! Constructor
/*!
   \param comp
   \param padre
*/
BlDateDelegate::BlDateDelegate(BlMainCompany *comp, QObject *parent)
   : QItemDelegate(parent)
   , m_company(comp)
{
   _depura(QString(metaObject()->className()) + "::BlDateDelegate", 0);

   _depura(QString(metaObject()->className()).prepend("END ") + "::BlDateDelegate", 0);
}


//! Crear la lista desplegable que servirá para editar el campo
/*!
   \param padre
   \param vis Parámetros de visualización para el elemento en la vista
   \param index Índice en el modelo del dato a editar
*/
QWidget *BlDateDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &vis, const QModelIndex &index) const
{
   _depura(QString(metaObject()->className()) + "::createEditor", 0);

   BlDateSearch *ds = new BlDateSearch(parent);
   ds->setMainCompany(m_company);

   _depura(QString(metaObject()->className()).prepend("END ") + "::createEditor", 0);

   return ds;
}


//! Poner la lista desplegable en la posición indicada por el valor del campo a editar
/*!
   \param editor
   \param index Índice en el modelo del dato a editar
*/
void BlDateDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   _depura(QString(metaObject()->className()) + "::setEditorData", 0);

   BlDateSearch *ds = (BlDateSearch *)editor;
   QString fecha = index.model()->data(index).toString();
   ds->setfecha(fecha);

   _depura(QString(metaObject()->className()).prepend("END ") + "::setEditorData", 0);
}


//! Guarda los cambios realizados en el widget sobre los datos asociados en la columna
/*!
   \param editor
   \param model
   \param index Índice en el modelo del dato a editar
*/
void BlDateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
   _depura(QString(metaObject()->className()) + "::setModelData", 0);

   QString fecha = ((BlDateSearch *)editor)->text();
   model->setData(index, fecha);

   _depura(QString(metaObject()->className()).prepend("END ") + "::setModelData", 0);
}


//! Establece el tama&ntilde;o y posición del widget editor
/*!
   \param editor
   \param vis Parámetros de visualización para el elemento en la vista
   \param index Índice en el modelo del dato a editar
*/
void BlDateDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &vis, const QModelIndex &index) const
{
   _depura(QString(metaObject()->className()) + "::updateEditorGeometry", 0);

   editor->setGeometry(vis.rect);

   _depura(QString(metaObject()->className()).prepend("END ") + "::updateEditorGeometry", 0);
}
