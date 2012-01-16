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

BcBulmaCont *g_pluginbc_cuenta = NULL;

///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_cuenta", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    g_pluginbc_cuenta = bcont;

    QMenu *pPluginMenu;
    /// Miramos si existe un menu Herramientas
    pPluginMenu = bcont->menuBar() ->findChild<QMenu *> ( "menuCuentas" );

    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
        pPluginMenu = new QMenu ( _ ( "&Cuentas" ), bcont->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuCuentas" ) );
    } // end if

    BlAction *accionA = new BlAction ( _ ( "&Plan contable" ), 0 );
    accionA->setStatusTip ( _ ( "Permite ver y modificar el plan contable" ) );
    accionA->setWhatsThis ( _ ( "Permite ver y modificar el plan contable" ) );
    accionA->setIcon(QIcon(QString::fromUtf8(":/Images/account_plan.png")));
    accionA->setObjectName("mui_actionPlanContable");
    pPluginMenu->addAction ( accionA );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bcont->menuBar() ->insertMenu ( bcont->menuMaestro->menuAction(), pPluginMenu );
    bcont->toolBar->addAction ( accionA );

    
    return 0;
}

int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG

    if (accion->objectName() == "mui_actionPlanContable") {
      
        BlDebug::blDebug ( Q_FUNC_INFO, 0, "mui_actionPlanContable" );

	if (!g_pluginbc_cuenta->company()->showWindow("BcPlanContableListView")) {
	    BcPlanContableListView *plan = new BcPlanContableListView( g_pluginbc_cuenta->company(), 0 );
	    plan->setObjectName("BcPlanContableListView");
	    plan->inicializa();
	    g_pluginbc_cuenta->company()->pWorkspace() ->addSubWindow ( plan );
	    plan->show();
	} // end if

    } // end if

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
Subform_Cuenta::Subform_Cuenta ( BlSubForm *parent ) : QObject ( parent )
{
    BL_FUNC_DEBUG
    
}

///
/**
**/
Subform_Cuenta::~Subform_Cuenta()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu
**/
void Subform_Cuenta::s_pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
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
    
}


///
/**
\param action
**/
void Subform_Cuenta::s_trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG
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

    
}


///
/**
**/
void Subform_Cuenta::editarCuenta ( QString idcuenta )
{
    BL_FUNC_DEBUG
        BcCuentaView * nuevae = new BcCuentaView ( ((BcBulmaCont *)g_main)->company(), 0 );
        nuevae->load ( idcuenta );
        ((BcBulmaCont *)g_main)->company() ->pWorkspace() ->addSubWindow ( nuevae );
        nuevae->show();
    
}


///
/**
**/
void Subform_Cuenta::nuevoCuenta( )
{
    BL_FUNC_DEBUG
        BcCuentaView * nuevae = new BcCuentaView ( ((BcBulmaCont *)g_main)->company(), 0 );
        ((BcBulmaCont *)g_main)->company() ->pWorkspace() ->addSubWindow ( nuevae );
        nuevae->mui_padre->setEnabled(TRUE);
        nuevae->show();
    
}


///
/**
**/
void Subform_Cuenta::seleccionarCuenta ( BlSubForm *sub )
{
    BL_FUNC_DEBUG
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
        
        return;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM articulo WHERE idcuenta = " + idArticle );
    if ( !cur->eof() ) {
        sub->lineaact()->setDbValue ( "idcuenta", idArticle );
        sub->lineaact()->setDbValue ( "codigocompletoarticulo", cur->value( "codigocompletoarticulo" ) );
        sub->lineaact()->setDbValue ( "nomarticulo", cur->value( "nomarticulo" ) );
    } // end if
    delete cur;
*/
    
}


///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    BL_FUNC_DEBUG
    Subform_Cuenta *subformods = new Subform_Cuenta ( sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    
    return 0;
}


int Busqueda_on_mui_buscar_clicked ( BlSearchWidget *busq )
{
    BL_FUNC_DEBUG
    int val = 0;
    if ( busq->tableName() == "cuenta" ) {

        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        blCenterOnScreen ( diag );

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
    return val;
}


/// ======================================================

int BlSubFormDelegate_createEditor ( BlSubFormDelegate *bl )
{
    BL_FUNC_DEBUG
    int ret = 0;
    if ( g_fieldName == "codigo" || g_fieldName == "codigo1" ) {
        BlDbCompleterComboBox * editor = new BlDbCompleterComboBox ( g_editor );
        editor->setObjectName ( "EditCodigoCuenta" );
        editor->setMainCompany ( ( BcCompany * ) bl->m_subform->mainCompany() );
        editor->m_valores["descripcion"] = "";
        editor->m_valores["codigo"] = "";
        editor->m_tabla = "cuenta";
        g_plugParams =  editor;
        ret = -1;
    } // end if

    

    return ret;
}



/// Hay cosas que deberian estar en el plugin de cuenta
int BlSubFormDelegate_setModelData ( BlSubFormDelegate *bl )
{
    BL_FUNC_DEBUG
    int ret = 0;
    if ( g_editor->objectName() == "EditCodigoCuenta" || g_editor->objectName() == "EditNombreCliente" ) {
        BlDbCompleterComboBox * comboBox = ( BlDbCompleterComboBox * ) g_editor;
        QString value = comboBox->currentText();
        value = value.left ( value.indexOf ( ".-" ) );
        g_model->setData ( g_index, value );
        ret = -1;
    } // end if
    
    return ret;
}


int BlSubFormDelegate_setEditorData ( BlSubFormDelegate *bl )
{
    BL_FUNC_DEBUG
    int ret = 0;
    if ( g_editor->objectName() == "EditCodigoCuenta" || g_editor->objectName() == "EditNombreCliente" ) {
        QString value = g_index.model() ->data ( g_index, Qt::DisplayRole ).toString();
        BlDbCompleterComboBox *comboBox = ( BlDbCompleterComboBox * ) g_editor ;
        comboBox->addItem ( value );
        ret = -1;
    } // end if
    
    return ret;
}

int BlSubForm_editFinished ( BlSubForm *sub )
{
    BL_FUNC_DEBUG
    if ( sub->m_campoactual->fieldName() == "codigo" ) {
   QString query = "SELECT idcuenta FROM cuenta WHERE upper (descripcion ||  ' ' || codigo) LIKE upper('" + sub->m_campoactual->text() + "%')";
// blMsgInfo(query);
        BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idcuenta", cur->value( "idcuenta" ) );
        } // end if
        delete cur;
    } // end if
    
    return 0;
}


int BlDbCompleterComboBox_textChanged (BlDbCompleterComboBox *bl)
{
  BL_FUNC_DEBUG

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
                QString cadwhere1 = " codigo = '" + blExtendStringWithZeros(bl->m_entrada, ((BcCompany *)bl->mainCompany())->numDigitosEmpresa()).replace("'","''") + "'";
                QString SQLQuery = "SELECT * FROM " + bl->m_tabla + " WHERE " +cadwhere1+ " OR " + cadwhere + "LIKE  upper('%" + bl->m_entrada.replace("'","''") + "%')";
                bl->m_cursorcombo = bl->mainCompany() ->loadQuery ( SQLQuery );
                bl->clear();
                while ( !bl->m_cursorcombo->eof() ) {
                    QMapIterator<QString, QString> i ( bl->m_valores );
                    QString cad = "";
                    QString sep = "";
                    QString cad1 = "";
                    while ( i.hasNext() ) {
                        i.next();
                        cad = cad + sep + bl->m_cursorcombo->value( i.key() );
                        if ( sep == "" ) {
                            cad1 = i.key();
                            sep = ".-";
                        } // end if
                    } // end while
                    bl->addItem ( cad , QVariant ( bl->m_cursorcombo->value( cad1 ) ) );
                    bl->m_cursorcombo->nextRecord();
                } // end while
                delete bl->m_cursorcombo;
     return 1;
        } // end if

    return 0;
}


int BcSubForm_pressedAsterisk ( BcSubForm *sub, void **ppCodigo )
{
    BL_FUNC_DEBUG

    QDialog *diag = new QDialog ( 0 );
    diag->setModal ( true );

    ///TODO: De esta manera se recarga de la base de datos toda la info de las cuentas cada
    /// vez que se necesita la lista de cuentas. Hay que buscar la manera de que este siempre
    /// disponible para no cargar el trabajo a la red ni al gestor de base de datos.
    BcCuentaListView *listcuentas = new BcCuentaListView ( ( BcCompany * ) sub->mainCompany(), diag, 0, BL_SELECT_MODE );
    QObject::connect ( listcuentas, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

    diag->exec();
        
    QString** cadena;
    cadena = (QString**) ppCodigo;
    (**cadena) = listcuentas->codigocuenta();

    delete diag;

    
    return 0;
}


int Busqueda_on_m_inputBusqueda_textChanged (BlSearchWidget *wid) {
    if (wid->tableName() != "cuenta") return 0;
    QString texto = wid->text();
    QString codigo = blExtendStringWithZeros(wid->text(), ((BcCompany *)wid->mainCompany())->numDigitosEmpresa() );
    QString SQLQuery = "SELECT * FROM cuenta WHERE codigo = '" + codigo + "'";
    BlDbRecordSet *cur = wid->mainCompany() ->loadQuery ( SQLQuery );
    if ( cur->numregistros() == 1 ) {
        /// Ponemos el id sin hacer una carga adicional
        wid->setId( cur->value( "idcuenta" ), FALSE);
        /// Inicializamos los valores de vuelta a ""
        QMapIterator<QString, QString> i ( wid->m_valores );
        while ( i.hasNext() ) {
            i.next();
            wid->m_valores.insert ( i.key(), cur->value( i.key() ) );
        } // end while
        wid->m_textBusqueda->setText(cur->value("codigo") + " " + cur->value("descripcion"));
        delete cur;
        return 1;
    } // end if
    delete cur;
    return 0;
}


