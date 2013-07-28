/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtCore/QObject>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>
#include <QtGui/QClipboard>
#include <QtCore/QTextStream>
#include <QtCore/QTextCodec>
#include <QtCore/QLocale>

#include "blmaincompany.h"
#include "pluginbl_clipboard.h"


///
/**
\return
**/
int entryPoint ( QApplication * )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbl_clipboard", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    
    return 0;
}


///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    BL_FUNC_DEBUG
    PluginBl_Clipboard *subformclip = new PluginBl_Clipboard ( sub );
    sub->connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformclip, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformclip, SLOT ( s_trataMenu ( QAction * ) ) );
    
    return 0;
}


///
/**
\param parent
**/
PluginBl_Clipboard::PluginBl_Clipboard ( BlSubForm *parent ) : QObject ( parent )
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
PluginBl_Clipboard::~PluginBl_Clipboard()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu
**/
void PluginBl_Clipboard::s_pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG

    BlSubForm *subform = ( BlSubForm * ) parent();

    menu->addSeparator();

    if (subform->isInsert() ) {
        menu->addAction (QIcon( ":/Images/clipboard.png" ), _ ( "Pegar desde hoja de calculo" ) );
    } else {
        menu->addAction (QIcon( ":/Images/clipboard.png" ), _ ( "Actualizar desde hoja de calculo" ) );
    } // end if

    
}


///
/**
\param action
**/
void PluginBl_Clipboard::s_trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG

    if ( action->text() == _ ( "Pegar desde hoja de calculo" ) ) {
        if ( g_theApp->clipboard() ->text().contains ( "\t" ) ) {
            pegaODS();
        } else {
            pegaSXC();
        } // end if
    } else if ( action->text() == _ ( "Actualizar desde hoja de calculo" ) ) {
        if ( g_theApp->clipboard() ->text().contains ( "\t" ) ) {
            pegaActualizaODS();
        } else {
        } // end if
    } // end if
    
    
}


/// Funcion que pega desde KSpread, que copia al portapapeles la informacion con campos de tamanyo fijo.
/**
**/
void PluginBl_Clipboard::pegaSXC()
{
    BL_FUNC_DEBUG
    BlSubForm *subform = ( BlSubForm * ) parent();
    QString clipboard = g_theApp->clipboard() ->text();

    QStringList lineas = clipboard.split ( "\n" );

    /// La primera linea tiene los nombres de las columnas.
    QStringList campos = lineas.at ( 0 ).simplified().split ( " " );

    /// Calculamos el tamanyo de cada campo.
    int numcampos = campos.size();
    int numchars  = lineas.at ( 0 ).size() / numcampos;

    /// Iteramos para cada linea
    for ( int i = 1; i < lineas.size() - 1 ; ++i ) {
        QString cadena_valores = lineas.at ( i );

        /// Creamos un elemento en la factura
        BlDbSubFormRecord  *linea1;
        linea1 = subform->lineaat ( subform->rowCount() - 1 );
        /// Haciendo el nuevo registro antes nos evitamos problemas de foco.
        subform->newRecord();

        /// Iteramos para cada columna.
        for ( int j = 0; j < numcampos; ++j ) {
            /// Cogemos un valor.
            QString valorcampo = cadena_valores.left ( numchars ).simplified();
            cadena_valores = cadena_valores.right ( cadena_valores.size() - numchars );
            linea1->setDbValue ( campos.at ( j ), valorcampo );
        } // end for
    } // end for
    
}


/// Funcion que pega desde OpenOffice.org, que copia al portapapeles la informacion con separador de campos un tabulador.
/**
**/
void PluginBl_Clipboard::pegaActualizaODS()
{
    BL_FUNC_DEBUG

    BlSubForm *subform = ( BlSubForm * ) parent();
    QString clipboard = g_theApp->clipboard() ->text();

    try {
      subform->mainCompany() ->begin();
 
      QStringList lineas = clipboard.split ( "\n" );

      /// La primera linea tiene los nombres de las columnas.
  // QStringList campos = lineas.at(0).simplified().split(" ");
      QStringList campos = lineas.at ( 0 ).split ( "\t" );

      /// Calculamos el tamanyo de cada campo.
      int numcampos = campos.size();

      QString query;
 
      /// Iteramos para cada linea
      for ( int i = 1; i < lineas.size() - 1 ; ++i ) {
	  QStringList campos_valores = lineas.at ( i ).split ( "\t" );

	  
	  query = "UPDATE " + subform->tableName() + " SET ";
	  QString field_indice = "";

	  
	  /// Iteramos para cada columna.
	  for ( int j = 0; j < numcampos; ++j ) {
	    
	      /// Cogemos un valor.
	      query += subform->mainCompany()->sanearCadena( campos.at (j) ) + " = '" +  subform->mainCompany()->sanearCadena( campos_valores.at (j) ) + "' ";
	      
	      if ( campos.at (j) == subform->dbFieldId() ) {
		  field_indice = campos_valores.at (j);
	      } // end if
	      
	      if (j < numcampos - 1) {
		  query += ", ";
	      } // end if
	      
	  } // end for
	  

	  query += " WHERE " + subform->dbFieldId() + " = '" + field_indice + "';";
	  int res = subform->mainCompany() ->runQuery(query);
 
      } // end for
      
      subform->mainCompany() ->commit();
      
      blMsgInfo(_("Actualizacion completada con exito."));
      
    } catch (...) {
      
	  subform->mainCompany() ->rollback();
      
    } // end try
    


    
}



/// Funcion que pega desde OpenOffice.org, que copia al portapapeles la informacion con separador de campos un tabulador.
/**
**/
void PluginBl_Clipboard::pegaODS()
{
    BL_FUNC_DEBUG
    BlSubForm *subform = ( BlSubForm * ) parent();
    QString clipboard = g_theApp->clipboard() ->text();

    QStringList lineas = clipboard.split ( "\n" );

    /// La primera linea tiene los nombres de las columnas.
// QStringList campos = lineas.at(0).simplified().split(" ");
    QStringList campos = lineas.at ( 0 ).split ( "\t" );

    /// Calculamos el tamanyo de cada campo.
    int numcampos = campos.size();

    /// Iteramos para cada linea
    for ( int i = 1; i < lineas.size() - 1 ; ++i ) {
        QStringList campos_valores = lineas.at ( i ).split ( "\t" );

        /// Creamos un elemento en la factura
        BlDbSubFormRecord  *linea1;
        linea1 = subform->lineaat ( subform->rowCount() - 1 );
        /// Haciendo el nuevo registro antes nos evitamos problemas de foco.
        subform->newRecord();

        /// Iteramos para cada columna.
        for ( int j = 0; j < numcampos; ++j ) {
            /// Cogemos un valor.
            linea1->setDbValue ( campos.at ( j ), campos_valores.at ( j ) );
        } // end for
    } // end for
    
}

///
/**
\param sub
\return
**/
int BlSubForm_preparaMenu ( BlSubForm *sub ) {
    BL_FUNC_DEBUG


    if (sub->isInsert() ) {
    QHBoxLayout *m_hboxLayout1 = sub->mui_menusubform->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( sub->mui_menusubform );
        m_hboxLayout1->setSpacing (0 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    

      BlToolButton *sel = new BlToolButton ( sub->mui_menusubform );
      sel->setStatusTip ( _ ( "Pegar desde hoja de calculo" ) );
      sel->setToolTip ( _ ( "Pegar desde hoja de calculo" ) );
      sel->setMinimumSize ( QSize ( 18, 18 ) );
      sel->setIcon ( QIcon ( ":/Images/clipboard.png" ) );
      sel->setIconSize ( QSize ( 18, 18 ) );    
      sel->setObjectName("mui_clipboardinput");
      m_hboxLayout1->addWidget ( sel );

    } // end if
    
    return 0;
}



int BlToolButton_released(BlToolButton *bot) {

  if (bot->objectName() == "mui_clipboardinput") {
	PluginBl_Clipboard clip( (BlSubForm *) bot->parent()->parent());
        if ( g_theApp->clipboard() ->text().contains ( "\t" ) ) {
            clip.pegaODS();
        } else {
            clip.pegaSXC();
        } // end if
  } //end if
  return 0;
}
