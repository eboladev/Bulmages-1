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
**/
BlGenericComboBoxDelegate::BlGenericComboBoxDelegate ( BlMainCompany *comp, QObject *parent)
   : BlSubFormDelegate ( parent )
   , m_company ( comp )
   , m_allowNull ( true )
   , m_fk_column ( -1 )
{
   blDebug ( "BlGenericComboBoxDelegate::BlGenericComboBoxDelegate", 0 );

   blDebug ( "END BlGenericComboBoxDelegate::BlGenericComboBoxDelegate", 0 );
}


/// Establecer el acceso a los datos que mostrar&aacute; la lista desplegable
/**
  \param table Tabla de la que tomar&aacute; los datos la lista desplegable
  \param id_field Campo identificador de dicha tabla
  \param text_field Campo con los valores a mostrar en la lista desplegable
**/
void BlGenericComboBoxDelegate::set_foreign_table ( const QString &table, const QString &id_field, const QString &text_field )
{
   blDebug ( "BlGenericComboBoxDelegate::set_foreign_table", 0 );

   m_table = table;
   m_id_field = id_field;
   m_text_field = text_field;

   blDebug ( "END BlGenericComboBoxDelegate::set_foreign_table", 0 );
}


/// Establecer qu&eacute; campo de la tabla referencia a la tabla de la que
/// tomar&aacute; sus datos la lista desplegable
/**
  \param fk_column N&uacute;mero de columna
  \param fk_field_name Nombre del campo
**/
void BlGenericComboBoxDelegate::set_foreign_field ( unsigned int fk_column, QString fk_field_name )
{
   blDebug ( "BlGenericComboBoxDelegate::set_foreign_field", 0 );

   m_fk_column = fk_column;
   m_fk_field_name = fk_field_name;

   blDebug ( "END BlGenericComboBoxDelegate::set_foreign_field", 0 );
}


/// Establecer un filtrado opcional usando un identificador adicional de la
/// tabla del desplegable compar&acute;ndolo con otro campo identificador
/// de la tabla del listado
/// Ejemplo: filtrar el desplegable seg&uacute;n el valor de otro en la mismo listado
/// Nota: no abusar de este filtro, ya que recarga el desplegable para cada
/// registro que editamos y puede ralentizar el uso del programa
/**
  \param fi_field_name Campo identificador en la tabla del desplegable
  \param fi_fk_field_name Campo identificador en la tabla del listado
**/
void BlGenericComboBoxDelegate::set_filter_id ( const QString &fi_field_name, const QString &fi_fk_field_name )
{
   m_fi_field_name = fi_field_name;
   m_fi_fk_field_name = fi_fk_field_name;
}


/// Establecer un filtrado opcional sobre la tabla usada por el desplegable
/**
  \param cond Condici&oacute;n para la cl&aacute;usula WHERE
**/
void BlGenericComboBoxDelegate::set_where_condition ( const QString &cond )
{
   m_cond = cond;
}


///
/**
  \param v
**/
void BlGenericComboBoxDelegate::setAllowNull ( bool v )
{
   m_allowNull = v;
}


/// Verificar la inicializaci&oaucte;n de los atributos
/// y poner el concepto que le corresponda a cada campo de la columna
/**
  \param combo_field_name
**/
void BlGenericComboBoxDelegate::initialize ( const QString &combo_field_name )
{
   blDebug ( "BlGenericComboBoxDelegate::initialize", 0 );

   m_combo_field_name = combo_field_name;

   if ( m_fk_column <= -1
   || m_fk_field_name.isEmpty()
   || m_table.isEmpty()
   || m_id_field.isEmpty()
   || m_text_field.isEmpty()
   || m_combo_field_name.isEmpty() ) {
	blMsgError ( _( "Falta definir algún parámetro necesario para BlGenericComboBoxDelegate." ) , (QWidget *) parent() );
   }

   /// En vez de dibujar los valores en paint, los establecemos como datos de la tabla
   /// para evitar pegas con ordenación, filtros e impresión
   for ( int i = 0; i < m_subform->mui_list->rowCount(); i++ )
   {
	QString fk_id = m_subform->dbValue ( m_fk_field_name, i );

	if ( fk_id.isEmpty() ) continue;

	QString valor = m_company->loadQuery ( query_only_one ( fk_id ) )->valor ( m_text_field );

	m_subform->setDbValue(combo_field_name, i, valor);
   } // end for

   blDebug ( "END BlGenericComboBoxDelegate::initialize", 0 );
}


/// Crear la lista desplegable que servirá para editar el campo
/**
   \param parent
   \param vis Parámetros de visualización para el elemento en la vista
   \param index &Iacute;ndice en el modelo del dato a editar
   \return Widget editor
**/
QWidget *BlGenericComboBoxDelegate::createEditor ( QWidget *parent, const QStyleOptionViewItem &vis, const QModelIndex &index )  const
{
   blDebug ( "BlGenericComboBoxDelegate::createEditor", 0 );

   BlComboBox *cbox = new BlComboBox ( parent );
   cbox->setMainCompany ( m_company );
   cbox->setQuery ( query() );
   cbox->setFieldId ( m_id_field );
   cbox->m_valores[m_text_field] = "";
   cbox->setAllowNull ( true );

   blDebug ( "END BlGenericComboBoxDelegate::createEditor", 0 );

   return cbox;
}


/// Poner la lista desplegable en la posición indicada por el valor del campo a editar
/**
   \param editor
   \param index &Iacute;ndice en el modelo del dato a editar
**/
void BlGenericComboBoxDelegate::setEditorData ( QWidget *editor, const QModelIndex &index )  const
{
   blDebug ( "BlGenericComboBoxDelegate::setEditorData", 0 );

   BlComboBox *cbox = ( BlComboBox *) editor;
   QModelIndex index_id_field = index.model()->index( index.row(), m_fk_column );
   QString id = index_id_field.data().toString();

   if ( !m_fi_field_name.isEmpty() && !m_fi_fk_field_name.isEmpty() )
   {
	cbox->setQuery ( query ( index.row() ) );
   } // end if

   cbox->setId ( id );

   /// Si no hay id previo, usar el primer elemento si no se permite el valor nulo
   if ( id.isEmpty() && !m_allowNull )
   {
	/// Si s&oacute;lo tenemos el valor nulo disponible en la lista desplegable,
	/// avisar de ello, ya que es un valor requerido
	if ( cbox->count() <= 1 )
	{
	   blMsgError ( _( "No existen elementos en el listado \"%1\", y el campo \"%2\" requiere alguno." ).arg ( m_table ).arg ( m_combo_field_name ) , (QWidget *) parent() );
	   return;
	}
	cbox->setCurrentIndex ( 1 );
   }

   connect ( cbox, SIGNAL ( activated ( QString ) ),
		 this, SLOT ( emit_currentValueChangedByUser ( QString ) ) );

   blDebug ( "END BlGenericComboBoxDelegate::setEditorData", 0 );
}


/// Guarda los cambios realizados en el widget sobre los datos asociados en la columna
/**
   \param editor
   \param model
   \param index &Iacute;ndice en el modelo del dato a editar
**/
void BlGenericComboBoxDelegate::setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index )  const
{
   blDebug ( "BlGenericComboBoxDelegate::setModelData", 0 );

   QModelIndex index_id_field = index.model()->index( index.row(), m_fk_column );

   QString id = ( ( BlComboBox *) editor )->id();
   QString text = ( ( BlComboBox *) editor )->currentText();

   /// No permitir dejar el campo vac&iacute;o si as&iacute; se ha establecido
   if ( ( id.isEmpty() && !m_allowNull ) ) return;

   model->setData ( index_id_field, id );
   model->setData ( index, text );

   blDebug ( "END BlGenericComboBoxDelegate::setModelData", 0 );
}


/// Establece el tama&ntilde;o y posición del widget editor
/**
   \param editor
   \param vis Par&aacute;metros de visualización para el elemento en la vista
   \param index &Iacute;ndice en el modelo del dato a editar
**/
void BlGenericComboBoxDelegate::updateEditorGeometry ( QWidget *editor, const QStyleOptionViewItem &vis, const QModelIndex &index )  const
{
   blDebug ( "BlGenericComboBoxDelegate::updateEditorGeometry", 0 );

   editor->setGeometry ( vis.rect );

   blDebug ( "END BlGenericComboBoxDelegate::updateEditorGeometry", 0 );
}


/// Formar la consulta que rellena al desplegable
/**
  \param row Si se indica (>-1), utilizar el filtrado opcional sobre otro campo
**/
QString BlGenericComboBoxDelegate::query ( int row ) const
{
   blDebug ( "BlGenericComboBoxDelegate::query", 0 );

   QString q = QString ( "SELECT %1, %2 FROM %3" )
			    .arg(m_id_field)
			    .arg(m_text_field)
			    .arg(m_table);

   if ( !m_cond.isEmpty() ) {
	q += QString ( " WHERE %1" )
			.arg(m_cond);
   } // end if


   /// Filtrar usando otra columna de la tabla si se ha indicado
   if ( row > -1 )
   {
	QString fi_fk_field_value = m_subform->dbValue( m_fi_fk_field_name, row );

	/// Filtrar por otra columna si dicha columna contiene un valor identificador
	/// En caso de que no, se muestra el listado completo como si no hubiera filtro
	if ( !fi_fk_field_value.isEmpty() )
	{

	   if ( m_cond.isEmpty() ) {
		q += QString ( " WHERE " );
	   } else {
		q += QString ( " AND " );
	   } // end if

	   q += QString ( "%1 = %2")
			   .arg ( m_fi_field_name )
			   .arg ( fi_fk_field_value );
	} // end if
   } // end if

   q += QString ( " ORDER BY %1" )
		   .arg ( m_text_field );

   blDebug ( "END BlGenericComboBoxDelegate::query", 0 );

   return q;
}


/// Devolver el valor de para cierto identificador de la tabla del desplegable
/**
  \param fk_id
**/
QString BlGenericComboBoxDelegate::query_only_one ( const QString &fk_id ) const
{
   blDebug ( "BlGenericComboBoxDelegate::query_only_one", 0 );

   QString q = QString ( "SELECT %1 FROM %2 WHERE %3 = %4" )
		   .arg ( m_text_field )
		   .arg ( m_table )
		   .arg ( m_id_field )
		   .arg ( fk_id );

   blDebug ( "END BlGenericComboBoxDelegate::query_only_one", 0 );

   return q;
}


/// Emite la se&ntilde;al de valor cambiado
/**
  \param value
**/
void BlGenericComboBoxDelegate::emit_currentValueChangedByUser ( QString value )
{
   blDebug ( "BlGenericComboBoxDelegate::emit_currentValueChangedByUser", 0, value );

   /// Evitar emitir la se&ntilde;al si en realidad no hay cambio de valor
   if ( m_subform->dbValue ( m_combo_field_name, m_subform->currentRow() ) != value )
   {
	emit currentValueChangedByUser ( value );
   }

   blDebug ( "END BlGenericComboBoxDelegate::emit_currentValueChangedByUser", 0, value );
}
