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

#include "pluginbf_articulo.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "articulolist.h"
#include "articuloview.h"
#include "tiposarticuloview.h"
#include "familiasview.h"


ArticuloList *g_articulosList = NULL;
BfBulmaFact *g_pluginbf_articulo = NULL;


BlAction * g_pbf_articulo_actionA = NULL;
BlAction * g_pbf_articulo_actionB = NULL;
BlAction * g_pbf_articulo_actionC = NULL;
BlAction * g_pbf_articulo_actionD = NULL;

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
    blBindTextDomain ( "pluginbf_articulo", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    g_pluginbf_articulo = bges;

    if ( bges->company()->hasTablePrivilege ( "articulo", "SELECT" ) ) {

        /// Miramos si existe un menu Articulos
        QMenu *pPluginMenu = bges->newMenu ( _("&Articulos"), "menuArticulos", "menuMaestro" );
        pPluginMenu->addSeparator();

        /// El men&uacute; de Articulos en la secci&oacute;n de art&iacute;culos.
        

        BlAction *accionA = g_pbf_articulo_actionA = new BlAction ( _ ( "&Articulos" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/product-list.png" ) ) );
        accionA->setStatusTip ( _ ( "Articulos" ) );
        accionA->setWhatsThis ( _ ( "Articulos" ) );
        accionA->setObjectName("mui_actionArticulos");
        pPluginMenu->addAction ( accionA );
        bges->Listados->addAction ( accionA );

        BlAction *accionB = g_pbf_articulo_actionB = new BlAction ( _ ( "&Nuevo articulo" ), 0 );
        accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/product.png" ) ) );
        accionB->setStatusTip ( _ ( "Nuevo articulo" ) );
        accionB->setWhatsThis ( _ ( "Nuevo articulo" ) );
        accionB->setObjectName("mui_actionArticuloNuevo");
        pPluginMenu->addAction ( accionB );
        bges->Fichas->addAction ( accionB );

        pPluginMenu->addSeparator();
        BlAction *accionC = g_pbf_articulo_actionC = new BlAction ( _ ( "&Tipos de articulo" ), 0 );
        accionC->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/product-type.png" ) ) );
        accionC->setStatusTip ( _ ( "Tipos de articulo" ) );
        accionC->setWhatsThis ( _ ( "Tipos de articulo" ) );
        accionC->setObjectName("mui_actionArticulosTipo");
        pPluginMenu->addAction ( accionC );
        bges->Fichas->addAction ( accionC );

        pPluginMenu->addSeparator();
        BlAction *accionD = g_pbf_articulo_actionD = new BlAction ( _ ( "&Familias" ), 0 );
        accionD->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/product-family.png" ) ) );
        accionD->setStatusTip ( _ ( "Familias" ) );
        accionD->setWhatsThis ( _ ( "Familias" ) );
        accionD->setObjectName("mui_actionFamilias");
        pPluginMenu->addAction ( accionD );
        bges->Fichas->addAction ( accionD );
    } // end if

    return 0;
}

int exitPoint ( BfBulmaFact *bges ) {
    BL_FUNC_DEBUG
    
    /* delete g_articulosList; */ // Es borrado al vaciar el windowList
    
    delete g_pbf_articulo_actionA;
    delete g_pbf_articulo_actionB;
    delete g_pbf_articulo_actionC;
    delete g_pbf_articulo_actionD;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, _("PluginBf_Articulo") );

    if (accion->objectName() == "mui_actionArticulos") {
        if ( g_articulosList ) {
            g_articulosList->hide();
            g_articulosList->show();
        } // end if
    } // end if

    if (accion->objectName() == "mui_actionArticuloNuevo") {
        ArticuloView * bud = new ArticuloView ( g_pluginbf_articulo->company(), NULL );
        g_pluginbf_articulo->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if

    if (accion->objectName() == "mui_actionArticulosTipo") {
        TipoArticuloList *pag = new TipoArticuloList ( g_pluginbf_articulo->company(), 0, FALSE );
        g_pluginbf_articulo->company()->m_pWorkspace->addSubWindow ( pag );
        pag->show();
    } // end if

    if (accion->objectName() == "mui_actionFamilias") {
        FamiliasView *pag = new FamiliasView ( g_pluginbf_articulo->company(), 0, FALSE );
        g_pluginbf_articulo->company()->m_pWorkspace->addSubWindow ( pag );
        pag->show();
    } // end if

    
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "articulo", "SELECT" ) ) {
        g_articulosList = new ArticuloList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_articulosList );
        g_articulosList->hide();
    }// end if
    return 0;
}


int Busqueda_on_mui_buscar_clicked ( BlSearchWidget *busq )
{
    if ( busq->tableName() == "articulo" ) {

        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        blCenterOnScreen ( diag );

        ArticuloList *arts = new ArticuloList ( ( BfCompany * ) busq->mainCompany(), diag, 0, BL_SELECT_MODE );
        busq->connect ( arts, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

        /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
        /// para que sea redimensionable y aparezca el titulo de la ventana.
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget ( arts );
        layout->setMargin ( 0 );
        layout->setSpacing ( 0 );
        diag->setLayout ( layout );
        diag->setWindowTitle ( arts->windowTitle() );

        diag->exec();
        if ( arts->idarticulo() != "" ) {
            busq->setId ( arts->idarticulo() );
        } // end if
        delete diag;


        return 1;
    } // end if


    if ( busq->tableName() == "tipo_articulo" ) {

        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        blCenterOnScreen ( diag );

        TipoArticuloList *arts = new TipoArticuloList ( ( BfCompany * ) busq->mainCompany(), 0, TRUE );

        busq->connect ( arts, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

        /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
        /// para que sea redimensionable y aparezca el titulo de la ventana.
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget ( arts );
        layout->setMargin ( 0 );
        layout->setSpacing ( 0 );
        diag->setLayout ( layout );
        diag->setWindowTitle ( arts->windowTitle() );

        diag->exec();
        if ( arts->codtipo_articulo() != "" ) {
            busq->setId ( arts->idtipo_articulo() );
        } // end if
        delete diag;

        return 1;
    } // end if


    if ( busq->tableName() == "familia" ) {

        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        blCenterOnScreen ( diag );

        FamiliasView *arts = new FamiliasView ( ( BfCompany * ) busq->mainCompany(), 0, TRUE );

        busq->connect ( arts, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

        /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
        /// para que sea redimensionable y aparezca el titulo de la ventana.
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget ( arts );
        layout->setMargin ( 0 );
        layout->setSpacing ( 0 );
        diag->setLayout ( layout );
        diag->setWindowTitle ( arts->windowTitle() );

        diag->exec();
        if ( arts->idFamilia() != "" ) {
            busq->setId ( arts->idFamilia() );
        } // end if
        delete diag;

        return 1;
    } // end if


    return 0;
}


int BfSubForm_pressedAsterisk ( BfSubForm *sub )
{

    if ( sub->m_campoactual->fieldName() != "codigocompletoarticulo" ) {
        
        return 0;
    } // end if

    ArticuloList *artlist = new ArticuloList ( ( BfCompany * ) sub->mainCompany(), NULL, 0, BL_SELECT_MODE );
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    sub->setEnabled ( false );
    blCenterOnScreen(artlist);
    artlist->show();
    artlist->m_filtro->setFocus(Qt::PopupFocusReason);
    while ( !artlist->isHidden() )
        g_theApp->processEvents();
    sub->setEnabled ( true );
    QString idArticle = artlist->idarticulo();
    delete artlist;

    /// Si no tenemos un idarticulo salimos ya que significa que no se ha seleccionado ninguno.
    if ( idArticle == "" ) {
        
        return 0;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM articulo WHERE idarticulo = " + idArticle );
    if ( !cur->eof() ) {
        sub->m_registrolinea->setDbValue ( "idarticulo", idArticle );
        sub->m_registrolinea->setDbValue ( "codigocompletoarticulo", cur->value( "codigocompletoarticulo" ) );
	sub->m_registrolinea->setDbValue ( "nomarticulo", cur->value( "nomarticulo" ) );
    } // end if
    delete cur;

    return 0;
}


/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewArticuloView ( BfCompany *v )
{
    ArticuloView *h = new ArticuloView ( v, 0 );
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
SubForm_Articulo::SubForm_Articulo ( BlSubForm *parent ) : QObject ( parent )
{
    BL_FUNC_DEBUG
    
}

///
/**
**/
SubForm_Articulo::~SubForm_Articulo()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu
**/
void SubForm_Articulo::s_pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
    BfSubForm *sub = ( BfSubForm * ) parent();
    BlSubFormHeader *header = sub->header ( "codigocompletoarticulo" );
    if ( header ) {
        menu->addSeparator();
        menu->addAction ( QIcon ( ":/Images/product.png"), _ ( "Nuevo articulo" ) );
        QString idarticulo = sub->dbValue ( "idarticulo" );
        if ( idarticulo != "" ) menu->addAction ( _ ( "Editar articulo" ) );
        if ( ! ( header->options() & BlSubFormHeader::DbNoWrite ) )  {
            menu->addAction ( QIcon ( ":/Images/product-list.png"), _ ( "Seleccionar articulo" ) );
        } // end if
    } // end if
    
}


///
/**
\param action
**/
void SubForm_Articulo::s_trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG
    BfSubForm *sub = ( BfSubForm * ) parent();
    if ( action->text() == _ ( "Editar articulo" ) ) {
        QString idarticulo = sub->dbValue ( "idarticulo" );
        if ( idarticulo != "" )
            editarArticulo ( idarticulo );
    } else if ( action->text() == _ ( "Seleccionar articulo" ) ) {
        seleccionarArticulo ( sub );
    } else if ( action->text() == _ ( "Nuevo articulo" )  ) {
        nuevoArticulo();
    } // end if

    
}


///
/**
**/
void SubForm_Articulo::editarArticulo ( QString idarticulo )
{
    BL_FUNC_DEBUG
    BlSubForm * subf = ( BlSubForm * ) parent();
    ArticuloView * art = new ArticuloView ( ( BfCompany * ) subf->mainCompany(), 0 );
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
void SubForm_Articulo::nuevoArticulo( )
{
    BL_FUNC_DEBUG
  
    BlSubForm * sub = ( BlSubForm * ) parent();

    QString idarticleold="0";
    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT max(idarticulo) as id FROM articulo " );
    if ( !cur->eof() ) {
	idarticleold = cur->value("id");
    } // end if
    delete cur;
    
    ArticuloView *artlist = new ArticuloView ( ( BfCompany * ) sub->mainCompany(), 0 );
	/// Desabilitamos el borrado automatico de la clase y lo realizaremos manualmente para que no se llame al isHidden sobre un objeto eliminado.
    artlist->setAttribute ( Qt::WA_DeleteOnClose, FALSE );
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    sub->setEnabled ( false );
    artlist->show();
    while ( !artlist->isHidden() )
        g_theApp->processEvents();
    sub->setEnabled ( true );
    delete artlist;
	
    cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM articulo ORDER BY idarticulo DESC LIMIT 1" );
    if ( !cur->eof() ) {
		if (cur->value("idarticulo") != idarticleold) {
		  sub->lineaact()->setDbValue ( "idarticulo", cur->value("idarticulo") );
		  sub->lineaact()->setDbValue ( "codigocompletoarticulo", cur->value( "codigocompletoarticulo" ) );
		      sub->m_registrolinea->setDbValue ( "nomarticulo", cur->value( "nomarticulo" ) );
		} // end if
    } // end if
    delete cur;

    
}


///
/**
**/
void SubForm_Articulo::seleccionarArticulo ( BfSubForm *sub )
{
    BL_FUNC_DEBUG

    if (!sub) sub= (BfSubForm *) parent();
    
    
    ArticuloList *artlist = new ArticuloList ( ( BfCompany * ) sub->mainCompany(), NULL, 0, BL_SELECT_MODE );
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    sub->setEnabled ( false );
    artlist->show();
    while ( !artlist->isHidden() )
        g_theApp->processEvents();
    sub->setEnabled ( true );
    QString idArticle = artlist->idarticulo();
    delete artlist;

    /// Si no tenemos un idarticulo salimos ya que significa que no se ha seleccionado ninguno.
    if ( idArticle == "" ) {
        
        return;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM articulo WHERE idarticulo = " + idArticle );
    if ( !cur->eof() ) {
        sub->lineaact()->setDbValue ( "idarticulo", idArticle );
        sub->lineaact()->setDbValue ( "codigocompletoarticulo", cur->value( "codigocompletoarticulo" ) );
	sub->m_registrolinea->setDbValue ( "nomarticulo", cur->value( "nomarticulo" ) );
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
    SubForm_Articulo *subformods = new SubForm_Articulo ( sub );
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
    BlSubFormHeader *header = sub->header ( "codigocompletoarticulo" );
    if ( header ) {
	SubForm_Articulo *subformods = new SubForm_Articulo ( sub );
	
	QHBoxLayout *m_hboxLayout1 = sub->mui_menusubform->findChild<QHBoxLayout *> ( "hboxLayout1" );
	if ( !m_hboxLayout1 ) {
	    m_hboxLayout1 = new QHBoxLayout ( sub->mui_menusubform );
	    m_hboxLayout1->setSpacing (0 );
	    m_hboxLayout1->setMargin ( 0 );
	    m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
	} // end if
	
	if ( ! ( header->options() & BlSubFormHeader::DbNoWrite ) )  {
	  QToolButton *sel = new QToolButton ( sub->mui_menusubform );
	  sel->setStatusTip ( "Nuevo Articulo" );
	  sel->setToolTip ( "Nuevo Articulo" );
	  sel->setMinimumSize ( QSize ( 18, 18 ) );
	  sel->setIcon ( QIcon ( ":/Images/product.png" ) );
	  sel->setIconSize ( QSize ( 18, 18 ) );    
	  m_hboxLayout1->addWidget ( sel );
	  sel->connect (sel, SIGNAL(released()), subformods, SLOT(nuevoArticulo()));
	
	  QToolButton *sel1 = new QToolButton ( sub->mui_menusubform );
	  sel1->setStatusTip ( "Seleccionar Articulo" );
	  sel1->setToolTip ( "Seleccionar Articulo" );
	  sel1->setMinimumSize ( QSize ( 18, 18 ) );
	  sel1->setIcon ( QIcon ( ":/Images/product-list.png" ) );
	  sel1->setIconSize ( QSize ( 18, 18 ) );    
	  m_hboxLayout1->addWidget ( sel1 );
	  sel1->connect (sel1, SIGNAL(released()), subformods, SLOT(seleccionarArticulo()));
	} // end if
    } // end if
    return 0;
}



/// Terminamos el tema de iconos en el menu de subformulario

int BlSubFormDelegate_createEditor ( BlSubFormDelegate *bl )
{
    BL_FUNC_DEBUG
    int ret = 0;
    if ( g_fieldName == "codigocompletoarticulo" ) {
        g_main->statusBar()->showMessage(_("<F2> Abre selector de articulos."), 2000);
        BlDbCompleterComboBox * editor = new BlDbCompleterComboBox ( g_editor );
        editor->setObjectName ( "EditCodigoCompletoArticulo" );
        editor->setMainCompany ( ( BfCompany * ) bl->m_subform->mainCompany() );
	editor->m_valores["codigocompletoarticulo"] = "";
	editor->m_valores["nomarticulo"] = "";
        editor->m_tabla = "articulo";
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
    if ( g_editor->objectName() == "EditCodigoCompletoArticulo" ) {
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
    if ( g_editor->objectName() == "EditCodigoCompletoArticulo" ) {
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
    if ( sub->m_campoactual->fieldName() == "codigocompletoarticulo" ) {
        QString params[1]= {  sub->m_campoactual->text() };
	QString query = "SELECT idarticulo FROM articulo WHERE codigocompletoarticulo = $1";
        BlDbRecordSet *cur = sub->mainCompany() -> loadQuery( query, 1, params );
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idarticulo", cur->value( "idarticulo" ) );
        } // end if
        delete cur;
    } // end if
    
    return 0;
}


int BlDbCompleterComboBox_textChanged (BlDbCompleterComboBox *bl) {
  BL_FUNC_DEBUG

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
   return 0;
}

