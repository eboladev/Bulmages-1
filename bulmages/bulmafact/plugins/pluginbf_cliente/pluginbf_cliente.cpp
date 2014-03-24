/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include <stdio.h>

#include "pluginbf_cliente.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "clienteview.h"
#include "clientslist.h"




ClientsList *g_clientesList = NULL;
BfBulmaFact *g_pluginbf_cliente = NULL;

///
/**
\param bges
\return
**/
int entryPoint ( BlMainWindow *bges )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_cliente", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    if (bges->objectName() != "BtBulmaTPVBase") {
	g_pluginbf_cliente = (BfBulmaFact * ) bges;
	if ( g_pluginbf_cliente->company()->hasTablePrivilege ( "cliente", "SELECT" ) ) {

	    /// Miramos si existe un menu Ventas
	    QMenu *pPluginMenu = g_pluginbf_cliente->newMenu ( _("&Ventas"), "menuVentas", "menuMaestro" );
	    pPluginMenu->addSeparator();

	    BlAction *accionA = new BlAction ( _ ( "&Clientes" ), 0 );
	    accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/client-list.png" ) ) );
	    accionA->setStatusTip ( _ ( "Clientes" ) );
	    accionA->setWhatsThis ( _ ( "Clientes" ) );
	    accionA->setObjectName("mui_actionClientes");

	    pPluginMenu->addAction ( accionA );
	    g_pluginbf_cliente->Listados->addAction ( accionA );

	    BlAction *accionB = new BlAction ( _ ( "&Nuevo cliente" ), 0 );
	    accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/client.png" ) ) );
	    accionB->setStatusTip ( _ ( "Nuevo cliente" ) );
	    accionB->setWhatsThis ( _ ( "Nuevo cliente" ) );
	    accionB->setObjectName("mui_actionNuevoCliente");

	    pPluginMenu->addAction ( accionB );
	    g_pluginbf_cliente->Fichas->addAction ( accionB );

	} // end if
    } // end if

    return 0;
}

int BlAction_actionTriggered(BlAction *accion) {
    if (accion->objectName() == "mui_actionClientes") {
        if ( g_clientesList ) {
            g_clientesList->hide();
            g_clientesList->show();
        }// end if
    } // end if
    
    if (accion->objectName() == "mui_actionNuevoCliente") {
        ClienteView * bud = new ClienteView ( g_pluginbf_cliente->company(), NULL );
        g_pluginbf_cliente->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if
    
    return 0;
} // end if


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "cliente", "SELECT" ) ) {
        g_clientesList = new ClientsList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_clientesList );
        g_clientesList->hide();
    }// end if
    return 0;
}


int Busqueda_on_mui_buscar_clicked ( BlSearchWidget *busq )
{
    if ( busq->tableName() == "cliente" ) {

        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        blCenterOnScreen ( diag );

        ClientsList *clients = new ClientsList ( ( BfCompany * ) busq->mainCompany(), diag, 0, BL_SELECT_MODE );
        busq->connect ( clients, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

        /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
        /// para que sea redimensionable y aparezca el titulo de la ventana.
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget ( clients );
        layout->setMargin ( 0 );
        layout->setSpacing ( 0 );
        diag->setLayout ( layout );
        diag->setWindowTitle ( clients->windowTitle() );

        diag->exec();

        if ( clients->idclient() != "" ) {
            busq->setId ( clients->idclient() );
        } // end if

        delete diag;
        return 1;
    } // end if

    return 0;
}


/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewClienteView ( BfCompany *v )
{
    ClienteView *h = new ClienteView ( v, 0 );
    g_plugParams = h;
    return 1;
}

/// Apertura de un elemento controlado a partir del parametro g_plugParams tabla_identificador
int Plugin_open(BfCompany * comp) {
  BL_FUNC_DEBUG
  QString cad = *((QString*)g_plugParams);
  QStringList args = cad.split("_");
  if (args[0] == "cliente") {
        ClienteView * bud = new ClienteView ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( bud );
	QString id =  args[1];
	bud->load(id);
        bud->show();

  } // end if
  return 0;
}



int CorrectorWidget_corregir(BlWidget *corrector) {
   BL_FUNC_DEBUG
   
    /// En Windows no se soportan las rutas relativas para el HTML
#ifdef Q_OS_WIN32
	    QString cupath = QDir::currentPath().replace("program", "").replace(".bulmages","");
	    QString src= g_confpr->value( CONF_PROGDATA).replace("..",cupath);
#else
	    QString src = g_confpr->value( CONF_PROGDATA);
#endif
	  
    QString query = "SELECT * FROM cliente WHERE coalesce(length(cifcliente),0) < 6";
    BlDbRecordSet *cur = corrector->mainCompany() ->loadQuery ( query );
    while ( ! cur->eof() ) {

           QString cadena = "<HR><table><tr><td colspan=2><img src='file:///" + src + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El cliente <B>" + cur->value( "nomcliente" ) + "</B> no tiene CIF.</td></tr><tr><td><a name='masinfo' href='abredoc?op=masinfo&tabla=cliente&id=" + cur->value( "idcliente" ) + "'>+info</a></td><td></td></tr></table>";
	   *(QString *) g_plugParams += cadena;
	
        cur->nextRecord();
    } // end while
    delete cur;
	    
	    
    query = "SELECT * FROM cliente";
   cur = corrector->mainCompany()->loadQuery ( query );
   while ( ! cur->eof() ) {
       QChar digito;
       if ( ! blValidateSpainCIFNIFCode ( cur->value( "cifcliente" ), digito ) ) {
           QString cadena = "<HR><table><tr><td colspan=2><img src='file:///" + src + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El cliente ," + cur->value( "cifcliente" ) + " <B>" + cur->value( "nomcliente" ) + "</B> tiene CIF invalido. Digito de Control:" + QString ( digito ) + "</td></tr><tr><td><a name='masinfo' href='abredoc?op=masinfo&tabla=cliente&id=" + cur->value( "idcliente" ) + "'>+info</a></td><td></td></tr></table>";
	   *(QString *) g_plugParams += cadena;
       } // end if
       cur->nextRecord();
   } // end while
   delete cur;
   return 0;
}
