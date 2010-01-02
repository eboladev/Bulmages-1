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
    CuentaList *artlist = new CuentaList ( ( BcCompany * ) sub->mainCompany(), NULL, 0, BL_SELECT_MODE );
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

//        ClientsList *listcuentas = new ClientsList ( ( BcCompany * ) busq->mainCompany(), diag, 0, BL_SELECT_MODE );
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


/// ======================================================

int BlSubFormDelegate_createEditor ( BlSubFormDelegate *bl )
{
    _depura ( "pluginbc_cuenta::BlSubFormDelegate_createEditor", 0 );
    int ret = 0;
    if ( g_nomcampo == "codigo" || g_nomcampo == "codigo1" ) {
        BlDbCompleterComboBox * editor = new BlDbCompleterComboBox ( g_editor );
        editor->setObjectName ( "EditCodigoCuenta" );
        editor->setMainCompany ( ( BcCompany * ) bl->m_subform->mainCompany() );
        editor->m_valores["descripcion"] = "";
        editor->m_valores["codigo"] = "";
        editor->m_tabla = "cuenta";
        g_plugParams =  editor;
        ret = -1;
    } // end if

    _depura ( "END pluginbc_cuenta::BlSubFormDelegate_createEditor", 0 );

    return ret;
}



/// Hay cosas que deberian estar en el plugin de cuenta
int BlSubFormDelegate_setModelData ( BlSubFormDelegate *bl )
{
    _depura ( "pluginbc_cuenta::BlSubFormDelegate_setModelData", 0 );
    int ret = 0;
    if ( g_editor->objectName() == "EditCodigoCuenta" || g_editor->objectName() == "EditNombreCliente" ) {
        BlDbCompleterComboBox * comboBox = ( BlDbCompleterComboBox * ) g_editor;
        QString value = comboBox->currentText();
        value = value.left ( value.indexOf ( ".-" ) );
        g_model->setData ( g_index, value );
        ret = -1;
    } // end if
    _depura ( "END pluginbc_cuenta::BlSubFormDelegate_setModelData", 0 );
    return ret;
}


int BlSubFormDelegate_setEditorData ( BlSubFormDelegate *bl )
{
    _depura ( "pluginbc_cuenta::BlSubFormDelegate_setEditorData", 0 );
    int ret = 0;
    if ( g_editor->objectName() == "EditCodigoCuenta" || g_editor->objectName() == "EditNombreCliente" ) {
        QString value = g_index.model() ->data ( g_index, Qt::DisplayRole ).toString();
        BlDbCompleterComboBox *comboBox = ( BlDbCompleterComboBox * ) g_editor ;
        comboBox->addItem ( value );
        ret = -1;
    } // end if
    _depura ( "END pluginbc_cuenta::BlSubFormDelegate_setEditorData", 0 );
    return ret;
}

int BlSubForm_editFinished ( BlSubForm *sub )
{
    _depura ( "pluginbc_cuenta::BlSubForm_editFinished", 0 );
    if ( sub->m_campoactual->nomcampo() == "codigo" ) {
   QString query = "SELECT idcuenta FROM cuenta WHERE upper (descripcion ||  ' ' || codigo) LIKE upper('" + sub->m_campoactual->text() + "%')";
// mensajeInfo(query);
        BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idcuenta", cur->valor ( "idcuenta" ) );
        } // end if
        delete cur;
    } // end if
    _depura ( "END pluginbc_cuenta::BlSubForm_editFinished", 0 );
    return 0;
}


int BlDbCompleterComboBox_textChanged (BlDbCompleterComboBox *bl)
{
  _depura("BlDbCompleterComboBox_textChanged", 0);

        if ( bl->m_entrada.size() >= 3 && bl->m_tabla == "cuenta") {
                QString cadwhere = "";
                /// Inicializamos los valores de vuelta a ""
                QMapIterator<QString, QString> i ( bl->m_valores );
                QString cador = "";
                while ( i.hasNext() ) {
                    i.next();
                    cadwhere = cadwhere + cador + " upper(" + i.key() + ")";
                    cador = " || ' ' ||";
                } // end while

                QString SQLQuery = "SELECT * FROM " + bl->m_tabla + " WHERE " + cadwhere + "LIKE  upper('%" + bl->m_entrada + "%')";
                bl->m_cursorcombo = bl->mainCompany() ->loadQuery ( SQLQuery );
                bl->clear();
                while ( !bl->m_cursorcombo->eof() ) {
                    QMapIterator<QString, QString> i ( bl->m_valores );
                    QString cad = "";
                    QString sep = "";
                    QString cad1 = "";
                    while ( i.hasNext() ) {
                        i.next();
                        cad = cad + sep + bl->m_cursorcombo->valor ( i.key() );
                        if ( sep == "" ) {
                            cad1 = i.key();
                            sep = " ";
                        } // end if
                    } // end while
                    bl->addItem ( cad , QVariant ( bl->m_cursorcombo->valor ( cad1 ) ) );
                    bl->m_cursorcombo->nextRecord();
                } // end while
                delete bl->m_cursorcombo;

  _depura("END BlDbCompleterComboBox_textChanged", 0);

     return 1;
        } // end if
  _depura("END BlDbCompleterComboBox_textChanged", 0);

    return 0;
}



int BcSubForm_pressedAsterisk ( BcSubForm *sub )
{
    _depura ( "BcSubForm_pressedAsterisk" );

    if ( sub->m_campoactual->nomcampo() != "codigo" ) {
        _depura ( "END BfSubForm::pressedAsterisk", 0 );
        return 0;
    } // end if

/*
    TutoresList *tutoreslist = new TutoresList ( ( BcCompany * ) sub->mainCompany(), NULL, 0, BL_SELECT_MODE );
    
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    sub->setEnabled ( false );
    tutoreslist->show();
    
    while ( !tutoreslist->isHidden() )
        g_theApp->processEvents();
        
    sub->setEnabled ( true );
    QString idCliente = tutoreslist->idcliente();
    
    delete tutoreslist;

    /// Si no tenemos un idtutor salimos ya que significa que no se ha seleccionado ninguno.
    if ( idCliente == "" ) {
        _depura ( "END BfSubForm::pressedAsterisk", 0 );
        return 0;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM cliente WHERE idcliente = " + idCliente );
    if ( !cur->eof() ) {
        sub->m_registrolinea->setDbValue ( "idcliente", idCliente );
        sub->m_registrolinea->setDbValue ( "nomcliente", cur->valor ( "nomcliente" ));
    } // end if
    
    delete cur;
*/    
    _depura ( "END BfSubForm_pressedAsterisk" );
    return 0;
}



