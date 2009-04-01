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

#include <QMenu>
#include <QAction>
#include <QObject>
#include <QMessageBox>
#include <QInputDialog>
#include <QStringList>
#include <QTextCodec>
#include <QLocale>

#include "pluginbf_cuadrante.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "cuadranteview.h"
#include "cuadrantediarioview.h"


///
/**
**/
myplugin4::myplugin4()
{
    _depura ( "myplugin4::myplugin4", 0 );
    _depura ( "END myplugin4::myplugin4", 0 );
}


///
/**
**/
myplugin4::~myplugin4()
{
    _depura ( "myplugin4::~myplugin4", 0 );
    _depura ( "END myplugin4::~myplugin4", 0 );
}


///
/**
**/
void myplugin4::elslot()
{
    _depura ( "myplugin4::elslot", 0 );
    CuadranteView *cuad = new CuadranteView ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->pWorkspace() ->addWindow ( cuad );
    cuad->show();
    _depura ( "END myplugin4::elslot", 0 );
}


///
/**
**/
void myplugin4::elslot1()
{
    _depura ( "myplugin4::elslot1", 0 );
    CuadranteDiarioView *cuad = new CuadranteDiarioView ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->pWorkspace() ->addWindow ( cuad );
    cuad->show();
    _depura ( "END myplugin4::elslot1", 0 );
}


///
/**
\param bges
**/
void myplugin4::inicializa ( Bulmafact *bges )
{
    _depura ( "myplugin4::inicializa", 0 );
    /// Creamos el men&uacute;.
    setMainCompany ( bges->getcompany() );
    m_bulmafact = bges;
    QMenu *pPluginMenu;
    /// Miramos si existe un menu Herramientas
    pPluginMenu = bges->menuBar() ->findChild<QMenu *> ( "Herramientas" );

    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
        pPluginMenu = new QMenu ( "&Herramientas", bges->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "Herramientas" ) );
    } // end if

    QAction *accion = new QAction ( "&Cuadrante Semanal", 0 );
    accion->setStatusTip ( "Permite realizar Plannings Laborales" );
    accion->setWhatsThis ( "Podra destinar los trabajadores a distintos almacenes" );
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    pPluginMenu->addAction ( accion );

    QAction *accion1 = new QAction ( "&Cuadrante Diario", 0 );
    accion1->setStatusTip ( "Permite realizar Plannings Laborales" );
    accion1->setWhatsThis ( "Podra destinar los trabajadores a distintos almacenes" );
    connect ( accion1, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );
    pPluginMenu->addAction ( accion1 );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar() ->insertMenu ( bges->menuVentana->menuAction(), pPluginMenu );
    _depura ( "END myplugin4::inicializa", 0 );
}


///
/**
\param bges
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginbf_cuadrante", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    myplugin4 *plug = new myplugin4();
    plug->inicializa ( bges );

    _depura ( "END entryPoint", 0 );
    return 0;
}


///
/**
\param alm
\return
**/
int AlmacenView_AlmacenView ( AlmacenView *alm )
{
    _depura ( "esxtoy en la clase almacen", 0 );

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
    _depura ( "TrabajadorView_TrabajadorView", 0 );
    BfSubForm *l = new BfSubForm ( trab );
    l->setObjectName ( QString::fromUtf8 ( "m_ausencias" ) );
    l->setMainCompany ( trab->mainCompany() );
    l->setDbTableName ( "ausencia" );
    l->setDbFieldId ( "idausencia" );
    l->addSubFormHeader ( "fechainausencia", BlDbField::DbVarChar, BlDbField::DbNothing , BlSubFormHeader::DbNone, _( "Fecha inicial ausencia" ) );
    l->addSubFormHeader ( "fechafinausencia", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone , _( "Fecha final ausencia" ) );
    l->addSubFormHeader ( "motivoausencia", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone , _( "Motivo" ) );
    l->addSubFormHeader ( "idausencia", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _( "ID ausencia" ) );
    l->addSubFormHeader ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _( "ID trabajador" ) );
    l->setinsercion ( TRUE );
    l->setDelete ( TRUE );
    l->setSortingEnabled ( FALSE );
    trab->mui_tab->addTab ( l, "Ausencias" );
    trab->dialogChanges_setQObjectExcluido ( l->mui_list );

    _depura ( "END TrabajadorView_TrabajadorView", 0 );
    return 0;
}


///
/**
\param trab
\return
**/
int TrabajadorView_on_mui_lista_currentItemChanged_Post ( TrabajadorView *trab )
{
    _depura ( "TrabajadorView_on_mui_lista_currentItemChanged_Post", 0 );
    BfSubForm *l = trab->findChild<BfSubForm *> ( "m_ausencias" );
    l->cargar ( "SELECT * FROM ausencia WHERE idtrabajador = " + trab->idtrabajador() );
    _depura ( "END TrabajadorView_on_mui_lista_currentItemChanged_Post", 0 );
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
    l->setColumnValue ( "idtrabajador", trab->idtrabajador() );
    l->guardar();
    return 0;
}

