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

#include "bldatedelegate.h"

/// Constructor
/**
   \param comp
   \param parent
   \param allowNull
**/
BlDateDelegate::BlDateDelegate ( BlMainCompany *comp, QObject *parent, bool allowNull )
   : QItemDelegate ( parent )
   , m_company ( comp )
   , m_allowNull ( allowNull )
{
   blDebug ( "BlDateDelegate::BlDateDelegate", 0 ) ;

   blDebug ( "END BlDateDelegate::BlDateDelegate", 0 ) ;
}


/// Crear el calendario que servir&aacute; para editar el campo
/**
   \param parent
   \param vis Par&aacute;metros de visualizaci&oacute;n para el elemento en la vista
   \param index &Iacute;ndice en el modelo del dato a editar
**/
QWidget *BlDateDelegate::createEditor ( QWidget *parent, const QStyleOptionViewItem &vis, const QModelIndex &index ) const
{
   blDebug ( "BlDateDelegate::createEditor", 0 ) ;

   BlDateSearch *ds = new BlDateSearch ( parent ) ;
   ds->setMainCompany ( m_company ) ;

   blDebug ( "END BlDateDelegate::createEditor", 0 ) ;

   return ds;
}


/// Poner el buscador de fecha en la posici&oacute;n indicada por el valor del campo a editar
/**
   \param editor
   \param index &Iacute;ndice en el modelo del dato a editar
**/
void BlDateDelegate::setEditorData ( QWidget *editor, const QModelIndex &index ) const
{
   blDebug ( "BlDateDelegate::setEditorData", 0 ) ;

   BlDateSearch *ds = ( BlDateSearch * ) editor;
   QString fecha = index.model()->data ( index ).toString();

   /// Si no hay fecha previa, usar la de hoy si no se permite el valor nulo
   if ( fecha.isEmpty() && !m_allowNull )
   {
	fecha = QDate::currentDate().toString("dd/MM/yyyy");
   }

   ds->setfecha ( fecha );

   blDebug ( "END BlDateDelegate::setEditorData", 0 ) ;
}


/// Guarda los cambios realizados en el widget sobre los datos asociados en la columna
/**
   \param editor
   \param model
   \param index &Iacute;ndice en el modelo del dato a editar
**/
void BlDateDelegate::setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
   blDebug ( "BlDateDelegate::setModelData", 0 ) ;

   QString fecha = ( ( BlDateSearch * ) editor ) ->text();
   model->setData ( index, fecha ) ;

   blDebug ( "END BlDateDelegate::setModelData", 0 ) ;
}


/// Establece el tama&ntilde;o y posici&oacute;n del widget editor
/**
   \param editor
   \param vis Par&aacute;metros de visualizaci&oacute;n para el elemento en la vista
   \param index &Iacute;ndice en el modelo del dato a editar
**/
void BlDateDelegate::updateEditorGeometry ( QWidget *editor, const QStyleOptionViewItem &vis, const QModelIndex &index ) const
{
   blDebug ( "BlDateDelegate::updateEditorGeometry", 0 ) ;

   editor->setGeometry ( vis.rect ) ;

   blDebug ( "END BlDateDelegate::updateEditorGeometry", 0 ) ;
}


///
/**
  \param v
**/
void BlDateDelegate::setAllowNull ( bool v )
{
   m_allowNull = v;
}
