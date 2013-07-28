/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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
#include <QtWidgets/QToolButton>

#include "pluginbf_printercocina.h"
#include "blcombobox.h"

///
/**
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_PrinterCocina", "0.12.1-0001");
    
    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_printercocina", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    return 0;
}





/// Al crear la ventana de trabajadores tambien creamos un combo box para el tipo de trabajador.
/**
\param trab
\return
**/

int ArticuloView_ArticuloView_Post ( ArticuloView *trab )
{
    BL_FUNC_DEBUG

    trab->addDbField ( "idprintercocina", BlDbField::DbNumeric, BlDbField::DbNothing, _ ( "Impresion en Cocina" ) );


    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing ( 2 );
    hboxLayout160->setMargin ( 0 );
    hboxLayout160->setObjectName ( QString::fromUtf8 ( "hboxLayout19" ) );

    QLabel *textLabel2_9_26 = new QLabel ( trab->m_frameplugin );
    textLabel2_9_26->setObjectName ( QString::fromUtf8 ( "textLabel2_9_9" ) );
    hboxLayout160->addWidget ( textLabel2_9_26 );
    textLabel2_9_26->setText ( "Impresora de Cocina." );

    /// Datos por defecto.
    BlComboBox *pcocina = new BlComboBox (trab->m_frameplugin);
    pcocina->setObjectName ( QString::fromUtf8 ( "mui_idprintercocina" ));
    pcocina->setMainCompany ( trab->mainCompany() );
    pcocina->setQuery ( "SELECT * FROM printercocina  ORDER BY nombreprintercocina" );
    pcocina->setTableName ( "printercocina" );
    pcocina->setFieldId ( "idprintercocina" );
    pcocina->m_valores["nombreprintercocina"] = "";
    pcocina->m_valores["colaprintercocina"] = "";
    pcocina->setAllowNull ( true );
    pcocina->setId ( "" );
    hboxLayout160->addWidget ( pcocina );

    /// Comprobamos que exista el layout.
    QVBoxLayout *m_hboxLayout1 = trab->m_frameplugin->findChild<QVBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QVBoxLayout ( trab->m_frameplugin );
        m_hboxLayout1->setSpacing ( 0 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    m_hboxLayout1->addLayout ( hboxLayout160 );
    return 0;
}



