/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#include <QKeyEvent>
#include <QEvent>
#include <QLineEdit>
#include <QTextEdit>

#include "cuadranteqtreewidget.h"
#include "blconfiguration.h"
#include "blfunctions.h"



/** Constructor de CuadranteQTreeWidget clase derivada de QTableWidget con
un eventHandler especifico
*/
/**
\param parent
**/
CuadranteQTreeWidget::CuadranteQTreeWidget ( QWidget *parent ) : QTreeWidget ( parent )
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
CuadranteQTreeWidget::~CuadranteQTreeWidget()
{
    
}


///
/**
**/
void CuadranteQTreeWidget::startDrag ( Qt::DropActions supportedActions )
{
    BL_FUNC_DEBUG

    QDrag *drag = new QDrag ( this );
    QMimeData *mimeData = new QMimeData;

    mimeData->setText ( currentItem() ->text ( 1 ) );
//         mimeData->setText("macagonsaputa");
    drag->setMimeData ( mimeData );
//         drag->setPixmap(iconPixmap);

    Qt::DropAction dropAction = drag->start ( Qt::ActionMask );

    
}





