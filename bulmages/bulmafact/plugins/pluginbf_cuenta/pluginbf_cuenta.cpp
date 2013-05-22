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

#include "pluginbf_cuenta.h"
#include "bccuentalistview.h"
#include "bcplancontablelistview.h"
#include "bccuentaview.h"
#include "bfsubform.h"


BfBulmaFact * g_pluginbf_cuenta=NULL;


///
/**
\param bcont
**/
int entryPoint ( BlMainWindow *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_cuenta", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    g_pluginbf_cuenta = (BfBulmaFact *) bcont;

    QMenu *pPluginMenu;
    /// Miramos si existe un menu Herramientas
    pPluginMenu = g_pluginbf_cuenta->menuBar() ->findChild<QMenu *> ( "menuCuentas" );

    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
        pPluginMenu = new QMenu ( _ ( "&Cuentas" ), g_pluginbf_cuenta->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuCuentas" ) );
    } // end if

    BlAction *accionA = new BlAction ( _ ( "&Plan contable" ), 0 );
    accionA->setStatusTip ( _ ( "Permite ver y modificar el plan contable" ) );
    accionA->setWhatsThis ( _ ( "Permite ver y modificar el plan contable" ) );
    accionA->setIcon(QIcon(QString::fromUtf8(":/Images/account_plan.png")));
    accionA->setObjectName("mui_actionPlanContable");
    pPluginMenu->addAction ( accionA );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    g_pluginbf_cuenta->menuBar() ->insertMenu ( g_pluginbf_cuenta->menuMaestro->menuAction(), pPluginMenu );
    g_pluginbf_cuenta->Listados->addAction ( accionA );

    
    return 0;
}

int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG

    if (accion->objectName() == "mui_actionPlanContable") {
      
        BlDebug::blDebug ( Q_FUNC_INFO, 0, "mui_actionPlanContable" );

	if (!g_pluginbf_cuenta->company()->showWindow("BcPlanContableListView")) {
	    BcPlanContableListView *plan = new BcPlanContableListView( g_pluginbf_cuenta->company(), 0 );
	    plan->setObjectName("BcPlanContableListView");
	    plan->inicializa();
	    g_pluginbf_cuenta->company()->pWorkspace() ->addSubWindow ( plan );
	    plan->show();
	} // end if

    } // end if

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

        /// Creamos una instancia del selector de cuentas.
        BcCuentaListView *listcuentas = new BcCuentaListView ( ( BfCompany * ) busq->mainCompany(), diag, 0, BL_SELECT_MODE );


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
        editor->setMainCompany ( ( BfCompany * ) bl->m_subform->mainCompany() );
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
                QString cadwhere1 = " codigo = '" + blExtendStringWithZeros(bl->m_entrada, (g_confpr->value(CONF_CONT_NUMDIGITOSEMPRESA).toInt())).replace("'","''") + "'";
		
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


int BfSubForm_pressedAsterisk ( BfSubForm *sub )
{
    BL_FUNC_DEBUG

    QDialog *diag = new QDialog ( 0 );
    diag->setModal ( true );

    BlDbSubFormRecord *rec = sub->m_registrolinea;
    BlDbSubFormField *camp = sub->m_campoactual;
    
    if (camp->fieldName() != "codigo") return 0;
    
    ///TODO: De esta manera se recarga de la base de datos toda la info de las cuentas cada
    /// vez que se necesita la lista de cuentas. Hay que buscar la manera de que este siempre
    /// disponible para no cargar el trabajo a la red ni al gestor de base de datos.
    BcCuentaListView *listcuentas = new BcCuentaListView ( ( BfCompany * ) sub->mainCompany(), diag, 0, BL_SELECT_MODE );
    QObject::connect ( listcuentas, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

    diag->exec();
        
    QString codigo= listcuentas->codigocuenta();

    delete diag;

    
    
    if ( codigo != "" ) {
        QString query = "SELECT * FROM cuenta WHERE codigo = '" + codigo + "'";
        BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            if ( camp->fieldName() == "codigo" ) {
                rec->setDbValue ( "idcuenta", cur->value( "idcuenta" ) );
                rec->setDbValue ( "codigo", cur->value( "codigo" ) );
                rec->setDbValue ( "tipocuenta", cur->value( "tipocuenta" ) );
                rec->setDbValue ( "descripcion", cur->value( "descripcion" ) );
                if ( rec->exists ( "idc_coste" ) && cur->value( "idc_coste" ) != "" ) {
                    rec->setDbValue ( "idc_coste", cur->value( "idc_coste" ) );
                    QString query1 = "SELECT * FROM c_coste WHERE idc_coste = " + cur->value( "idc_coste" );
                    BlDbRecordSet *curss = sub->mainCompany() ->loadQuery ( query1 );
                    rec->setDbValue ( "nomc_coste", curss->value( "nombre" ) );
                    delete curss;
                } // end if

            } // end if
            if ( camp->fieldName() == "codigoctacliente" ) {
                rec->setDbValue ( "idctacliente", cur->value( "idcuenta" ) );
                rec->setDbValue ( "codigoctacliente", cur->value( "codigo" ) );
                rec->setDbValue ( "tipoctacliente", cur->value( "tipocuenta" ) );
                rec->setDbValue ( "nomctacliente", cur->value( "descripcion" ) );

            } // end if
        } // end if
        delete cur;
    } // end if
    
    
    return 0;
}


int Busqueda_on_m_inputBusqueda_textChanged (BlSearchWidget *wid) {
    if (wid->tableName() != "cuenta") return 0;
    QString texto = wid->text();
    QString codigo = blExtendStringWithZeros(wid->text(), (g_confpr->value(CONF_CONT_NUMDIGITOSEMPRESA).toInt() ));
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





/// --------------------------------------------------------------
/// --------- Implemento la edicion de cuentas -----------------
/// Con esta funcionalidad creamos menus contextuales en todos los subformularios donde
/// Aparezca el identificador de cuenta como elemento y permite de forma sencilla
/// La creacion, la edicion, y la seleccion.
///


/**
\param parent
**/
SubForm_Cuenta::SubForm_Cuenta ( BlSubForm *parent ) : QObject ( parent )
{
    BL_FUNC_DEBUG
    
}

///
/**
**/
SubForm_Cuenta::~SubForm_Cuenta()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu
**/
void SubForm_Cuenta::s_pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
    BfSubForm *sub = ( BfSubForm * ) parent();
    BlSubFormHeader *header = sub->header ( "codigocuenta" );
    if ( header ) {
        menu->addSeparator();
        menu->addAction ( QIcon ( ":/ImgGestionAula/icons/cuenta.png" ), _ ( "Nuevo cuenta" ) );
        QString idcuenta = sub->dbValue ( "idcuenta" );
        if ( idcuenta != "" ) menu->addAction ( QIcon ( ":/ImgGestionAula/icons/cuenta.png" ), _ ( "Editar cuenta" ) );
        if ( ! ( header->options() & BlSubFormHeader::DbNoWrite ) )  {
            menu->addAction ( QIcon ( ":/ImgGestionAula/icons/cuenta-list.png" ), _ ( "Seleccionar cuenta" ) );
        } // end if
    } // end if
    
}


///
/**
\param action
**/
void SubForm_Cuenta::s_trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG
    BfSubForm *sub = ( BfSubForm * ) parent();
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
void SubForm_Cuenta::editarCuenta ( QString idcuenta )
{
    BL_FUNC_DEBUG
    BlSubForm * subf = ( BlSubForm * ) parent();
    BcCuentaView * art = new BcCuentaView ( ( BfCompany * ) subf->mainCompany(), 0 );
    subf->mainCompany() ->m_pWorkspace->addSubWindow ( art );
    /// Si la carga no va bien entonces terminamos.
    if ( art->load ( idcuenta ) ) {
        delete art;
        return;
    } // end if
    art->hide();
    art->show();
    
}



///
/**
**/
void SubForm_Cuenta::nuevoCuenta( )
{
    BL_FUNC_DEBUG
    BlSubForm * subf = ( BlSubForm * ) parent();
    BcCuentaView * art = new BcCuentaView ( ( BfCompany * ) subf->mainCompany(), 0 );
    art->setAttribute ( Qt::WA_DeleteOnClose, FALSE );
    
    subf->mainCompany() ->m_pWorkspace->addSubWindow ( art );
    subf->setEnabled(false);
    art->hide();
    art->show();
    
    while ( !art->isHidden() )
        g_theApp->processEvents();
    subf->setEnabled ( true );
    QString idCuenta = art->dbValue("idcuenta");
    if (idCuenta != "") {
        subf->lineaact()->setDbValue ( "idcuenta", idCuenta );
        subf->lineaact()->setDbValue ( "descripcion", art->dbValue ( "descripcion" ) );      
        subf->lineaact()->setDbValue ( "codigo", art->dbValue ( "codigo" ) );      
    } // end if
    delete art;    
    
    
}


///
/**
**/
void SubForm_Cuenta::seleccionarCuenta ( BlSubForm *sub )
{
    BL_FUNC_DEBUG
    
    if (!sub) sub= (BfSubForm *) parent();
    
    BcPlanContableListView *artlist = new BcPlanContableListView ( ( BfCompany * ) sub->mainCompany(), NULL, 0, BL_SELECT_MODE );
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    sub->setEnabled ( false );
    artlist->show();
    while ( !artlist->isHidden() )
        g_theApp->processEvents();
    sub->setEnabled ( true );
    QString idCuenta = artlist->idcuenta();
    delete artlist;

    /// Si no tenemos un idcuenta salimos ya que significa que no se ha seleccionado ninguno.
    if ( idCuenta == "" ) {
        
        return;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM cuenta WHERE idcuenta = " + idCuenta );
    if ( !cur->eof() ) {
        sub->lineaact()->setDbValue ( "idcuenta", idCuenta );
        sub->lineaact()->setDbValue ( "codigo", cur->value( "codigo" ) );
        sub->lineaact()->setDbValue ( "descripcion", cur->value( "descripcion" ) );
    } // end if
    delete cur;
}


///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    BL_FUNC_DEBUG
    SubForm_Cuenta *subformods = new SubForm_Cuenta ( sub );
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
    BlSubFormHeader *header = sub->header ( "codigo1" );
    if (!header) 
        header = sub->header ( "codigo" );
    if ( header ) {
        SubForm_Cuenta *subformods = new SubForm_Cuenta ( sub );
        
        QHBoxLayout *m_hboxLayout1 = sub->mui_menusubform->findChild<QHBoxLayout *> ( "hboxLayout1" );
        if ( !m_hboxLayout1 ) {
            m_hboxLayout1 = new QHBoxLayout ( sub->mui_menusubform );
            m_hboxLayout1->setSpacing (0 );
            m_hboxLayout1->setMargin ( 0 );
            m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
        } // end if
        
        if ( ! ( header->options() & BlSubFormHeader::DbNoWrite ) )  {
          QToolButton *sel = new QToolButton ( sub->mui_menusubform );
          sel->setStatusTip ( _("Nueva cuenta") );
          sel->setToolTip ( _("Nueva cuenta") );
          sel->setMinimumSize ( QSize ( 18, 18 ) );
          sel->setIcon ( QIcon ( ":/ImgGestionAula/icons/cuenta.png" ) );
          sel->setIconSize ( QSize ( 18, 18 ) );    
          m_hboxLayout1->addWidget ( sel );
          sel->connect (sel, SIGNAL(released()), subformods, SLOT(nuevoCuenta()));
        
          QToolButton *sel1 = new QToolButton ( sub->mui_menusubform );
          sel1->setStatusTip ( _("Seleccionar cuenta") );
          sel1->setToolTip ( _("Seleccionar cuenta") );
          sel1->setMinimumSize ( QSize ( 18, 18 ) );
          sel1->setIcon ( QIcon ( ":/ImgGestionAula/icons/cuenta-list.png" ) );
          sel1->setIconSize ( QSize ( 18, 18 ) );
          m_hboxLayout1->addWidget ( sel1 );
          sel1->connect (sel1, SIGNAL(released()), subformods, SLOT(seleccionarCuenta()));
        } // end if
    } // end if

    return 0;
}







/// Agregamos la cuenta al cliente
/// ==============================
/* CLIENTE */
/********************************************/



int ClienteView_ClienteView ( ClienteView *cliente )
{
    BL_FUNC_DEBUG
  
    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing ( 2 );
    hboxLayout160->setMargin ( 0 );
    hboxLayout160->setObjectName ( QString::fromUtf8 ( "hboxLayout161" ) );
//    ClienteCuenta *clientecuenta = new ClienteCuenta(cliente);

    cliente->addDbField ( "idcuenta", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Id cuenta cliente" ) );
    
    BlSearchWidget *cta = new BlSearchWidget(cliente->m_frameplugin);
    /// Iniciamos los componentes
    cta->setMainCompany(cliente->mainCompany());
    cta->setLabel ( _ ( "Cuenta contable:" ) );
    cta->setTableName ( "cuenta" );
    cta->setFieldId("idcuenta");
    cta->m_valores["descripcion"] = "";
    cta->m_valores["codigo"] = "";
    cta->setObjectName("mui_idcuenta");
    
    hboxLayout160->addWidget(cta);
    
    /// Comprobamos que exista el layout.
    QVBoxLayout *m_hboxLayout1 = cliente->m_frameplugin->findChild<QVBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QVBoxLayout ( cliente->m_frameplugin );
        m_hboxLayout1->setSpacing ( 0 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if

    m_hboxLayout1->addLayout ( hboxLayout160 );
    
    return 0;
}

int ClienteView_Guardar_Pre(ClienteView *cliente) {
    BL_FUNC_DEBUG
    if (cliente->dbValue("idcuenta") == "") {
	
    /// Suponiendo que las cuentas son num&eacute;ricas, al crear una nueva cuenta
    /// buscamos entre las que ser&aacute;n sus hermanas y le asignamos el n&uacute;mero
    /// siguiente que le corresponda.
    QString query;
    QString codpadre = "4300";
    QString codint = codpadre;
    int tipocuenta = 0;
    
    query = "SELECT * FROM cuenta WHERE idcuenta = id_cuenta('" + codpadre + "')";
    BlDbRecordSet *cur2 = cliente->mainCompany()->loadQuery( query );
    if (!cur2->eof()) {
        tipocuenta = cur2->value( "tipocuenta" ).toInt();
    } // end if
    delete cur2;
    
    query = "SELECT * FROM cuenta WHERE padre = id_cuenta('" + codpadre + "') ORDER BY codigo DESC LIMIT 1";
    BlDbRecordSet *cur = cliente->mainCompany() ->loadQuery ( query );
    if ( !cur->eof() ) {
        long int valor = cur->value( "codigo" ).toLong();
        valor ++;
        codint.setNum ( valor );

    } else {

	// PONER UN PARAMETRO APROPIADO
        while ( codint.length() <  g_confpr->value(CONF_CONT_NUMDIGITOSEMPRESA).toInt() - 1 ) {
            codint = codint + "0";
        } // end while
        codint = codint + "1";
    }
    delete cur;
    query = "INSERT INTO cuenta (codigo,padre, tipocuenta, descripcion) VALUES ('"+codint+"',id_cuenta('"+codpadre+"'),"+QString::number(tipocuenta)+",'"+cliente->mui_nomcliente->text()+"')";
    cliente->mainCompany()->runQuery(query);
    BlDbRecordSet *cur1 = cliente->mainCompany()->loadQuery("SELECT id_cuenta('"+codint+"') AS id");
    
    cliente->setDbValue("idcuenta",cur1->value("id"));
    delete cur1;
    
    blMsgError("Se ha creado la cuenta " + codint);
	
    } // end if
    return 0;
}



int ProveedorView_ProveedorView ( ProveedorView *prov )
{
    BL_FUNC_DEBUG
  
    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing ( 2 );
    hboxLayout160->setMargin ( 0 );
    hboxLayout160->setObjectName ( QString::fromUtf8 ( "hboxLayout161" ) );

    prov->addDbField ( "idcuenta", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Id cuenta prov" ) );
    
    BlSearchWidget *cta = new BlSearchWidget(prov->m_frameplugin);
    /// Iniciamos los componentes
    cta->setMainCompany(prov->mainCompany());
    cta->setLabel ( _ ( "Cuenta contable:" ) );
    cta->setTableName ( "cuenta" );
    cta->setFieldId("idcuenta");
    cta->m_valores["descripcion"] = "";
    cta->m_valores["codigo"] = "";
    cta->setObjectName("mui_idcuenta");
    
    hboxLayout160->addWidget(cta);
    
    /// Comprobamos que exista el layout.
    QVBoxLayout *m_hboxLayout1 = prov->m_frameplugin->findChild<QVBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QVBoxLayout ( prov->m_frameplugin );
        m_hboxLayout1->setSpacing ( 0 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if

    m_hboxLayout1->addLayout ( hboxLayout160 );
    
    return 0;
}



int ProveedorView_Guardar_Pre(ProveedorView *proveedor) {
    BL_FUNC_DEBUG
    if (proveedor->dbValue("idcuenta") == "") {
	
    /// Suponiendo que las cuentas son num&eacute;ricas, al crear una nueva cuenta
    /// buscamos entre las que ser&aacute;n sus hermanas y le asignamos el n&uacute;mero
    /// siguiente que le corresponda.
    QString query;
    QString codpadre = "4000";
    QString codint = codpadre;
    int tipocuenta = 0;
    
    query = "SELECT * FROM cuenta WHERE idcuenta = id_cuenta('" + codpadre + "')";
    BlDbRecordSet *cur2 = proveedor->mainCompany()->loadQuery( query );
    if (!cur2->eof()) {
        tipocuenta = cur2->value( "tipocuenta" ).toInt();
    } // end if
    delete cur2;
    
    query = "SELECT * FROM cuenta WHERE padre = id_cuenta('" + codpadre + "') ORDER BY codigo DESC LIMIT 1";
    BlDbRecordSet *cur = proveedor->mainCompany() ->loadQuery ( query );
    if ( !cur->eof() ) {
        long int valor = cur->value( "codigo" ).toLong();
        valor ++;
        codint.setNum ( valor );

    } else {

	// PONER UN PARAMETRO APROPIADO
        while ( codint.length() <  g_confpr->value(CONF_CONT_NUMDIGITOSEMPRESA).toInt() - 1 ) {
            codint = codint + "0";
        } // end while
        codint = codint + "1";
    }
    delete cur;
    query = "INSERT INTO cuenta (codigo,padre, tipocuenta, descripcion) VALUES ('"+codint+"',id_cuenta('"+codpadre+"'),"+QString::number(tipocuenta)+",'"+proveedor->mui_nomproveedor->text()+"')";
    proveedor->mainCompany()->runQuery(query);
    BlDbRecordSet *cur1 = proveedor->mainCompany()->loadQuery("SELECT id_cuenta('"+codint+"') AS id");
    
    proveedor->setDbValue("idcuenta",cur1->value("id"));
    delete cur1;
    
    blMsgError("Se ha creado la cuenta " + codint);
	
    } // end if
    return 0;
}




int FamiliasView_FamiliasView ( FamiliasView *fam )
{
    BL_FUNC_DEBUG
  
    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing ( 2 );
    hboxLayout160->setMargin ( 0 );
    hboxLayout160->setObjectName ( QString::fromUtf8 ( "hboxLayout161" ) );

//    fam->addDbField ( "idcuenta", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Id cuenta fam" ) );
    
    BlSearchWidget *cta = new BlSearchWidget(fam->m_frameplugin);
    /// Iniciamos los componentes
    cta->setMainCompany(fam->mainCompany());
    cta->setLabel ( _ ( "Cuenta contable:" ) );
    cta->setTableName ( "cuenta" );
    cta->setFieldId("idcuenta");
    cta->m_valores["descripcion"] = "";
    cta->m_valores["codigo"] = "";
    cta->setObjectName("mui_idcuenta");
    
    hboxLayout160->addWidget(cta);
    
    
    // =======
    QHBoxLayout *hboxLayout163 = new QHBoxLayout();
    hboxLayout163->setSpacing ( 2 );
    hboxLayout163->setMargin ( 0 );
    hboxLayout163->setObjectName ( QString::fromUtf8 ( "hboxLayout163" ) );

//    fam->addDbField ( "idcuenta", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Id cuenta fam" ) );
    
    BlSearchWidget *cta1 = new BlSearchWidget(fam->m_frameplugin);
    /// Iniciamos los componentes
    cta1->setMainCompany(fam->mainCompany());
    cta1->setLabel ( _ ( "Cuenta contable compras:" ) );
    cta1->setTableName ( "cuenta" );
    cta1->setFieldId("idcuentav");
    cta1->m_valores["descripcion"] = "";
    cta1->m_valores["codigo"] = "";
    cta1->setObjectName("mui_idcuentav");
    
    hboxLayout163->addWidget(cta1);
    
    
    
    /// Comprobamos que exista el layout.
    QVBoxLayout *m_hboxLayout1 = fam->m_frameplugin->findChild<QVBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QVBoxLayout ( fam->m_frameplugin );
        m_hboxLayout1->setSpacing ( 0 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if

    m_hboxLayout1->addLayout ( hboxLayout160 );
    m_hboxLayout1->addLayout ( hboxLayout163 );
    
    return 0;
}

int FamiliasView_Guardar_Pre ( FamiliasView *fam )
{
    QString query;
  
    try {

	QString estado;
	QString idcuenta = fam->findChild<BlSearchWidget *>("mui_idcuenta")->id();
	QString idcuentav = fam->findChild<BlSearchWidget *>("mui_idcuentav")->id();
	
	if (idcuenta == "") {

	      /// Suponiendo que las cuentas son num&eacute;ricas, al crear una nueva cuenta
	      /// buscamos entre las que ser&aacute;n sus hermanas y le asignamos el n&uacute;mero
	      /// siguiente que le corresponda.
	      QString query;
	      QString codpadre = "700";
	      QString codint = codpadre;
	      int tipocuenta = 0;
	      
	      query = "SELECT * FROM cuenta WHERE idcuenta = id_cuenta('" + codpadre + "')";
	      BlDbRecordSet *cur2 = fam->mainCompany()->loadQuery( query );
	      if (!cur2->eof()) {
		  tipocuenta = cur2->value( "tipocuenta" ).toInt();
	      } // end if
	      delete cur2;
	      
	      query = "SELECT * FROM cuenta WHERE padre = id_cuenta('" + codpadre + "') ORDER BY codigo DESC LIMIT 1";
	      BlDbRecordSet *cur = fam->mainCompany() ->loadQuery ( query );
	      if ( !cur->eof() ) {
		  long int valor = cur->value( "codigo" ).toLong();
		  valor ++;
		  codint.setNum ( valor );

	      } else {

		  // PONER UN PARAMETRO APROPIADO
		  while ( codint.length() <  g_confpr->value(CONF_CONT_NUMDIGITOSEMPRESA).toInt() - 1 ) {
		      codint = codint + "0";
		  } // end while
		  codint = codint + "1";
	      }
	      delete cur;
	      query = "INSERT INTO cuenta (codigo,padre, tipocuenta, descripcion) VALUES ('"+codint+"',id_cuenta('"+codpadre+"'),"+QString::number(tipocuenta)+",'"+fam->mui_nomFamilia->text()+"')";
	      fam->mainCompany()->runQuery(query);
	      
	      query = "UPDATE familia SET idcuenta = id_cuenta('"+codint+"') WHERE idfamilia = " + fam->idFamiliaModified();
	      fam->mainCompany()->runQuery(query);
	
	      fam->findChild<BlSearchWidget *>("mui_idcuenta")->setText(codint);
	      
	      blMsgError("Se ha creado la cuenta " + codint);
	} else {
	      query = "UPDATE familia SET idcuenta = "+idcuenta+" WHERE idfamilia = " + fam->idFamiliaModified();
	      fam->mainCompany()->runQuery(query);
	      fam->findChild<BlSearchWidget *>("mui_idcuenta")->setId(idcuenta);

	      
	} // end if
	
	/// LA cuenta para compras
	if (idcuentav == "") {

	      /// Suponiendo que las cuentas son num&eacute;ricas, al crear una nueva cuenta
	      /// buscamos entre las que ser&aacute;n sus hermanas y le asignamos el n&uacute;mero
	      /// siguiente que le corresponda.
	      QString query;
	      QString codpadre = "600";
	      QString codint = codpadre;
	      int tipocuenta = 0;
	      
	      query = "SELECT * FROM cuenta WHERE idcuenta = id_cuenta('" + codpadre + "')";
	      BlDbRecordSet *cur2 = fam->mainCompany()->loadQuery( query );
	      if (!cur2->eof()) {
		  tipocuenta = cur2->value( "tipocuenta" ).toInt();
	      } // end if
	      delete cur2;
	      
	      query = "SELECT * FROM cuenta WHERE padre = id_cuenta('" + codpadre + "') ORDER BY codigo DESC LIMIT 1";
	      BlDbRecordSet *cur = fam->mainCompany() ->loadQuery ( query );
	      if ( !cur->eof() ) {
		  long int valor = cur->value( "codigo" ).toLong();
		  valor ++;
		  codint.setNum ( valor );

	      } else {

		  // PONER UN PARAMETRO APROPIADO
		  while ( codint.length() <  g_confpr->value(CONF_CONT_NUMDIGITOSEMPRESA).toInt() - 1 ) {
		      codint = codint + "0";
		  } // end while
		  codint = codint + "1";
	      }
	      delete cur;
	      query = "INSERT INTO cuenta (codigo,padre, tipocuenta, descripcion) VALUES ('"+codint+"',id_cuenta('"+codpadre+"'),"+QString::number(tipocuenta)+",'"+fam->mui_nomFamilia->text()+"')";
	      fam->mainCompany()->runQuery(query);
	      
	      query = "UPDATE familia SET idcuentav = id_cuenta('"+codint+"') WHERE idfamilia = " + fam->idFamiliaModified();
	      fam->mainCompany()->runQuery(query);
	
	      fam->findChild<BlSearchWidget *>("mui_idcuentav")->setText(codint);
	      
	      blMsgError("Se ha creado la cuenta " + codint);
	} else {
	      query = "UPDATE familia SET idcuentav = "+idcuentav+" WHERE idfamilia = " + fam->idFamiliaModified();
	      fam->mainCompany()->runQuery(query);
	      fam->findChild<BlSearchWidget *>("mui_idcuentav")->setId(idcuenta);

	      
	} // end if
	
	
    } catch (int e) {
	throw -1;
    } // end try

  
    return 0;
}


int FamiliasView_currentItemChanged_Post ( FamiliasView *fam )
{
    BL_FUNC_DEBUG

    BlDbRecordSet *rec;
    QString query;
     
    if ( fam->idFamilia() == "" ) {
      fam->findChild<BlSearchWidget *>("mui_idcuenta")->setId("");
      fam->findChild<BlSearchWidget *>("mui_idcuentav")->setId("");
    } else {
      query = "SELECT idcuenta, idcuentav FROM familia WHERE idfamilia = '" + fam->idFamilia() + "' LIMIT 1";
      rec = fam->mainCompany()->loadQuery(query);
      // Si no hay datos no se hace nada.
      if ( rec != NULL ) {
	    fam->findChild<BlSearchWidget *>("mui_idcuenta")->setId(rec->value( "idcuenta" ));      
	    fam->findChild<BlSearchWidget *>("mui_idcuentav")->setId(rec->value( "idcuentav" ));      
      } // end if
      delete rec;
    } // end if
    return 0;
}





// ==================================

int FPagoView_FPagoView ( FPagoView *fam )
{
    BL_FUNC_DEBUG
  
    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing ( 2 );
    hboxLayout160->setMargin ( 0 );
    hboxLayout160->setObjectName ( QString::fromUtf8 ( "hboxLayout161" ) );

//    fam->addDbField ( "idcuenta", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Id cuenta fam" ) );
    
    BlSearchWidget *cta = new BlSearchWidget(fam->m_frameplugin);
    /// Iniciamos los componentes
    cta->setMainCompany(fam->mainCompany());
    cta->setLabel ( _ ( "Cuenta contable:" ) );
    cta->setTableName ( "cuenta" );
    cta->setFieldId("idcuenta");
    cta->m_valores["descripcion"] = "";
    cta->m_valores["codigo"] = "";
    cta->setObjectName("mui_idcuenta");
    
    hboxLayout160->addWidget(cta);
    
    /// Comprobamos que exista el layout.
    QVBoxLayout *m_hboxLayout1 = fam->m_frameplugin->findChild<QVBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QVBoxLayout ( fam->m_frameplugin );
        m_hboxLayout1->setSpacing ( 0 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if

    m_hboxLayout1->addLayout ( hboxLayout160 );
    
    return 0;
}

int FPagoView_Guardar_Pre ( FPagoView *fam )
{
    QString query;
  
    try {

	QString estado;
	QString idcuenta = fam->findChild<BlSearchWidget *>("mui_idcuenta")->id();
	
	if (idcuenta == "") {

	      /// Suponiendo que las cuentas son num&eacute;ricas, al crear una nueva cuenta
	      /// buscamos entre las que ser&aacute;n sus hermanas y le asignamos el n&uacute;mero
	      /// siguiente que le corresponda.
	      QString query;
	      QString codpadre = "570";
	      QString codint = codpadre;
	      int tipocuenta = 0;
	      
	      query = "SELECT * FROM cuenta WHERE idcuenta = id_cuenta('" + codpadre + "')";
	      BlDbRecordSet *cur2 = fam->mainCompany()->loadQuery( query );
	      if (!cur2->eof()) {
		  tipocuenta = cur2->value( "tipocuenta" ).toInt();
	      } // end if
	      delete cur2;
	      
	      query = "SELECT * FROM cuenta WHERE padre = id_cuenta('" + codpadre + "') ORDER BY codigo DESC LIMIT 1";
	      BlDbRecordSet *cur = fam->mainCompany() ->loadQuery ( query );
	      if ( !cur->eof() ) {
		  long int valor = cur->value( "codigo" ).toLong();
		  valor ++;
		  codint.setNum ( valor );

	      } else {

		  // PONER UN PARAMETRO APROPIADO
		  while ( codint.length() <  g_confpr->value(CONF_CONT_NUMDIGITOSEMPRESA).toInt() - 1 ) {
		      codint = codint + "0";
		  } // end while
		  codint = codint + "1";
	      }
	      delete cur;
	      query = "INSERT INTO cuenta (codigo,padre, tipocuenta, descripcion) VALUES ('"+codint+"',id_cuenta('"+codpadre+"'),"+QString::number(tipocuenta)+",'"+fam->mui_descforma_pago->text()+"')";
	      fam->mainCompany()->runQuery(query);
	      
	      query = "UPDATE forma_pago SET idcuenta = id_cuenta('"+codint+"') WHERE idforma_pago = " + fam->idFormaPago();
	      fam->mainCompany()->runQuery(query);
	
	      fam->findChild<BlSearchWidget *>("mui_idcuenta")->setText(codint);
	      
	      blMsgError("Se ha creado la cuenta " + codint);
	} else {
	      query = "UPDATE forma_pago SET idcuenta = "+idcuenta+" WHERE idforma_pago = " + fam->idFormaPago();
	      fam->mainCompany()->runQuery(query);
	      fam->findChild<BlSearchWidget *>("mui_idcuenta")->setId(idcuenta);

	      
	} // end if
    } catch (int e) {
	throw -1;
    } // end try

  
    return 0;
}


int FPagoView_currentItemChanged_Post ( FPagoView *fam )
{
    BL_FUNC_DEBUG

    BlDbRecordSet *rec;
    QString query;
     
    if ( fam->idFormaPago() == "" ) {
      fam->findChild<BlSearchWidget *>("mui_idcuenta")->setId("");
    } else {
      query = "SELECT idcuenta FROM forma_pago WHERE idforma_pago = '" + fam->idFormaPago() + "' LIMIT 1";
      rec = fam->mainCompany()->loadQuery(query);
      // Si no hay datos no se hace nada.
      if ( rec != NULL ) {
	    fam->findChild<BlSearchWidget *>("mui_idcuenta")->setId(rec->value( "idcuenta" ));      
      } // end if
      delete rec;
    } // end if
    return 0;
}




/*

int ClienteView_cargarPost_Post ( ClienteView *cliente )
{
    BlDbRecordSet *rec;

    cliente->mainCompany()->begin();
  
    // 1) coge idcuentabanco de banco.
    // 2) se conecta a contabilidad.
    // 3) busca el codigo cuenta usando el idcuenta de banco.
    // 4) rellena qlineedit con codigo cuenta.
    
    QString query = "SELECT idcuentacliente FROM cliente WHERE idcliente = '" + cliente->dbValue(cliente->fieldId()) + "' LIMIT 1";
    
    
    rec = cliente->mainCompany()->loadQuery(query);
    
    // Si no hay datos en idcuenta no se hace nada.
    if ( rec != NULL ) {

	cliente->mainCompany()->run("SELECT conectabulmacont()");
	
	QString query = "SELECT codigo FROM bc_cuenta WHERE idcuenta = '" + rec->value("idcuentacliente") + "' LIMIT 1";
	rec = cliente->mainCompany()->loadQuery(query);
	
	cliente->findChild<QLineEdit *>("mui_cuenta_cliente")->setText( rec->value("codigo") );
	
    } // end if

  
    cliente->mainCompany()->commit();

    return 0;
}


int ClienteView_Guardar_Pre ( ClienteView *cliente )
{
    BlDbRecordSet *rec;

    /// En la creacion del cliente no existe ID.
    if ( !cliente->dbValue ( cliente->fieldId() ).isEmpty() ) {
    
      QString query = "SELECT idcuentacliente FROM cliente WHERE idcliente = '" + cliente->dbValue(cliente->fieldId()) + "' LIMIT 1";

      rec = cliente->mainCompany()->loadQuery(query);

      if ( rec != NULL ) {
	  cliente->setDbValue("idcuentacliente", cliente->dbValue("idcuentacliente"));
      } // end if
      
      delete rec;

    } else {
      cliente->setDbValue("idcuentacliente", "0");
    } // end if

    return 0;
}


int ClienteView_Guardar_Post ( ClienteView *cliente )
{
    BlDbRecordSet *rec_cliente;
    BlDbRecordSet *tmp_cliente;
    BlDbRecordSet *rsa;
    QString query;

    try {

      cliente->mainCompany()->run("SELECT conectabulmacont()");
      
      /// Hace la comprobacion de los datos introducidos y que son validos en la contabilidad.
      /// La cuenta que se especifique tiene que existir en la contabilidad. No se crean de forma
      /// automatica.
      QString cuentacliente = cliente->mainCompany()->sanearCadena(cliente->findChild<QLineEdit *>("mui_cuenta_cliente")->text());

      if ( cliente->dbValue(cliente->fieldId()).isEmpty() ) return -1;

      
      if (cuentacliente.isEmpty()) {

	/// En el caso de dejar vacio el campo de cuenta preferente
	/// se mira si antes se hizo utilizo la cuenta preferente
	/// y restaura la cuenta original.
	query = "SELECT origenidcuentacliente FROM cliente WHERE idcliente = " + cliente->dbValue(cliente->fieldId());
	tmp_cliente = cliente->mainCompany()->loadQuery(query);

	if (!tmp_cliente->value("origenidcuentacliente").isEmpty()) {
	    /// Restaura cuenta original
	    query = "UPDATE cliente SET idcuentacliente = " + tmp_cliente->value("origenidcuentacliente") + " WHERE idcliente = " + cliente->dbValue(cliente->fieldId());
	    cliente->mainCompany()->runQuery(query);
	    
	    /// Vacia campo origen.
	    query = "UPDATE cliente SET origenidcuentacliente = NULL WHERE idcliente = " + cliente->dbValue(cliente->fieldId());
	    cliente->mainCompany()->runQuery(query);
	} // end if
	
      } else {
	/// Verifica que el idcuenta guardado y el codigo introducido coincidan
	/// si no coinciden entonces el codigo ha cambiado y se tiene que buscar
	/// a que idcuenta corresponde y hacer la actualizacion.
	
	query = "SELECT codigo FROM bc_cuenta WHERE idcuenta = '" + cliente->dbValue("idcuentacliente") + "'";
	rsa = cliente->mainCompany()->loadQuery(query);
	if (rsa != NULL) {
	  
	  if (rsa->value("codigo") != cuentacliente) {
		  
	      QString query_cliente = "SELECT idcuenta FROM bc_cuenta WHERE codigo = '" + cuentacliente + "' LIMIT 1";
	      rec_cliente = cliente->mainCompany()->loadQuery(query_cliente);
	      
	      if (rec_cliente->numregistros() <= 0) {
		throw -300;
	      } // end if

	      /// Almacena la informacion de la cuenta antes de sobreescribirla.
	      /// Verifica primero que el campo este vacio.
	      query = "SELECT origenidcuentacliente FROM cliente WHERE idcliente = " + cliente->dbValue(cliente->fieldId());
	      tmp_cliente = cliente->mainCompany()->loadQuery(query);
	      
	      if (tmp_cliente->value("origenidcuentacliente").isEmpty()) {
		query = "SELECT idcuentacliente FROM cliente WHERE idcliente = " + cliente->dbValue(cliente->fieldId());
		tmp_cliente = cliente->mainCompany()->loadQuery(query);
		query = "UPDATE cliente SET origenidcuentacliente = " + tmp_cliente->value("idcuentacliente") + " WHERE idcliente = " + cliente->dbValue(cliente->fieldId());
		cliente->mainCompany()->runQuery(query);
	      } // end if

	      query = "UPDATE cliente SET idcuentacliente = " + rec_cliente->value("idcuenta") + " WHERE idcliente = " + cliente->dbValue(cliente->fieldId());
	      cliente->mainCompany()->runQuery(query);
	      
	    } // end if
	    
	  } // end if
	  
	} // end if
	
    } catch (int e) {
	if (e == -300) {
	  blMsgError(_("La cuenta no existe en la contabilidad."));
	} // end if
	throw -1;
    } // end try

    return 0;
}

*/
