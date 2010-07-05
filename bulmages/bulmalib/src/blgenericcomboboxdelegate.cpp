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

#include "blgenericcomboboxdelegate.h"

/// Constructor
/**
   \param comp
   \param parent
   \param table
   \param id_field
   \param text_field
   \param allow_null Por defecto se admite el valor nulo
   \param cond Condición opcional para un WHERE de consulta
**/
BlGenericComboBoxDelegate::BlGenericComboBoxDelegate ( int id_column, BlMainCompany *comp, QObject *parent, QString table, QString id_field, QString text_field, bool allow_null, QString cond )
   : BlSubFormDelegate ( parent )
   , m_id_column ( id_column )
   , m_company ( comp )
   , m_table ( table )
   , m_id_field ( id_field )
   , m_text_field ( text_field )
   , m_allow_null ( allow_null )
   , m_cond ( cond )
{
   _depura ( "BlGenericComboBoxDelegate::BlGenericComboBoxDelegate", 0 ) ;

   _depura ( "END BlGenericComboBoxDelegate::BlGenericComboBoxDelegate", 0 ) ;
}


/// Personalizar el aspecto del campo
/**
   \param painter
   \param vis Parámetros de visualización para el elemento en la vista
   \param index Índice en el modelo del dato a editar
**/
void BlGenericComboBoxDelegate::paint ( QPainter *painter, const QStyleOptionViewItem &vis, const QModelIndex &index )  const
{
   _depura ( "BlGenericComboBoxDelegate::paint", 0 ) ;

   QModelIndex index_ant = index.model()->index( index.row(), m_id_column );

   BlSubFormDelegate::paint ( painter, vis, index );

   // La posición no coincide: no tiene en cuenta las cabeceras
   QPoint pos = vis.rect.topLeft();
   pos.setX ( pos.x() + 5 );
   pos.setY ( pos.y() + 17 );

   QString id = index_ant.model()->data ( index_ant ).toString();

   if ( !id.isEmpty () )
   {
	// Aquí no usamos la condición, ya que tenemos el "id" concreto a mostrar y no es necesario filtrar más
	QString consulta = QString ( "SELECT %1 FROM %2 WHERE %3 = %4" )
				 .arg ( m_text_field )
				 .arg ( m_table )
				 .arg ( m_id_field )
				 .arg ( index_ant.model()->data ( index_ant ) .toString() );

	QString valor = m_company->loadQuery ( consulta )->valor ( m_text_field );

	painter->setPen ( Qt::SolidLine );
	painter->drawText ( pos, m_company->loadQuery ( consulta )->valor ( m_text_field ) );
   }

   _depura ( "END BlGenericComboBoxDelegate::paint", 0 ) ;
}


/// Crear la lista desplegable que servirá para editar el campo
/**
   \param parent
   \param vis Parámetros de visualización para el elemento en la vista
   \param index Índice en el modelo del dato a editar
**/
QWidget *BlGenericComboBoxDelegate::createEditor ( QWidget *parent, const QStyleOptionViewItem &vis, const QModelIndex &index )  const
{
   _depura ( "BlGenericComboBoxDelegate::createEditor", 0 ) ;

   QString consulta = QString ( "SELECT %1, %2 FROM %3" )
			    .arg(m_id_field)
			    .arg(m_text_field)
			    .arg(m_table);

   if (!m_cond.isEmpty())
   {
	consulta += QString ( " WHERE %1" )
			.arg(m_cond);
   }

   consulta += QString ( " ORDER BY %2" )
		   .arg ( m_text_field );

   BlComboBox *cbox = new BlComboBox ( parent );
   cbox->setMainCompany ( m_company );
   cbox->setQuery ( consulta );
   cbox->setFieldId ( m_id_field );
   cbox->m_valores[m_text_field] = "";
   cbox->setAllowNull ( true );

   _depura ( "END BlGenericComboBoxDelegate::createEditor", 0 ) ;

   return cbox;
}


/// Poner la lista desplegable en la posición indicada por el valor del campo a editar
/**
   \param editor
   \param index Índice en el modelo del dato a editar
**/
void BlGenericComboBoxDelegate::setEditorData ( QWidget *editor, const QModelIndex &index )  const
{
   _depura ( "BlGenericComboBoxDelegate::setEditorData", 0 ) ;

   BlComboBox *cbox = ( BlComboBox *) editor;
   QModelIndex index_ant = index.model()->index( index.row(), m_id_column );
   QString id = index_ant.data().toString();

   cbox->setId(id);

   _depura ( "END BlGenericComboBoxDelegate::setEditorData", 0 ) ;
}


/// Guarda los cambios realizados en el widget sobre los datos asociados en la columna
/**
   \param editor
   \param model
   \param index Índice en el modelo del dato a editar
**/
void BlGenericComboBoxDelegate::setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index )  const
{
   _depura ( "BlGenericComboBoxDelegate::setModelData", 0 ) ;

   QModelIndex index_ant = index.model()->index( index.row(), m_id_column );

   QString id = ( ( BlComboBox *) editor ) ->id();

   // No permitir dejar el campo vacío si así se ha establecido
   if ( ( id.isEmpty() && !m_allow_null ) )
   {
	return;
   }

   model->setData ( index_ant, id );

   _depura ( "END BlGenericComboBoxDelegate::setModelData", 0 ) ;
}


/// Establece el tama&ntilde;o y posición del widget editor
/**
   \param editor
   \param vis Parámetros de visualización para el elemento en la vista
   \param index Índice en el modelo del dato a editar
**/
void BlGenericComboBoxDelegate::updateEditorGeometry ( QWidget *editor, const QStyleOptionViewItem &vis, const QModelIndex &index )  const
{
   _depura ( "BlGenericComboBoxDelegate::updateEditorGeometry", 0 ) ;

   editor->setGeometry ( vis.rect ) ;

   _depura ( "END BlGenericComboBoxDelegate::updateEditorGeometry", 0 ) ;
}
