/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2010 by Fco. Javier M. C.                               *
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

#include "blmaincompany.h"
#include "blcompanydialog.h"
#include "blcountryview.h"


///
/**
\param doc
**/
void BlMainCompany::setListVentanas ( BlWindowListDock *doc )
{
    BL_FUNC_DEBUG
    m_windowListDock = doc;
    
}


///
/**
\return
**/
BlWorkspace *BlMainCompany::pWorkspace()
{
    BL_FUNC_DEBUG
    
    return m_pWorkspace;
}


///
/**
\return
**/
BlWindowListDock *BlMainCompany::listVentanas()
{
    BL_FUNC_DEBUG
    
    return m_windowListDock;
}




///
/**
\param qw
**/
void BlMainCompany::setWorkspace ( BlWorkspace *qw )
{
    BL_FUNC_DEBUG
    m_pWorkspace = qw;
    connect( qw, SIGNAL( deselectDockAll() ), this, SLOT( deselectWindow() ) );
    
}


///
/**
\param pb
**/
void BlMainCompany::setProgressBar ( QProgressBar *pb )
{
    BL_FUNC_DEBUG
    m_progressbar = pb;
}


///
/**
\param nom
\param obj
\param compdup
\return
**/
int BlMainCompany::insertWindow ( QString nom, QObject *obj, bool compdup, QString titulo )
{
    BL_FUNC_DEBUG
    return m_windowListDock->insertWindow ( nom, obj, compdup, titulo );
}


///
/**
\param nom
\param obj
\return
**/
#ifdef AREA_QMDI
  int BlMainCompany::selectWindow ( QString nom, QMdiSubWindow *obj )
#else
  int BlMainCompany::selectWindow ( QString nom, QObject *obj )
#endif
{
    BL_FUNC_DEBUG
    
    return m_windowListDock->selectWindow ( nom, obj );
}


///
/**
\return
**/
int BlMainCompany::deselectWindow()
{
    BL_FUNC_DEBUG
    
    return m_windowListDock->deselectWindow();
}


///
/**
\param nom
**/
void BlMainCompany::removeWindow ( QObject *nom )
{
    BL_FUNC_DEBUG
    m_windowListDock->removeWindow ( nom );
    
}


/// No precisa de operaciones en su construccion.
/**
**/
BlMainCompany::BlMainCompany()
{
  BL_FUNC_DEBUG 
  
}


/// El destructor de la clase BlMainCompany borra toda la memoria almacenada.
/**
**/
BlMainCompany::~BlMainCompany()
{
    BL_FUNC_DEBUG
    /// cerramos todas las ventanas y las DestructiveClose se borran.
    m_windowListDock->vaciar();
    /// \NOTE: El vaciado de las ventanas debe hacerse en la clase hija ya que el destructor de la clase derivada se ejecuta primero y se generaria un segfault.
    
}


/// Inicializa la base de datos que se pasa, si se pasa una cadena vacia
/// entonces invoca el selector de empresa.
/**
\param bd
\param tipo
**/
void BlMainCompany::init ( QString bd, QString tipo )
{
    BL_FUNC_DEBUG
    if ( bd == "" ) {
        bd = searchCompany ( tipo );
    } // end if
    /// Hacemos visible el ProgressBar mientras se habre la base de datos y se cargan
    /// los datos de la en las ventanas.
    m_progressbar->setVisible ( true );

    inicializa ( bd );
    
    g_plugins->run("BlMainCompany_init", this);
}


/// Se utiliza para mostrar un selector de empresas BlCompanyDialog
/// Al usuario debe seleccionar una empresa y el sistema empieza la inicializacion de
/// clases a partir de dicha inicializacion.
/**
\param tipo
\return
**/
QString BlMainCompany::searchCompany ( QString tipo )
{
    /// El cambio de empresa se realiza desde el selector.
    BL_FUNC_DEBUG
    BlCompanyDialog *nuevae = new BlCompanyDialog ( 0, tipo );
    nuevae->exec();
    BlDebug::blDebug ( "Vamos a cambiar la empresa", 0 );
    QString bd = nuevae->dbName();
    BlDebug::blDebug ( "Empresa cambiada a " + bd, 0 );
    delete nuevae;
    /// Si no se ha seleccionado ninguna base de datos entonces abortamos.
    if ( bd == "" ) {
        exit ( 1 );
    } // end if
    
    return bd;
}


/// Cambia el estado de la ventana de Indexador. Visible o no visible.
/**
\param visible
**/
void BlMainCompany::s_indexadorCambiaEstado ( bool visible )
{
    BL_FUNC_DEBUG
    m_windowListDock->setVisibilityState ( visible );
    
}


///
/**
**/
void BlMainCompany::muestraPaises()
{
    BL_FUNC_DEBUG
    BlCountryView *pais = new BlCountryView ( this, 0 );
    m_pWorkspace->addSubWindow ( pais );
    pais->show();
}


///
/**
**/
void BlMainCompany::dbPatchVersionCheck(QString plugin, QString version)
{
    BL_FUNC_DEBUG
    QString query = "SELECT * FROM configuracion WHERE nombre = '" + sanearCadena(plugin) + "'";
    BlDbRecordSet *rs;
    rs = loadQuery ( query );

    if ( rs != NULL ) {
      if (rs->value("valor") != version) {
        /// La version del plugin no coincide con la version del parche instalado en la base de datos.
        blMsgError(_("La base de datos no esta preparada para el plugin:") + " " + plugin + ".\n" + _("El programa no funcionara adecuadamente.") );
      } // end if
    } // end if

    delete rs;
}


///
/**
**/
void BlMainCompany::dbVersionCheck(QString program, QString version)
{
    BL_FUNC_DEBUG
    QString query = "SELECT * FROM configuracion WHERE nombre = '" + sanearCadena(program) + "'";
    BlDbRecordSet *rs;
    rs = loadQuery ( query );

    if ( rs != NULL ) {
      if (rs->value("valor") != version) {
        /// La version del programa no coincide con la version de la base de datos instalada.
        blMsgError(_("La version de la base de datos no coincide con el programa.\n\nEl programa no funcionara adecuadamente.\n")+program+"\n"+version+"\n" );
      } // end if
    } // end if

    delete rs;
}


/// Ejecuta la acci&oacute;n 'show()' sobre una ventana. Si no se encuentra la ventana devuelve 'false'.
/// Sirve tambi&eacute;n para comprobar la existencia de la ventana y as&iacute; evitar ventanas duplicadas.
/**
**/
bool BlMainCompany::showWindow(QString objectName)
{
    BL_FUNC_DEBUG
    bool found = false;
    
    /// Busca ventanas con el objectName.
    for (int i = 0; i < m_windowListDock->numVentanas(); i++) {

	if (m_windowListDock->ventana(i)->objectName() == objectName) {
	    found = true;
	    /// Si esta maximizada solo se muestra, en caso de estar minimizada se restaura.
	    /// Tambi&eacute;n hace que se muestre en primer plano.
	    if ( ( ( QWidget * ) m_windowListDock->ventana(i) )->isMaximized() ) {
		( ( QWidget * ) m_windowListDock->ventana(i) )->showMaximized();
	    } else {
		( ( QWidget * ) m_windowListDock->ventana(i) )->showNormal();
	    } // end if
	    
	    #ifdef AREA_QMDI
		m_pWorkspace->setActiveSubWindow(( QMdiSubWindow *)m_windowListDock->ventana(i));
	    #else
		m_pWorkspace->setActiveWindow(( QWidget * )m_windowListDock->ventana(i));
	    #endif
	    
	} // end if

    } // end for

    return found;
}

