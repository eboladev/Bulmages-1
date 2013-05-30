/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#include "total.h"
#include <QtWidgets/QWidget>

Total::Total ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );
    if (g_confpr->valueTrue(CONF_TPV_REIMPRIMIR)) {
        mui_imprimiralcobrar->hide();
    } // end if
}

Total::~Total()
{}

void Total::on_mui_cobrar_clicked()
{
    ( ( BtCompany * ) mainCompany() )->cobrar(mui_imprimiralcobrar->isChecked());
}

void Total::on_mui_abrircajon_clicked()
{
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_F7 );
}

void Total::on_mui_imprimir_clicked()
{
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_F6 );
}
