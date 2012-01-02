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
#include "blaction.h"


ApunteContableView *g_ap = NULL;
BfBulmaFact *g_pluginbf_minicontabilidad = NULL;


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_MiniContabilidad", "0.12.1-0002");

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_minicontabilidad", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    g_pluginbf_minicontabilidad = bges;



    if ( g_pluginbf_minicontabilidad->company()->hasTablePrivilege ( "partida", "SELECT" ) ) {

        /// Miramos si existe un menu Articulos
        QMenu *pPluginMenu = g_pluginbf_minicontabilidad->newMenu ( _("Economia"), "menuEconomia", "menuMaestro" );
        pPluginMenu->addSeparator();

        pPluginMenu->addSeparator();
        BlAction *accionA = new BlAction ( _ ( "&Partidas contables" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/account_plan.png" ) ) );
        accionA->setStatusTip ( _ ( "Partidas contables" ) );
        accionA->setWhatsThis ( _ ( "Partidas contables" ) );
        accionA->setObjectName("mui_actionContablesPartidas");
        pPluginMenu->addAction ( accionA );
        g_pluginbf_minicontabilidad->Fichas->addAction ( accionA );

        BlAction *accionB = new BlAction ( _ ( "&Anotaciones contables" ), 0 );
        accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/balance-sheet.png" ) ) );
        accionB->setStatusTip ( _ ( "Anotaciones contables" ) );
        accionB->setWhatsThis ( _ ( "Anotaciones contables" ) );
        accionB->setObjectName("mui_actionContablesAnotaciones");
        pPluginMenu->addAction ( accionB );
        g_pluginbf_minicontabilidad->Fichas->addAction ( accionB );

        BlAction *accionC = new BlAction ( _ ( "&Previsiones" ), 0 );
        accionC->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/book-list.png" ) ) );
        accionC->setStatusTip ( _ ( "Presupuesto Contables" ) );
        accionC->setWhatsThis ( _ ( "Presupuestos Contables" ) );
        accionC->setObjectName("mui_actionContablesPresupuestos");
        pPluginMenu->addAction ( accionC );
        g_pluginbf_minicontabilidad->Fichas->addAction ( accionC );
     } // end if
    g_ap = new ApunteContableView ( ( BfCompany * ) bges->company(), 0 );
    bges->company() ->m_pWorkspace->addSubWindow ( g_ap );
    
    return 0;
}

int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG

    if (accion->objectName() == "mui_actionContablesPartidas") {
        PartidasView *pag = new PartidasView ( g_pluginbf_minicontabilidad->company(), 0, FALSE );
        g_pluginbf_minicontabilidad->company()->m_pWorkspace->addSubWindow ( pag );
        pag->show();
    } // end if

    if (accion->objectName() == "mui_actionContablesAnotaciones") {
        g_ap->hide();
        g_ap->show();
    } // end if

    if (accion->objectName() == "mui_actionContablesPresupuestos") {
        PresupuestoContableList *pag = new PresupuestoContableList ( g_pluginbf_minicontabilidad->company(), 0 );
        g_pluginbf_minicontabilidad->company()->m_pWorkspace->addSubWindow ( pag );
        pag->show();
    } // end if

    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    BL_FUNC_DEBUG

    if ( comp->hasTablePrivilege ( "partida", "SELECT" ) ) {

    } // end if

    
    return 0;
}


int Busqueda_on_mui_buscar_clicked ( BlSearchWidget *busq )
{
    BL_FUNC_DEBUG

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
    BL_FUNC_DEBUG
    int ret = 0;
    if ( g_fieldName == "codigocompletopartida"  ) {
        BlDbCompleterComboBox * editor = new BlDbCompleterComboBox ( g_editor );
        editor->setObjectName ( "EditCodigoCompletoPartida" );
        editor->setMainCompany ( ( BfCompany * ) bl->m_subform->mainCompany() );
	editor->m_valores["codigocompletopartida"] = "";
        editor->m_valores["nombrepartida"] = "";
        editor->m_tabla = "partida";
        g_plugParams =  editor;
        ret = -1;
    } // end if

    
    return ret;
}




int BlSubFormDelegate_setModelData ( BlSubFormDelegate *bl )
{
    BL_FUNC_DEBUG
    int ret = 0;
    if ( g_editor->objectName() == "EditCodigoCompletoPartida" ) {
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
    if ( g_editor->objectName() == "EditCodigoCompletoPartida"  ) {
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
    if ( sub->m_campoactual->fieldName() == "codigocompletopartida" ) {
	QString query = "SELECT idpartida, nombrepartida, codigocompletopartida FROM partida WHERE upper (codigocompletopartida) LIKE upper('" + sub->m_campoactual->text() + "%')";

        BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idpartida", cur->value( "idpartida" ) );
            sub->m_registrolinea->setDbValue ( "codigocompletopartida", cur->value( "codigocompletopartida" ) );
            sub->m_registrolinea->setDbValue ( "nombrepartida", cur->value( "nombrepartida" ) );
        } // end if
        delete cur;
    } // end if
    
    return 0;
}


/*
int BlDbCompleterComboBox_textChanged (BlDbCompleterComboBox *bl) {
	BL_FUNC_DEBUG

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
                        cad = cad + sep + bl->m_cursorcombo->value( i.key() );
                        if ( sep == "" ) {
                            cad1 = i.key();
                            sep = " ";
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
*/












int BfSubForm_pressedAsterisk ( BfSubForm *sub )
{
    BL_FUNC_DEBUG

    if ( sub->m_campoactual->fieldName() != "codigocompletopartida" ) {
        
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
        sub->m_registrolinea->setDbValue ( "codigocompletopartida", cur->value( "codigocompletopartida" ));
        sub->m_registrolinea->setDbValue ( "nombrepartida", cur->value( "nombrepartida" ));
    } // end if
    
    delete cur;
	  
	  
        } // end if
        delete diag;

    

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
SubForm_MiniContabilidad::SubForm_MiniContabilidad ( BlSubForm *parent ) : QObject ( parent )
{
    BL_FUNC_DEBUG
    
}

///
/**
**/
SubForm_MiniContabilidad::~SubForm_MiniContabilidad()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu
**/
void SubForm_MiniContabilidad::s_pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
    BfSubForm *sub = ( BfSubForm * ) parent();
    BlSubFormHeader *header = sub->header ( "codigocompletopartida" );
    if ( header ) {
        menu->addSeparator();
        menu->addAction ( _ ( "Gestionar partidas" ) );
	menu->addAction ( _ ( "Seleccionar partida" ) );
    } // end if
    
}


///
/**
\param action
**/
void SubForm_MiniContabilidad::s_trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG
    BfSubForm *sub = ( BfSubForm * ) parent();
    if ( action->text() == _ ( "Gestionar partidas" ) ) {
            gestionarPartidas ( sub);
    } else if ( action->text() == _ ( "Seleccionar partida" ) ) {
        seleccionarPartida ( sub );
    } // end if

    
}


///
/**
**/
void SubForm_MiniContabilidad::gestionarPartidas (  BfSubForm *sub )
{
    BL_FUNC_DEBUG
    PartidasView *pag = new PartidasView ( ( BfCompany * ) sub->mainCompany(), 0, FALSE );
    sub->mainCompany() ->m_pWorkspace->addSubWindow ( pag );
    pag->show();
    
}



///
/**
**/
void SubForm_MiniContabilidad::seleccionarPartida ( BfSubForm *sub )
{
    BL_FUNC_DEBUG

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
        sub->lineaact()->setDbValue ( "codigocompletopartida", cur->value( "codigocompletopartida" ) );
        sub->lineaact()->setDbValue ( "nombrepartida", cur->value( "nombrepartida" ) );
    } // end if
    delete cur;
	} // end if
    delete diag;
    
    
}


///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    BL_FUNC_DEBUG
    SubForm_MiniContabilidad *subformods = new SubForm_MiniContabilidad ( sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    
    return 0;
}





