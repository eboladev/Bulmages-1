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

#include "pluginbf_proveedor.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "provedit.h"
#include "providerslist.h"



ProveedorList *g_providersList = NULL;
BfBulmaFact *g_pluginbf_proveedor = NULL;



///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_proveedor", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );
    g_pluginbf_proveedor = bges;

    if ( bges->company()->hasTablePrivilege ( "proveedor", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->newMenu ( _("&Compras"), "menuCompras", "menuMaestro" );
        pPluginMenu->addSeparator();
        
        /// El men&uacute; de proveedores
        BlAction *accionA = new BlAction ( _ ( "&Proveedores" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier-list.png" ) ) );
        accionA->setStatusTip ( _ ( "Proveedores" ) );
        accionA->setWhatsThis ( _ ( "Proveedores" ) );
        accionA->setObjectName("mui_actionProveedores");
        pPluginMenu->addAction ( accionA );
        bges->Listados->addAction ( accionA );

        BlAction *accionB = new BlAction ( _ ( "&Nuevo proveedor" ), 0 );
        accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier.png" ) ) );
        accionB->setStatusTip ( _ ( "Nuevo proveedor" ) );
        accionB->setWhatsThis ( _ ( "Nuevo proveedor" ) );
        accionB->setObjectName("mui_actionProveedorNuevo");
        pPluginMenu->addAction ( accionB );
        bges->Fichas->addAction ( accionB );

    } // end if

    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, "PluginBf_Proveedor" );
    if (accion->objectName() == "mui_actionProveedores") {
        if ( g_providersList ) {
            g_providersList->hide();
            g_providersList->show();
        } // end if
    } // end if

    if (accion->objectName() == "mui_actionProveedorNuevo") {
        ProveedorView * bud = new ProveedorView ( g_pluginbf_proveedor->company(), NULL );
        g_pluginbf_proveedor->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "proveedor", "SELECT" ) ) {
        g_providersList = new ProveedorList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_providersList );
        g_providersList->hide();
    } // end if
    return 0;
}


int Busqueda_on_mui_buscar_clicked ( BlSearchWidget *busq )
{
    if ( busq->tableName() == "proveedor" ) {


        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        blCenterOnScreen ( diag );

        ProveedorList *clients = new ProveedorList ( ( BfCompany * ) busq->mainCompany(), diag, 0, BL_SELECT_MODE );
        busq->connect ( clients, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

        /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
        /// para que sea redimensionable y aparezca el titulo de la ventana.
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget ( clients );
        layout->setMargin ( 0 );
        layout->setSpacing ( 0 );
        diag->setLayout ( layout );
        diag->setWindowTitle ( clients->windowTitle() );

        diag->exec();
        if ( clients->idprovider() != "" ) {
            busq->setId ( clients->idprovider() );
        } // end if
        delete diag;


        return 1;
    } // end if
    return 0;

}


/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewProveedorView ( BfCompany *v )
{
    ProveedorView *h = new ProveedorView ( v, 0 );
    g_plugParams = h;
    return 1;
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
SubForm_Proveedor::SubForm_Proveedor ( BlSubForm *parent ) : QObject ( parent )
{
    BL_FUNC_DEBUG
    
}

///
/**
**/
SubForm_Proveedor::~SubForm_Proveedor()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu
**/
void SubForm_Proveedor::s_pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
    BfSubForm *sub = ( BfSubForm * ) parent();
    BlSubFormHeader *header = sub->header ( "cifproveedor" );
    if ( header ) {
        menu->addSeparator();
        menu->addAction ( QIcon ( ":/Images/supplier.png"), _ ( "Nuevo proveedor" ) );
        QString idarticulo = sub->dbValue ( "idproveedor" );
        if ( idarticulo != "" ) menu->addAction ( _ ( "Editar proveedor" ) );
        if ( ! ( header->options() & BlSubFormHeader::DbNoWrite ) )  {
            menu->addAction ( QIcon ( ":/Images/supplier-list.png"), _ ( "Seleccionar proveedor" ) );
        } // end if
    } // end if
    
}


///
/**
\param action
**/
void SubForm_Proveedor::s_trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG
    BfSubForm *sub = ( BfSubForm * ) parent();
    if ( action->text() == _ ( "Editar proveedor" ) ) {
        QString idarticulo = sub->dbValue ( "idproveedor" );
        if ( idarticulo != "" )
            editarProveedor ( idarticulo );
    } else if ( action->text() == _ ( "Seleccionar proveedor" ) ) {
        seleccionarProveedor ( sub );
    } else if ( action->text() == _ ( "Nuevo proveedor" )  ) {
        nuevoProveedor();
    } // end if

    
}


///
/**
**/
void SubForm_Proveedor::editarProveedor ( QString idarticulo )
{
    BL_FUNC_DEBUG
    BlSubForm * subf = ( BlSubForm * ) parent();
    ProveedorView * art = new ProveedorView ( ( BfCompany * ) subf->mainCompany(), 0 );
    subf->mainCompany() ->m_pWorkspace->addSubWindow ( art );
    /// Si la carga no va bien entonces terminamos.
    if ( art->load ( idarticulo ) ) {
        delete art;
	
        return;
    } // end if
    art->hide();
    art->show();
    
}


///
/**
**/
void SubForm_Proveedor::nuevoProveedor( )
{
    BL_FUNC_DEBUG
  
    BlSubForm * sub = ( BlSubForm * ) parent();

    QString idarticleold="0";
    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT max(idproveedor) as id FROM proveedor " );
    if ( !cur->eof() ) {
	idarticleold = cur->value("id");
    } // end if
    delete cur;
    
    ProveedorView *artlist = new ProveedorView ( ( BfCompany * ) sub->mainCompany(), 0 );
	/// Desabilitamos el borrado automatico de la clase y lo realizaremos manualmente para que no se llame al isHidden sobre un objeto eliminado.
    artlist->setAttribute ( Qt::WA_DeleteOnClose, false );
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    sub->setEnabled ( false );
    artlist->show();
    while ( !artlist->isHidden() )
        g_theApp->processEvents();
    sub->setEnabled ( true );
    delete artlist;
	
    cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM proveedor ORDER BY idproveedor DESC LIMIT 1" );
    if ( !cur->eof() ) {
		if (cur->value("idproveedor") != idarticleold) {
		  sub->lineaact()->setDbValue ( "idproveedor", cur->value("idproveedor") );
		  sub->lineaact()->setDbValue ( "cifproveedor", cur->value( "cifproveedor" ) );
		  sub->m_registrolinea->setDbValue ( "nomproveedor", cur->value( "nomproveedor" ) );
		} // end if
    } // end if
    delete cur;

    
}


///
/**
**/
void SubForm_Proveedor::seleccionarProveedor ( BfSubForm *sub )
{
    BL_FUNC_DEBUG

    if (!sub) sub= (BfSubForm *) parent();
    
    
    ProveedorList *artlist = new ProveedorList ( ( BfCompany * ) sub->mainCompany(), NULL, 0, BL_SELECT_MODE );
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    sub->setEnabled ( false );
    artlist->show();
    while ( !artlist->isHidden() )
        g_theApp->processEvents();
    sub->setEnabled ( true );
    QString idArticle = artlist->idprovider();
    delete artlist;

    /// Si no tenemos un idarticulo salimos ya que significa que no se ha seleccionado ninguno.
    if ( idArticle == "" ) {
        
        return;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM proveedor WHERE idproveedor = " + idArticle );
    if ( !cur->eof() ) {
        sub->lineaact()->setDbValue ( "idproveedor", idArticle );
        sub->lineaact()->setDbValue ( "cifproveedor", cur->value( "cifproveedor" ) );
	sub->m_registrolinea->setDbValue ( "nomproveedor", cur->value( "nomproveedor" ) );
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
    SubForm_Proveedor *subformods = new SubForm_Proveedor ( sub );
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
    BlSubFormHeader *header = sub->header ( "cifproveedor" );
    if ( header ) {
	SubForm_Proveedor *subformods = new SubForm_Proveedor ( sub );
	
	QHBoxLayout *m_hboxLayout1 = sub->mui_menusubform->findChild<QHBoxLayout *> ( "hboxLayout1" );
	if ( !m_hboxLayout1 ) {
	    m_hboxLayout1 = new QHBoxLayout ( sub->mui_menusubform );
	    m_hboxLayout1->setSpacing (0 );
	    m_hboxLayout1->setMargin ( 0 );
	    m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
	} // end if
	
	if ( ! ( header->options() & BlSubFormHeader::DbNoWrite ) )  {
	  QToolButton *sel = new QToolButton ( sub->mui_menusubform );
	  sel->setStatusTip ( "Nuevo Proveedor" );
	  sel->setToolTip ( "Nuevo Proveedor" );
	  sel->setMinimumSize ( QSize ( 18, 18 ) );
	  sel->setIcon ( QIcon ( ":/Images/supplier.png" ) );
	  sel->setIconSize ( QSize ( 18, 18 ) );    
	  m_hboxLayout1->addWidget ( sel );
	  sel->connect (sel, SIGNAL(released()), subformods, SLOT(nuevoProveedor()));
	
	  QToolButton *sel1 = new QToolButton ( sub->mui_menusubform );
	  sel1->setStatusTip ( "Seleccionar Proveedor" );
	  sel1->setToolTip ( "Seleccionar Proveedor" );
	  sel1->setMinimumSize ( QSize ( 18, 18 ) );
	  sel1->setIcon ( QIcon ( ":/Images/supplier-list.png" ) );
	  sel1->setIconSize ( QSize ( 18, 18 ) );    
	  m_hboxLayout1->addWidget ( sel1 );
	  sel1->connect (sel1, SIGNAL(released()), subformods, SLOT(seleccionarProveedor()));
	} // end if
    } // end if
    return 0;
}



/// Terminamos el tema de iconos en el menu de subformulario

int BlSubFormDelegate_createEditor ( BlSubFormDelegate *bl )
{
    BL_FUNC_DEBUG
    int ret = 0;
    if ( g_fieldName == "cifproveedor" ) {
        g_main->statusBar()->showMessage(_("<F2> Abre selector de proveedores."), 2000);
        BlDbCompleterComboBox * editor = new BlDbCompleterComboBox ( g_editor );
        editor->setObjectName ( "EditCifProveedor" );
        editor->setMainCompany ( ( BfCompany * ) bl->m_subform->mainCompany() );
	editor->m_valores["cifproveedor"] = "";
	editor->m_valores["nomproveedor"] = "";
        editor->m_tabla = "idproveedor";
        g_plugParams =  editor;
        ret = -1;
    } // end if
    return ret;
}


/// Hay cosas que deberian estar en el plugin de alumno
int BlSubFormDelegate_setModelData ( BlSubFormDelegate *bl )
{
    BL_FUNC_DEBUG
    int ret = 0;
    if ( g_editor->objectName() == "EditCifProveedor" ) {
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
    if ( g_editor->objectName() == "EditCifProveedor" ) {
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
    if ( sub->m_campoactual->fieldName() == "cifproveedor" ) {
        QString params[1]= {  sub->m_campoactual->text() };
	QString query = "SELECT idproveedor FROM proveedor WHERE cifproveedor = $1";
        BlDbRecordSet *cur = sub->mainCompany() -> loadQuery( query, 1, params );
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idproveedor", cur->value( "idproveedor" ) );
	    
        } // end if
        delete cur;
    } // end if
    
    return 0;
}


int BlDbCompleterComboBox_textChanged (BlDbCompleterComboBox *bl) {
  BL_FUNC_DEBUG
/*
  if ( bl->m_entrada.size() >= 3 && bl->m_tabla == "articulo") {
           // no se si es el autoComplete o que pero em criden a
           // aquesta senyal quan omplo el combo, amb el primer valor
           // i si no m'aturo ara, recalcularia el combo amb nomes
           // aquest valor encara que l'usuari nomes hagi escrit
           // un prefix que permeti mes candidats
           if ( bl->entrada().indexOf ( ".-" ) < 0 )  {
                QString cadwhere = "";
                /// Inicializamos los valores de vuelta a ""
                QString SQLQuery = "SELECT * FROM " + bl->m_tabla + " WHERE upper(codigocompletoarticulo) LIKE  upper('"+bl->m_entrada+"'||'%') OR upper(nomarticulo) LIKE upper('"+bl->m_entrada+"'||'%') ORDER BY nomarticulo";
                bl->m_cursorcombo = bl->mainCompany() ->load ( SQLQuery);
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
           }
	   return 1;
   } // end if
   */
   return 0;
}





