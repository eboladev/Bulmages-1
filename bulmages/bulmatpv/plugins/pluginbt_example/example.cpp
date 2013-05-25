/***************************************************************************
 *   Copyright (C) 2011 by Daniel Ripoll Osma                              *
 *   dripoll.public@gmail.com                                              *
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

/// Dependencias de Qt
#include <QtWidgets/QWidget>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QMainWindow>

/// Dependencias de BulmaGes
#include "example.h"
#include "blfunctions.h"
#include "btcompany.h"
#include "blmainwindow.h"
#include "blplugins.h"




///
/**
Construimos la clase CambiarDebug
\param emp
\param parent
*/
ExampleView::ExampleView ( BlMainCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{

    /// Cargamos la UI, que contiene tiene un QCheckBox (mui_debug)
    setupUi ( this );
    
    /// Establecemos el texto de mui_line_example a "Clicked"
    mui_line_example->setText(_("Desactivado"));

}

int ExampleView::on_mui_button_example_clicked (bool state){
    /// Al dar click a mui_button_example, automáticamente se conecta y establecemos el texto de mui_line_example a "Clicked"
    mui_line_example->setText( _("Activado") );
    return 0;
    }

///
/**
Destruimos la clase ExampleView
\param
**/
ExampleView::~ExampleView()
{
    BlDebug::blDebug ( Q_FUNC_INFO, 0);
    BlDebug::blDebug ( ( "END ", Q_FUNC_INFO ), 0);
}
