/***************************************************************************
 *   Copyright (C) 2009 by Aron Galdon                                     *
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

#ifndef BLFORMLISTFILTER_H
#define BLFORMLISTFILTER_H

#include "blwidget.h"
#include "blsubform.h"
#include "ui_blformlistfilterbase.h"


/// BlFormListFilter, widget que permite filtrar un listado.
/**
  Dependiendo del tipo de la columna en la que se encuentra el cursor,
  el filtro se adaptar&aacute;. Los tipos contemplados por la clase son:
  - intervalo de dos fechas
  - valor num&eacute;rico  ( entero o con decimales )
  - booleano  ( mediante un QCheckBox triestado )
  - texto  ( permitiendo decidir si buscamos la cadena al principio, en cualquier sitio o exactamente )

  Forma de uso desde un widget tipo "List" de un listado concreto de plugin:

  1) Agregar un QWidget llamado "mui_filtador" y promocionarlo a BlFormListFilter.

  2) Agregar en la barra de botones un bot&oacute;n (puede usarse el que hay: "mui_filtrar").

  3) Activarle a dicho bot&oacute;n la propiedad checkable y ponerle el icono i_filter.png.

  4) Para que el filtro pueda acceder al listado y se muestre y oculte al pulsar el bot&oacute;n,
  en el constructor del listado concreto, agregar:
     mui_filtrador->setSubFormList(mui_list);
     connect(mui_filtrar, SIGNAL(released()), mui_filtrador, SLOT(showHideFilter()));

  5) Para que se aplique el filtrado al actualizar el listado, terminar la consulta SQL de presentar() con esto:
     ...WHERE 1 = 1 " + mui_filtrador->generarFiltro());

**/
class BL_EXPORT BlFormListFilter : public BlWidget, public Ui_BlFormListFilterBase
{
   Q_OBJECT

public:
   BlFormListFilter ( QWidget *parent );
   void startFilter  (  ) ;
   virtual QString generarFiltro  (  );
   void hideFilterWidgets  (  );
   void configureFilterToType  (  );
   void setSubFormList ( BlSubForm *list );

protected:
   QString generateDateFilter ( QString campo, QString texto );
   QString generateNumericFilter ( QString campo, QString texto );
   QString generateBooleanFilter ( QString campo, Qt::CheckState );
   QString generateTextFilter ( QString campo, QString texto );

   /// Almacena el n&uacute;mero de columna activa para realizar los filtrados.
   int m_columna_actual;

public slots:
   virtual void showHideFilter (  );
   virtual void cleanFilter (  );
   virtual void updatePosition ( int fila, int columna, int fila_anterior, int columna_anterior );

private:
   /// Puntero al subformulario con el listado que deseamos filtrar
   BlSubForm * mui_subform_list;
};

#endif // BLFORMLISTFILTER_H
