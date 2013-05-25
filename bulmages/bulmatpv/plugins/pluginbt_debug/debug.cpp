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


#include <QtWidgets/QWidget>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QMainWindow>

#include "debug.h"
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
CambiarDebug::CambiarDebug ( BlMainCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    BL_FUNC_DEBUG
    /// Cargamos la UI, que contiene tiene un QCheckBox (mui_debug)
    setupUi ( this );

    
    /// Comprovamos el estado de CONF_DEBUG en los archivos de configuracion y checkeamos mui_debug, segun estos.
    if ( g_confpr->value( CONF_DEBUG ) == "true" ) {
        mui_debug->setChecked ( true );
        BlDebug::blDebug ( ( Q_FUNC_INFO, _("Debug activado") ), 0 );
    } else {
        mui_debug->setChecked ( false );
        BlDebug::blDebug ( ( Q_FUNC_INFO,  _("Debug desactivado") ), 0 );
    }


}

///
/**
Destruimos la clase CambiarDebug
\param
**/
CambiarDebug::~CambiarDebug()
{
    BlDebug::blDebug ( Q_FUNC_INFO, 0);
    BlDebug::blDebug ( ( "END ",Q_FUNC_INFO ), 0);
}



///
/**
Establecemos el estado del debug en base al state del checkbox
\param state
**/
void CambiarDebug::on_mui_debug_stateChanged ( int state )
{
    BL_FUNC_DEBUG
    /// Establecemos el estado de CONF_DEBUG, sobreescribiendo lo establecido por los archivos y sin guardar el estado
    if (state == Qt::Checked) {
        g_confpr->setValue( CONF_DEBUG, "true");
        BlDebug::blDebug ( ( Q_FUNC_INFO, _("Debug activado") ), 0 );
    } else if (state == Qt::Unchecked) {
        g_confpr->setValue( CONF_DEBUG, "false");
        BlDebug::blDebug ( ( Q_FUNC_INFO,  _("Debug desactivado") ), 0 );
    } // end if

   
}

