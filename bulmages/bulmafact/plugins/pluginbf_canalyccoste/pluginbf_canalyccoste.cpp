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

#include <QtWidgets/QAction>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtCore/QObject>
#include <QtCore/QTextCodec>
#include <QtCore/QLocale>

#include "bldockwidget.h"
#include "bccanalseleccionarview.h"
#include "bccentrocosteseleccionarview.h"
#include "bccentrocosteview.h"
#include "bccanalview.h"
#include "pluginbf_canalyccoste.h"
#include "bfcompany.h"


BfBulmaFact *g_pluginbf_canalyccoste = NULL;


///
/**
\param bcont
**/
int entryPoint ( BfBulmaFact *bcont )
{
    BL_FUNC_DEBUG

    g_selcanales = new BcCanalSeleccionarView ( bcont->company(), 0 );
    g_selccostes = new BcCentroCosteSeleccionarView (bcont->company(), 0);
    
    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_canalyccoste", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );
    g_pluginbf_canalyccoste = bcont;

    QMenu *pPluginMenu = bcont->menuMaestro;
    pPluginMenu->addSeparator();
    
    BlAction *accionA = new BlAction ( _ ( "&Canal" ), 0 );
    accionA->setStatusTip ( _ ( "Ver Canales" ) );
    accionA->setWhatsThis ( _ ( "Ver Canales" ) );
    accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/BulmaCont32x32/images/png/i_canalyccoste.xpm" ) ) );
    accionA->setObjectName("mui_actionCanal");
    pPluginMenu->addAction ( accionA );
    bcont->Listados->addAction ( accionA );

    BlAction *accionB = new BlAction ( _ ( "&Centros de Coste" ), 0 );
    accionB->setStatusTip ( _ ( "Ver Centros de Coste" ) );
    accionB->setWhatsThis ( _ ( "Ver Centros de Coste" ) );
    accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/BulmaCont32x32/images/png/i_canalyccoste.xpm" ) ) );
    accionB->setObjectName("mui_actionCCoste");
    pPluginMenu->addAction ( accionB );
    bcont->Listados->addAction ( accionB );
    
    
    
// =================================================
    /// Vamos a probar con un docwindow.
    BlDockWidget *doc1 = new BlDockWidget ( _ ( "Selector de Canales" ), bcont );
    doc1->setObjectName("mui_selcanales");
    doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );

    doc1->setGeometry ( 100, 100, 100, 500 );
    doc1->resize ( 330, 250 );
    bcont->addDockWidget ( Qt::RightDockWidgetArea, doc1 );

    doc1->show();

    doc1->setWidget ( g_selcanales );

// =================================================	
	
    
/**
 *
 *
 *
 *
 */	
// =================================================
    /// Vamos a probar con un docwindow.
    BlDockWidget *doc2 = new BlDockWidget ( _ ( "Selector de Centros de Coste" ), bcont );
    doc2->setObjectName("mui_selcostes");
    doc2->setFeatures ( QDockWidget::AllDockWidgetFeatures );

    doc2->setGeometry ( 100, 100, 100, 500 );
    doc2->resize ( 330, 250 );
    bcont->addDockWidget ( Qt::RightDockWidgetArea, doc2 );

    doc2->show();

    doc2->setWidget ( g_selccostes );

    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG

    if (accion->objectName() == "mui_actionCanal") {
	BcCanalView *canal = new BcCanalView ( g_pluginbf_canalyccoste->company(), 0 );
	g_pluginbf_canalyccoste->company()->pWorkspace()->addSubWindow ( canal );      
	canal->show();
    } // end if

    if (accion->objectName() == "mui_actionCCoste") {
	BcCentroCosteView *ccoste = new BcCentroCosteView ( g_pluginbf_canalyccoste->company(), 0 );
	g_pluginbf_canalyccoste->company()->pWorkspace()->addSubWindow ( ccoste );      
	ccoste->show();
    } // end if
    
    return 0;
}

int PgetSelCostes(QString &text) {
    BL_FUNC_DEBUG
    text = g_selccostes->cadcoste();
    return 0;
}
    
int PgetSelCanales(QString &text) {
    BL_FUNC_DEBUG
    text = g_selcanales->cadCanal();
    return 0;
}

int PgetSinCanal(bool &text) {
    BL_FUNC_DEBUG
    text = g_selcanales->sinCanal();
    return 0;
}

    
    
/// ==============================================================

int BlSubFormDelegate_createEditor ( BlSubFormDelegate *bl )
{
    BL_FUNC_DEBUG
    int ret = 0;
    if ( g_fieldName == "nomcanal" ) {
        BlComboBox * editor = new BlComboBox ( g_editor );
        editor->setObjectName ( "EditCanal" );
        editor->setMainCompany ( ( BfCompany * ) bl->m_subform->mainCompany() );
        editor->setQuery ( "SELECT * FROM canal ORDER BY nombre" );
	editor->m_valores["nombre"] = "";
        editor->setTableName ("canal");
	editor->setFieldId("idcanal");
	editor->setAllowNull (true);
	editor->setId("");
        g_plugParams =  editor;
        ret = -1;
    } // end if

    if ( g_fieldName == "nomc_coste" ) {
        BlComboBox * editor = new BlComboBox ( g_editor );
        editor->setObjectName ( "EditCCoste" );
        editor->setMainCompany ( ( BfCompany * ) bl->m_subform->mainCompany() );
        editor->setQuery ( "SELECT * FROM c_coste ORDER BY nombre" );
	editor->m_valores["nombre"] = "";
        editor->setTableName ("c_coste");
	editor->setFieldId("idc_coste");
	editor->setAllowNull (true);
	editor->setId("");
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
    if ( g_editor->objectName() == "EditCanal"  || g_editor->objectName() =="EditCCoste") {
        BlComboBox * comboBox = ( BlComboBox * ) g_editor;
        QString value = comboBox->currentText();
        g_model->setData ( g_index, value );
        ret = -1;
    } // end if
    
    return ret;
}


int BlSubFormDelegate_setEditorData ( BlSubFormDelegate *bl )
{
    BL_FUNC_DEBUG
    int ret = 0;
    if ( g_editor->objectName() == "EditCanal" || g_editor->objectName() == "EditCCoste" ) {
        QString value = g_index.model() ->data ( g_index, Qt::DisplayRole ).toString();
        BlComboBox *comboBox = ( BlComboBox * ) g_editor ;
        comboBox->setId ( value );
        ret = -1;
    } // end if
    return ret;
}

int BlSubForm_editFinished ( BlSubForm *sub )
{
    BL_FUNC_DEBUG
    if ( sub->m_campoactual->fieldName() == "nomcanal" ) {
        QString params[1]= {  sub->m_campoactual->text() };
	QString query = "SELECT idcanal FROM canal WHERE nombre = $1";
        BlDbRecordSet *cur = sub->mainCompany() -> loadQuery( query, 1, params );
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idcanal", cur->value( "idcanal" ) );
        } // end if
        delete cur;
    } // end if
    if ( sub->m_campoactual->fieldName() == "nomc_coste" ) {
        QString params[1]= {  sub->m_campoactual->text() };
	QString query = "SELECT idc_coste FROM c_coste WHERE nombre = $1";
        BlDbRecordSet *cur = sub->mainCompany() -> loadQuery( query, 1, params );
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idc_coste", cur->value( "idc_coste" ) );
        } // end if
        delete cur;
    } // end if
    
    return 0;
}
    