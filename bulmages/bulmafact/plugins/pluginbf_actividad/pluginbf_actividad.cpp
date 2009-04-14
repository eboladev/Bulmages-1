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

#include "pluginbf_actividad.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "actividadview.h"
#include "actividadeslist.h"
#include "tipoactividadview.h"
#include "blsearchwidget.h"

ActividadesList *g_actividadesList = NULL;

///
/**
**/
MyPlugProf::MyPlugProf()
{
    _depura ( "MyPlugProf::MyPlugProf", 0 );
    _depura ( "END MyPlugProf::MyPlugProf", 0 );
}


///
/**
**/
MyPlugProf::~MyPlugProf()
{
    _depura ( "MyPlugProf::~MyPlugProf", 0 );
    _depura ( "END MyPlugProf::~MyPlugProf", 0 );
}


///
/**
**/
void MyPlugProf::elslot()
{
    _depura ( "MyPlugProf::elslot", 0 );
    if ( g_actividadesList ) {
        g_actividadesList->hide();
        g_actividadesList->show();
    }// end if
    _depura ( "END MyPlugProf::elslot", 0 );
}

///
/**
**/
void MyPlugProf::elslot1()
{
    _depura ( "MyPlugProf::elslot1", 0 );
    ActividadView * bud = new ActividadView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    _depura ( "END MyPlugProf::elslot1", 0 );
}


///
/**
**/
void MyPlugProf::elslot2()
{
    _depura ( "MyPlugProf::elslot2", 0 );
    TipoActividadView * bud = new TipoActividadView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    _depura ( "END MyPlugProf::elslot2", 0 );
}


///
/**
\param bges
**/
void MyPlugProf::inicializa ( BfBulmaFact *bges )
{
    _depura ( "MyPlugProf::inicializa", 0 );

    if ( bges->getcompany()->hasTablePrivilege ( "cobro", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->newMenu ( "&Docencia", "menuDocencia", "menuMaestro" );
        pPluginMenu->addSeparator();


        QAction *ntipoact = new QAction ( _ ( "&Tipos de Actividad" ), 0 );
        ntipoact->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/actividad.svg" ) ) );
        ntipoact->setStatusTip ( _ ( "Tipos de Actividad" ) );
        ntipoact->setWhatsThis ( _ ( "Tipos de Actividad" ) );
        pPluginMenu->addAction ( ntipoact );
        bges->Fichas->addAction ( ntipoact );
        connect ( ntipoact, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );
        pPluginMenu->addSeparator();

        /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
        m_bges = bges;
        setMainCompany ( bges->getcompany() );
        QAction *planCuentas = new QAction ( _ ( "&Actividades" ), 0 );
        planCuentas->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/actividad.svg" ) ) );
        planCuentas->setStatusTip ( _ ( "Actividades" ) );
        planCuentas->setWhatsThis ( _ ( "Actividades" ) );
        pPluginMenu->addAction ( planCuentas );
        bges->Listados->addAction ( planCuentas );
        connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

        QAction *npago = new QAction ( _ ( "&Nueva actividad" ), 0 );
        npago->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/actividad.svg" ) ) );
        npago->setStatusTip ( _ ( "Nueva actividad" ) );
        npago->setWhatsThis ( _ ( "Nueva actividad" ) );
        pPluginMenu->addAction ( npago );
        bges->Fichas->addAction ( npago );
        connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );
    }// end if
    _depura ( "END MyPlugProf::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    _depura ( "Punto de entrada del plugin de actividades\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_actividad", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    MyPlugProf *plug = new MyPlugProf();
    plug->inicializa ( bges );
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "actividad", "SELECT" ) ) {
        g_actividadesList = new ActividadesList ( comp, NULL );
        comp->m_pWorkspace->addWindow ( g_actividadesList );
        g_actividadesList->hide();
    }// end if
    return 0;
}




int BlSubFormDelegate_createEditor ( BlSubFormDelegate *bl )
{
    _depura ( "pluginbf_actividad::BlSubFormDelegate_createEditor", 0 );
    int ret = 0;
    if ( g_nomcampo == "nombreactividad" ) {
        BlDbCompleterComboBox * editor = new BlDbCompleterComboBox ( g_editor );
        editor->setObjectName ( "EditNombreActividad" );
        editor->setMainCompany ( ( BfCompany * ) bl->m_subform->mainCompany() );
        editor->m_valores["nombreactividad"] = "";
        editor->m_tabla = "actividad";
        g_plugParams =  editor;
        ret = -1;
    } // end if
    _depura ( "END pluginbf_actividad::BlSubFormDelegate_createEditor", 0 );
    return ret;
}

int BlSubFormDelegate_setModelData ( BlSubFormDelegate *bl )
{
    _depura ( "pluginbf_actividad::BlSubFormDelegate_setModelData", 0 );
    int ret = 0;
    if ( g_editor->objectName() == "EditNombreActividad" ) {
        BlDbCompleterComboBox * comboBox = ( BlDbCompleterComboBox * ) g_editor;
        QString value = comboBox->currentText();
        value = value.left ( value.indexOf ( ".-" ) );
        g_model->setData ( g_index, value );
        ret = -1;
    } // end if
    _depura ( "END pluginbf_actividad::BlSubFormDelegate_setModelData", 0 );
    return ret;
}


int BlSubFormDelegate_setEditorData ( BlSubFormDelegate *bl )
{
    _depura ( "pluginbf_actividad::BlSubFormDelegate_setEditorData", 0 );
    int ret = 0;
    if ( g_editor->objectName() == "EditNombreActividad" ) {
        QString value = g_index.model() ->data ( g_index, Qt::DisplayRole ).toString();
        BlDbCompleterComboBox *comboBox = ( BlDbCompleterComboBox * ) g_editor;
        comboBox->addItem ( value );
        ret = -1;
    } // end if
    _depura ( "END pluginbf_actividad::BlSubFormDelegate_setEditorData", 0 );
    return ret;
}

int BlSubForm_editFinished ( BlSubForm *sub )
{
    _depura ( "pluginbf_actividad::BlSubForm_editFinished", 0 );
    if ( sub->m_campoactual->nomcampo() == "nombreactividad" ) {
        BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT idactividad FROM actividad WHERE nombreactividad = '" + sub->m_campoactual->text() + "'" );
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idactividad", cur->valor ( "idactividad" ) );
        } // end if
        delete cur;
    } // end if

    _depura ( "END pluginbf_actividad::BlSubForm_editFinished", 0 );
    return 0;
}

