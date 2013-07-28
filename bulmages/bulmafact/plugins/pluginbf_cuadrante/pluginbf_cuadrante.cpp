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

#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtCore/QObject>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>
#include <QtCore/QTextCodec>
#include <QtCore/QLocale>

#include "pluginbf_cuadrante.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "cuadranteview.h"
#include "cuadrantediarioview.h"

BfBulmaFact *g_pluginbf_cuadrante = NULL;

///
/**
\param bges
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_Cuadrante", "0.9.3-0002");

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_cuadrante", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );
    g_pluginbf_cuadrante = bges;

    /// Creamos el men&uacute;.
    QMenu *pPluginMenu;
    
    /// Miramos si existe un menu Herramientas
    pPluginMenu = bges->menuBar() ->findChild<QMenu *> ( "Herramientas" );

    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
        pPluginMenu = new QMenu ( _("&Herramientas"), bges->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "Herramientas" ) );
    } // end if

    BlAction *accionA = new BlAction ( _("&Cuadrante Semanal"), 0 );
    accionA->setStatusTip ( _("Permite realizar Plannings Laborales") );
    accionA->setWhatsThis ( _("Podra destinar los trabajadores a distintos almacenes") );
    accionA->setObjectName("mui_actionCuadranteSemanal");
    pPluginMenu->addAction ( accionA );

    BlAction *accionB = new BlAction ( _("&Cuadrante Diario"), 0 );
    accionB->setStatusTip ( _("Permite realizar Plannings Laborales") );
    accionB->setWhatsThis ( _("Podra destinar los trabajadores a distintos almacenes") );
    accionB->setObjectName("mui_actionCuadranteDiario");
    pPluginMenu->addAction ( accionB );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar() ->insertMenu ( bges->menuVentana->menuAction(), pPluginMenu );

    
    return 0;
}



int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, "pluginbf_cuadrante" );
    if (accion->objectName() == "mui_actionCuadranteSemanal") {
        CuadranteView *cuad = new CuadranteView ( g_pluginbf_cuadrante->company(), 0 );
        g_pluginbf_cuadrante->company()->pWorkspace() ->addSubWindow ( cuad );
        cuad->show();
    } // end if
    if (accion->objectName() == "mui_actionCuadranteDiario") {
        CuadranteDiarioView *cuad = new CuadranteDiarioView ( g_pluginbf_cuadrante->company(), 0 );
        g_pluginbf_cuadrante->company()->pWorkspace() ->addSubWindow ( cuad );
        cuad->show();
    } // end if

    
    return 0;
}



///
/**
\param alm
\return
**/
int AlmacenView_AlmacenView ( AlmacenView *alm )
{
    BL_FUNC_DEBUG

    alm->addDbField ( "aperturaalmacen", BlDbField::DbVarChar, BlDbField::DbNothing,  "Apertura Manyana" );
    alm->addDbField ( "cierrealmacen", BlDbField::DbVarChar, BlDbField::DbNothing, "Cierre Manyana" );
    alm->addDbField ( "apertura1almacen", BlDbField::DbVarChar, BlDbField::DbNothing,  "Apertura Tarde" );
    alm->addDbField ( "cierre1almacen", BlDbField::DbVarChar, BlDbField::DbNothing, "Cierre Tarde" );


// ---------------
    /// Creamos el horario de manyana.
    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing ( 2 );
    hboxLayout160->setMargin ( 0 );
    hboxLayout160->setObjectName ( QString::fromUtf8 ( "hboxLayout16" ) );

    /// Apertura
    QLabel *textLabel2_9_26 = new QLabel ( alm->mui_frameplugin );
    textLabel2_9_26->setObjectName ( QString::fromUtf8 ( "textLabel2_9_2" ) );
    textLabel2_9_26->setText ( "Apertura Manyanas" );
    hboxLayout160->addWidget ( textLabel2_9_26 );

    QLineEdit *horain = new QLineEdit ( alm );
    horain->setObjectName ( "mui_aperturaalmacen" );
    hboxLayout160->addWidget ( horain );

    /// Cierre
    QLabel *textLabel2_9_27 = new QLabel ( alm->mui_frameplugin );
    textLabel2_9_27->setText ( "Cierre Manyanas" );
    hboxLayout160->addWidget ( textLabel2_9_27 );

    QLineEdit *horafin = new QLineEdit ( alm );
    horafin->setObjectName ( "mui_cierrealmacen" );

    hboxLayout160->addWidget ( horafin );


// ---------------

// ---------------
    /// Creamos el horario de tardes.
    QHBoxLayout *hboxLayout170 = new QHBoxLayout();
    hboxLayout170->setSpacing ( 2 );
    hboxLayout170->setMargin ( 0 );
    hboxLayout170->setObjectName ( QString::fromUtf8 ( "hboxLayout17" ) );

    /// Apertura
    QLabel *textLabel2_9_28 = new QLabel ( alm->mui_frameplugin );
    textLabel2_9_28->setText ( "Apertura Tardes" );
    hboxLayout170->addWidget ( textLabel2_9_28 );

    QLineEdit *horaint = new QLineEdit ( alm );
    horaint->setObjectName ( "mui_apertura1almacen" );
    hboxLayout170->addWidget ( horaint );

    /// Cierre
    QLabel *textLabel2_9_29 = new QLabel ( alm->mui_frameplugin );
    textLabel2_9_29->setText ( "Cierre Tardes" );
    hboxLayout170->addWidget ( textLabel2_9_29 );

    QLineEdit *horafint = new QLineEdit ( alm );
    horafint->setObjectName ( "mui_cierre1almacen" );

    hboxLayout170->addWidget ( horafint );


// ---------------


    /// Comprobamos que exista el layout.
    QVBoxLayout *m_hboxLayout1 = alm->mui_frameplugin->findChild<QVBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QVBoxLayout ( alm->mui_frameplugin );
        m_hboxLayout1->setSpacing ( 0 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
//       m_hboxLayout1->addWidget(horain);
    m_hboxLayout1->addLayout ( hboxLayout160 );
    m_hboxLayout1->addLayout ( hboxLayout170 );
    return 0;
}


///
/**
\param trab
\return
**/
int TrabajadorView_TrabajadorView ( TrabajadorView *trab )
{
    BL_FUNC_DEBUG
    BfSubForm *l = new BfSubForm ( trab );
    l->setObjectName ( QString::fromUtf8 ( "m_ausencias" ) );
    l->setMainCompany ( trab->mainCompany() );
    l->setDbTableName ( "ausencia" );
    l->setDbFieldId ( "idausencia" );
    l->addSubFormHeader ( "fechainausencia", BlDbField::DbVarChar, BlDbField::DbNothing , BlSubFormHeader::DbNone, _ ( "Fecha inicial ausencia" ) );
    l->addSubFormHeader ( "fechafinausencia", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone , _ ( "Fecha final ausencia" ) );
    l->addSubFormHeader ( "motivoausencia", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone , _ ( "Motivo" ) );
    l->addSubFormHeader ( "idausencia", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID ausencia" ) );
    l->addSubFormHeader ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID trabajador" ) );
    l->setInsert ( true );
    l->setDelete ( true );
    l->setSortingEnabled ( false );
    trab->mui_tab->addTab ( l, "Ausencias" );
    trab->dialogChanges_setExcludedObject ( l->mui_list );

    
    return 0;
}


///
/**
\param trab
\return
**/
int TrabajadorView_on_mui_lista_currentItemChanged_Post ( TrabajadorView *trab )
{
    BL_FUNC_DEBUG

    BfSubForm *l = trab->findChild<BfSubForm *> ( "m_ausencias" );
    if (l) {
      l->load ( "SELECT * FROM ausencia WHERE idtrabajador = " + trab->mdb_idtrabajador );
    } // end if
    
    return 0;
}


///
/**
\param trab
\return
**/
int TrabajadorView_on_mui_guardar_clicked ( TrabajadorView *trab )
{
    BfSubForm * l = trab->findChild<BfSubForm *> ( "m_ausencias" );
    if (l) {
      l->setColumnValue ( "idtrabajador", trab->mdb_idtrabajador );
      l->save();
    } // end if
    return 0;
}

