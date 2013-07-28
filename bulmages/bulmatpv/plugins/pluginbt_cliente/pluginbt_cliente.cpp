/***************************************************************************
 *   Copyright (C) 2010 by Tomeu Borras Riera                              *
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

#include "pluginbt_cliente.h"

#include "bltoolbutton.h"

#include "btcompany.h"
#include "blplugins.h"
#include "btticket.h"
#include "bldockwidget.h"
#include "blapplication.h"
#include "blfunctions.h"
#include "blmainwindow.h"
#include "blplugins.h"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QToolButton>
#include <QtCore/QProcess>
#include <QtWidgets/QDockWidget>


///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbt_cliente", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    
    return 0;
}



int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{
      BL_FUNC_DEBUG
    BlToolButton *boton_cliente = new BlToolButton(etpv, etpv);

        boton_cliente->setObjectName(QString::fromUtf8("mui_cliente"));
        boton_cliente->setMinimumSize(QSize(72, 72));
        boton_cliente->setMaximumSize(QSize(200, 72));
        boton_cliente->setFocusPolicy(Qt::NoFocus);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Images/client.png"), QSize(), QIcon::Normal, QIcon::Off);
        boton_cliente->setIcon(icon);
        boton_cliente->setIconSize(QSize(32, 32));
        boton_cliente->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        boton_cliente->setText(N_("Cliente", 0));

    QFrame *fr = g_main->findChild<QFrame *> ( "mui_frameabrevs" );
    if ( fr ) {
        QHBoxLayout *m_hboxLayout1 = fr->findChild<QHBoxLayout *> ( "hboxLayout1" );
        if ( !m_hboxLayout1 ) {
            m_hboxLayout1 = new QHBoxLayout ( fr );
            m_hboxLayout1->setSpacing ( 5 );
            m_hboxLayout1->setMargin ( 5 );
            m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
        } // end if
	m_hboxLayout1->addWidget (boton_cliente);
    } // end if



    return 0;
}


///
/**
\return
**/
int exitPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG
    
    return 0;
}


int BlToolButton_released(BlToolButton *bot) {
      BL_FUNC_DEBUG
  if (bot->objectName() == "mui_cliente") {

    BtCompany * emp = ( BtCompany * ) bot->mainCompany();

    if ( emp->valorBtInput() == "" ) return 0;
    QString query = "SELECT * FROM cliente WHERE codcliente = '" + emp->valorBtInput() + "'";
    BlDbRecordSet *cur = emp->loadQuery ( query );
    if ( !cur->eof() ) {
        emp->ticketActual() ->setDbValue ( "idcliente", cur->value( "idcliente" ) );
    } else {
        emp->ticketActual() ->setDbValue ( "idcliente", g_confpr->value( CONF_IDCLIENTE_DEFECTO ) );
    } // end if
    delete cur;
    emp->ticketActual() ->pintar();
    emp->setValorBtInput ( "" );
    emp->pulsaTecla ( 0, "" );
    return 1;
  } //end if
  return 0;
}
