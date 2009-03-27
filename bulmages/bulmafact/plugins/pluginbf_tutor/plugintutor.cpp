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

#include "plugintutor.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "tutorview.h"
#include "tutoreslist.h"

#include "blsearchwidget.h"

TutoresList *g_tutoresList=NULL;

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
    if (g_tutoresList) {
	g_tutoresList->hide();
	g_tutoresList->show();
    }// end if
    _depura ( "END MyPlugProf::elslot", 0 );
}

///
/**
**/
void MyPlugProf::elslot1()
{
    _depura ( "MyPlugProf::elslot1", 0 );
        TutorView * bud = new TutorView((BfCompany *)mainCompany(), NULL);
        mainCompany() ->m_pWorkspace->addWindow ( bud );
        bud->show();
    _depura ( "END MyPlugProf::elslot1", 0 );
}



///
/**
\param bges
**/
void MyPlugProf::inicializa ( Bulmafact *bges )
{
    _depura ( "MyPlugProf::inicializa", 0 );

    if ( bges->getcompany()->hasTablePrivilege ( "cliente", "SELECT" ) ) {

    /// Miramos si existe un menu Ventas
	QMenu *pPluginMenu = bges->newMenu("&Docencia", "menuDocencia", "menuMaestro");

	/// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
	m_bges = bges;
	setMainCompany ( bges->getcompany() );
	QAction *planCuentas = new QAction ( _( "&Tutores/Socios" ), 0 );
	planCuentas->setIcon(QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/tutor.svg" ) ));
	planCuentas->setStatusTip ( _( "Tutores" ) );
	planCuentas->setWhatsThis ( _( "Tutores" ) );
	pPluginMenu->addAction ( planCuentas );
	bges->Listados->addAction (planCuentas);
	connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

	QAction *npago = new QAction ( _( "&Nuevo tutor/socio" ), 0 );
	npago->setIcon(QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/tutor.svg" ) ));
	npago->setStatusTip ( _( "Nuevo tutor/socio" ) );
	npago->setWhatsThis ( _( "Nuevo tutor/socio" ) );
	pPluginMenu->addAction ( npago );
	bges->Fichas->addAction (npago);
	connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );
    }// end if
    _depura ( "END MyPlugProf::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Punto de entrada del plugin de tutores\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginbf_tutor", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    MyPlugProf *plug = new MyPlugProf();
    plug->inicializa ( bges );
    return 0;
}


int BfCompany_createMainWindows_Post(BfCompany *comp) {
  _depura("pluginbf_tutor::BfCompany_createMainWindows_Post", 0);
    if ( comp->hasTablePrivilege ( "cliente", "SELECT" ) ) {
      g_tutoresList = new TutoresList( comp, NULL );	
      comp->m_pWorkspace->addWindow ( g_tutoresList );
      g_tutoresList->hide();
    }// end if
  _depura("END pluginbf_tutor::BfCompany_createMainWindows_Post", 0);
    return 0;
}


int BlSubFormDelegate_createEditor(BlSubFormDelegate *bl) {
  _depura("pluginbf_tutor::BlSubFormDelegate_createEditor", 0);
  int ret = 0;
    if (g_nomcampo == "nombrealumno") {
        BlDbCompleterComboBox * editor = new BlDbCompleterComboBox ( g_editor );
        editor->setObjectName("EditNombreAlumno");
        editor->setMainCompany ( ( BfCompany * ) bl->m_subform->mainCompany() );
        editor->m_valores["nombrealumno"] = "";
        editor->m_tabla = "alumno";
        g_plugParams =  editor;
        ret = -1;
  } // end if
  
    if (g_nomcampo == "nomcliente") {
        BlDbCompleterComboBox * editor = new BlDbCompleterComboBox ( g_editor );
        editor->setObjectName("EditNombreCliente");
        editor->setMainCompany ( ( BfCompany * ) bl->m_subform->mainCompany() );
        editor->m_valores["nomcliente"] = "";
        editor->m_tabla = "cliente";
        g_plugParams =  editor;
        ret = -1;
  } // end if  
  _depura("END pluginbf_tutor::BlSubFormDelegate_createEditor", 0);

  return ret;
}

int BlSubFormDelegate_setModelData(BlSubFormDelegate *bl) {
  _depura("pluginbf_tutor::BlSubFormDelegate_setModelData", 0);
  int ret = 0;
    if (g_editor->objectName() == "EditNombreAlumno" || g_editor->objectName() == "EditNombreCliente") {
        BlDbCompleterComboBox * comboBox = (BlDbCompleterComboBox *) g_editor;
        QString value = comboBox->currentText();
        value = value.left ( value.indexOf ( ".-" ) );
        g_model->setData ( g_index, value );
        ret = -1;
    } // end if
  _depura("END pluginbf_tutor::BlSubFormDelegate_setModelData", 0);
  return ret;
}


int BlSubFormDelegate_setEditorData(BlSubFormDelegate *bl) {
  _depura("pluginbf_tutor::BlSubFormDelegate_setEditorData", 0);
  int ret = 0;
    if (g_editor->objectName() == "EditNombreAlumno" || g_editor->objectName() == "EditNombreCliente") {
        QString value = g_index.model() ->data ( g_index, Qt::DisplayRole ).toString();
        BlDbCompleterComboBox *comboBox = (BlDbCompleterComboBox *) g_editor ;
        comboBox->addItem ( value );
        ret = -1;
      } // end if
  _depura("END pluginbf_tutor::BlSubFormDelegate_setEditorData", 0);
  return ret;
}

int BlSubForm_editFinished(BlSubForm *sub) {
  _depura("pluginbf_tutor::BlSubForm_editFinished", 0);
    if ( sub->m_campoactual->nomcampo() == "nombrealumno" ) {
        BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT idalumno FROM alumno WHERE nombrealumno = '" + sub->m_campoactual->text() + "'" );
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idalumno", cur->valor ( "idalumno" ) );
        } // end if
        delete cur;
    } // end if
    if ( sub->m_campoactual->nomcampo() == "nomcliente" ) {
        BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT idcliente FROM cliente WHERE nomcliente = '" + sub->m_campoactual->text() + "'" );
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idcliente", cur->valor ( "idcliente" ) );
        } // end if
        delete cur;
    } // end if
    _depura("END pluginbf_tutor::BlSubForm_editFinished", 0);
  return 0;
}
