/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
 *   Copyright (C) 2012 by Daniel Ripoll Osma.                             *
 *   info@danielripoll.es                                                  *
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

#include "impqtoolbutton.h"
#include "blfunctions.h"
/// Necesarios para importacion de efactura
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNode>
#include <QString>
#include <QFileDialog>
#include <QMap>
#include <QList>

#include "presupuestoview.h"
#include "blfixed.h"
#include "facturaview.h"
#include "bfcompany.h"
#include "bldb.h"

#include "pluginbf_correo.h"

///
/**
\param pres
\param ped
\param alb
\param fac
\param parent
**/
EmailQToolButton::EmailQToolButton ( PresupuestoView *pres, PedidoClienteView *ped, AlbaranClienteView *alb, FacturaView *fac , QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
    m_presupuestoView = pres;
    m_pedidoClienteView = ped;
    m_albaranClienteView = alb;
    m_facturaView = fac;
    setBoton();
    
}


///
/**
**/
EmailQToolButton::~EmailQToolButton()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void EmailQToolButton::setBoton()
{
    BL_FUNC_DEBUG
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( _ ( "Enviar por e-mail al cliente" ) );
    setToolTip ( _ ( "Enviar por e-mail al cliente" ) );
    setMinimumSize ( QSize ( 32, 32 ) );
    setMaximumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/mail-send.png" ) ) );
    setIconSize ( QSize ( 22, 22 ) );
    
}


///
/**
\ void
**/
void EmailQToolButton::click()
{
    BL_FUNC_DEBUG

    QString dir_email = g_confpr->value( CONF_EMAIL_CLIENT );
    if (  dir_email == "") {
        LaunchChoseMailer();
        } // end if
        
    QString bcc = "";
    QString body = "";
    QString subject = "";
    QString email = "";
    QString attached = "";

    if ( m_presupuestoView != NULL ) {
        m_companyact = m_presupuestoView->mainCompany();


        QString id = m_presupuestoView->dbValue ( "idpresupuesto" );
        QString num = m_presupuestoView->dbValue ( "numpresupuesto" );
        QString ref = m_presupuestoView->dbValue ( "refpresupuesto" );
        QString idcliente = m_presupuestoView->dbValue ( "idcliente" );
        QString query = "SELECT mailcliente from cliente WHERE idcliente=" + idcliente;
        BlDbRecordSet *curs = m_companyact->loadQuery ( query );
        email = curs->value( "mailcliente" );
        delete curs;


        if ( m_presupuestoView->generateRML() ) {
            blCreatePDF ( "presupuesto" );

            QString oldName =  g_confpr->value( CONF_DIR_USER ) + "presupuesto.pdf";
            QString newName = g_confpr->value( CONF_DIR_USER ) + "presupuesto" + num + ".pdf";
            blMoveFile(oldName, newName);
           
            subject = _("Presupuesto ") + num;
            body = _("Adjunto le enviamos el presupuesto numero ") + num + _(" con referencia ") + ref +"\n";
            body += _("Atentamente\n\n\n");
            attached = g_confpr->value( CONF_DIR_USER ) + "presupuesto" + num + ".pdf";

            bfSendEmail( email, bcc, subject, body, attached );
        } // end if
    } // end if

    if ( m_pedidoClienteView != NULL ) {
        m_companyact = m_pedidoClienteView->mainCompany();


        QString id = m_pedidoClienteView->dbValue ( "idpedidocliente" );
        QString num = m_pedidoClienteView->dbValue ( "numpedidocliente" );
        QString ref = m_pedidoClienteView->dbValue ( "refpedidocliente" );

        QString idcliente = m_pedidoClienteView->dbValue ( "idcliente" );
        QString query = "SELECT mailcliente from cliente WHERE idcliente=" + idcliente;
        BlDbRecordSet *curs = m_companyact->loadQuery ( query );
        email = curs->value( "mailcliente" );
        delete curs;


        if ( m_pedidoClienteView->generateRML() ) {
            blCreatePDF ( "pedidocliente" );
          
            QString oldName =  g_confpr->value( CONF_DIR_USER ) + "pedidocliente.pdf";
            QString newName = g_confpr->value( CONF_DIR_USER ) + "pedidocliente" + num + ".pdf";
            blMoveFile(oldName, newName);
                        
            QString subject = _("Pedido ") + num;
            QString body = _("Adjunto le enviamos el pedido numero ") + num + _(" con referencia ") + ref +"\n";
            body += _("Atentamente\n\n\n");
            QString attached = g_confpr->value( CONF_DIR_USER ) + "pedidocliente" + num + ".pdf";

            bfSendEmail( email, bcc, subject, body, attached );
        } // end if
    } // end if


    if ( m_albaranClienteView != NULL ) {
        m_companyact = m_albaranClienteView->mainCompany();


        QString id = m_albaranClienteView->dbValue ( "idalbaran" );
        QString num = m_albaranClienteView->dbValue ( "numalbaran" );
        QString ref = m_albaranClienteView->dbValue ( "refalbaran" );

        QString idcliente = m_albaranClienteView->dbValue ( "idcliente" );
        QString query = "SELECT mailcliente from cliente WHERE idcliente=" + idcliente;
        BlDbRecordSet *curs = m_companyact->loadQuery ( query );
        email = curs->value( "mailcliente" );
        delete curs;


        if ( m_albaranClienteView->generateRML() ) {
            blCreatePDF ( "albaran" );
            
            QString oldName =  g_confpr->value( CONF_DIR_USER ) + "albaran.pdf";
            QString newName = g_confpr->value( CONF_DIR_USER ) + "albaran" + num + ".pdf";
            blMoveFile(oldName, newName);

            subject = _("Albaran ") + num;
            body = _("Adjunto le enviamos el albaran numero ") + num + _(" con referencia ") + ref +"\n";
            body += _( "Atentamente\n\n\n");
            attached = g_confpr->value( CONF_DIR_USER ) + "albaran" + num + ".pdf";

            bfSendEmail( email, bcc, subject, body, attached );
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
        BlDbRecordSet *curs = m_companyact->loadQuery ( query );
        email = curs->value( "mailcliente" );
        delete curs;

        if ( m_facturaView->generateRML() ) {
            blCreatePDF ( "factura" );

            QString oldName =  g_confpr->value( CONF_DIR_USER ) + "factura.pdf";
            QString newName = g_confpr->value( CONF_DIR_USER ) + "factura" + serie + num + ".pdf";
            blMoveFile(oldName, newName);
           
            subject = _("Factura ") + num;
            body = _("Adjunto le enviamos la factura numero ") + serie + num + _(" con fecha ") + fecha;
            body += _("Sin otro particular, reciba un cordial saludo\n\n\n");
            attached = g_confpr->value( CONF_DIR_USER ) + "factura" + serie + num + ".pdf";
            

            bfSendEmail( email, bcc, subject, body, attached );
        } // end if
    } // end if

    
}


