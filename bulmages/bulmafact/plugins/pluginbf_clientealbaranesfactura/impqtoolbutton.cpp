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

#include <QtWidgets/QWidget>
#include "impqtoolbutton.h"
#include "blfunctions.h"

/// Necesarios para importacion de efactura
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNode>
#include <QtCore/QString>
#include <QtWidgets/QFileDialog>
#include <QtCore/QMap>
#include <QtCore/QList>

#include "facturapview.h"
#include "blfixed.h"
#include "presupuestolist.h"
#include "presupuestoview.h"
#include "pedidosclientelist.h"
#include "pedidoclienteview.h"
#include "albaranclienteview.h"
#include "albaranclientelist.h"
#include "facturaview.h"
#include "bfcompany.h"
#include "bldb.h"


///
/**
\param pres
\param ped
\param alb
\param fac
\param parent
**/
ImpQToolButton::ImpQToolButton ( PresupuestoList *pres, PedidosClienteList *ped, AlbaranClienteList *alb,  FacturasList *fac , QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
    m_presupuestoList = pres;
    m_pedidosClienteList = ped;
    m_albaranClienteList = alb;
    m_facturasList = fac;

    setBoton();
    
}


///
/**
**/
ImpQToolButton::~ImpQToolButton()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void ImpQToolButton::setBoton()
{
    BL_FUNC_DEBUG
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta1" ) );
    setStatusTip ( "Facturar elementos seleccionados" );
    setToolTip ( "Facturar elementos seleccionados" );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( QString::fromUtf8 ( ":/BfBulmaFact48x48/images/png/48x48/i_bulmages_ventas_facturas.48x48.png" ) ) );
    setIconSize ( QSize ( 22, 22 ) );
    
}


///
/**
**/
void ImpQToolButton::click()
{
    BL_FUNC_DEBUG

    QString res = "";

    if ( m_albaranClienteList != NULL ) {
        m_companyact = ( BfCompany * ) m_albaranClienteList->mainCompany();
        BlSubForm *sub = m_albaranClienteList->mui_list;

        /// Creamos la factura
        /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
        int resur = g_plugins->run ( "SNewFacturaView", m_companyact );
        if ( !resur ) {
            blMsgInfo ( "no se pudo crear instancia de factura" );
            return;
        } // end if
        FacturaView *fac = ( FacturaView * ) g_plugParams;
        m_companyact->m_pWorkspace->addSubWindow ( fac );

        /// Cargamos un elemento que no existe para inicializar bien la clase.
        fac->load ( "0" );

        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "true" ) {
                QString id = rec->dbValue ( "idalbaran" );

                /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
                int resur = g_plugins->run ( "SNewAlbaranClienteView", m_companyact );
                if ( !resur ) {
                    blMsgInfo ( "no se pudo crear instancia de albaran" );
                    return;
                } // end if
                AlbaranClienteView *pres = ( AlbaranClienteView * ) g_plugParams;
                pres->load ( id );
                if ( pres->mui_procesadoalbaran->isChecked() ) {
                    pres->close();
                    return;
                } // end if
                m_companyact->pWorkspace() ->addSubWindow ( pres );
                m_companyact->insertWindow ( pres->windowTitle(), pres, false );
                pres->show();

                // El calculo de descuentos es complejo
                BlFixed descgen1 = pres->getlistadescuentos() ->sumarCampo ( "proporciondalbaran" );
                BlFixed descgen = BlFixed ( "1.000" ) - descgen1 / BlFixed ( "100" );

                /// Agregamos a comentarios que albaran se corresponde.
                QString comm = fac->dbValue ( "comentfactura" ) + "(" + _ ( "ALBARAN: Num " ) + pres->dbValue ( "numalbaran" ) + _ ( "Ref:" ) + " " + pres->dbValue ( "refalbaran" ) + _ ( "Fecha:" ) + " " + pres->dbValue ( "fechaalbaran" ) + ")\n";
                fac->setDbValue ( "comentfactura", comm );
                fac->setDbValue ( "idforma_pago", pres->dbValue ( "idforma_pago" ) );
                fac->setDbValue ( "reffactura", pres->dbValue ( "refalbaran" ) );
                fac->setDbValue ( "idcliente", pres->dbValue ( "idcliente" ) );
                fac->setDbValue ( "idalmacen", pres->dbValue ( "idalmacen" ) );

                QString l;
                BlDbSubFormRecord *linea, *linea1;
                for ( int i = 0; i < pres->getlistalineas() ->rowCount(); ++i ) {
                    linea = pres->getlistalineas() ->lineaat ( i );
                    if ( linea->dbValue ( "idarticulo" ) != "" ) {
                        linea1 = fac->getlistalineas() ->lineaat ( fac->getlistalineas() ->rowCount() - 1 );
                        /// Haciendo el nuevo registro antes nos evitamos problemas de foco.
                        fac->getlistalineas() ->newRecord();
                        fac->getlistalineas() ->setProcesarCambios ( false );
                        linea1->setDbValue ( "codigocompletoarticulo", linea->dbValue ( "codigocompletoarticulo" ) );
                        linea1->setDbValue ( "desclfactura", linea->dbValue ( "desclalbaran" ) );
                        linea1->setDbValue ( "cantlfactura", linea->dbValue ( "cantlalbaran" ) );
                        linea1->setDbValue ( "pvplfactura", linea->dbValue ( "pvplalbaran" ) );
                        linea1->setDbValue ( "ivalfactura", linea->dbValue ( "ivalalbaran" ) );
                        BlFixed descpar1 = BlFixed ( linea->dbValue ( "descuentolalbaran" ) );
                        BlFixed descpar = BlFixed ( "1.000" ) - descpar1 / BlFixed ( "100" );
                        BlFixed desc = ( BlFixed ( "1.00" ) - descpar * descgen ) * BlFixed ( "100.00" );
                        linea1->setDbValue ( "descuentolfactura", desc.toQString ( '.' ) );
                        linea1->setDbValue ( "idarticulo", linea->dbValue ( "idarticulo" ) );
                        linea1->setDbValue ( "nomarticulo", linea->dbValue ( "nomarticulo" ) );
                        fac->getlistalineas() ->setProcesarCambios ( true );
                        linea1->refresh();
                    } // end if
                } // end for

                pres->mui_procesadoalbaran->setChecked ( true );
            } // end if
        } // end for

        fac->calculaypintatotales();
        fac->pintar();
        fac->show();

    } // end if

    
}

