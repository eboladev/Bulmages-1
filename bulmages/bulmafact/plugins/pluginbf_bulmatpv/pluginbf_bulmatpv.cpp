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

#include <QAction>
#include <QObject>
#include <QMessageBox>
#include <QLineEdit>
#include <QTabWidget>

#include "bfcompany.h"
#include "pluginbf_bulmatpv.h"
#include "blfunctions.h"
#include "zlist.h"
#include "blform.h"
#include "articulotpv.h"
#include "familiastpv.h"

BfBulmaFact *g_pluginbf_bulmatpv = NULL;


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    g_pluginbf_bulmatpv = bges;
    
    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_bulmatpv", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    /// Miramos si existe un menu Ventas
    QMenu *pPluginMenu = NULL;
    pPluginMenu = bges->menuBar() ->findChild<QMenu *> ( "menuVentas" );
    
    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
        QMenu *pPluginVentas = NULL;
        pPluginVentas = bges->menuBar() ->findChild<QMenu *> ( "menuMaestro" );
        pPluginMenu = new QMenu ( _("&Ventas"), bges->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuVentas" ) );
        bges->menuBar()->insertMenu ( pPluginVentas->menuAction(), pPluginMenu );
    } // end if

    /// Creamos el men&uacute;.
    BlAction *accionA = new BlAction ( _("&Cuadres de caja"), 0 );
    accionA->setStatusTip ( _("Listado de cuadres de caja") );
    accionA->setWhatsThis ( _("Listado de cuadres de caja") );
    accionA->setObjectName("mui_actionCuadresCaja");
    
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( accionA );


    
    return 0;
}
int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    if (accion->objectName() == "mui_actionCuadresCaja") {
        ZList *vehiculoview = new ZList ( g_pluginbf_bulmatpv->company() );
        g_pluginbf_bulmatpv->company()->m_pWorkspace->addSubWindow ( vehiculoview );
        vehiculoview->show();
    } //end if
    
    return 0;
}


/**
\param trab
\return
**/
int ArticuloView_ArticuloView_Post ( ArticuloView *trab )
{
    BL_FUNC_DEBUG

    trab->addDbField ( "visibletpvarticulo", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Visible en TPV" ) );
    trab->addDbField ( "etiquetavisibletpvarticulo", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Etiqueta visible en TPV" ) );
    trab->addDbField ( "ordentpvarticulo", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Orden en TPV" ) );
    trab->addDbField ( "colortpvarticulo", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Color en TPV" ) );

    ArticuloTPV *a = new ArticuloTPV ( trab );
    a->setMainCompany ( trab->mainCompany() );
    a->setObjectName ( QString::fromUtf8 ( "m_articulotpv" ) );
    trab->mui_tab->addTab ( a, _ ( "T.P.V." ) );

    
    return 0;
}



int FamiliasView_FamiliasView ( FamiliasView *famv )
{
    FamiliasTPV *a = new FamiliasTPV ( famv );
    a->setMainCompany ( famv->mainCompany() );
    a->setObjectName ( QString::fromUtf8 ( "m_familiatpv" ) );
    famv->mui_tab->addTab ( a, _ ( "T.P.V." ) );

    return 0;
}



int FamiliasView_Guardar_Pre ( FamiliasView *famv )
{
    QString query;
  
    try {

	QString estado;
	
	if ( famv->findChild<QCheckBox *>("mui_visibletpvfamilia")->checkState() == Qt::Checked ) {
	  estado = "TRUE";
	} else {
	  estado = "FALSE";	  
	} // end if
      
	query = "UPDATE familia SET visibletpvfamilia = '" + estado + "', ordentpvfamilia = '" + famv->findChild<QLineEdit *>("mui_ordentpvfamilia")->text() + "', colortpvfamilia = '" + famv->findChild<QLineEdit *>("mui_colortpvfamilia")->text() + "'" + " WHERE idfamilia = " + famv->idFamiliaModified();
	famv->mainCompany()->runQuery(query);
      
    } catch (int e) {
	throw -1;
    } // end try

  
    return 0;
}


int FamiliasView_Guardar_Post ( FamiliasView *famv )
{
    return 0;
}


int FamiliasView_currentItemChanged_Post ( FamiliasView *famv )
{
    BL_FUNC_DEBUG

    BlDbRecordSet *rec;
    BlDbRecordSet *rec2;

    if ( famv->idFamilia().isEmpty() ) {
      famv->findChild<QCheckBox *>("mui_visibletpvfamilia")->setEnabled(FALSE);
      famv->findChild<QLineEdit *>("mui_ordentpvfamilia")->setEnabled(FALSE);
      famv->findChild<QLineEdit *>("mui_colortpvfamilia")->setEnabled(FALSE);
      famv->findChild<QPushButton *>("mui_seleccionarcolor")->setEnabled(FALSE);
      famv->findChild<QCheckBox *>("mui_visibletpvfamilia")->setCheckState(Qt::Checked);
      famv->findChild<QLineEdit *>("mui_ordentpvfamilia")->setText("");
      famv->findChild<QLineEdit *>("mui_colortpvfamilia")->setText("");

    } else {
      famv->findChild<QCheckBox *>("mui_visibletpvfamilia")->setEnabled(TRUE);
      famv->findChild<QLineEdit *>("mui_ordentpvfamilia")->setEnabled(TRUE);
      famv->findChild<QLineEdit *>("mui_colortpvfamilia")->setEnabled(TRUE);
      famv->findChild<QPushButton *>("mui_seleccionarcolor")->setEnabled(TRUE);
      
      famv->mainCompany()->begin();  
    
      QString query;

      query = "SELECT visibletpvfamilia, ordentpvfamilia, colortpvfamilia FROM familia WHERE idfamilia = '" + famv->idFamilia() + "' LIMIT 1";
      rec = famv->mainCompany()->loadQuery(query);
      
      // Si no hay datos no se hace nada.
      if ( rec != NULL ) {

	    if (rec->value( "visibletpvfamilia" ) == "f") {
		famv->findChild<QCheckBox *>("mui_visibletpvfamilia")->setCheckState(Qt::Unchecked);
	    } else {
		famv->findChild<QCheckBox *>("mui_visibletpvfamilia")->setCheckState(Qt::Checked);
	    } // end if
	    
	    famv->findChild<QLineEdit *>("mui_ordentpvfamilia")->setText(rec->value( "ordentpvfamilia" ));
	    famv->findChild<QLineEdit *>("mui_colortpvfamilia")->setText(rec->value( "colortpvfamilia" ));
	      
      } // end if
    
      famv->mainCompany()->commit();
      
    } // end if
    return 0;
}



