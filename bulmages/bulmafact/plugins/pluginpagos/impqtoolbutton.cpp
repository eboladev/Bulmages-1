/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#include <QWidget>
/// Necesarios para importacion de efactura
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNode>
#include <QString>
#include <QFileDialog>
#include <QMap>
#include <QList>

#include "impqtoolbutton.h"
#include "blfunctions.h"
#include "facturapview.h"
#include "albaranproveedorview.h"
#include "pedidoproveedorview.h"
#include "blfixed.h"
#include "bfcompany.h"
#include "bldb.h"
#include "pagoview.h"


///
/**
\param pres
\param ped
\param alb
\param fac
\param parent
**/
GenPagoQToolButton::GenPagoQToolButton ( QWidget *fac , QWidget *parent ) : QToolButton ( parent )
{
    _depura ( "GenPagoQToolButton::GenPagoQToolButton", 0 );
    m_object = fac;
    setBoton();
    _depura ( "END GenPagoQToolButton::GenPagoQToolButton", 0 );
}


///
/**
**/
GenPagoQToolButton::~GenPagoQToolButton()
{
    _depura ( "GenPagoQToolButton::~GenPagoQToolButton", 0 );
    _depura ( "END GenPagoQToolButton::~GenPagoQToolButton", 0 );
}


///
/**
**/
void GenPagoQToolButton::setBoton()
{
    _depura ( "GenPagoQToolButton::setBoton", 0 );
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( "Generar Pago" );
    setToolTip ( "Generar Pago" );
    setMinimumSize ( QSize ( 32, 32 ) );
    setMaximumSize ( QSize (32, 32 ) );
    setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/pay.svg" ) ) );
    setIconSize ( QSize ( 32, 32 ) );
    setContentsMargins (0, 0, 0, 0);
    _depura ( "END GenPagoQToolButton::setBoton", 0 );
}


///
/**
**/
void GenPagoQToolButton::click()
{
    _depura ( "ImpQToolButton::click", 0 );

    if (m_object->objectName() == "FacturaProveedorBase" ) {
	FacturaProveedorView *fpv = (FacturaProveedorView *) m_object;
	PagoView *bud = new PagoView((BfCompany *)fpv->mainCompany(), NULL);
	fpv->mainCompany() ->m_pWorkspace->addWindow ( bud );
	bud->setDBvalue ( "idproveedor", fpv->DBvalue ( "idproveedor" ) );
	bud->setDBvalue ( "cantpago", fpv->m_totalfacturap->text() );
	bud->setDBvalue ( "refpago", fpv->DBvalue ( "reffacturap" ) );
	bud->setDBvalue ( "comentpago", fpv->DBvalue ( "descfacturap" ) );
	bud->pintar();
	bud->show();
    }// end if

    if (m_object->objectName() == "AlbaranProveedorBase" ) {
	AlbaranProveedorView *fpv = (AlbaranProveedorView *) m_object;
	PagoView *bud = new PagoView((BfCompany *)fpv->mainCompany(), NULL);
	fpv->mainCompany() ->m_pWorkspace->addWindow ( bud );
	bud->setDBvalue ( "idproveedor", fpv->DBvalue ( "idproveedor" ) );
	bud->setDBvalue ( "cantpago", fpv->m_totalalbaranp->text() );
	bud->setDBvalue ( "refpago", fpv->DBvalue ( "refalbaranp" ) );
	bud->setDBvalue ( "comentpago", fpv->DBvalue ( "descalbaranp" ) );
	bud->pintar();
	bud->show();
    }// end if

    if (m_object->objectName() == "PedidoProveedorBase" ) {
	PedidoProveedorView *fpv = (PedidoProveedorView *) m_object;
	PagoView *bud = new PagoView((BfCompany *)fpv->mainCompany(), NULL);
	fpv->mainCompany() ->m_pWorkspace->addWindow ( bud );
	bud->setDBvalue ( "idproveedor", fpv->DBvalue ( "idproveedor" ) );
	bud->setDBvalue ( "cantpago", fpv->mui_totalPedido->text() );
	bud->setDBvalue ( "refpago", fpv->DBvalue ( "refpedidoproveedor" ) );
	bud->setDBvalue ( "comentpago", fpv->DBvalue ( "descpedidoproveedor" ) );
	bud->pintar();
	bud->show();
    }// end if

    _depura ( "END ImpQToolButton::click", 0 );
}


