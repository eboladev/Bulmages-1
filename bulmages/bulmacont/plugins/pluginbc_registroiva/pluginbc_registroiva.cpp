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

BcBulmaCont *g_pluginbc_registroiva;

///
/**
\return
**/
int entryPoint ( BcBulmaCont *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_registroiva", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_pluginbc_registroiva = bcont;
    
    QMenu *pPluginMenu = bcont->newMenu(_("&Iva"), "menuIva", "menuMaestro");

    /// Creamos el men&uacute;.

    BlAction *accionA = new BlAction ( _ ( "&Registro de IVA" ), 0 );
    accionA->setStatusTip ( _ ( "Registro de IVA" ) );
    accionA->setWhatsThis ( _ ( "Registro de IVA" ) );
    accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/vat.png" ) ) );
    accionA->setObjectName("mui_actionRegistroIVA");
    pPluginMenu->addAction ( accionA );
    bcont->toolBar->addAction ( accionA );

    BlAction *accionB = new BlAction ( _ ( "&Cobros y pagos" ), 0 );
    accionB->setStatusTip ( _ ( "Cobros y pagos" ) );
    accionB->setWhatsThis ( _ ( "Cobros y pagos" ) );
    accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/bank-list.png") ) );
    accionB->setObjectName ("mui_actionCobrosYPagos");
    pPluginMenu->addAction ( accionB );
    bcont->toolBar->addAction ( accionB );


    BlAction *accionC = new BlAction ( _ ( "&Modelo 347" ), 0 );
    accionC->setStatusTip ( _ ( "Modelo 347" ) );
    accionC->setWhatsThis ( _ ( "Modelo 347" ) );
//    accion2->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/bank-list.png") ) );
    accionC->setObjectName ("mui_actionModelo347");
    pPluginMenu->addAction ( accionC );

    return 0;
}

int BlAction_actionTriggered(BlAction *accion) {
    if (accion->objectName() == "mui_actionRegistroIVA") {
	ListRegistroIvaView *perd = new ListRegistroIvaView ( g_pluginbc_registroiva->company(), "0" );
	perd->inicializa();
	g_pluginbc_registroiva->company() ->pWorkspace() ->addSubWindow ( perd );
	perd->show();
    } // end if
    
    if (accion->objectName() == "mui_actionCobrosYPagos") {
	cobropagoview *adoc = new cobropagoview ( g_pluginbc_registroiva->company(), 0 );
	g_pluginbc_registroiva->company() ->pWorkspace() ->addSubWindow ( adoc );
	adoc->show();
    } // end if
    if (accion->objectName() == "mui_actionModelo347") {
      BcModelo347ListView *dlg347 = new BcModelo347ListView ( g_pluginbc_registroiva->company(), "0" );
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
    BL_FUNC_DEBUG

    QHBoxLayout *layout = l->findChild<QHBoxLayout *> ("mui_plugbotones");

    if (layout) {
	RegIVAQToolButton *mui_exporta_efactura2 = new RegIVAQToolButton ( l );
	mui_exporta_efactura2->setObjectName("m_registroiva");
	layout->addWidget ( mui_exporta_efactura2 );
    } // end if

    return 0;
}



/// Buscamos en el asiento si hay indicios de una factura y actuamos en consecuencia.
/**
\param as
\return
**/
int BcAsientoForm_guardaAsiento1_post ( BcAsientoForm *as )
{
    BL_FUNC_DEBUG
    
    
    BcCompany *companyact = as->companyact();
    QString cuentas = "";
    QString query = "SELECT valor FROM configuracion WHERE nombre = 'RegistroEmitida' OR nombre = 'RegistroSoportada'";
    BlDbRecordSet *curvalor = companyact->loadQuery ( query );
    while ( !curvalor->eof() ) {
        /// Preparamos una expresi&oacute;n regular para usar en la consulta.
        cuentas += curvalor->value( "valor" ) + "%|";
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
        int idborrador = cursborr->value( "contra" ).toInt();
        RegistroIvaView *reg = new RegistroIvaView ( companyact, 0 );
        reg->inicializa1 ( idborrador );
        companyact->pWorkspace() ->addSubWindow ( reg );
        reg->show();
        cursborr->nextRecord();
    } // end while
    delete cursborr;
    
    
    return 0;
}


///
/**
\param as
\return
**/
int BcAsientoSubForm_boton_iva ( BcAsientoSubForm *as )
{
    BL_FUNC_DEBUG
    
    as->save();
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
    
    
    return 0;
}
