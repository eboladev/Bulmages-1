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

#include "listconfiguracionview.h"
#include "company.h"
#include "blconfiguration.h"
#include "busquedafecha.h"
#include "blfunctions.h"


/** La ventana se construye como derivada de Ficha, inicializa el subformulario  y mete la ventana en el visor de ventanas
*/
/**
\param comp
\param parent
**/
ListConfiguracionView::ListConfiguracionView ( Company *comp, QWidget *parent ) : FichaBf ( comp, parent )
{
    _depura ( "ListConfiguracionView::ListConfiguracionView", 1 );
    setupUi ( this );

    setTitleName ( _( "Configuracion" ) );
    setDBTableName ( "configuracion" );
    setDBCampoId ( "nombre" );

    this->setAttribute ( Qt::WA_DeleteOnClose );
    mui_listado->setEmpresaBase ( empresaBase() );
    mui_listado->cargar();

    meteWindow ( windowTitle(), this, FALSE );
    _depura ( "END ListConfiguracionView::ListConfiguracionView", 1 );
}


/** La destruccion de la clase no requiere de acciones especificas
*/
/**
**/
ListConfiguracionView::~ListConfiguracionView()
{
    _depura ( "ListConfiguracionView::~ListConfiguracionView", 0 );
    _depura ( "END ListConfiguracionView::~ListConfiguracionView", 0 );
}



/// ===================================== SUBFORMULARIO ===============================================
/** Inicializacion del subformulario donde se indican todos los campos que deben aparecer
*/
///
/**
\param parent
**/
ListConfiguracionSubForm::ListConfiguracionSubForm ( QWidget *parent ) : SubForm2Bf ( parent )
{
    _depura ( "ListConfiguracionSubForm::ListConfiguracionSubForm", 0 );
    setDBTableName ( "configuracion" );
    setDBCampoId ( "nombre" );
    addSHeader ( "nombreorig", BlDbField::DBvarchar, BlDbField::DBDupPrimaryKey | BlDbField::DBNoSave, BlSubFormHeader::DBNoView | BlSubFormHeader::DBBlockView, "nombre" );
    addSHeader ( "nombre", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNoWrite, _( "Nombre" ) );
    addSHeader ( "valor", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Valor" ) );
    setinsercion ( FALSE );
    setDelete ( FALSE );
    _depura ( "END ListConfiguracionSubForm::ListConfiguracionSubForm", 0 );
}

