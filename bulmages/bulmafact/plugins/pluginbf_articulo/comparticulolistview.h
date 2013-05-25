/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#ifndef LISTCOMPARTICULOVIEW_H
#define LISTCOMPARTICULOVIEW_H

#include <QtCore/QEvent>

#include "bfcompany.h"
#include "bltablewidget.h"
#include "bfsubform.h"
#include "pdefs_pluginbf_articulo.h"


/// Muestra y administra el listado de los componentes de un art&iacute;culo.
/** Implementa el subformulario de componentes de articulo que aparece en la ficha de articulo.
    Sobrecarga los metodos necesarios para que el subformulario trabaje con la tabla comparticulo
    de la base de datos.
    Al ser una clase derivada de BfSubForm para funcionar debe ser previamente inicializada
    con setcompany. */
class PLUGINBF_ARTICULO_EXPORT ListCompArticuloView : public BfSubForm
{
    Q_OBJECT

public:
    /// Indica el idarticulo de la ficha con el que trabajamos.
    QString mdb_idarticulo;
    ListCompArticuloView ( QWidget *parent = 0, const char *name = 0 );
    ~ListCompArticuloView();

public slots:
    virtual void load ( QString idarticulo );
    virtual void editFinished ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp );
    virtual void pressedAsterisk ( int row, int col, BlDbSubFormRecord *rec, BlDbSubFormField *camp );
};

#endif

