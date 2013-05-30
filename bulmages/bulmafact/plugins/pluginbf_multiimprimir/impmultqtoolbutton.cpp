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
/// Necesarios para importacion de efactura
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNode>
#include <QtCore/QString>
#include <QtWidgets/QFileDialog>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QTextStream>

#include "blfunctions.h"
#include "blfixed.h"
#include "presupuestolist.h"
#include "presupuestoview.h"
#include "pedidosclientelist.h"
#include "pedidoclienteview.h"
#include "albaranclienteview.h"
#include "albaranclientelist.h"
#include "facturaview.h"
#include "cobroview.h"
#include "bfcompany.h"
#include "bldb.h"
#include "impmultqtoolbutton.h"


///
/**
\param pres
\param ped
\param alb
\param fac
\param cob
\param parent
\param
**/
ImpQToolButton::ImpQToolButton ( PresupuestoList *pres, PedidosClienteList *ped, AlbaranClienteList *alb,  FacturasList *fac, CobrosList *cob , QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
    m_presupuestoList = pres;
    m_pedidosClienteList = ped;
    m_albaranClienteList = alb;
    m_facturasList = fac;
    m_cobrosList = cob;

    m_listado = (BlFormList *) parent->parent()->parent();

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
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( _ ( "Imprimir elementos seleccionados" ) );
    setToolTip ( _ ( "Imprimir elementos seleccionados" ) );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( QString::fromUtf8 ( ":/Genericos32x32/images/png/i_print1.png" ) ) );
    setIconSize ( QSize ( 22, 22 ) );
    
}


///
/**
\return
**/
void ImpQToolButton::click()
{
    BL_FUNC_DEBUG

    BlDbRecordSet *cur1 = NULL;
    BlDbRecordSet *cur = NULL;

    try {

        QString res = "";

        if ( m_presupuestoList != NULL ) {
            m_companyact = ( BfCompany * ) m_presupuestoList->mainCompany();
            BlSubForm *sub = m_presupuestoList->mui_list;

            /// Reseteamos los valores
            for ( int i = 0; i < sub->rowCount(); i++ ) {
                BlDbSubFormRecord *rec = sub->lineaat ( i );
                rec->refresh();
                QString val = rec->dbValue ( "selector" );
                if ( val == "true" ) {
                    QString id = rec->dbValue ( "idpresupuesto" );
                    /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
                    int resur = g_plugins->run ( "SNewPresupuestoView", m_companyact );
                    if ( !resur ) {
                        blMsgInfo ( "no se pudo crear instancia de presupuesto" );
                        return;
                    } // end if
                    PresupuestoView *pres = ( PresupuestoView * ) g_plugParams;
                    pres->load ( id );

                    if ( pres->generateRML() ) {


                        blCreatePDF ( "presupuesto" );

                        QString oldName = g_confpr->value( CONF_DIR_USER ) + "presupuesto.pdf";
                        QString newName = g_confpr->value( CONF_DIR_USER ) + "presupuesto" + id +".pdf";
                        blMoveFile(oldName, newName);
                        
                        res += g_confpr->value( CONF_DIR_USER ) + "presupuesto" + id + ".pdf ";
                    } // end if
                    pres->close();
                } // end if
            } // end for
        }

        if ( m_facturasList != NULL ) {
            m_companyact = ( BfCompany * ) m_facturasList->mainCompany();
            BlSubForm *sub = m_facturasList->mui_list;

            /// Reseteamos los valores
            for ( int i = 0; i < sub->rowCount(); i++ ) {
                BlDbSubFormRecord *rec = sub->lineaat ( i );
                rec->refresh();
                QString val = rec->dbValue ( "selector" );
                if ( val == "true" ) {
                    QString id = rec->dbValue ( "idfactura" );
		    QString num = rec->dbValue ( "numfactura" );
		    QString serie = rec->dbValue ( "codigoserie_factura" );


                    /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
                    int resur = g_plugins->run ( "SNewFacturaView", m_companyact );
                    if ( !resur ) {
                        blMsgInfo ( _ ( "no se pudo crear instancia de factura" ) );
                        return;
                    } // end if
                    FacturaView *pres = ( FacturaView * ) g_plugParams;

                    pres->load ( id );

                    if ( pres->generateRML() ) {
                        blCreatePDF ( "factura" );
                        QString oldName =  g_confpr->value( CONF_DIR_USER ) + "factura.pdf";
                        QString newName = g_confpr->value( CONF_DIR_USER ) + "factura" + serie + num  + ".pdf";
                        blMoveFile(oldName, newName);
                        
                        res += g_confpr->value( CONF_DIR_USER ) + "factura" + serie + num + ".pdf ";
                    } // end if
                    pres->close();
                } // end if
            } // end for
        } // end if

        if ( m_pedidosClienteList != NULL ) {
            m_companyact = ( BfCompany * ) m_pedidosClienteList->mainCompany();
            BlSubForm *sub = m_pedidosClienteList->mui_list;

            /// Reseteamos los valores
            for ( int i = 0; i < sub->rowCount(); i++ ) {
                BlDbSubFormRecord *rec = sub->lineaat ( i );
                rec->refresh();
                QString val = rec->dbValue ( "selector" );
                if ( val == "true" ) {
                    QString id = rec->dbValue ( "idpedidocliente" );

                    /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
                    int resur = g_plugins->run ( "SNewPedidoClienteView", m_companyact );
                    if ( !resur ) {
                        blMsgInfo ( _ ( "no se pudo crear instancia de pedido cliente" ) );
                        return;
                    } // end if
                    PedidoClienteView *pres = ( PedidoClienteView * ) g_plugParams;
                    pres->load ( id );

                    if ( pres->generateRML() ) {


                        blCreatePDF ( "pedidocliente" );
                        
                        QString oldName =  g_confpr->value( CONF_DIR_USER ) + "pedidocliente.pdf";
                        QString newName = g_confpr->value( CONF_DIR_USER ) + "pedidocliente" + id + ".pdf";
                        blMoveFile(oldName, newName);
                        
                        res += g_confpr->value( CONF_DIR_USER ) + "pedidocliente" + id + ".pdf ";
                    } // end if
                    pres->close();
                } // end if
            } // end for
        } // end if


        if ( m_albaranClienteList != NULL ) {
            m_companyact = ( BfCompany * ) m_albaranClienteList->mainCompany();
            BlSubForm *sub = m_albaranClienteList->mui_list;

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
                        blMsgInfo ( _ ( "no se pudo crear instancia de albaran" ) );
                        return;
                    } // end if
                    AlbaranClienteView *pres = ( AlbaranClienteView * ) g_plugParams;
                    pres->load ( id );

                    if ( pres->generateRML() ) {


                        blCreatePDF ( "albaran" );
                        
                        QString oldName =  g_confpr->value( CONF_DIR_USER ) + "albaran.pdf";
                        QString newName = g_confpr->value( CONF_DIR_USER ) + "albaran" + id + ".pdf";
                        blMoveFile(oldName, newName);
                        
                        res += g_confpr->value( CONF_DIR_USER ) + "albaran" + id + ".pdf ";
                    } // end if
                    pres->close();
                } // end if
            } // end for
        } // end if

        /// TRATAMOS LOS COBROS CUYO LISTADO ES ESPECIAL.
        if ( m_cobrosList != NULL ) {

            m_companyact = ( BfCompany * ) m_cobrosList->mainCompany();
            BlSubForm *sub = m_cobrosList->mui_list;
            QString txt = "";

            /// Copiamos el archivo.
            QString archivo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "recibos.rml";
            QString archivod = g_confpr->value( CONF_DIR_USER ) + "recibos.rml";
            blCopyFile(archivo,archivod);

            /// Copiamos el logo
            QString archivologo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "logo.jpg";
            QString logousuario = g_confpr->value( CONF_DIR_USER ) + "logo.jpg";
            blCopyFile(archivologo,logousuario);
            
            QFile file;
            file.setFileName ( archivod );
            file.open ( QIODevice::ReadOnly );
            QTextStream stream ( &file );
            QString buff = stream.readAll();
            file.close();



            /// Reseteamos los valores
            int j = 0;
            for ( int i = 0; i < sub->rowCount(); i++ ) {

                BlDbSubFormRecord *rec = sub->lineaat ( i );
                rec->refresh();
                QString val = rec->dbValue ( "selector" );

                if ( val == "true" ) {
                    QString id = rec->dbValue ( "idcobro" );
                    /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
                    int resur = g_plugins->run ( "SNewCobroView", m_companyact );
                    if ( !resur ) {
                        blMsgInfo ( "no se pudo crear instancia de cobro" );
                        return;
                    } // end if
                    CobroView *pres = ( CobroView * ) g_plugParams;
                    pres->load ( id );


                    int col1 = j % 3;
                    double col = 0;
                    if ( col1 == 2 ) col = 1.00;
                    if ( col1 == 1 ) col = 10;
                    if ( col1 == 0 ) col = 19.5;

                    txt += " <storyPlace x=\"0cm\" y=\"0cm\" width=\"15cm\" height=\"1cm\">\n";
                    txt += " <setFont name=\"Courier\" size=\"8\"/>\n";
                    txt += " <drawString x=\"4.7cm\" y=\"" + QString::number ( col + 7.6 ) + "cm\">" + pres->dbValue ( "idcobro" ) + "</drawString>\n";
                    txt += " <drawString x=\"7.2cm\" y=\"" + QString::number ( col + 7.6 ) + "cm\">PALMA DE MALLORCA</drawString>\n";
                    txt += " <drawString x=\"14.2cm\" y=\"" + QString::number ( col + 7.6 ) + "cm\">EUROS " + pres->dbValue ( "cantcobro" ) + "</drawString>\n";

                    txt += " <drawString x=\"5.7cm\" y=\"" + QString::number ( col + 6.8 ) + "cm\">" + pres->dbValue ( "fechacobro" ) + "</drawString>\n";
                    txt += " <drawString x=\"12.4cm\" y=\"" + QString::number ( col + 6.8 ) + "cm\">" + pres->dbValue ( "fechavenccobro" ) + "</drawString>\n";

                    if ( pres->dbValue ( "idbanco" ) != "" ) {
                        QString query = "SELECT * FROM banco WHERE idbanco =" + pres->dbValue ( "idbanco" );
                        cur1 = m_companyact->loadQuery ( query );
                        if ( !cur1->eof() ) {
                            txt += " <drawString x=\"9.4cm\" y=\"" + QString::number ( col + 4.4 ) + "cm\">" + cur1->value( "nombanco" ) + "</drawString>\n";
                            txt += " <drawString x=\"9.4cm\" y=\"" + QString::number ( col + 4 ) + "cm\">" + cur1->value( "pobbanco" ) + "</drawString>\n";
                            txt += " <drawString x=\"12.4cm\" y=\"" + QString::number ( col + 3.5 ) + "cm\">" + cur1->value( "codentidadbanco" ) + " " + cur1->value( "codagenciabanco" ) + " " + cur1->value( "dcbanco" ) + " " + cur1->value( "numcuentabanco" ) + "</drawString>\n";
                        } // end if
                        delete cur1;
                    } // end if


                    if ( pres->dbValue ( "idcliente" ) != "" ) {
                        QString query = "SELECT * FROM cliente WHERE idcliente =" + pres->dbValue ( "idcliente" );
                        cur = m_companyact->loadQuery ( query );
                        if ( !cur->eof() ) {
                            txt += " <drawString x=\"4.8cm\" y=\"" + QString::number ( col + 2.3 ) + "cm\">" + cur->value( "nomcliente" ) + "</drawString>\n";
                            txt += " <drawString x=\"4.8cm\" y=\"" + QString::number ( col + 1.5 ) + "cm\">" + cur->value( "dircliente" ) + "</drawString>\n";
                            txt += " <drawString x=\"4.8cm\" y=\"" + QString::number ( col + 1.1 ) + "cm\">" + cur->value( "cpcliente" ) + "</drawString>\n";
                        } // end if
                        delete cur;
                    } // end if

                    txt += " <drawString x=\"4.8cm\" y=\"" + QString::number ( col + 5.6 ) + "cm\">a</drawString>\n";
                    txt += " <drawString x=\"4.8cm\" y=\"" + QString::number ( col + 5.2 ) + "cm\">" + blNumberToText ( pres->dbValue ( "cantcobro" ) ) + "</drawString>\n";

                    txt += " </storyPlace>\n";

                    if ( col1 == 2 )
                        txt += "<nextPage/><nextFrame/>\n";
                    j++;
                    delete pres;

                } // end if

            } // end for


            buff.replace ( "[story]", txt );

            res = g_confpr->value( CONF_DIR_USER ) + "recibos.pdf ";


            if ( file.open ( QIODevice::WriteOnly ) ) {
                QTextStream stream ( &file );
                stream << buff;
                file.close();
            } // end if


            blCreateAndLoadPDF ( "recibos" );

            return;
        } // end if


        if ( m_listado->objectName() == "RecibosListBase" ) {
            m_companyact = ( BfCompany * ) m_listado->mainCompany();
            BlSubForm *sub = m_listado->subForm();

            /// Reseteamos los valores
            for ( int i = 0; i < sub->rowCount(); i++ ) {
                BlDbSubFormRecord *rec = sub->lineaat ( i );
                rec->refresh();
                QString val = rec->dbValue ( "selector" );
                if ( val == "true" ) {
                    QString id = rec->dbValue ( "idrecibo" );

                    /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
                    int resur = g_plugins->run ( "SNewReciboView", m_companyact );
                    if ( !resur ) {
                        blMsgInfo ( _ ( "no se pudo crear instancia de pedido cliente" ) );
                        return;
                    } // end if
                    ReciboView *pres = ( ReciboView * ) g_plugParams;
                    pres->load ( id );

                    if ( pres->generateRML() ) {


                        blCreatePDF ( "recibo" );

                        QString oldName =  g_confpr->value( CONF_DIR_USER ) + "recibo.pdf";
                        QString newName = g_confpr->value( CONF_DIR_USER ) + "recibo" + id + ".pdf";
                        blMoveFile(oldName, newName);
                        
                        res += g_confpr->value( CONF_DIR_USER ) + "recibo" + id + ".pdf ";
                    } // end if
                    pres->close();
                } // end if
            } // end for
        } // end if

	
#ifndef Q_OS_WIN32
	QString comando = "";
	if ((g_confpr->value(CONF_CUPS_DEFAULT_PRINTER).isEmpty() || g_confpr->value(CONF_CUPS_DEFAULT_PRINTER).toLower() == "none")) {
	  comando = "lpr " + res;
	} else {
	  comando = "lp -d" +g_confpr->value(CONF_CUPS_DEFAULT_PRINTER) + " " + res;
	} // end if
	system ( comando.toLatin1().data() );
	comando = "rm " + res;
	system ( comando.toLatin1().data() );
#else
	/// Estaria bien imprimir directamente en la impresora, pero no hay forma de hacerlo en Windows.
	QString comando = "start " + g_confpr->value( CONF_PDF ) + " " + res;
	system ( comando.toLatin1().data() );
#endif
        

    } catch ( ... ) {
        blMsgInfo ( "Error en los calculos" );
        /// Si ha quedado memoria reservada la liberamos.
        if ( cur1 ) delete cur1;
        if ( cur ) delete cur;
    } // end try
}


///
/**
\param pres
\param ped
\param alb
\param fac
\param cob
\param parent
**/
SelQToolButton::SelQToolButton ( PresupuestoList *pres, PedidosClienteList *ped, AlbaranClienteList *alb,  FacturasList *fac , CobrosList *cob, QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
    m_presupuestoList = pres;
    m_pedidosClienteList = ped;
    m_albaranClienteList = alb;
    m_facturasList = fac;
    m_cobrosList = cob;
    m_listado = (BlFormList *) parent->parent()->parent();
    setBoton();
    
}


///
/**
**/
SelQToolButton::~SelQToolButton()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void SelQToolButton::setBoton()
{
    BL_FUNC_DEBUG
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( _ ( "Invertir seleccion de elementos" ) );
    setToolTip ( _ ( "Invertir seleccion de elementos" ) );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/selection-invert.png" ) ) );
    setIconSize ( QSize ( 22, 22 ) );

    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    
}

///
/**
**/
void SelQToolButton::click()
{
    BL_FUNC_DEBUG

    // Es posible que esto se haya cargado antes de cargar el company por eso
    // No me fio de que la asignacion en el constructor haya ido bien y reasigno aqui


    QString res = "";


    if ( m_listado != NULL ) {
        m_companyact = ( BfCompany * ) m_listado->mainCompany();
        BlSubForm *sub = m_listado->subForm();
        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );
            rec->refresh();
            if ( rec->dbValue ( "selector" ) == "true" ) {
                rec->setDbValue ( "selector", "false" );
            } else {
                rec->setDbValue ( "selector", "true" );
            } // end if
        } // end for
    }

    
}

// ==================================================================================


///
/**
\param pres
\param ped
\param alb
\param fac
\param cob
\param parent
**/
EmailQToolButton::EmailQToolButton ( PresupuestoList *pres, PedidosClienteList *ped, AlbaranClienteList *alb,  FacturasList *fac, CobrosList *cob , QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
    m_presupuestoList = pres;
    m_pedidosClienteList = ped;
    m_albaranClienteList = alb;
    m_facturasList = fac;
    m_cobrosList = cob;

    m_listado = (BlFormList *) parent->parent()->parent();

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
    setStatusTip ( _ ( "Enviar elementos seleccionados por e-mail" ) );
    setToolTip ( _ ( "Enviar elementos seleccionados por e-mail" ) );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/mail-send.png" ) ) );
    setIconSize ( QSize ( 22, 22 ) );
    
}


///
/**
**/
void EmailQToolButton::click()
{
    BL_FUNC_DEBUG

    if ( m_presupuestoList != NULL ) {
        m_companyact = ( BfCompany * ) m_presupuestoList->mainCompany();
        BlSubForm *sub = m_presupuestoList->mui_list;

        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "true" ) {
                QString id = rec->dbValue ( "idpresupuesto" );
		QString num = rec->dbValue ( "numpresupuesto" );
		QString ref = rec->dbValue ("refpresupuesto" );
                QString idcliente = rec->dbValue ( "idcliente" );
                QString query = "SELECT mailcliente from cliente WHERE idcliente=" + idcliente;
                BlDbRecordSet *curs = m_companyact->loadQuery ( query );
                QString email = curs->value( "mailcliente" );

                /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
                int resur = g_plugins->run ( "SNewPresupuestoView", m_companyact );
                if ( !resur ) {
                    blMsgInfo ( _ ( "no se pudo crear instancia de presupuesto" ) );
                    return;
                } // end if
                PresupuestoView *pres = ( PresupuestoView * ) g_plugParams;
                pres->load ( id );

                if ( pres->generateRML() ) {


                    blCreatePDF ( "presupuesto" );
                    
                    QString oldName =  g_confpr->value( CONF_DIR_USER ) + "presupuesto.pdf";
                    QString newName = g_confpr->value( CONF_DIR_USER ) + "presupuesto" + num + ".pdf";
                    blMoveFile(oldName, newName);
    
		    QString subject = _("Presupuesto ") + num;
		    QString body = _("Adjunto le enviamos el presupuesto numero ") + num + _(" con referencia ") + ref +"\n";
		    body += _("Atentamente\n\n\n\"");
		    QString attached = g_confpr->value( CONF_DIR_USER ) + "presupuesto" + num + ".pdf";
		    QString bcc= "";
		    
		    blSendEmail( email, bcc, subject, body, attached );
		    
                } // end if
                pres->close();
            } // end if
        } // end for
    } // end if



    if ( m_pedidosClienteList != NULL ) {
        m_companyact = ( BfCompany * ) m_pedidosClienteList->mainCompany();
        BlSubForm *sub = m_pedidosClienteList->mui_list;

        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "true" ) {
                QString id = rec->dbValue ( "idpedidocliente" );
                QString idcliente = rec->dbValue ( "idcliente" );
		QString num = rec->dbValue ( "numpedidocliente" );
		QString ref = rec->dbValue ( "refpedidocliente" );
                QString query = "SELECT mailcliente from cliente WHERE idcliente=" + idcliente;
                BlDbRecordSet *curs = m_companyact->loadQuery ( query );
                QString email = curs->value( "mailcliente" );

                /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
                int resur = g_plugins->run ( "SNewPedidoClienteView", m_companyact );
                if ( !resur ) {
                    blMsgInfo ( _ ( "no se pudo crear instancia de pedido cliente" ) );
                    return;
                } // end if
                PedidoClienteView *pres = ( PedidoClienteView * ) g_plugParams;
                pres->load ( id );

                if ( pres->generateRML() ) {


                    blCreatePDF ( "pedidocliente" );
                   
                    QString oldName =  g_confpr->value( CONF_DIR_USER ) + "pedidocliente.pdf";
                    QString newName = g_confpr->value( CONF_DIR_USER ) + "pedidocliente" + num + ".pdf";
                    blMoveFile(oldName, newName);
		    
		    QString subject = _("Pedido ") + num;
		    QString body = _("Adjunto le enviamos el pedido numero ") + num + _(" con referencia ") + ref +"\n";
		    body += _("Atentamente\n\n\n\"");
		    QString attached = g_confpr->value( CONF_DIR_USER ) + "pedidocliente" + num + ".pdf";
		    QString bcc= "";
		
		    blSendEmail( email, bcc, subject, body, attached );
		    
                } // end if
                pres->close();
            } // end if
        } // end for
    } // end if




    if ( m_albaranClienteList != NULL ) {
        m_companyact = ( BfCompany * ) m_albaranClienteList->mainCompany();
        BlSubForm *sub = m_albaranClienteList->mui_list;

        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "true" ) {
                QString id = rec->dbValue ( "idalbaran" );
		QString num = rec->dbValue ( "numalbaran" );
		QString ref = rec->dbValue ( "refalbaran" );
		
                QString idcliente = rec->dbValue ( "idcliente" );
                QString query = "SELECT mailcliente from cliente WHERE idcliente=" + idcliente;
                BlDbRecordSet *curs = m_companyact->loadQuery ( query );
                QString email = curs->value( "mailcliente" );

                /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
                int resur = g_plugins->run ( "SNewPedidoClienteView", m_companyact );
                if ( !resur ) {
                    blMsgInfo ( _ ( "no se pudo crear instancia de pedido cliente" ) );
                    return;
                } // end if
                PedidoClienteView *pres = ( PedidoClienteView * ) g_plugParams;                pres->load ( id );

                if ( pres->generateRML() ) {


                    blCreatePDF ( "albaran" );

                    QString oldName =  g_confpr->value( CONF_DIR_USER ) + "albaran.pdf";
                    QString newName = g_confpr->value( CONF_DIR_USER ) + "albaran" + num + ".pdf";
                    blMoveFile(oldName, newName);

		    
		    QString subject = _("Albaran ") + num;
		    QString body = _("Adjunto le enviamos el albaran numero ") + num + _(" con referencia ") + ref +"\n";
		    body += _( "Atentamente\n\n\n\"");
		    QString attached = g_confpr->value( CONF_DIR_USER ) + "albaran" + num + ".pdf";
		    QString bcc= "";
		    
		    blSendEmail( email, bcc, subject, body, attached );
		    
                } // end if
                pres->close();
            } // end if
        } // end for
    } // end if




    if ( m_facturasList != NULL ) {
        m_companyact = ( BfCompany * ) m_facturasList->mainCompany();
        BlSubForm *sub = m_facturasList->mui_list;

        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "true" ) {
                QString id = rec->dbValue ( "idfactura" );
                QString idcliente = rec->dbValue ( "idcliente" );
                QString num = rec->dbValue ( "numfactura" );
                QString serie = rec->dbValue ( "codigoserie_factura" );
                QString ref = rec->dbValue ( "reffactura" );
                QString fecha = rec->dbValue ( "ffactura" );

                QString query = "SELECT mailcliente from cliente WHERE idcliente=" + idcliente;
                BlDbRecordSet *curs = m_companyact->loadQuery ( query );
                QString email = curs->value( "mailcliente" );

                /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
                int resur = g_plugins->run ( "SNewFacturaView", m_companyact );
                if ( !resur ) {
                    blMsgInfo ( _ ( "no se pudo crear instancia de factura" ) );
                    return;
                } // end if
                FacturaView *pres = ( FacturaView * ) g_plugParams;
                pres->load ( id );

                if ( pres->generateRML() ) {


                    blCreatePDF ( "factura" );
                    
                    QString oldName =  g_confpr->value( CONF_DIR_USER ) + "factura.pdf";
                    QString newName = g_confpr->value( CONF_DIR_USER ) + "factura" + serie + num + ".pdf";
                    blMoveFile(oldName, newName);

		    QString subject = _("Factura ") + num;
		    QString body = _("Adjunto le enviamos la factura numero ") + serie + num + _(" con fecha ") + fecha +"\n";
		    body += _("Sin otro particular, reciba un cordial saludo\n\n\n");
		    QString attached = g_confpr->value( CONF_DIR_USER ) + "factura" + serie + num + ".pdf";
		    QString bcc= "";

		    blSendEmail( email, bcc, subject, body, attached );
		    
                } // end if
                pres->close();
            } // end if
        } // end for
    } // end if

    if ( m_cobrosList != NULL ) {
        m_companyact = ( BfCompany * ) m_cobrosList->mainCompany();
        BlSubForm *sub = m_cobrosList->mui_list;

        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "true" ) {
                QString id = rec->dbValue ( "idcobro" );
                QString idcliente = rec->dbValue ( "idcliente" );
                QString ref = rec->dbValue ( "refcobro" );
                QString fecha = rec->dbValue ( "fcobro" );
		fecha.replace("/", "_");

                QString query = "SELECT mailcliente from cliente WHERE idcliente=" + idcliente;
                BlDbRecordSet *curs = m_companyact->loadQuery ( query );
                QString email = curs->value( "mailcliente" );


				int resur = g_plugins->run ( "SNewCobroView", m_companyact );
				if ( !resur ) {
					blMsgInfo ( "no se pudo crear instancia de cobro" );
					return;
				} // end if
				CobroView *pres = ( CobroView * ) g_plugParams;
                pres->load ( id );

                blCreatePDF ( "recibo" );
                
                QString oldName =  g_confpr->value( CONF_DIR_USER ) + "recibo.pdf";
                QString newName = g_confpr->value( CONF_DIR_USER ) + "recibo" + fecha + ref + ".pdf";
                blMoveFile(oldName, newName);
	
		QString subject = _("Recibo ") + fecha + ref;
		QString body = _("Adjunto le enviamos el recibo con referencia ") + ref + _(" y fecha ") + fecha +"\n";
		body += _("Sin otro particular, reciba un cordial saludo\n\n\n");
		QString attached = g_confpr->value( CONF_DIR_USER ) + "recibo" + fecha + ref + ".pdf";
	  	QString bcc= "";

		blSendEmail( email, bcc, subject, body, attached );
		
                delete pres;
            } // end if
        } // end for
    } // end if


    
}



///
/**
\param pres
\param ped
\param alb
\param fac
\param cob
\param parent
**/
ProcesarQToolButton::ProcesarQToolButton ( PresupuestoList *pres, PedidosClienteList *ped, AlbaranClienteList *alb,  FacturasList *fac, CobrosList *cob , QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
    m_presupuestoList = pres;
    m_pedidosClienteList = ped;
    m_albaranClienteList = alb;
    m_facturasList = fac;
    m_cobrosList = cob;

    m_listado = (BlFormList *) parent->parent()->parent();

    setBoton();
    
}


///
/**
**/
ProcesarQToolButton::~ProcesarQToolButton()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void ProcesarQToolButton::setBoton()
{
    BL_FUNC_DEBUG
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "procesa" ) );
    setStatusTip ( _ ( "Procesar elementos seleccionados" ) );
    setToolTip ( _ ( "Procesar elementos seleccionados" ) );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( QString::fromUtf8 ( ":/BulmaCont32x32/images/png/i_ok.png" ) ) );
    setIconSize ( QSize ( 22, 22 ) );
    
}


///
/**
**/
void ProcesarQToolButton::click()
{
    BL_FUNC_DEBUG

    if ( QMessageBox::question ( this,
                                  _ ( "Procesar elementos" ),
                                  _ ( "Va a procesar todos los elementos seleccionados. Desea continuar ?" ),
                                  _ ( "&Si" ), _ ( "&No" ), QString::null, 0, 1 ) ) {

        
        return;
    }

    QString res = "";


    if ( m_presupuestoList != NULL ) {
        m_companyact = ( BfCompany * ) m_presupuestoList->mainCompany();
        BlSubForm *sub = m_presupuestoList->mui_list;
        QString ids = "";
        QString separador = "";
        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "true" ) {
                ids += separador + rec->dbValue ( "idpresupuesto" );
                separador = ",";
            } // end if
        } // end for
        QString query = "UPDATE presupuesto set procesadopresupuesto = true WHERE idpresupuesto in ("+ids+")";
        if (separador == ",") 
          m_companyact->runQuery ( query );
    } // end if



    if ( m_pedidosClienteList != NULL ) {
        m_companyact = ( BfCompany * ) m_pedidosClienteList->mainCompany();
        BlSubForm *sub = m_pedidosClienteList->mui_list;
        QString ids = "";
        QString separador = "";

        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );

            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "true" ) {
                ids += separador + rec->dbValue ( "idpedidocliente" );
                separador = ",";
            } // end if
        } // end for
        QString query = "UPDATE pedidocliente set procesadopedidocliente = true WHERE idpedidocliente in ("+ids+")";
        if (separador == ",") 
          m_companyact->runQuery ( query );
    } // end if




    if ( m_albaranClienteList != NULL ) {
        m_companyact = ( BfCompany * ) m_albaranClienteList->mainCompany();
        BlSubForm *sub = m_albaranClienteList->mui_list;
        QString ids = "";
        QString separador = "";

        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "true" ) {
                ids += separador + rec->dbValue ( "idalbaran" );
                separador = ",";
            } // end if
        } // end for
        QString query = "UPDATE albaran set procesadoalbaran = true WHERE idalbaran in ("+ids+")";
        if (separador == ",") 
          m_companyact->runQuery ( query );
    } // end if




    if ( m_facturasList != NULL ) {
        m_companyact = ( BfCompany * ) m_facturasList->mainCompany();
        BlSubForm *sub = m_facturasList->mui_list;
        QString ids = "";
        QString separador = "";

        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "true" ) {
                ids += separador + rec->dbValue ( "idfactura" );
                separador = ",";
            } // end if
        } // end for

        QString query = "UPDATE factura set procesadafactura = true WHERE idfactura in ("+ids+")";
        if (separador == ",") 
          m_companyact->runQuery ( query );
    } // end if

    if ( m_cobrosList != NULL ) {
        m_companyact = ( BfCompany * ) m_cobrosList->mainCompany();
        BlSubForm *sub = m_cobrosList->mui_list;
        QString ids = "";
        QString separador = "";

        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "true" ) {
                ids += separador + rec->dbValue ( "idcobro" );
                separador = ",";
            } // end if
        } // end for
        QString query = "UPDATE cobro set previsioncobro = false WHERE idcobro in ("+ids+")";
        if (separador == ",") 
          m_companyact->runQuery ( query );
    } // end if


    
}






// ==================================================================================


///
/**
\param pres
\param ped
\param alb
\param fac
\param cob
\param parent
**/
SumarQToolButton::SumarQToolButton ( PresupuestoList *pres, PedidosClienteList *ped, AlbaranClienteList *alb,  FacturasList *fac, CobrosList *cob , QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
    m_presupuestoList = pres;
    m_pedidosClienteList = ped;
    m_albaranClienteList = alb;
    m_facturasList = fac;
    m_cobrosList = cob;

    m_listado = (BlFormList *) parent->parent()->parent();

    setBoton();
    
}


///
/**
**/
SumarQToolButton::~SumarQToolButton()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void SumarQToolButton::setBoton()
{
    BL_FUNC_DEBUG
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "procesa" ) );
    setStatusTip ( _ ( "Totalizar elementos seleccionados" ) );
    setToolTip ( _ ( "Totalizar elementos seleccionados" ) );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon (  g_confpr->value( CONF_PROGDATA ) + "icons/suma.png" ) );
    setIconSize ( QSize ( 22, 22 ) );
    
}


///
/**
**/
void SumarQToolButton::click()
{
    BL_FUNC_DEBUG

    // Es posible que esto se haya cargado antes de cargar el company por eso
    // No me fio de que la asignacion en el constructor haya ido bien y reasigno aqui


    QString res = "";


    if ( m_presupuestoList != NULL ) {
        m_companyact = ( BfCompany * ) m_presupuestoList->mainCompany();
        BlSubForm *sub = m_presupuestoList->mui_list;
        QString ids = "";
        QString separador = "";
        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "true" ) {
                ids += separador + rec->dbValue ( "idpresupuesto" );
                separador = ",";
            } // end if
        } // end for
        QString query = "SELECT coalesce(SUM(totalpresupuesto), 0) AS total FROM  presupuesto WHERE idpresupuesto in ("+ids+")";
        if (separador == ",") {
          BlDbRecordSet *curs = m_companyact->loadQuery ( query );
          blMsgInfo("Total : " + curs->value( "total" ));
          delete curs;
        } // end if
    } // end if



    if ( m_pedidosClienteList != NULL ) {
        m_companyact = ( BfCompany * ) m_pedidosClienteList->mainCompany();
        BlSubForm *sub = m_pedidosClienteList->mui_list;
        QString ids = "";
        QString separador = "";

        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );

            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "true" ) {
                ids += separador + rec->dbValue ( "idpedidocliente" );
                separador = ",";
            } // end if
        } // end for
        QString query = "SELECT coalesce(SUM(totalpedidocliente), 0) AS total FROM  pedidocliente WHERE idpedidocliente in ("+ids+")";
        if (separador == ",") {
          BlDbRecordSet *curs = m_companyact->loadQuery ( query );
          blMsgInfo("Total : " + curs->value( "total" ));
          delete curs;
        } // end if
    } // end if




    if ( m_albaranClienteList != NULL ) {
        m_companyact = ( BfCompany * ) m_albaranClienteList->mainCompany();
        BlSubForm *sub = m_albaranClienteList->mui_list;
        QString ids = "";
        QString separador = "";

        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "true" ) {
                ids += separador + rec->dbValue ( "idalbaran" );
                separador = ",";
            } // end if
        } // end for
        QString query = "SELECT coalesce(SUM(totalalbaran), 0) AS total FROM  albaran WHERE idalbaran in ("+ids+")";
        if (separador == ",") {
          BlDbRecordSet *curs = m_companyact->loadQuery ( query );
          blMsgInfo("Total : " + curs->value( "total" ));
          delete curs;
        } // end if
    } // end if




    if ( m_facturasList != NULL ) {
        m_companyact = ( BfCompany * ) m_facturasList->mainCompany();
        BlSubForm *sub = m_facturasList->mui_list;
        QString ids = "";
        QString separador = "";

        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "true" ) {
                ids += separador + rec->dbValue ( "idfactura" );
                separador = ",";
            } // end if
        } // end for

        QString query = "SELECT coalesce(SUM(totalfactura), 0) AS total FROM  factura WHERE idfactura in ("+ids+")";
        if (separador == ",") {
          BlDbRecordSet *curs = m_companyact->loadQuery ( query );
          blMsgInfo("Total : " + curs->value( "total" ));
          delete curs;
        } // end if
    } // end if

    if ( m_cobrosList != NULL ) {
        m_companyact = ( BfCompany * ) m_cobrosList->mainCompany();
        BlSubForm *sub = m_cobrosList->mui_list;
        QString ids = "";
        QString separador = "";

        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "true" ) {
                ids += separador + rec->dbValue ( "idcobro" );
                separador = ",";
            } // end if
        } // end for
        QString query = "SELECT coalesce(SUM(cantcobro), 0) AS total FROM  cobro WHERE idcobro in ("+ids+")";
        if (separador == ",") {
          BlDbRecordSet *curs = m_companyact->loadQuery ( query );
          blMsgInfo("Total : " + curs->value( "total" ));
          delete curs;
        } // end if
    } // end if


    
}





