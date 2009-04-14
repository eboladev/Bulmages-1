/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C. (Porting to QT4)              *
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

#include <fstream>

#include <QMessageBox>
#include <QWidget>
#include <QComboBox>
#include <QToolButton>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>

#include "bfconfiguracionview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"


/** La ventana se construye como derivada de Ficha, inicializa el subformulario  y mete la ventana en el visor de ventanas
*/
/**
\param comp
\param parent
**/
BfConfiguracionView::BfConfiguracionView ( BfCompany *comp, QWidget *parent ) : BfForm ( comp, parent )
{
    _depura ( "BfConfiguracionView::BfConfiguracionView", 1 );
    setupUi ( this );

    setTitleName ( _ ( "Configuracion" ) );
    setDbTableName ( "configuracion" );
    setDbFieldId ( "nombre" );

    this->setAttribute ( Qt::WA_DeleteOnClose );
    mui_listado->setMainCompany ( mainCompany() );
    mui_listado->cargar();

    meteWindow ( windowTitle(), this, FALSE );
    _depura ( "END BfConfiguracionView::BfConfiguracionView", 1 );
}


/** La destruccion de la clase no requiere de acciones especificas
*/
/**
**/
BfConfiguracionView::~BfConfiguracionView()
{
    _depura ( "BfConfiguracionView::~BfConfiguracionView", 0 );
    _depura ( "END BfConfiguracionView::~BfConfiguracionView", 0 );
}



/// ===================================== SUBFORMULARIO ===============================================
/** Inicializacion del subformulario donde se indican todos los campos que deben aparecer
*/
///
/**
\param parent
**/
BfConfiguracionSubForm::BfConfiguracionSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "BfConfiguracionSubForm::BfConfiguracionSubForm", 0 );
    setDbTableName ( "configuracion" );
    setDbFieldId ( "nombre" );
    addSubFormHeader ( "nombreorig", BlDbField::DbVarChar, BlDbField::DbDupPrimaryKey | BlDbField::DbNoSave, BlSubFormHeader::DbHideView | BlSubFormHeader::DbDisableView, "nombre" );
    addSubFormHeader ( "nombre", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite, _ ( "Nombre" ) );
    addSubFormHeader ( "valor", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Valor" ) );
    setInsert ( FALSE );
    setDelete ( FALSE );
    _depura ( "END BfConfiguracionSubForm::BfConfiguracionSubForm", 0 );
}

