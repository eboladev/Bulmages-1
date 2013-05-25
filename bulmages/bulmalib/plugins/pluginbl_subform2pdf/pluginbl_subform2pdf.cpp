/***************************************************************************
 *   Copyright (C) 2007 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
 *                                                                         *
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QTextCodec>
#include <QtCore/QLocale>

#include "pluginbl_subform2pdf.h"


///
/**
\return
**/
int entryPoint ( QApplication * )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbl_subform2pdf", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    
    return 0;
}


///
/**
\param parent
**/
PluginBl_SubForm2PDF::PluginBl_SubForm2PDF ( BlSubForm *parent ) : QObject ( parent )
{
    BL_FUNC_DEBUG
    
}

///
/**
**/
PluginBl_SubForm2PDF::~PluginBl_SubForm2PDF()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu
**/
void PluginBl_SubForm2PDF::s_pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG

    QAction *accionA = new QAction  ( QIcon(":/Images/document-print.png"), _ ( "Imprimir" ), menu );
    accionA->setObjectName("mui_actionSubForm2PDF");

    menu->addSeparator();
    menu->addAction ( accionA );
    
}


///
/**
\param action
**/
void PluginBl_SubForm2PDF::s_trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG
    if ( action->objectName() == "mui_actionSubForm2PDF") {
        imprimir();
    } // end if
    
}


///
/**
**/
void PluginBl_SubForm2PDF::imprimir()
{
    BL_FUNC_DEBUG

    BlSubForm * subf = ( BlSubForm * ) parent();

    subf->printPDF ( "" );
    
}


///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    BL_FUNC_DEBUG
    PluginBl_SubForm2PDF *subformods = new PluginBl_SubForm2PDF ( sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    
    return 0;
}


///
/**
\param sub
\return
**/
int BlSubForm_preparaMenu ( BlSubForm *sub ) {
    BL_FUNC_DEBUG

    PluginBl_SubForm2PDF *subformods = new PluginBl_SubForm2PDF ( sub );
    
    QHBoxLayout *m_hboxLayout1 = sub->mui_menusubform->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( sub->mui_menusubform );
        m_hboxLayout1->setSpacing (0 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    

      QToolButton *sel = new QToolButton ( sub->mui_menusubform );
      sel->setStatusTip ( _("Imprimir") );
      sel->setToolTip ( _("Imprimir") );
      sel->setMinimumSize ( QSize ( 18, 18 ) );
      sel->setIcon ( QIcon ( ":/Images/document-print.png" ) );
      sel->setIconSize ( QSize ( 18, 18 ) );    
      m_hboxLayout1->addWidget ( sel );
      sel->connect (sel, SIGNAL(released()), subformods, SLOT(imprimir ( )));

    
    return 0;
}

