/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
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

#include <cstdio>

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>
#include <QTextCodec>
#include <QLocale>
#include <QInputDialog>

#include "pluginbc_asiento.h"
#include "bcasientoview.h"
#include "bccompany.h"
#include "bcasientolistview.h"

BcAsientoView *g_asiento;
BcAsientoListView *g_listasientos;
BcBulmaCont *g_pluginbc_asiento = NULL;


///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_asiento", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_asiento = NULL;
    g_pluginbc_asiento = bcont;


    /// Creamos la ventana de asientos que va a ser fija.
    g_asiento = new BcAsientoView ( ( BcCompany * ) g_pluginbc_asiento->company(), 0 );
    g_pluginbc_asiento->company()->pWorkspace() ->addSubWindow ( g_asiento );

    QMenu *pPluginMenu = bcont->newMenu(_("&Asiento"), "menuAsiento", "menuMaestro");

    BlAction *accionA = new BlAction ( _ ( "&Asiento contable" ), 0 );
    accionA->setStatusTip ( _ ( "Permite ver y modificar asientos" ) );
    accionA->setWhatsThis ( _ ( "Podra disponer de la informacion del asiento" ) );
    accionA->setIcon(QIcon(QString::fromUtf8(":/Images/book.png")));
    accionA->setObjectName("mui_actionAsientoContable");
    pPluginMenu->addAction ( accionA );
    bcont->toolBar->addAction ( accionA );


    BlAction *accionB = new BlAction ( _ ( "&Lista de asientos contables" ), 0 );
    accionB->setStatusTip ( _ ( "Permite ver el listado de asientos" ) );
    accionB->setWhatsThis ( _ ( "Permite ver el listado de asientos" ) );
    accionB->setIcon(QIcon(QString::fromUtf8(":/Images/book-list.png")));
    accionB->setObjectName("mui_actionAsientoContableLista");
    pPluginMenu->addAction ( accionB );
    bcont->toolBar->addAction ( accionB );

    pPluginMenu->addSeparator();

    BlAction *accionC = new BlAction ( _ ( "&Espaciar asientos contables" ), 0 );
    accionC->setStatusTip ( _ ( "Ordena los asientos dejando espacios entre ellos" ) );
    accionC->setWhatsThis ( _ ( "Ordena los asientos dejando espacios entre ellos" ) );
    accionC->setObjectName("mui_actionAsientoContableEspaciar");
    pPluginMenu->addAction ( accionC );


    BlAction *accionD = new BlAction ( _ ( "&Reordenar asientos contables" ), 0 );
    accionD->setStatusTip ( _ ( "Ordena los asientos" ) );
    accionD->setWhatsThis ( _ ( "Ordena los asientos" ) );
    accionD->setObjectName("mui_actionAsientoContableOrdenar");
    pPluginMenu->addAction ( accionD );

    pPluginMenu->addSeparator();

    BlAction *accionE = new BlAction ( _ ( "Asiento de a&pertura" ), 0 );
    accionE->setStatusTip ( _ ( "Asiento de apertura" ) );
    accionE->setWhatsThis ( _ ( "Asiento de apertura" ) );
    accionE->setObjectName("mui_actionAsientoApertura");
    pPluginMenu->addAction ( accionE );

    BlAction *accionF = new BlAction ( _ ( "Asiento de &cierre" ), 0 );
    accionF->setStatusTip ( _ ( "Asiento de cierre" ) );
    accionF->setWhatsThis ( _ ( "Asiento de cierre" ) );
    accionF->setObjectName("mui_actionAsientoCierre");
    pPluginMenu->addAction ( accionF );

    BlAction *accionG = new BlAction ( _ ( "Asiento de &regularizacion" ), 0 );
    accionG->setStatusTip ( _ ( "Asiento de regularizacion" ) );
    accionG->setWhatsThis ( _ ( "Asiento de regularizacion" ) );
    accionG->setObjectName("mui_actionAsientoRegularizacion");
    pPluginMenu->addAction ( accionG );


    
    return 0;
}

int BlAction_actionTriggered(BlAction *accion) {

    if (accion->objectName() == "mui_actionAsientoContable") {
        if (g_asiento == NULL) {
            g_asiento = new BcAsientoView ( g_pluginbc_asiento->company(), 0 );
            g_pluginbc_asiento->company()->pWorkspace() -> addSubWindow ( g_asiento );
        } // end if
        g_asiento->hide();
        g_asiento->show();
    } // end if
    if (accion->objectName() == "mui_actionAsientoContableLista") {
        if (g_listasientos == NULL) {
            g_listasientos = new BcAsientoListView ( g_pluginbc_asiento->company() );
            g_listasientos->presentar();
            g_pluginbc_asiento->company()->pWorkspace() -> addSubWindow ( g_listasientos );
        } // end if
        g_listasientos->hide();
        g_listasientos->show();
    } // end if

    if (accion->objectName() == "mui_actionAsientoContableEspaciar") {
        BlDbRecordSet *cur = NULL;
        try {
            cur = g_pluginbc_asiento->company()->loadQuery ( "SELECT abreasientos()" );
            g_main->statusBar() ->showMessage ( _ ( "Se han espaciado los asientos" ), 2000 );
            delete cur;
        } catch ( ... ) {
            blMsgError ( _("Ha habido un error al espaciar los asientos") );
            if ( cur ) delete cur;
        return 1;
        } // end try
    } // end if

    if (accion->objectName() == "mui_actionAsientoContableOrdenar") {
        QString query = "SELECT reordenaasientosall()";
        BlDbRecordSet *cur = NULL;
        try {
            cur = g_pluginbc_asiento->company()->loadQuery ( query );
            g_main->statusBar() ->showMessage ( _ ( "Se han ordenado los asientos" ), 2000 );
            delete cur;
        } catch ( ... ) {
            blMsgError ( _("Ha habido un error al ordenar los asientos") );
            if ( cur ) delete cur;
            return 1;
        }
    } // end if

    if (accion->objectName() == "mui_actionAsientoApertura") {
        QString hoy = QDate::currentDate().toString ( "dd/MM/yyyy" );
        QString finicial = "01/01/" + hoy.right ( 4 );
        bool ok;
    
        QString text1 = QInputDialog::getText ( 0,
                _ ( "Fecha" ), _ ( "Fecha:" ), QLineEdit::Normal,
                hoy, &ok );
        if ( ok && !text1.isEmpty() ) {
            hoy = text1;
        } else {
            return 0;
        } // end if
        g_asiento->show();
        g_asiento->asientoApertura ( hoy );
    } // end if

    if (accion->objectName() == "mui_actionAsientoCierre") {
        QString hoy = QDate::currentDate().toString ( "dd/MM/yyyy" );
        QString finicial = "01/01/" + hoy.right ( 4 );
    
        bool ok;
        QString text = QInputDialog::getText ( 0,
                _ ( "Fecha inicial" ), _ ( "Fecha inicial ejercicio:" ), QLineEdit::Normal,
                finicial, &ok );
    
        if ( ok && !text.isEmpty() ) {
            finicial = text;
        } else {
            return 0;
        } // end if
    
        QString text1 = QInputDialog::getText ( 0,
                _ ( "Fecha final" ), _ ( "Fecha final ejercicio:" ), QLineEdit::Normal,
                hoy, &ok );

        if ( ok && !text1.isEmpty() ) {   
            hoy = text1;
        } else {
            return 0;
        } // end if

        g_asiento->show();
        g_asiento->asientoCierre ( finicial, hoy );

        } // end if
        if (accion->objectName() == "mui_actionAsientoRegularizacion") {
            QString hoy = QDate::currentDate().toString ( "dd/MM/yyyy" );
            QString finicial = "01/01/" + hoy.right ( 4 );

            bool ok;
            QString text = QInputDialog::getText ( 0,
                    _ ( "Fecha inicial" ), _ ( "Fecha inicial regularizacion:" ), QLineEdit::Normal,
                    finicial, &ok );
            if ( ok && !text.isEmpty() ) {
                finicial = text;
            } else {
                return 0;
            } // end if
   
            QString text1 = QInputDialog::getText ( 0,
                    _ ( "Fecha final" ), _ ( "Fecha final regularizacion:" ), QLineEdit::Normal,
                    hoy, &ok );
            if ( ok && !text1.isEmpty() ) {
                hoy = text1;
            } else {
                return 0;
            } // end if
            g_asiento->show();
            g_asiento->asientoRegularizacion ( finicial, hoy );
        } // end if

        return 0;
}


/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewBcAsientoView ( BcCompany * )
{
    g_plugParams = g_asiento;
    return 1;
}

