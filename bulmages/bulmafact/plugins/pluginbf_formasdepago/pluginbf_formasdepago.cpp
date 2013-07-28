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

#include "pluginbf_formasdepago.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "fpagoview.h"
#include "blcombobox.h"


BfBulmaFact *g_pluginbf_formasdepago = NULL;

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
    blBindTextDomain ( "pluginbf_formasdepago", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );
    g_pluginbf_formasdepago = bges;

    if ( bges->company()->hasTablePrivilege ( "forma_pago", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->menuMaestro;
        pPluginMenu->addSeparator();
        
        BlAction *accionA = new BlAction ( _ ( "&Formas de pago" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/payment-method.png" ) ) );
        accionA->setStatusTip ( _ ( "Formas de pago" ) );
        accionA->setWhatsThis ( _ ( "Formas de pago" ) );
        accionA->setObjectName("mui_actionFormasPago");

        pPluginMenu->addAction ( accionA );
        bges->Listados->addAction ( accionA );

    } // end if

    return 0;
}

int BlAction_actionTriggered(BlAction *accion) {
    if (accion->objectName() == "mui_actionFormasPago") {
        FPagoView * bud = new FPagoView ( g_pluginbf_formasdepago->company(), NULL );
        g_pluginbf_formasdepago->company() ->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if
    return 0;
} 

/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewFPagoView ( BfCompany *v )
{
    FPagoView *h = new FPagoView ( v, 0 );
    g_plugParams = h;
    return 1;
}



int BlSubFormDelegate_createEditor ( BlSubFormDelegate *bl )
{
    BL_FUNC_DEBUG
    int ret = 0;
    if ( g_fieldName == "descforma_pago" ) {
        BlComboBox * editor = new BlComboBox ( g_editor );
        editor->setObjectName ( "EditFormaPago" );
        editor->setMainCompany ( ( BfCompany * ) bl->m_subform->mainCompany() );
        editor->setQuery ( "SELECT * FROM forma_pago ORDER BY descforma_pago" );
	editor->m_valores["descforma_pago"] = "";
        editor->setTableName ("forma_pago");
	editor->setFieldId("idforma_pago");
	editor->setAllowNull (false);
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
    if ( g_editor->objectName() == "EditFormaPago" ) {
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
    if ( g_editor->objectName() == "EditFormaPago" ) {
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
    if ( sub->m_campoactual->fieldName() == "descforma_pago" ) {
        QString params[1]= {  sub->m_campoactual->text() };
	QString query = "SELECT idforma_pago FROM forma_pago WHERE descforma_pago = $1";
        BlDbRecordSet *cur = sub->mainCompany() -> loadQuery( query, 1, params );
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idforma_pago", cur->value( "idforma_pago" ) );
        } // end if
        delete cur;
    } // end if
    
    return 0;
}


















