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

///
/**
**/
MyPlugArt::MyPlugArt()
{
    _depura ( "MyPlugArt::MyPlugArt", 0 );
    _depura ( "END MyPlugArt::MyPlugArt", 0 );
}


///
/**
**/
MyPlugArt::~MyPlugArt()
{
    _depura ( "MyPlugArt::~MyPlugArt", 0 );
    _depura ( "END MyPlugArt::~MyPlugArt", 0 );
}


///
/**
**/
void MyPlugArt::elslot()
{
    _depura ( "MyPlugArt::elslot", 0 );
    if ( g_articulosList ) {
        g_articulosList->hide();
        g_articulosList->show();
    }// end if
    _depura ( "END MyPlugArt::elslot", 0 );
}

///
/**
**/
void MyPlugArt::elslot1()
{
    _depura ( "MyPlugArt::elslot1", 0 );
    ArticuloView * bud = new ArticuloView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    _depura ( "END MyPlugArt::elslot1", 0 );
}



///
/**
**/
void MyPlugArt::elslot2()
{
    _depura ( "MyPlugArt::elslot2", 0 );
    TipoArticuloList *pag = new TipoArticuloList ( ( BfCompany * ) mainCompany(), 0, FALSE );
    mainCompany() ->m_pWorkspace->addWindow ( pag );
    pag->show();
    _depura ( "END MyPlugArt::elslot2", 0 );
}


///
/**
**/
void MyPlugArt::elslot3()
{
    _depura ( "MyPlugArt::elslot3", 0 );
    FamiliasView *pag = new FamiliasView ( ( BfCompany * ) mainCompany(), 0, FALSE );
    mainCompany() ->m_pWorkspace->addWindow ( pag );
    pag->show();
    _depura ( "END MyPlugArt::elslot3", 0 );
}



///
/**
\param bges
**/
void MyPlugArt::inicializa ( BfBulmaFact *bges )
{
    _depura ( "MyPlugArt::inicializa", 0 );

    if ( bges->company()->hasTablePrivilege ( "articulo", "SELECT" ) ) {

        /// Miramos si existe un menu Articulos
        QMenu *pPluginMenu = bges->newMenu ( "&Articulos", "menuArticulos", "menuMaestro" );
        pPluginMenu->addSeparator();

        /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
        m_bges = bges;
        setMainCompany ( bges->company() );
        QAction *planCuentas = new QAction ( _ ( "&Articulos" ), 0 );
        planCuentas->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/product-list.png" ) ) );
        planCuentas->setStatusTip ( _ ( "Articulos" ) );
        planCuentas->setWhatsThis ( _ ( "Articulos" ) );
        pPluginMenu->addAction ( planCuentas );
        bges->Listados->addAction ( planCuentas );
        connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

        QAction *npago = new QAction ( _ ( "&Nuevo Articulo" ), 0 );
        npago->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/product.png" ) ) );
        npago->setStatusTip ( _ ( "Nuevo articulo" ) );
        npago->setWhatsThis ( _ ( "Nuevo articulo" ) );
        pPluginMenu->addAction ( npago );
        bges->Fichas->addAction ( npago );
        connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );

        pPluginMenu->addSeparator();
        QAction *tart = new QAction ( _ ( "&Tipos de Articulo" ), 0 );
        tart->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/product-family.png" ) ) );
        tart->setStatusTip ( _ ( "Tipos de Articulo" ) );
        tart->setWhatsThis ( _ ( "Tipos de Articulo" ) );
        pPluginMenu->addAction ( tart );
        bges->Fichas->addAction ( tart );
        connect ( tart, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );

        pPluginMenu->addSeparator();
        QAction *tfam = new QAction ( _ ( "&Familias" ), 0 );
        tfam->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/product-family.png" ) ) );
        tfam->setStatusTip ( _ ( "Familias" ) );
        tfam->setWhatsThis ( _ ( "Familias" ) );
        pPluginMenu->addAction ( tfam );
        bges->Fichas->addAction ( tfam );
        connect ( tfam, SIGNAL ( activated() ), this, SLOT ( elslot3() ) );

    }// end if
    _depura ( "END MyPlugArt::inicializa", 0 );
}





///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    _depura ( "Punto de Entrada del plugin de Clientes\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_articulo", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    MyPlugArt *plug = new MyPlugArt();
    plug->inicializa ( bges );
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "articulo", "SELECT" ) ) {
        g_articulosList = new ArticuloList ( comp, NULL );
        comp->m_pWorkspace->addWindow ( g_articulosList );
        g_articulosList->hide();
    }// end if
    return 0;
}


int Busqueda_on_mui_buscar_released ( BlSearchWidget *busq )
{
    if ( busq->tableName() == "articulo" ) {


        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        centrarEnPantalla ( diag );

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
        centrarEnPantalla ( diag );

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
        centrarEnPantalla ( diag );

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

    if ( sub->m_campoactual->nomcampo() != "codigocompletoarticulo" ) {
        _depura ( "END BfSubForm::pressedAsterisk", 0 );
        return 0;
    } // end if

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
        _depura ( "END BfSubForm::pressedAsterisk", 0 );
        return 0;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM articulo WHERE idarticulo = " + idArticle );
    if ( !cur->eof() ) {
        sub->m_registrolinea->setDbValue ( "idarticulo", idArticle );
        sub->m_registrolinea->setDbValue ( "codigocompletoarticulo", cur->valor ( "codigocompletoarticulo" ) );
        sub->m_registrolinea->setDbValue ( "nomarticulo", cur->valor ( "nomarticulo" ) );
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
MyPlugArt1::MyPlugArt1 ( BlSubForm *parent ) : QObject ( parent )
{
    _depura ( "MyPlugArt1::MyPlugArt1", 0 );
    _depura ( "END MyPlugArt1::MyPlugArt1", 0 );
}

///
/**
**/
MyPlugArt1::~MyPlugArt1()
{
    _depura ( "MyPlugArt1::~MyPlugArt1", 0 );
    _depura ( "END MyPlugArt1::~MyPlugArt1", 0 );
}


///
/**
\param menu
**/
void MyPlugArt1::s_pintaMenu ( QMenu *menu )
{
    _depura ( "MyPlugArt1::s_pintaMenu", 0 );
    BfSubForm *sub = ( BfSubForm * ) parent();
    BlSubFormHeader *header = sub->header ( "codigocompletoarticulo" );
    if ( header ) {
        menu->addSeparator();
        menu->addAction ( _ ( "Nuevo articulo" ) );
        QString idarticulo = sub->dbValue ( "idarticulo" );
        if ( idarticulo != "" ) menu->addAction ( _ ( "Editar articulo" ) );
        if ( ! ( header->options() & BlSubFormHeader::DbNoWrite ) )  {
            menu->addAction ( _ ( "Seleccionar articulo" ) );
        } // end if
    } // end if
    _depura ( "END MyPlugArt1::s_pintaMenu", 0 );
}


///
/**
\param action
**/
void MyPlugArt1::s_trataMenu ( QAction *action )
{
    _depura ( "MyPlugArt1::s_trataMenu", 0 );
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

    _depura ( "END MyPlugArt1::s_trataMenu", 0 );
}


///
/**
**/
void MyPlugArt1::editarArticulo ( QString idarticulo )
{
    _depura ( "MyPlugArt1::editarArticulo", 0 );
    BlSubForm * subf = ( BlSubForm * ) parent();
    ArticuloView * art = new ArticuloView ( ( BfCompany * ) subf->mainCompany(), 0 );
    subf->mainCompany() ->m_pWorkspace->addWindow ( art );
    /// Si la carga no va bien entonces terminamos.
    if ( art->cargar ( idarticulo ) ) {
        delete art;
        _depura ( "END ArticuloList::editar", 0, "Carga Erronea" );
        return;
    } // end if
    art->hide();
    art->show();
    _depura ( "END MyPlugArt1::editarArticulo", 0 );
}



///
/**
**/
void MyPlugArt1::nuevoArticulo( )
{
    _depura ( "MyPlugArt1::editarArticulo", 0 );
    BlSubForm * subf = ( BlSubForm * ) parent();
    ArticuloView * art = new ArticuloView ( ( BfCompany * ) subf->mainCompany(), 0 );
    subf->mainCompany() ->m_pWorkspace->addWindow ( art );
    art->hide();
    art->show();
    _depura ( "END MyPlugArt1::editarArticulo", 0 );
}


///
/**
**/
void MyPlugArt1::seleccionarArticulo ( BfSubForm *sub )
{
    _depura ( "MyPlugArt1::editarArticulo", 0 );

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
        _depura ( "END BfSubForm::pressedAsterisk", 0 );
        return;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM articulo WHERE idarticulo = " + idArticle );
    if ( !cur->eof() ) {
        sub->lineaact()->setDbValue ( "idarticulo", idArticle );
        sub->lineaact()->setDbValue ( "codigocompletoarticulo", cur->valor ( "codigocompletoarticulo" ) );
        sub->lineaact()->setDbValue ( "nomarticulo", cur->valor ( "nomarticulo" ) );
    } // end if
    delete cur;

    _depura ( "END MyPlugArt1::editarArticulo", 0 );
}


///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    _depura ( "BlSubForm_BlSubForm_Post", 0 );
    MyPlugArt1 *subformods = new MyPlugArt1 ( sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    _depura ( "END BlSubForm_BlSubForm_Post", 0 );
    return 0;
}



int BlSubFormDelegate_createEditor ( BlSubFormDelegate *bl )
{
    _depura ( "pluginbf_articulo::BlSubFormDelegate_createEditor", 0 );
    int ret = 0;
    if ( g_nomcampo == "codigocompletoarticulo" ) {
        BlDbCompleterComboBox * editor = new BlDbCompleterComboBox ( g_editor );
        editor->setObjectName ( "EditCodigoCompletoArticulo" );
        editor->setMainCompany ( ( BfCompany * ) bl->m_subform->mainCompany() );
	editor->m_valores["codigocompletoarticulo"] = "";
	editor->m_valores["nomarticulo"] = "";
        editor->m_tabla = "articulo";
        g_plugParams =  editor;
        ret = -1;
    } // end if

    _depura ( "END pluginbf_articulo::BlSubFormDelegate_createEditor", 0 );

    return ret;
}



/// Hay cosas que deberian estar en el plugin de alumno
int BlSubFormDelegate_setModelData ( BlSubFormDelegate *bl )
{
    _depura ( "pluginbf_articulo::BlSubFormDelegate_setModelData", 0 );
    int ret = 0;
    if ( g_editor->objectName() == "EditCodigoCompletoArticulo" ) {
        BlDbCompleterComboBox * comboBox = ( BlDbCompleterComboBox * ) g_editor;
        QString value = comboBox->currentText();
        value = value.left ( value.indexOf ( ".-" ) );
        g_model->setData ( g_index, value );
        ret = -1;
    } // end if
    _depura ( "END pluginbf_articulo::BlSubFormDelegate_setModelData", 0 );
    return ret;
}


int BlSubFormDelegate_setEditorData ( BlSubFormDelegate *bl )
{
    _depura ( "pluginbf_articulo::BlSubFormDelegate_setEditorData", 0 );
    int ret = 0;
    if ( g_editor->objectName() == "EditCodigoCompletoArticulo" ) {
        QString value = g_index.model() ->data ( g_index, Qt::DisplayRole ).toString();
        BlDbCompleterComboBox *comboBox = ( BlDbCompleterComboBox * ) g_editor ;
        comboBox->addItem ( value );
        ret = -1;
    } // end if
    _depura ( "END pluginbf_articulo::BlSubFormDelegate_setEditorData", 0 );
    return ret;
}

int BlSubForm_editFinished ( BlSubForm *sub )
{
    _depura ( "pluginbf_articulo::BlSubForm_editFinished", 0 );
    if ( sub->m_campoactual->nomcampo() == "codigocompletoarticulo" ) {
	QString query = "SELECT idarticulo FROM articulo WHERE codigocompletoarticulo ='" + sub->m_campoactual->text() + "'";
        BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idarticulo", cur->valor ( "idarticulo" ) );
        } // end if
        delete cur;
    } // end if
    _depura ( "END pluginbf_articulo::BlSubForm_editFinished", 0 );
    return 0;
}


int BlDbCompleterComboBox_textChanged (BlDbCompleterComboBox *bl) {
  _depura("BlDbCompleterComboBox_textChanged", 0);

        if ( bl->m_entrada.size() >= 3 && bl->m_tabla == "articulo") {
                QString cadwhere = "";
                /// Inicializamos los valores de vuelta a ""
                QString SQLQuery = "SELECT * FROM " + bl->m_tabla + " WHERE upper(codigocompletoarticulo) LIKE  upper('" + bl->mainCompany()->sanearCadena(bl->m_entrada) + "%')";
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
                            sep = ".-";
                        } // end if
                    } // end while
                    bl->addItem ( cad , QVariant ( bl->m_cursorcombo->valor ( cad1 ) ) );
                    bl->m_cursorcombo->nextRecord();
                } // end while
                delete bl->m_cursorcombo;
        } // end if
  _depura("END BlDbCompleterComboBox_textChanged", 0);

    return 0;
}





