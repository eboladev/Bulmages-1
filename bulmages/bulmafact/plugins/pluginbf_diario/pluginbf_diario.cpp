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

#include "pluginbf_diario.h"
#include "bcdiarioview.h"
#include "bfcompany.h"


BcDiarioView *g_diario;
BfBulmaFact *g_pluginbf_diario = NULL;


///
/**
\param bcont
**/
int entryPoint ( BfBulmaFact *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_diario", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_diario = NULL;
    g_pluginbf_diario = bcont;

    QMenu *pPluginMenu = bcont->newMenu( _("&Ver"), "menuVer", "menuMaestro");

    BlAction *accionA = new BlAction ( _ ( "&Libro Diario" ), 0 );
    accionA->setStatusTip ( _ ( "Permite ver el Diario" ) );
    accionA->setWhatsThis ( _ ( "Podra disponer de la informacion del Diario" ) );
    accionA->setIcon(QIcon(QString::fromUtf8(":/Images/balance-sheet.png")));
    accionA->setObjectName("mui_actionDiario");
    pPluginMenu->addAction ( accionA );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bcont->Listados->addAction ( accionA );
    
    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG

    if (accion->objectName() == "mui_actionDiario") {

        BlDebug::blDebug ( Q_FUNC_INFO, 0, "mui_actionDiario" );

      	if (!g_pluginbf_diario->company()->showWindow("BcDiarioView")) {
	    g_diario = new BcDiarioView ( g_pluginbf_diario->company(), 0 );
	    g_diario->setObjectName("BcDiarioView");
	    g_pluginbf_diario->company()->pWorkspace()->addSubWindow ( g_diario );
	    g_diario->show();
	} // end if

    } // end if
    
    return 0;
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
SubForm_Diario::SubForm_Diario ( BlSubForm *parent ) : QObject ( parent )
{
    BL_FUNC_DEBUG
    
}

///
/**
**/
SubForm_Diario::~SubForm_Diario()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu
**/
void SubForm_Diario::s_pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
    BlSubForm *sub = ( BlSubForm * ) parent();
    BlSubFormHeader *header = sub->header ( "codigo" );
    if ( header ) {
        menu->addSeparator();
        QString idcuenta = sub->dbValue ( "idcuenta" );
        if ( idcuenta != "" )  {
	    menu->addAction ( _ ( "Diario (dia)" ) );
	    menu->addAction ( _ ( "Diario (mes)" ) );
	    menu->addAction ( _ ( "Diario (año)" ) );
	} // end if
    } // end if
}


///
/**
\param action
**/
void SubForm_Diario::s_trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG
    BlSubForm *sub = ( BlSubForm * ) parent();
    QString idcuenta = sub->dbValue ( "idcuenta" );

    if ( idcuenta != "" ) {
	if ( action->text() == _ ( "Diario (dia)" ) ) {
            boton_diario1 ( 0 );
	} // end if

	if ( action->text() == _ ( "Diario (mes)" ) ) {
            boton_diario1 ( 1 );
	} // end if
	
	if ( action->text() == _ ( "Diario (año)" ) ) {
            boton_diario1 ( 2 );
	} // end if
    } // end if
}

///
/**
\param action
**/
void SubForm_Diario::botonera ( )
{
    BL_FUNC_DEBUG
            boton_diario1 ( 0 );
}


///
/**
\param action
**/
void SubForm_Diario::botonera1 ( )
{
    BL_FUNC_DEBUG
            boton_diario1 ( 1 );
}


///
/**
\param action
**/
void SubForm_Diario::botonera2 ( )
{
    BL_FUNC_DEBUG
            boton_diario1 ( 2 );
}


/// Si el parametro pasado es un:
/// 0 -> del d&iacute;a actual
/// 1 -> del mes actual
/// 2 -> del a&ntilde;o actual
/**
\param tipo
**/
void SubForm_Diario::boton_diario1 ( int tipo )
{
    BL_FUNC_DEBUG

    BlSubForm *sub = ( BlSubForm * ) parent();
    QDate fecha1, fecha2, fechaact;
    QString fecha = sub->dbValue ( "fecha" ).left ( 10 );
    
    QString codigo = sub->dbValue ( "codigo" );
    if ( codigo != "" ) {
        fechaact = blNormalizeDate ( fecha );
        switch ( tipo ) {
        case 0:
            fecha1.setYMD ( fechaact.year(), fechaact.month(), fechaact.day() );
            fecha2.setYMD ( fechaact.year(), fechaact.month(), fechaact.day() );
            break;
        case 1:
            fecha1.setYMD ( fechaact.year(), fechaact.month(), 1 );
            fecha2.setYMD ( fechaact.year(), fechaact.month(), fechaact.daysInMonth() );
            break;
        case 2:
            fecha1.setYMD ( fechaact.year(), 1, 1 );
            fecha2.setYMD ( fechaact.year(), 12, 31 );
            break;
        } // end switch
        
        if ( !g_diario) {
            g_diario = new BcDiarioView ( g_pluginbf_diario->company(), 0 );
	    g_diario->setObjectName("BcDiarioView");
            g_pluginbf_diario->company()->pWorkspace()->addSubWindow ( g_diario );
	} // end if
        
        g_diario ->inicializa1 ( ( char * ) fecha1.toString ( "dd/MM/yyyy" ).toAscii().constData(), ( char * ) fecha2.toString ( "dd/MM/yyyy" ).toAscii().constData(), 0 );
        g_diario ->accept();
	g_diario->hide();
	g_diario->show();
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
    SubForm_Diario *subformods = new SubForm_Diario ( sub );
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
	SubForm_Diario *subformods = new SubForm_Diario ( sub );
	
	QHBoxLayout *m_hboxLayout1 = sub->mui_menusubform->findChild<QHBoxLayout *> ( "hboxLayout1" );
	if ( !m_hboxLayout1 ) {
	    m_hboxLayout1 = new QHBoxLayout ( sub->mui_menusubform );
	    m_hboxLayout1->setSpacing (0 );
	    m_hboxLayout1->setMargin ( 0 );
	    m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
	} // end if
	

	  QToolButton *sel = new QToolButton ( sub->mui_menusubform );
	  sel->setStatusTip ( "Diario (dia)" );
	  sel->setToolTip ( "Diario (dia)" );
	  sel->setMinimumSize ( QSize ( 18, 18 ) );
	  sel->setIcon ( QIcon ( ":/Images/balance-sheet.png" ) );
	  sel->setIconSize ( QSize ( 18, 18 ) );    
	  m_hboxLayout1->addWidget ( sel );
	  sel->connect (sel, SIGNAL(released()), subformods, SLOT(botonera()));
	
	  QToolButton *sel1 = new QToolButton ( sub->mui_menusubform );
	  sel1->setStatusTip ( "Diario (mes)" );
	  sel1->setToolTip ( "Diario (mes)" );
	  sel1->setMinimumSize ( QSize ( 18, 18 ) );
	  sel1->setIcon ( QIcon ( ":/Images/balance-sheet.png" ) );
	  sel1->setIconSize ( QSize ( 18, 18 ) );    
	  m_hboxLayout1->addWidget ( sel1 );
	  sel1->connect (sel1, SIGNAL(released()), subformods, SLOT(botonera1()));
	  
	  QToolButton *sel2 = new QToolButton ( sub->mui_menusubform );
	  sel2->setStatusTip ( "Diario (año)" );
	  sel2->setToolTip ( "Diario (año)" );
	  sel2->setMinimumSize ( QSize ( 18, 18 ) );
	  sel2->setIcon ( QIcon ( ":/Images/balance-sheet.png" ) );
	  sel2->setIconSize ( QSize ( 18, 18 ) );    
	  m_hboxLayout1->addWidget ( sel2 );
	  sel2->connect (sel2, SIGNAL(released()), subformods, SLOT(botonera2()));

    } // end if
    return 0;
}








