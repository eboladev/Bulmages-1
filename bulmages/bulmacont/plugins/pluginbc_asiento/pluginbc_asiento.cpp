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

    	if (!g_pluginbc_asiento->company()->showWindow("BcAsientoListView")) {
	    BcAsientoListView *asientoListView = new BcAsientoListView ( g_pluginbc_asiento->company(), 0 );
	    asientoListView->setObjectName("BcAsientoListView");
	    asientoListView->presentar();
            g_pluginbc_asiento->company()->pWorkspace() -> addSubWindow ( asientoListView );
	    asientoListView->show();
	} // end if

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









/// --------------------------------------------------------------
/// --------- Implemento la seleccion de diarios -----------------
/// Con esta funcionalidad creamos menus contextuales en todos los subformularios donde
/// Aparezca el identificador de articulo como elemento y permite de forma sencilla
/// La creacion, la edicion, y la seleccion.
///

/**
\param parent
**/
SubForm_Asiento::SubForm_Asiento ( BlSubForm *parent ) : QObject ( parent )
{
    BL_FUNC_DEBUG
    
}

///
/**
**/
SubForm_Asiento::~SubForm_Asiento()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu
**/
void SubForm_Asiento::s_pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
    BlSubForm *sub = ( BlSubForm * ) parent();
    BlSubFormHeader *header = sub->header ( "idasiento" );
    if ( header ) {
        menu->addSeparator();
        QString idcuenta = sub->dbValue ( "idasiento" );
        if ( idcuenta != "" )  {
	    menu->addAction ( _ ( "Mostrar Asiento" ) );
	} // end if
    } // end if
}


///
/**
\param action
**/
void SubForm_Asiento::s_trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG
    BlSubForm *sub = ( BlSubForm * ) parent();
    QString idcuenta = sub->dbValue ( "idasiento" );

    if ( idcuenta != "" ) {
	if ( action->text() == _ ( "Mostrar Asiento" ) ) {
            mostrarAsiento ();
	} // end if
    } // end if
}



/// Si el parametro pasado es un:
/// 0 -> del d&iacute;a actual
/// 1 -> del mes actual
/// 2 -> del a&ntilde;o actual
/**
\param tipo
**/
void SubForm_Asiento::mostrarAsiento (  )
{
    BL_FUNC_DEBUG

    BlSubForm *sub = ( BlSubForm * ) parent();
    
    QString numasiento = sub->dbValue ( "idasiento" );
    if ( numasiento != "" ) {
        g_asiento ->muestraAsiento ( numasiento.toInt() );
        g_asiento->hide();
	g_asiento->show();
    } // end if  
}



///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    BL_FUNC_DEBUG
    SubForm_Asiento *subformods = new SubForm_Asiento ( sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    
    return 0;
}



/// Miramos de poner los iconos del menu de subformularios
///
/**
\param sub
\return
**/
int BlSubForm_preparaMenu ( BlSubForm *sub ) {
    BL_FUNC_DEBUG
    BlSubFormHeader *header = sub->header ( "codigo" );
    if ( header ) {
	SubForm_Asiento *subformods = new SubForm_Asiento ( sub );
	
	QHBoxLayout *m_hboxLayout1 = sub->mui_menusubform->findChild<QHBoxLayout *> ( "hboxLayout1" );
	if ( !m_hboxLayout1 ) {
	    m_hboxLayout1 = new QHBoxLayout ( sub->mui_menusubform );
	    m_hboxLayout1->setSpacing (0 );
	    m_hboxLayout1->setMargin ( 0 );
	    m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
	} // end if
	

	  QToolButton *sel = new QToolButton ( sub->mui_menusubform );
	  sel->setStatusTip ( "Mostrar Asiento" );
	  sel->setToolTip ( "Mostrar Asiento" );
	  sel->setMinimumSize ( QSize ( 18, 18 ) );
	  sel->setIcon ( QIcon ( ":/Images/book.png" ) );
	  sel->setIconSize ( QSize ( 18, 18 ) );    
	  m_hboxLayout1->addWidget ( sel );
	  sel->connect (sel, SIGNAL(released()), subformods, SLOT(mostrarAsiento()));
	
    } // end if
    return 0;
}










