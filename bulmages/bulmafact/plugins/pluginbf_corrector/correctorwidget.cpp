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

#include "correctorwidget.h"
#include "confreglasview.h"
#include "clienteview.h"
#include "provedit.h"
#include "facturaview.h"


/** Inicializacion de la clase.
    En el constructor del corrector no se hacen tareas especiales. */
/**
\param parent
\param fl
**/
correctorwidget::correctorwidget ( QWidget* parent, Qt::WFlags fl )
        : BlWidget ( parent, fl )
{
    blDebug ( "correctorwidget::correctorwidget", 0 );
    setupUi ( this );
    QObject::connect ( mui_browser, SIGNAL ( anchorClicked ( const QUrl ) ), this, SLOT ( alink ( const QUrl ) ) );
    blDebug ( "END correctorwidget::correctorwidget", 0 );
}


///
/**
**/
correctorwidget::~correctorwidget()
{
    blDebug ( "correctorwidget::~correctorwidget", 0 );
    blDebug ( "END correctorwidget::~correctorwidget", 0 );
}


/** Se ha pulsado sobre el boton de configuracion de reglas.
    Activa la ventana de correcion de reglas \ref confreglasview */
/**
**/
void correctorwidget::on_mui_configurar_clicked()
{
    blDebug ( "correctorwidget::on_mui_configurar_clicked", 0 );
    confreglasview *conf = new confreglasview();
    conf->exec();
    delete conf;
    blDebug ( "END correctorwidget::on_mui_configurar_clicked", 0 );
}


/** Se encarga de llevar a cabo las diferentes pruebas y generar el informe.
    Esta funcion es el disparador del corrector, que se encarga de hacer los tests a la
    empresa y sacar la ventana de resultados. */
/**
**/
void correctorwidget::on_mui_corregir_clicked()
{
    blDebug ( "correctorwidget::on_mui_corregir_clicked", 0 );
    textBrowser = "<HTML><BODY BGCOLOR='#FFFFFF'>";
    QString cadena;

    QString query = "SELECT * from factura WHERE reffactura NOT IN (SELECT refalbaran FROM albaran)";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
    while ( ! cur->eof() ) {
        cadena = "<img src='" + g_confpr->valor ( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>La factura num. <B>" + cur->valor ( "numfactura" ) + "</B> No esta avalada por ningun albaran, esto puede ser causa de descontrol en el stock.";
        agregarError ( cadena, "factura", "idfactura=" + cur->valor ( "idfactura" ) );
        cur->nextRecord();
    } // end while
    delete cur;

    query = "SELECT * from cliente WHERE length(cifcliente) < 6";
    cur = mainCompany() ->loadQuery ( query );
    while ( ! cur->eof() ) {
        cadena = "<img src='" + g_confpr->valor ( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El cliente <B>" + cur->valor ( "nomcliente" ) + "</B> no tiene CIF.";
        agregarError ( cadena, "cliente", "idcliente=" + cur->valor ( "idcliente" ) );
        cur->nextRecord();
    } // end while
    delete cur;

    query = "SELECT * from proveedor WHERE length(cifproveedor) < 6";
    cur = mainCompany() ->loadQuery ( query );
    while ( ! cur->eof() ) {
        cadena = "<img src='" + g_confpr->valor ( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El proveedor <B>" + cur->valor ( "nomproveedor" ) + "</B> no tiene CIF.";
        agregarError ( cadena, "proveedor", "idproveedor=" + cur->valor ( "idproveedor" ) );
        cur->nextRecord();
    } // end while
    delete cur;


    query = "SELECT * FROM cliente";
    cur = mainCompany()->loadQuery ( query );
    while ( ! cur->eof() ) {
        QChar digito;
        if ( ! blValidateSpainCIFNIFCode ( cur->valor ( "cifcliente" ), digito ) ) {
            cadena = "<img src='" + g_confpr->valor ( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El cliente ," + cur->valor ( "cifcliente" ) + " <B>" + cur->valor ( "nomcliente" ) + "</B> tiene CIF invalido. Digito de Control:" + QString ( digito );
            agregarError ( cadena, "cliente", "idcliente=" + cur->valor ( "idcliente" ) );
        } // end if
        cur->nextRecord();
    } // end while
    delete cur;

    query = "SELECT * FROM proveedor";
    cur = mainCompany()->loadQuery ( query );
    while ( ! cur->eof() ) {
        QChar digito;
        if ( ! blValidateSpainCIFNIFCode ( cur->valor ( "cifproveedor" ), digito ) ) {
            cadena = "<img src='" + g_confpr->valor ( CONF_PROGDATA ) + "icons/messagebox_warning.png'>&nbsp;&nbsp;<B><I>Warning:</I></B><BR>El proveedor <B>" + cur->valor ( "nomproveedor" ) + "," + cur->valor ( "cifproveedor" ) + "</B> tiene CIF invalido. Digito de Control: " + QString ( digito );
            agregarError ( cadena, "proveedor", "idproveedor=" + cur->valor ( "idproveedor" ) );
        } // end if
        cur->nextRecord();
    } // end while
    delete cur;


    textBrowser += "</BODY></HTML>";
    mui_browser->setHtml ( textBrowser );
    blDebug ( "END correctorwidget::on_mui_corregir_clicked", 0 );
}


///
/**
\param mensaj
**/
void correctorwidget::setMensaje ( QString mensaje )
{
    blDebug ( "correctorwidget::setMensaje", 0 );
    mui_browser->setHtml ( mensaje );
    blDebug ( "END correctorwidget::setMensaje", 0 );
}



/** Detecta la pulsacion de un enlace en el widget de resultados.
    Se activa esta funcion cuando se ha pulsado un link en el Widget de resultados
    QTextBrowser.
    De esta forma la aplicacion puede interactuar con la pagina web que se muestra. */
/**
\param url
\return
**/
void correctorwidget::alink ( const QUrl &url )
{
    blDebug ( "correctorwidget::alinke", 0 );
    QString linker = url.fragment();
    QStringList list = linker.split ( "=" );
    if ( list[0] == "idcliente" ) {
        /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
        int resur = g_plugins->lanza ( "SNewClienteView", ( BfCompany * ) mainCompany() );
        if ( !resur ) {
            blMsgInfo ( "No se pudo crear instancia de cliente" );
            return;
        } // end if
        ClienteView *prov = ( ClienteView * ) g_plugParams;
        if ( prov->cargar ( list[1] ) ) {
            delete prov;
            return;
        } // end if
        ( ( BfCompany * ) mainCompany() ) ->m_pWorkspace->addWindow ( prov );
        prov->show();
    } else if ( list[0] == "idfactura" ) {
        /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
        int resur = g_plugins->lanza ( "SNewFacturaView", ( BfCompany * ) mainCompany() );
        if ( !resur ) {
            blMsgInfo ( "No se pudo crear instancia de factura" );
            return;
        } // end if
        FacturaView *prov = ( FacturaView * ) g_plugParams;
        if ( prov->cargar ( list[1] ) ) {
            delete prov;
            return;
        } // end if
        ( ( BfCompany * ) mainCompany() ) ->m_pWorkspace->addWindow ( prov );
        prov->show();
    } else if ( list[0] == "idproveedor" ) {
        /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
        int resur = g_plugins->lanza ( "SNewProveedorView", ( BfCompany * ) mainCompany() );
        if ( !resur ) {
            blMsgInfo ( "No se pudo crear instancia de cliente" );
            return;
        } // end if
        ProveedorView *prov = ( ProveedorView * ) g_plugParams;
        if ( prov->cargar ( list[1] ) ) {
            delete prov;
            return;
        } // end if
        ( ( BfCompany * ) mainCompany() ) ->m_pWorkspace->addWindow ( prov );
        prov->show();
    }// end if
    blDebug ( "END correctorwidget::alinke", 0 );
}


/** Se encarga de agregar un error en el browser que los presenta.
    Sirve para para introducir los errores hayados en el corrector y que se vean por
    pantalla. */
/**
\param texto
\param texto1
\param texto2
**/
void correctorwidget::agregarError ( QString texto, QString texto1, QString texto2 )
{
    blDebug ( "correctorwidget::agregarError", 0 );
    textBrowser += "<HR><table><tr><td colspan=2>" + texto + "</td></tr><tr><td><a name='masinfo' href='#" + texto1 + "'>+ info</a></td><td><a name='" + texto1 + "' href='#" + texto2 + "'>ver error</a></td></tr></table>";
    blDebug ( "END correctorwidget::agregarError", 0 );
}


///
/**
\param a
**/
void correctorwidget::cambia ( bool a )
{
    blDebug ( "correctorwidget::cambia", 0 );
    if ( a ) {
        dock->hide();
        dock->show();
        dock->showMaximized();
    } else {
        dock->hide();
    } // end if
    blDebug ( "END correctorwidget::cambia", 0 );
}

