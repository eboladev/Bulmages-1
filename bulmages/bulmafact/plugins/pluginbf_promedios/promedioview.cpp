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

#include <QtGui/QCloseEvent>

#include "promedioview.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "blplugins.h"


///
/**
\param comp
\param parent
**/
PromedioView::PromedioView ( BfCompany *comp, QWidget *parent )
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
PromedioView::~PromedioView()
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
int PromedioView::load ( QString idarticulo )
{
    BL_FUNC_DEBUG

    QString query = "SELECT sum(pvplalbaranp*cantlalbaranp)::NUMERIC(12,2) AS tot, sum(cantlalbaranp) as und, max(pvplalbaranp) AS mayor, min(pvplalbaranp) AS menor, avg(pvplalbaranp)::NUMERIC(12,2) AS media, max(cantlalbaranp) as undmayorcompras, min(cantlalbaranp) AS undmenorcompras, avg(cantlalbaranp)::NUMERIC(12,2) AS undavgcompras  FROM lalbaranp WHERE idarticulo=" + idarticulo;
    BlDbRecordSet *cur = m_companyact->loadQuery ( query );
    if ( !cur->eof() ) {
        mui_totalcompras->setText ( cur->value( "tot" ) );
        mui_mayorcompras->setText ( cur->value( "mayor" ) );
        mui_menorcompras->setText ( cur->value( "menor" ) );
        mui_avgcompras->setText ( cur->value( "media" ) );
        mui_undcompras->setText ( cur->value( "und" ) );
        mui_undmayorcompras->setText ( cur->value( "undmayorcompras" ) );
        mui_undmenorcompras->setText ( cur->value( "undmenorcompras" ) );
        mui_undavgcompras->setText ( cur->value( "undavgcompras" ) );
    } // end if
    delete cur;

    
    query = "SELECT sum(pvplalbaran*cantlalbaran)::NUMERIC(12,2) AS tot, sum(cantlalbaran) as und, max(pvplalbaran) AS mayor, min(pvplalbaran) AS menor, avg(pvplalbaran)::NUMERIC(12,2) AS media, max(cantlalbaran) as undmayorventas, min(cantlalbaran) AS undmenorventas, avg(cantlalbaran)::NUMERIC(12,2) AS undavgventas  FROM lalbaran WHERE idarticulo=" + idarticulo;
    BlDbRecordSet *cur1 = m_companyact->loadQuery ( query );
    if ( !cur1->eof() ) {
        mui_totalventas->setText ( cur1->value( "tot" ) );
        mui_mayorventas->setText ( cur1->value( "mayor" ) );
        mui_menorventas->setText ( cur1->value( "menor" ) );
        mui_avgventas->setText ( cur1->value( "media" ) );
        mui_undventas->setText ( cur1->value( "und" ) );
        mui_undmayorventas->setText ( cur1->value( "undmayorventas" ) );
        mui_undmenorventas->setText ( cur1->value( "undmenorventas" ) );
        mui_undavgventas->setText ( cur1->value( "undavgventas" ) );
    } // end if
    delete cur1;

    
    return 0;
}

