/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

// #include <stdio.h>

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>

#include "pluginbc_registroiva.h"
#include "bccompany.h"
#include "registroivaview.h"
#include "cobropagoview.h"

#include "bcasientoform.h"
#include "bcasientosubform.h"
#include "listregistroivaview.h"
#include "registroivaview.h"
#include "regivaqtoolbutton.h"
#include "bcmodelo347listview.h"

BcBulmaCont *g_bcont;

///
/**
\return
**/
int entryPoint ( BcBulmaCont *bcont )
{
    blDebug ( "Punto de entrada del plugin registroIVA", 0 );

    g_bcont = bcont;
    
    QMenu *pPluginMenu = bcont->newMenu(_("&Iva"), "menuIva", "menuMaestro");

    /// Creamos el men&uacute;.

    BlAction *accion = new BlAction ( _ ( "&Registro de IVA" ), 0 );
    accion->setStatusTip ( _ ( "Registro de IVA" ) );
    accion->setWhatsThis ( _ ( "Registro de IVA" ) );
    accion->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/vat.png" ) ) );
    accion->setObjectName("regiva");
    
    BlAction *accion1 = new BlAction ( _ ( "&Cobros y pagos" ), 0 );
    accion1->setStatusTip ( _ ( "Cobros y pagos" ) );
    accion1->setWhatsThis ( _ ( "Cobros y pagos" ) );
    accion1->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/bank-list.png") ) );
    accion1->setObjectName ("cobrospagos");

    BlAction *accion2 = new BlAction ( _ ( "&Modelo 347" ), 0 );
    accion2->setStatusTip ( _ ( "Modelo 347" ) );
    accion2->setWhatsThis ( _ ( "Modelo 347" ) );
//    accion2->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/bank-list.png") ) );
    accion2->setObjectName ("m347");
    
    pPluginMenu->addAction ( accion );
    pPluginMenu->addAction ( accion1 );
    pPluginMenu->addAction ( accion2 );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bcont->toolBar->addAction ( accion );
    bcont->toolBar->addAction ( accion1 );
    
    
    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_registroiva", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    return 0;
}

int BlAction_triggered(BlAction *accion) {
    if (accion->objectName() == "regiva") {
	ListRegistroIvaView *perd = new ListRegistroIvaView ( g_bcont->empresaactual(), "0" );
	perd->inicializa();
	g_bcont->empresaactual() ->pWorkspace() ->addSubWindow ( perd );
	perd->show();
    } // end if
    
    if (accion->objectName() == "cobrospagos") {
	cobropagoview *adoc = new cobropagoview ( g_bcont->empresaactual(), 0 );
	g_bcont->empresaactual() ->pWorkspace() ->addSubWindow ( adoc );
	adoc->show();
    } // end if
    if (accion->objectName() == "m347") {
      BcModelo347ListView *dlg347 = new BcModelo347ListView ( g_bcont->empresaactual(), "0" );
      dlg347->exec();
      delete dlg347;
    } // end if
    return 0;
} // end if






///
/**
\param l
\return
**/
int BcAsientoView_BcAsientoView ( BcAsientoView *l )
{
    blDebug ( "BcAsientoView_BcAsientoView", 0 );

    QFrame *plug = l->findChild<QFrame *> ("mui_plugbotones");

    RegIVAQToolButton *mui_exporta_efactura2 = new RegIVAQToolButton ( l,  plug );
    mui_exporta_efactura2->setObjectName("m_registroiva");
    if (plug) {
	QHBoxLayout *m_hboxLayout1 = plug->findChild<QHBoxLayout *> ( "hboxLayout1" );
	if ( !m_hboxLayout1 ) {
	    m_hboxLayout1 = new QHBoxLayout ( plug );
	    m_hboxLayout1->setSpacing ( 5 );
	    m_hboxLayout1->setMargin ( 5 );
	    m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
	} // end if
	m_hboxLayout1->addWidget ( mui_exporta_efactura2 );
    } // end if
    blDebug ( "END BcAsientoView_BcAsientoView", 0 );
    return 0;
}



/// Buscamos en el asiento si hay indicios de una factura y actuamos en consecuencia.
/**
\param as
\return
**/
int BcAsientoForm_guardaAsiento1_post ( BcAsientoForm *as )
{
    blDebug ( "BcAsientoForm_guardaAsiento1_post", 0 );
    
    
    BcCompany *companyact = as->companyact();
    QString cuentas = "";
    QString query = "SELECT valor FROM configuracion WHERE nombre = 'RegistroEmitida' OR nombre = 'RegistroSoportada'";
    BlDbRecordSet *curvalor = companyact->loadQuery ( query );
    while ( !curvalor->eof() ) {
        /// Preparamos una expresi&oacute;n regular para usar en la consulta.
        cuentas += curvalor->valor ( "valor" ) + "%|";
        curvalor->nextRecord();
    } // end while
    delete curvalor;
    /// Le quitamos el &uacute;ltimo '|' que nos sobra.
    cuentas.truncate ( cuentas.length() - 1 );

    /// Recorremos la tabla en busca de entradas de factura no introducidas y las
    /// preguntamos antes de cerrar nada.
    QString SQLQuery = "SELECT bcontrapartidaborr(idborrador) AS contra FROM borrador LEFT JOIN cuenta ON borrador.idcuenta = cuenta.idcuenta WHERE idasiento = " + as->dbValue ( "idasiento" ) + " AND codigo SIMILAR TO '" + companyact->sanearCadena ( cuentas.toAscii().constData() ) + "' GROUP BY contra";

    BlDbRecordSet *cursborr = companyact->loadQuery ( SQLQuery );
    while ( !cursborr->eof() ) {
        int idborrador = cursborr->valor ( "contra" ).toInt();
        RegistroIvaView *reg = new RegistroIvaView ( companyact, 0 );
        reg->inicializa1 ( idborrador );
        companyact->pWorkspace() ->addSubWindow ( reg );
        reg->show();
        cursborr->nextRecord();
    } // end while
    delete cursborr;
    
    blDebug ( "END BcAsientoForm_guardaAsiento1_post", 0 );
    return 0;
}



///
/**
\param as
\return
**/
int BcAsientoSubForm_boton_iva ( BcAsientoSubForm *as )
{
    blDebug ( "BcAsientoSubForm_boton_iva", 0 );
    
    as->guardar();
    try {
        int idborrador = as->dbValue ( "idborrador" ).toInt();
        RegistroIvaView *nuevae = new RegistroIvaView ( ( BcCompany * ) as->mainCompany(), 0 );
        nuevae->inicializa1 ( idborrador );
        ( ( BcCompany * ) as->mainCompany() ) ->pWorkspace() ->addSubWindow ( nuevae );
        nuevae->show();
    } catch ( ... ) {
        blMsgInfo ( _ ( "Debe seleccionar un apunte" ) );
        return 0;
    } // end try
    
    blDebug ( "END BcAsientoSubForm_boton_iva", 0 );
    return 0;
}
