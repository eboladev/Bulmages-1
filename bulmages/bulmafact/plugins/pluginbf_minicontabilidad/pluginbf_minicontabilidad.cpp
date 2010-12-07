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

#include "pluginbf_minicontabilidad.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "partidasview.h"
#include "acontableview.h"
#include "presupuestocontablelist.h"


ApunteContableView *g_ap = NULL;

///
/**
**/
MyPlugArt::MyPlugArt( BfBulmaFact *bges) 
{
    blDebug ( "MyPlugArt::MyPlugArt", 0 );
    setMainCompany(bges->company());
    m_bges = bges;
    blDebug ( "END MyPlugArt::MyPlugArt", 0 );
}


///
/**
**/
MyPlugArt::~MyPlugArt()
{
    blDebug ( "MyPlugArt::~MyPlugArt", 0 );
    blDebug ( "END MyPlugArt::~MyPlugArt", 0 );
}

///
/**
**/
void MyPlugArt::elslot1()
{
    blDebug ( "MyPlugArt::elslot1", 0 );
    PresupuestoContableList *pag = new PresupuestoContableList ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->m_pWorkspace->addSubWindow ( pag );
    pag->show();
    blDebug ( "END MyPlugArt::elslot1", 0 );
}


///
/**
**/
void MyPlugArt::elslot2()
{
    blDebug ( "MyPlugArt::elslot3", 0 );
    g_ap->hide();
    g_ap->show();
    blDebug ( "END MyPlugArt::elslot3", 0 );
}



///
/**
**/
void MyPlugArt::elslot3()
{
    blDebug ( "MyPlugArt::elslot3", 0 );
    PartidasView *pag = new PartidasView ( ( BfCompany * ) mainCompany(), 0, FALSE );
    mainCompany() ->m_pWorkspace->addSubWindow ( pag );
    pag->show();
    blDebug ( "END MyPlugArt::elslot3", 0 );
}



///
/**
\param bges
**/
void MyPlugArt::inicializa ( ) 
{
    blDebug ( "MyPlugArt::inicializa", 0 );

    if ( m_bges->company()->hasTablePrivilege ( "partida", "SELECT" ) ) {

        /// Miramos si existe un menu Articulos
        QMenu *pPluginMenu = m_bges->newMenu ( _("Economia"), "menuEconomia", "menuMaestro" );
        pPluginMenu->addSeparator();

        pPluginMenu->addSeparator();
        QAction *tfam = new QAction ( _ ( "&Partidas contables" ), 0 );
        tfam->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/account_plan.png" ) ) );
        tfam->setStatusTip ( _ ( "Partidas contables" ) );
        tfam->setWhatsThis ( _ ( "Partidas contables" ) );
        pPluginMenu->addAction ( tfam );
        m_bges->Fichas->addAction ( tfam );
        connect ( tfam, SIGNAL ( activated() ), this, SLOT ( elslot3() ) );

        QAction *tfam1 = new QAction ( _ ( "&Anotaciones contables" ), 0 );
        tfam1->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/balance-sheet.png" ) ) );
        tfam1->setStatusTip ( _ ( "Anotaciones contables" ) );
        tfam1->setWhatsThis ( _ ( "Anotaciones contables" ) );
        pPluginMenu->addAction ( tfam1 );
        m_bges->Fichas->addAction ( tfam1 );
        connect ( tfam1, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );

        QAction *tfam2 = new QAction ( _ ( "&Previsiones" ), 0 );
        tfam2->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/book-list.png" ) ) );
        tfam2->setStatusTip ( _ ( "Presupuesto Contables" ) );
        tfam2->setWhatsThis ( _ ( "Presupuestos Contables" ) );
        pPluginMenu->addAction ( tfam2 );
        m_bges->Fichas->addAction ( tfam2 );
        connect ( tfam2, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );

     }// end if
    blDebug ( "END MyPlugArt::inicializa", 0 );
}





///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de Entrada del plugin de MiniContabilidad\n", 0 );

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_MiniContabilidad", "0.11.1-0002");

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_minicontabilidad", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    MyPlugArt *plug = new MyPlugArt(bges);
    plug->inicializa ( );

    g_ap = new ApunteContableView ( ( BfCompany * ) bges->company(), 0 );
    bges->company() ->m_pWorkspace->addSubWindow ( g_ap );
    
    
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "partida", "SELECT" ) ) {

    }// end if
    return 0;
}


int Busqueda_on_mui_buscar_clicked ( BlSearchWidget *busq )
{

    if ( busq->tableName() == "partida" ) {

        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        blCenterOnScreen ( diag );

        PartidasView *arts = new PartidasView ( ( BfCompany * ) busq->mainCompany(), 0, TRUE );

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
        if ( arts->idPartida() != "" ) {
            busq->setId ( arts->idPartida() );
        } // end if
        delete diag;

        return 1;
    } // end if


    return 0;
}


/// Tratamos el caso de introduccion de partidas en subformularios


int BlSubFormDelegate_createEditor ( BlSubFormDelegate *bl )
{
    blDebug ( "pluginbf_minicontabilidad::BlSubFormDelegate_createEditor", 0 );
    int ret = 0;
    if ( g_nomcampo == "codigocompletopartida"  ) {
        BlDbCompleterComboBox * editor = new BlDbCompleterComboBox ( g_editor );
        editor->setObjectName ( "EditCodigoCompletoPartida" );
        editor->setMainCompany ( ( BfCompany * ) bl->m_subform->mainCompany() );
	editor->m_valores["codigocompletopartida"] = "";
        editor->m_valores["nombrepartida"] = "";
        editor->m_tabla = "partida";
        g_plugParams =  editor;
        ret = -1;
    } // end if
    blDebug ( "END pluginbf_minicontabilidad::BlSubFormDelegate_createEditor", 0 );

    return ret;
}




int BlSubFormDelegate_setModelData ( BlSubFormDelegate *bl )
{
    blDebug ( "pluginbf_minicontabilidad::BlSubFormDelegate_setModelData", 0 );
    int ret = 0;
    if ( g_editor->objectName() == "EditCodigoCompletoPartida" ) {
        BlDbCompleterComboBox * comboBox = ( BlDbCompleterComboBox * ) g_editor;
        QString value = comboBox->currentText();
        value = value.left ( value.indexOf ( ".-" ) );
        g_model->setData ( g_index, value );
        ret = -1;
    } // end if
    blDebug ( "END pluginbf_minicontabilidad::BlSubFormDelegate_setModelData", 0 );
    return ret;
}


int BlSubFormDelegate_setEditorData ( BlSubFormDelegate *bl )
{
    blDebug ( "pluginbf_minicontabilidad::BlSubFormDelegate_setEditorData", 0 );
    int ret = 0;
    if ( g_editor->objectName() == "EditCodigoCompletoPartida"  ) {
        QString value = g_index.model() ->data ( g_index, Qt::DisplayRole ).toString();
        BlDbCompleterComboBox *comboBox = ( BlDbCompleterComboBox * ) g_editor ;
        comboBox->addItem ( value );
        ret = -1;
    } // end if
    blDebug ( "END pluginbf_minicontabilidad::BlSubFormDelegate_setEditorData", 0 );
    return ret;
}


int BlSubForm_editFinished ( BlSubForm *sub )
{
    blDebug ( "pluginbf_minicontabilidad::BlSubForm_editFinished", 0 );
    if ( sub->m_campoactual->nomcampo() == "codigocompletopartida" ) {
	QString query = "SELECT idpartida, nombrepartida, codigocompletopartida FROM partida WHERE upper (codigocompletopartida) LIKE upper('" + sub->m_campoactual->text() + "%')";

        BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idpartida", cur->valor ( "idpartida" ) );
            sub->m_registrolinea->setDbValue ( "codigocompletopartida", cur->valor ( "codigocompletopartida" ) );
            sub->m_registrolinea->setDbValue ( "nombrepartida", cur->valor ( "nombrepartida" ) );
        } // end if
        delete cur;
    } // end if
    blDebug ( "END pluginbf_minicontabilidad::BlSubForm_editFinished", 0 );
    return 0;
}


/*
int BlDbCompleterComboBox_textChanged (BlDbCompleterComboBox *bl) {
  blDebug("BlDbCompleterComboBox_textChanged", 0);

        if ( bl->m_entrada.size() >= 3 && bl->m_tabla == "alumno") {
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

  blDebug("END BlDbCompleterComboBox_textChanged", 0);

	  return 1;
        } // end if
  blDebug("END BlDbCompleterComboBox_textChanged", 0);

    return 0;
}
*/












int BfSubForm_pressedAsterisk ( BfSubForm *sub )
{
    blDebug ( "BfSubForm_pressedAsterisk" );

    if ( sub->m_campoactual->nomcampo() != "codigocompletopartida" ) {
        blDebug ( "END BfSubForm::pressedAsterisk", 0 );
        return 0;
    } // end if

        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        blCenterOnScreen ( diag );

        PartidasView *arts = new PartidasView ( ( BfCompany * ) sub->mainCompany(), 0, TRUE );

        diag->connect ( arts, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

        /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
        /// para que sea redimensionable y aparezca el titulo de la ventana.
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget ( arts );
        layout->setMargin ( 0 );
        layout->setSpacing ( 0 );
        diag->setLayout ( layout );
        diag->setWindowTitle ( arts->windowTitle() );

        diag->exec();
        if ( arts->idPartida() != "" ) {

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM partida WHERE idpartida = " + arts->idPartida() );
    if ( !cur->eof() ) {
        sub->m_registrolinea->setDbValue ( "idpartida", arts->idPartida() );
        sub->m_registrolinea->setDbValue ( "codigocompletopartida", cur->valor ( "codigocompletopartida" ));
        sub->m_registrolinea->setDbValue ( "nombrepartida", cur->valor ( "nombrepartida" ));
    } // end if
    
    delete cur;
	  
	  
        } // end if
        delete diag;

    blDebug ( "END BfSubForm_pressedAsterisk" );

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
    blDebug ( "MyPlugArt1::MyPlugArt1", 0 );
    blDebug ( "END MyPlugArt1::MyPlugArt1", 0 );
}

///
/**
**/
MyPlugArt1::~MyPlugArt1()
{
    blDebug ( "MyPlugArt1::~MyPlugArt1", 0 );
    blDebug ( "END MyPlugArt1::~MyPlugArt1", 0 );
}


///
/**
\param menu
**/
void MyPlugArt1::s_pintaMenu ( QMenu *menu )
{
    blDebug ( "MyPlugArt1::s_pintaMenu", 0 );
    BfSubForm *sub = ( BfSubForm * ) parent();
    BlSubFormHeader *header = sub->header ( "codigocompletopartida" );
    if ( header ) {
        menu->addSeparator();
        menu->addAction ( _ ( "Gestionar partidas" ) );
	menu->addAction ( _ ( "Seleccionar partida" ) );
    } // end if
    blDebug ( "END MyPlugArt1::s_pintaMenu", 0 );
}


///
/**
\param action
**/
void MyPlugArt1::s_trataMenu ( QAction *action )
{
    blDebug ( "MyPlugArt1::s_trataMenu", 0 );
    BfSubForm *sub = ( BfSubForm * ) parent();
    if ( action->text() == _ ( "Gestionar partidas" ) ) {
            gestionarPartidas ( sub);
    } else if ( action->text() == _ ( "Seleccionar partida" ) ) {
        seleccionarPartida ( sub );
    } // end if

    blDebug ( "END MyPlugArt1::s_trataMenu", 0 );
}


///
/**
**/
void MyPlugArt1::gestionarPartidas (  BfSubForm *sub )
{
    blDebug ( "MyPlugArt1::editarArticulo", 0 );
    PartidasView *pag = new PartidasView ( ( BfCompany * ) sub->mainCompany(), 0, FALSE );
    sub->mainCompany() ->m_pWorkspace->addSubWindow ( pag );
    pag->show();
    blDebug ( "END MyPlugArt1::editarArticulo", 0 );
}



///
/**
**/
void MyPlugArt1::seleccionarPartida ( BfSubForm *sub )
{
    blDebug ( "MyPlugArt1::editarArticulo", 0 );

        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        blCenterOnScreen ( diag );

        PartidasView *arts = new PartidasView ( ( BfCompany * ) sub->mainCompany(), 0, TRUE );

        diag->connect ( arts, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

        /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
        /// para que sea redimensionable y aparezca el titulo de la ventana.
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget ( arts );
        layout->setMargin ( 0 );
        layout->setSpacing ( 0 );
        diag->setLayout ( layout );
        diag->setWindowTitle ( arts->windowTitle() );

        diag->exec();
        if ( arts->idPartida() != "" ) {

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM partida WHERE idpartida = " + arts->idPartida() );

    if ( !cur->eof() ) {
        sub->lineaact()->setDbValue ( "idpartida", arts->idPartida() );
        sub->lineaact()->setDbValue ( "codigocompletopartida", cur->valor ( "codigocompletopartida" ) );
        sub->lineaact()->setDbValue ( "nombrepartida", cur->valor ( "nombrepartida" ) );
    } // end if
    delete cur;
	} // end if
    delete diag;
    
    blDebug ( "END MyPlugArt1::editarArticulo", 0 );
}


///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    blDebug ( "BlSubForm_BlSubForm_Post", 0 );
    MyPlugArt1 *subformods = new MyPlugArt1 ( sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    blDebug ( "END BlSubForm_BlSubForm_Post", 0 );
    return 0;
}





