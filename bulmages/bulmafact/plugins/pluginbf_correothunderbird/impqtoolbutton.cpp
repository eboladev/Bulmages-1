/***************************************************************************
 *   Copyright(C) 2008 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
 *                                                                         *
 *   Copyright(C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
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

#include "impqtoolbutton.h"
#include "blfunctions.h"
#include "presupuestoview.h"
#include "blfixed.h"
#include "facturaview.h"
#include "bfcompany.h"
#include "bldb.h"


#ifdef Q_OS_WIN32
#define CAD_MOVE QString("move ")
#define CAD_THUNDERBIRD QString("\"C:\\Archivos de programa\\Mozilla Thunderbird\\thunderbird.exe\" ")
#define CAD_COMILLAS QString("\"")
#else
#define CAD_MOVE QString("mv ")
#define CAD_THUNDERBIRD QString("thunderbird ")
#define CAD_COMILLAS("")
#endif

///
/**
\param pres
\param ped
\param alb
\param fac
\param parent
**/
EmailThunderbirdQToolButton::EmailThunderbirdQToolButton ( PresupuestoView *pres, PedidoClienteView *ped, AlbaranClienteView *alb, FacturaView *fac , QWidget *parent ) : QToolButton ( parent )
{
    blDebug ( "EmailThunderbirdQToolButton::EmailThunderbirdQToolButton", 0 );

    m_presupuestoView = pres;
    m_pedidoClienteView = ped;
    m_albaranClienteView = alb;
    m_facturaView = fac;

    setBoton();

    blDebug ( "END EmailThunderbirdQToolButton::EmailThunderbirdQToolButton", 0 );
}

///
/**
**/
EmailThunderbirdQToolButton::~EmailThunderbirdQToolButton()
{
    blDebug ( "EmailThunderbirdQToolButton::~EmailThunderbirdQToolButton", 0 );
    blDebug ( "END EmailThunderbirdQToolButton::~EmailThunderbirdQToolButton", 0 );
}

///
/**
**/
void EmailThunderbirdQToolButton::setBoton()
{
    blDebug ( "EmailThunderbirdQToolButton::setBoton", 0 );

    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );

    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( _ ( "Enviar por e-mail con Thunderbird al cliente" ) );
    setToolTip ( _ ( "Enviar por e-mail con Thunderbird al cliente" ) );
    setMinimumSize ( QSize ( 32, 32 ) );
    setMaximumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( g_confpr->valor ( CONF_PROGDATA ) + "icons/icon_mozilla-thunderbird.xpm"  ) );
    setIconSize ( QSize ( 22, 22 ) );

    blDebug ( "END EmailThunderbirdQToolButton::setBoton", 0 );
}

///
/**
**/
void EmailThunderbirdQToolButton::click()
{
    blDebug ( "EmailThunderbirdQToolButton::click", 0 );

    if ( m_presupuestoView != NULL ) {
    
        m_companyact = m_presupuestoView->mainCompany();
        
        QString id = m_presupuestoView->dbValue ( "idpresupuesto" );
        QString num = m_presupuestoView->dbValue ( "numpresupuesto" );
        QString ref = m_presupuestoView->dbValue ( "refpresupuesto" );
        QString idcliente = m_presupuestoView->dbValue ( "idcliente" );
        QString query = "SELECT mailcliente from cliente WHERE idcliente=" + idcliente;
        
        if (id == "") {
            blMsgInfo("Este presupuesto aún no se ha guardado en la base de datos. Por favor, guárdelo e inténtelo de nuevo.");
            return;
        }
        
        if (idcliente == "") {
            blMsgInfo("No hay seleccionado ningún cliente en el presupuesto. Por favor, selecciónelo e inténtelo de nuevo.");
            return;
        }
        
        BlDbRecordSet *curs = m_companyact->loadQuery ( query );
        QString email = curs->valor ( "mailcliente" );

        if ( m_presupuestoView->generaRML() ) {
            blCreatePDF ( "presupuesto" );

            QString cad = CAD_MOVE + CAD_COMILLAS + g_confpr->valor ( CONF_DIR_USER ) + "presupuesto.pdf" + CAD_COMILLAS +" "+ CAD_COMILLAS + g_confpr->valor ( CONF_DIR_USER ) + "presupuesto" + num + ".pdf"+ CAD_COMILLAS;
#ifdef Q_OS_WIN32
			cad.replace("/","\\");
#endif
            system ( cad.toAscii().data() );

#ifdef Q_OS_WIN32
            cad = CAD_THUNDERBIRD + " -compose to='" + email + "',subject='Presupuesto " + num + "',body='Adjunto remito presupuesto numero " + num + ". Con referencia " + ref + " Atentamente\',attachment='file://" + g_confpr->valor ( CONF_DIR_USER ) + "presupuesto" + num + ".pdf'";
#else
            cad = CAD_THUNDERBIRD + " -compose to='" + email + "',subject='Presupuesto " + num + "',body='Adjunto remito presupuesto numero " + num + ". Con referencia " + ref + "\n\n Atentamente\n',attachment='file://" + g_confpr->valor ( CONF_DIR_USER ) + "presupuesto" + num + ".pdf'";
#endif
            system ( cad.toAscii().data() );
        } // end if
        
    } // end if

    if ( m_pedidoClienteView != NULL ) {
    
        m_companyact = m_pedidoClienteView->mainCompany();

        QString id = m_pedidoClienteView->dbValue ( "idpedidocliente" );
        QString num = m_pedidoClienteView->dbValue ( "numpedidocliente" );
        QString ref = m_pedidoClienteView->dbValue ( "refpedidocliente" );
        QString idcliente = m_pedidoClienteView->dbValue ( "idcliente" );
        QString query = "SELECT mailcliente from cliente WHERE idcliente=" + idcliente;
        
        if (id == "") {
            blMsgInfo("Este pedido aún no se ha guardado en la base de datos. Por favor, guárdelo e inténtelo de nuevo.");
            return;
        }
        
        if (idcliente == "") {
            blMsgInfo("No hay seleccionado ningún cliente en el pedido. Por favor, selecciónelo e inténtelo de nuevo.");
            return;
        }
        
        BlDbRecordSet *curs = m_companyact->loadQuery ( query );
        QString email = curs->valor ( "mailcliente" );

        if ( m_pedidoClienteView->generaRML() ) {
            blCreatePDF ( "pedidocliente" );

            QString cad = CAD_MOVE + CAD_COMILLAS + g_confpr->valor ( CONF_DIR_USER ) + "pedidocliente.pdf"+ CAD_COMILLAS +" " + CAD_COMILLAS + g_confpr->valor ( CONF_DIR_USER ) + "pedidocliente" + num + ".pdf" + CAD_COMILLAS;
#ifdef Q_OS_WIN32
			cad.replace("/","\\");
#endif
            system ( cad.toAscii().data() );

#ifdef Q_OS_WIN32
            cad = CAD_THUNDERBIRD + "  -compose to='" + email + "',subject='Pedido " + num + "',body='Adjunto remito pedido numero " + num + ". Con referencia " + ref + " Atentamente',attachment='file://" + g_confpr->valor ( CONF_DIR_USER ) + "pedidocliente" + num + ".pdf'";
#else
            cad = CAD_THUNDERBIRD + "  -compose to='" + email + "',subject='Pedido " + num + "',body='Adjunto remito pedido numero " + num + ". Con referencia " + ref + "\n\n Atentamente\n',attachment='file://" + g_confpr->valor ( CONF_DIR_USER ) + "pedidocliente" + num + ".pdf'";
#endif
            system ( cad.toAscii().data() );
        } // end if
        
    } // end if

    if ( m_albaranClienteView != NULL ) {
    
        m_companyact = m_albaranClienteView->mainCompany();

        QString id = m_albaranClienteView->dbValue ( "idalbaran" );
        QString num = m_albaranClienteView->dbValue ( "numalbaran" );
        QString ref = m_albaranClienteView->dbValue ( "refalbaran" );
        QString idcliente = m_albaranClienteView->dbValue ( "idcliente" );
        QString query = "SELECT mailcliente from cliente WHERE idcliente=" + idcliente;
        
        if (id == "") {
            blMsgInfo("Este albaran aún no se ha guardado en la base de datos. Por favor, guárdelo e inténtelo de nuevo.");
            return;
        }
        
        if (idcliente == "") {
            blMsgInfo("No hay seleccionado ningún cliente en el albarán. Por favor, selecciónelo e inténtelo de nuevo.");
            return;
        }
        
        BlDbRecordSet *curs = m_companyact->loadQuery ( query );
        QString email = curs->valor ( "mailcliente" );

        if ( m_albaranClienteView->generaRML() ) {
            blCreatePDF ( "albaran" );

            QString cad = CAD_MOVE + CAD_COMILLAS + g_confpr->valor ( CONF_DIR_USER ) + "albaran.pdf"+ CAD_COMILLAS +" " + CAD_COMILLAS + g_confpr->valor ( CONF_DIR_USER ) + "albaran" + num + ".pdf"+ CAD_COMILLAS;
#ifdef Q_OS_WIN32
			cad.replace("/","\\");
#endif
            system ( cad.toAscii().data() );

#ifdef Q_OS_WIN32
            cad = CAD_THUNDERBIRD + "  -compose to='" + email + "',subject='Albaran " + num + "',body='Adjunto remito albaran numero " + num + ". Con referencia " + ref + " Atentamente',attachment='file://" + g_confpr->valor ( CONF_DIR_USER ) + "albaran" + num + ".pdf'";
#else
            cad = CAD_THUNDERBIRD + "  -compose to='" + email + "',subject='Albaran " + num + "',body='Adjunto remito albaran numero " + num + ". Con referencia " + ref + "\n\n Atentamente\n',attachment='file://" + g_confpr->valor ( CONF_DIR_USER ) + "albaran" + num + ".pdf'";
#endif
            system ( cad.toAscii().data() );
        } // end if
        
    } // end if

    if ( m_facturaView != NULL ) {
    
        m_companyact = m_facturaView->mainCompany();

        QString id = m_facturaView->dbValue ( "idfactura" );
        QString num = m_facturaView->dbValue ( "numfactura" );
        QString serie = m_facturaView->dbValue ( "codigoserie_factura" );
        QString ref = m_facturaView->dbValue ( "reffactura" );
        QString fecha = m_facturaView->dbValue ( "ffactura" );
        QString idcliente = m_facturaView->dbValue ( "idcliente" );
        QString query = "SELECT mailcliente from cliente WHERE idcliente=" + idcliente;
        
        if (id == "") {
            blMsgInfo("Este presupuesto aún no se ha guardado en la base de datos. Por favor, guárdelo e inténtelo de nuevo.");
            return;
        }
        
        if (idcliente == "") {
            blMsgInfo("No hay seleccionado ningún cliente en el presupuesto. Por favor, selecciónelo e inténtelo de nuevo.");
            return;
        }
        
        BlDbRecordSet *curs = m_companyact->loadQuery ( query );
        QString email = curs->valor ( "mailcliente" );

        if ( m_facturaView->generaRML() ) {
            blCreatePDF ( "factura" );

            QString cad = CAD_MOVE + CAD_COMILLAS + g_confpr->valor ( CONF_DIR_USER ) + "factura.pdf "  + CAD_COMILLAS +" "+ CAD_COMILLAS + g_confpr->valor ( CONF_DIR_USER ) + "factura" + serie + num + ".pdf" + CAD_COMILLAS;
			cad.replace("/","\\");

            system ( cad.toAscii().data() );
			
#ifdef Q_OS_WIN32
            cad = CAD_THUNDERBIRD + "  -compose to='" + email + "',subject='Factura " + num + "',body='Estimado cliente: Adjunto le enviamos la factura numero " + serie + num + " con fecha " + fecha + " Sin otro particular reciba un cordial saludo.',attachment='file://" + g_confpr->valor ( CONF_DIR_USER ) + "factura" + serie + num + ".pdf'";
#else
            cad = CAD_THUNDERBIRD + "  -compose to='" + email + "',subject='Factura " + num + "',body='Estimado cliente:\n\nAdjunto le enviamos la factura numero " + serie + num + " con fecha " + fecha + "\nSin otro particular reciba un cordial saludo.\n',attachment='file://" + g_confpr->valor ( CONF_DIR_USER ) + "factura" + serie + num + ".pdf'";
#endif
            system ( cad.toAscii().data() );
        } // end if
    } // end if

    blDebug ( "END EmailThunderbirdQToolButton::click", 0 );
}
