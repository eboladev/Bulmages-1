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


ArticuloList *g_articulosList=NULL;

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
    if (g_articulosList) {
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
        ArticuloView * bud = new ArticuloView((BfCompany *)mainCompany(), NULL);
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
    TipoArticuloList *pag = new TipoArticuloList ( (BfCompany *)mainCompany(), 0, FALSE );
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
    FamiliasView *pag = new FamiliasView ( (BfCompany *)mainCompany(), 0, FALSE );
    mainCompany() ->m_pWorkspace->addWindow ( pag );
    pag->show();
    _depura ( "END MyPlugArt::elslot3", 0 );
}



///
/**
\param bges
**/
void MyPlugArt::inicializa ( Bulmafact *bges )
{
    _depura ( "MyPlugArt::inicializa", 0 );

    if ( bges->getcompany()->has_table_privilege ( "articulo", "SELECT" ) ) {

    /// Miramos si existe un menu Articulos
	QMenu *pPluginMenu = bges->newMenu("&Articulos", "menuArticulos", "menuMaestro");
	pPluginMenu->addSeparator();

	/// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
	m_bges = bges;
	setMainCompany ( bges->getcompany() );
	QAction *planCuentas = new QAction ( _( "&Articulos" ), 0 );
	planCuentas->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/product-list.svg" ) ));
	planCuentas->setStatusTip ( _( "Articulos" ) );
	planCuentas->setWhatsThis ( _( "Articulos" ) );
	pPluginMenu->addAction ( planCuentas );
	bges->Listados->addAction (planCuentas);
	connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

	QAction *npago = new QAction ( _( "&Nuevo Articulo" ), 0 );
	npago->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/product.svg" ) ));
	npago->setStatusTip ( _( "Nuevo cliente" ) );
	npago->setWhatsThis ( _( "Nuevo cliente" ) );
	pPluginMenu->addAction ( npago );
	bges->Fichas->addAction (npago);
	connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );

	pPluginMenu->addSeparator();
	QAction *tart = new QAction ( _( "&Tipos de Articulo" ), 0 );
	tart->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/product-family.svg" ) ));
	tart->setStatusTip ( _( "Tipos de Articulo" ) );
	tart->setWhatsThis ( _( "Tipos de Articulo" ) );
	pPluginMenu->addAction ( tart );
	bges->Fichas->addAction (tart);
	connect ( tart, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );

	pPluginMenu->addSeparator();
	QAction *tfam = new QAction ( _( "&Familias" ), 0 );
	tfam->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/product-family.svg" ) ));
	tfam->setStatusTip ( _( "Familias" ) );
	tfam->setWhatsThis ( _( "Familias" ) );
	pPluginMenu->addAction ( tfam );
	bges->Fichas->addAction (tfam);
	connect ( tfam, SIGNAL ( activated() ), this, SLOT ( elslot3() ) );

    }// end if
    _depura ( "END MyPlugArt::inicializa", 0 );
}





///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Punto de Entrada del plugin de Clientes\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginbf_articulo", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    MyPlugArt *plug = new MyPlugArt();
    plug->inicializa ( bges );
    return 0;
}


int BfCompany_createMainWindows_Post(BfCompany *comp) {
    if ( comp->has_table_privilege ( "articulo", "SELECT" ) ) {
	g_articulosList = new ArticuloList( comp, NULL );	
	comp->m_pWorkspace->addWindow ( g_articulosList );
	g_articulosList->hide();
    }// end if
    return 0;
}


int Busqueda_on_mui_buscar_clicked(Busqueda *busq) {
	if (busq->tableName() == "articulo") {


    QDialog *diag = new QDialog ( 0 );
    diag->setModal ( true );
    diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
    centrarEnPantalla ( diag );

    ArticuloList *arts = new ArticuloList ( ( BfCompany * ) busq->mainCompany(), diag, 0, ArticuloList::SelectMode );
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



	if (busq->tableName() == "tipo_articulo") {



    QDialog *diag = new QDialog ( 0 );
    diag->setModal ( true );
    diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
    centrarEnPantalla ( diag );

    TipoArticuloList *arts = new TipoArticuloList ( (BfCompany *) busq->mainCompany(), 0, TRUE );

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



	if (busq->tableName() == "familia") {



    QDialog *diag = new QDialog ( 0 );
    diag->setModal ( true );
    diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
    centrarEnPantalla ( diag );

    FamiliasView *arts = new FamiliasView ( (BfCompany *) busq->mainCompany(), 0, TRUE );

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


int BfSubForm_pressedAsterisk(BfSubForm *sub) {

    if ( sub->m_campoactual->nomcampo() != "codigocompletoarticulo" ) {
        _depura ( "END BfSubForm::pressedAsterisk", 0 );
        return 0;
    } // end if

    ArticuloList *artlist = new ArticuloList ( ( BfCompany * ) sub->mainCompany(), NULL, 0, ArticuloList::SelectMode );
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    sub->setEnabled ( false );
    artlist->show();
    while ( !artlist->isHidden() )
        theApp->processEvents();
    sub->setEnabled ( true );
    QString idArticle = artlist->idarticulo();
    delete artlist;

    /// Si no tenemos un idarticulo salimos ya que significa que no se ha seleccionado ninguno.
    if ( idArticle == "" ) {
        _depura ( "END BfSubForm::pressedAsterisk", 0 );
        return 0;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->cargacursor ( "SELECT * FROM articulo WHERE idarticulo = " + idArticle );
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
int SNewArticuloView (BfCompany *v)
{
	ArticuloView *h = new ArticuloView(v, 0);
	g_plugParams = h;
	return 1;
}
