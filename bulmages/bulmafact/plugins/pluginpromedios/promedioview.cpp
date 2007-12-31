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

#include "promedioview.h"
#include "company.h"
#include "funcaux.h"
#include "plugins.h"


///
/**
\param comp
\param parent
**/
PromedioView::PromedioView ( Company *comp, QWidget *parent )
        : QWidget ( parent ) , dialogChanges ( this ), DBRecord ( comp )
{
    _depura ( "PromedioView::INIT_constructor()", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    m_companyact = comp;
    setupUi ( this );
    _depura ( "PromedioView::END_constructor()", 0 );
}


///
/**
**/
PromedioView::~PromedioView()
{
    _depura ( "PromedioView::INIT_destructor()\n", 0 );
    m_companyact->sacaWindow ( this );
    _depura ( "PromedioView::END_destructor()\n", 0 );
}




/// Esta funci&oacute;n carga un art&iacute;culo de la base de datos y lo presenta.
/// Si el par&aacute;metro pasado no es un identificador v&aacute;lido entonces se pone
/// la ventana de edici&oacute;n en modo de inserci&oacute;n.
/**
\param idarticulo
\return
**/
int PromedioView::cargar ( QString idarticulo )
{
    _depura ( "PromedioView::cargar(" + idarticulo + ")", 0 );

    cursor2 *cur = m_companyact->cargacursor ( "SELECT sum(pvplalbaranp*cantlalbaranp)::NUMERIC(12,2) AS tot, sum(cantlalbaranp) as und, max(pvplalbaranp) AS mayor, min(pvplalbaranp) AS menor, avg(pvplalbaranp)::NUMERIC(12,2) AS media, max(cantlalbaranp) as undmayorcompras, min(cantlalbaranp) AS undmenorcompras, avg(cantlalbaranp)::NUMERIC(12,2) AS undavgcompras  FROM lalbaranp WHERE idarticulo=" + idarticulo );
    if ( !cur->eof() ) {
        mui_totalcompras->setText ( cur->valor ( "tot" ) );
        mui_mayorcompras->setText ( cur->valor ( "mayor" ) );
        mui_menorcompras->setText ( cur->valor ( "menor" ) );
        mui_avgcompras->setText ( cur->valor ( "media" ) );
        mui_undcompras->setText ( cur->valor ( "und" ) );
        mui_undmayorcompras->setText ( cur->valor ( "undmayorcompras" ) );
        mui_undmenorcompras->setText ( cur->valor ( "undmenorcompras" ) );
        mui_undavgcompras->setText ( cur->valor ( "undavgcompras" ) );
    } // end if
    delete cur;


    cursor2 *cur1 = m_companyact->cargacursor ( "SELECT sum(pvplalbaran*cantlalbaran)::NUMERIC(12,2) AS tot, sum(cantlalbaran) as und, max(pvplalbaran) AS mayor, min(pvplalbaran) AS menor, avg(pvplalbaran)::NUMERIC(12,2) AS media, max(cantlalbaran) as undmayorventas, min(cantlalbaran) AS undmenorventas, avg(cantlalbaran)::NUMERIC(12,2) AS undavgventas  FROM lalbaran WHERE idarticulo=" + idarticulo );
    if ( !cur1->eof() ) {
        mui_totalventas->setText ( cur1->valor ( "tot" ) );
        mui_mayorventas->setText ( cur1->valor ( "mayor" ) );
        mui_menorventas->setText ( cur1->valor ( "menor" ) );
        mui_avgventas->setText ( cur1->valor ( "media" ) );
        mui_undventas->setText ( cur1->valor ( "und" ) );
        mui_undmayorventas->setText ( cur1->valor ( "undmayorventas" ) );
        mui_undmenorventas->setText ( cur1->valor ( "undmenorventas" ) );
        mui_undavgventas->setText ( cur1->valor ( "undavgventas" ) );
    } // end if
    delete cur1;

    _depura ( "END PromedioView::cargar()", 0 );
    return 0;
}

