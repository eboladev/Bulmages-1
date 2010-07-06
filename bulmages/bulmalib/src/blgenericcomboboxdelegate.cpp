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
   \param fk_column
   \param fk_field_name
   \param comp
   \param parent
   \param table
   \param id_field
   \param text_field
   \param combo_field_name
   \param allow_null Por defecto se admite el valor nulo
   \param cond Condición opcional para un WHERE de consulta
**/
BlGenericComboBoxDelegate::BlGenericComboBoxDelegate ( int fk_column, QString fk_field_name, BlMainCompany *comp, QObject *parent, QString table, QString id_field, QString text_field, QString combo_field_name, bool allow_null, QString cond )
   : BlSubFormDelegate ( parent )
   , m_fk_column ( fk_column )
   , m_company ( comp )
   , m_table ( table )
   , m_id_field ( id_field )
   , m_text_field ( text_field )
   , m_allow_null ( allow_null )
   , m_cond ( cond )
{
   _depura ( "BlGenericComboBoxDelegate::BlGenericComboBoxDelegate", 0 ) ;

   /// En vez de dibujar los valores en paint, los establecemos como datos de la tabla
   /// para evitar pegas con ordenación, filtros e impresión
   for ( int i = 0; i < m_subform->mui_list->rowCount(); i++ )
   {
	QString fk_id = m_subform->dbValue ( fk_field_name, i );

	if ( fk_id.isEmpty() ) {
	   continue;
	}

	QString consulta = QString ( "SELECT %1 FROM %2 WHERE %3 = %4" )
				 .arg ( m_text_field )
				 .arg ( m_table )
				 .arg ( m_id_field )
				 .arg ( fk_id );

	QString valor = comp->loadQuery ( consulta )->valor ( m_text_field );

	m_subform->setDbValue(combo_field_name, i, valor);
   }

   _depura ( "END BlGenericComboBoxDelegate::BlGenericComboBoxDelegate", 0 ) ;
}


/// Personalizar el aspecto del campo
/**
   \param painter
   \param vis Parámetros de visualización para el elemento en la vista
   \param index Índice en el modelo del dato a editar
**/
//void BlGenericComboBoxDelegate::paint ( QPainter *painter, const QStyleOptionViewItem &vis, const QModelIndex &index ) const
//{
//   _depura ( "BlGenericComboBoxDelegate::paint", 0 ) ;
//
//   BlSubFormDelegate::paint ( painter, vis, index );
//
//   _depura ( "END BlGenericComboBoxDelegate::paint", 0 ) ;
//}


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

   if (!m_cond.isEmpty()) {
	consulta += QString ( " WHERE %1" )
			.arg(m_cond);
   } // end if

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
   QModelIndex index_id_field = index.model()->index( index.row(), m_fk_column );
   QString id = index_id_field.data().toString();

   cbox->setId ( id );

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

   QModelIndex index_id_field = index.model()->index( index.row(), m_fk_column );

   QString id = ( ( BlComboBox *) editor )->id();
   QString text = ( ( BlComboBox *) editor )->currentText();

   /// No permitir dejar el campo vac&iacute;o si as&iacute; se ha establecido
   if ( ( id.isEmpty() && !m_allow_null ) ) {
	return;
   } // end if

   model->setData ( index_id_field, id );
   model->setData ( index, text );

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
