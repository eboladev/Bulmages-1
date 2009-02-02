/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C. (Porting to QT4)              *
 *   fcojavmc@todo-redes.com                                               *
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

#include <QMessageBox>
#include <QWidget>
#include <QComboBox>
#include <QToolButton>
#include <QTextStream>
#include <QLayout>
#include <fstream>

#include "listzonacomercialview.h"
#include "company.h"
#include "configuracion.h"
#include "busquedacliente.h"
#include "busquedafecha.h"

#include "funcaux.h"


///
/**
\param comp
\param parent
**/
ListZonaComercialView::ListZonaComercialView ( Company *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "INIT_ListZonaComercialView::ListZonaComercialView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_listado->setEmpresaBase ( comp );
    mui_listado->cargar();
    meteWindow ( windowTitle(), this );
    _depura ( "END_ListZonaComercialView::ListZonaComercialView", 0 );
}


///
/**
**/
ListZonaComercialView::~ListZonaComercialView()
{
    _depura ( "ListZonaComercialView::~ListZonaComercialView", 0 );
    empresaBase() ->sacaWindow ( this );
    _depura ( "ListZonaComercialView::~ListZonaComercialView", 0 );
}


/// ===================================== SUBFORMULARIO ===============================================
///
/**
\param parent
**/
ListZonaComercialSubForm::ListZonaComercialSubForm ( QWidget *parent ) : SubForm2Bf ( parent )
{
    _depura ( "ListZonaComercialSubForm::ListZonaComercialSubForm", 0 );
    setDBTableName ( "zonacomercial" );
    setDBCampoId ( "idzonacomercial" );
    addSHeader ( "idzonacomercial", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, _( "ID zona comercial" ) );
    addSHeader ( "nomzonacomercial", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, _( "Nombre de la zona comercial" ) );
    setinsercion ( TRUE );
    _depura ( "END ListZonaComercialSubForm::ListZonaComercialSubForm", 0 );
}

