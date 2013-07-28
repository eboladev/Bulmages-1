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

#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtCore/QObject>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QToolButton>

#include <stdio.h>

#include "pluginbf_efactura.h"
#include "efactura.h"
// #include "efacturaenvio.h"
#include "efacturarecepcion.h"
#include "bfcompany.h"
#include "blfunctions.h"
// Exportacion
#include "facturaview.h"
#include "efqtoolbutton.h"
// Importacion
#include "facturasplist.h"
#include "efqtoolbutton_importar.h"


///
/**
**/
efacturabf::efacturabf()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
efacturabf::~efacturabf()
{
    BL_FUNC_DEBUG
    
}


/// Esto es lo que se activa al hacer click sobre el menu
/**
**/
void efacturabf::elslot()
{
    BL_FUNC_DEBUG
    EFactura *ef = new EFactura ( ( BfCompany * ) mainCompany() );
    ef->show();

//  EFacturaEnvio *efv = new EFacturaEnvio(mainCompany());
//  efv->show();

//  EFacturaRecepcion *efr = new EFacturaRecepcion(mainCompany());
//  efr->show();

//     fprintf(stderr,"Sa ha activado el slot\n");
//     QMessageBox::warning(0,
//                          "Titulo de la ventana",
//                          "Mensaje.",
//                          QMessageBox::Ok,
//                          QMessageBox::Cancel);
    
}


///
/**
**/
void efacturabf::elslot1()
{
    BL_FUNC_DEBUG
    EFacturaRecepcion *efr = new EFacturaRecepcion ( ( BfCompany * ) mainCompany() );
    efr->show();
    
}


///
/**
\param bges
**/
void efacturabf::inicializa ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG
    /// Creamos la conexion de la base de datos
    setMainCompany ( bges->company() );

    /// Creamos el menu.
//  QMenu *pPluginMenu = new QMenu("&Factura Electronica");
//
//  QAction *accion = new QAction("&Configuracion de e-factura", 0);
//  accion->setStatusTip("Muestra statustip");
//  accion->setWhatsThis("Muestra que es esto");
//
//  QAction *accion1 = new QAction("&Recepcion de e-facturas", 0);
//  accion1->setStatusTip("Muestra statustip");
//  accion1->setWhatsThis("Muestra que es esto");
//
//  connect(accion, SIGNAL(activated()), this, SLOT(elslot()));
//  connect(accion1, SIGNAL(activated()), this, SLOT(elslot1()));
//
//  /// Anyadimos los submenus al menu de efactura
//  pPluginMenu->addAction(accion);
//  pPluginMenu->addAction(accion1);
//
//  /// Anyadimos la nueva opcion al menu principal del programa.
//  bges->menuBar()->addMenu(pPluginMenu);
    
}


///
/**
\param bges
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_efactura", confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    efacturabf *efact = new efacturabf();
    efact->inicializa ( bges );
    /// SOLO A MODO DE EJEMPLO: se modifica el titulo de la ventana principal
    /// del programa para indicar que el plugin se ha cargado.
    bges->setWindowTitle ( "Prueba de plugin e-factura." );

    
    return 0;
}


/// Dibuja el boton de exportar en el formulario de factura a cliente
/**
\param fac
\return
**/
int FacturaView_FacturaView ( FacturaView *fac )
{
    BL_FUNC_DEBUG

    EFQToolButton *mui_exporta_efactura = new EFQToolButton ( fac, fac->mui_plugbotones );
    mui_exporta_efactura->setObjectName ( QString::fromUtf8 ( "exporta" ) );
    mui_exporta_efactura->setMinimumSize ( QSize ( 32, 32 ) );
    mui_exporta_efactura->setIcon ( QIcon ( g_confpr->value( CONF_PROGDATA ) + "efactura/efactura.png" ) );
    mui_exporta_efactura->setIconSize ( QSize ( 22, 22 ) );
    mui_exporta_efactura->setStatusTip ( "Exportar eFactura" );
    mui_exporta_efactura->setToolTip ( "Exportar eFactura" );

    QHBoxLayout *m_hboxLayout1 = fac->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( fac->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 5 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if

    m_hboxLayout1->addWidget ( mui_exporta_efactura );

    

    return 0;
}


/// Dibuja el boton de importar en el listado de facturas de proveedor
/**
\param listfac
\return
**/
int FacturasProveedorList_FacturasProveedorList_Post ( FacturasProveedorList *listfac )
{
    BL_FUNC_DEBUG

    EFQToolButtonImportar *mui_importa_efactura = new EFQToolButtonImportar ( listfac, listfac->mui_plugbotones );
    mui_importa_efactura->setObjectName ( QString::fromUtf8 ( "importa" ) );
    mui_importa_efactura->setMinimumSize ( QSize ( 32, 32 ) );
    mui_importa_efactura->setIcon ( QIcon ( g_confpr->value( CONF_PROGDATA ) + "efactura/efactura.png" ) );
    mui_importa_efactura->setIconSize ( QSize ( 22, 22 ) );
    mui_importa_efactura->setStatusTip ( "Importar eFactura" );
    mui_importa_efactura->setToolTip ( "Importar eFactura" );

    QHBoxLayout *m_hboxLayout1 = listfac->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( listfac->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 5 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    m_hboxLayout1->addWidget ( mui_importa_efactura );

    

    return 0;
}
