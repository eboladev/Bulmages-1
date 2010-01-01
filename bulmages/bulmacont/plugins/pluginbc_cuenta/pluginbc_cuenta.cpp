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

#include "pluginbc_cuenta.h"
#include "bccuentalistview.h"
#include "bcplancontablelistview.h"
#include "bccuentaview.h"

BcPlanContableListView *g_plancontable;

///
/**
**/
MyPluginCuenta::MyPluginCuenta()
{
    _depura ( "MyPluginCuenta::MyPluginCuenta", 0 );
    _depura ( "END MyPluginCuenta::MyPluginCuenta", 0 );
}


///
/**
**/
MyPluginCuenta::~MyPluginCuenta()
{
    _depura ( "MyPluginCuenta::~MyPluginCuenta", 0 );
    _depura ( "END MyPluginCuenta::~MyPluginCuenta", 0 );
}



///
/**
**/
void MyPluginCuenta::elslot()
{
    _depura ( "MyPluginCuenta::elslot", 0 );
    if (g_plancontable == NULL) {
      g_plancontable = new BcPlanContableListView ( ( BcCompany * ) mainCompany(), 0 );
      g_plancontable->inicializa();
      mainCompany() ->pWorkspace() ->addWindow ( g_plancontable );
    } // end if
    g_plancontable->hide();
    g_plancontable->show();
    _depura ( "END MyPluginCuenta::elslot", 0 );
}



///
/**
\param bges
**/
void MyPluginCuenta::inicializa ( BcBulmaCont *bges )
{
    _depura ( "MyPluginCuenta::inicializa", 0 );

    /// Creamos el men&uacute;.
    setMainCompany ( (BlMainCompany *)bges->empresaactual() );
    m_bulmacont = bges;
    QMenu *pPluginMenu;
    /// Miramos si existe un menu Herramientas
    pPluginMenu = bges->menuBar() ->findChild<QMenu *> ( _ ( "menuCuentas" ) );

    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
        pPluginMenu = new QMenu ( _ ( "&Cuentas Contables" ), bges->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuCuentas" ) );
    } // end if

    QAction *accion = new QAction ( _ ( "&Plan Contable" ), 0 );
    accion->setStatusTip ( _ ( "Permite ver y modificar el plan contable" ) );
    accion->setWhatsThis ( _ ( "Permite ver y modificar el plan contable" ) );
    accion->setIcon(QIcon(QString::fromUtf8(":/Images/account_plan.png")));
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    pPluginMenu->addAction ( accion );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar() ->insertMenu ( bges->menuMaestro->menuAction(), pPluginMenu );
    bges->toolBar->addAction ( accion );

    _depura ( "END MyPluginCuenta::inicializa", 0 );
}


///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    _depura ( "entryPoint::entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbc_cuenta", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_plancontable = NULL;

    MyPluginCuenta *plug = new MyPluginCuenta();
    plug->inicializa ( bcont );

    _depura ( "END entryPoint::entryPoint", 0 );
    return 0;
}

/// --------------------------------------------------------------
/// --------- Implemento la edicion de articulos -----------------
/// Con esta funcionalidad creamos menus contextuales en todos los subformularios donde
/// Aparezca el identificador de articulo como elemento y permite de forma sencilla
/// La creacion, la edicion, y la seleccion.
///

/**
\param parent
**/
MyPluginCuenta1::MyPluginCuenta1 ( BlSubForm *parent ) : QObject ( parent )
{
    _depura ( "MyPluginCuenta1::MyPluginCuenta1", 0 );
    _depura ( "END MyPluginCuenta1::MyPluginCuenta1", 0 );
}

///
/**
**/
MyPluginCuenta1::~MyPluginCuenta1()
{
    _depura ( "MyPluginCuenta1::~MyPluginCuenta1", 0 );
    _depura ( "END MyPluginCuenta1::~MyPluginCuenta1", 0 );
}


///
/**
\param menu
**/
void MyPluginCuenta1::s_pintaMenu ( QMenu *menu )
{
    _depura ( "MyPluginCuenta1::s_pintaMenu", 0 );
    BlSubForm *sub = ( BlSubForm * ) parent();
    BlSubFormHeader *header = sub->header ( "codigo" );
    if ( header ) {
        menu->addSeparator();
        menu->addAction ( _ ( "Nueva cuenta" ) );
        QString idcuenta = sub->dbValue ( "idcuenta" );
        if ( idcuenta != "" ) menu->addAction ( _ ( "Editar cuenta" ) );
        if ( ! ( header->options() & BlSubFormHeader::DbNoWrite ) )  {
            menu->addAction ( _ ( "Seleccionar cuenta" ) );
        } // end if
    } // end if
    _depura ( "END MyPluginCuenta1::s_pintaMenu", 0 );
}


///
/**
\param action
**/
void MyPluginCuenta1::s_trataMenu ( QAction *action )
{
    _depura ( "MyPluginCuenta1::s_trataMenu", 0 );
    BlSubForm *sub = ( BlSubForm * ) parent();
    if ( action->text() == _ ( "Editar cuenta" ) ) {
        QString idcuenta = sub->dbValue ( "idcuenta" );
        if ( idcuenta != "" )
            editarCuenta ( idcuenta );
    } else if ( action->text() == _ ( "Seleccionar cuenta" ) ) {
        seleccionarCuenta ( sub );
    } else if ( action->text() == _ ( "Nueva cuenta" )  ) {
        nuevoCuenta();
    } // end if

    _depura ( "END MyPluginCuenta1::s_trataMenu", 0 );
}


///
/**
**/
void MyPluginCuenta1::editarCuenta ( QString idcuenta )
{
    _depura ( "MyPluginCuenta1::editarCuenta", 0 );
        BcCuentaView * nuevae = new BcCuentaView ( ((BcBulmaCont *)g_main)->empresaactual(), 0 );
        nuevae->cargar ( idcuenta );
        ((BcBulmaCont *)g_main)->empresaactual() ->pWorkspace() ->addWindow ( nuevae );
        nuevae->show();
    _depura ( "END MyPluginCuenta1::editarCuenta", 0 );
}


///
/**
**/
void MyPluginCuenta1::nuevoCuenta( )
{
    _depura ( "MyPluginCuenta1::editarCuenta", 0 );
        BcCuentaView * nuevae = new BcCuentaView ( ((BcBulmaCont *)g_main)->empresaactual(), 0 );
        ((BcBulmaCont *)g_main)->empresaactual() ->pWorkspace() ->addWindow ( nuevae );
        nuevae->mui_padre->setEnabled(TRUE);
        nuevae->show();
    _depura ( "END MyPluginCuenta1::editarCuenta", 0 );
}


///
/**
**/
void MyPluginCuenta1::seleccionarCuenta ( BlSubForm *sub )
{
    _depura ( "MyPluginCuenta1::editarCuenta", 0 );
/*
    CuentaList *artlist = new CuentaList ( ( BfCompany * ) sub->mainCompany(), NULL, 0, BL_SELECT_MODE );
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    sub->setEnabled ( false );
    artlist->show();
    while ( !artlist->isHidden() )
        g_theApp->processEvents();
    sub->setEnabled ( true );
    QString idArticle = artlist->idcuenta();
    delete artlist;

    /// Si no tenemos un idcuenta salimos ya que significa que no se ha seleccionado ninguno.
    if ( idArticle == "" ) {
        _depura ( "END BlSubForm::pressedAsterisk", 0 );
        return;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM articulo WHERE idcuenta = " + idArticle );
    if ( !cur->eof() ) {
        sub->lineaact()->setDbValue ( "idcuenta", idArticle );
        sub->lineaact()->setDbValue ( "codigocompletoarticulo", cur->valor ( "codigocompletoarticulo" ) );
        sub->lineaact()->setDbValue ( "nomarticulo", cur->valor ( "nomarticulo" ) );
    } // end if
    delete cur;
*/
    _depura ( "END MyPluginCuenta1::editarCuenta", 0 );
}


///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    _depura ( "BlSubForm_BlSubForm_Post", 0 );
    MyPluginCuenta1 *subformods = new MyPluginCuenta1 ( sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    _depura ( "END BlSubForm_BlSubForm_Post", 0 );
    return 0;
}


int Busqueda_on_mui_buscar_released ( BlSearchWidget *busq )
{
    _depura("pluginbc_cuenta::Busqueda_on_mui_buscar_released");
    int val = 0;
    if ( busq->tableName() == "cuenta" ) {

        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        centrarEnPantalla ( diag );

//        ClientsList *listcuentas = new ClientsList ( ( BfCompany * ) busq->mainCompany(), diag, 0, BL_SELECT_MODE );
        /// Creamos una instancia del selector de cuentas.
        BcCuentaListView *listcuentas = new BcCuentaListView ( ( BcCompany * ) busq->mainCompany(), diag, 0, BL_SELECT_MODE );


        busq->connect ( listcuentas, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

        /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
        /// para que sea redimensionable y aparezca el titulo de la ventana.
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget ( listcuentas );
        layout->setMargin ( 0 );
        layout->setSpacing ( 0 );
        diag->setLayout ( layout );
        diag->setWindowTitle ( listcuentas->windowTitle() );

        diag->exec();

        if ( listcuentas->idcuenta() != "" ) {
            busq->setId ( listcuentas->idcuenta() );
        } // end if

        delete diag;


        val = 1;
    } // end if

    _depura("END pluginbc_cuenta::Busqueda_on_mui_buscar_released");
    return val;
}


