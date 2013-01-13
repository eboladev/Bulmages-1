/***************************************************************************
 *   Copyright (C) 2004 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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

#include <QCloseEvent>

#include "qwebcamview.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "blplugins.h"


///
/**
\param comp
\param parent
**/
QWebCamView::QWebCamView ( BfCompany *comp, QWidget *parent )
        : QWidget ( parent ) , BlDialogChanges ( this ), BlDbRecord ( comp )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    m_companyact = comp;
    setupUi ( this );
    
}


///
/**
**/
QWebCamView::~QWebCamView()
{
    BL_FUNC_DEBUG
    m_companyact->removeWindow ( this );
    
}




/// Esta funci&oacute;n carga un art&iacute;culo de la base de datos y lo presenta.
/// Si el par&aacute;metro pasado no es un identificador v&aacute;lido entonces se pone
/// la ventana de edici&oacute;n en modo de inserci&oacute;n.
/**
\param idarticulo
\return
**/
int QWebCamView::load ( QString idarticulo )
{
    BL_FUNC_DEBUG
    
    return 0;
}

